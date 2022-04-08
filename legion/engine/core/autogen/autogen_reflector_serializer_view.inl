#include "autogen_reflector_serializer_view.hpp"
#include "../../core/serialization/serializer_views/serializer_view.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::core::serialization::serializer_view>(legion::core::serialization::serializer_view& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::core::serialization::serializer_view>();
        refl.typeName = "legion::core::serialization::serializer_view";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::core::serialization::serializer_view>(const legion::core::serialization::serializer_view& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::core::serialization::serializer_view>();
        refl.typeName = "legion::core::serialization::serializer_view";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
