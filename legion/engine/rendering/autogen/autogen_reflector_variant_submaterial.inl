#include "autogen_reflector_variant_submaterial.hpp"
#include "../../rendering/data/material.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::rendering::variant_submaterial>(legion::rendering::variant_submaterial& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::rendering::variant_submaterial>();
        refl.typeName = "legion::rendering::variant_submaterial";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::rendering::variant_submaterial>(const legion::rendering::variant_submaterial& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::rendering::variant_submaterial>();
        refl.typeName = "legion::rendering::variant_submaterial";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
