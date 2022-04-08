#include "autogen_prototype_tagTHREADNAME_INFO.hpp"
#include "../../core/async/thread_util.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::core::async::tagTHREADNAME_INFO>(const legion::core::async::tagTHREADNAME_INFO& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::core::async::tagTHREADNAME_INFO>();
        prot.typeName = "legion::core::async::tagTHREADNAME_INFO";
        return prot;
    }
}
