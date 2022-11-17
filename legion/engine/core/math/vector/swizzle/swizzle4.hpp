#pragma once

#include <core/math/vector/swizzle/swizzle_base.hpp>

#define _MATH_SWIZZLE_4_1_(type)\
struct{ type x, y, z, w; };\
struct{ type r, g, b, a; };\
struct{ type s, t, p, q; };\

#define _MATH_SWIZZLE_4_2_(type)\
struct{::legion::core::math::swizzle<type, 4, 0, 0> xx;};\
struct{::legion::core::math::swizzle<type, 4, 0, 1> xy;};\
struct{::legion::core::math::swizzle<type, 4, 0, 2> xz;};\
struct{::legion::core::math::swizzle<type, 4, 0, 3> xw;};\
struct{::legion::core::math::swizzle<type, 4, 1, 0> yx;};\
struct{::legion::core::math::swizzle<type, 4, 1, 1> yy;};\
struct{::legion::core::math::swizzle<type, 4, 1, 2> yz;};\
struct{::legion::core::math::swizzle<type, 4, 1, 3> yw;};\
struct{::legion::core::math::swizzle<type, 4, 2, 0> zx;};\
struct{::legion::core::math::swizzle<type, 4, 2, 1> zy;};\
struct{::legion::core::math::swizzle<type, 4, 2, 2> zz;};\
struct{::legion::core::math::swizzle<type, 4, 2, 3> zw;};\
struct{::legion::core::math::swizzle<type, 4, 3, 0> wx;};\
struct{::legion::core::math::swizzle<type, 4, 3, 1> wy;};\
struct{::legion::core::math::swizzle<type, 4, 3, 2> wz;};\
struct{::legion::core::math::swizzle<type, 4, 3, 3> ww;};\
\
struct{::legion::core::math::swizzle<type, 4, 0, 0> rr;};\
struct{::legion::core::math::swizzle<type, 4, 0, 1> rg;};\
struct{::legion::core::math::swizzle<type, 4, 0, 2> rb;};\
struct{::legion::core::math::swizzle<type, 4, 0, 3> ra;};\
struct{::legion::core::math::swizzle<type, 4, 1, 0> gr;};\
struct{::legion::core::math::swizzle<type, 4, 1, 1> gg;};\
struct{::legion::core::math::swizzle<type, 4, 1, 2> gb;};\
struct{::legion::core::math::swizzle<type, 4, 1, 3> ga;};\
struct{::legion::core::math::swizzle<type, 4, 2, 0> br;};\
struct{::legion::core::math::swizzle<type, 4, 2, 1> bg;};\
struct{::legion::core::math::swizzle<type, 4, 2, 2> bb;};\
struct{::legion::core::math::swizzle<type, 4, 2, 3> ba;};\
struct{::legion::core::math::swizzle<type, 4, 3, 0> ar;};\
struct{::legion::core::math::swizzle<type, 4, 3, 1> ag;};\
struct{::legion::core::math::swizzle<type, 4, 3, 2> ab;};\
struct{::legion::core::math::swizzle<type, 4, 3, 3> aa;};\
\
struct{::legion::core::math::swizzle<type, 4, 0, 0> ss;};\
struct{::legion::core::math::swizzle<type, 4, 0, 1> st;};\
struct{::legion::core::math::swizzle<type, 4, 0, 2> sp;};\
struct{::legion::core::math::swizzle<type, 4, 0, 3> sq;};\
struct{::legion::core::math::swizzle<type, 4, 1, 0> ts;};\
struct{::legion::core::math::swizzle<type, 4, 1, 1> tt;};\
struct{::legion::core::math::swizzle<type, 4, 1, 2> tp;};\
struct{::legion::core::math::swizzle<type, 4, 1, 3> tq;};\
struct{::legion::core::math::swizzle<type, 4, 2, 0> ps;};\
struct{::legion::core::math::swizzle<type, 4, 2, 1> pt;};\
struct{::legion::core::math::swizzle<type, 4, 2, 2> pp;};\
struct{::legion::core::math::swizzle<type, 4, 2, 3> pq;};\
struct{::legion::core::math::swizzle<type, 4, 3, 0> qs;};\
struct{::legion::core::math::swizzle<type, 4, 3, 1> qt;};\
struct{::legion::core::math::swizzle<type, 4, 3, 2> qp;};\
struct{::legion::core::math::swizzle<type, 4, 3, 3> qq;};\


