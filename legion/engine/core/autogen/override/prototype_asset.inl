#include "prototype_asset.hpp"
#include "../../../core/data/mesh.hpp"
#include "../../../core/data/image.hpp"

namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<assets::asset<mesh>>(const assets::asset<mesh>& obj)
    {
        prototype prot;
        prot.typeId = typeHash<assets::asset<mesh>>();
        prot.typeName = "legion::core::assets::asset<mesh>";
        {
            static const reflectable_attribute reflectable_attr{};
            prot.attributes.push_back(std::cref(reflectable_attr));
        }
        return prot;
    }
    template<>
    L_NODISCARD prototype make_prototype<assets::asset<image>>(const assets::asset<image>& obj)
    {
        prototype prot;
        prot.typeId = typeHash<assets::asset<image>>();
        prot.typeName = "legion::core::assets::asset<image>";
        {
            static const reflectable_attribute reflectable_attr{};
            prot.attributes.push_back(std::cref(reflectable_attr));
        }
        return prot;
    }
}
