#pragma once

#include <rendering/util/gui.hpp>
#include <imgui/imgui_internal.h>

#include <core/core.hpp>
#include <rendering/rendering.hpp>
#include <rendering/pipeline/gui/stages/imguirenderstage.hpp>
#include <rendering/systems/renderer.hpp>

#include "../renderstages/mousehover.hpp"

LEGION_CLANG_SUPPRESS_WARNING_WITH_PUSH("-Wdeprecated-declarations")

namespace legion
{
    using namespace legion::rendering;
    using namespace legion::core::filesystem::literals;
    //using namespace legion::core::scenemanagement;

    struct click_action : public app::input_action<click_action> {};

    class GuiTestSystem : public System<GuiTestSystem>
    {
        static bool captured;

        bool clicked = false;

    public:
        static bool isEditingText;

        static void CaptureKeyboard(bool cap = true)
        {
            captured = cap;
        }

        static void ReleaseKeyboard()
        {
            captured = false;
        }

        ecs::filter<camera, transform> cameraQuery;
        ecs::entity selected;

        math::mat4 view = math::mat4(1.0f);
        math::mat4 projection = math::mat4(1.0f);
        math::mat4 model = math::mat4(1.0f);
        imgui::filebrowser::ImGuiFileBrowser browser;

        void onClick(click_action& event)
        {
            if (event.released())
                clicked = true;
        }

        void setup()
        {
            static_cast<DefaultPipeline*>(Renderer::getMainPipeline())->attachStage<ImGuiStage>();

            app::InputSystem::createBinding<click_action>(app::inputmap::method::MOUSE_LEFT);
            bindToEvent<click_action, &GuiTestSystem::onClick>();

            //gui code goes here
            ImGuiStage::addGuiRender<GuiTestSystem, &GuiTestSystem::onGUI>(this);

            app::window& window = ecs::world.get_component<app::window>();

            model_handle cubeModel;
            material_handle decalMaterial;

            {
                application::context_guard guard(window);

                cubeModel = ModelCache::create_model("cube", "assets://models/Cube.obj"_view);
                decalMaterial = MaterialCache::create_material("decal", "assets://shaders/decal.shs"_view);
                decalMaterial.set_param(SV_ALBEDO, TextureCache::create_texture("engine://resources/default/albedo"_view));
                decalMaterial.set_param(SV_NORMALHEIGHT, TextureCache::create_texture("engine://resources/default/normalHeight"_view));
                decalMaterial.set_param(SV_MRDAO, TextureCache::create_texture("engine://resources/default/MRDAo"_view));
                decalMaterial.set_param(SV_EMISSIVE, TextureCache::create_texture("engine://resources/default/emissive"_view));
                decalMaterial.set_param(SV_HEIGHTSCALE, 0.f);
            }

            auto decalEntity = createEntity("Decal");

            position pos(5.f, 0.f, 5.f);
            scale scal(3.f, 2.f, 3.f);
            rotation rot = angleAxis(math::quarter_pi<float>(), math::vec3::up);
            model = compose(scal, rot, pos);

            decalEntity.add_component<transform>(pos, rot, scal);
            decalEntity.add_component(gfx::mesh_renderer(decalMaterial, cubeModel));

        }

