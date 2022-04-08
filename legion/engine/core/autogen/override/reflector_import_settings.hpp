#pragma once
#include <core/types/reflector.hpp>
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
    L_NODISCARD reflector make_reflector<assets::import_settings<mesh>>(assets::import_settings<mesh>& obj);
    template<>
    L_NODISCARD const reflector make_reflector<const assets::import_settings<mesh>>(const assets::import_settings<mesh>& obj);
}
