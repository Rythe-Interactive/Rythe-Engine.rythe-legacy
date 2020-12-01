#pragma once
#ifdef _MSC_VER
#define TO_CHAR_T(X) L ## X
#else
#define TO_CHAR_T(X) X
#endif
