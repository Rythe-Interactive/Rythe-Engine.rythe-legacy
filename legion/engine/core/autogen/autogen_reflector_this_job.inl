#include "autogen_reflector_this_job.hpp"
#include "../../core/async/job_pool.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::core::async::this_job>(legion::core::async::this_job& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::core::async::this_job>();
        refl.typeName = "legion::core::async::this_job";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::core::async::this_job>(const legion::core::async::this_job& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::core::async::this_job>();
        refl.typeName = "legion::core::async::this_job";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
