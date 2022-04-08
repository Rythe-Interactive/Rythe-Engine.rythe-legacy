#include "autogen_reflector_texture.hpp"
#include "../../rendering/data/texture.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::rendering::texture>(legion::rendering::texture& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::rendering::texture>();
        refl.typeName = "legion::rendering::texture";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::rendering::texture>(const legion::rendering::texture& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::rendering::texture>();
        refl.typeName = "legion::rendering::texture";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
