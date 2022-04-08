#include "autogen_reflector_scene.hpp"
#include "../../core/scenemanagement/components/scene.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::core::scenemanagement::scene>(legion::core::scenemanagement::scene& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::core::scenemanagement::scene>();
        refl.typeName = "legion::core::scenemanagement::scene";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::core::scenemanagement::scene>(const legion::core::scenemanagement::scene& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::core::scenemanagement::scene>();
        refl.typeName = "legion::core::scenemanagement::scene";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
