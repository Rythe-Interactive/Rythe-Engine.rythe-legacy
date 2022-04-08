#include "autogen_reflector_reflectable_attribute.hpp"
#include "../../core/types/attributes.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::core::reflectable_attribute>(legion::core::reflectable_attribute& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::core::reflectable_attribute>();
        refl.typeName = "legion::core::reflectable_attribute";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::core::reflectable_attribute>(const legion::core::reflectable_attribute& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::core::reflectable_attribute>();
        refl.typeName = "legion::core::reflectable_attribute";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
