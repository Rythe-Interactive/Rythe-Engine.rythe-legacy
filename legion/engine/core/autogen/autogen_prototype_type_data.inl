#include "autogen_prototype_type_data.hpp"
#include "../../core/common/hash.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::core::detail::type_data>(const legion::core::detail::type_data& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::core::detail::type_data>();
        prot.typeName = "legion::core::detail::type_data";
        return prot;
    }
}
