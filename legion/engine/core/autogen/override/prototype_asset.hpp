#pragma once
#include <core/types/prototype.hpp>
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
    L_NODISCARD prototype make_prototype<assets::asset<mesh>>(const assets::asset<mesh>& obj);
    template<>
    L_NODISCARD prototype make_prototype<assets::asset<image>>(const assets::asset<image>& obj);
}
