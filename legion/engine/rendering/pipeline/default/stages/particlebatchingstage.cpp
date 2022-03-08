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

            auto filter = emitter.get_uniform<mesh_filter>("mesh_filter");
            auto renderer = emitter.get_uniform<mesh_renderer>("renderer");

            auto& posBuffer = emitter.get_buffer<position>("posBuffer");
            auto& scaleBuffer = emitter.get_buffer<scale>("scaleBuffer");

            auto& batch = (*batches)[renderer.material][model_handle{ filter.shared_mesh.id() }];
            if (batch.second.empty())
                batchList.push_back(std::ref(batch));
            auto start = batch.second.size();
            batch.second.insert(batch.second.end(), posBuffer.size(), math::mat4());

            scale scal = math::vec3::one;
            if (emitter.localScale && ent.has_component<scale>())
                scal = ent.get_component<scale>();

            rotation rot = math::quat(0, 0, 0, 1);
            if (emitter.localRotation && ent.has_component<rotation>())
                rot = ent.get_component<rotation>().get();

            position origin = math::vec3();
            if (emitter.localPosition && ent.has_component<position>())
                origin = ent.get_component<position>().get();


            queueJobs(posBuffer.size(), [&](id_type jobId)
                {
                    auto transScale = scal * scaleBuffer[jobId];
                    auto transPos = origin + math::rotate(rot, posBuffer[jobId]) * transScale;
                    batch.second[jobId + start] = math::compose(transScale, rot, transPos);
                }).wait();
        }
    }

    priority_type ParticleBatchingStage::priority()
    {
        return setup_priority;
    }
}
