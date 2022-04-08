#include "autogen_reflector_buffer.hpp"
#include "../../rendering/data/buffer.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::rendering::buffer>(legion::rendering::buffer& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::rendering::buffer>();
        refl.typeName = "legion::rendering::buffer";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::rendering::buffer>(const legion::rendering::buffer& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::rendering::buffer>();
        refl.typeName = "legion::rendering::buffer";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
