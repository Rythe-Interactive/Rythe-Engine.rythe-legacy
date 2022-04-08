#include "autogen_reflector_type_data.hpp"
#include "../../core/common/hash.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::core::detail::type_data>(legion::core::detail::type_data& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::core::detail::type_data>();
        refl.typeName = "legion::core::detail::type_data";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::core::detail::type_data>(const legion::core::detail::type_data& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::core::detail::type_data>();
        refl.typeName = "legion::core::detail::type_data";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
