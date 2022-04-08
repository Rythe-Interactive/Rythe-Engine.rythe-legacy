#include "autogen_reflector_ShaderCache.hpp"
#include "../../rendering/data/shader.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::rendering::ShaderCache>(legion::rendering::ShaderCache& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::rendering::ShaderCache>();
        refl.typeName = "legion::rendering::ShaderCache";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::rendering::ShaderCache>(const legion::rendering::ShaderCache& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::rendering::ShaderCache>();
        refl.typeName = "legion::rendering::ShaderCache";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
