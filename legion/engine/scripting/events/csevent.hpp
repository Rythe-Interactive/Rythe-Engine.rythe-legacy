#pragma once
#include <core/core.hpp>
namespace legion::scripting
{
    struct csevent : events::event<csevent>
    {
        csevent(){}
        csevent(id_type t, size_type s, void* p):
            type(t),size(s),ptr(p){}

        id_type type;
        size_type size;
        void* ptr;
    };
}
