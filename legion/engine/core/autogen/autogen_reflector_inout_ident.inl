#include "autogen_reflector_inout_ident.hpp"
#include "../../core/compute/high_level/function.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::core::compute::inout_ident>(legion::core::compute::inout_ident& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::core::compute::inout_ident>();
        refl.typeName = "legion::core::compute::inout_ident";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::core::compute::inout_ident>(const legion::core::compute::inout_ident& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::core::compute::inout_ident>();
        refl.typeName = "legion::core::compute::inout_ident";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
