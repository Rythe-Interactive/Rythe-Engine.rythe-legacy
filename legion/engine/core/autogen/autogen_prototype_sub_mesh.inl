#include "autogen_prototype_sub_mesh.hpp"
#include "../../core/data/mesh.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::core::sub_mesh>(const legion::core::sub_mesh& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::core::sub_mesh>();
        prot.typeName = "legion::core::sub_mesh";
        {
            static const reflectable_attribute reflectable_attr{};
            prot.attributes.push_back(std::cref(reflectable_attr));
        }
        prot.members.emplace("name", member_value("name", make_prototype(obj.name)));
        prot.members.emplace("indexCount", member_value("indexCount", primitive_value{typeHash(obj.indexCount), std::any(obj.indexCount)}));
        prot.members.emplace("indexOffset", member_value("indexOffset", primitive_value{typeHash(obj.indexOffset), std::any(obj.indexOffset)}));
        prot.members.emplace("materialIndex", member_value("materialIndex", primitive_value{typeHash(obj.materialIndex), std::any(obj.materialIndex)}));
        return prot;
    }
}
