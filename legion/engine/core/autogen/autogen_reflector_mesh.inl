#include "autogen_reflector_mesh.hpp"
#include "../../core/data/mesh.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::core::mesh>(legion::core::mesh& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::core::mesh>();
        refl.typeName = "legion::core::mesh";
        {
            static const reflectable_attribute reflectable_attr{};
            refl.attributes.push_back(std::cref(reflectable_attr));
        }
        refl.members.emplace("vertices", member_reference("vertices", make_reflector(obj.vertices)));
        refl.members.emplace("colors", member_reference("colors", make_reflector(obj.colors)));
        refl.members.emplace("normals", member_reference("normals", make_reflector(obj.normals)));
        refl.members.emplace("uvs", member_reference("uvs", make_reflector(obj.uvs)));
        refl.members.emplace("tangents", member_reference("tangents", make_reflector(obj.tangents)));
        refl.members.emplace("indices", member_reference("indices", make_reflector(obj.indices)));
        refl.members.emplace("materials", member_reference("materials", make_reflector(obj.materials)));
        refl.members.emplace("windingOrder", member_reference("windingOrder", primitive_reference{typeHash(obj.windingOrder), &obj.windingOrder}));
        refl.members.emplace("submeshes", member_reference("submeshes", make_reflector(obj.submeshes)));
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::core::mesh>(const legion::core::mesh& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::core::mesh>();
        refl.typeName = "legion::core::mesh";
        {
            static const reflectable_attribute reflectable_attr{};
            refl.attributes.push_back(std::cref(reflectable_attr));
        }
        refl.members.emplace("vertices", member_reference("vertices", make_reflector(obj.vertices)));
        refl.members.emplace("colors", member_reference("colors", make_reflector(obj.colors)));
        refl.members.emplace("normals", member_reference("normals", make_reflector(obj.normals)));
        refl.members.emplace("uvs", member_reference("uvs", make_reflector(obj.uvs)));
        refl.members.emplace("tangents", member_reference("tangents", make_reflector(obj.tangents)));
        refl.members.emplace("indices", member_reference("indices", make_reflector(obj.indices)));
        refl.members.emplace("materials", member_reference("materials", make_reflector(obj.materials)));
        refl.members.emplace("windingOrder", member_reference("windingOrder", primitive_reference{typeHash(obj.windingOrder), &obj.windingOrder}));
        refl.members.emplace("submeshes", member_reference("submeshes", make_reflector(obj.submeshes)));
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
