#include <rendering/rendering.hpp>
#include <rendering/data/particlepolicies/renderpolicy.hpp>

namespace legion::rendering
{
    void rendering_policy::setup(particle_emitter& emitter)
    {
        if (!emitter.has_uniform<mesh_filter>("mesh_filter"))
            emitter.create_uniform<mesh_filter>("mesh_filter", mesh_filter(model.get_mesh()));
        if (!emitter.has_uniform<gfx::mesh_renderer>("renderer"))
            emitter.create_uniform<gfx::mesh_renderer>("renderer", gfx::mesh_renderer(material, model));
    }
}
