#pragma once
#include <core/core.hpp>
#include <application/application.hpp>
#include <rendering/rendering.hpp>
#include <rendering/debugrendering.hpp>
#include <audio/audio.hpp>
#include "../renderstages/mousehover.hpp"
#include "gui_test.hpp"

struct example_comp
{

};

struct tonemap_action : public lgn::app::input_action<tonemap_action> {};
struct reload_shaders_action : public lgn::app::input_action<reload_shaders_action> {};
struct switch_skybox_action : public lgn::app::input_action<switch_skybox_action> {};
struct toggle_anim_action : public lgn::app::input_action<toggle_anim_action> {};

class ExampleSystem final : public legion::System<ExampleSystem>
{
    lgn::size_type frames = 0;
    lgn::time64 totalTime = 0;
    lgn::time::stopwatch<lgn::time64> timer;
    std::array<lgn::time64, 18000> times;
    bool toggle_anim = false;

public:
    void setup()
    {
        using namespace legion;
        log::filter(log::severity_debug);
        log::debug("ExampleSystem setup");

        app::InputSystem::createBinding<tonemap_action>(app::inputmap::method::F2);
        app::InputSystem::createBinding<reload_shaders_action>(app::inputmap::method::F3);
        app::InputSystem::createBinding<switch_skybox_action>(app::inputmap::method::F4);
        app::InputSystem::createBinding<toggle_anim_action>(app::inputmap::method::F8);

        bindToEvent<tonemap_action, &ExampleSystem::onTonemapSwitch>();
        bindToEvent<reload_shaders_action, &ExampleSystem::onShaderReload>();
        bindToEvent<switch_skybox_action, &ExampleSystem::onSkyboxSwitch>();
        bindToEvent<toggle_anim_action, &ExampleSystem::onToggleAnim>();

        auto* pipeline = dynamic_cast<gfx::DefaultPipeline*>(gfx::Renderer::getMainPipeline());
        if (pipeline)
            pipeline->attachStage<MouseHover>();

        app::window& win = ecs::world.get_component<app::window>();
        app::context_guard guard(win);

        auto material = gfx::MaterialCache::create_material("White", fs::view("assets://shaders/color.shs"));
        material.set_param("color", math::colors::green);

        auto model = gfx::ModelCache::create_model("Sphere", fs::view("assets://models/sphere.obj"));

        auto riggedMesh = gfx::ModelCache::create_model("RiggedModel", fs::view("assets://models/BrainStem.gltf"));

        {
            material = gfx::MaterialCache::create_material("Animated", fs::view("assets://shaders/animatedmesh.shs"));
            auto ent = createEntity("Rigged Model");
            ent.add_component<transform>();
            gfx::skinned_mesh_renderer& skinned = ent.add_component(gfx::skinned_mesh_renderer(material, riggedMesh));
            animator& anim = ent.add_component<animator>(animator{ "Anim_0" });
            anim.skeleton = assets::AssetCache<skeleton>::get("Node_3");
            skinned.m_skeleton = anim.skeleton;
        }

        /*{
            auto ent = createEntity("Sun");
            ent.add_component(gfx::light::directional(math::color(1, 1, 0.8f), 10.f));
            auto [pos, rot, scal] = ent.add_component<transform>();
            pos = position(0.f, 3.f, 6.f);
            rot = rotation::lookat(math::vec3::zero, math::vec3(1.f, -1.f, 0.1f));
        }*/

        //#if defined(LEGION_DEBUG)
        //        for (int i = 0; i < 2000; i++)
        //#else
        //        for (int i = 0; i < 20000; i++)
        //#endif
        //        {
        //            auto ent = createEntity();
        //
        //            auto [pos, rot, scal] = ent.add_component<transform>();
        //            pos = math::sphericalRand(5.f);
        //            scal = scale(0.1f, 0.1f, 0.1f);
        //
        //            ent.add_component<example_comp, velocity>();
        //            ent.add_component(gfx::mesh_renderer(material, model));
        //        }

        material = gfx::MaterialCache::create_material("iron", fs::view("engine://shaders/default_lit.shs"));

        material.set_param("alphaCutoff", 0.5f);
        material.set_param("useAlbedoTex", true);
        material.set_param("useRoughnessTex", true);
        material.set_param("useNormal", true);
        material.set_param("useAmbientOcclusion", true);
        material.set_param("useHeight", true);
        material.set_param("useMetallicTex", true);

        material.set_param("useEmissiveTex", false);
        material.set_param("useMetallicRoughness", false);

        material.set_param("ambientOcclusionTex", rendering::TextureCache::create_texture(fs::view("assets://textures/iron/rustediron-ao.png")));
        material.set_param("heightTex", rendering::TextureCache::create_texture(fs::view("assets://textures/iron/rustediron-height.png")));
        material.set_param("heightScale", 1.f);
        material.set_param("metallicTex", rendering::TextureCache::create_texture(fs::view("assets://textures/iron/rustediron-metallic.png")));
        material.set_param("emissiveColor", math::colors::black);

        material.set_param("albedoTex", rendering::TextureCache::create_texture(fs::view("assets://textures/iron/rustediron-albedo-2048.png")));
        material.set_param("normalTex", rendering::TextureCache::create_texture(fs::view("assets://textures/iron/rustediron-normal.png")));
        material.set_param("roughnessTex", rendering::TextureCache::create_texture(fs::view("assets://textures/iron/rustediron-roughness.png")));

        //{
        //    auto ent = createEntity(material.get_name());
        //    ent.add_component<example_comp>();
        //    auto [pos, rot, scal] = ent.add_component<transform>();
        //    pos = math::vec3(0.f, 0.f, 0.f);
        //    scal = math::vec3(3.f);
        //    rot = math::angleAxis(math::half_pi<float>(), math::vec3::up);
        //    ent.add_component(gfx::mesh_renderer(material, model));
        //}

        material = gfx::MaterialCache::create_material("rock", fs::view("assets://shaders/pbr.shs"));

        material.set_param(SV_ALBEDO, rendering::TextureCache::create_texture(fs::view("assets://textures/detailedRock/Rock020_1K_Albedo.png")));
        material.set_param(SV_NORMALHEIGHT, rendering::TextureCache::create_texture(fs::view("assets://textures/detailedRock/Rock020_1K_NormalHeight.png")));
        material.set_param(SV_MRDAO, rendering::TextureCache::create_texture(fs::view("assets://textures/detailedRock/Rock020_1K_MRDAo.png")));
        material.set_param(SV_EMISSIVE, rendering::TextureCache::create_texture(fs::view("assets://textures/detailedRock/Rock020_1K_emissive.png")));
        material.set_param(SV_HEIGHTSCALE, 1.f);

        //{
        //    auto ent = createEntity(material.get_name());
        //    ent.add_component<example_comp>();
        //    auto [pos, rot, scal] = ent.add_component<transform>();
        //    pos = math::vec3(0.f, 0.f, 4.f);
        //    rot = math::angleAxis(math::half_pi<float>(), math::vec3::up);
        //    scal = math::vec3(3.f);
        //    ent.add_component(gfx::mesh_renderer(material, model));
        //}

        material = gfx::MaterialCache::create_material("copper", fs::view("assets://shaders/pbr.shs"));

        material.set_param(SV_ALBEDO, rendering::TextureCache::create_texture(fs::view("assets://textures/copper/copper-albedo-2048.png")));
        material.set_param(SV_NORMALHEIGHT, rendering::TextureCache::create_texture(fs::view("assets://textures/copper/copper-normalHeight-2048.png")));
        material.set_param(SV_MRDAO, rendering::TextureCache::create_texture(fs::view("assets://textures/copper/copper-MRDAo-2048.png")));
        material.set_param(SV_EMISSIVE, rendering::TextureCache::create_texture(fs::view("assets://textures/copper/copper-emissive-2048.png")));
        material.set_param(SV_HEIGHTSCALE, 1.f);

        //{
        //    auto ent = createEntity(material.get_name());
        //    ent.add_component<example_comp>();
        //    auto [pos, rot, scal] = ent.add_component<transform>();
        //    pos = math::vec3(0.f, 0.f, 8.f);
        //    rot = math::angleAxis(math::half_pi<float>(), math::vec3::up);
        //    scal = math::vec3(3.f);
        //    ent.add_component(gfx::mesh_renderer(material, model));
        //}

        material = gfx::MaterialCache::create_material("wood", fs::view("engine://shaders/default_lit.shs"));

        material.set_param("alphaCutoff", 0.5f);
        material.set_param("useAlbedoTex", true);
        material.set_param("useRoughnessTex", true);
        material.set_param("useNormal", true);
        material.set_param("useHeight", true);

        material.set_param("useAmbientOcclusion", false);
        material.set_param("useMetallicTex", false);
        material.set_param("useEmissiveTex", false);
        material.set_param("useMetallicRoughness", false);

        material.set_param("heightTex", rendering::TextureCache::create_texture(fs::view("assets://textures/wood/Wood035_1K_Displacement.png")));
        material.set_param("heightScale", 1.f);
        material.set_param("metallicValue", 0.f);
        material.set_param("emissiveColor", math::colors::black);

        material.set_param("albedoTex", rendering::TextureCache::create_texture(fs::view("assets://textures/wood/WoodColor.png")));
        material.set_param("normalTex", rendering::TextureCache::create_texture(fs::view("assets://textures/wood/Wood035_1K_Normal.png")));
        material.set_param("roughnessTex", rendering::TextureCache::create_texture(fs::view("assets://textures/wood/Wood035_1K_Roughness.png")));

        //{
        //    auto ent = createEntity(material.get_name());
        //    ent.add_component<example_comp>();
        //    auto [pos, rot, scal] = ent.add_component<transform>();
        //    pos = math::vec3(0.f, 4.f, 0.f);
        //    rot = math::angleAxis(math::half_pi<float>(), math::vec3::up);
        //    scal = math::vec3(3.f);
        //    ent.add_component(gfx::mesh_renderer(material, model));
        //}

        material = gfx::MaterialCache::create_material("paint", fs::view("assets://shaders/pbr.shs"));

        material.set_param(SV_ALBEDO, rendering::TextureCache::create_texture(fs::view("assets://textures/paint/paint-peeling-albedo-2048.png")));
        material.set_param(SV_NORMALHEIGHT, rendering::TextureCache::create_texture(fs::view("assets://textures/paint/paint-peeling-normalHeight-2048.png")));
        material.set_param(SV_MRDAO, rendering::TextureCache::create_texture(fs::view("assets://textures/paint/paint-peeling-MRDAo-2048.png")));
        material.set_param(SV_EMISSIVE, rendering::TextureCache::create_texture(fs::view("assets://textures/paint/paint-peeling-emissive-2048.png")));
        material.set_param(SV_HEIGHTSCALE, 1.f);

        //{
        //    auto ent = createEntity(material.get_name());
        //    ent.add_component<example_comp>();
        //    auto [pos, rot, scal] = ent.add_component<transform>();
        //    pos = math::vec3(0.f, 4.f, 4.f);
        //    rot = math::angleAxis(math::half_pi<float>(), math::vec3::up);
        //    scal = math::vec3(3.f);
        //    ent.add_component(gfx::mesh_renderer(material, model));
        //}

        material = gfx::MaterialCache::create_material("tiles", fs::view("engine://shaders/default_lit.shs"));

        material.set_param("alphaCutoff", 0.5f);
        material.set_param("useAlbedoTex", true);
        material.set_param("useRoughnessTex", true);
        material.set_param("useNormal", true);

        material.set_param("useMetallicTex", false);
        material.set_param("useHeight", false);
        material.set_param("useAmbientOcclusion", false);
        material.set_param("useEmissiveTex", false);
        material.set_param("useMetallicRoughness", false);

        material.set_param("metallicValue", 0.f);
        material.set_param("emissiveColor", math::colors::black);

        material.set_param("albedoTex", rendering::TextureCache::create_texture(fs::view("assets://textures/tile/tileColor.png")));
        material.set_param("normalTex", rendering::TextureCache::create_texture(fs::view("assets://textures/tile/tileNormal.png")));
        material.set_param("roughnessTex", rendering::TextureCache::create_texture(fs::view("assets://textures/tile/tileRoughness.png")));

        //{
        //    auto ent = createEntity(material.get_name());
        //    ent.add_component<example_comp>();
        //    auto [pos, rot, scal] = ent.add_component<transform>();
        //    pos = math::vec3(0.f, 4.f, 8.f);
        //    rot = math::angleAxis(math::half_pi<float>(), math::vec3::up);
        //    scal = math::vec3(3.f);
        //    ent.add_component(gfx::mesh_renderer(material, model));
        //}

        material = gfx::MaterialCache::create_material("slate", fs::view("assets://shaders/pbr.shs"));

        material.set_param(SV_ALBEDO, rendering::TextureCache::create_texture(fs::view("assets://textures/slate/slate-albedo-2048.png")));
        material.set_param(SV_NORMALHEIGHT, rendering::TextureCache::create_texture(fs::view("assets://textures/slate/slate-normalHeight-2048.png")));
        material.set_param(SV_MRDAO, rendering::TextureCache::create_texture(fs::view("assets://textures/slate/slate-MRDAo-2048.png")));
        material.set_param(SV_EMISSIVE, rendering::TextureCache::create_texture(fs::view("assets://textures/slate/slate-emissive-2048.png")));
        material.set_param(SV_HEIGHTSCALE, 1.f);

        //{
        //    auto ent = createEntity(material.get_name());
        //    ent.add_component<example_comp>();
        //    auto [pos, rot, scal] = ent.add_component<transform>();
        //    pos = math::vec3(0.f, 8.f, 0.f);
        //    rot = math::angleAxis(math::half_pi<float>(), math::vec3::up);
        //    scal = math::vec3(3.f);
        //    ent.add_component(gfx::mesh_renderer(material, model));
        //}

        material = gfx::MaterialCache::create_material("bricks", fs::view("engine://shaders/default_lit.shs"));

        material.set_param("alphaCutoff", 0.5f);
        material.set_param("useAlbedoTex", true);
        material.set_param("useRoughnessTex", true);
        material.set_param("useNormal", true);
        material.set_param("useHeight", true);
        material.set_param("useAmbientOcclusion", true);

        material.set_param("useMetallicTex", false);
        material.set_param("useEmissiveTex", false);
        material.set_param("useMetallicRoughness", false);

        material.set_param("metallicValue", 0.f);
        material.set_param("emissiveColor", math::colors::black);

        material.set_param("albedoTex", rendering::TextureCache::create_texture(fs::view("assets://textures/brick/brickColor.png")));
        material.set_param("roughnessTex", rendering::TextureCache::create_texture(fs::view("assets://textures/brick/brickRoughness.png")));
        material.set_param("normalTex", rendering::TextureCache::create_texture(fs::view("assets://textures/brick/brickNormal.png")));
        material.set_param("ambientOcclusionTex", rendering::TextureCache::create_texture(fs::view("assets://textures/brick/brickAO.png")));
        material.set_param("heightTex", rendering::TextureCache::create_texture(fs::view("assets://textures/brick/brickDisplacement.png")));
        material.set_param("heightScale", 0.1f);

        //{
        //    auto ent = createEntity(material.get_name());
        //    ent.add_component<example_comp>();
        //    auto [pos, rot, scal] = ent.add_component<transform>();
        //    pos = math::vec3(0.f, 8.f, 4.f);
        //    rot = math::angleAxis(math::half_pi<float>(), math::vec3::up);
        //    scal = math::vec3(3.f);
        //    ent.add_component(gfx::mesh_renderer(material, model));
        //}

        material = gfx::MaterialCache::create_material("default", fs::view("assets://shaders/pbr.shs"));

        material.set_param(SV_ALBEDO, rendering::TextureCache::create_texture(fs::view("engine://resources/default/albedo")));
        material.set_param(SV_NORMALHEIGHT, rendering::TextureCache::create_texture(fs::view("engine://resources/default/normalHeight")));
        material.set_param(SV_MRDAO, rendering::TextureCache::create_texture(fs::view("engine://resources/default/MRDAo")));
        material.set_param(SV_EMISSIVE, rendering::TextureCache::create_texture(fs::view("engine://resources/default/emissive")));
        material.set_param(SV_HEIGHTSCALE, 1.f);

        //{
        //    auto ent = createEntity(material.get_name());
        //    ent.add_component<example_comp>();
        //    auto [pos, rot, scal] = ent.add_component<transform>();
        //    pos = math::vec3(0.f, 8.f, 8.f);
        //    rot = math::angleAxis(math::half_pi<float>(), math::vec3::up);
        //    scal = math::vec3(3.f);
        //    ent.add_component(gfx::mesh_renderer(material, model));
        //}

        bindToEvent<events::exit, &ExampleSystem::onExit>();
    }

