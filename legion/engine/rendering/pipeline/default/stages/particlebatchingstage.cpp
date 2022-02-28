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

        for (auto& ent : emitterFilter)
        {
            auto& emitter = ent.get_component<particle_emitter>().get();

            auto filter = emitter.getUniform<mesh_filter>("mesh_filter").get();
            auto renderer = emitter.getUniform<mesh_renderer>("renderer").get();

            auto& posBuffer = emitter.getBuffer<position>();
            auto& scaleBuffer = emitter.getBuffer<scale>();

            /*OPTICK_EVENT("Clear instances");
                        for (auto [material, models] : *batches)
                            for (auto [model, batch] : models)
                            {
                                batch.first.clear();
                                batch.second.clear();
                            }
                            */

            std::vector<std::reference_wrapper<std::pair<std::vector<ecs::entity>, std::vector<math::mat4>>>> batchList;
            for (size_type idx = 0; idx < posBuffer.size(); idx++)
            {
                auto& batch = (*batches)[renderer.material][model_handle{ filter.shared_mesh.id() }];
                if (batch.first.empty())
                    batchList.push_back(std::ref(batch));
                batch.first.push_back(ent);
                batch.second.emplace_back();
            }

            std::vector<ecs::entity> entityList;
            entityList.reserve(posBuffer.size());
            std::vector<std::reference_wrapper<math::mat4>> matricies;
            matricies.reserve(posBuffer.size());


            for (size_type i = 0; i < batchList.size(); i++)
            {
                auto& [entities, matrices] = batchList[i].get();
                if (entities.size() == 0)
                    continue;

                entityList.insert(entityList.end(), entities.begin(), entities.end());
                for (auto& mat : matrices)
                    matricies.push_back(std::ref(mat));
            }

            for (size_type i = 0; i < posBuffer.size(); i++)
            {
                matricies[i].get() = math::compose(scaleBuffer[i], rotation(), posBuffer[i]);
            }
            //if (posBuffer.size())
            //{
            //    auto poolSize = (schd::Scheduler::jobPoolSize() + 1) * 2;
            //    size_type jobSize = math::iround(math::ceil(posBuffer.size() / static_cast<float>(poolSize)));

            //    queueJobs(poolSize, [&](id_type jobId)
            //        {
            //            auto start = jobId * jobSize;
            //            auto end = start + jobSize;
            //            if (end > posBuffer.size())
            //                end = posBuffer.size();
            //            for (size_type i = start; i < end; i++)
            //                matricies[i].get() = transform(posBuffer[i], rotation(), scale(1, 1, 1)).to_world_matrix();
            //        }
            //    ).wait();
            //}
        }
    }

    priority_type ParticleBatchingStage::priority()
    {
        return setup_priority;
    }
}
