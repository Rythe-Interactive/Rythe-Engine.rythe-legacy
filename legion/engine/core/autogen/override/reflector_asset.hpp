#pragma once
#include <core/types/reflector.hpp>
namespace legion::core
{
    struct mesh;
    struct image;
    namespace assets
    {
        template<typename T>
        struct asset;
    }
}
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<assets::asset<mesh>>(assets::asset<mesh>& obj);
    template<>
    L_NODISCARD const reflector make_reflector<const assets::asset<mesh>>(const assets::asset<mesh>& obj);
    template<>
    L_NODISCARD reflector make_reflector<assets::asset<image>>(assets::asset<image>& obj);
    template<>
    L_NODISCARD const reflector make_reflector<const assets::asset<image>>(const assets::asset<image>& obj);
}
