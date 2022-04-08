#include "autogen_prototype_ring_sync_lock.hpp"
#include "../../core/async/ring_sync_lock.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::core::async::ring_sync_lock>(const legion::core::async::ring_sync_lock& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::core::async::ring_sync_lock>();
        prot.typeName = "legion::core::async::ring_sync_lock";
        return prot;
    }
}
