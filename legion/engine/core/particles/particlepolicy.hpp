#pragma once
#include <core/math/math.hpp>
#include <core/defaults/defaultcomponents.hpp>
#include <core/particles/particlebuffer.hpp>
/**
 * @file particlepolicy.hpp
 * @brief
 */

namespace legion::core
{
    struct particle_policy_base
    {
        std::unordered_map<id_type, std::unique_ptr<particle_buffer_base>> buffers;

        virtual void OnInit(size_type begin, size_type end) = 0;
        virtual void OnUpdate(float deltaTime, size_type count) = 0;
        virtual void OnDestroy() = 0;
    };

    template<typename policy, typename ...buffers>
    struct particle_policy : public particle_policy_base
    {
        particle_policy()
        {
            auto& _buffers = { ...buffers };
            for (auto& buffer : _buffers)
            {
                buffers.emplace(typeHash<_buffers::value_type>(),make_unique(buffer));
            }
        }
        virtual void OnInit(size_type begin, size_type end) = 0;
        virtual void OnUpdate(float deltaTime, size_type count) = 0;
        virtual void OnDestroy() = 0;

        template<typename bufferType>
        particle_buffer<bufferType>& getBuffer()
        {
            return buffers[typeHash<bufferType>()].get();
        }
    };

    struct example_policy : public particle_policy<example_policy, float>
    {
        void OnInit(size_type begin, size_type end) override
        {
            for (size_type particleId = begin; particleId != end; particleId++)
            {
                auto& floatBuffer = getBuffer<float>();
                floatBuffer[particleId] = 10;
            }
        }

        void OnUpdate(float deltaTime, size_type count) override
        {
            auto& floatBuffer = getBuffer<float>();
            for (size_type particleId =0; particleId < count; particleId++)
            {
                floatBuffer[particleId] -= deltaTime;
                log::debug("ID: %u =%f",particleId,floatBuffer[particleId]);
            }
        }

        void OnDestroy() override
        {
            log::debug("Destroyed");
        }
    };
}
