#include "autogen_reflector_player_fly.hpp"
#include "../../sandbox/systems/simplecameracontroller.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<::player_fly>(::player_fly& obj)
    {
        reflector refl;
        refl.typeId = typeHash<::player_fly>();
        refl.typeName = "player_fly";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const ::player_fly>(const ::player_fly& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<::player_fly>();
        refl.typeName = "player_fly";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
