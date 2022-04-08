#include "autogen_reflector_ring_sync_lock.hpp"
#include "../../core/async/ring_sync_lock.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::core::async::ring_sync_lock>(legion::core::async::ring_sync_lock& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::core::async::ring_sync_lock>();
        refl.typeName = "legion::core::async::ring_sync_lock";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::core::async::ring_sync_lock>(const legion::core::async::ring_sync_lock& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::core::async::ring_sync_lock>();
        refl.typeName = "legion::core::async::ring_sync_lock";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
