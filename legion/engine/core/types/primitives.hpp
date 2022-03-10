#pragma once
#include <cstddef> 
#include <cstdint> 
#include <vector>  
#include <string>
#include <string_view>
#include <float.h>

/**
 * @file primitives.hpp
 */

namespace legion::core
{
    using uint8 = ::std::uint8_t;
    using uint16 = ::std::uint16_t;
    using uint32 = ::std::uint32_t;
    using uint64 = ::std::uint64_t;
    using uint_max = ::std::uintmax_t;

    using uint_least8 = ::std::uint_least8_t;
    using uint_least16 = ::std::uint_least16_t;
    using uint_least32 = ::std::uint_least32_t;
    using uint_least64 = ::std::uint_least64_t;

    using int8 = ::std::int8_t;
    using int16 = ::std::int16_t;
    using int32 = ::std::int32_t;
    using int64 = ::std::int64_t;
    using int_max = ::std::intmax_t;

    using int_least8 = ::std::int_least8_t;
    using int_least16 = ::std::int_least16_t;
    using int_least32 = ::std::int_least32_t;
    using int_least64 = ::std::int_least64_t;

    using size_type = ::std::size_t;
    using index_type = ::std::size_t;
    using diff_type = ::std::ptrdiff_t;
    using ptr_type = std::uintptr_t;

    using float32 = float;
    using float64 = double;
    using float_max = long double;

    using time32 = float32;
    using time64 = float64;
    using time_max = float_max;

    using cstring = const char*;

    using uint = uint32;

    using byte = uint8;
    using word = uint16;
    using dword = uint32;
    using qword = uint64;

    using byte_vec = ::std::vector<byte>;
    using word_vec = ::std::vector<word>;
    using dword_vec = ::std::vector<dword>;
    using qword_vec = ::std::vector<qword>;

    using bitfield8 = byte;
    using bitfield16 = uint16;
    using bitfield32 = uint32;
    using bitfield64 = uint64;

    using priority_type = int8;
#define default_priority 0
#define PRIORITY_MAX CHAR_MAX
#define PRIORITY_MIN CHAR_MIN

    using id_type = size_type;

#define invalid_id 0

    using hours = time32;
    using minutes = time32;
    using seconds = time32;
    using milli_seconds = time32;
    using micro_seconds = time32;

    using fast_time = time32;
    using fast_hours = fast_time;
    using fast_minutes = fast_time;
    using fast_seconds = fast_time;
    using fast_milli_seconds = fast_time;
    using fast_micro_seconds = fast_time;
    using fast_nano_seconds = fast_time;

    using precise_time = time64;
    using precise_hours = precise_time;
    using precise_minutes = precise_time;
    using precise_seconds = precise_time;
    using precise_milli_seconds = precise_time;
    using precise_micro_seconds = precise_time;
    using precise_nano_seconds = precise_time;
}
