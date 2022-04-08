#pragma once
#include <core/types/reflector.hpp>
namespace legion::core::async
{
    struct tagTHREADNAME_INFO;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::core::async::tagTHREADNAME_INFO>(legion::core::async::tagTHREADNAME_INFO& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::core::async::tagTHREADNAME_INFO>(const legion::core::async::tagTHREADNAME_INFO& obj);
}
