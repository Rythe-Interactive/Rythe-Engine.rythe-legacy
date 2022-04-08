#include "autogen_prototype_material_data.hpp"
#include "../../core/data/mesh.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::core::material_data>(const legion::core::material_data& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::core::material_data>();
        prot.typeName = "legion::core::material_data";
        {
            static const reflectable_attribute reflectable_attr{};
            prot.attributes.push_back(std::cref(reflectable_attr));
        }
        prot.members.emplace("name", member_value("name", primitive_value{typeHash(obj.name), std::any(obj.name)}));
        prot.members.emplace("transparencyMode", member_value("transparencyMode", primitive_value{typeHash(obj.transparencyMode), std::any(obj.transparencyMode)}));
        prot.members.emplace("alphaCutoff", member_value("alphaCutoff", primitive_value{typeHash(obj.alphaCutoff), std::any(obj.alphaCutoff)}));
        prot.members.emplace("doubleSided", member_value("doubleSided", primitive_value{typeHash(obj.doubleSided), std::any(obj.doubleSided)}));
        prot.members.emplace("albedoValue", member_value("albedoValue", make_prototype(obj.albedoValue)));
        prot.members.emplace("metallicValue", member_value("metallicValue", primitive_value{typeHash(obj.metallicValue), std::any(obj.metallicValue)}));
        prot.members.emplace("roughnessValue", member_value("roughnessValue", primitive_value{typeHash(obj.roughnessValue), std::any(obj.roughnessValue)}));
        prot.members.emplace("emissiveValue", member_value("emissiveValue", make_prototype(obj.emissiveValue)));
        return prot;
    }
}
