#include "autogen_reflector_this_engine.hpp"
#include "../../core/engine/engine.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::core::this_engine>(legion::core::this_engine& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::core::this_engine>();
        refl.typeName = "legion::core::this_engine";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::core::this_engine>(const legion::core::this_engine& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::core::this_engine>();
        refl.typeName = "legion::core::this_engine";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
