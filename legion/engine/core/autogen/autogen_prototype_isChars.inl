#include "autogen_prototype_isChars.hpp"
#include "../../core/common/string_extra.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::core::common::isChars>(const legion::core::common::isChars& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::core::common::isChars>();
        prot.typeName = "legion::core::common::isChars";
        return prot;
    }
}
