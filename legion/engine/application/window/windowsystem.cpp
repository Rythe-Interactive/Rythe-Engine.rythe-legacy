#include <application/window/windowsystem.hpp>
#include <rendering/debugrendering.hpp>

namespace legion::application
{
    sparse_map<GLFWwindow*, ecs::component<window>> WindowSystem::m_windowComponents;
    async::spinlock WindowSystem::m_creationLock;

    async::spinlock WindowSystem::m_creationRequestLock;
    std::vector<WindowSystem::window_request> WindowSystem::m_creationRequests;

    async::spinlock WindowSystem::m_fullscreenRequestLock;
    std::vector<WindowSystem::fullscreen_toggle_request> WindowSystem::m_fullscreenRequests;

    async::spinlock WindowSystem::m_iconRequestLock;
    std::vector<WindowSystem::icon_request> WindowSystem::m_iconRequests;

    void WindowSystem::closeWindow(GLFWwindow* window)
    {
        if (!ContextHelper::initialized())
            return;

        {
            std::lock_guard guard(m_creationLock); // Lock all creation sensitive data.

            auto handle = m_windowComponents[window];

            async::spinlock* lock = nullptr;
            if (handle.valid())
            {
                raiseEvent<window_close>(m_windowComponents[window]); // Trigger any callbacks that want to know about any windows closing.

                if (!ContextHelper::windowShouldClose(window)) // If a callback canceled the window destruction then we should cancel.
                    return;

                {
                    lock = handle->lock;
                    std::lock_guard guard(*lock); // "deleting" the window is technically writing, so we copy the pointer and use that to lock it.
                    handle.get() = invalid_window; // We mark the window as deleted without deleting it yet. It can cause users to find invalid windows,                                                    
                                                  // but at least they won't use a destroyed component after the lock unlocks.

                    handle.destroy();
                    m_windowComponents.erase(window);
                }

                if (handle.owner == ecs::world)
                {
                    raiseEvent<events::exit>(); // If the current window we're closing is the main window we want to close the application.
                }                                           // (we might want to leave this up to the user at some point.)

                ContextHelper::destroyWindow(window); // After all traces of the window throughout the engine have been erased we actually close the window.
            }
            if (lock)
                delete lock;
        }
    }

    void WindowSystem::onWindowMoved(GLFWwindow* window, int x, int y)
    {
        if (m_windowComponents.contains(window))
            raiseEvent<window_move>(m_windowComponents[window], math::ivec2(x, y));
    }

    void WindowSystem::onWindowResize(GLFWwindow* win, int width, int height)
    {
        if (m_windowComponents.contains(win))
        {
            window& wincomp = m_windowComponents[win];
            wincomp.m_size = math::ivec2(width, height);
            raiseEvent<window_resize>(m_windowComponents[win], wincomp.m_size);
        }
    }

    void WindowSystem::onWindowRefresh(GLFWwindow* window)
    {
        if (m_windowComponents.contains(window))
            raiseEvent<window_refresh>(m_windowComponents[window]);
    }

    void WindowSystem::onWindowFocus(GLFWwindow* window, int focused)
    {
        if (m_windowComponents.contains(window))
            raiseEvent<window_focus>(m_windowComponents[window], focused);
    }

    void WindowSystem::onWindowIconify(GLFWwindow* window, int iconified)
    {
        if (m_windowComponents.contains(window))
            raiseEvent<window_iconified>(m_windowComponents[window], iconified);
    }

    void WindowSystem::onWindowMaximize(GLFWwindow* window, int maximized)
    {
        if (m_windowComponents.contains(window))
            raiseEvent<window_maximized>(m_windowComponents[window], maximized);
    }

    void WindowSystem::onWindowFrameBufferResize(GLFWwindow* window, int width, int height)
    {
        if (m_windowComponents.contains(window))
            raiseEvent<window_framebuffer_resize>(m_windowComponents[window], math::ivec2(width, height));
    }