    void shutdown()
    {
        lgn::log::debug("ExampleSystem shutdown");
    }

    void debug_skeleton(legion::core::joint& parentJoint, legion::core::math::color debugColor = legion::core::math::colors::blue)
    {
        using namespace legion;
        math::vec3 pos;
        math::quat rot;
        math::vec3 scal;
        math::decompose(parentJoint.animatedTransform, scal, rot, pos);
        for (joint& j : parentJoint.children)
        {
            debug_skeleton(j, debugColor);
            math::vec3 c_pos;
            math::quat c_rot;
            math::vec3 c_scal;
            math::decompose(j.animatedTransform, c_scal, c_rot, c_pos);
            debug::drawLine(pos, c_pos, debugColor, 1.0f);
        }
    }

    void onShaderReload(reload_shaders_action& event)
    {
        using namespace legion;
        if (event.pressed())
        {
            auto targetWin = ecs::world.get_component<app::window>();
            if (!targetWin)
                return;

            app::window& win = targetWin.get();

            if (!app::WindowSystem::windowStillExists(win.handle))
                return;

            app::context_guard guard(win);
            if (guard.contextIsValid())
            {
                gfx::ShaderCache::clear_checked_paths();
                gfx::ShaderCache::reload_shaders();

                auto [lock, materials] = gfx::MaterialCache::get_all_materials();
                async::readonly_guard materialsGuard(lock);
                for (auto& [id, mat] : materials)
                    mat.reload();
            }
        }
    }

