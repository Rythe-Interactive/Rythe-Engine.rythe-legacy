#include "autogen_prototype_player_move.hpp"
#include "../../sandbox/systems/simplecameracontroller.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<::player_move>(const ::player_move& obj)
    {
        prototype prot;
        prot.typeId = typeHash<::player_move>();
        prot.typeName = "player_move";
        return prot;
    }
}
