#include "autogen_prototype_shader_import_settings.hpp"
#include "../../rendering/util/settings.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::rendering::shader_import_settings>(const legion::rendering::shader_import_settings& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::rendering::shader_import_settings>();
        prot.typeName = "legion::rendering::shader_import_settings";
        return prot;
    }
}