    void onTonemapSwitch(tonemap_action& event)
    {
        using namespace legion;
        if (event.pressed())
        {
            static size_type type = static_cast<size_type>(gfx::tonemapping_type::legion);
            type = (type + 1) % (static_cast<size_type>(gfx::tonemapping_type::unreal3) + 1);

            auto typeEnum = static_cast<gfx::tonemapping_type>(type);

            std::string algorithmName;
            switch (typeEnum)
            {
            case gfx::tonemapping_type::aces:
                algorithmName = "aces";
                break;
            case gfx::tonemapping_type::reinhard:
                algorithmName = "reinhard";
                break;
            case gfx::tonemapping_type::reinhard_jodie:
                algorithmName = "reinhard_jodie";
                break;
            case gfx::tonemapping_type::legion:
                algorithmName = "legion";
                break;
            case gfx::tonemapping_type::unreal3:
                algorithmName = "unreal3";
                break;
            default:
                algorithmName = "legion";
                break;
            }
            log::debug("Set tonemapping algorithm to {}", algorithmName);

            gfx::Tonemapping::setAlgorithm(typeEnum);
        }
    }

    void onSkyboxSwitch(switch_skybox_action& event)
    {
        using namespace legion;
        using namespace rendering;
        if (event.pressed())
        {
            static size_type idx = 0;
            static texture_handle textures[4] = {};
            static bool initialized = false;

            if (!initialized)
            {
                auto targetWin = ecs::world.get_component<app::window>();
                if (!targetWin)
                    return;

                app::window& win = targetWin.get();

                if (!app::WindowSystem::windowStillExists(win.handle))
                    return;

                app::context_guard guard(win);
                if (guard.contextIsValid())
                {
                    auto importSettings = texture_import_settings{
                            texture_type::two_dimensional, true, channel_format::eight_bit, texture_format::rgba_hdr,
                            texture_components::rgba, true, true, 0, texture_mipmap::linear_mipmap_linear, texture_mipmap::linear,
                            texture_wrap::edge_clamp,texture_wrap::repeat, texture_wrap::edge_clamp };

                    textures[0] = TextureCache::create_texture("morning islands", fs::view("assets://textures/HDRI/morning_islands.jpg"), importSettings);
                    textures[1] = TextureCache::create_texture("earth", fs::view("assets://textures/HDRI/earth.png"), importSettings);
                    textures[2] = TextureCache::create_texture("park", fs::view("assets://textures/HDRI/park.jpg"), importSettings);
                    textures[3] = TextureCache::create_texture("atmosphere", fs::view("assets://textures/HDRI/planetatmo.png"), importSettings);
                    initialized = true;
                }
                else
                    return;
            }

            idx = (idx + 1) % 4;
            auto skyboxRenderer = ecs::world.get_component<skybox_renderer>();
            skyboxRenderer->material.set_param(SV_SKYBOX, textures[idx]);

            log::debug("Set skybox to {}", textures[idx].get_texture().name);
        }
    }

