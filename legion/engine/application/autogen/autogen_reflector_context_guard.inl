#include "autogen_reflector_context_guard.hpp"
#include "../../application/window/window.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::application::context_guard>(legion::application::context_guard& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::application::context_guard>();
        refl.typeName = "legion::application::context_guard";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::application::context_guard>(const legion::application::context_guard& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::application::context_guard>();
        refl.typeName = "legion::application::context_guard";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
