#pragma once
#include <core/math/vector/vector.hpp>

/**
 * @file color.hpp
 */

namespace legion::core::math
{
    /**@class color
     * @brief Color oriented wrapper around float4.
     * @ref legion::core::math::float4
     */
    struct color : public float4
    {
        using float4::vector;

        constexpr color(const float4& other) : float4(other) {}
        constexpr color(float _r, float _g, float _b) : float4(_r, _g, _b, 1.f) {}

        using float4::operator=;
        using float4::operator[];
        using float4::length;
        using float4::length2;
    };

    /**@class dcolor
     * @brief Double precision version of legion::core::math::color.
     * @ref legion::core::math::double4
     */
    struct dcolor : public double4
    {
        using double4::vector;

        constexpr dcolor(const double4& other) : double4(other) {}
        constexpr dcolor(double _r, double _g, double _b) : double4(_r, _g, _b, 1.0) {}

        using double4::operator=;
        using double4::operator[];
        using double4::length;
        using double4::length2;
    };

    /**
     * @brief Default colors in RGBA.
     */
    namespace colors
    {
        const static color black{ 0.f, 0.f, 0.f };
        const static color transparent{ 0.f, 0.f, 0.f, 0.f };
        const static color white{ 1.f, 1.f, 1.f };
        const static color lightgrey{ 0.75f, 0.75f, 0.75f };
        const static color grey{ 0.5f, 0.5f, 0.5f };
        const static color darkgrey{ 0.25f, 0.25f, 0.25f };
        const static color red{ 1.f, 0.f, 0.f };
        const static color green{ 0.f, 1.f, 0.f };
        const static color blue{ 0.f, 0.f, 1.f };
        const static color yellow{ 1.f, 1.f, 0.f };
        const static color cyan{ 0.f, 1.f, 1.f };
        const static color magenta{ 1.f, 0.f, 1.f };
        const static color orange{ 1.0f, 0.584f, 0.f };
        const static color cornflower{ 0.392f, 0.584f, 0.929f, 1.0f };
    }
}
