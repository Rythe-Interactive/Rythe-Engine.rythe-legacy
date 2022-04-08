#include "autogen_reflector_attribute_base.hpp"
#include "../../core/types/attributes.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::core::attribute_base>(legion::core::attribute_base& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::core::attribute_base>();
        refl.typeName = "legion::core::attribute_base";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::core::attribute_base>(const legion::core::attribute_base& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::core::attribute_base>();
        refl.typeName = "legion::core::attribute_base";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
