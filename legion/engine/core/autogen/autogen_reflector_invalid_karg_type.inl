#include "autogen_reflector_invalid_karg_type.hpp"
#include "../../core/compute/high_level/function.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::core::compute::invalid_karg_type>(legion::core::compute::invalid_karg_type& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::core::compute::invalid_karg_type>();
        refl.typeName = "legion::core::compute::invalid_karg_type";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::core::compute::invalid_karg_type>(const legion::core::compute::invalid_karg_type& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::core::compute::invalid_karg_type>();
        refl.typeName = "legion::core::compute::invalid_karg_type";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
