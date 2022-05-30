#pragma once
#include <core/math/math.hpp>
#include <core/ecs/ecs.hpp>
#include <core/data/mesh.hpp>
#include <core/logging/logging.hpp>


namespace legion::core
{
    struct position : public math::float3
    {
        position(const math::float3& src) : math::float3(src) {}
        position& operator=(const math::float3& src) { math::float3::operator=(src); return *this; }
        position& operator=(math::float3&& src) { math::float3::operator=(src); return *this; }

        using math::float3::vector;
        using math::float3::operator=;
        using math::float3::operator[];
    };

    struct rotation : public math::quat
    {
        rotation(const math::quat& src) : math::quat(src) {}
        rotation& operator=(const math::quat& src) { math::quat::operator=(src); return *this; }
        rotation& operator=(math::quat&& src) { math::quat::operator=(src); return *this; }

        using math::quat::quaternion;
        using math::quat::operator=;
        using math::quat::operator[];

        L_NODISCARD static rotation angle_axis(scalar angle, const vec_type& vec) noexcept { return math::quat::angle_axis(angle, vec); }
        L_NODISCARD static rotation look_at(const vec_type& pos, const vec_type& center, const vec_type& up = vec_type::up) noexcept { return math::quat::look_at(pos, center, up); }
    };

    struct scale : public math::float3
    {
        scale(const math::float3& src) : math::float3(src) {}
        scale& operator=(const math::float3& src) { math::float3::operator=(src); return *this; }
        scale& operator=(math::float3&& src) { math::float3::operator=(src); return *this; }

        using math::float3::vector;
        using math::float3::operator=;
        using math::float3::operator[];
    };

    struct transform : public ecs::archetype<position, rotation, scale>
    {
        using base = ecs::archetype<position, rotation, scale>;
        using base::archetype;

        L_NODISCARD math::float4x4 from_world_matrix()
        {
            return math::inverse(to_world_matrix());
        }

        L_NODISCARD math::float4x4 to_world_matrix()
        {
            OPTICK_EVENT();
            if (owner->parent)
            {
                transform parentTrans = owner->parent.get_component<transform>();
                if (parentTrans)
                    return parentTrans.to_world_matrix() * to_parent_matrix();
            }
            return to_parent_matrix();
        }

        L_NODISCARD math::float4x4 from_parent_matrix()
        {
            return math::inverse(to_parent_matrix());
        }

        L_NODISCARD math::float4x4 to_parent_matrix()
        {
            OPTICK_EVENT();
            auto [position, rotation, scale] = values();
            return math::compose(scale, rotation, position);
        }

    };

    struct velocity : public math::float3
    {
        velocity(const math::float3& src) : math::float3(src) {}
        velocity& operator=(const math::float3& src) { math::float3::operator=(src); return *this; }
        velocity& operator=(math::float3&& src) { math::float3::operator=(src); return *this; }

        using math::float3::vector;
        using math::float3::operator=;
        using math::float3::operator[];
    };

    struct mesh_filter
    {
        assets::asset<mesh> shared_mesh;

        explicit mesh_filter(const assets::asset<mesh>& src) : shared_mesh(src) {}

        RULE_OF_5_NOEXCEPT(mesh_filter);

        bool operator==(const mesh_filter& other) const { return shared_mesh == other.shared_mesh; }
    };
}

#if !defined(DOXY_EXCLUDE)
namespace std // NOLINT(cert-dcl58-cpp)
{
    template <::std::size_t I>
    struct tuple_element<I, legion::core::transform>
    {
        using type = typename legion::core::element_at_t<I, legion::core::position, legion::core::rotation, legion::core::scale>;
    };

    template<>
    struct tuple_size<legion::core::transform>
        : public std::integral_constant<std::size_t, 3>
    {
    };

    template<>
    struct tuple_size<const legion::core::transform>
        : public std::integral_constant<std::size_t, 3>
    {
    };

    template<>
    struct tuple_size<volatile legion::core::transform>
        : public std::integral_constant<std::size_t, 3>
    {
    };

    template<>
    struct tuple_size<const volatile legion::core::transform>
        : public std::integral_constant<std::size_t, 3>
    {
    };

}
#endif

#if !defined(DOXY_EXCLUDE)
namespace fmt
{
    using namespace legion::core;

    template <>
    struct fmt::formatter<position> {
        char presentation = 'f';

        constexpr auto parse(format_parse_context& ctx) {

            auto it = ctx.begin(), end = ctx.end();
            if (it != end && (*it == 'f' || *it == 'e')) presentation = *it++;

            if (it != end && *it != '}')
                throw format_error("invalid format");

            return it;
        }

        template <typename FormatContext>
        auto format(const position& p, FormatContext& ctx) {
            return format_to(
                ctx.out(),

                presentation == 'f' ? "{:f}" : "{:e}",
                static_cast<math::float3>(p));
        }
    };

    template <>
    struct fmt::formatter<velocity> {
        char presentation = 'f';

        constexpr auto parse(format_parse_context& ctx) {

            auto it = ctx.begin(), end = ctx.end();
            if (it != end && (*it == 'f' || *it == 'e')) presentation = *it++;

            if (it != end && *it != '}')
                throw format_error("invalid format");

            return it;
        }

        template <typename FormatContext>
        auto format(const velocity& p, FormatContext& ctx) {
            return format_to(
                ctx.out(),

                presentation == 'f' ? "{:f}" : "{:e}",
                static_cast<math::float3>(p));
        }
    };

    template <>
    struct fmt::formatter<rotation> {
        char presentation = 'f';

        constexpr auto parse(format_parse_context& ctx) {

            auto it = ctx.begin(), end = ctx.end();
            if (it != end && (*it == 'f' || *it == 'e')) presentation = *it++;

            if (it != end && *it != '}')
                throw format_error("invalid format");

            return it;
        }

        template <typename FormatContext>
        auto format(const rotation& r, FormatContext& ctx) {
            return format_to(
                ctx.out(),

                presentation == 'f' ? "{:f}" : "{:e}",
                static_cast<math::quat>(r));
        }
    };

    template <>
    struct fmt::formatter<scale> {
        char presentation = 'f';

        constexpr auto parse(format_parse_context& ctx) {

            auto it = ctx.begin(), end = ctx.end();
            if (it != end && (*it == 'f' || *it == 'e')) presentation = *it++;

            if (it != end && *it != '}')
                throw format_error("invalid format");

            return it;
        }

        template <typename FormatContext>
        auto format(const scale& s, FormatContext& ctx) {
            return format_to(
                ctx.out(),

                presentation == 'f' ? "{:f}" : "{:e}",
                static_cast<math::float3>(s));
        }
    };
}
#endif
