#pragma once
#include <core/types/primitives.hpp>
#include <core/math/vector/vector.hpp>
#include <core/math/util/meta.hpp>
#include <core/math/interpolation/interpolation.hpp>
#include <core/math/basic/basic_funcs.hpp>

namespace legion::core::math
{
    // perlin
    template<typename VectorType>
    L_NODISCARD L_ALWAYS_INLINE constexpr auto perlin(VectorType&& value) noexcept;

    // perlin
    template<typename VectorType>
    L_NODISCARD L_ALWAYS_INLINE constexpr auto perlin(VectorType&& value, decay_vector_t<VectorType>& derivative) noexcept;
}

#include <core/math/noise/perlin.inl>
