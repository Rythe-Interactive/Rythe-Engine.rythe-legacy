#pragma once
#include <core/types/reflector.hpp>
struct SpawnRandomHullOnCameraLoc;
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<::SpawnRandomHullOnCameraLoc>(::SpawnRandomHullOnCameraLoc& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const ::SpawnRandomHullOnCameraLoc>(const ::SpawnRandomHullOnCameraLoc& obj);
}
