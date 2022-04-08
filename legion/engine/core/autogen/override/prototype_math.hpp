#pragma once
#include <core/types/prototype.hpp>
#include "../../../core/math/math.hpp"

namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<math::vec2>(const math::vec2& obj);
    template<>
    L_NODISCARD prototype make_prototype<math::vec3>(const math::vec3& obj);
    template<>
    L_NODISCARD prototype make_prototype<math::vec4>(const math::vec4& obj);
    template<>
    L_NODISCARD prototype make_prototype<math::mat4>(const math::mat4& obj);
}
