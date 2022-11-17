#pragma once
#include <core/types/primitives.hpp>
#include <core/math/vector/vector.hpp>
#include <core/math/matrix/matrix.hpp>
#include <core/math/quaternion/quaternion_base.hpp>
#include <core/math/util/meta.hpp>
#include <core/math/basic/constraint.hpp>

namespace legion::core::math
{
    // lerp
    template<typename TypeA, typename TypeB, typename InterpType>
    L_NODISCARD L_ALWAYS_INLINE constexpr auto lerp(TypeA&& a, TypeB&& b, InterpType&& t) noexcept;
}

#include <core/math/interpolation/lerp.inl>
