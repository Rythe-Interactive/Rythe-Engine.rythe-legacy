#include "autogen_reflector_screen_quad.hpp"
#include "../../rendering/data/screen_quad.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::rendering::screen_quad>(legion::rendering::screen_quad& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::rendering::screen_quad>();
        refl.typeName = "legion::rendering::screen_quad";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::rendering::screen_quad>(const legion::rendering::screen_quad& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::rendering::screen_quad>();
        refl.typeName = "legion::rendering::screen_quad";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
