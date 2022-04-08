#pragma once
#include <core/types/reflector.hpp>
struct SpawnHullActive;
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<::SpawnHullActive>(::SpawnHullActive& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const ::SpawnHullActive>(const ::SpawnHullActive& obj);
}
