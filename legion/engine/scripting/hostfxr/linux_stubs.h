#pragma once

//fake implementations to make constexpr-if happy

#include <windows.h>

inline HMODULE dlopen(const char*,int){}
enum{RTLD_LAZY, RTLD_LOCAL};
inline void* dlsym(HMODULE,const char*);