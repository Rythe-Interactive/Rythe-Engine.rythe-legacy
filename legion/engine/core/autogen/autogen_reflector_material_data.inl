#include "autogen_reflector_material_data.hpp"
#include "../../core/data/mesh.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::core::material_data>(legion::core::material_data& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::core::material_data>();
        refl.typeName = "legion::core::material_data";
        {
            static const reflectable_attribute reflectable_attr{};
            refl.attributes.push_back(std::cref(reflectable_attr));
        }
        refl.members.emplace("name", member_reference("name", primitive_reference{typeHash(obj.name), &obj.name}));
        refl.members.emplace("transparencyMode", member_reference("transparencyMode", primitive_reference{typeHash(obj.transparencyMode), &obj.transparencyMode}));
        refl.members.emplace("alphaCutoff", member_reference("alphaCutoff", primitive_reference{typeHash(obj.alphaCutoff), &obj.alphaCutoff}));
        refl.members.emplace("doubleSided", member_reference("doubleSided", primitive_reference{typeHash(obj.doubleSided), &obj.doubleSided}));
        refl.members.emplace("albedoValue", member_reference("albedoValue", make_reflector(obj.albedoValue)));
        refl.members.emplace("metallicValue", member_reference("metallicValue", primitive_reference{typeHash(obj.metallicValue), &obj.metallicValue}));
        refl.members.emplace("roughnessValue", member_reference("roughnessValue", primitive_reference{typeHash(obj.roughnessValue), &obj.roughnessValue}));
        refl.members.emplace("emissiveValue", member_reference("emissiveValue", make_reflector(obj.emissiveValue)));
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::core::material_data>(const legion::core::material_data& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::core::material_data>();
        refl.typeName = "legion::core::material_data";
        {
            static const reflectable_attribute reflectable_attr{};
            refl.attributes.push_back(std::cref(reflectable_attr));
        }
        refl.members.emplace("name", member_reference("name", primitive_reference{typeHash(obj.name), &obj.name}));
        refl.members.emplace("transparencyMode", member_reference("transparencyMode", primitive_reference{typeHash(obj.transparencyMode), &obj.transparencyMode}));
        refl.members.emplace("alphaCutoff", member_reference("alphaCutoff", primitive_reference{typeHash(obj.alphaCutoff), &obj.alphaCutoff}));
        refl.members.emplace("doubleSided", member_reference("doubleSided", primitive_reference{typeHash(obj.doubleSided), &obj.doubleSided}));
        refl.members.emplace("albedoValue", member_reference("albedoValue", make_reflector(obj.albedoValue)));
        refl.members.emplace("metallicValue", member_reference("metallicValue", primitive_reference{typeHash(obj.metallicValue), &obj.metallicValue}));
        refl.members.emplace("roughnessValue", member_reference("roughnessValue", primitive_reference{typeHash(obj.roughnessValue), &obj.roughnessValue}));
        refl.members.emplace("emissiveValue", member_reference("emissiveValue", make_reflector(obj.emissiveValue)));
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
