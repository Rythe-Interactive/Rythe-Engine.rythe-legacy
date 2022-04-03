#include <chrono>

#include "../systems/examplesystem.hpp"
#include "../defaults/defaultpolicies.hpp"
#include <rendering/data/particlepolicies/renderpolicy.hpp>
#include <rendering/data/particlepolicies/flipbookpolicy.hpp>


void ExampleSystem::setup()
{
    using namespace legion;
    log::filter(log::severity_debug);
    log::debug("ExampleSystem setup");

    app::InputSystem::createBinding<tonemap_action>(app::inputmap::method::F2);
    app::InputSystem::createBinding<switch_skybox_action>(app::inputmap::method::F3);
    app::InputSystem::createBinding<auto_exposure_action>(app::inputmap::method::F4);
    app::InputSystem::createBinding<reload_shaders_action>(app::inputmap::method::F5);
    bindToEvent<tonemap_action, &ExampleSystem::onTonemapSwitch>();
    bindToEvent<switch_skybox_action, &ExampleSystem::onSkyboxSwitch>();
    bindToEvent<auto_exposure_action, &ExampleSystem::onAutoExposureSwitch>();
    bindToEvent<reload_shaders_action, &ExampleSystem::onShaderReload>();
    bindToEvent<events::exit, &ExampleSystem::onExit>();

    //Serialization Test
    srl::SerializerRegistry::registerSerializer<example_comp>();
    srl::SerializerRegistry::registerSerializer<ecs::entity>();
    srl::SerializerRegistry::registerSerializer<position>();
    srl::SerializerRegistry::registerSerializer<rotation>();
    srl::SerializerRegistry::registerSerializer<velocity>();
    srl::SerializerRegistry::registerSerializer<scale>();
    srl::SerializerRegistry::registerSerializer<assets::import_settings<mesh>>();
    srl::SerializerRegistry::registerSerializer<sub_mesh>();
    srl::SerializerRegistry::registerSerializer<mesh>();
    srl::SerializerRegistry::registerSerializer<assets::asset<mesh>>();
    srl::SerializerRegistry::registerSerializer<material_data>();
    srl::SerializerRegistry::registerSerializer<mesh_filter>();

    auto* pipeline = dynamic_cast<gfx::DefaultPipeline*>(gfx::Renderer::getMainPipeline());
    if (pipeline)
        pipeline->attachStage<MouseHover>();

    app::window& win = ecs::world.get_component<app::window>();
    app::context_guard guard(win);

    srl::load<srl::bson, ecs::entity>(fs::view("assets://scenes/scene1.bson"), "scene");
}

void ExampleSystem::onShaderReload(reload_shaders_action& event)
{
    using namespace legion;
    if (event.released())
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

void ExampleSystem::onAutoExposureSwitch(auto_exposure_action& event)
{
    using namespace legion;
    if (event.released())
    {
        static float defaultExposure = gfx::Tonemapping::getExposure();
        bool enabled = !gfx::Tonemapping::autoExposureEnabled();
        gfx::Tonemapping::enableAutoExposure(enabled);
        if (!enabled)
            gfx::Tonemapping::setExposure(defaultExposure);

        log::debug("Auto exposure {}", enabled ? "enabled" : "disabled");
    }
}

void ExampleSystem::onTonemapSwitch(tonemap_action& event)
{
    using namespace legion;
    if (event.released())
    {
        static size_type type = static_cast<size_type>(gfx::tonemapping_type::aces);
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

void ExampleSystem::onSkyboxSwitch(switch_skybox_action& event)
{
    using namespace legion;
    using namespace rendering;
    if (event.released())
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


void ExampleSystem::onExit(L_MAYBEUNUSED lgn::events::exit& event)
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

void ExampleSystem::update(legion::time::span deltaTime)
{
    using namespace legion;

    static float exposure = gfx::Tonemapping::getExposure();
    auto currentExposure = gfx::Tonemapping::getExposure();
    if (math::abs(exposure - currentExposure) > 0.1f)
    {
        log::debug("Current exposure: {:.1f}", currentExposure);
        exposure = currentExposure;
    }

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
                debug::drawLine((worldMat * math::vec4(edge.first.x, edge.first.y, edge.first.z, 1.f)).xyz(), (worldMat * math::vec4(edge.second.x, edge.second.y, edge.second.z, 1.f)).xyz(), math::colors::orange, 10.f);
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
                debug::drawLine((worldMat * math::vec4(edge.first.x, edge.first.y, edge.first.z, 1.f)).xyz(), (worldMat * math::vec4(edge.second.x, edge.second.y, edge.second.z, 1.f)).xyz(), math::colors::green, 10.f);
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
}
