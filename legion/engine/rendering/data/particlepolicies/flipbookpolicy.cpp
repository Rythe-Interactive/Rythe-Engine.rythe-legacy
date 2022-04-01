#include <rendering/rendering.hpp>
#include <rendering/data/particlepolicies/flipbookpolicy.hpp>

namespace legion::rendering
{
    void flipbook_policy::setup(particle_emitter& emitter)
    {
        if (!emitter.has_buffer<scale>("scaleBuffer"))
            emitter.create_buffer<scale>("scaleBuffer");
        if (!emitter.has_buffer<size_type>("frameID"))
            emitter.create_buffer<size_type>("frameID");

        if (!emitter.has_uniform<int>("frameCount"))
            emitter.create_uniform<int>("frameCount");
    }

    void flipbook_policy::onInit(particle_emitter& emitter, size_type start, size_type end)
    {
    }

    void flipbook_policy::onUpdate(particle_emitter& emitter, float deltaTime, size_type count)
    {
        auto& frameIDBuffer = emitter.get_buffer<size_type>("frameID");
        auto& ageBuffer = emitter.get_buffer<life_time>("lifetimeBuffer");
        auto& frameCount = emitter.get_uniform<int>("frameCount");

        for (size_type idx = 0; idx < count; idx++)
        {
            auto& lifeTime = ageBuffer[idx];
            frameIDBuffer[idx] = math::clamp(static_cast<int>(lifeTime.age / lifeTime.max) * frameCount, 0, frameCount - 1);
        }

    }

    void flipbook_policy::onDestroy(particle_emitter& emitter, size_type start, size_type end)
    {
    }

}
