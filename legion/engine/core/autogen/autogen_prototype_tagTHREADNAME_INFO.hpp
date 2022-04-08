#pragma once
#include <core/types/prototype.hpp>
namespace legion::core::async
{
    struct tagTHREADNAME_INFO;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::core::async::tagTHREADNAME_INFO>(const legion::core::async::tagTHREADNAME_INFO& obj);
}
