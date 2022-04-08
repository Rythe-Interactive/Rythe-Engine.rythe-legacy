#include "autogen_prototype_inout_ident.hpp"
#include "../../core/compute/high_level/function.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::core::compute::inout_ident>(const legion::core::compute::inout_ident& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::core::compute::inout_ident>();
        prot.typeName = "legion::core::compute::inout_ident";
        return prot;
    }
}
