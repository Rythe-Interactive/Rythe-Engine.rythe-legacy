#include "autogen_prototype_player_strive.hpp"
#include "../../sandbox/systems/simplecameracontroller.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<::player_strive>(const ::player_strive& obj)
    {
        prototype prot;
        prot.typeId = typeHash<::player_strive>();
        prot.typeName = "player_strive";
        return prot;
    }
}
