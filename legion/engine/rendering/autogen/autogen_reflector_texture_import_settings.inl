#include "autogen_reflector_texture_import_settings.hpp"
#include "../../rendering/data/texture.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::rendering::texture_import_settings>(legion::rendering::texture_import_settings& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::rendering::texture_import_settings>();
        refl.typeName = "legion::rendering::texture_import_settings";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::rendering::texture_import_settings>(const legion::rendering::texture_import_settings& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::rendering::texture_import_settings>();
        refl.typeName = "legion::rendering::texture_import_settings";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
