#include "autogen_prototype_karg.hpp"
#include "../../core/compute/high_level/function.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::core::compute::karg>(const legion::core::compute::karg& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::core::compute::karg>();
        prot.typeName = "legion::core::compute::karg";
        return prot;
    }
}
