#include "autogen_reflector_karg.hpp"
#include "../../core/compute/high_level/function.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::core::compute::karg>(legion::core::compute::karg& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::core::compute::karg>();
        refl.typeName = "legion::core::compute::karg";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::core::compute::karg>(const legion::core::compute::karg& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::core::compute::karg>();
        refl.typeName = "legion::core::compute::karg";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
