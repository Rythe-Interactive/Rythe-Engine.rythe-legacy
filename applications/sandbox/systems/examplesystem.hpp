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

namespace legion
{
    struct pause_action : public app::input_action<pause_action> {};
    struct play_action : public app::input_action<play_action> {};
    struct stop_action : public app::input_action<stop_action> {};
struct tonemap_action : public lgn::app::input_action<tonemap_action> {};
struct reload_shaders_action : public lgn::app::input_action<reload_shaders_action> {};
struct switch_skybox_action : public lgn::app::input_action<switch_skybox_action> {};
namespace legion::core
{

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

        auto* pipeline = dynamic_cast<gfx::DefaultPipeline*>(gfx::Renderer::getMainPipeline());
        if (pipeline)
            pipeline->attachStage<MouseHover>();
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
        
    }

    void playEmitter(legion::play_action& action);

    void pauseEmitter(legion::pause_action& action);

    void stopEmitter(legion::stop_action& action);
};
