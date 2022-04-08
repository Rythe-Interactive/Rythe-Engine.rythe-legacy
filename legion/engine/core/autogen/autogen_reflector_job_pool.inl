#include "autogen_reflector_job_pool.hpp"
#include "../../core/async/job_pool.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::core::async::job_pool>(legion::core::async::job_pool& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::core::async::job_pool>();
        refl.typeName = "legion::core::async::job_pool";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::core::async::job_pool>(const legion::core::async::job_pool& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::core::async::job_pool>();
        refl.typeName = "legion::core::async::job_pool";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