    void WindowSystem::onWindowContentRescale(GLFWwindow* window, float xscale, float yscale)
    {
        if (m_windowComponents.contains(window))
            raiseEvent<window_content_rescale>(m_windowComponents[window], math::fvec2(xscale, xscale));
    }

    void WindowSystem::onItemDroppedInWindow(GLFWwindow* window, int count, const char** paths)
    {
        if (m_windowComponents.contains(window))
            raiseEvent<window_item_dropped>(m_windowComponents[window], count, paths);
    }

    void WindowSystem::onMouseEnterWindow(GLFWwindow* window, int entered)
    {
        if (m_windowComponents.contains(window))
            raiseEvent<mouse_enter_window>(m_windowComponents[window], entered);
    }

    void WindowSystem::onKeyInput(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if (m_windowComponents.contains(window))
            raiseEvent<key_input>(m_windowComponents[window], key, scancode, action, mods);
    }

    void WindowSystem::onCharInput(GLFWwindow* window, uint codepoint)
    {
        if (m_windowComponents.contains(window))
            raiseEvent<char_input>(m_windowComponents[window], codepoint);
    }

    void WindowSystem::onMouseMoved(GLFWwindow* window, double xpos, double ypos)
    {
        if (m_windowComponents.contains(window))
            raiseEvent<mouse_moved>(m_windowComponents[window], math::dvec2(xpos, ypos) / (math::dvec2)ContextHelper::getFramebufferSize(window));
    }

    void WindowSystem::onMouseButton(GLFWwindow* window, int button, int action, int mods)
    {
        if (m_windowComponents.contains(window))
            raiseEvent<mouse_button>(m_windowComponents[window], button, action, mods);
    }

    void WindowSystem::onMouseScroll(GLFWwindow* window, double xoffset, double yoffset)
    {
        if (m_windowComponents.contains(window))
            raiseEvent<mouse_scrolled>(m_windowComponents[window], math::dvec2(xoffset, yoffset));
    }

    void WindowSystem::onExit(events::exit& event)
    {
        std::lock_guard guard(m_creationLock);
        static ecs::filter<window> windowFilter;

        for (auto entity : windowFilter)
        {
            auto handle = entity.get_component<window>();

            async::spinlock* lock = nullptr;
            if (handle.valid())
            {
                window& win = handle;
                raiseEvent<window_close>(handle); // Trigger any callbacks that want to know about any windows closing.

                {
                    lock = win.lock;
                    std::lock_guard guard(*lock); // "deleting" the window is technically writing, so we copy the pointer and use that to lock it.
                    win = invalid_window; // We mark the window as deleted without deleting it yet. It can cause users to find invalid windows,                                                    
                                                  // but at least they won't use a destroyed component after the lock unlocks.
                    handle.destroy();
                    m_windowComponents.erase(win);
                }

                ContextHelper::destroyWindow(win); // After all traces of the window throughout the engine have been erased we actually close the window.
            }
            if (lock)
                delete lock;
        }

        m_exit = true;
        ContextHelper::terminate();
    }

    bool WindowSystem::windowStillExists(GLFWwindow* win)
    {
        return m_windowComponents.contains(win);
    }

    void WindowSystem::requestIconChange(id_type entityId, assets::asset<image> icon)
    {
        if (entityId)
        {
            std::lock_guard guard(m_iconRequestLock);
            m_iconRequests.emplace_back(entityId, icon);
        }
        else
            log::warn("Icon change denied, invalid entity given.");
    }

    void WindowSystem::requestIconChange(id_type entityId, const std::string& iconName)
    {
        if (entityId)
        {
            std::lock_guard guard(m_iconRequestLock);
            m_iconRequests.emplace_back(entityId, iconName);
        }
        else
            log::warn("Icon change denied, invalid entity given.");
    }

    void WindowSystem::requestFullscreenToggle(id_type entityId, math::ivec2 position, math::ivec2 size)
    {
        if (entityId)
        {
            std::lock_guard guard(m_fullscreenRequestLock);
            m_fullscreenRequests.emplace_back(entityId, position, size);
        }
        else
            log::warn("Fullscreen toggle denied, invalid entity given.");
    }

