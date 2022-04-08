#include "autogen_reflector_serializer_base.hpp"
#include "../../core/serialization/serializer/serializer_base.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::core::serialization::serializer_base>(legion::core::serialization::serializer_base& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::core::serialization::serializer_base>();
        refl.typeName = "legion::core::serialization::serializer_base";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::core::serialization::serializer_base>(const legion::core::serialization::serializer_base& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::core::serialization::serializer_base>();
        refl.typeName = "legion::core::serialization::serializer_base";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
