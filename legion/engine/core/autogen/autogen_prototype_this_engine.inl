#include "autogen_prototype_this_engine.hpp"
#include "../../core/engine/engine.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::core::this_engine>(const legion::core::this_engine& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::core::this_engine>();
        prot.typeName = "legion::core::this_engine";
        return prot;
    }
}
