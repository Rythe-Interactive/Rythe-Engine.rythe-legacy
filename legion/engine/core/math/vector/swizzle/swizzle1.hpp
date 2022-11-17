#pragma once

#include <core/math/vector/swizzle/swizzle_base.hpp>

#define _MATH_SWIZZLE_1_1_(type)\
struct{ type x; };\
struct{ type u; };\
struct{ type r; };\
struct{ type s; };\

#define _MATH_SWIZZLE_1_2_(type)\
struct{::legion::core::math::swizzle<type, 1, 0, 0> xx;};\
\
struct{::legion::core::math::swizzle<type, 1, 0, 0> uu;};\
\
struct{::legion::core::math::swizzle<type, 1, 0, 0> rr;};\
\
struct{::legion::core::math::swizzle<type, 1, 0, 0> ss;};\

#define _MATH_SWIZZLE_1_3_(type)\
struct{::legion::core::math::swizzle<type, 1, 0, 0, 0> xxx;};\
\
struct{::legion::core::math::swizzle<type, 1, 0, 0, 0> uuu;};\
\
struct{::legion::core::math::swizzle<type, 1, 0, 0, 0> rrr;};\
\
struct{::legion::core::math::swizzle<type, 1, 0, 0, 0> sss;};\

#define _MATH_SWIZZLE_1_4_(type)\
struct{::legion::core::math::swizzle<type, 1, 0, 0, 0, 0> xxxx;};\
\
struct{::legion::core::math::swizzle<type, 1, 0, 0, 0, 0> uuuu;};\
\
struct{::legion::core::math::swizzle<type, 1, 0, 0, 0, 0> rrrr;};\
\
struct{::legion::core::math::swizzle<type, 1, 0, 0, 0, 0> ssss;};\
