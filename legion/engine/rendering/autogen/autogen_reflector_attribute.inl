#include "autogen_reflector_attribute.hpp"
#include "../../rendering/data/shader.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::rendering::attribute>(legion::rendering::attribute& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::rendering::attribute>();
        refl.typeName = "legion::rendering::attribute";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::rendering::attribute>(const legion::rendering::attribute& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::rendering::attribute>();
        refl.typeName = "legion::rendering::attribute";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
