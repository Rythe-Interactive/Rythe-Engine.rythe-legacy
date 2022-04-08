#include "autogen_reflector_buffer_base.hpp"
#include "../../core/compute/high_level/function.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::core::compute::detail::buffer_base>(legion::core::compute::detail::buffer_base& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::core::compute::detail::buffer_base>();
        refl.typeName = "legion::core::compute::detail::buffer_base";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::core::compute::detail::buffer_base>(const legion::core::compute::detail::buffer_base& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::core::compute::detail::buffer_base>();
        refl.typeName = "legion::core::compute::detail::buffer_base";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
