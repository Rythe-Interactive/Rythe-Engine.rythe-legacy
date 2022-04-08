#include "autogen_prototype_error_t.hpp"
#include "../../core/common/result.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::core::common::error_t>(const legion::core::common::error_t& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::core::common::error_t>();
        prot.typeName = "legion::core::common::error_t";
        return prot;
    }
}
