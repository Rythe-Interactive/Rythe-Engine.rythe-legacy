#include "autogen_reflector_out_ident.hpp"
#include "../../core/compute/high_level/function.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::core::compute::out_ident>(legion::core::compute::out_ident& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::core::compute::out_ident>();
        refl.typeName = "legion::core::compute::out_ident";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::core::compute::out_ident>(const legion::core::compute::out_ident& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::core::compute::out_ident>();
        refl.typeName = "legion::core::compute::out_ident";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
