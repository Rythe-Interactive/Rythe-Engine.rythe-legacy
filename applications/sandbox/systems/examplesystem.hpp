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
namespace legion::core
{
    struct example_policy : public particle_policy<example_policy>
    {
        NO_DTOR_RULE5_NOEXCEPT(example_policy);
        ~example_policy() = default;

        virtual void OnSetup(particle_emitter& emitter) override
        {
            emitter.setUniform<float>("scaleFactor", .2f);
        }

        virtual void OnInit(particle_emitter& emitter, size_type start, size_type end) override
        {
            auto scaleFactor = emitter.getUniform<float>("scaleFactor").get();
            for (size_type idx = start; idx <= end; idx++)
            {
                emitter.getBuffer<scale>()[idx] = scale(scaleFactor);
            }
        }

        virtual void OnUpdate(particle_emitter& emitter, float deltaTime, size_type count) override
        {
            auto& ageBuffer = emitter.getBuffer<life_time>();
            auto& scaleBuffer = emitter.getBuffer<scale>();

            auto scaleFactor = emitter.getUniform<float>("scaleFactor").get();

            for (size_type idx = 0; idx < count; idx++)
            {
                scaleBuffer[idx] = scale(scaleFactor - ((ageBuffer[idx].age / ageBuffer[idx].max) * scaleFactor));
            }
        }

        virtual void OnDestroy(particle_emitter& emitter, size_type start, size_type end) override
        {
        }
    };

    struct orbital_policy : public particle_policy<orbital_policy>
    {
        NO_DTOR_RULE5_NOEXCEPT(orbital_policy);
        ~orbital_policy() = default;

        const double C_MASS = 100.f;
        const double P_MASS = 50.f;
        const double G_FORCE = .1f;

        virtual void OnSetup(particle_emitter& emitter) override
        {

        }

        virtual void OnInit(particle_emitter& emitter, size_type start, size_type end) override
        {
            for (size_type idx = start; idx <= end; idx++)
            {
                auto minBound = math::sphericalRand(1.f);
                auto maxBound = minBound * 5.f;
                emitter.getBuffer<position>()[idx] = minBound + maxBound;
                emitter.getBuffer<velocity>()[idx] = math::normalize(math::cross(emitter.getBuffer<position>()[idx], math::vec3::up)) * 2.f;
            }
        }

        virtual void OnUpdate(particle_emitter& emitter, float deltaTime, size_type count) override
        {
            auto& posBuffer = emitter.getBuffer<position>();
            auto& velBuffer = emitter.getBuffer<velocity>();

            for (size_type idx = 0; idx < count; idx++)
            {
                auto r2 = math::pow(posBuffer[idx].x, 2) + math::pow(posBuffer[idx].y, 2) + math::pow(posBuffer[idx].z, 2);
                auto force = G_FORCE * ((P_MASS * C_MASS) / r2);
                velBuffer[idx] += math::normalize(-posBuffer[idx]) * (force / P_MASS) * deltaTime;
                posBuffer[idx] += velBuffer[idx] * deltaTime;
            }
        }

        virtual void OnDestroy(particle_emitter& emitter, size_type start, size_type end) override
        {

        }
    };

    struct fountain_policy : public particle_policy<fountain_policy>
    {
        NO_DTOR_RULE5_NOEXCEPT(fountain_policy);
        ~fountain_policy() = default;

        virtual void OnSetup(particle_emitter& emitter) override
        {

        }

        virtual void OnInit(particle_emitter& emitter, size_type start, size_type end) override
        {
            for (size_type idx = start; idx <= end; idx++)
            {
                emitter.getBuffer<position>()[idx] = math::vec3::zero;
                auto direction = math::vec3::up + math::normalize(math::vec3(math::linearRand(-5.f, 5.f), math::linearRand(-5.f, 5.f), math::linearRand(-5.f, 5.f)));
                emitter.getBuffer<velocity>()[idx] = direction * 2;
            }
        }

        virtual void OnUpdate(particle_emitter& emitter, float deltaTime, size_type count) override
        {
            auto& posBuffer = emitter.getBuffer<position>();
            auto& velBuffer = emitter.getBuffer<velocity>();
            for (size_type idx = 0; idx < count; idx++)
            {
                posBuffer[idx] += velBuffer[idx] * deltaTime;
                velBuffer[idx] += math::vec3(0.f, -9.8f, 0.f) * deltaTime;
            }
        }

