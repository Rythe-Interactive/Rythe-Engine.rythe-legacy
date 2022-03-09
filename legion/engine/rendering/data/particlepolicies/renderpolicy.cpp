#include <rendering/rendering.hpp>
#include <rendering/data/particlepolicies/renderpolicy.hpp>

namespace legion::rendering
{
    void rendering_policy::OnSetup(particle_emitter& emitter)
    {
        if (!emitter.has_uniform<mesh_filter>("mesh_filter"))
            emitter.create_uniform<mesh_filter>("mesh_filter", mesh_filter(model.get_mesh()));
        if (!emitter.has_uniform<gfx::mesh_renderer>("renderer"))
            emitter.create_uniform<gfx::mesh_renderer>("renderer", gfx::mesh_renderer(material, model));
    }

    void rendering_policy::OnInit(particle_emitter& emitter, size_type start, size_type end)
    {

    }

    void rendering_policy::OnUpdate(particle_emitter& emitter, float deltaTime, size_type count)
    {

    }

    void rendering_policy::OnDestroy(particle_emitter& emitter, size_type start, size_type end)
    {

    }
}
