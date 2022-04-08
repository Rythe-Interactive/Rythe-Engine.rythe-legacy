#include "autogen_reflector_implement_signal_t.hpp"
#include "../../core/common/inteface_traits.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::core::interfaces::implement_signal_t>(legion::core::interfaces::implement_signal_t& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::core::interfaces::implement_signal_t>();
        refl.typeName = "legion::core::interfaces::implement_signal_t";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::core::interfaces::implement_signal_t>(const legion::core::interfaces::implement_signal_t& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::core::interfaces::implement_signal_t>();
        refl.typeName = "legion::core::interfaces::implement_signal_t";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
