#include "autogen_prototype_scene.hpp"
#include "../../core/scenemanagement/components/scene.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::core::scenemanagement::scene>(const legion::core::scenemanagement::scene& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::core::scenemanagement::scene>();
        prot.typeName = "legion::core::scenemanagement::scene";
        return prot;
    }
}
