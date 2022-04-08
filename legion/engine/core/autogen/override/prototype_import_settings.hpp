#pragma once
#include <core/types/prototype.hpp>
namespace legion::core
{
    struct mesh;
    namespace assets
    {
        template<typename T>
        struct import_settings;
    }
}
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<assets::import_settings<mesh>>(const assets::import_settings<mesh>& obj);
}
