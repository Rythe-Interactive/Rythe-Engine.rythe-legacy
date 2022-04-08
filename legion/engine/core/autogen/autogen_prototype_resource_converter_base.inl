#include "autogen_prototype_resource_converter_base.hpp"
#include "../../core/filesystem/assetimporter.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::core::filesystem::detail::resource_converter_base>(const legion::core::filesystem::detail::resource_converter_base& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::core::filesystem::detail::resource_converter_base>();
        prot.typeName = "legion::core::filesystem::detail::resource_converter_base";
        return prot;
    }
}
