#include "autogen_reflector_isChars.hpp"
#include "../../core/common/string_extra.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::core::common::isChars>(legion::core::common::isChars& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::core::common::isChars>();
        refl.typeName = "legion::core::common::isChars";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::core::common::isChars>(const legion::core::common::isChars& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::core::common::isChars>();
        refl.typeName = "legion::core::common::isChars";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
