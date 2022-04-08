#include "autogen_prototype_buffer_base.hpp"
#include "../../core/compute/high_level/function.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::core::compute::detail::buffer_base>(const legion::core::compute::detail::buffer_base& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::core::compute::detail::buffer_base>();
        prot.typeName = "legion::core::compute::detail::buffer_base";
        return prot;
    }
}
