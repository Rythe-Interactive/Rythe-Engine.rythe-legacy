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
        prot.members.emplace("windingOrder", member_value("windingOrder", primitive_value{typeHash(obj.windingOrder), std::any(obj.windingOrder)}));
        return prot;
    }
}
