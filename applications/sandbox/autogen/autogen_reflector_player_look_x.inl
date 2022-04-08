#include "autogen_reflector_player_look_x.hpp"
#include "../../sandbox/systems/simplecameracontroller.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<::player_look_x>(::player_look_x& obj)
    {
        reflector refl;
        refl.typeId = typeHash<::player_look_x>();
        refl.typeName = "player_look_x";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const ::player_look_x>(const ::player_look_x& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<::player_look_x>();
        refl.typeName = "player_look_x";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
