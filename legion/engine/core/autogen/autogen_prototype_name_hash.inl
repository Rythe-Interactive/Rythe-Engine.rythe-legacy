#include "autogen_prototype_name_hash.hpp"
#include "../../core/common/hash.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::core::name_hash>(const legion::core::name_hash& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::core::name_hash>();
        prot.typeName = "legion::core::name_hash";
        return prot;
    }
}