    void onToggleAnim(toggle_anim_action& event)
    {
        using namespace legion;
        if (event.pressed())
        {
            auto& ent = GuiTestSystem::selected;
            if (ent != invalid_id)
                if (ent.has_component<animator>())
                {
                    animator& anim = ent.get_component<animator>();
                    anim.play = !anim.play;
                }
        }
    }

    void onExit(L_MAYBEUNUSED lgn::events::exit& event)
    {
        using namespace legion;

        time64 avg0 = totalTime / frames;
        time64 avg1 = timer.elapsed_time() / frames;

        std::set<time64, std::greater<time64>> orderedTimes;

        for (auto& time : times)
            orderedTimes.insert(time);

        time64 onePcLow = 0;
        time64 pointOnePcLow = 0;

        size_type i = 0;
        for (auto& time : orderedTimes)
        {
            i++;
            onePcLow += time;

            if (i <= math::max<size_type>(math::uround(frames / 1000.0), 1))
            {
                pointOnePcLow += time;
            }

            if (i >= math::max<size_type>(math::uround(frames / 100.0), 1))
            {
                break;
            }
        }

        pointOnePcLow /= math::max<size_type>(math::uround(frames / 1000.0), 1);
        onePcLow /= math::max<size_type>(math::uround(frames / 100.0), 1);

        log::info("1%Low {:.3f} 0.1%Low {:.3f} Avg {:.3f} Measured Avg {:.3f}", onePcLow, pointOnePcLow, avg0, avg1);
        log::info("1%Low {:.3f} 0.1%Low {:.3f} Avg {:.3f} Measured Avg {:.3f}", 1.0 / onePcLow, 1.0 / pointOnePcLow, 1.0 / avg0, 1.0 / avg1);
    }

