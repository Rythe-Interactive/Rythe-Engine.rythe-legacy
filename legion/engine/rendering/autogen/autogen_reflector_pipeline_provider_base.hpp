#pragma once
#include <core/types/reflector.hpp>
namespace legion::rendering
{
    struct pipeline_provider_base;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::rendering::pipeline_provider_base>(legion::rendering::pipeline_provider_base& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::rendering::pipeline_provider_base>(const legion::rendering::pipeline_provider_base& obj);
}
