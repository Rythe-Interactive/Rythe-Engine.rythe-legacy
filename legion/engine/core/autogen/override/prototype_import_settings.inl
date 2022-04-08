#include "prototype_import_settings.hpp"
#include "../../../core/data/mesh.hpp"

namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<assets::import_settings<mesh>>(const assets::import_settings<mesh>& obj)
    {
        prototype prot;
        prot.typeId = typeHash<assets::import_settings<mesh>>();
        prot.typeName = "legion::core::assets::import_settings<mesh>";
        {
            static const reflectable_attribute reflectable_attr{};
            prot.attributes.push_back(std::cref(reflectable_attr));
        }

        prot.members.emplace("triangulate", member_value("triangulate", primitive_value{ typeHash(obj.triangulate), std::any(obj.triangulate) }));
        prot.members.emplace("keepNativeCoords", member_value("keepNativeCoords", primitive_value{ typeHash(obj.keepNativeCoords), std::any(obj.keepNativeCoords) }));
        prot.members.emplace("flipVerticalTexcoords", member_value("flipVerticalTexcoords", primitive_value{ typeHash(obj.flipVerticalTexcoords), std::any(obj.flipVerticalTexcoords) }));
        prot.members.emplace("windingOrder", member_value("windingOrder", primitive_value{ typeHash(obj.windingOrder), std::any(obj.windingOrder) }));
        prot.members.emplace("transform", member_value("transform", make_prototype(obj.transform)));
        return prot;
    }
}
