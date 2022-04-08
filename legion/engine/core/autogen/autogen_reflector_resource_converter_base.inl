#include "autogen_reflector_resource_converter_base.hpp"
#include "../../core/filesystem/assetimporter.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::core::filesystem::detail::resource_converter_base>(legion::core::filesystem::detail::resource_converter_base& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::core::filesystem::detail::resource_converter_base>();
        refl.typeName = "legion::core::filesystem::detail::resource_converter_base";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::core::filesystem::detail::resource_converter_base>(const legion::core::filesystem::detail::resource_converter_base& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::core::filesystem::detail::resource_converter_base>();
        refl.typeName = "legion::core::filesystem::detail::resource_converter_base";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
