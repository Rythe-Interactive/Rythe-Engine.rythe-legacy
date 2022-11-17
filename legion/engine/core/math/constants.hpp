#pragma once

namespace legion::core::math {
    namespace detail {

        //note that this is in fact way more digits than even long double can usually handle

        constexpr static long double very_precise_e =       2.71828182845904523536028747135266249775724709369995957496l;
        constexpr static long double very_precise_log2e =   1.69314718055994530941723212145817656807550013436025525412l;
        constexpr static long double very_precise_log10e =  3.30258509299404568401799145468436420760110148862877297603l;

        constexpr static long double very_precise_pi =      3.14159265358979323846264338327950288419716939937510582097l;
        constexpr static long double very_precise_tau =     6.28318530717958647692528676655900576839433879875021164194l;
        constexpr static long double very_precise_sqrt2 =   1.41421356237309504880168872420969807856967187537694807317l;
        constexpr static long double very_precise_sqrt3 =   1.73205080756887729352744634150587236694280525381038062805l;

        constexpr static long double very_precise_egamma =  0.57721566490153286060651209008240243104215933593992359880l;
        constexpr static long double very_precise_phi =     1.61803398874989484820458683436563811772030917980576286213l;

        // I have honestly no idea what you would use this for,
        // but it is a neat constant so have fun with it anyways
        constexpr static long double vp_feigenbaum =        4.66920160910299067185320382046620161725818557747576863274l;
    }

    template <typename T = float>
    constexpr T euler() { return static_cast<T>(detail::very_precise_e); }

    template <typename T = float>
    constexpr T inv_euler() { return T(1) / euler<T>(); }

    template <typename T = float>
    constexpr T log2e() { return static_cast<T>(detail::very_precise_log2e); }

    template <typename T = float>
    constexpr T log10e() { return static_cast<T>(detail::very_precise_log10e); }

    template <typename T = float>
    constexpr T pi() { return static_cast<T>(detail::very_precise_pi); }

    template <typename T = float>
    constexpr T half_pi() { return pi<T>() * T(0.5); }

    template <typename T = float>
    constexpr T quarter_pi() { return pi<T>() * T(0.25); }

    template <typename T = float>
    constexpr T tau() { return static_cast<T>(detail::very_precise_tau); }

    template <typename T = float>
    constexpr T pau() { return pi<T>() * T(1.5); }

    template <typename T = float>
    constexpr T inv_pi() { return T(1) / pi<T>(); }

    template <typename T = float>
    constexpr T sqrt2() { return static_cast<T>(detail::very_precise_sqrt2);}

    template <typename T = float>
    constexpr T inv_sqrt2() { return T(1) / sqrt2<T>(); }

    template <typename T = float>
    constexpr T sqrt3() { return static_cast<T>(detail::very_precise_sqrt3);}

    template <typename T = float>
    constexpr T inv_sqrt3() { return T(1) / sqrt3<T>(); }

    template <typename T = float>
    constexpr T egamma() { return static_cast<T>(detail::very_precise_egamma); }

    template <typename T = float>
    constexpr T phi() { return static_cast<T>(detail::very_precise_phi); }

    template <typename T = float>
    constexpr T feigenbaum() { return static_cast<T>(detail::vp_feigenbaum); }
}