    void update(legion::time::span deltaTime)
    {
        using namespace legion;

        auto hoveredEntityId = MouseHover::getHoveredEntityId();
        if (hoveredEntityId != invalid_id)
        {
            auto ent = ecs::Registry::getEntity(hoveredEntityId);

            if (ent != GuiTestSystem::selected && ent.has_component<transform>())
            {
                transform transf = ent.get_component<transform>();

                math::mat4 worldMat = transf.to_world_matrix();
                math::vec3 min = math::vec3(-0.5f, -0.5f, -0.5f);
                math::vec3 max = math::vec3(0.5f, 0.5f, 0.5f);

                std::pair<math::vec3, math::vec3> edges[] = {
                    std::make_pair(min, math::vec3(min.x, min.y, max.z)),
                    std::make_pair(math::vec3(min.x, min.y, max.z), math::vec3(max.x, min.y, max.z)),
                    std::make_pair(math::vec3(max.x, min.y, max.z), math::vec3(max.x, min.y, min.z)),
                    std::make_pair(math::vec3(max.x, min.y, min.z), min),

                    std::make_pair(max, math::vec3(max.x, max.y, min.z)),
                    std::make_pair(math::vec3(max.x, max.y, min.z), math::vec3(min.x, max.y, min.z)),
                    std::make_pair(math::vec3(min.x, max.y, min.z), math::vec3(min.x, max.y, max.z)),
                    std::make_pair(math::vec3(min.x, max.y, max.z), max),

                    std::make_pair(min, math::vec3(min.x, max.y, min.z)),
                    std::make_pair(math::vec3(min.x, min.y, max.z), math::vec3(min.x, max.y, max.z)),
                    std::make_pair(math::vec3(max.x, min.y, max.z), math::vec3(max.x, max.y, max.z)),
                    std::make_pair(math::vec3(max.x, min.y, min.z), math::vec3(max.x, max.y, min.z))
                };

                for (auto& edge : edges)
                    debug::drawLine((worldMat * math::vec4(edge.first.x, edge.first.y, edge.first.z, 1.f)).xyz(), (worldMat * math::vec4(edge.second.x, edge.second.y, edge.second.z, 1.f)).xyz(), math::colors::orange);
            }
        }

        if (GuiTestSystem::selected != invalid_id)
        {
            if (GuiTestSystem::selected.has_component<transform>())
            {
                transform transf = GuiTestSystem::selected.get_component<transform>();

                math::mat4 worldMat = transf.to_world_matrix();
                math::vec3 min = math::vec3(-0.5f, -0.5f, -0.5f);
                math::vec3 max = math::vec3(0.5f, 0.5f, 0.5f);

                std::pair<math::vec3, math::vec3> edges[] = {
                    std::make_pair(min, math::vec3(min.x, min.y, max.z)),
                    std::make_pair(math::vec3(min.x, min.y, max.z), math::vec3(max.x, min.y, max.z)),
                    std::make_pair(math::vec3(max.x, min.y, max.z), math::vec3(max.x, min.y, min.z)),
                    std::make_pair(math::vec3(max.x, min.y, min.z), min),

                    std::make_pair(max, math::vec3(max.x, max.y, min.z)),
                    std::make_pair(math::vec3(max.x, max.y, min.z), math::vec3(min.x, max.y, min.z)),
                    std::make_pair(math::vec3(min.x, max.y, min.z), math::vec3(min.x, max.y, max.z)),
                    std::make_pair(math::vec3(min.x, max.y, max.z), max),

                    std::make_pair(min, math::vec3(min.x, max.y, min.z)),
                    std::make_pair(math::vec3(min.x, min.y, max.z), math::vec3(min.x, max.y, max.z)),
                    std::make_pair(math::vec3(max.x, min.y, max.z), math::vec3(max.x, max.y, max.z)),
                    std::make_pair(math::vec3(max.x, min.y, min.z), math::vec3(max.x, max.y, min.z))
                };

                for (auto& edge : edges)
                    debug::drawLine((worldMat * math::vec4(edge.first.x, edge.first.y, edge.first.z, 1.f)).xyz(), (worldMat * math::vec4(edge.second.x, edge.second.y, edge.second.z, 1.f)).xyz(), math::colors::green);
            }
        }
        static bool firstFrame = true;
        if (firstFrame)
        {
            timer.start();
            firstFrame = false;
        }

        if (GuiTestSystem::selected != invalid_id)
        {
            if (GuiTestSystem::selected.has_component<animator>())
            {
                auto& rig = GuiTestSystem::selected.get_component<animator>()->skeleton->rootJoint;
                debug_skeleton(rig);
            }
        }
    }
};
