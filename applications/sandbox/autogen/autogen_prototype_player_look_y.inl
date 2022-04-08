#include "autogen_prototype_player_look_y.hpp"
#include "../../sandbox/systems/simplecameracontroller.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<::player_look_y>(const ::player_look_y& obj)
    {
        prototype prot;
        prot.typeId = typeHash<::player_look_y>();
        prot.typeName = "player_look_y";
        return prot;
    }
}
