#include "autogen_prototype_attribute_base.hpp"
#include "../../core/types/attributes.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::core::attribute_base>(const legion::core::attribute_base& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::core::attribute_base>();
        prot.typeName = "legion::core::attribute_base";
        return prot;
    }
}
