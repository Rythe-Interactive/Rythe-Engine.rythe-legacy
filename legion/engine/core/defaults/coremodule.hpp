#pragma once
#include <core/engine/module.hpp>

/**
 * @file coremodule.hpp
 */
namespace legion::core
{
    /**@class CoreModule
     * @brief Custom module.
     */
    class CoreModule final : public Module
    {
    public:
        virtual void setup() override;

        virtual priority_type priority() override;
    };
}