#define _MATH_SWIZZLE_4_3_(type)\
struct{::legion::core::math::swizzle<type, 4, 0, 0, 0> xxx;};\
struct{::legion::core::math::swizzle<type, 4, 0, 0, 1> xxy;};\
struct{::legion::core::math::swizzle<type, 4, 0, 0, 2> xxz;};\
struct{::legion::core::math::swizzle<type, 4, 0, 0, 3> xxw;};\
struct{::legion::core::math::swizzle<type, 4, 0, 1, 0> xyx;};\
struct{::legion::core::math::swizzle<type, 4, 0, 1, 1> xyy;};\
struct{::legion::core::math::swizzle<type, 4, 0, 1, 2> xyz;};\
struct{::legion::core::math::swizzle<type, 4, 0, 1, 3> xyw;};\
struct{::legion::core::math::swizzle<type, 4, 0, 2, 0> xzx;};\
struct{::legion::core::math::swizzle<type, 4, 0, 2, 1> xzy;};\
struct{::legion::core::math::swizzle<type, 4, 0, 2, 2> xzz;};\
struct{::legion::core::math::swizzle<type, 4, 0, 2, 3> xzw;};\
struct{::legion::core::math::swizzle<type, 4, 0, 3, 0> xwx;};\
struct{::legion::core::math::swizzle<type, 4, 0, 3, 1> xwy;};\
struct{::legion::core::math::swizzle<type, 4, 0, 3, 2> xwz;};\
struct{::legion::core::math::swizzle<type, 4, 0, 3, 3> xww;};\
struct{::legion::core::math::swizzle<type, 4, 1, 0, 0> yxx;};\
struct{::legion::core::math::swizzle<type, 4, 1, 0, 1> yxy;};\
struct{::legion::core::math::swizzle<type, 4, 1, 0, 2> yxz;};\
struct{::legion::core::math::swizzle<type, 4, 1, 0, 3> yxw;};\
struct{::legion::core::math::swizzle<type, 4, 1, 1, 0> yyx;};\
struct{::legion::core::math::swizzle<type, 4, 1, 1, 1> yyy;};\
struct{::legion::core::math::swizzle<type, 4, 1, 1, 2> yyz;};\
struct{::legion::core::math::swizzle<type, 4, 1, 1, 3> yyw;};\
struct{::legion::core::math::swizzle<type, 4, 1, 2, 0> yzx;};\
struct{::legion::core::math::swizzle<type, 4, 1, 2, 1> yzy;};\
struct{::legion::core::math::swizzle<type, 4, 1, 2, 2> yzz;};\
struct{::legion::core::math::swizzle<type, 4, 1, 2, 3> yzw;};\
struct{::legion::core::math::swizzle<type, 4, 1, 3, 0> ywx;};\
struct{::legion::core::math::swizzle<type, 4, 1, 3, 1> ywy;};\
struct{::legion::core::math::swizzle<type, 4, 1, 3, 2> ywz;};\
struct{::legion::core::math::swizzle<type, 4, 1, 3, 3> yww;};\
struct{::legion::core::math::swizzle<type, 4, 2, 0, 0> zxx;};\
struct{::legion::core::math::swizzle<type, 4, 2, 0, 1> zxy;};\
struct{::legion::core::math::swizzle<type, 4, 2, 0, 2> zxz;};\
struct{::legion::core::math::swizzle<type, 4, 2, 0, 3> zxw;};\
struct{::legion::core::math::swizzle<type, 4, 2, 1, 0> zyx;};\
struct{::legion::core::math::swizzle<type, 4, 2, 1, 1> zyy;};\
struct{::legion::core::math::swizzle<type, 4, 2, 1, 2> zyz;};\
struct{::legion::core::math::swizzle<type, 4, 2, 1, 3> zyw;};\
struct{::legion::core::math::swizzle<type, 4, 2, 2, 0> zzx;};\
struct{::legion::core::math::swizzle<type, 4, 2, 2, 1> zzy;};\
struct{::legion::core::math::swizzle<type, 4, 2, 2, 2> zzz;};\
struct{::legion::core::math::swizzle<type, 4, 2, 2, 3> zzw;};\
struct{::legion::core::math::swizzle<type, 4, 2, 3, 0> zwx;};\
struct{::legion::core::math::swizzle<type, 4, 2, 3, 1> zwy;};\
struct{::legion::core::math::swizzle<type, 4, 2, 3, 2> zwz;};\
struct{::legion::core::math::swizzle<type, 4, 2, 3, 3> zww;};\
struct{::legion::core::math::swizzle<type, 4, 3, 0, 0> wxx;};\
struct{::legion::core::math::swizzle<type, 4, 3, 0, 1> wxy;};\
struct{::legion::core::math::swizzle<type, 4, 3, 0, 2> wxz;};\
struct{::legion::core::math::swizzle<type, 4, 3, 0, 3> wxw;};\
struct{::legion::core::math::swizzle<type, 4, 3, 1, 0> wyx;};\
struct{::legion::core::math::swizzle<type, 4, 3, 1, 1> wyy;};\
struct{::legion::core::math::swizzle<type, 4, 3, 1, 2> wyz;};\
struct{::legion::core::math::swizzle<type, 4, 3, 1, 3> wyw;};\
struct{::legion::core::math::swizzle<type, 4, 3, 2, 0> wzx;};\
struct{::legion::core::math::swizzle<type, 4, 3, 2, 1> wzy;};\
struct{::legion::core::math::swizzle<type, 4, 3, 2, 2> wzz;};\
struct{::legion::core::math::swizzle<type, 4, 3, 2, 3> wzw;};\
struct{::legion::core::math::swizzle<type, 4, 3, 3, 0> wwx;};\
struct{::legion::core::math::swizzle<type, 4, 3, 3, 1> wwy;};\
struct{::legion::core::math::swizzle<type, 4, 3, 3, 2> wwz;};\
struct{::legion::core::math::swizzle<type, 4, 3, 3, 3> www;};\
\
struct{::legion::core::math::swizzle<type, 4, 0, 0, 0> rrr;};\
struct{::legion::core::math::swizzle<type, 4, 0, 0, 1> rrg;};\
struct{::legion::core::math::swizzle<type, 4, 0, 0, 2> rrb;};\
struct{::legion::core::math::swizzle<type, 4, 0, 0, 3> rra;};\
struct{::legion::core::math::swizzle<type, 4, 0, 1, 0> rgr;};\
struct{::legion::core::math::swizzle<type, 4, 0, 1, 1> rgg;};\
struct{::legion::core::math::swizzle<type, 4, 0, 1, 2> rgb;};\
struct{::legion::core::math::swizzle<type, 4, 0, 1, 3> rga;};\
struct{::legion::core::math::swizzle<type, 4, 0, 2, 0> rbr;};\
struct{::legion::core::math::swizzle<type, 4, 0, 2, 1> rbg;};\
struct{::legion::core::math::swizzle<type, 4, 0, 2, 2> rbb;};\
struct{::legion::core::math::swizzle<type, 4, 0, 2, 3> rba;};\
struct{::legion::core::math::swizzle<type, 4, 0, 3, 0> rar;};\
struct{::legion::core::math::swizzle<type, 4, 0, 3, 1> rag;};\
struct{::legion::core::math::swizzle<type, 4, 0, 3, 2> rab;};\
struct{::legion::core::math::swizzle<type, 4, 0, 3, 3> raa;};\
struct{::legion::core::math::swizzle<type, 4, 1, 0, 0> grr;};\
struct{::legion::core::math::swizzle<type, 4, 1, 0, 1> grg;};\
struct{::legion::core::math::swizzle<type, 4, 1, 0, 2> grb;};\
struct{::legion::core::math::swizzle<type, 4, 1, 0, 3> gra;};\
struct{::legion::core::math::swizzle<type, 4, 1, 1, 0> ggr;};\
struct{::legion::core::math::swizzle<type, 4, 1, 1, 1> ggg;};\
struct{::legion::core::math::swizzle<type, 4, 1, 1, 2> ggb;};\
struct{::legion::core::math::swizzle<type, 4, 1, 1, 3> gga;};\
struct{::legion::core::math::swizzle<type, 4, 1, 2, 0> gbr;};\
struct{::legion::core::math::swizzle<type, 4, 1, 2, 1> gbg;};\
struct{::legion::core::math::swizzle<type, 4, 1, 2, 2> gbb;};\
struct{::legion::core::math::swizzle<type, 4, 1, 2, 3> gba;};\
struct{::legion::core::math::swizzle<type, 4, 1, 3, 0> gar;};\
struct{::legion::core::math::swizzle<type, 4, 1, 3, 1> gag;};\
struct{::legion::core::math::swizzle<type, 4, 1, 3, 2> gab;};\
struct{::legion::core::math::swizzle<type, 4, 1, 3, 3> gaa;};\
struct{::legion::core::math::swizzle<type, 4, 2, 0, 0> brr;};\
struct{::legion::core::math::swizzle<type, 4, 2, 0, 1> brg;};\
struct{::legion::core::math::swizzle<type, 4, 2, 0, 2> brb;};\
struct{::legion::core::math::swizzle<type, 4, 2, 0, 3> bra;};\
struct{::legion::core::math::swizzle<type, 4, 2, 1, 0> bgr;};\
struct{::legion::core::math::swizzle<type, 4, 2, 1, 1> bgg;};\
struct{::legion::core::math::swizzle<type, 4, 2, 1, 2> bgb;};\
struct{::legion::core::math::swizzle<type, 4, 2, 1, 3> bga;};\
struct{::legion::core::math::swizzle<type, 4, 2, 2, 0> bbr;};\
struct{::legion::core::math::swizzle<type, 4, 2, 2, 1> bbg;};\
struct{::legion::core::math::swizzle<type, 4, 2, 2, 2> bbb;};\
struct{::legion::core::math::swizzle<type, 4, 2, 2, 3> bba;};\
struct{::legion::core::math::swizzle<type, 4, 2, 3, 0> bar;};\
struct{::legion::core::math::swizzle<type, 4, 2, 3, 1> bag;};\
struct{::legion::core::math::swizzle<type, 4, 2, 3, 2> bab;};\
struct{::legion::core::math::swizzle<type, 4, 2, 3, 3> baa;};\
struct{::legion::core::math::swizzle<type, 4, 3, 0, 0> arr;};\
struct{::legion::core::math::swizzle<type, 4, 3, 0, 1> arg;};\
struct{::legion::core::math::swizzle<type, 4, 3, 0, 2> arb;};\
struct{::legion::core::math::swizzle<type, 4, 3, 0, 3> ara;};\
struct{::legion::core::math::swizzle<type, 4, 3, 1, 0> agr;};\
struct{::legion::core::math::swizzle<type, 4, 3, 1, 1> agg;};\
struct{::legion::core::math::swizzle<type, 4, 3, 1, 2> agb;};\
struct{::legion::core::math::swizzle<type, 4, 3, 1, 3> aga;};\
struct{::legion::core::math::swizzle<type, 4, 3, 2, 0> abr;};\
struct{::legion::core::math::swizzle<type, 4, 3, 2, 1> abg;};\
struct{::legion::core::math::swizzle<type, 4, 3, 2, 2> abb;};\
struct{::legion::core::math::swizzle<type, 4, 3, 2, 3> aba;};\
struct{::legion::core::math::swizzle<type, 4, 3, 3, 0> aar;};\
struct{::legion::core::math::swizzle<type, 4, 3, 3, 1> aag;};\
struct{::legion::core::math::swizzle<type, 4, 3, 3, 2> aab;};\
struct{::legion::core::math::swizzle<type, 4, 3, 3, 3> aaa;};\
\
struct{::legion::core::math::swizzle<type, 4, 0, 0, 0> sss;};\
struct{::legion::core::math::swizzle<type, 4, 0, 0, 1> sst;};\
struct{::legion::core::math::swizzle<type, 4, 0, 0, 2> ssp;};\
struct{::legion::core::math::swizzle<type, 4, 0, 0, 3> ssq;};\
struct{::legion::core::math::swizzle<type, 4, 0, 1, 0> sts;};\
struct{::legion::core::math::swizzle<type, 4, 0, 1, 1> stt;};\
struct{::legion::core::math::swizzle<type, 4, 0, 1, 2> stp;};\
struct{::legion::core::math::swizzle<type, 4, 0, 1, 3> stq;};\
struct{::legion::core::math::swizzle<type, 4, 0, 2, 0> sps;};\
struct{::legion::core::math::swizzle<type, 4, 0, 2, 1> spt;};\
struct{::legion::core::math::swizzle<type, 4, 0, 2, 2> spp;};\
struct{::legion::core::math::swizzle<type, 4, 0, 2, 3> spq;};\
struct{::legion::core::math::swizzle<type, 4, 0, 3, 0> sqs;};\
struct{::legion::core::math::swizzle<type, 4, 0, 3, 1> sqt;};\
struct{::legion::core::math::swizzle<type, 4, 0, 3, 2> sqp;};\
struct{::legion::core::math::swizzle<type, 4, 0, 3, 3> sqq;};\
struct{::legion::core::math::swizzle<type, 4, 1, 0, 0> tss;};\
struct{::legion::core::math::swizzle<type, 4, 1, 0, 1> tst;};\
struct{::legion::core::math::swizzle<type, 4, 1, 0, 2> tsp;};\
struct{::legion::core::math::swizzle<type, 4, 1, 0, 3> tsq;};\
struct{::legion::core::math::swizzle<type, 4, 1, 1, 0> tts;};\
struct{::legion::core::math::swizzle<type, 4, 1, 1, 1> ttt;};\
struct{::legion::core::math::swizzle<type, 4, 1, 1, 2> ttp;};\
struct{::legion::core::math::swizzle<type, 4, 1, 1, 3> ttq;};\
struct{::legion::core::math::swizzle<type, 4, 1, 2, 0> tps;};\
struct{::legion::core::math::swizzle<type, 4, 1, 2, 1> tpt;};\
struct{::legion::core::math::swizzle<type, 4, 1, 2, 2> tpp;};\
struct{::legion::core::math::swizzle<type, 4, 1, 2, 3> tpq;};\
struct{::legion::core::math::swizzle<type, 4, 1, 3, 0> tqs;};\
struct{::legion::core::math::swizzle<type, 4, 1, 3, 1> tqt;};\
struct{::legion::core::math::swizzle<type, 4, 1, 3, 2> tqp;};\
struct{::legion::core::math::swizzle<type, 4, 1, 3, 3> tqq;};\
struct{::legion::core::math::swizzle<type, 4, 2, 0, 0> pss;};\
struct{::legion::core::math::swizzle<type, 4, 2, 0, 1> pst;};\
struct{::legion::core::math::swizzle<type, 4, 2, 0, 2> psp;};\
struct{::legion::core::math::swizzle<type, 4, 2, 0, 3> psq;};\
struct{::legion::core::math::swizzle<type, 4, 2, 1, 0> pts;};\
struct{::legion::core::math::swizzle<type, 4, 2, 1, 1> ptt;};\
struct{::legion::core::math::swizzle<type, 4, 2, 1, 2> ptp;};\
struct{::legion::core::math::swizzle<type, 4, 2, 1, 3> ptq;};\
struct{::legion::core::math::swizzle<type, 4, 2, 2, 0> pps;};\
struct{::legion::core::math::swizzle<type, 4, 2, 2, 1> ppt;};\
struct{::legion::core::math::swizzle<type, 4, 2, 2, 2> ppp;};\
struct{::legion::core::math::swizzle<type, 4, 2, 2, 3> ppq;};\
struct{::legion::core::math::swizzle<type, 4, 2, 3, 0> pqs;};\
struct{::legion::core::math::swizzle<type, 4, 2, 3, 1> pqt;};\
struct{::legion::core::math::swizzle<type, 4, 2, 3, 2> pqp;};\
struct{::legion::core::math::swizzle<type, 4, 2, 3, 3> pqq;};\
struct{::legion::core::math::swizzle<type, 4, 3, 0, 0> qss;};\
struct{::legion::core::math::swizzle<type, 4, 3, 0, 1> qst;};\
struct{::legion::core::math::swizzle<type, 4, 3, 0, 2> qsp;};\
struct{::legion::core::math::swizzle<type, 4, 3, 0, 3> qsq;};\
struct{::legion::core::math::swizzle<type, 4, 3, 1, 0> qts;};\
struct{::legion::core::math::swizzle<type, 4, 3, 1, 1> qtt;};\
struct{::legion::core::math::swizzle<type, 4, 3, 1, 2> qtp;};\
struct{::legion::core::math::swizzle<type, 4, 3, 1, 3> qtq;};\
struct{::legion::core::math::swizzle<type, 4, 3, 2, 0> qps;};\
struct{::legion::core::math::swizzle<type, 4, 3, 2, 1> qpt;};\
struct{::legion::core::math::swizzle<type, 4, 3, 2, 2> qpp;};\
struct{::legion::core::math::swizzle<type, 4, 3, 2, 3> qpq;};\
struct{::legion::core::math::swizzle<type, 4, 3, 3, 0> qqs;};\
struct{::legion::core::math::swizzle<type, 4, 3, 3, 1> qqt;};\
struct{::legion::core::math::swizzle<type, 4, 3, 3, 2> qqp;};\
struct{::legion::core::math::swizzle<type, 4, 3, 3, 3> qqq;};\