        virtual void OnDestroy(particle_emitter& emitter, size_type start, size_type end) override
        {

        }
    };

    struct rendering_policy : particle_policy<rendering_policy>
    {
        NO_DTOR_RULE5_NOEXCEPT(rendering_policy);
        ~rendering_policy() = default;

        virtual void OnSetup(particle_emitter& emitter) override
        {
            auto model = gfx::ModelCache::create_model("Sphere", fs::view("assets://models/sphere.obj"));
            auto material = gfx::MaterialCache::create_material("Particle", fs::view("assets://shaders/color.shs"));
            material.set_param("color", math::color(.6f, 0, .5f, 1));

            emitter.setUniform<mesh_filter>("mesh_filter", mesh_filter(model.get_mesh()));
            emitter.setUniform<gfx::mesh_renderer>("renderer", gfx::mesh_renderer(material, model));
        }

        virtual void OnInit(particle_emitter& emitter, size_type start, size_type end) override
        {

        }

        virtual void OnUpdate(particle_emitter& emitter, float deltaTime, size_type count) override
        {

        }

        virtual void OnDestroy(particle_emitter& emitter, size_type start, size_type end) override
        {

        }
    };
}

class ExampleSystem final : public legion::System<ExampleSystem>
{
    lgn::size_type frames = 0;
    lgn::time64 totalTime = 0;
    lgn::time::stopwatch<lgn::time64> timer;
    std::array<lgn::time64, 18000> times;

public:
    void setup()
    {
        using namespace legion;
        log::filter(log::severity_debug);
        log::debug("ExampleSystem setup");

        app::InputSystem::createBinding<tonemap_action>(app::inputmap::method::F2);
        app::InputSystem::createBinding<reload_shaders_action>(app::inputmap::method::F3);
        app::InputSystem::createBinding<switch_skybox_action>(app::inputmap::method::F4);
        bindToEvent<tonemap_action, &ExampleSystem::onTonemapSwitch>();
        bindToEvent<reload_shaders_action, &ExampleSystem::onShaderReload>();
        bindToEvent<switch_skybox_action, &ExampleSystem::onSkyboxSwitch>();

        auto* pipeline = dynamic_cast<gfx::DefaultPipeline*>(gfx::Renderer::getMainPipeline());
        if (pipeline)
            pipeline->attachStage<MouseHover>();

        app::window& win = ecs::world.get_component<app::window>();
        app::context_guard guard(win);

        auto model = gfx::ModelCache::create_model("Sphere", fs::view("assets://models/sphere.obj"));

        auto material = gfx::MaterialCache::create_material("White", fs::view("assets://shaders/color.shs"));
        material.set_param("color", math::colors::white);
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

        {
            auto ent = createEntity(material.get_name());
            ent.add_component<example_comp>();
            auto [pos, rot, scal] = ent.add_component<transform>();
            pos = math::vec3(0.f, 0.f, 0.f);
            scal = math::vec3(3.f);
            rot = math::angleAxis(math::half_pi<float>(), math::vec3::up);
            ent.add_component(gfx::mesh_renderer(material, model));
        }

        material = gfx::MaterialCache::create_material("rock", fs::view("assets://shaders/pbr.shs"));

        material.set_param(SV_ALBEDO, rendering::TextureCache::create_texture(fs::view("assets://textures/detailedRock/Rock020_1K_Albedo.png")));
        material.set_param(SV_NORMALHEIGHT, rendering::TextureCache::create_texture(fs::view("assets://textures/detailedRock/Rock020_1K_NormalHeight.png")));
        material.set_param(SV_MRDAO, rendering::TextureCache::create_texture(fs::view("assets://textures/detailedRock/Rock020_1K_MRDAo.png")));
        material.set_param(SV_EMISSIVE, rendering::TextureCache::create_texture(fs::view("assets://textures/detailedRock/Rock020_1K_emissive.png")));
        material.set_param(SV_HEIGHTSCALE, 1.f);

        {
            auto ent = createEntity(material.get_name());
            ent.add_component<example_comp>();
            auto [pos, rot, scal] = ent.add_component<transform>();
            pos = math::vec3(0.f, 0.f, 4.f);
            rot = math::angleAxis(math::half_pi<float>(), math::vec3::up);
            scal = math::vec3(3.f);
            ent.add_component(gfx::mesh_renderer(material, model));
        }

        material = gfx::MaterialCache::create_material("copper", fs::view("assets://shaders/pbr.shs"));

        material.set_param(SV_ALBEDO, rendering::TextureCache::create_texture(fs::view("assets://textures/copper/copper-albedo-2048.png")));
        material.set_param(SV_NORMALHEIGHT, rendering::TextureCache::create_texture(fs::view("assets://textures/copper/copper-normalHeight-2048.png")));
        material.set_param(SV_MRDAO, rendering::TextureCache::create_texture(fs::view("assets://textures/copper/copper-MRDAo-2048.png")));
        material.set_param(SV_EMISSIVE, rendering::TextureCache::create_texture(fs::view("assets://textures/copper/copper-emissive-2048.png")));
        material.set_param(SV_HEIGHTSCALE, 1.f);

        {
            auto ent = createEntity(material.get_name());
            ent.add_component<example_comp>();
            auto [pos, rot, scal] = ent.add_component<transform>();
            pos = math::vec3(0.f, 0.f, 8.f);
            rot = math::angleAxis(math::half_pi<float>(), math::vec3::up);
            scal = math::vec3(3.f);
            ent.add_component(gfx::mesh_renderer(material, model));
        }

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

        {
            auto ent = createEntity(material.get_name());
            ent.add_component<example_comp>();
            auto [pos, rot, scal] = ent.add_component<transform>();
            pos = math::vec3(0.f, 4.f, 0.f);
            rot = math::angleAxis(math::half_pi<float>(), math::vec3::up);
            scal = math::vec3(3.f);
            ent.add_component(gfx::mesh_renderer(material, model));
        }

        material = gfx::MaterialCache::create_material("paint", fs::view("assets://shaders/pbr.shs"));

        material.set_param(SV_ALBEDO, rendering::TextureCache::create_texture(fs::view("assets://textures/paint/paint-peeling-albedo-2048.png")));
        material.set_param(SV_NORMALHEIGHT, rendering::TextureCache::create_texture(fs::view("assets://textures/paint/paint-peeling-normalHeight-2048.png")));
        material.set_param(SV_MRDAO, rendering::TextureCache::create_texture(fs::view("assets://textures/paint/paint-peeling-MRDAo-2048.png")));
        material.set_param(SV_EMISSIVE, rendering::TextureCache::create_texture(fs::view("assets://textures/paint/paint-peeling-emissive-2048.png")));
        material.set_param(SV_HEIGHTSCALE, 1.f);

        {
            auto ent = createEntity(material.get_name());
            ent.add_component<example_comp>();
            auto [pos, rot, scal] = ent.add_component<transform>();
            pos = math::vec3(0.f, 4.f, 4.f);
            rot = math::angleAxis(math::half_pi<float>(), math::vec3::up);
            scal = math::vec3(3.f);
            ent.add_component(gfx::mesh_renderer(material, model));
        }

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

        {
            auto ent = createEntity(material.get_name());
            ent.add_component<example_comp>();
            auto [pos, rot, scal] = ent.add_component<transform>();
            pos = math::vec3(0.f, 4.f, 8.f);
            rot = math::angleAxis(math::half_pi<float>(), math::vec3::up);
            scal = math::vec3(3.f);
            ent.add_component(gfx::mesh_renderer(material, model));
        }

        material = gfx::MaterialCache::create_material("slate", fs::view("assets://shaders/pbr.shs"));

        material.set_param(SV_ALBEDO, rendering::TextureCache::create_texture(fs::view("assets://textures/slate/slate-albedo-2048.png")));
        material.set_param(SV_NORMALHEIGHT, rendering::TextureCache::create_texture(fs::view("assets://textures/slate/slate-normalHeight-2048.png")));
        material.set_param(SV_MRDAO, rendering::TextureCache::create_texture(fs::view("assets://textures/slate/slate-MRDAo-2048.png")));
        material.set_param(SV_EMISSIVE, rendering::TextureCache::create_texture(fs::view("assets://textures/slate/slate-emissive-2048.png")));
        material.set_param(SV_HEIGHTSCALE, 1.f);

        {
            auto ent = createEntity(material.get_name());
            ent.add_component<example_comp>();
            auto [pos, rot, scal] = ent.add_component<transform>();
            pos = math::vec3(0.f, 8.f, 0.f);
            rot = math::angleAxis(math::half_pi<float>(), math::vec3::up);
            scal = math::vec3(3.f);
            ent.add_component(gfx::mesh_renderer(material, model));
        }

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

        {
            auto ent = createEntity(material.get_name());
            ent.add_component<example_comp>();
            auto [pos, rot, scal] = ent.add_component<transform>();
            pos = math::vec3(0.f, 8.f, 4.f);
            rot = math::angleAxis(math::half_pi<float>(), math::vec3::up);
            scal = math::vec3(3.f);
            ent.add_component(gfx::mesh_renderer(material, model));
        }

        material = gfx::MaterialCache::create_material("default", fs::view("assets://shaders/pbr.shs"));

        material.set_param(SV_ALBEDO, rendering::TextureCache::create_texture(fs::view("engine://resources/default/albedo")));
        material.set_param(SV_NORMALHEIGHT, rendering::TextureCache::create_texture(fs::view("engine://resources/default/normalHeight")));
        material.set_param(SV_MRDAO, rendering::TextureCache::create_texture(fs::view("engine://resources/default/MRDAo")));
        material.set_param(SV_EMISSIVE, rendering::TextureCache::create_texture(fs::view("engine://resources/default/emissive")));
        material.set_param(SV_HEIGHTSCALE, 1.f);

        {
            auto ent = createEntity(material.get_name());
            ent.add_component<example_comp>();
            auto [pos, rot, scal] = ent.add_component<transform>();
            pos = math::vec3(0.f, 8.f, 8.f);
            rot = math::angleAxis(math::half_pi<float>(), math::vec3::up);
            scal = math::vec3(3.f);
            ent.add_component(gfx::mesh_renderer(material, model));
        }

        bindToEvent<events::exit, &ExampleSystem::onExit>();
    }