    void WindowSystem::requestWindow(id_type entityId, math::ivec2 size, const std::string& name, assets::asset<image> icon, GLFWmonitor* monitor, GLFWwindow* share, int swapInterval, const std::vector<std::pair<int, int>>& hints)
    {
        if (entityId)
        {
            std::lock_guard guard(m_creationRequestLock);
            m_creationRequests.emplace_back(entityId, size, name, icon, monitor, share, swapInterval, hints);
        }
        else
            log::warn("Window creation denied, invalid entity given.");
    }

    void WindowSystem::requestWindow(id_type entityId, math::ivec2 size, const std::string& name, assets::asset<image> icon, GLFWmonitor* monitor, GLFWwindow* share, int swapInterval)
    {
        if (entityId)
        {
            std::lock_guard guard(m_creationRequestLock);
            m_creationRequests.emplace_back(entityId, size, name, icon, monitor, share, swapInterval);
        }
        else
            log::warn("Window creation denied, invalid entity given.");
    }

    void WindowSystem::requestWindow(id_type entityId, math::ivec2 size, const std::string& name, const std::string& iconName, GLFWmonitor* monitor, GLFWwindow* share, int swapInterval, const std::vector<std::pair<int, int>>& hints)
    {
        if (entityId)
        {
            std::lock_guard guard(m_creationRequestLock);
            m_creationRequests.emplace_back(entityId, size, name, iconName, monitor, share, swapInterval);
        }
        else
            log::warn("Window creation denied, invalid entity given.");
    }

    void WindowSystem::requestWindow(id_type entityId, math::ivec2 size, const std::string& name, const std::string& iconName, GLFWmonitor* monitor, GLFWwindow* share, int swapInterval)
    {
        if (entityId)
        {
            std::lock_guard guard(m_creationRequestLock);
            m_creationRequests.emplace_back(entityId, size, name, iconName, monitor, share, swapInterval);
        }
        else
            log::warn("Window creation denied, invalid entity given.");
    }

    void WindowSystem::setup()
    {
        using namespace filesystem::literals;

        m_exit = false;
        m_defaultIcon = *assets::load<image>("Legion Icon", "engine://resources/legion/icon"_view, assets::import_settings<image>{ true, true, false });

        bindToEvent<events::exit, &WindowSystem::onExit>();

        if (m_creationRequests.empty() || (std::find_if(m_creationRequests.begin(), m_creationRequests.end(), [](window_request& r) { return r.entityId == ecs::world_entity_id; }) == m_creationRequests.end()))
            requestWindow(ecs::world, math::ivec2(1360, 768), "LEGION Engine", assets::invalid_asset<image>, nullptr, nullptr, 1); // Create the request for the main window.

        if (!ContextHelper::initialized()) // Initialize context.
            if (!ContextHelper::init())
            {
                exit();
                return; // If we can't initialize we can't create any windows, not creating the main window means the engine should shut down.
            }
        log::trace("Creating main window.");
        createWindows();
        showMainWindow();

        createProcess<&WindowSystem::refreshWindows>("Rendering");
        createProcess<&WindowSystem::handleWindowEvents>("Input");
    }

