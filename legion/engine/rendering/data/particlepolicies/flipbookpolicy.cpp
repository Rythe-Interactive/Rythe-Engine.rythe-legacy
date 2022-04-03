#include <rendering/rendering.hpp>
#include <rendering/data/particlepolicies/flipbookpolicy.hpp>

namespace legion::rendering
{
    void flipbook_policy::setup(particle_emitter& emitter)
    {
        if (!emitter.has_buffer<scale>("scaleBuffer"))
            emitter.create_buffer<scale>("scaleBuffer");
        if (!emitter.has_buffer<uint>("frameID"))
            emitter.create_buffer<uint>("frameID", 1);

        if (!emitter.has_uniform<uint>("frameCount"))
            emitter.create_uniform<uint>("frameCount");
    }

    void flipbook_policy::onUpdate(particle_emitter& emitter, float deltaTime, size_type count)
    {
        auto& frameIDBuffer = emitter.get_buffer<uint>("frameID");
        auto& ageBuffer = emitter.get_buffer<life_time>("lifetimeBuffer");
        auto& frameCount = emitter.get_uniform<uint>("frameCount");

        for (size_type idx = 0; idx < count; idx++)
        {
            auto& lifeTime = ageBuffer[idx];
            frameIDBuffer[idx] = math::clamp(static_cast<uint>(((lifeTime.age / lifeTime.max) * frameCount) + 0.5f), 0u, frameCount - 1);
        }

    }
}
