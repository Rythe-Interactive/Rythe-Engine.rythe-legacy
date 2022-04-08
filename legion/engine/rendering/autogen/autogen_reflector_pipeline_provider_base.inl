#include "autogen_reflector_pipeline_provider_base.hpp"
#include "../../rendering/systems/renderer.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::rendering::pipeline_provider_base>(legion::rendering::pipeline_provider_base& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::rendering::pipeline_provider_base>();
        refl.typeName = "legion::rendering::pipeline_provider_base";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::rendering::pipeline_provider_base>(const legion::rendering::pipeline_provider_base& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::rendering::pipeline_provider_base>();
        refl.typeName = "legion::rendering::pipeline_provider_base";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
