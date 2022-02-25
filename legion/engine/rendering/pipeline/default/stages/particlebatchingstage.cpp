#include <rendering/pipeline/default/stages/particlebatchingstage.hpp>

namespace legion::rendering
{
    void ParticleBatchingStage::setup(app::window& context)
    {

    }

    void ParticleBatchingStage::render(app::window& context, camera& cam, const camera::camera_input& camInput, time::span deltaTime)
    {

    }

    priority_type ParticleBatchingStage::priority()
    {
        return setup_priority;
    }
}
