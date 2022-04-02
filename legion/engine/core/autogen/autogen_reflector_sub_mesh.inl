#include "autogen_reflector_sub_mesh.hpp"
#include "../../core/data/mesh.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::core::sub_mesh>(legion::core::sub_mesh& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::core::sub_mesh>();
        refl.typeName = "legion::core::sub_mesh";
        {
            static const reflectable_attribute reflectable_attr{};
            refl.attributes.push_back(std::cref(reflectable_attr));
        }
        refl.members.emplace("name", member_reference("name", make_reflector(obj.name)));
        refl.members.emplace("indexCount", member_reference("indexCount", primitive_reference{typeHash(obj.indexCount), &obj.indexCount}));
        refl.members.emplace("indexOffset", member_reference("indexOffset", primitive_reference{typeHash(obj.indexOffset), &obj.indexOffset}));
        refl.members.emplace("materialIndex", member_reference("materialIndex", primitive_reference{typeHash(obj.materialIndex), &obj.materialIndex}));
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::core::sub_mesh>(const legion::core::sub_mesh& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::core::sub_mesh>();
        refl.typeName = "legion::core::sub_mesh";
        {
            static const reflectable_attribute reflectable_attr{};
            refl.attributes.push_back(std::cref(reflectable_attr));
        }
        refl.members.emplace("name", member_reference("name", make_reflector(obj.name)));
        refl.members.emplace("indexCount", member_reference("indexCount", primitive_reference{typeHash(obj.indexCount), &obj.indexCount}));
        refl.members.emplace("indexOffset", member_reference("indexOffset", primitive_reference{typeHash(obj.indexOffset), &obj.indexOffset}));
        refl.members.emplace("materialIndex", member_reference("materialIndex", primitive_reference{typeHash(obj.materialIndex), &obj.materialIndex}));
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
