#include "autogen_reflector_vertexarray.hpp"
#include "../../rendering/data/vertexarray.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::rendering::vertexarray>(legion::rendering::vertexarray& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::rendering::vertexarray>();
        refl.typeName = "legion::rendering::vertexarray";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::rendering::vertexarray>(const legion::rendering::vertexarray& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::rendering::vertexarray>();
        refl.typeName = "legion::rendering::vertexarray";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
