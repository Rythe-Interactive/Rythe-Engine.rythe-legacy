#include "autogen_reflector_player_strive.hpp"
#include "../../sandbox/systems/simplecameracontroller.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<::player_strive>(::player_strive& obj)
    {
        reflector refl;
        refl.typeId = typeHash<::player_strive>();
        refl.typeName = "player_strive";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const ::player_strive>(const ::player_strive& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<::player_strive>();
        refl.typeName = "player_strive";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
