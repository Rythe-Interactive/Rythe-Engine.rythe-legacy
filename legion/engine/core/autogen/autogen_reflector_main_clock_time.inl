#include "autogen_reflector_main_clock_time.hpp"
#include "../../core/time/main_clock.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::core::time::main_clock_time>(legion::core::time::main_clock_time& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::core::time::main_clock_time>();
        refl.typeName = "legion::core::time::main_clock_time";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::core::time::main_clock_time>(const legion::core::time::main_clock_time& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::core::time::main_clock_time>();
        refl.typeName = "legion::core::time::main_clock_time";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
