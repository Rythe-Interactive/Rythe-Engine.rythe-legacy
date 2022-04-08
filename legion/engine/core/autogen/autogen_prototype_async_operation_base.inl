#include "autogen_prototype_async_operation_base.hpp"
#include "../../core/async/async_operation.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::core::async::async_operation_base>(const legion::core::async::async_operation_base& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::core::async::async_operation_base>();
        prot.typeName = "legion::core::async::async_operation_base";
        return prot;
    }
}
