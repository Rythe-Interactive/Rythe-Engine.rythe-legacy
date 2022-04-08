#include "autogen_prototype_pipeline_provider_base.hpp"
#include "../../rendering/systems/renderer.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::rendering::pipeline_provider_base>(const legion::rendering::pipeline_provider_base& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::rendering::pipeline_provider_base>();
        prot.typeName = "legion::rendering::pipeline_provider_base";
        return prot;
    }
}
