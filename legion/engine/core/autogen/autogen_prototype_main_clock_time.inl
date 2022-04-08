#include "autogen_prototype_main_clock_time.hpp"
#include "../../core/time/main_clock.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::core::time::main_clock_time>(const legion::core::time::main_clock_time& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::core::time::main_clock_time>();
        prot.typeName = "legion::core::time::main_clock_time";
        return prot;
    }
}
