#include "autogen_reflector_framebuffer.hpp"
#include "../../rendering/data/framebuffer.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::rendering::framebuffer>(legion::rendering::framebuffer& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::rendering::framebuffer>();
        refl.typeName = "legion::rendering::framebuffer";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::rendering::framebuffer>(const legion::rendering::framebuffer& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::rendering::framebuffer>();
        refl.typeName = "legion::rendering::framebuffer";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
