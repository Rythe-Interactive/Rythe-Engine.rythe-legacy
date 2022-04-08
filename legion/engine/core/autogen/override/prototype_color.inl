#include "prototype_color.hpp"
#include "../../../core/math/color.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::core::math::color>(const legion::core::math::color& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::core::math::color>();
        prot.typeName = "legion::core::math::color";
        prot.members.emplace("r", member_value("r", primitive_value{ typeHash<float>(), ::std::any(obj.r) }));
        prot.members.emplace("g", member_value("g", primitive_value{ typeHash<float>(), ::std::any(obj.g) }));
        prot.members.emplace("b", member_value("b", primitive_value{ typeHash<float>(), ::std::any(obj.b) }));
        prot.members.emplace("a", member_value("a", primitive_value{ typeHash<float>(), ::std::any(obj.a) }));
        return prot;
    }
}
