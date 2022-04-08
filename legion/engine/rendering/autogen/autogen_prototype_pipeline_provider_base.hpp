#pragma once
#include <core/types/prototype.hpp>
namespace legion::rendering
{
    struct pipeline_provider_base;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::rendering::pipeline_provider_base>(const legion::rendering::pipeline_provider_base& obj);
}
