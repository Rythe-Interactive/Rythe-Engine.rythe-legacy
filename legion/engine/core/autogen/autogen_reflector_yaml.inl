#include "autogen_reflector_yaml.hpp"
#include "../../core/serialization/serializer_views/yaml.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::core::serialization::yaml>(legion::core::serialization::yaml& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::core::serialization::yaml>();
        refl.typeName = "legion::core::serialization::yaml";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::core::serialization::yaml>(const legion::core::serialization::yaml& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::core::serialization::yaml>();
        refl.typeName = "legion::core::serialization::yaml";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
