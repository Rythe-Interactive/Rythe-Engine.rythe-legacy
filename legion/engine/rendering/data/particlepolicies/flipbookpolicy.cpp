#include <rendering/rendering.hpp>
#include <rendering/data/particlepolicies/flipbookpolicy.hpp>

namespace legion::rendering
{
    void flipbook_policy::setup(particle_emitter& emitter)
    {
        if (!emitter.has_buffer<uint>("frameID"))
            emitter.create_buffer<uint>("frameID");
    }

    void flipbook_policy::onInit(particle_emitter& emitter, size_type start, size_type end)
    {
    }

    void flipbook_policy::onUpdate(particle_emitter& emitter, float deltaTime, size_type count)
    {
        //auto& ageBuffer = emitter.get_buffer<life_time>("lifetimeBuffer");
        auto& frameIDBuffer = emitter.get_buffer<uint>("frameID");

        for (size_type idx = 0; idx < count; idx++)
        {
            frameIDBuffer[idx] = 1;
        }
    }
}
