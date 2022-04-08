#include "autogen_reflector_model.hpp"
#include "../../rendering/data/model.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::rendering::model>(legion::rendering::model& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::rendering::model>();
        refl.typeName = "legion::rendering::model";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::rendering::model>(const legion::rendering::model& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::rendering::model>();
        refl.typeName = "legion::rendering::model";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
