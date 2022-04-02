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
        prot.members.emplace("name", member_value("name", make_prototype(obj.name)));
        prot.members.emplace("transparencyMode", member_value("transparencyMode", primitive_value{typeHash(obj.transparencyMode), std::any(obj.transparencyMode)}));
        prot.members.emplace("alphaCutoff", member_value("alphaCutoff", primitive_value{typeHash(obj.alphaCutoff), std::any(obj.alphaCutoff)}));
        prot.members.emplace("doubleSided", member_value("doubleSided", primitive_value{typeHash(obj.doubleSided), std::any(obj.doubleSided)}));
        prot.members.emplace("albedoValue", member_value("albedoValue", make_prototype(obj.albedoValue)));
        prot.members.emplace("albedoMap", member_value("albedoMap", make_prototype(obj.albedoMap)));
        prot.members.emplace("metallicValue", member_value("metallicValue", primitive_value{typeHash(obj.metallicValue), std::any(obj.metallicValue)}));
        prot.members.emplace("metallicMap", member_value("metallicMap", make_prototype(obj.metallicMap)));
        prot.members.emplace("roughnessValue", member_value("roughnessValue", primitive_value{typeHash(obj.roughnessValue), std::any(obj.roughnessValue)}));
        prot.members.emplace("roughnessMap", member_value("roughnessMap", make_prototype(obj.roughnessMap)));
        prot.members.emplace("metallicRoughnessMap", member_value("metallicRoughnessMap", make_prototype(obj.metallicRoughnessMap)));
        prot.members.emplace("emissiveValue", member_value("emissiveValue", make_prototype(obj.emissiveValue)));
        prot.members.emplace("emissiveMap", member_value("emissiveMap", make_prototype(obj.emissiveMap)));
        prot.members.emplace("normalMap", member_value("normalMap", make_prototype(obj.normalMap)));
        prot.members.emplace("aoMap", member_value("aoMap", make_prototype(obj.aoMap)));
        prot.members.emplace("heightMap", member_value("heightMap", make_prototype(obj.heightMap)));
        return prot;
    }
}
