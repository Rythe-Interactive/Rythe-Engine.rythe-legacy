#pragma once
#include "../engine_include.hpp"

#include <rendering/util/gui.hpp>
#include <imgui/imgui_internal.h>
#include <rendering/pipeline/gui/stages/imguirenderstage.hpp>

#include "../renderstages/mousehover.hpp"

#include "../debug_utils.hpp"

LEGION_CLANG_SUPPRESS_WARNING_WITH_PUSH("-Wdeprecated-declarations")

namespace legion
{
    using namespace legion::rendering;
    using namespace legion::core::filesystem::literals;
    //using namespace legion::core::scenemanagement;

    struct click_action : public app::input_action<click_action> {};

    class CustomEditor
    {
    public:
        inline static math::mat4 view = math::mat4(1.f);
        inline static math::mat4 projection = math::mat4(1.f);

        virtual void drawEditor(ecs::entity target) LEGION_PURE;
        virtual id_type componentId() LEGION_PURE;
    };
    class GuiTestSystem : public System<GuiTestSystem>
    {
        static bool captured;

        bool clicked = false;

        inline static std::vector<std::unique_ptr<CustomEditor>> editors;

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

        template<typename T, std::enable_if_t<std::is_base_of_v<CustomEditor, T>, bool> = true, typename... Args>
        static void addCustomEditor(Args&&... args)
        {
            editors.push_back(std::make_unique<T>(std::forward<Args>(args)...));
        }

        ecs::filter<camera, transform> cameraQuery;
        static ecs::entity selected;

        math::mat4 view = math::mat4(1.f);
        math::mat4 projection = math::mat4(1.f);
        math::mat4 model = math::mat4(1.f);
        imgui::filebrowser::ImGuiFileBrowser browser;

        void onClick(click_action& event)
        {
            if (event.pressed())
                clicked = true;
        }

        void setup()
        {
            static_cast<DefaultPipeline*>(Renderer::getMainPipeline())->attachStage<ImGuiStage>();

            app::InputSystem::createBinding<click_action>(app::inputmap::method::MOUSE_LEFT);
            bindToEvent<click_action, &GuiTestSystem::onClick>();

            //gui code goes here
            ImGuiStage::addGuiRender<GuiTestSystem, &GuiTestSystem::onGUI>(this);
        }