    void WindowSystem::createWindows()
    {
        if (m_exit) // If the engine is exiting then we can't create new windows.
            return;

        std::lock_guard guard(m_creationRequestLock);
        for (auto& request : m_creationRequests)
        {
            if (!ecs::Registry::checkEntity(request.entityId))
            {
                log::warn("Window creation denied, entity {} doesn't exist.", request.entityId);
                continue;
            }

            log::trace("creating a window");

            if (request.hints.size())
            {
                for (auto& [hint, value] : request.hints)
                    ContextHelper::windowHint(hint, value);
            }
            else // Default window hints.
            {
                ContextHelper::windowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
                ContextHelper::windowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
                ContextHelper::windowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_FALSE);
                ContextHelper::windowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

                GLFWmonitor* monitor = request.monitor;

                if (!request.monitor)
                    monitor = ContextHelper::getPrimaryMonitor();

                const GLFWvidmode* mode = ContextHelper::getVideoMode(monitor);

                ContextHelper::windowHint(GLFW_RED_BITS, mode->redBits);
                ContextHelper::windowHint(GLFW_GREEN_BITS, mode->greenBits);
                ContextHelper::windowHint(GLFW_BLUE_BITS, mode->blueBits);
                ContextHelper::windowHint(GLFW_REFRESH_RATE, mode->refreshRate);
            }

            if (request.size == math::ivec2(0, 0))
                request.size = { 400, 400 };

            if (request.name.empty())
                request.name = "LEGION Engine";

            assets::asset<image> icon = request.icon;
            if (icon == assets::invalid_asset<image>)
                icon = m_defaultIcon;

            ContextHelper::windowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

            window win = ContextHelper::createWindow(request.size, request.name.c_str(), request.monitor, request.share);

            if (icon->components() == image_components::rgba && icon->format() == channel_format::eight_bit)
            {
                GLFWimage iconImg{ icon->resolution().x, icon->resolution().y, icon->data() };

                ContextHelper::setWindowIcon(win, 1, &iconImg);
            }

            win.m_title = request.name;
            win.m_isFullscreen = (request.monitor != nullptr);
            win.m_swapInterval = request.swapInterval;
            win.m_size = request.size;

            auto setCallbacks = [](const window& win)
            {
                ContextHelper::makeContextCurrent(win);
                ContextHelper::swapInterval(win.m_swapInterval);
                ContextHelper::setWindowCloseCallback(win, &WindowSystem::closeWindow);
                ContextHelper::setWindowPosCallback(win, &WindowSystem::onWindowMoved);
                ContextHelper::setWindowSizeCallback(win, &WindowSystem::onWindowResize);
                ContextHelper::setWindowRefreshCallback(win, &WindowSystem::onWindowRefresh);
                ContextHelper::setWindowFocusCallback(win, &WindowSystem::onWindowFocus);
                ContextHelper::setWindowIconifyCallback(win, &WindowSystem::onWindowIconify);
                ContextHelper::setWindowMaximizeCallback(win, &WindowSystem::onWindowMaximize);
                ContextHelper::setFramebufferSizeCallback(win, &WindowSystem::onWindowFrameBufferResize);
                ContextHelper::setWindowContentScaleCallback(win, &WindowSystem::onWindowContentRescale);
                ContextHelper::setDropCallback(win, &WindowSystem::onItemDroppedInWindow);
                ContextHelper::setCursorEnterCallback(win, &WindowSystem::onMouseEnterWindow);
                ContextHelper::setKeyCallback(win, &WindowSystem::onKeyInput);
                ContextHelper::setCharCallback(win, &WindowSystem::onCharInput);
                ContextHelper::setCursorPosCallback(win, &WindowSystem::onMouseMoved);
                ContextHelper::setMouseButtonCallback(win, &WindowSystem::onMouseButton);
                ContextHelper::setScrollCallback(win, &WindowSystem::onMouseScroll);
                ContextHelper::makeContextCurrent(nullptr);
            };

            ecs::entity ent = ecs::Registry::getEntity(request.entityId);
            ecs::component<window> handle = ent.get_component<window>();
            if (!handle)
            {
                win.lock = new async::spinlock();

                std::lock_guard wguard(*win.lock);     // This is the only code that has access to win.lock right now, so there's no deadlock risk.
                std::lock_guard cguard(m_creationLock);// Locking them both separately is faster than using a multilock.
                m_windowComponents.insert(win, handle);
                handle = ent.add_component<window>(win);

                log::trace("created window: {}", request.name);


                // Set all callbacks.
                setCallbacks(win);
            }
            else
            {
                window& oldWindow = handle;

                std::scoped_lock wguard(*oldWindow.lock, m_creationLock);

                m_windowComponents.erase(oldWindow);
                ContextHelper::destroyWindow(oldWindow);

                win.lock = oldWindow.lock;
                handle.get() = win;
                m_windowComponents.insert(win, handle);

                log::trace("replaced window: {}", request.name);


                // Set all callbacks.
                setCallbacks(win);
            }
        }

