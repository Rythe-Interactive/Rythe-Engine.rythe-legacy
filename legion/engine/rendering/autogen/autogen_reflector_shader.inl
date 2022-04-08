#include "autogen_reflector_shader.hpp"
#include "../../rendering/data/shader.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::rendering::shader>(legion::rendering::shader& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::rendering::shader>();
        refl.typeName = "legion::rendering::shader";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::rendering::shader>(const legion::rendering::shader& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::rendering::shader>();
        refl.typeName = "legion::rendering::shader";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
