#include "reflector_import_settings.hpp"
#include "../../../core/data/mesh.hpp"

namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<assets::import_settings<mesh>>(assets::import_settings<mesh>& obj)
    {
        reflector refl;
        refl.typeId = typeHash<assets::import_settings<mesh>>();
        refl.typeName = "legion::core::assets::import_settings<mesh>";
        {
            static const reflectable_attribute reflectable_attr{};
            refl.attributes.push_back(std::cref(reflectable_attr));
        }
        refl.members.emplace("triangulate", member_reference("triangulate", primitive_reference{ typeHash(obj.triangulate), &obj.triangulate }));
        refl.members.emplace("keepNativeCoords", member_reference("keepNativeCoords", primitive_reference{ typeHash(obj.keepNativeCoords), &obj.keepNativeCoords }));
        refl.members.emplace("flipVerticalTexcoords", member_reference("flipVerticalTexcoords", primitive_reference{ typeHash(obj.flipVerticalTexcoords), &obj.flipVerticalTexcoords }));
        refl.members.emplace("windingOrder", member_reference("windingOrder", primitive_reference{ typeHash(obj.windingOrder), &obj.windingOrder }));
        refl.members.emplace("transform", member_reference("transform", make_reflector(obj.transform)));
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const assets::import_settings<mesh>>(const assets::import_settings<mesh>& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<assets::import_settings<mesh>>();
        refl.typeName = "legion::core::assets::import_settings<mesh>";
        {
            static const reflectable_attribute reflectable_attr{};
            refl.attributes.push_back(std::cref(reflectable_attr));
        }
        refl.members.emplace("triangulate", member_reference("triangulate", primitive_reference{ typeHash(obj.triangulate), &obj.triangulate }));
        refl.members.emplace("keepNativeCoords", member_reference("keepNativeCoords", primitive_reference{ typeHash(obj.keepNativeCoords), &obj.keepNativeCoords }));
        refl.members.emplace("flipVerticalTexcoords", member_reference("flipVerticalTexcoords", primitive_reference{ typeHash(obj.flipVerticalTexcoords), &obj.flipVerticalTexcoords }));
        refl.members.emplace("windingOrder", member_reference("windingOrder", primitive_reference{ typeHash(obj.windingOrder), &obj.windingOrder }));
        refl.members.emplace("transform", member_reference("transform", make_reflector(obj.transform)));
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
