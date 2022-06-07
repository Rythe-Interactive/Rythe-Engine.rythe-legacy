#pragma once
#include <core/events/event.hpp>
#include <core/math/math.hpp>

namespace legion::debug
{
#if !defined drawLine

#define debug_line_event CONCAT(PROJECT_NAME, debug_line)

    struct debug_line_event final : public core::events::event_base
    {
        core::math::vec3 start;
        core::math::vec3 end;
        core::math::color color = core::math::colors::white;
        float width = 1.f;
        float time = 0;
        mutable float timeBuffer = 0;
        bool ignoreDepth = false;

        debug_line_event(core::math::vec3 start, core::math::vec3 end, core::math::color color = core::math::colors::white, float width = 1.f, float time = 0, bool ignoreDepth = false) : start(start), end(end), color(color), width(width), time(time), ignoreDepth(ignoreDepth) {}
        debug_line_event() = default;
        debug_line_event(const debug_line_event&) = default;
        debug_line_event(debug_line_event&&) = default;

        debug_line_event& operator=(const debug_line_event&) = default;
        debug_line_event& operator=(debug_line_event&&) = default;

        bool operator==(const debug_line_event& other) const
        {
            return start == other.start && end == other.end && color == other.color && width == other.width && ignoreDepth == other.ignoreDepth;
        }

        virtual core::id_type get_id()
        {
            static core::id_type id = core::nameHash("debug_line");
            return id;
        }
    };

#define drawLine CONCAT(PROJECT_NAME, DrawLine)

    inline void drawLine(core::math::vec3 start, core::math::vec3 end, core::math::color color = core::math::colors::white, float width = 1.f, float time = 0, bool ignoreDepth = false)
    {
        auto val = debug_line_event(start, end, color, width, time, ignoreDepth);
        core::events::EventBus::raiseEvent(val);
    }

#define drawCube CONCAT(PROJECT_NAME, DrawCube)

    inline void drawCube(core::math::vec3 min, core::math::vec3 max, core::math::color color = core::math::colors::white, float width = 1.f, float time = 0, bool ignoreDepth = false)
    {
        //draws all 12 cube edges 
        drawLine(min, core::math::vec3(max.x, min.y, min.z), color, width, time, ignoreDepth);
        drawLine(min, core::math::vec3(min.x, max.y, min.z), color, width, time, ignoreDepth);
        drawLine(min, core::math::vec3(min.x, min.y, max.z), color, width, time, ignoreDepth);
        drawLine(core::math::vec3(min.x, max.y, max.z), max, color, width, time, ignoreDepth);
        drawLine(core::math::vec3(max.x, max.y, min.z), max, color, width, time, ignoreDepth);
        drawLine(core::math::vec3(max.x, min.y, max.z), max, color, width, time, ignoreDepth);
        drawLine(core::math::vec3(max.x, min.y, min.z), core::math::vec3(max.x, max.y, min.z), color, width, time, ignoreDepth);
        drawLine(core::math::vec3(max.x, min.y, min.z), core::math::vec3(max.x, min.y, max.z), color, width, time, ignoreDepth);
        drawLine(core::math::vec3(min.x, max.y, min.z), core::math::vec3(max.x, max.y, min.z), color, width, time, ignoreDepth);
        drawLine(core::math::vec3(min.x, max.y, min.z), core::math::vec3(min.x, max.y, max.z), color, width, time, ignoreDepth);
        drawLine(core::math::vec3(min.x, min.y, max.z), core::math::vec3(max.x, min.y, max.z), color, width, time, ignoreDepth);
        drawLine(core::math::vec3(min.x, min.y, max.z), core::math::vec3(min.x, max.y, max.z), color, width, time, ignoreDepth);
    }

#endif


}

#if !defined(DOXY_EXCLUDE)
namespace std
{
    template<>
    struct hash<legion::debug::debug_line_event>
    {
        std::size_t operator()(legion::debug::debug_line_event const& line) const noexcept
        {
            std::hash<legion::core::math::vec3> vecHasher;
            std::hash<legion::core::math::color> colHasher;
            std::hash<float> fltHasher;
            std::hash<bool> boolHasher;

            size_t seed = 0;
            legion::core::math::detail::hash_combine(seed, vecHasher(line.start));
            legion::core::math::detail::hash_combine(seed, vecHasher(line.end));
            legion::core::math::detail::hash_combine(seed, colHasher(line.color));
            legion::core::math::detail::hash_combine(seed, fltHasher(line.width));
            legion::core::math::detail::hash_combine(seed, boolHasher(line.ignoreDepth));
            return seed;
        }
    };
}
#endif
