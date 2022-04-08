#include "autogen_reflector_engine_id.hpp"
#include "../../core/engine/engine.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::core::engine_id>(legion::core::engine_id& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::core::engine_id>();
        refl.typeName = "legion::core::engine_id";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::core::engine_id>(const legion::core::engine_id& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::core::engine_id>();
        refl.typeName = "legion::core::engine_id";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
