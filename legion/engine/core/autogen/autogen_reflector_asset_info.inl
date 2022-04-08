#include "autogen_reflector_asset_info.hpp"
#include "../../core/assets/assetcache.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::core::assets::detail::asset_info>(legion::core::assets::detail::asset_info& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::core::assets::detail::asset_info>();
        refl.typeName = "legion::core::assets::detail::asset_info";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::core::assets::detail::asset_info>(const legion::core::assets::detail::asset_info& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::core::assets::detail::asset_info>();
        refl.typeName = "legion::core::assets::detail::asset_info";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
