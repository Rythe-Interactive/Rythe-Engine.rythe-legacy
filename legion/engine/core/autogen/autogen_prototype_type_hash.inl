#include "autogen_prototype_type_hash.hpp"
#include "../../core/common/hash.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::core::type_hash>(const legion::core::type_hash& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::core::type_hash>();
        prot.typeName = "legion::core::type_hash";
        return prot;
    }
}