        // BuildTree creates a rudimentary Entity View, as entities do currently not have the ability to be named
        // This is going to display IDs, which isn't great but still gives a little insight
        // In how the Scene is currently structured, it will also try to show the names of the components of the
        // entities, which makes identifying them easier
        //
        bool BuildTree(ecs::entity handle)
        {
            static ecs::entity selectedForNameEdit;
            static char buffer[512];
            ImGuiTreeNodeFlags flags = 0;

            if (handle == selected)
                flags |= ImGuiTreeNodeFlags_Selected;

            if (ImGui::TreeNodeEx(reinterpret_cast<void*>(handle->id), flags, "%s", handle->name.c_str()))
            {
                if (handle->name.empty())
                {
                    if (handle == ecs::world)
                        handle->name = "World";
                    else if (selected.has_component<camera>())
                        handle->name = "camera";
                    else
                        handle->name = "Entity " + std::to_string(handle->id);
                }

                if (!handle.has_component<camera>())
                {
                    if (selectedForNameEdit == handle)
                    {
                        if (ImGui::InputText(" ###", buffer, 512, ImGuiInputTextFlags_EnterReturnsTrue))
                        {
                            handle->name = buffer;
                            selectedForNameEdit = ecs::entity();
                        }
                    }
                    else
                    {
                        ImGui::Text("");
                    }

                    ImGui::SameLine();
                    if (ImGui::Button("Change Name"))
                    {
                        if (selectedForNameEdit == handle)
                        {
                            handle->name = buffer;
                            selectedForNameEdit = ecs::entity();
                        }
                        else
                        {
                            selectedForNameEdit = handle;
                            strcpy(buffer, handle->name.data());
                        }
                    }
                }
                else
                    ImGui::Text("");

                if (handle != ecs::world && !handle.has_component<camera>()/* && !handle.has_component<scene>()*/)
                {
                    ImGui::SameLine();
                    if (ImGui::Button("Destroy"))
                    {
                        handle.destroy();
                        ImGui::TreePop();
                        return false;
                    }
                }

                if (selected != handle)
                {
                    ImGui::SameLine();
                    if (ImGui::Button("Select"))
                    {
                        selected = handle;
                    }
                }

                ImGui::Separator();

                if (ImGui::TreeNode("Children"))
                {
                    ImGui::SameLine();
                    if (ImGui::Button("Add child"))
                    {
                        (void)createEntity(handle);
                    }

                    std::vector<ecs::entity> toErase;
                    for (auto child : handle)
                    {
                        if (!BuildTree(child))
                            toErase.push_back(child);
                    }

                    if (toErase.size())
                    {
                        for (auto child : toErase)
                            handle.remove_child(child);
                    }

                    ImGui::TreePop();
                }


                if (ImGui::TreeNode("Components"))
                {
                    for (id_type id : handle.component_composition())
                    {
                        ImGui::Text("%s", ecs::Registry::getFamilyName(id).c_str());
                    }

                    ImGui::TreePop();
                }

                ImGui::TreePop();
            }
            return true;
        }

