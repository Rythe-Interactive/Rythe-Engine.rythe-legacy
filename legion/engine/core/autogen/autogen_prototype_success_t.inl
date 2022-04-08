#include "autogen_prototype_success_t.hpp"
#include "../../core/common/result.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::core::common::success_t>(const legion::core::common::success_t& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::core::common::success_t>();
        prot.typeName = "legion::core::common::success_t";
        return prot;
    }
}