        m_creationRequests.clear();
    }

    void WindowSystem::fullscreenWindows()
    {
        if (m_exit) // If the engine is exiting then we can't change any windows.
            return;

        std::lock_guard guard(m_fullscreenRequestLock);
        for (auto& request : m_fullscreenRequests)
        {
            if (!ecs::Registry::checkEntity(request.entityId))
            {
                log::warn("Fullscreen toggle denied, entity {} doesn't exist.", request.entityId);
                continue;
            }

            auto ent = ecs::Registry::getEntity(request.entityId);
            auto handle = ent.get_component<window>();
            if (!handle)
            {
                log::warn("Fullscreen toggle denied, entity {} doesn't have a window.", request.entityId);
                continue;
            }

            window& win = handle;
            std::lock_guard wguard(*win.lock);

            if (win.m_isFullscreen)
            {
                GLFWmonitor* monitor = ContextHelper::getPrimaryMonitor();
                const GLFWvidmode* mode = ContextHelper::getVideoMode(monitor);

                ContextHelper::setWindowMonitor(win, nullptr, request.position, request.size, mode->refreshRate);
                win.m_size = request.size;
                log::trace("Set window {} to windowed.", request.entityId);
            }
            else
            {
                GLFWmonitor* monitor = ContextHelper::getCurrentMonitor(win);
                const GLFWvidmode* mode = ContextHelper::getVideoMode(monitor);

                ContextHelper::setWindowMonitor(win, monitor, { 0 ,0 }, math::ivec2(mode->width, mode->height), mode->refreshRate);
                win.m_size = math::ivec2(mode->width, mode->height);
                ContextHelper::makeContextCurrent(win);
                ContextHelper::swapInterval(win.m_swapInterval);
                ContextHelper::makeContextCurrent(nullptr);
                log::trace("Set window {} to fullscreen.", request.entityId);
            }

            win.m_isFullscreen = !win.m_isFullscreen;
        }
        m_fullscreenRequests.clear();
    }

    void WindowSystem::updateWindowIcons()
    {
        if (m_exit) // If the engine is exiting then we can't change any windows.
            return;

        std::lock_guard guard(m_iconRequestLock);
        for (auto& request : m_iconRequests)
        {

            if (!ecs::Registry::checkEntity(request.entityId))
            {
                log::warn("Icon change denied, entity {} doesn't exist.", request.entityId);
                continue;
            }

            auto ent = ecs::Registry::getEntity(request.entityId);
            auto handle = ent.get_component<window>();
            if (!handle)
            {
                log::warn("Icon change denied, entity {} doesn't have a window.", request.entityId);
                continue;
            }

            if (request.icon->components() != image_components::rgba || request.icon->format() != channel_format::eight_bit)
            {
                log::warn("Icon change denied, image {} has the wrong format. The needed format is 4 channels with 8 bits per channel.", request.icon.name());
                continue;
            }

            GLFWimage icon{ request.icon->resolution().x, request.icon->resolution().y, request.icon->data() };

            ContextHelper::setWindowIcon(handle->handle, 1, &icon);
        }
        m_iconRequests.clear();
    }

    void WindowSystem::refreshWindows(time::time_span<fast_time> deltaTime)
    {
        if (!ContextHelper::initialized())
            return;

        std::lock_guard guard(m_creationLock);
        static ecs::filter<window> windowFilter;
        for (auto entity : windowFilter)
        {
            window& win = entity.get_component<window>();
            {
                context_guard guard(win);
                if (!guard.contextIsValid())
                    continue;
                ContextHelper::swapBuffers(win);
            }
        }
    }

    void WindowSystem::handleWindowEvents(time::time_span<fast_time> deltaTime)
    {
        createWindows();
        updateWindowIcons();
        fullscreenWindows();

        if (!ContextHelper::initialized())
            return;

        ContextHelper::pollEvents();
        ContextHelper::updateWindowFocus();
    }

}
