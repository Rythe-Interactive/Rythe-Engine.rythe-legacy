#include "autogen_prototype_range_attribute.hpp"
#include "../../core/types/attributes.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::core::range_attribute>(const legion::core::range_attribute& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::core::range_attribute>();
        prot.typeName = "legion::core::range_attribute";
        return prot;
    }
}
