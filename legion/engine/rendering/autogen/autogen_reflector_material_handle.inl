#include "autogen_reflector_material_handle.hpp"
#include "../../rendering/data/material.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::rendering::material_handle>(legion::rendering::material_handle& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::rendering::material_handle>();
        refl.typeName = "legion::rendering::material_handle";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::rendering::material_handle>(const legion::rendering::material_handle& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::rendering::material_handle>();
        refl.typeName = "legion::rendering::material_handle";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
