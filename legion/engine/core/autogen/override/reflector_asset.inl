#include "reflector_asset.hpp"
#include "../../../core/data/mesh.hpp"
#include "../../../core/data/image.hpp"

namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<assets::asset<mesh>>(assets::asset<mesh>& obj)
    {
        reflector refl;
        refl.typeId = typeHash<assets::asset<mesh>>();
        refl.typeName = "legion::core::assets::asset<mesh>";
        {
            static const reflectable_attribute reflectable_attr{};
            refl.attributes.push_back(std::cref(reflectable_attr));
        }
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const assets::asset<mesh>>(const assets::asset<mesh>& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<assets::asset<mesh>>();
        refl.typeName = "legion::core::assets::asset<mesh>";
        {
            static const reflectable_attribute reflectable_attr{};
            refl.attributes.push_back(std::cref(reflectable_attr));
        }
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
    template<>
    L_NODISCARD reflector make_reflector<assets::asset<image>>(assets::asset<image>& obj)
    {
        reflector refl;
        refl.typeId = typeHash<assets::asset<image>>();
        refl.typeName = "legion::core::assets::asset<image>";
        {
            static const reflectable_attribute reflectable_attr{};
            refl.attributes.push_back(std::cref(reflectable_attr));
        }
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const assets::asset<image>>(const assets::asset<image>& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<assets::asset<image>>();
        refl.typeName = "legion::core::assets::asset<image>";
        {
            static const reflectable_attribute reflectable_attr{};
            refl.attributes.push_back(std::cref(reflectable_attr));
        }
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
