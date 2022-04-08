#include "autogen_reflector_texture_handle.hpp"
#include "../../rendering/data/texture.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::rendering::texture_handle>(legion::rendering::texture_handle& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::rendering::texture_handle>();
        refl.typeName = "legion::rendering::texture_handle";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::rendering::texture_handle>(const legion::rendering::texture_handle& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::rendering::texture_handle>();
        refl.typeName = "legion::rendering::texture_handle";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