        void DisplayFileHandling()
        {
            bool loadTexture = false;
            bool loadModel = false;
            bool loadMaterial = false;
            bool loadShader = false;

            static char buffer[512];
            static shader_handle shader;
            static bool createdNewShader = false;
            static material_handle material;

            if (ImGui::BeginMainMenuBar())
            {
                if (ImGui::BeginMenu("File"))
                {
                    if (ImGui::MenuItem("New Texture"))
                        loadTexture = true;

                    if (ImGui::MenuItem("New Model"))
                        loadModel = true;
                    
                    if (ImGui::MenuItem("New Material"))
                    {
                        memset(buffer, '\0', 512);
                        shader = invalid_shader_handle;
                        material = invalid_material_handle;
                        createdNewShader = false;
                        loadMaterial = true;
                    }

                    //if (ImGui::BeginMenu("Save Scene"))
                    //{
                    //    if (!SceneManager::currentScene)
                    //    {
                    //        SceneManager::currentScene = SceneManager::create_scene();
                    //    }
                    //    auto sceneEntity = SceneManager::currentScene.entity;
                    //    std::string sceneName = sceneEntity.get_name();

                    //    std::string text = "Save scene as:";
                    //    text += sceneName;
                    //    if (ImGui::Button(text.c_str()))
                    //    {
                    //        SceneManager::create_scene(sceneName, sceneEntity);
                    //    }
                    //    ImGui::EndMenu();
                    //}

                    //if (ImGui::BeginMenu("Load Scene"))
                    //{
                    //    for (auto& [id, name] : SceneManager::sceneNames)
                    //    {
                    //        if (id && ImGui::MenuItem(name.c_str()))
                    //        {
                    //            SceneManager::load_scene(name);
                    //        }
                    //    }
                    //    ImGui::EndMenu();
                    //}
                    ImGui::EndMenu();
                }
                ImGui::EndMainMenuBar();
            }

            if (loadTexture)
                ImGui::OpenPopup("Load Texture");

            if (loadModel)
                ImGui::OpenPopup("Load Model");

            if (loadMaterial)
            {
                ImGui::OpenPopup("New Material");
                loadMaterial = false;
            }

            if (ImGui::BeginPopupModal("New Material"))
            {
                ImGui::Text("Material name:");
                ImGui::SameLine();
                ImGui::InputText("##material", buffer, 512, ImGuiInputTextFlags_EnterReturnsTrue);

                ImGui::Text("Shader:");

                std::string shaderName;

                if (shader != invalid_shader_handle)
                    shaderName = shader.get_name();

                ImGui::SameLine();
                ImGui::InputText("##shader", shaderName.data(), shaderName.size(), ImGuiInputTextFlags_ReadOnly);
                ImGui::SameLine();
                if (ImGui::Button("Load Shader"))
                {
                    loadShader = true;
                }

                if (shader != invalid_shader_handle && material == invalid_material_handle)
                {
                    material = MaterialCache::create_material("_internal_tmp_material_", shader);
                }

                if (material != invalid_material_handle)
                {
                    ImGui::Indent();

                    auto variants = material.get_variants();

                    int currentVariantIdx;

                    auto currentVariant = material.current_variant();
                    if (currentVariant == 0)
                        currentVariant = nameHash("default");

                    std::vector<cstring> variantNames;
                    variantNames.reserve(variants.size());
                    for (size_type i = 0; i < variants.size(); i++)
                    {
                        variantNames.push_back(variants[i].get().name.c_str());
                        if (nameHash(variants[i].get().name) == currentVariant)
                            currentVariantIdx = i;
                    }

                    ImGui::Text("Variant:");
                    ImGui::SameLine();
                    if (ImGui::Combo("##variant", &currentVariantIdx, variantNames.data(), variantNames.size()))
                    {
                        material.set_variant(variantNames[currentVariantIdx]);
                    }

                    for (auto& [id, paramPtr] : variants[currentVariantIdx].get().parameters)
                    {
                        DisplayParamEditor(material, paramPtr->get_name(), paramPtr->type());
                    }

                    ImGui::Unindent();
                }

                if (ImGui::Button("Confirm"))
                {
                    auto newMat = MaterialCache::create_material(buffer, shader);

                    auto variants = material.get_variants();
                    auto newVariants = newMat.get_variants();
                    for (size_type i = 0; i < variants.size(); i++)
                    {
                        auto& variant = variants[i].get();
                        auto& newVariant = newVariants[i].get();

                        for (auto& [id, ptr] : variant.parameters)
                        {
                            newVariant.parameters[id].swap(ptr);
                        }
                    }

                    material.destroy();

                    memset(buffer, '\0', 512);
                    shader = invalid_shader_handle;
                    material = invalid_material_handle;
                    createdNewShader = false;
                    ImGui::CloseCurrentPopup();
                }
                ImGui::SameLine();
                if (ImGui::Button("Cancel"))
                {
                    material.destroy();

                    if (createdNewShader)
                        shader.destroy();

                    memset(buffer, '\0', 512);
                    shader = invalid_shader_handle;
                    material = invalid_material_handle;
                    createdNewShader = false;
                    ImGui::CloseCurrentPopup();
                }

                ImGui::EndPopup();
            }

            if(loadShader)
                ImGui::OpenPopup("Load Shader");

            if (browser.showFileDialog("Load Texture", imgui::filebrowser::ImGuiFileBrowser::DialogMode::OPEN))
            {
                TextureCache::create_texture(browser.selected_fn, fs::view(browser.selected_path));
            }

            if (browser.showFileDialog("Load Model", imgui::filebrowser::ImGuiFileBrowser::DialogMode::OPEN))
            {
                ModelCache::create_model(browser.selected_fn, fs::view(browser.selected_path));
            }

            if (browser.showFileDialog("Load Shader", imgui::filebrowser::ImGuiFileBrowser::DialogMode::OPEN))
            {
                if (ShaderCache::has_shader(browser.selected_fn))
                {
                    shader = ShaderCache::get_handle(browser.selected_fn);
                }
                else
                {
                    createdNewShader = true;
                    shader = ShaderCache::create_shader(browser.selected_fn, fs::view(browser.selected_path));
                }
                loadMaterial = true;
            }

            if (loadMaterial)
                ImGui::OpenPopup("New Material");
        }

