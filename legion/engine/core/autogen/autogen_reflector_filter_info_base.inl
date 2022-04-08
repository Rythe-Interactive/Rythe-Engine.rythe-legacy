#include "autogen_reflector_filter_info_base.hpp"
#include "../../core/ecs/filters/filter_info.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::core::ecs::filter_info_base>(legion::core::ecs::filter_info_base& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::core::ecs::filter_info_base>();
        refl.typeName = "legion::core::ecs::filter_info_base";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::core::ecs::filter_info_base>(const legion::core::ecs::filter_info_base& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::core::ecs::filter_info_base>();
        refl.typeName = "legion::core::ecs::filter_info_base";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
