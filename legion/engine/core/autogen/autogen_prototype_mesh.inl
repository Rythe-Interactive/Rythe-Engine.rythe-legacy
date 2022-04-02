#include "autogen_prototype_mesh.hpp"
#include "../../core/data/mesh.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::core::mesh>(const legion::core::mesh& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::core::mesh>();
        prot.typeName = "legion::core::mesh";
        {
            static const reflectable_attribute reflectable_attr{};
            prot.attributes.push_back(std::cref(reflectable_attr));
        }
        prot.members.emplace("vertices", member_value("vertices", make_prototype(obj.vertices)));
        prot.members.emplace("colors", member_value("colors", make_prototype(obj.colors)));
        prot.members.emplace("normals", member_value("normals", make_prototype(obj.normals)));
        prot.members.emplace("uvs", member_value("uvs", make_prototype(obj.uvs)));
        prot.members.emplace("tangents", member_value("tangents", make_prototype(obj.tangents)));
        prot.members.emplace("indices", member_value("indices", make_prototype(obj.indices)));
        prot.members.emplace("materials", member_value("materials", make_prototype(obj.materials)));
        prot.members.emplace("windingOrder", member_value("windingOrder", primitive_value{typeHash(obj.windingOrder), std::any(obj.windingOrder)}));
        prot.members.emplace("submeshes", member_value("submeshes", make_prototype(obj.submeshes)));
        return prot;
    }
}
