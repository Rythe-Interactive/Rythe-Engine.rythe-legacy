#include <rendering/rendering.hpp>
#include <rendering/data/particlepolicies/renderpolicy.hpp>

namespace legion::rendering
{
    void rendering_policy::setup(particle_emitter& emitter)
    {
        emitter.create_uniform<mesh_filter>("mesh_filter", mesh_filter(model.get_mesh()));
        emitter.create_uniform<gfx::mesh_renderer>("renderer", gfx::mesh_renderer(material, model));
    }
}
