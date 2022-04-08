#include "autogen_prototype_this_job.hpp"
#include "../../core/async/job_pool.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::core::async::this_job>(const legion::core::async::this_job& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::core::async::this_job>();
        prot.typeName = "legion::core::async::this_job";
        return prot;
    }
}