        template<typename Vec>
        bool DisplayVec(cstring name, Vec& value)
        {
            if constexpr (std::is_same_v<typename Vec::value_type, float>)
            {
                return ImGui::InputScalarN(name, ImGuiDataType_Float, math::value_ptr(value), Vec::length());
            }
            else if constexpr (std::is_same_v<typename Vec::value_type, int>)
            {
                return ImGui::InputScalarN(name, ImGuiDataType_S32, math::value_ptr(value), Vec::length());
            }
            else if constexpr (std::is_same_v<typename Vec::value_type, bool>)
            {
                ImGuiWindow* window = ImGui::GetCurrentWindow();
                if (window->SkipItems)
                    return false;

                ImGuiContext& g = *ImGui::GetCurrentContext();
                bool value_changed = false;
                ImGui::BeginGroup();
                ImGui::PushID(name);
                ImGui::PushMultiItemsWidths(Vec::length(), ImGui::CalcItemWidth());

                for (int i = 0; i < Vec::length(); i++)
                {
                    ImGui::PushID(i);
                    if (i > 0)
                        ImGui::SameLine(0, g.Style.ItemInnerSpacing.x);

                    std::string label = std::string("##") + name + std::to_string(i);

                    value_changed |= ImGui::Checkbox(label.c_str(), &value[i]);

                    ImGui::PopID();
                    ImGui::PopItemWidth();
                }
                ImGui::PopID();

                const char* label_end = ImGui::FindRenderedTextEnd(name);
                if (name != label_end)
                {
                    ImGui::SameLine(0.0f, g.Style.ItemInnerSpacing.x);
                    ImGui::TextEx(name, label_end);
                }

                ImGui::EndGroup();
                return value_changed;
            }
        }

        template<typename T>
        bool DisplayValue(cstring name, T& value)
        {
            if constexpr (std::is_same_v<T, float>)
            {
                return ImGui::InputFloat(name, &value);
            }
            else if constexpr (std::is_same_v<T, bool>)
            {
                return ImGui::Checkbox(name, &value);
            }
            else if constexpr (std::is_same_v<T, int>)
            {
                return ImGui::InputInt(name, &value);
            }
            else
            {
                return DisplayVec<T>(name, value);
            }
        }

