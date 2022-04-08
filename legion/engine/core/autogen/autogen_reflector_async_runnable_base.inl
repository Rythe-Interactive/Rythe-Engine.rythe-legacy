#include "autogen_reflector_async_runnable_base.hpp"
#include "../../core/async/async_runnable.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::core::async::async_runnable_base>(legion::core::async::async_runnable_base& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::core::async::async_runnable_base>();
        refl.typeName = "legion::core::async::async_runnable_base";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::core::async::async_runnable_base>(const legion::core::async::async_runnable_base& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::core::async::async_runnable_base>();
        refl.typeName = "legion::core::async::async_runnable_base";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
