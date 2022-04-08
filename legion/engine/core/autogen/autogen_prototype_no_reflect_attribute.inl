#include "autogen_prototype_no_reflect_attribute.hpp"
#include "../../core/types/attributes.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::core::no_reflect_attribute>(const legion::core::no_reflect_attribute& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::core::no_reflect_attribute>();
        prot.typeName = "legion::core::no_reflect_attribute";
        return prot;
    }
}