        bool DisplayParamEditor(material_handle material, const std::string& name, id_type type)
        {
            if (common::starts_with(name, "lgn_"))
                return false;

            ImGui::Text("%s:", name.c_str());
            ImGui::SameLine();

            std::string label = "##" + name;

            if (type == typeHash<texture_handle>())
            {
                int currentTextureIdx;
                auto value = material.get_param<texture_handle>(name);

                auto textures = TextureCache::get_all();

                std::vector<cstring> textureNames;
                textureNames.reserve(textures.size());
                for (size_type i = 0; i < textures.size(); i++)
                {
                    textureNames.push_back(textures[i].get_texture().name.c_str());
                    if (textures[i] == value)
                        currentTextureIdx = i;
                }

                if (ImGui::Combo(label.c_str(), &currentTextureIdx, textureNames.data(), textureNames.size()))
                {
                    material.set_param<texture_handle>(name, textures[currentTextureIdx]);
                    return true;
                }
            }
            else if(type == typeHash<float>())
            {
                float value = material.get_param<float>(name);
                if (DisplayValue(label.c_str(), value))
                {
                    material.set_param(name, value);
                    return true;
                }
            }
            else if (type == typeHash<math::vec2>())
            {
                math::vec2 value = material.get_param<math::vec2>(name);
                if (DisplayValue(label.c_str(), value))
                {
                    material.set_param(name, value);
                    return true;
                }
            }
            else if (type == typeHash<math::vec3>())
            {
                math::vec3 value = material.get_param<math::vec3>(name);
                if (DisplayValue(label.c_str(), value))
                {
                    material.set_param(name, value);
                    return true;
                }
            }
            else if (type == typeHash<math::vec4>())
            {
                math::vec4 value = material.get_param<math::vec4>(name);
                if (DisplayValue(label.c_str(), value))
                {
                    material.set_param(name, value);
                    return true;
                }
            }
            else if (type == typeHash<uint>())
            {
                int value = material.get_param<uint>(name);
                if (DisplayValue(label.c_str(), value))
                {
                    material.set_param<uint>(name, value);
                    return true;
                }
            }
            else if (type == typeHash<int>())
            {
                int value = material.get_param<int>(name);
                if (DisplayValue(label.c_str(), value))
                {
                    material.set_param(name, value);
                    return true;
                }
            }
            else if (type == typeHash<math::ivec2>())
            {
                math::ivec2 value = material.get_param<math::ivec2>(name);
                if (DisplayValue(label.c_str(), value))
                {
                    material.set_param(name, value);
                    return true;
                }
            }
            else if (type == typeHash<math::ivec3>())
            {
                math::ivec3 value = material.get_param<math::ivec3>(name);
                if (DisplayValue(label.c_str(), value))
                {
                    material.set_param(name, value);
                    return true;
                }
            }
            else if (type == typeHash<math::ivec4>())
            {
                math::ivec4 value = material.get_param<math::ivec4>(name);
                if (DisplayValue(label.c_str(), value))
                {
                    material.set_param(name, value);
                    return true;
                }
            }
            else if (type == typeHash<bool>())
            {
                bool value = material.get_param<bool>(name);
                if (DisplayValue(label.c_str(), value))
                {
                    material.set_param(name, value);
                    return true;
                }
            }
            else if (type == typeHash<math::bvec2>())
            {
                math::bvec2 value = material.get_param<math::bvec2>(name);
                if (DisplayValue(label.c_str(), value))
                {
                    material.set_param(name, value);
                    return true;
                }
            }
            else if (type == typeHash<math::bvec3>())
            {
                math::bvec3 value = material.get_param<math::bvec3>(name);
                if (DisplayValue(label.c_str(), value))
                {
                    material.set_param(name, value);
                    return true;
                }
            }
            else if (type == typeHash<math::bvec4>())
            {
                math::bvec4 value = material.get_param<math::bvec4>(name);
                if (DisplayValue(label.c_str(), value))
                {
                    material.set_param(name, value);
                    return true;
                }
            }
            else if (type == typeHash<math::mat2>())
            {
                math::mat2 value = material.get_param<math::mat2>(name);
                bool changed = false;
                std::string label0 = label + '0';
                changed |= DisplayValue(label0.c_str(), value[0]);
                std::string label1 = label + '1';
                changed |= DisplayValue(label1.c_str(), value[1]);
                if (changed)
                {
                    material.set_param(name, value);
                    return true;
                }
            }
            else if (type == typeHash<math::mat3>())
            {
                math::mat3 value = material.get_param<math::mat3>(name);
                bool changed = false;
                std::string label0 = label + '0';
                changed |= DisplayValue(label0.c_str(), value[0]);
                std::string label1 = label + '1';
                changed |= DisplayValue(label1.c_str(), value[1]);
                std::string label2 = label + '2';
                changed |= DisplayValue(label2.c_str(), value[2]);
                if (changed)
                {
                    material.set_param(name, value);
                    return true;
                }
            }
            else if (type == typeHash<math::mat4>())
            {
                math::mat4 value = material.get_param<math::mat4>(name);
                bool changed = false;
                std::string label0 = label + '0';
                changed |= DisplayValue(label0.c_str(), value[0]);
                std::string label1 = label + '1';
                changed |= DisplayValue(label1.c_str(), value[1]);
                std::string label2 = label + '2';
                changed |= DisplayValue(label2.c_str(), value[2]);
                std::string label3 = label + '3';
                changed |= DisplayValue(label3.c_str(), value[3]);
                if (changed)
                {
                    material.set_param(name, value);
                    return true;
                }
            }
            else
            {
                ImGui::Text("Unknown parameter type");
            }

            return false;
        }

