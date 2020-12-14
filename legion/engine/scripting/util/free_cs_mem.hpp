#pragma once
#include <core/platform/platform.hpp>

inline void free_cs_mem(void* mem)
{
    #if defined(LEGION_WINDOWS)
        LocalFree(mem);
    #else
        free(mem);
    #endif
}
