#include "autogen_reflector_type_hash.hpp"
#include "../../core/common/hash.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::core::type_hash>(legion::core::type_hash& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::core::type_hash>();
        refl.typeName = "legion::core::type_hash";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::core::type_hash>(const legion::core::type_hash& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::core::type_hash>();
        refl.typeName = "legion::core::type_hash";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
