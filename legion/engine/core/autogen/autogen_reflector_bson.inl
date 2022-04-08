#include "autogen_reflector_bson.hpp"
#include "../../core/serialization/serializer_views/bson.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::core::serialization::bson>(legion::core::serialization::bson& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::core::serialization::bson>();
        refl.typeName = "legion::core::serialization::bson";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::core::serialization::bson>(const legion::core::serialization::bson& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::core::serialization::bson>();
        refl.typeName = "legion::core::serialization::bson";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
