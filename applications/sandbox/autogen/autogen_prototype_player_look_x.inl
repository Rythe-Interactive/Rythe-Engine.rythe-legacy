#include "autogen_prototype_player_look_x.hpp"
#include "../../sandbox/systems/simplecameracontroller.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<::player_look_x>(const ::player_look_x& obj)
    {
        prototype prot;
        prot.typeId = typeHash<::player_look_x>();
        prot.typeName = "player_look_x";
        return prot;
    }
}
