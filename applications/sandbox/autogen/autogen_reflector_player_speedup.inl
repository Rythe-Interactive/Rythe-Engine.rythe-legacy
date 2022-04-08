#include "autogen_reflector_player_speedup.hpp"
#include "../../sandbox/systems/simplecameracontroller.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<::player_speedup>(::player_speedup& obj)
    {
        reflector refl;
        refl.typeId = typeHash<::player_speedup>();
        refl.typeName = "player_speedup";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const ::player_speedup>(const ::player_speedup& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<::player_speedup>();
        refl.typeName = "player_speedup";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
