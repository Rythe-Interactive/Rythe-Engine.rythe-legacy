#include "autogen_prototype_implement_signal_t.hpp"
#include "../../core/common/inteface_traits.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::core::interfaces::implement_signal_t>(const legion::core::interfaces::implement_signal_t& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::core::interfaces::implement_signal_t>();
        prot.typeName = "legion::core::interfaces::implement_signal_t";
        return prot;
    }
}
