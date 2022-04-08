#include "autogen_reflector_model_handle.hpp"
#include "../../rendering/data/model.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::rendering::model_handle>(legion::rendering::model_handle& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::rendering::model_handle>();
        refl.typeName = "legion::rendering::model_handle";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::rendering::model_handle>(const legion::rendering::model_handle& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::rendering::model_handle>();
        refl.typeName = "legion::rendering::model_handle";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
