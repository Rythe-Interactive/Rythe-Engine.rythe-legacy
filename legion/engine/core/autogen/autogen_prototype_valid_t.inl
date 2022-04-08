#include "autogen_prototype_valid_t.hpp"
#include "../../core/common/result.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::core::common::valid_t>(const legion::core::common::valid_t& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::core::common::valid_t>();
        prot.typeName = "legion::core::common::valid_t";
        return prot;
    }
}
