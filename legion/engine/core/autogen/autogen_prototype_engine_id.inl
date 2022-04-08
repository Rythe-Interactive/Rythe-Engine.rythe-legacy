#include "autogen_prototype_engine_id.hpp"
#include "../../core/engine/engine.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::core::engine_id>(const legion::core::engine_id& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::core::engine_id>();
        prot.typeName = "legion::core::engine_id";
        return prot;
    }
}
