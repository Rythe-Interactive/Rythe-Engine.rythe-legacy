#include <rendering/pipeline/default/stages/particlebatchingstage.hpp>

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

            auto filter = emitter.getUniform<mesh_filter>("mesh_filter").get();
            auto renderer = emitter.getUniform<mesh_renderer>("renderer").get();

            auto& posBuffer = emitter.getBuffer<position>();
            auto& scaleBuffer = emitter.getBuffer<scale>();

            auto& batch = (*batches)[renderer.material][model_handle{ filter.shared_mesh.id() }];
            if (batch.second.empty())
                batchList.push_back(std::ref(batch));
            auto start = batch.second.size();
            batch.second.insert(batch.second.end(), posBuffer.size(), math::mat4());

            queueJobs(posBuffer.size(), [&](id_type jobId)
                {
                    batch.second[jobId + start] = math::compose(scaleBuffer[jobId], rotation(), posBuffer[jobId]);
                }).wait();
        }
    }

    priority_type ParticleBatchingStage::priority()
    {
        return setup_priority;
    }
}
