#include "autogen_reflector_player_move.hpp"
#include "../../sandbox/systems/simplecameracontroller.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<::player_move>(::player_move& obj)
    {
        reflector refl;
        refl.typeId = typeHash<::player_move>();
        refl.typeName = "player_move";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const ::player_move>(const ::player_move& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<::player_move>();
        refl.typeName = "player_move";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
