#include "autogen_reflector_shader_variant.hpp"
#include "../../rendering/data/shader.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::rendering::shader_variant>(legion::rendering::shader_variant& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::rendering::shader_variant>();
        refl.typeName = "legion::rendering::shader_variant";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::rendering::shader_variant>(const legion::rendering::shader_variant& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::rendering::shader_variant>();
        refl.typeName = "legion::rendering::shader_variant";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
