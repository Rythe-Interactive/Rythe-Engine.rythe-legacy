#include "autogen_reflector_escape_cursor_action.hpp"
#include "../../sandbox/systems/simplecameracontroller.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<::escape_cursor_action>(::escape_cursor_action& obj)
    {
        reflector refl;
        refl.typeId = typeHash<::escape_cursor_action>();
        refl.typeName = "escape_cursor_action";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const ::escape_cursor_action>(const ::escape_cursor_action& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<::escape_cursor_action>();
        refl.typeName = "escape_cursor_action";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