        void onGUI(L_MAYBEUNUSED app::window& context, camera& cam, const camera::camera_input& camInput, L_MAYBEUNUSED time::span deltaTime)
        {
            //if (!SceneManager::currentScene)
              //  SceneManager::currentScene = SceneManager::create_scene();

            ImGuiIO& io = ImGui::GetIO();

            if (clicked && !io.MouseDownOwned[0])
            {
                auto selectedId = MouseHover::getHoveredEntityId();
                if (selectedId != invalid_id)
                    selected = ecs::Registry::getEntity(selectedId);
                else
                    selected = ecs::entity();
            }

            clicked = false;

            if (captured)
            {
                io.WantCaptureKeyboard = true;
                isEditingText = false;
            }
            else
                isEditingText = io.WantCaptureKeyboard;

            setProjectionAndView(io.DisplaySize.x / io.DisplaySize.y, cam, camInput);

            using namespace imgui;
            gizmo::SetOrthographic(false);

            DisplayFileHandling();

            std::string windowName = "Entities";

            if (selected)
            {
                windowName += "  (selected:";

                if (selected->name.empty())
                {
                    if (selected == ecs::world)
                        selected->name = "World";
                    else if (selected.has_component<camera>())
                        selected->name = "camera";
                    else
                        selected->name = "Entity " + std::to_string(selected->id);
                }

                windowName += selected->name;

                windowName += ")";
            }

            windowName += "###";

            if (base::Begin(windowName.c_str()))
            {
                BuildTree(ecs::world);
            }
            base::End();

            static bool showGizmo = true;

            gizmo::BeginFrame();

            base::Begin("Edit Entity");
            if (selected)
            {
                if (selected.has_component<camera>())
                {
                    ImGui::Text("Changing camera transform not allowed.");
                }
                else if (selected.has_component<position>() && selected.has_component<rotation>() && selected.has_component<scale>())
                {
                    if (base::RadioButton("Show Gizmo", showGizmo))
                        showGizmo = !showGizmo;

                    if (showGizmo)
                    {
                        position& pos = selected.get_component<position>();
                        rotation& rot = selected.get_component<rotation>();
                        scale& scal = selected.get_component<scale>();
                        model = compose(scal, rot, pos);
                        gizmo::EditTransform(value_ptr(view), value_ptr(projection), value_ptr(model), true);
                        decompose(model, scal, rot, pos);
                    }
                }
                else if (ImGui::Button("Add Transform"))
                {
                    selected.add_component<transform>();
                }

                if (selected.has_component<mesh_renderable>())
                {
                    mesh_renderer& renderer = selected.get_component<mesh_renderer>();
                    mesh_filter& meshFilter = selected.get_component<mesh_filter>();

                    int currentModelIdx;

                    auto models = assets::AssetCache<mesh>::getAll();

                    std::vector<cstring> modelNames;
                    modelNames.reserve(models.size());
                    for (size_type i = 0; i < models.size(); i++)
                    {
                        modelNames.push_back(models[i].name().c_str());
                        if (models[i] == meshFilter.shared_mesh)
                            currentModelIdx = i;
                    }

                    ImGui::Text("Model:");

                    ImGui::SameLine();
                    if (ImGui::Combo("##model", &currentModelIdx, modelNames.data(), modelNames.size()))
                    {
                        meshFilter.shared_mesh = models[currentModelIdx];
                    }

                    int currentMaterialIdx;

                    auto [lock, rawMaterials] = MaterialCache::get_all_materials();
                    std::vector<material_handle> materials;

                    {
                        async::readonly_guard guard(lock);
                        materials.reserve(rawMaterials.size());
                        for (auto& [id, mat] : rawMaterials)
                            materials.push_back(material_handle{ id });
                    }

                    std::vector<cstring> materialNames;

                    materialNames.reserve(materials.size());
                    for (size_type i = 0; i < materials.size(); i++)
                    {
                        materialNames.push_back(materials[i].get_name().c_str());
                        if (materials[i] == renderer.material)
                            currentMaterialIdx = i;
                    }

                    ImGui::Text("Material:");
                    ImGui::SameLine();
                    if (ImGui::Combo("##material", &currentMaterialIdx, materialNames.data(), materialNames.size()))
                    {
                        renderer.material = materials[currentMaterialIdx];
                    }

                    ImGui::Indent();

                    auto variants = renderer.material.get_variants();

                    int currentVariantIdx;

                    auto currentVariant = renderer.material.current_variant();
                    if (currentVariant == 0)
                        currentVariant = nameHash("default");

                    std::vector<cstring> variantNames;
                    variantNames.reserve(variants.size());
                    for (size_type i = 0; i < variants.size(); i++)
                    {
                        variantNames.push_back(variants[i].get().name.c_str());
                        if (nameHash(variants[i].get().name) == currentVariant)
                            currentVariantIdx = i;
                    }

                    ImGui::Text("Variant:");
                    ImGui::SameLine();
                    if (ImGui::Combo("##variant", &currentVariantIdx, variantNames.data(), variantNames.size()))
                    {
                        renderer.material.set_variant(variantNames[currentVariantIdx]);
                    }

                    for (auto& [id, paramPtr] : variants[currentVariantIdx].get().parameters)
                    {
                        DisplayParamEditor(renderer.material, paramPtr->get_name(), paramPtr->type());
                    }

                    ImGui::Unindent();
                }
                else if (ImGui::Button("Add Renderable"))
                {
                    selected.add_component<gfx::mesh_renderable>();
                }
            }
            else
            {
                ImGui::Text("Select an entity to edit its transform.");
            }
            base::End();
        }

        void setProjectionAndView(float aspect, const camera& cam, const camera::camera_input& camInput)
        {
            view = camInput.view;
            projection = cam.get_projection(aspect);
        }
    };

}

LEGION_CLANG_SUPPRESS_WARNING_POP
