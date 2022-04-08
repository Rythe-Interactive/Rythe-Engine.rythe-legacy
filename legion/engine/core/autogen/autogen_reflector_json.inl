#include "autogen_reflector_json.hpp"
#include "../../core/serialization/serializer_views/json.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::core::serialization::json>(legion::core::serialization::json& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::core::serialization::json>();
        refl.typeName = "legion::core::serialization::json";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::core::serialization::json>(const legion::core::serialization::json& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::core::serialization::json>();
        refl.typeName = "legion::core::serialization::json";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