#define _MATH_SWIZZLE_4_4_(type)\
struct{::legion::core::math::swizzle<type, 4, 0, 0, 0, 0> xxxx;};\
struct{::legion::core::math::swizzle<type, 4, 0, 0, 0, 1> xxxy;};\
struct{::legion::core::math::swizzle<type, 4, 0, 0, 0, 2> xxxz;};\
struct{::legion::core::math::swizzle<type, 4, 0, 0, 0, 3> xxxw;};\
struct{::legion::core::math::swizzle<type, 4, 0, 0, 1, 0> xxyx;};\
struct{::legion::core::math::swizzle<type, 4, 0, 0, 1, 1> xxyy;};\
struct{::legion::core::math::swizzle<type, 4, 0, 0, 1, 2> xxyz;};\
struct{::legion::core::math::swizzle<type, 4, 0, 0, 1, 3> xxyw;};\
struct{::legion::core::math::swizzle<type, 4, 0, 0, 2, 0> xxzx;};\
struct{::legion::core::math::swizzle<type, 4, 0, 0, 2, 1> xxzy;};\
struct{::legion::core::math::swizzle<type, 4, 0, 0, 2, 2> xxzz;};\
struct{::legion::core::math::swizzle<type, 4, 0, 0, 2, 3> xxzw;};\
struct{::legion::core::math::swizzle<type, 4, 0, 0, 3, 0> xxwx;};\
struct{::legion::core::math::swizzle<type, 4, 0, 0, 3, 1> xxwy;};\
struct{::legion::core::math::swizzle<type, 4, 0, 0, 3, 2> xxwz;};\
struct{::legion::core::math::swizzle<type, 4, 0, 0, 3, 3> xxww;};\
struct{::legion::core::math::swizzle<type, 4, 0, 1, 0, 0> xyxx;};\
struct{::legion::core::math::swizzle<type, 4, 0, 1, 0, 1> xyxy;};\
struct{::legion::core::math::swizzle<type, 4, 0, 1, 0, 2> xyxz;};\
struct{::legion::core::math::swizzle<type, 4, 0, 1, 0, 3> xyxw;};\
struct{::legion::core::math::swizzle<type, 4, 0, 1, 1, 0> xyyx;};\
struct{::legion::core::math::swizzle<type, 4, 0, 1, 1, 1> xyyy;};\
struct{::legion::core::math::swizzle<type, 4, 0, 1, 1, 2> xyyz;};\
struct{::legion::core::math::swizzle<type, 4, 0, 1, 1, 3> xyyw;};\
struct{::legion::core::math::swizzle<type, 4, 0, 1, 2, 0> xyzx;};\
struct{::legion::core::math::swizzle<type, 4, 0, 1, 2, 1> xyzy;};\
struct{::legion::core::math::swizzle<type, 4, 0, 1, 2, 2> xyzz;};\
struct{::legion::core::math::swizzle<type, 4, 0, 1, 2, 3> xyzw;};\
struct{::legion::core::math::swizzle<type, 4, 0, 1, 3, 0> xywx;};\
struct{::legion::core::math::swizzle<type, 4, 0, 1, 3, 1> xywy;};\
struct{::legion::core::math::swizzle<type, 4, 0, 1, 3, 2> xywz;};\
struct{::legion::core::math::swizzle<type, 4, 0, 1, 3, 3> xyww;};\
struct{::legion::core::math::swizzle<type, 4, 0, 2, 0, 0> xzxx;};\
struct{::legion::core::math::swizzle<type, 4, 0, 2, 0, 1> xzxy;};\
struct{::legion::core::math::swizzle<type, 4, 0, 2, 0, 2> xzxz;};\
struct{::legion::core::math::swizzle<type, 4, 0, 2, 0, 3> xzxw;};\
struct{::legion::core::math::swizzle<type, 4, 0, 2, 1, 0> xzyx;};\
struct{::legion::core::math::swizzle<type, 4, 0, 2, 1, 1> xzyy;};\
struct{::legion::core::math::swizzle<type, 4, 0, 2, 1, 2> xzyz;};\
struct{::legion::core::math::swizzle<type, 4, 0, 2, 1, 3> xzyw;};\
struct{::legion::core::math::swizzle<type, 4, 0, 2, 2, 0> xzzx;};\
struct{::legion::core::math::swizzle<type, 4, 0, 2, 2, 1> xzzy;};\
struct{::legion::core::math::swizzle<type, 4, 0, 2, 2, 2> xzzz;};\
struct{::legion::core::math::swizzle<type, 4, 0, 2, 2, 3> xzzw;};\
struct{::legion::core::math::swizzle<type, 4, 0, 2, 3, 0> xzwx;};\
struct{::legion::core::math::swizzle<type, 4, 0, 2, 3, 1> xzwy;};\
struct{::legion::core::math::swizzle<type, 4, 0, 2, 3, 2> xzwz;};\
struct{::legion::core::math::swizzle<type, 4, 0, 2, 3, 3> xzww;};\
struct{::legion::core::math::swizzle<type, 4, 0, 3, 0, 0> xwxx;};\
struct{::legion::core::math::swizzle<type, 4, 0, 3, 0, 1> xwxy;};\
struct{::legion::core::math::swizzle<type, 4, 0, 3, 0, 2> xwxz;};\
struct{::legion::core::math::swizzle<type, 4, 0, 3, 0, 3> xwxw;};\
struct{::legion::core::math::swizzle<type, 4, 0, 3, 1, 0> xwyx;};\
struct{::legion::core::math::swizzle<type, 4, 0, 3, 1, 1> xwyy;};\
struct{::legion::core::math::swizzle<type, 4, 0, 3, 1, 2> xwyz;};\
struct{::legion::core::math::swizzle<type, 4, 0, 3, 1, 3> xwyw;};\
struct{::legion::core::math::swizzle<type, 4, 0, 3, 2, 0> xwzx;};\
struct{::legion::core::math::swizzle<type, 4, 0, 3, 2, 1> xwzy;};\
struct{::legion::core::math::swizzle<type, 4, 0, 3, 2, 2> xwzz;};\
struct{::legion::core::math::swizzle<type, 4, 0, 3, 2, 3> xwzw;};\
struct{::legion::core::math::swizzle<type, 4, 0, 3, 3, 0> xwwx;};\
struct{::legion::core::math::swizzle<type, 4, 0, 3, 3, 1> xwwy;};\
struct{::legion::core::math::swizzle<type, 4, 0, 3, 3, 2> xwwz;};\
struct{::legion::core::math::swizzle<type, 4, 0, 3, 3, 3> xwww;};\
struct{::legion::core::math::swizzle<type, 4, 1, 0, 0, 0> yxxx;};\
struct{::legion::core::math::swizzle<type, 4, 1, 0, 0, 1> yxxy;};\
struct{::legion::core::math::swizzle<type, 4, 1, 0, 0, 2> yxxz;};\
struct{::legion::core::math::swizzle<type, 4, 1, 0, 0, 3> yxxw;};\
struct{::legion::core::math::swizzle<type, 4, 1, 0, 1, 0> yxyx;};\
struct{::legion::core::math::swizzle<type, 4, 1, 0, 1, 1> yxyy;};\
struct{::legion::core::math::swizzle<type, 4, 1, 0, 1, 2> yxyz;};\
struct{::legion::core::math::swizzle<type, 4, 1, 0, 1, 3> yxyw;};\
struct{::legion::core::math::swizzle<type, 4, 1, 0, 2, 0> yxzx;};\
struct{::legion::core::math::swizzle<type, 4, 1, 0, 2, 1> yxzy;};\
struct{::legion::core::math::swizzle<type, 4, 1, 0, 2, 2> yxzz;};\
struct{::legion::core::math::swizzle<type, 4, 1, 0, 2, 3> yxzw;};\
struct{::legion::core::math::swizzle<type, 4, 1, 0, 3, 0> yxwx;};\
struct{::legion::core::math::swizzle<type, 4, 1, 0, 3, 1> yxwy;};\
struct{::legion::core::math::swizzle<type, 4, 1, 0, 3, 2> yxwz;};\
struct{::legion::core::math::swizzle<type, 4, 1, 0, 3, 3> yxww;};\
struct{::legion::core::math::swizzle<type, 4, 1, 1, 0, 0> yyxx;};\
struct{::legion::core::math::swizzle<type, 4, 1, 1, 0, 1> yyxy;};\
struct{::legion::core::math::swizzle<type, 4, 1, 1, 0, 2> yyxz;};\
struct{::legion::core::math::swizzle<type, 4, 1, 1, 0, 3> yyxw;};\
struct{::legion::core::math::swizzle<type, 4, 1, 1, 1, 0> yyyx;};\
struct{::legion::core::math::swizzle<type, 4, 1, 1, 1, 1> yyyy;};\
struct{::legion::core::math::swizzle<type, 4, 1, 1, 1, 2> yyyz;};\
struct{::legion::core::math::swizzle<type, 4, 1, 1, 1, 3> yyyw;};\
struct{::legion::core::math::swizzle<type, 4, 1, 1, 2, 0> yyzx;};\
struct{::legion::core::math::swizzle<type, 4, 1, 1, 2, 1> yyzy;};\
struct{::legion::core::math::swizzle<type, 4, 1, 1, 2, 2> yyzz;};\
struct{::legion::core::math::swizzle<type, 4, 1, 1, 2, 3> yyzw;};\
struct{::legion::core::math::swizzle<type, 4, 1, 1, 3, 0> yywx;};\
struct{::legion::core::math::swizzle<type, 4, 1, 1, 3, 1> yywy;};\
struct{::legion::core::math::swizzle<type, 4, 1, 1, 3, 2> yywz;};\
struct{::legion::core::math::swizzle<type, 4, 1, 1, 3, 3> yyww;};\
struct{::legion::core::math::swizzle<type, 4, 1, 2, 0, 0> yzxx;};\
struct{::legion::core::math::swizzle<type, 4, 1, 2, 0, 1> yzxy;};\
struct{::legion::core::math::swizzle<type, 4, 1, 2, 0, 2> yzxz;};\
struct{::legion::core::math::swizzle<type, 4, 1, 2, 0, 3> yzxw;};\
struct{::legion::core::math::swizzle<type, 4, 1, 2, 1, 0> yzyx;};\
struct{::legion::core::math::swizzle<type, 4, 1, 2, 1, 1> yzyy;};\
struct{::legion::core::math::swizzle<type, 4, 1, 2, 1, 2> yzyz;};\
struct{::legion::core::math::swizzle<type, 4, 1, 2, 1, 3> yzyw;};\
struct{::legion::core::math::swizzle<type, 4, 1, 2, 2, 0> yzzx;};\
struct{::legion::core::math::swizzle<type, 4, 1, 2, 2, 1> yzzy;};\
struct{::legion::core::math::swizzle<type, 4, 1, 2, 2, 2> yzzz;};\
struct{::legion::core::math::swizzle<type, 4, 1, 2, 2, 3> yzzw;};\
struct{::legion::core::math::swizzle<type, 4, 1, 2, 3, 0> yzwx;};\
struct{::legion::core::math::swizzle<type, 4, 1, 2, 3, 1> yzwy;};\
struct{::legion::core::math::swizzle<type, 4, 1, 2, 3, 2> yzwz;};\
struct{::legion::core::math::swizzle<type, 4, 1, 2, 3, 3> yzww;};\
struct{::legion::core::math::swizzle<type, 4, 1, 3, 0, 0> ywxx;};\
struct{::legion::core::math::swizzle<type, 4, 1, 3, 0, 1> ywxy;};\
struct{::legion::core::math::swizzle<type, 4, 1, 3, 0, 2> ywxz;};\
struct{::legion::core::math::swizzle<type, 4, 1, 3, 0, 3> ywxw;};\
struct{::legion::core::math::swizzle<type, 4, 1, 3, 1, 0> ywyx;};\
struct{::legion::core::math::swizzle<type, 4, 1, 3, 1, 1> ywyy;};\
struct{::legion::core::math::swizzle<type, 4, 1, 3, 1, 2> ywyz;};\
struct{::legion::core::math::swizzle<type, 4, 1, 3, 1, 3> ywyw;};\
struct{::legion::core::math::swizzle<type, 4, 1, 3, 2, 0> ywzx;};\
struct{::legion::core::math::swizzle<type, 4, 1, 3, 2, 1> ywzy;};\
struct{::legion::core::math::swizzle<type, 4, 1, 3, 2, 2> ywzz;};\
struct{::legion::core::math::swizzle<type, 4, 1, 3, 2, 3> ywzw;};\
struct{::legion::core::math::swizzle<type, 4, 1, 3, 3, 0> ywwx;};\
struct{::legion::core::math::swizzle<type, 4, 1, 3, 3, 1> ywwy;};\
struct{::legion::core::math::swizzle<type, 4, 1, 3, 3, 2> ywwz;};\
struct{::legion::core::math::swizzle<type, 4, 1, 3, 3, 3> ywww;};\
struct{::legion::core::math::swizzle<type, 4, 2, 0, 0, 0> zxxx;};\
struct{::legion::core::math::swizzle<type, 4, 2, 0, 0, 1> zxxy;};\
struct{::legion::core::math::swizzle<type, 4, 2, 0, 0, 2> zxxz;};\
struct{::legion::core::math::swizzle<type, 4, 2, 0, 0, 3> zxxw;};\
struct{::legion::core::math::swizzle<type, 4, 2, 0, 1, 0> zxyx;};\
struct{::legion::core::math::swizzle<type, 4, 2, 0, 1, 1> zxyy;};\
struct{::legion::core::math::swizzle<type, 4, 2, 0, 1, 2> zxyz;};\
struct{::legion::core::math::swizzle<type, 4, 2, 0, 1, 3> zxyw;};\
struct{::legion::core::math::swizzle<type, 4, 2, 0, 2, 0> zxzx;};\
struct{::legion::core::math::swizzle<type, 4, 2, 0, 2, 1> zxzy;};\
struct{::legion::core::math::swizzle<type, 4, 2, 0, 2, 2> zxzz;};\
struct{::legion::core::math::swizzle<type, 4, 2, 0, 2, 3> zxzw;};\
struct{::legion::core::math::swizzle<type, 4, 2, 0, 3, 0> zxwx;};\
struct{::legion::core::math::swizzle<type, 4, 2, 0, 3, 1> zxwy;};\
struct{::legion::core::math::swizzle<type, 4, 2, 0, 3, 2> zxwz;};\
struct{::legion::core::math::swizzle<type, 4, 2, 0, 3, 3> zxww;};\
struct{::legion::core::math::swizzle<type, 4, 2, 1, 0, 0> zyxx;};\
struct{::legion::core::math::swizzle<type, 4, 2, 1, 0, 1> zyxy;};\
struct{::legion::core::math::swizzle<type, 4, 2, 1, 0, 2> zyxz;};\
struct{::legion::core::math::swizzle<type, 4, 2, 1, 0, 3> zyxw;};\
struct{::legion::core::math::swizzle<type, 4, 2, 1, 1, 0> zyyx;};\
struct{::legion::core::math::swizzle<type, 4, 2, 1, 1, 1> zyyy;};\
struct{::legion::core::math::swizzle<type, 4, 2, 1, 1, 2> zyyz;};\
struct{::legion::core::math::swizzle<type, 4, 2, 1, 1, 3> zyyw;};\
struct{::legion::core::math::swizzle<type, 4, 2, 1, 2, 0> zyzx;};\
struct{::legion::core::math::swizzle<type, 4, 2, 1, 2, 1> zyzy;};\
struct{::legion::core::math::swizzle<type, 4, 2, 1, 2, 2> zyzz;};\
struct{::legion::core::math::swizzle<type, 4, 2, 1, 2, 3> zyzw;};\
struct{::legion::core::math::swizzle<type, 4, 2, 1, 3, 0> zywx;};\
struct{::legion::core::math::swizzle<type, 4, 2, 1, 3, 1> zywy;};\
struct{::legion::core::math::swizzle<type, 4, 2, 1, 3, 2> zywz;};\
struct{::legion::core::math::swizzle<type, 4, 2, 1, 3, 3> zyww;};\
struct{::legion::core::math::swizzle<type, 4, 2, 2, 0, 0> zzxx;};\
struct{::legion::core::math::swizzle<type, 4, 2, 2, 0, 1> zzxy;};\
struct{::legion::core::math::swizzle<type, 4, 2, 2, 0, 2> zzxz;};\
struct{::legion::core::math::swizzle<type, 4, 2, 2, 0, 3> zzxw;};\
struct{::legion::core::math::swizzle<type, 4, 2, 2, 1, 0> zzyx;};\
struct{::legion::core::math::swizzle<type, 4, 2, 2, 1, 1> zzyy;};\
struct{::legion::core::math::swizzle<type, 4, 2, 2, 1, 2> zzyz;};\
struct{::legion::core::math::swizzle<type, 4, 2, 2, 1, 3> zzyw;};\
struct{::legion::core::math::swizzle<type, 4, 2, 2, 2, 0> zzzx;};\
struct{::legion::core::math::swizzle<type, 4, 2, 2, 2, 1> zzzy;};\
struct{::legion::core::math::swizzle<type, 4, 2, 2, 2, 2> zzzz;};\
struct{::legion::core::math::swizzle<type, 4, 2, 2, 2, 3> zzzw;};\
struct{::legion::core::math::swizzle<type, 4, 2, 2, 3, 0> zzwx;};\
struct{::legion::core::math::swizzle<type, 4, 2, 2, 3, 1> zzwy;};\
struct{::legion::core::math::swizzle<type, 4, 2, 2, 3, 2> zzwz;};\
struct{::legion::core::math::swizzle<type, 4, 2, 2, 3, 3> zzww;};\
struct{::legion::core::math::swizzle<type, 4, 2, 3, 0, 0> zwxx;};\
struct{::legion::core::math::swizzle<type, 4, 2, 3, 0, 1> zwxy;};\
struct{::legion::core::math::swizzle<type, 4, 2, 3, 0, 2> zwxz;};\
struct{::legion::core::math::swizzle<type, 4, 2, 3, 0, 3> zwxw;};\
struct{::legion::core::math::swizzle<type, 4, 2, 3, 1, 0> zwyx;};\
struct{::legion::core::math::swizzle<type, 4, 2, 3, 1, 1> zwyy;};\
struct{::legion::core::math::swizzle<type, 4, 2, 3, 1, 2> zwyz;};\
struct{::legion::core::math::swizzle<type, 4, 2, 3, 1, 3> zwyw;};\
struct{::legion::core::math::swizzle<type, 4, 2, 3, 2, 0> zwzx;};\
struct{::legion::core::math::swizzle<type, 4, 2, 3, 2, 1> zwzy;};\
struct{::legion::core::math::swizzle<type, 4, 2, 3, 2, 2> zwzz;};\
struct{::legion::core::math::swizzle<type, 4, 2, 3, 2, 3> zwzw;};\
struct{::legion::core::math::swizzle<type, 4, 2, 3, 3, 0> zwwx;};\
struct{::legion::core::math::swizzle<type, 4, 2, 3, 3, 1> zwwy;};\
struct{::legion::core::math::swizzle<type, 4, 2, 3, 3, 2> zwwz;};\
struct{::legion::core::math::swizzle<type, 4, 2, 3, 3, 3> zwww;};\
struct{::legion::core::math::swizzle<type, 4, 3, 0, 0, 0> wxxx;};\
struct{::legion::core::math::swizzle<type, 4, 3, 0, 0, 1> wxxy;};\
struct{::legion::core::math::swizzle<type, 4, 3, 0, 0, 2> wxxz;};\
struct{::legion::core::math::swizzle<type, 4, 3, 0, 0, 3> wxxw;};\
struct{::legion::core::math::swizzle<type, 4, 3, 0, 1, 0> wxyx;};\
struct{::legion::core::math::swizzle<type, 4, 3, 0, 1, 1> wxyy;};\
struct{::legion::core::math::swizzle<type, 4, 3, 0, 1, 2> wxyz;};\
struct{::legion::core::math::swizzle<type, 4, 3, 0, 1, 3> wxyw;};\
struct{::legion::core::math::swizzle<type, 4, 3, 0, 2, 0> wxzx;};\
struct{::legion::core::math::swizzle<type, 4, 3, 0, 2, 1> wxzy;};\
struct{::legion::core::math::swizzle<type, 4, 3, 0, 2, 2> wxzz;};\
struct{::legion::core::math::swizzle<type, 4, 3, 0, 2, 3> wxzw;};\
struct{::legion::core::math::swizzle<type, 4, 3, 0, 3, 0> wxwx;};\
struct{::legion::core::math::swizzle<type, 4, 3, 0, 3, 1> wxwy;};\
struct{::legion::core::math::swizzle<type, 4, 3, 0, 3, 2> wxwz;};\
struct{::legion::core::math::swizzle<type, 4, 3, 0, 3, 3> wxww;};\
struct{::legion::core::math::swizzle<type, 4, 3, 1, 0, 0> wyxx;};\
struct{::legion::core::math::swizzle<type, 4, 3, 1, 0, 1> wyxy;};\
struct{::legion::core::math::swizzle<type, 4, 3, 1, 0, 2> wyxz;};\
struct{::legion::core::math::swizzle<type, 4, 3, 1, 0, 3> wyxw;};\
struct{::legion::core::math::swizzle<type, 4, 3, 1, 1, 0> wyyx;};\
struct{::legion::core::math::swizzle<type, 4, 3, 1, 1, 1> wyyy;};\
struct{::legion::core::math::swizzle<type, 4, 3, 1, 1, 2> wyyz;};\
struct{::legion::core::math::swizzle<type, 4, 3, 1, 1, 3> wyyw;};\
struct{::legion::core::math::swizzle<type, 4, 3, 1, 2, 0> wyzx;};\
struct{::legion::core::math::swizzle<type, 4, 3, 1, 2, 1> wyzy;};\
struct{::legion::core::math::swizzle<type, 4, 3, 1, 2, 2> wyzz;};\
struct{::legion::core::math::swizzle<type, 4, 3, 1, 2, 3> wyzw;};\
struct{::legion::core::math::swizzle<type, 4, 3, 1, 3, 0> wywx;};\
struct{::legion::core::math::swizzle<type, 4, 3, 1, 3, 1> wywy;};\
struct{::legion::core::math::swizzle<type, 4, 3, 1, 3, 2> wywz;};\
struct{::legion::core::math::swizzle<type, 4, 3, 1, 3, 3> wyww;};\
struct{::legion::core::math::swizzle<type, 4, 3, 2, 0, 0> wzxx;};\
struct{::legion::core::math::swizzle<type, 4, 3, 2, 0, 1> wzxy;};\
struct{::legion::core::math::swizzle<type, 4, 3, 2, 0, 2> wzxz;};\
struct{::legion::core::math::swizzle<type, 4, 3, 2, 0, 3> wzxw;};\
struct{::legion::core::math::swizzle<type, 4, 3, 2, 1, 0> wzyx;};\
struct{::legion::core::math::swizzle<type, 4, 3, 2, 1, 1> wzyy;};\
struct{::legion::core::math::swizzle<type, 4, 3, 2, 1, 2> wzyz;};\
struct{::legion::core::math::swizzle<type, 4, 3, 2, 1, 3> wzyw;};\
struct{::legion::core::math::swizzle<type, 4, 3, 2, 2, 0> wzzx;};\
struct{::legion::core::math::swizzle<type, 4, 3, 2, 2, 1> wzzy;};\
struct{::legion::core::math::swizzle<type, 4, 3, 2, 2, 2> wzzz;};\
struct{::legion::core::math::swizzle<type, 4, 3, 2, 2, 3> wzzw;};\
struct{::legion::core::math::swizzle<type, 4, 3, 2, 3, 0> wzwx;};\
struct{::legion::core::math::swizzle<type, 4, 3, 2, 3, 1> wzwy;};\
struct{::legion::core::math::swizzle<type, 4, 3, 2, 3, 2> wzwz;};\
struct{::legion::core::math::swizzle<type, 4, 3, 2, 3, 3> wzww;};\
struct{::legion::core::math::swizzle<type, 4, 3, 3, 0, 0> wwxx;};\
struct{::legion::core::math::swizzle<type, 4, 3, 3, 0, 1> wwxy;};\
struct{::legion::core::math::swizzle<type, 4, 3, 3, 0, 2> wwxz;};\
struct{::legion::core::math::swizzle<type, 4, 3, 3, 0, 3> wwxw;};\
struct{::legion::core::math::swizzle<type, 4, 3, 3, 1, 0> wwyx;};\
struct{::legion::core::math::swizzle<type, 4, 3, 3, 1, 1> wwyy;};\
struct{::legion::core::math::swizzle<type, 4, 3, 3, 1, 2> wwyz;};\
struct{::legion::core::math::swizzle<type, 4, 3, 3, 1, 3> wwyw;};\
struct{::legion::core::math::swizzle<type, 4, 3, 3, 2, 0> wwzx;};\
struct{::legion::core::math::swizzle<type, 4, 3, 3, 2, 1> wwzy;};\
struct{::legion::core::math::swizzle<type, 4, 3, 3, 2, 2> wwzz;};\
struct{::legion::core::math::swizzle<type, 4, 3, 3, 2, 3> wwzw;};\
struct{::legion::core::math::swizzle<type, 4, 3, 3, 3, 0> wwwx;};\
struct{::legion::core::math::swizzle<type, 4, 3, 3, 3, 1> wwwy;};\
struct{::legion::core::math::swizzle<type, 4, 3, 3, 3, 2> wwwz;};\
struct{::legion::core::math::swizzle<type, 4, 3, 3, 3, 3> wwww;};\
\
struct{::legion::core::math::swizzle<type, 4, 0, 0, 0, 0> rrrr;};\
struct{::legion::core::math::swizzle<type, 4, 0, 0, 0, 1> rrrg;};\
struct{::legion::core::math::swizzle<type, 4, 0, 0, 0, 2> rrrb;};\
struct{::legion::core::math::swizzle<type, 4, 0, 0, 0, 3> rrra;};\
struct{::legion::core::math::swizzle<type, 4, 0, 0, 1, 0> rrgr;};\
struct{::legion::core::math::swizzle<type, 4, 0, 0, 1, 1> rrgg;};\
struct{::legion::core::math::swizzle<type, 4, 0, 0, 1, 2> rrgb;};\
struct{::legion::core::math::swizzle<type, 4, 0, 0, 1, 3> rrga;};\
struct{::legion::core::math::swizzle<type, 4, 0, 0, 2, 0> rrbr;};\
struct{::legion::core::math::swizzle<type, 4, 0, 0, 2, 1> rrbg;};\
struct{::legion::core::math::swizzle<type, 4, 0, 0, 2, 2> rrbb;};\
struct{::legion::core::math::swizzle<type, 4, 0, 0, 2, 3> rrba;};\
struct{::legion::core::math::swizzle<type, 4, 0, 0, 3, 0> rrar;};\
struct{::legion::core::math::swizzle<type, 4, 0, 0, 3, 1> rrag;};\
struct{::legion::core::math::swizzle<type, 4, 0, 0, 3, 2> rrab;};\
struct{::legion::core::math::swizzle<type, 4, 0, 0, 3, 3> rraa;};\
struct{::legion::core::math::swizzle<type, 4, 0, 1, 0, 0> rgrr;};\
struct{::legion::core::math::swizzle<type, 4, 0, 1, 0, 1> rgrg;};\
struct{::legion::core::math::swizzle<type, 4, 0, 1, 0, 2> rgrb;};\
struct{::legion::core::math::swizzle<type, 4, 0, 1, 0, 3> rgra;};\
struct{::legion::core::math::swizzle<type, 4, 0, 1, 1, 0> rggr;};\
struct{::legion::core::math::swizzle<type, 4, 0, 1, 1, 1> rggg;};\
struct{::legion::core::math::swizzle<type, 4, 0, 1, 1, 2> rggb;};\
struct{::legion::core::math::swizzle<type, 4, 0, 1, 1, 3> rgga;};\
struct{::legion::core::math::swizzle<type, 4, 0, 1, 2, 0> rgbr;};\
struct{::legion::core::math::swizzle<type, 4, 0, 1, 2, 1> rgbg;};\
struct{::legion::core::math::swizzle<type, 4, 0, 1, 2, 2> rgbb;};\
struct{::legion::core::math::swizzle<type, 4, 0, 1, 2, 3> rgba;};\
struct{::legion::core::math::swizzle<type, 4, 0, 1, 3, 0> rgar;};\
struct{::legion::core::math::swizzle<type, 4, 0, 1, 3, 1> rgag;};\
struct{::legion::core::math::swizzle<type, 4, 0, 1, 3, 2> rgab;};\
struct{::legion::core::math::swizzle<type, 4, 0, 1, 3, 3> rgaa;};\
struct{::legion::core::math::swizzle<type, 4, 0, 2, 0, 0> rbrr;};\
struct{::legion::core::math::swizzle<type, 4, 0, 2, 0, 1> rbrg;};\
struct{::legion::core::math::swizzle<type, 4, 0, 2, 0, 2> rbrb;};\
struct{::legion::core::math::swizzle<type, 4, 0, 2, 0, 3> rbra;};\
struct{::legion::core::math::swizzle<type, 4, 0, 2, 1, 0> rbgr;};\
struct{::legion::core::math::swizzle<type, 4, 0, 2, 1, 1> rbgg;};\
struct{::legion::core::math::swizzle<type, 4, 0, 2, 1, 2> rbgb;};\
struct{::legion::core::math::swizzle<type, 4, 0, 2, 1, 3> rbga;};\
struct{::legion::core::math::swizzle<type, 4, 0, 2, 2, 0> rbbr;};\
struct{::legion::core::math::swizzle<type, 4, 0, 2, 2, 1> rbbg;};\
struct{::legion::core::math::swizzle<type, 4, 0, 2, 2, 2> rbbb;};\
struct{::legion::core::math::swizzle<type, 4, 0, 2, 2, 3> rbba;};\
struct{::legion::core::math::swizzle<type, 4, 0, 2, 3, 0> rbar;};\
struct{::legion::core::math::swizzle<type, 4, 0, 2, 3, 1> rbag;};\
struct{::legion::core::math::swizzle<type, 4, 0, 2, 3, 2> rbab;};\
struct{::legion::core::math::swizzle<type, 4, 0, 2, 3, 3> rbaa;};\
struct{::legion::core::math::swizzle<type, 4, 0, 3, 0, 0> rarr;};\
struct{::legion::core::math::swizzle<type, 4, 0, 3, 0, 1> rarg;};\
struct{::legion::core::math::swizzle<type, 4, 0, 3, 0, 2> rarb;};\
struct{::legion::core::math::swizzle<type, 4, 0, 3, 0, 3> rara;};\
struct{::legion::core::math::swizzle<type, 4, 0, 3, 1, 0> ragr;};\
struct{::legion::core::math::swizzle<type, 4, 0, 3, 1, 1> ragg;};\
struct{::legion::core::math::swizzle<type, 4, 0, 3, 1, 2> ragb;};\
struct{::legion::core::math::swizzle<type, 4, 0, 3, 1, 3> raga;};\
struct{::legion::core::math::swizzle<type, 4, 0, 3, 2, 0> rabr;};\
struct{::legion::core::math::swizzle<type, 4, 0, 3, 2, 1> rabg;};\
struct{::legion::core::math::swizzle<type, 4, 0, 3, 2, 2> rabb;};\
struct{::legion::core::math::swizzle<type, 4, 0, 3, 2, 3> raba;};\
struct{::legion::core::math::swizzle<type, 4, 0, 3, 3, 0> raar;};\
struct{::legion::core::math::swizzle<type, 4, 0, 3, 3, 1> raag;};\
struct{::legion::core::math::swizzle<type, 4, 0, 3, 3, 2> raab;};\
struct{::legion::core::math::swizzle<type, 4, 0, 3, 3, 3> raaa;};\
struct{::legion::core::math::swizzle<type, 4, 1, 0, 0, 0> grrr;};\
struct{::legion::core::math::swizzle<type, 4, 1, 0, 0, 1> grrg;};\
struct{::legion::core::math::swizzle<type, 4, 1, 0, 0, 2> grrb;};\
struct{::legion::core::math::swizzle<type, 4, 1, 0, 0, 3> grra;};\
struct{::legion::core::math::swizzle<type, 4, 1, 0, 1, 0> grgr;};\
struct{::legion::core::math::swizzle<type, 4, 1, 0, 1, 1> grgg;};\
struct{::legion::core::math::swizzle<type, 4, 1, 0, 1, 2> grgb;};\
struct{::legion::core::math::swizzle<type, 4, 1, 0, 1, 3> grga;};\
struct{::legion::core::math::swizzle<type, 4, 1, 0, 2, 0> grbr;};\
struct{::legion::core::math::swizzle<type, 4, 1, 0, 2, 1> grbg;};\
struct{::legion::core::math::swizzle<type, 4, 1, 0, 2, 2> grbb;};\
struct{::legion::core::math::swizzle<type, 4, 1, 0, 2, 3> grba;};\
struct{::legion::core::math::swizzle<type, 4, 1, 0, 3, 0> grar;};\
struct{::legion::core::math::swizzle<type, 4, 1, 0, 3, 1> grag;};\
struct{::legion::core::math::swizzle<type, 4, 1, 0, 3, 2> grab;};\
struct{::legion::core::math::swizzle<type, 4, 1, 0, 3, 3> graa;};\
struct{::legion::core::math::swizzle<type, 4, 1, 1, 0, 0> ggrr;};\
struct{::legion::core::math::swizzle<type, 4, 1, 1, 0, 1> ggrg;};\
struct{::legion::core::math::swizzle<type, 4, 1, 1, 0, 2> ggrb;};\
struct{::legion::core::math::swizzle<type, 4, 1, 1, 0, 3> ggra;};\
struct{::legion::core::math::swizzle<type, 4, 1, 1, 1, 0> gggr;};\
struct{::legion::core::math::swizzle<type, 4, 1, 1, 1, 1> gggg;};\
struct{::legion::core::math::swizzle<type, 4, 1, 1, 1, 2> gggb;};\
struct{::legion::core::math::swizzle<type, 4, 1, 1, 1, 3> ggga;};\
struct{::legion::core::math::swizzle<type, 4, 1, 1, 2, 0> ggbr;};\
struct{::legion::core::math::swizzle<type, 4, 1, 1, 2, 1> ggbg;};\
struct{::legion::core::math::swizzle<type, 4, 1, 1, 2, 2> ggbb;};\
struct{::legion::core::math::swizzle<type, 4, 1, 1, 2, 3> ggba;};\
struct{::legion::core::math::swizzle<type, 4, 1, 1, 3, 0> ggar;};\
struct{::legion::core::math::swizzle<type, 4, 1, 1, 3, 1> ggag;};\
struct{::legion::core::math::swizzle<type, 4, 1, 1, 3, 2> ggab;};\
struct{::legion::core::math::swizzle<type, 4, 1, 1, 3, 3> ggaa;};\
struct{::legion::core::math::swizzle<type, 4, 1, 2, 0, 0> gbrr;};\
struct{::legion::core::math::swizzle<type, 4, 1, 2, 0, 1> gbrg;};\
struct{::legion::core::math::swizzle<type, 4, 1, 2, 0, 2> gbrb;};\
struct{::legion::core::math::swizzle<type, 4, 1, 2, 0, 3> gbra;};\
struct{::legion::core::math::swizzle<type, 4, 1, 2, 1, 0> gbgr;};\
struct{::legion::core::math::swizzle<type, 4, 1, 2, 1, 1> gbgg;};\
struct{::legion::core::math::swizzle<type, 4, 1, 2, 1, 2> gbgb;};\
struct{::legion::core::math::swizzle<type, 4, 1, 2, 1, 3> gbga;};\
struct{::legion::core::math::swizzle<type, 4, 1, 2, 2, 0> gbbr;};\
struct{::legion::core::math::swizzle<type, 4, 1, 2, 2, 1> gbbg;};\
struct{::legion::core::math::swizzle<type, 4, 1, 2, 2, 2> gbbb;};\
struct{::legion::core::math::swizzle<type, 4, 1, 2, 2, 3> gbba;};\
struct{::legion::core::math::swizzle<type, 4, 1, 2, 3, 0> gbar;};\
struct{::legion::core::math::swizzle<type, 4, 1, 2, 3, 1> gbag;};\
struct{::legion::core::math::swizzle<type, 4, 1, 2, 3, 2> gbab;};\
struct{::legion::core::math::swizzle<type, 4, 1, 2, 3, 3> gbaa;};\
struct{::legion::core::math::swizzle<type, 4, 1, 3, 0, 0> garr;};\
struct{::legion::core::math::swizzle<type, 4, 1, 3, 0, 1> garg;};\
struct{::legion::core::math::swizzle<type, 4, 1, 3, 0, 2> garb;};\
struct{::legion::core::math::swizzle<type, 4, 1, 3, 0, 3> gara;};\
struct{::legion::core::math::swizzle<type, 4, 1, 3, 1, 0> gagr;};\
struct{::legion::core::math::swizzle<type, 4, 1, 3, 1, 1> gagg;};\
struct{::legion::core::math::swizzle<type, 4, 1, 3, 1, 2> gagb;};\
struct{::legion::core::math::swizzle<type, 4, 1, 3, 1, 3> gaga;};\
struct{::legion::core::math::swizzle<type, 4, 1, 3, 2, 0> gabr;};\
struct{::legion::core::math::swizzle<type, 4, 1, 3, 2, 1> gabg;};\
struct{::legion::core::math::swizzle<type, 4, 1, 3, 2, 2> gabb;};\
struct{::legion::core::math::swizzle<type, 4, 1, 3, 2, 3> gaba;};\
struct{::legion::core::math::swizzle<type, 4, 1, 3, 3, 0> gaar;};\
struct{::legion::core::math::swizzle<type, 4, 1, 3, 3, 1> gaag;};\
struct{::legion::core::math::swizzle<type, 4, 1, 3, 3, 2> gaab;};\
struct{::legion::core::math::swizzle<type, 4, 1, 3, 3, 3> gaaa;};\
struct{::legion::core::math::swizzle<type, 4, 2, 0, 0, 0> brrr;};\
struct{::legion::core::math::swizzle<type, 4, 2, 0, 0, 1> brrg;};\
struct{::legion::core::math::swizzle<type, 4, 2, 0, 0, 2> brrb;};\
struct{::legion::core::math::swizzle<type, 4, 2, 0, 0, 3> brra;};\
struct{::legion::core::math::swizzle<type, 4, 2, 0, 1, 0> brgr;};\
struct{::legion::core::math::swizzle<type, 4, 2, 0, 1, 1> brgg;};\
struct{::legion::core::math::swizzle<type, 4, 2, 0, 1, 2> brgb;};\
struct{::legion::core::math::swizzle<type, 4, 2, 0, 1, 3> brga;};\
struct{::legion::core::math::swizzle<type, 4, 2, 0, 2, 0> brbr;};\
struct{::legion::core::math::swizzle<type, 4, 2, 0, 2, 1> brbg;};\
struct{::legion::core::math::swizzle<type, 4, 2, 0, 2, 2> brbb;};\
struct{::legion::core::math::swizzle<type, 4, 2, 0, 2, 3> brba;};\
struct{::legion::core::math::swizzle<type, 4, 2, 0, 3, 0> brar;};\
struct{::legion::core::math::swizzle<type, 4, 2, 0, 3, 1> brag;};\
struct{::legion::core::math::swizzle<type, 4, 2, 0, 3, 2> brab;};\
struct{::legion::core::math::swizzle<type, 4, 2, 0, 3, 3> braa;};\
struct{::legion::core::math::swizzle<type, 4, 2, 1, 0, 0> bgrr;};\
struct{::legion::core::math::swizzle<type, 4, 2, 1, 0, 1> bgrg;};\
struct{::legion::core::math::swizzle<type, 4, 2, 1, 0, 2> bgrb;};\
struct{::legion::core::math::swizzle<type, 4, 2, 1, 0, 3> bgra;};\
struct{::legion::core::math::swizzle<type, 4, 2, 1, 1, 0> bggr;};\
struct{::legion::core::math::swizzle<type, 4, 2, 1, 1, 1> bggg;};\
struct{::legion::core::math::swizzle<type, 4, 2, 1, 1, 2> bggb;};\
struct{::legion::core::math::swizzle<type, 4, 2, 1, 1, 3> bgga;};\
struct{::legion::core::math::swizzle<type, 4, 2, 1, 2, 0> bgbr;};\
struct{::legion::core::math::swizzle<type, 4, 2, 1, 2, 1> bgbg;};\
struct{::legion::core::math::swizzle<type, 4, 2, 1, 2, 2> bgbb;};\
struct{::legion::core::math::swizzle<type, 4, 2, 1, 2, 3> bgba;};\
struct{::legion::core::math::swizzle<type, 4, 2, 1, 3, 0> bgar;};\
struct{::legion::core::math::swizzle<type, 4, 2, 1, 3, 1> bgag;};\
struct{::legion::core::math::swizzle<type, 4, 2, 1, 3, 2> bgab;};\
struct{::legion::core::math::swizzle<type, 4, 2, 1, 3, 3> bgaa;};\
struct{::legion::core::math::swizzle<type, 4, 2, 2, 0, 0> bbrr;};\
struct{::legion::core::math::swizzle<type, 4, 2, 2, 0, 1> bbrg;};\
struct{::legion::core::math::swizzle<type, 4, 2, 2, 0, 2> bbrb;};\
struct{::legion::core::math::swizzle<type, 4, 2, 2, 0, 3> bbra;};\
struct{::legion::core::math::swizzle<type, 4, 2, 2, 1, 0> bbgr;};\
struct{::legion::core::math::swizzle<type, 4, 2, 2, 1, 1> bbgg;};\
struct{::legion::core::math::swizzle<type, 4, 2, 2, 1, 2> bbgb;};\
struct{::legion::core::math::swizzle<type, 4, 2, 2, 1, 3> bbga;};\
struct{::legion::core::math::swizzle<type, 4, 2, 2, 2, 0> bbbr;};\
struct{::legion::core::math::swizzle<type, 4, 2, 2, 2, 1> bbbg;};\
struct{::legion::core::math::swizzle<type, 4, 2, 2, 2, 2> bbbb;};\
struct{::legion::core::math::swizzle<type, 4, 2, 2, 2, 3> bbba;};\
struct{::legion::core::math::swizzle<type, 4, 2, 2, 3, 0> bbar;};\
struct{::legion::core::math::swizzle<type, 4, 2, 2, 3, 1> bbag;};\
struct{::legion::core::math::swizzle<type, 4, 2, 2, 3, 2> bbab;};\
struct{::legion::core::math::swizzle<type, 4, 2, 2, 3, 3> bbaa;};\
struct{::legion::core::math::swizzle<type, 4, 2, 3, 0, 0> barr;};\
struct{::legion::core::math::swizzle<type, 4, 2, 3, 0, 1> barg;};\
struct{::legion::core::math::swizzle<type, 4, 2, 3, 0, 2> barb;};\
struct{::legion::core::math::swizzle<type, 4, 2, 3, 0, 3> bara;};\
struct{::legion::core::math::swizzle<type, 4, 2, 3, 1, 0> bagr;};\
struct{::legion::core::math::swizzle<type, 4, 2, 3, 1, 1> bagg;};\
struct{::legion::core::math::swizzle<type, 4, 2, 3, 1, 2> bagb;};\
struct{::legion::core::math::swizzle<type, 4, 2, 3, 1, 3> baga;};\
struct{::legion::core::math::swizzle<type, 4, 2, 3, 2, 0> babr;};\
struct{::legion::core::math::swizzle<type, 4, 2, 3, 2, 1> babg;};\
struct{::legion::core::math::swizzle<type, 4, 2, 3, 2, 2> babb;};\
struct{::legion::core::math::swizzle<type, 4, 2, 3, 2, 3> baba;};\
struct{::legion::core::math::swizzle<type, 4, 2, 3, 3, 0> baar;};\
struct{::legion::core::math::swizzle<type, 4, 2, 3, 3, 1> baag;};\
struct{::legion::core::math::swizzle<type, 4, 2, 3, 3, 2> baab;};\
struct{::legion::core::math::swizzle<type, 4, 2, 3, 3, 3> baaa;};\
struct{::legion::core::math::swizzle<type, 4, 3, 0, 0, 0> arrr;};\
struct{::legion::core::math::swizzle<type, 4, 3, 0, 0, 1> arrg;};\
struct{::legion::core::math::swizzle<type, 4, 3, 0, 0, 2> arrb;};\
struct{::legion::core::math::swizzle<type, 4, 3, 0, 0, 3> arra;};\
struct{::legion::core::math::swizzle<type, 4, 3, 0, 1, 0> argr;};\
struct{::legion::core::math::swizzle<type, 4, 3, 0, 1, 1> argg;};\
struct{::legion::core::math::swizzle<type, 4, 3, 0, 1, 2> argb;};\
struct{::legion::core::math::swizzle<type, 4, 3, 0, 1, 3> arga;};\
struct{::legion::core::math::swizzle<type, 4, 3, 0, 2, 0> arbr;};\
struct{::legion::core::math::swizzle<type, 4, 3, 0, 2, 1> arbg;};\
struct{::legion::core::math::swizzle<type, 4, 3, 0, 2, 2> arbb;};\
struct{::legion::core::math::swizzle<type, 4, 3, 0, 2, 3> arba;};\
struct{::legion::core::math::swizzle<type, 4, 3, 0, 3, 0> arar;};\
struct{::legion::core::math::swizzle<type, 4, 3, 0, 3, 1> arag;};\
struct{::legion::core::math::swizzle<type, 4, 3, 0, 3, 2> arab;};\
struct{::legion::core::math::swizzle<type, 4, 3, 0, 3, 3> araa;};\
struct{::legion::core::math::swizzle<type, 4, 3, 1, 0, 0> agrr;};\
struct{::legion::core::math::swizzle<type, 4, 3, 1, 0, 1> agrg;};\
struct{::legion::core::math::swizzle<type, 4, 3, 1, 0, 2> agrb;};\
struct{::legion::core::math::swizzle<type, 4, 3, 1, 0, 3> agra;};\
struct{::legion::core::math::swizzle<type, 4, 3, 1, 1, 0> aggr;};\
struct{::legion::core::math::swizzle<type, 4, 3, 1, 1, 1> aggg;};\
struct{::legion::core::math::swizzle<type, 4, 3, 1, 1, 2> aggb;};\
struct{::legion::core::math::swizzle<type, 4, 3, 1, 1, 3> agga;};\
struct{::legion::core::math::swizzle<type, 4, 3, 1, 2, 0> agbr;};\
struct{::legion::core::math::swizzle<type, 4, 3, 1, 2, 1> agbg;};\
struct{::legion::core::math::swizzle<type, 4, 3, 1, 2, 2> agbb;};\
struct{::legion::core::math::swizzle<type, 4, 3, 1, 2, 3> agba;};\
struct{::legion::core::math::swizzle<type, 4, 3, 1, 3, 0> agar;};\
struct{::legion::core::math::swizzle<type, 4, 3, 1, 3, 1> agag;};\
struct{::legion::core::math::swizzle<type, 4, 3, 1, 3, 2> agab;};\
struct{::legion::core::math::swizzle<type, 4, 3, 1, 3, 3> agaa;};\
struct{::legion::core::math::swizzle<type, 4, 3, 2, 0, 0> abrr;};\
struct{::legion::core::math::swizzle<type, 4, 3, 2, 0, 1> abrg;};\
struct{::legion::core::math::swizzle<type, 4, 3, 2, 0, 2> abrb;};\
struct{::legion::core::math::swizzle<type, 4, 3, 2, 0, 3> abra;};\
struct{::legion::core::math::swizzle<type, 4, 3, 2, 1, 0> abgr;};\
struct{::legion::core::math::swizzle<type, 4, 3, 2, 1, 1> abgg;};\
struct{::legion::core::math::swizzle<type, 4, 3, 2, 1, 2> abgb;};\
struct{::legion::core::math::swizzle<type, 4, 3, 2, 1, 3> abga;};\
struct{::legion::core::math::swizzle<type, 4, 3, 2, 2, 0> abbr;};\
struct{::legion::core::math::swizzle<type, 4, 3, 2, 2, 1> abbg;};\
struct{::legion::core::math::swizzle<type, 4, 3, 2, 2, 2> abbb;};\
struct{::legion::core::math::swizzle<type, 4, 3, 2, 2, 3> abba;};\
struct{::legion::core::math::swizzle<type, 4, 3, 2, 3, 0> abar;};\
struct{::legion::core::math::swizzle<type, 4, 3, 2, 3, 1> abag;};\
struct{::legion::core::math::swizzle<type, 4, 3, 2, 3, 2> abab;};\
struct{::legion::core::math::swizzle<type, 4, 3, 2, 3, 3> abaa;};\
struct{::legion::core::math::swizzle<type, 4, 3, 3, 0, 0> aarr;};\
struct{::legion::core::math::swizzle<type, 4, 3, 3, 0, 1> aarg;};\
struct{::legion::core::math::swizzle<type, 4, 3, 3, 0, 2> aarb;};\
struct{::legion::core::math::swizzle<type, 4, 3, 3, 0, 3> aara;};\
struct{::legion::core::math::swizzle<type, 4, 3, 3, 1, 0> aagr;};\
struct{::legion::core::math::swizzle<type, 4, 3, 3, 1, 1> aagg;};\
struct{::legion::core::math::swizzle<type, 4, 3, 3, 1, 2> aagb;};\
struct{::legion::core::math::swizzle<type, 4, 3, 3, 1, 3> aaga;};\
struct{::legion::core::math::swizzle<type, 4, 3, 3, 2, 0> aabr;};\
struct{::legion::core::math::swizzle<type, 4, 3, 3, 2, 1> aabg;};\
struct{::legion::core::math::swizzle<type, 4, 3, 3, 2, 2> aabb;};\
struct{::legion::core::math::swizzle<type, 4, 3, 3, 2, 3> aaba;};\
struct{::legion::core::math::swizzle<type, 4, 3, 3, 3, 0> aaar;};\
struct{::legion::core::math::swizzle<type, 4, 3, 3, 3, 1> aaag;};\
struct{::legion::core::math::swizzle<type, 4, 3, 3, 3, 2> aaab;};\
struct{::legion::core::math::swizzle<type, 4, 3, 3, 3, 3> aaaa;};\
\
struct{::legion::core::math::swizzle<type, 4, 0, 0, 0, 0> ssss;};\
struct{::legion::core::math::swizzle<type, 4, 0, 0, 0, 1> ssst;};\
struct{::legion::core::math::swizzle<type, 4, 0, 0, 0, 2> sssp;};\
struct{::legion::core::math::swizzle<type, 4, 0, 0, 0, 3> sssq;};\
struct{::legion::core::math::swizzle<type, 4, 0, 0, 1, 0> ssts;};\
struct{::legion::core::math::swizzle<type, 4, 0, 0, 1, 1> sstt;};\
struct{::legion::core::math::swizzle<type, 4, 0, 0, 1, 2> sstp;};\
struct{::legion::core::math::swizzle<type, 4, 0, 0, 1, 3> sstq;};\
struct{::legion::core::math::swizzle<type, 4, 0, 0, 2, 0> ssps;};\
struct{::legion::core::math::swizzle<type, 4, 0, 0, 2, 1> sspt;};\
struct{::legion::core::math::swizzle<type, 4, 0, 0, 2, 2> sspp;};\
struct{::legion::core::math::swizzle<type, 4, 0, 0, 2, 3> sspq;};\
struct{::legion::core::math::swizzle<type, 4, 0, 0, 3, 0> ssqs;};\
struct{::legion::core::math::swizzle<type, 4, 0, 0, 3, 1> ssqt;};\
struct{::legion::core::math::swizzle<type, 4, 0, 0, 3, 2> ssqp;};\
struct{::legion::core::math::swizzle<type, 4, 0, 0, 3, 3> ssqq;};\
struct{::legion::core::math::swizzle<type, 4, 0, 1, 0, 0> stss;};\
struct{::legion::core::math::swizzle<type, 4, 0, 1, 0, 1> stst;};\
struct{::legion::core::math::swizzle<type, 4, 0, 1, 0, 2> stsp;};\
struct{::legion::core::math::swizzle<type, 4, 0, 1, 0, 3> stsq;};\
struct{::legion::core::math::swizzle<type, 4, 0, 1, 1, 0> stts;};\
struct{::legion::core::math::swizzle<type, 4, 0, 1, 1, 1> sttt;};\
struct{::legion::core::math::swizzle<type, 4, 0, 1, 1, 2> sttp;};\
struct{::legion::core::math::swizzle<type, 4, 0, 1, 1, 3> sttq;};\
struct{::legion::core::math::swizzle<type, 4, 0, 1, 2, 0> stps;};\
struct{::legion::core::math::swizzle<type, 4, 0, 1, 2, 1> stpt;};\
struct{::legion::core::math::swizzle<type, 4, 0, 1, 2, 2> stpp;};\
struct{::legion::core::math::swizzle<type, 4, 0, 1, 2, 3> stpq;};\
struct{::legion::core::math::swizzle<type, 4, 0, 1, 3, 0> stqs;};\
struct{::legion::core::math::swizzle<type, 4, 0, 1, 3, 1> stqt;};\
struct{::legion::core::math::swizzle<type, 4, 0, 1, 3, 2> stqp;};\
struct{::legion::core::math::swizzle<type, 4, 0, 1, 3, 3> stqq;};\
struct{::legion::core::math::swizzle<type, 4, 0, 2, 0, 0> spss;};\
struct{::legion::core::math::swizzle<type, 4, 0, 2, 0, 1> spst;};\
struct{::legion::core::math::swizzle<type, 4, 0, 2, 0, 2> spsp;};\
struct{::legion::core::math::swizzle<type, 4, 0, 2, 0, 3> spsq;};\
struct{::legion::core::math::swizzle<type, 4, 0, 2, 1, 0> spts;};\
struct{::legion::core::math::swizzle<type, 4, 0, 2, 1, 1> sptt;};\
struct{::legion::core::math::swizzle<type, 4, 0, 2, 1, 2> sptp;};\
struct{::legion::core::math::swizzle<type, 4, 0, 2, 1, 3> sptq;};\
struct{::legion::core::math::swizzle<type, 4, 0, 2, 2, 0> spps;};\
struct{::legion::core::math::swizzle<type, 4, 0, 2, 2, 1> sppt;};\
struct{::legion::core::math::swizzle<type, 4, 0, 2, 2, 2> sppp;};\
struct{::legion::core::math::swizzle<type, 4, 0, 2, 2, 3> sppq;};\
struct{::legion::core::math::swizzle<type, 4, 0, 2, 3, 0> spqs;};\
struct{::legion::core::math::swizzle<type, 4, 0, 2, 3, 1> spqt;};\
struct{::legion::core::math::swizzle<type, 4, 0, 2, 3, 2> spqp;};\
struct{::legion::core::math::swizzle<type, 4, 0, 2, 3, 3> spqq;};\
struct{::legion::core::math::swizzle<type, 4, 0, 3, 0, 0> sqss;};\
struct{::legion::core::math::swizzle<type, 4, 0, 3, 0, 1> sqst;};\
struct{::legion::core::math::swizzle<type, 4, 0, 3, 0, 2> sqsp;};\
struct{::legion::core::math::swizzle<type, 4, 0, 3, 0, 3> sqsq;};\
struct{::legion::core::math::swizzle<type, 4, 0, 3, 1, 0> sqts;};\
struct{::legion::core::math::swizzle<type, 4, 0, 3, 1, 1> sqtt;};\
struct{::legion::core::math::swizzle<type, 4, 0, 3, 1, 2> sqtp;};\
struct{::legion::core::math::swizzle<type, 4, 0, 3, 1, 3> sqtq;};\
struct{::legion::core::math::swizzle<type, 4, 0, 3, 2, 0> sqps;};\
struct{::legion::core::math::swizzle<type, 4, 0, 3, 2, 1> sqpt;};\
struct{::legion::core::math::swizzle<type, 4, 0, 3, 2, 2> sqpp;};\
struct{::legion::core::math::swizzle<type, 4, 0, 3, 2, 3> sqpq;};\
struct{::legion::core::math::swizzle<type, 4, 0, 3, 3, 0> sqqs;};\
struct{::legion::core::math::swizzle<type, 4, 0, 3, 3, 1> sqqt;};\
struct{::legion::core::math::swizzle<type, 4, 0, 3, 3, 2> sqqp;};\
struct{::legion::core::math::swizzle<type, 4, 0, 3, 3, 3> sqqq;};\
struct{::legion::core::math::swizzle<type, 4, 1, 0, 0, 0> tsss;};\
struct{::legion::core::math::swizzle<type, 4, 1, 0, 0, 1> tsst;};\
struct{::legion::core::math::swizzle<type, 4, 1, 0, 0, 2> tssp;};\
struct{::legion::core::math::swizzle<type, 4, 1, 0, 0, 3> tssq;};\
struct{::legion::core::math::swizzle<type, 4, 1, 0, 1, 0> tsts;};\
struct{::legion::core::math::swizzle<type, 4, 1, 0, 1, 1> tstt;};\
struct{::legion::core::math::swizzle<type, 4, 1, 0, 1, 2> tstp;};\
struct{::legion::core::math::swizzle<type, 4, 1, 0, 1, 3> tstq;};\
struct{::legion::core::math::swizzle<type, 4, 1, 0, 2, 0> tsps;};\
struct{::legion::core::math::swizzle<type, 4, 1, 0, 2, 1> tspt;};\
struct{::legion::core::math::swizzle<type, 4, 1, 0, 2, 2> tspp;};\
struct{::legion::core::math::swizzle<type, 4, 1, 0, 2, 3> tspq;};\
struct{::legion::core::math::swizzle<type, 4, 1, 0, 3, 0> tsqs;};\
struct{::legion::core::math::swizzle<type, 4, 1, 0, 3, 1> tsqt;};\
struct{::legion::core::math::swizzle<type, 4, 1, 0, 3, 2> tsqp;};\
struct{::legion::core::math::swizzle<type, 4, 1, 0, 3, 3> tsqq;};\
struct{::legion::core::math::swizzle<type, 4, 1, 1, 0, 0> ttss;};\
struct{::legion::core::math::swizzle<type, 4, 1, 1, 0, 1> ttst;};\
struct{::legion::core::math::swizzle<type, 4, 1, 1, 0, 2> ttsp;};\
struct{::legion::core::math::swizzle<type, 4, 1, 1, 0, 3> ttsq;};\
struct{::legion::core::math::swizzle<type, 4, 1, 1, 1, 0> ttts;};\
struct{::legion::core::math::swizzle<type, 4, 1, 1, 1, 1> tttt;};\
struct{::legion::core::math::swizzle<type, 4, 1, 1, 1, 2> tttp;};\
struct{::legion::core::math::swizzle<type, 4, 1, 1, 1, 3> tttq;};\
struct{::legion::core::math::swizzle<type, 4, 1, 1, 2, 0> ttps;};\
struct{::legion::core::math::swizzle<type, 4, 1, 1, 2, 1> ttpt;};\
struct{::legion::core::math::swizzle<type, 4, 1, 1, 2, 2> ttpp;};\
struct{::legion::core::math::swizzle<type, 4, 1, 1, 2, 3> ttpq;};\
struct{::legion::core::math::swizzle<type, 4, 1, 1, 3, 0> ttqs;};\
struct{::legion::core::math::swizzle<type, 4, 1, 1, 3, 1> ttqt;};\
struct{::legion::core::math::swizzle<type, 4, 1, 1, 3, 2> ttqp;};\
struct{::legion::core::math::swizzle<type, 4, 1, 1, 3, 3> ttqq;};\
struct{::legion::core::math::swizzle<type, 4, 1, 2, 0, 0> tpss;};\
struct{::legion::core::math::swizzle<type, 4, 1, 2, 0, 1> tpst;};\
struct{::legion::core::math::swizzle<type, 4, 1, 2, 0, 2> tpsp;};\
struct{::legion::core::math::swizzle<type, 4, 1, 2, 0, 3> tpsq;};\
struct{::legion::core::math::swizzle<type, 4, 1, 2, 1, 0> tpts;};\
struct{::legion::core::math::swizzle<type, 4, 1, 2, 1, 1> tptt;};\
struct{::legion::core::math::swizzle<type, 4, 1, 2, 1, 2> tptp;};\
struct{::legion::core::math::swizzle<type, 4, 1, 2, 1, 3> tptq;};\
struct{::legion::core::math::swizzle<type, 4, 1, 2, 2, 0> tpps;};\
struct{::legion::core::math::swizzle<type, 4, 1, 2, 2, 1> tppt;};\
struct{::legion::core::math::swizzle<type, 4, 1, 2, 2, 2> tppp;};\
struct{::legion::core::math::swizzle<type, 4, 1, 2, 2, 3> tppq;};\
struct{::legion::core::math::swizzle<type, 4, 1, 2, 3, 0> tpqs;};\
struct{::legion::core::math::swizzle<type, 4, 1, 2, 3, 1> tpqt;};\
struct{::legion::core::math::swizzle<type, 4, 1, 2, 3, 2> tpqp;};\
struct{::legion::core::math::swizzle<type, 4, 1, 2, 3, 3> tpqq;};\
struct{::legion::core::math::swizzle<type, 4, 1, 3, 0, 0> tqss;};\
struct{::legion::core::math::swizzle<type, 4, 1, 3, 0, 1> tqst;};\
struct{::legion::core::math::swizzle<type, 4, 1, 3, 0, 2> tqsp;};\
struct{::legion::core::math::swizzle<type, 4, 1, 3, 0, 3> tqsq;};\
struct{::legion::core::math::swizzle<type, 4, 1, 3, 1, 0> tqts;};\
struct{::legion::core::math::swizzle<type, 4, 1, 3, 1, 1> tqtt;};\
struct{::legion::core::math::swizzle<type, 4, 1, 3, 1, 2> tqtp;};\
struct{::legion::core::math::swizzle<type, 4, 1, 3, 1, 3> tqtq;};\
struct{::legion::core::math::swizzle<type, 4, 1, 3, 2, 0> tqps;};\
struct{::legion::core::math::swizzle<type, 4, 1, 3, 2, 1> tqpt;};\
struct{::legion::core::math::swizzle<type, 4, 1, 3, 2, 2> tqpp;};\
struct{::legion::core::math::swizzle<type, 4, 1, 3, 2, 3> tqpq;};\
struct{::legion::core::math::swizzle<type, 4, 1, 3, 3, 0> tqqs;};\
struct{::legion::core::math::swizzle<type, 4, 1, 3, 3, 1> tqqt;};\
struct{::legion::core::math::swizzle<type, 4, 1, 3, 3, 2> tqqp;};\
struct{::legion::core::math::swizzle<type, 4, 1, 3, 3, 3> tqqq;};\
struct{::legion::core::math::swizzle<type, 4, 2, 0, 0, 0> psss;};\
struct{::legion::core::math::swizzle<type, 4, 2, 0, 0, 1> psst;};\
struct{::legion::core::math::swizzle<type, 4, 2, 0, 0, 2> pssp;};\
struct{::legion::core::math::swizzle<type, 4, 2, 0, 0, 3> pssq;};\
struct{::legion::core::math::swizzle<type, 4, 2, 0, 1, 0> psts;};\
struct{::legion::core::math::swizzle<type, 4, 2, 0, 1, 1> pstt;};\
struct{::legion::core::math::swizzle<type, 4, 2, 0, 1, 2> pstp;};\
struct{::legion::core::math::swizzle<type, 4, 2, 0, 1, 3> pstq;};\
struct{::legion::core::math::swizzle<type, 4, 2, 0, 2, 0> psps;};\
struct{::legion::core::math::swizzle<type, 4, 2, 0, 2, 1> pspt;};\
struct{::legion::core::math::swizzle<type, 4, 2, 0, 2, 2> pspp;};\
struct{::legion::core::math::swizzle<type, 4, 2, 0, 2, 3> pspq;};\
struct{::legion::core::math::swizzle<type, 4, 2, 0, 3, 0> psqs;};\
struct{::legion::core::math::swizzle<type, 4, 2, 0, 3, 1> psqt;};\
struct{::legion::core::math::swizzle<type, 4, 2, 0, 3, 2> psqp;};\
struct{::legion::core::math::swizzle<type, 4, 2, 0, 3, 3> psqq;};\
struct{::legion::core::math::swizzle<type, 4, 2, 1, 0, 0> ptss;};\
struct{::legion::core::math::swizzle<type, 4, 2, 1, 0, 1> ptst;};\
struct{::legion::core::math::swizzle<type, 4, 2, 1, 0, 2> ptsp;};\
struct{::legion::core::math::swizzle<type, 4, 2, 1, 0, 3> ptsq;};\
struct{::legion::core::math::swizzle<type, 4, 2, 1, 1, 0> ptts;};\
struct{::legion::core::math::swizzle<type, 4, 2, 1, 1, 1> pttt;};\
struct{::legion::core::math::swizzle<type, 4, 2, 1, 1, 2> pttp;};\
struct{::legion::core::math::swizzle<type, 4, 2, 1, 1, 3> pttq;};\
struct{::legion::core::math::swizzle<type, 4, 2, 1, 2, 0> ptps;};\
struct{::legion::core::math::swizzle<type, 4, 2, 1, 2, 1> ptpt;};\
struct{::legion::core::math::swizzle<type, 4, 2, 1, 2, 2> ptpp;};\
struct{::legion::core::math::swizzle<type, 4, 2, 1, 2, 3> ptpq;};\
struct{::legion::core::math::swizzle<type, 4, 2, 1, 3, 0> ptqs;};\
struct{::legion::core::math::swizzle<type, 4, 2, 1, 3, 1> ptqt;};\
struct{::legion::core::math::swizzle<type, 4, 2, 1, 3, 2> ptqp;};\
struct{::legion::core::math::swizzle<type, 4, 2, 1, 3, 3> ptqq;};\
struct{::legion::core::math::swizzle<type, 4, 2, 2, 0, 0> ppss;};\
struct{::legion::core::math::swizzle<type, 4, 2, 2, 0, 1> ppst;};\
struct{::legion::core::math::swizzle<type, 4, 2, 2, 0, 2> ppsp;};\
struct{::legion::core::math::swizzle<type, 4, 2, 2, 0, 3> ppsq;};\
struct{::legion::core::math::swizzle<type, 4, 2, 2, 1, 0> ppts;};\
struct{::legion::core::math::swizzle<type, 4, 2, 2, 1, 1> pptt;};\
struct{::legion::core::math::swizzle<type, 4, 2, 2, 1, 2> pptp;};\
struct{::legion::core::math::swizzle<type, 4, 2, 2, 1, 3> pptq;};\
struct{::legion::core::math::swizzle<type, 4, 2, 2, 2, 0> ppps;};\
struct{::legion::core::math::swizzle<type, 4, 2, 2, 2, 1> pppt;};\
struct{::legion::core::math::swizzle<type, 4, 2, 2, 2, 2> pppp;};\
struct{::legion::core::math::swizzle<type, 4, 2, 2, 2, 3> pppq;};\
struct{::legion::core::math::swizzle<type, 4, 2, 2, 3, 0> ppqs;};\
struct{::legion::core::math::swizzle<type, 4, 2, 2, 3, 1> ppqt;};\
struct{::legion::core::math::swizzle<type, 4, 2, 2, 3, 2> ppqp;};\
struct{::legion::core::math::swizzle<type, 4, 2, 2, 3, 3> ppqq;};\
struct{::legion::core::math::swizzle<type, 4, 2, 3, 0, 0> pqss;};\
struct{::legion::core::math::swizzle<type, 4, 2, 3, 0, 1> pqst;};\
struct{::legion::core::math::swizzle<type, 4, 2, 3, 0, 2> pqsp;};\
struct{::legion::core::math::swizzle<type, 4, 2, 3, 0, 3> pqsq;};\
struct{::legion::core::math::swizzle<type, 4, 2, 3, 1, 0> pqts;};\
struct{::legion::core::math::swizzle<type, 4, 2, 3, 1, 1> pqtt;};\
struct{::legion::core::math::swizzle<type, 4, 2, 3, 1, 2> pqtp;};\
struct{::legion::core::math::swizzle<type, 4, 2, 3, 1, 3> pqtq;};\
struct{::legion::core::math::swizzle<type, 4, 2, 3, 2, 0> pqps;};\
struct{::legion::core::math::swizzle<type, 4, 2, 3, 2, 1> pqpt;};\
struct{::legion::core::math::swizzle<type, 4, 2, 3, 2, 2> pqpp;};\
struct{::legion::core::math::swizzle<type, 4, 2, 3, 2, 3> pqpq;};\
struct{::legion::core::math::swizzle<type, 4, 2, 3, 3, 0> pqqs;};\
struct{::legion::core::math::swizzle<type, 4, 2, 3, 3, 1> pqqt;};\
struct{::legion::core::math::swizzle<type, 4, 2, 3, 3, 2> pqqp;};\
struct{::legion::core::math::swizzle<type, 4, 2, 3, 3, 3> pqqq;};\
struct{::legion::core::math::swizzle<type, 4, 3, 0, 0, 0> qsss;};\
struct{::legion::core::math::swizzle<type, 4, 3, 0, 0, 1> qsst;};\
struct{::legion::core::math::swizzle<type, 4, 3, 0, 0, 2> qssp;};\
struct{::legion::core::math::swizzle<type, 4, 3, 0, 0, 3> qssq;};\
struct{::legion::core::math::swizzle<type, 4, 3, 0, 1, 0> qsts;};\
struct{::legion::core::math::swizzle<type, 4, 3, 0, 1, 1> qstt;};\
struct{::legion::core::math::swizzle<type, 4, 3, 0, 1, 2> qstp;};\
struct{::legion::core::math::swizzle<type, 4, 3, 0, 1, 3> qstq;};\
struct{::legion::core::math::swizzle<type, 4, 3, 0, 2, 0> qsps;};\
struct{::legion::core::math::swizzle<type, 4, 3, 0, 2, 1> qspt;};\
struct{::legion::core::math::swizzle<type, 4, 3, 0, 2, 2> qspp;};\
struct{::legion::core::math::swizzle<type, 4, 3, 0, 2, 3> qspq;};\
struct{::legion::core::math::swizzle<type, 4, 3, 0, 3, 0> qsqs;};\
struct{::legion::core::math::swizzle<type, 4, 3, 0, 3, 1> qsqt;};\
struct{::legion::core::math::swizzle<type, 4, 3, 0, 3, 2> qsqp;};\
struct{::legion::core::math::swizzle<type, 4, 3, 0, 3, 3> qsqq;};\
struct{::legion::core::math::swizzle<type, 4, 3, 1, 0, 0> qtss;};\
struct{::legion::core::math::swizzle<type, 4, 3, 1, 0, 1> qtst;};\
struct{::legion::core::math::swizzle<type, 4, 3, 1, 0, 2> qtsp;};\
struct{::legion::core::math::swizzle<type, 4, 3, 1, 0, 3> qtsq;};\
struct{::legion::core::math::swizzle<type, 4, 3, 1, 1, 0> qtts;};\
struct{::legion::core::math::swizzle<type, 4, 3, 1, 1, 1> qttt;};\
struct{::legion::core::math::swizzle<type, 4, 3, 1, 1, 2> qttp;};\
struct{::legion::core::math::swizzle<type, 4, 3, 1, 1, 3> qttq;};\
struct{::legion::core::math::swizzle<type, 4, 3, 1, 2, 0> qtps;};\
struct{::legion::core::math::swizzle<type, 4, 3, 1, 2, 1> qtpt;};\
struct{::legion::core::math::swizzle<type, 4, 3, 1, 2, 2> qtpp;};\
struct{::legion::core::math::swizzle<type, 4, 3, 1, 2, 3> qtpq;};\
struct{::legion::core::math::swizzle<type, 4, 3, 1, 3, 0> qtqs;};\
struct{::legion::core::math::swizzle<type, 4, 3, 1, 3, 1> qtqt;};\
struct{::legion::core::math::swizzle<type, 4, 3, 1, 3, 2> qtqp;};\
struct{::legion::core::math::swizzle<type, 4, 3, 1, 3, 3> qtqq;};\
struct{::legion::core::math::swizzle<type, 4, 3, 2, 0, 0> qpss;};\
struct{::legion::core::math::swizzle<type, 4, 3, 2, 0, 1> qpst;};\
struct{::legion::core::math::swizzle<type, 4, 3, 2, 0, 2> qpsp;};\
struct{::legion::core::math::swizzle<type, 4, 3, 2, 0, 3> qpsq;};\
struct{::legion::core::math::swizzle<type, 4, 3, 2, 1, 0> qpts;};\
struct{::legion::core::math::swizzle<type, 4, 3, 2, 1, 1> qptt;};\
struct{::legion::core::math::swizzle<type, 4, 3, 2, 1, 2> qptp;};\
struct{::legion::core::math::swizzle<type, 4, 3, 2, 1, 3> qptq;};\
struct{::legion::core::math::swizzle<type, 4, 3, 2, 2, 0> qpps;};\
struct{::legion::core::math::swizzle<type, 4, 3, 2, 2, 1> qppt;};\
struct{::legion::core::math::swizzle<type, 4, 3, 2, 2, 2> qppp;};\
struct{::legion::core::math::swizzle<type, 4, 3, 2, 2, 3> qppq;};\
struct{::legion::core::math::swizzle<type, 4, 3, 2, 3, 0> qpqs;};\
struct{::legion::core::math::swizzle<type, 4, 3, 2, 3, 1> qpqt;};\
struct{::legion::core::math::swizzle<type, 4, 3, 2, 3, 2> qpqp;};\
struct{::legion::core::math::swizzle<type, 4, 3, 2, 3, 3> qpqq;};\
struct{::legion::core::math::swizzle<type, 4, 3, 3, 0, 0> qqss;};\
struct{::legion::core::math::swizzle<type, 4, 3, 3, 0, 1> qqst;};\
struct{::legion::core::math::swizzle<type, 4, 3, 3, 0, 2> qqsp;};\
struct{::legion::core::math::swizzle<type, 4, 3, 3, 0, 3> qqsq;};\
struct{::legion::core::math::swizzle<type, 4, 3, 3, 1, 0> qqts;};\
struct{::legion::core::math::swizzle<type, 4, 3, 3, 1, 1> qqtt;};\
struct{::legion::core::math::swizzle<type, 4, 3, 3, 1, 2> qqtp;};\
struct{::legion::core::math::swizzle<type, 4, 3, 3, 1, 3> qqtq;};\
struct{::legion::core::math::swizzle<type, 4, 3, 3, 2, 0> qqps;};\
struct{::legion::core::math::swizzle<type, 4, 3, 3, 2, 1> qqpt;};\
struct{::legion::core::math::swizzle<type, 4, 3, 3, 2, 2> qqpp;};\
struct{::legion::core::math::swizzle<type, 4, 3, 3, 2, 3> qqpq;};\
struct{::legion::core::math::swizzle<type, 4, 3, 3, 3, 0> qqqs;};\
struct{::legion::core::math::swizzle<type, 4, 3, 3, 3, 1> qqqt;};\
struct{::legion::core::math::swizzle<type, 4, 3, 3, 3, 2> qqqp;};\
struct{::legion::core::math::swizzle<type, 4, 3, 3, 3, 3> qqqq;};\