        // BuildTree creates a rudimentary Entity View, as entities do currently not have the ability to be named
        // This is going to display IDs, which isn't great but still gives a little insight
        // In how the Scene is currently structured, it will also try to show the names of the components of the
        // entities, which makes identifying them easier
        //
        void BuildTree(ecs::entity handle)
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
                        return;
                    }
                }

                if (!selected.valid() || selected != handle)
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
                        L_MAYBEUNUSED auto ent = createEntity(handle);

                    for (auto child : handle)
                        BuildTree(child);

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
        }

        void DisplayFileHandling()
        {
            bool loadTexture = false;
            bool loadModel = false;
            bool loadMaterial = false;
            bool saveEntity = false;
            bool loadEntity = false;

            static char buffer[512];
            static shader_handle shader;
            static bool createdNewShader = false;
            static material_handle material;
            static bool childToSelected = false;

            if (ImGui::BeginMainMenuBar())
            {
                if (ImGui::BeginMenu("File"))
                {
                    loadTexture = ImGui::MenuItem("New texture");

                    loadModel = ImGui::MenuItem("New model");

                    if (ImGui::MenuItem("New material"))
                    {
                        memset(buffer, '\0', 512);
                        shader = invalid_shader_handle;
                        material = invalid_material_handle;
                        createdNewShader = false;
                        loadMaterial = true;
                    }

                    saveEntity = ImGui::MenuItem("Save selected entity");

                    if (ImGui::BeginMenu("Load entity"))
                    {
                        if (ImGui::MenuItem("Child to world"))
                        {
                            loadEntity = true;
                            childToSelected = false;
                        }

                        if (ImGui::MenuItem("Child to selected"))
                        {
                            loadEntity = true;
                            childToSelected = true;
                        }
                        ImGui::EndMenu();
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
                ImGui::OpenPopup("Load texture");

            if (loadModel)
                ImGui::OpenPopup("Load model");

            if (loadMaterial)
                ImGui::OpenPopup("New material");

            if (saveEntity)
                ImGui::OpenPopup("Save entity");

            if (loadEntity)
                ImGui::OpenPopup("Load entity");

            if (ImGui::BeginPopupModal("New material"))
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

                if (ImGui::Button("Load shader"))
                {
                    ImGui::OpenPopup("Load shader");
                }

                if (browser.showFileDialog("Load shader", imgui::filebrowser::ImGuiFileBrowser::DialogMode::OPEN))
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

            if (browser.showFileDialog("Load texture", imgui::filebrowser::ImGuiFileBrowser::DialogMode::OPEN))
            {
                TextureCache::create_texture(browser.selected_fn, fs::view(browser.selected_path));
            }

            if (browser.showFileDialog("Load model", imgui::filebrowser::ImGuiFileBrowser::DialogMode::OPEN))
            {
                ModelCache::create_model(browser.selected_fn, fs::view(browser.selected_path));
            }

            if (browser.showFileDialog("Save entity", imgui::filebrowser::ImGuiFileBrowser::DialogMode::SAVE, ImVec2(0, 0), ".yaml,.json,.bson"))
            {
                if (browser.ext == ".yaml")
                    srl::write<srl::yaml>(fs::view(browser.selected_path), selected, "scene");
                else if (browser.ext == ".json")
                    srl::write<srl::json>(fs::view(browser.selected_path), selected, "scene");
                else if (browser.ext == ".bson")
                    srl::write<srl::bson>(fs::view(browser.selected_path), selected, "scene");
            }

            if (browser.showFileDialog("Load entity", imgui::filebrowser::ImGuiFileBrowser::DialogMode::OPEN, ImVec2(0, 0), ".yaml,.json,.bson"))
            {
                ecs::entity loadedEntity;
                if (browser.ext == ".yaml")
                    loadedEntity = *srl::load<srl::yaml, ecs::entity>(fs::view(browser.selected_path), "scene");
                else if (browser.ext == ".json")
                    loadedEntity = *srl::load<srl::json, ecs::entity>(fs::view(browser.selected_path), "scene");
                else if (browser.ext == ".bson")
                    loadedEntity = *srl::load<srl::bson, ecs::entity>(fs::view(browser.selected_path), "scene");

                if (childToSelected)
                {
                    selected.add_child(loadedEntity);
                }
            }
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
                    ImGui::SameLine(0.f, g.Style.ItemInnerSpacing.x);
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
            else if (type == typeHash<float>())
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

        void DisplayMeshFilterEditor(ecs::entity target)
        {
            mesh_filter& meshFilter = target.get_component<mesh_filter>();

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

            ImGui::LabelText("##meshfilter", "Mesh filter");

            ImGui::Indent();

            ImGui::Text("Model:");

            ImGui::SameLine();
            if (ImGui::Combo("##model", &currentModelIdx, modelNames.data(), modelNames.size()))
            {
                meshFilter.shared_mesh = models[currentModelIdx];
            }
            ImGui::Unindent();
        }

        void DisplayMeshRendererEditor(ecs::entity target)
        {
            mesh_renderer& renderer = target.get_component<mesh_renderer>();
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

            ImGui::LabelText("##meshrenderer", "Mesh renderer");

            ImGui::Indent();

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
            ImGui::Unindent();
        }

        void DisplayPrimitive(std::string_view name, primitive_reference& primitive)
        {
            if (primitive.typeId == typeHash<int>())
            {
                ImGui::InputInt(name.data(), primitive.cast<int>());
            }
            else if (primitive.typeId == typeHash<float>())
            {
                ImGui::InputFloat(name.data(), primitive.cast<float>());
            }
            else if (primitive.typeId == typeHash<bool>())
            {
                ImGui::Checkbox(name.data(), primitive.cast<bool>());
            }
            else if (primitive.typeId == typeHash<std::string>())
            {
                std::array<char, 512> buffer;
                buffer.fill('\0');
                if (ImGui::InputText(name.data(), buffer.data(), buffer.size()))
                {
                    primitive.cast<std::string>()->assign(buffer.data());
                }
            }
            else
            {
                std::string labelId = "##" + std::string(name);
                ImGui::LabelText(labelId.c_str(), "Unsupported primitive type");
            }
        }

        void DisplayGenericObjectEditor(std::string_view name, reflector object, size_type level = 0)
        {
            std::vector<char> labelPrefix;

            labelPrefix.insert(labelPrefix.begin(), level, '#');
            labelPrefix.push_back('#');
            labelPrefix.push_back('#');

            std::string labelId = labelPrefix.data() + std::string(name);
            ImGui::LabelText(labelId.c_str(), name.data());
            ImGui::Indent();

            for (auto& [memberName, member] : object.members)
            {
                ImGui::PushID(memberName.data());
                if (member.is_object)
                {
                    DisplayGenericObjectEditor(memberName, member.object, level + 1);
                }
                else
                {
                    DisplayPrimitive(memberName, member.primitive);
                }
                ImGui::PopID();
            }

            ImGui::Unindent();
        }

        bool DisplayGenericComponentEditor(ecs::entity target, id_type componentId)
        {
            static id_type prehandledComponentIds[] = {
                    typeHash<position>(), typeHash<rotation>(), typeHash<scale>(), typeHash<mesh_filter>(), typeHash<mesh_renderer>()
            };

            for (auto prehandled : prehandledComponentIds)
                if (prehandled == componentId)
                    return false;


            ImGui::Separator();

            auto refl = ecs::Registry::getComponentReflector(componentId, target);

            ImGui::PushID(componentId);

            std::string labelId = "##" + std::string(refl.typeName);
            ImGui::LabelText(labelId.c_str(), refl.typeName.data());
            ImGui::SameLine();
            if (ImGui::Button("Remove"))
            {
                ecs::Registry::destroyComponent(componentId, target);
                ImGui::PopID();
                return true;
            }

            ImGui::Indent();

            for (auto& editor : editors)
                if (editor->componentId() == componentId)
                {
                    editor->drawEditor(target);
                    ImGui::PopID();
                    return false;
                }

            for (auto& [name, member] : refl.members)
            {
                ImGui::PushID(name.data());
                if (member.is_object)
                {
                    DisplayGenericObjectEditor(name, member.object);
                }
                else
                {
                    DisplayPrimitive(name, member.primitive);
                }
                ImGui::PopID();
            }

            ImGui::Unindent();
            ImGui::PopID();

            return false;
        }

        void DisplayEntityEditor(ecs::entity target)
        {
            using namespace imgui;
            static bool showGizmo = true;

            CustomEditor::view = view;
            CustomEditor::projection = projection;

            std::string editorLabel = "Entity: ";
            if (target->name.empty())
                editorLabel += std::to_string(target->id);
            else
                editorLabel += target->name;

            ImGui::LabelText("##entity", editorLabel.c_str());
            ImGui::SameLine();
            if (ImGui::RadioButton("Active", target->active))
                target->active = !target->active;

            ImGui::Separator();

            if (target.has_component<position>() && target.has_component<rotation>() && target.has_component<scale>())
            {
                ImGui::LabelText("##transform", "Transform");

                ImGui::Indent();

                if (base::RadioButton("Show Gizmo", showGizmo))
                    showGizmo = !showGizmo;

                if (showGizmo)
                {
                    position& pos = target.get_component<position>();
                    rotation& rot = target.get_component<rotation>();
                    scale& scal = target.get_component<scale>();
                    model = compose(scal, rot, pos);
                    gizmo::EditTransform(value_ptr(view), value_ptr(projection), value_ptr(model), true);
                    decompose(model, scal, rot, pos);
                }

                ImGui::Unindent();
            }
            else if (ImGui::Button("Add Transform"))
            {
                target.add_component<transform>();
            }

            for (auto componentId : target.component_composition())
            {
                if (DisplayGenericComponentEditor(target, componentId))
                    return;
            }

            ImGui::Separator();

            if (ImGui::BeginMenu("Add component"))
            {
                auto& families = ecs::Registry::getFamilies();
                for (auto& [componentId, family] : families)
                {
                    if (!family->contains(target))
                    {
                        if (ImGui::MenuItem(ecs::Registry::getFamilyName(componentId).c_str()))
                        {
                            ecs::Registry::createComponent(componentId, target);
                        }
                    }
                }
                ImGui::EndMenu();
            }

            ImGui::Separator();

            if (target.has_component<mesh_filter>())
            {
                DisplayMeshFilterEditor(target);
            }

            if (target.has_component<mesh_renderer>())
            {
                DisplayMeshRendererEditor(target);
            }
        }

        void onGUI(L_MAYBEUNUSED app::window& context, camera& cam, const camera::camera_input& camInput, L_MAYBEUNUSED time::span deltaTime)
        {
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
                windowName += "  (Selected:";

                if (selected->name.empty())
                {
                    if (selected == ecs::world)
                        selected->name = "World";
                    else if (selected.has_component<camera>())
                        selected->name = "Camera";
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

            gizmo::BeginFrame();

            base::Begin("Edit entity");
            if (selected)
            {
                DisplayEntityEditor(selected);
            }
            else
            {
                ImGui::Text("Select an entity to edit its components.");
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
