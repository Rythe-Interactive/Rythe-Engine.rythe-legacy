#include "autogen_prototype_texture_import_settings.hpp"
#include "../../rendering/data/texture.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::rendering::texture_import_settings>(const legion::rendering::texture_import_settings& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::rendering::texture_import_settings>();
        prot.typeName = "legion::rendering::texture_import_settings";
        return prot;
    }
}
