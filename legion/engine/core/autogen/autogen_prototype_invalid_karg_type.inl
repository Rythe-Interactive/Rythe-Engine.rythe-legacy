#include "autogen_prototype_invalid_karg_type.hpp"
#include "../../core/compute/high_level/function.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::core::compute::invalid_karg_type>(const legion::core::compute::invalid_karg_type& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::core::compute::invalid_karg_type>();
        prot.typeName = "legion::core::compute::invalid_karg_type";
        return prot;
    }
}
