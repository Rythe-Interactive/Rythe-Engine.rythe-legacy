#include "autogen_prototype_async_runnable_base.hpp"
#include "../../core/async/async_runnable.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::core::async::async_runnable_base>(const legion::core::async::async_runnable_base& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::core::async::async_runnable_base>();
        prot.typeName = "legion::core::async::async_runnable_base";
        return prot;
    }
}
