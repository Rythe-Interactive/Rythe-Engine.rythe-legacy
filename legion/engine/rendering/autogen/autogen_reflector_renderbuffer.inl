#include "autogen_reflector_renderbuffer.hpp"
#include "../../rendering/data/renderbuffer.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::rendering::renderbuffer>(legion::rendering::renderbuffer& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::rendering::renderbuffer>();
        refl.typeName = "legion::rendering::renderbuffer";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::rendering::renderbuffer>(const legion::rendering::renderbuffer& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::rendering::renderbuffer>();
        refl.typeName = "legion::rendering::renderbuffer";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
