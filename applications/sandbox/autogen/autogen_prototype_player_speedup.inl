#include "autogen_prototype_player_speedup.hpp"
#include "../../sandbox/systems/simplecameracontroller.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<::player_speedup>(const ::player_speedup& obj)
    {
        prototype prot;
        prot.typeId = typeHash<::player_speedup>();
        prot.typeName = "player_speedup";
        return prot;
    }
}