    void shutdown()
    {
        lgn::log::debug("ExampleSystem shutdown");
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

        {
            ecs::filter<rotation, example_comp> filter;
            for (auto& ent : filter)
            {
                ent.get_component<rotation>().get() *= math::angleAxis(math::two_pi<float>() * 0.1f * deltaTime, math::vec3::up);
            }
        }

        ecs::filter<position, velocity, example_comp> filter;

        float dt = deltaTime;
        if (dt > 0.07f)
            return;

        if (filter.size())
        {
            auto poolSize = (schd::Scheduler::jobPoolSize() + 1);
            size_type jobSize = math::iround(math::ceil(filter.size() / static_cast<float>(poolSize)));

            queueJobs(poolSize, [&](id_type jobId)
                {
                    auto start = jobId * jobSize;
                    auto end = start + jobSize;
                    if (end > filter.size())
                        end = filter.size();

                    for (size_type i = start; i < end; i++)
                    {
                        auto& pos = filter[i].get_component<position>().get();
                        auto& vel = filter[i].get_component<velocity>().get();

                        if (vel == math::vec3::zero)
                            vel = math::normalize(pos);

                        math::vec3 perp;

                        perp = math::normalize(math::cross(vel, math::vec3::up));

                        math::vec3 rotated = (math::axisAngleMatrix(vel, math::perlin(pos) * math::pi<float>()) * math::vec4(perp.x, perp.y, perp.z, 0)).xyz();
                        rotated.y -= 0.5f;
                        rotated = math::normalize(rotated);

                        vel = math::normalize(vel + rotated * dt);

                        if (math::abs(vel.y) >= 0.9f)
                        {
                            auto rand = math::circularRand(1.f);
                            vel.y = 0.9f;
                            vel = math::normalize(vel + math::vec3(rand.x, 0.f, rand.y));
                        }

                        pos += vel * 0.3f * dt;
                    }
                }
            ).wait();
        }

        time64 delta = schd::Clock::lastTickDuration();

        if (frames < times.size())
        {
            times[frames] = delta;
            frames++;
            totalTime += delta;
        }
        else
        {

            //raiseEvent<events::exit>();
        }
    }
};
