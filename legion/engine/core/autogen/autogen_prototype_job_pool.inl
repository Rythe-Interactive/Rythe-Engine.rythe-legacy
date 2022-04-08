#include "autogen_prototype_job_pool.hpp"
#include "../../core/async/job_pool.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::core::async::job_pool>(const legion::core::async::job_pool& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::core::async::job_pool>();
        prot.typeName = "legion::core::async::job_pool";
        return prot;
    }
}
