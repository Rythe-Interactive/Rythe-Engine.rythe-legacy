#include "autogen_prototype_asset_info.hpp"
#include "../../core/assets/assetcache.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::core::assets::detail::asset_info>(const legion::core::assets::detail::asset_info& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::core::assets::detail::asset_info>();
        prot.typeName = "legion::core::assets::detail::asset_info";
        return prot;
    }
}
