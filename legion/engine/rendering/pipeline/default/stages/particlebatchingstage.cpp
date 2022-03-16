#include <rendering/pipeline/default/stages/particlebatchingstage.hpp>
#include <core/particles/particleemitter.hpp>

namespace legion::rendering
{
    void ParticleBatchingStage::setup(app::window& context)
    {

    }

    void ParticleBatchingStage::render(app::window& context, camera& cam, const camera::camera_input& camInput, time::span deltaTime)
    {
        static id_type batchesId = nameHash("mesh batches");
        auto* batches = get_meta<sparse_map<material_handle, sparse_map<model_handle, std::pair<std::vector<ecs::entity>, std::vector<math::mat4>>>>>(batchesId);
        static ecs::filter<particle_emitter> emitterFilter;

        std::vector<std::reference_wrapper<std::pair<std::vector<ecs::entity>, std::vector<math::mat4>>>> batchList;

        for (auto& ent : emitterFilter)
        {
            auto& emitter = ent.get_component<particle_emitter>().get();

            auto& posBuffer = emitter.get_buffer<position>("posBuffer");
            auto& scaleBuffer = emitter.get_buffer<scale>("scaleBuffer");

            auto compare = [&](position& a, position& b)
            {
                return math::distance2(a, camInput.pos) < math::distance2(b, camInput.pos);
            };

            for (size_type i = 1; i < emitter.currentParticleCount; i++)
            {
                auto j = i;
                while (j > 0 && compare(posBuffer[j - 1], posBuffer[j]))
                {
                    emitter.swap(j, j - 1);
                    j--;
                }
            }

            auto filter = emitter.get_uniform<mesh_filter>("mesh_filter");
            auto renderer = emitter.get_uniform<mesh_renderer>("renderer");

            auto& batch = (*batches)[renderer.material][model_handle{ filter.shared_mesh.id() }];
            if (batch.second.empty())
                batchList.push_back(std::ref(batch));
            auto start = batch.second.size();
            batch.second.insert(batch.second.end(), posBuffer.size(), math::mat4());


            scale scal;
            rotation rot;
            position origin;
            if (emitter.localSpace)
            {
                if (ent.has_component<scale>())
                    scal = ent.get_component<scale>();
                if (ent.has_component<rotation>())
                    rot = ent.get_component<rotation>();
                if (ent.has_component<position>())
                    origin = ent.get_component<position>();
            }

            math::mat4 parentMat = math::compose(scal, rot, origin);

            queueJobs(posBuffer.size(), [&](id_type jobId)
                {
                    if (emitter.is_alive(jobId))
                    {
                        math::mat4 localMat = math::compose(scaleBuffer[jobId], rotation(), posBuffer[jobId]);
                        batch.second[jobId + start] = parentMat * localMat;
                    }
                }).wait();
        }
    }

    priority_type ParticleBatchingStage::priority()
    {
        return setup_priority;
    }
}
