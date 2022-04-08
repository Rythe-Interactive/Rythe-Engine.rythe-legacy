#pragma once
#include <core/types/reflector.hpp>
struct nextPhysicsTimeStepContinue;
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<::nextPhysicsTimeStepContinue>(::nextPhysicsTimeStepContinue& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const ::nextPhysicsTimeStepContinue>(const ::nextPhysicsTimeStepContinue& obj);
}
