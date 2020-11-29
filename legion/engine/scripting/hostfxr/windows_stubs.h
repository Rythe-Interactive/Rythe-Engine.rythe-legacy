#pragma once

//fake implementations to make constexpr-if happy

inline void* LoadLibraryW(const char*);
inline void* GetProcAddress(void*,const char*);