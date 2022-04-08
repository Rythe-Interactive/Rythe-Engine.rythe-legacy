#include "autogen_prototype_impl.hpp"
#include "../../core/logging/logging.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::core::log::impl>(const legion::core::log::impl& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::core::log::impl>();
        prot.typeName = "legion::core::log::impl";
        return prot;
    }
}
