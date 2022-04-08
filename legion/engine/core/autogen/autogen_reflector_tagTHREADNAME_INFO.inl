#include "autogen_reflector_tagTHREADNAME_INFO.hpp"
#include "../../core/async/thread_util.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::core::async::tagTHREADNAME_INFO>(legion::core::async::tagTHREADNAME_INFO& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::core::async::tagTHREADNAME_INFO>();
        refl.typeName = "legion::core::async::tagTHREADNAME_INFO";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::core::async::tagTHREADNAME_INFO>(const legion::core::async::tagTHREADNAME_INFO& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::core::async::tagTHREADNAME_INFO>();
        refl.typeName = "legion::core::async::tagTHREADNAME_INFO";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
