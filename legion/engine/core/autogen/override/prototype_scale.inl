#include "prototype_scale.hpp"
#include "../../../core/defaults/defaultcomponents.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::core::scale>(const legion::core::scale& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::core::scale>();
        prot.typeName = "legion::core::scale";
        {
            static const reflectable_attribute reflectable_attr{};
            prot.attributes.push_back(std::cref(reflectable_attr));
        }
        prot.members.emplace("x", member_value("x", primitive_value{ typeHash<float>(), std::make_any<float>(obj.x) }));
        prot.members.emplace("y", member_value("y", primitive_value{ typeHash<float>(), std::make_any<float>(obj.y) }));
        prot.members.emplace("z", member_value("z", primitive_value{ typeHash<float>(), std::make_any<float>(obj.z) }));
        return prot;
    }
}
