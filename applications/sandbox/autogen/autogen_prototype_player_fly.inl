#include "autogen_prototype_player_fly.hpp"
#include "../../sandbox/systems/simplecameracontroller.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<::player_fly>(const ::player_fly& obj)
    {
        prototype prot;
        prot.typeId = typeHash<::player_fly>();
        prot.typeName = "player_fly";
        return prot;
    }
}
