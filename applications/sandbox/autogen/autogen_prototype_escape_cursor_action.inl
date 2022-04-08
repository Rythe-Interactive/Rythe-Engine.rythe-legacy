#include "autogen_prototype_escape_cursor_action.hpp"
#include "../../sandbox/systems/simplecameracontroller.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<::escape_cursor_action>(const ::escape_cursor_action& obj)
    {
        prototype prot;
        prot.typeId = typeHash<::escape_cursor_action>();
        prot.typeName = "escape_cursor_action";
        return prot;
    }
}
