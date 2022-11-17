#pragma once
#include <core/types/primitives.hpp>
#include <core/math/vector/vector.hpp>
#include <core/math/util/meta.hpp>
#include <core/math/noise/common.hpp>

namespace legion::core::math::noise
{
    // value noise
    template<typename VectorType>
    L_NODISCARD L_ALWAYS_INLINE constexpr auto value(VectorType&& value, int_least<sizeof(vector_scalar_t<VectorType>)> seed = 0) noexcept;

    // value noise
    template<typename VectorType>
    L_NODISCARD L_ALWAYS_INLINE constexpr auto value(VectorType&& value, decay_vector_t<VectorType>& derivative, int_least<sizeof(vector_scalar_t<VectorType>)> seed = 0) noexcept;
}

#include <core/math/noise/value.inl>
