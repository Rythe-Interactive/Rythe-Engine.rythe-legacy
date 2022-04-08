#pragma once
#include <core/types/reflector.hpp>
#include "../../../core/math/math.hpp"

namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<math::vec2>(math::vec2& obj);
    template<>
    L_NODISCARD const reflector make_reflector<const math::vec2>(const math::vec2& obj);
    template<>
    L_NODISCARD reflector make_reflector<math::vec3>(math::vec3& obj);
    template<>
    L_NODISCARD const reflector make_reflector<const math::vec3>(const math::vec3& obj);
    template<>
    L_NODISCARD reflector make_reflector<math::vec4>(math::vec4& obj);
    template<>
    L_NODISCARD const reflector make_reflector<const math::vec4>(const math::vec4& obj);
    template<>
    L_NODISCARD reflector make_reflector<math::mat4>(math::mat4& obj);
    template<>
    L_NODISCARD const reflector make_reflector<const math::mat4>(const math::mat4& obj);
}
