#include <rendering/pipeline/default/stages/meshbatchingstage.hpp>

namespace  legion::rendering
{
    void MeshBatchingStage::setup(app::window& context)
    {
        create_meta<sparse_map<material_handle, sparse_map<model_handle, std::pair<std::vector<id_type>, std::vector<math::mat4>>>>>("mesh batches");
    }

    void MeshBatchingStage::render(app::window& context, camera& cam, const camera::camera_input& camInput, time::span deltaTime)
    {
        (void)deltaTime;
        (void)camInput;
        (void)cam;
        (void)context;

        static id_type batchesId = nameHash("mesh batches");
        auto* batches = get_meta<sparse_map<material_handle, sparse_map<model_handle, std::pair<std::vector<id_type>, std::vector<math::mat4>>>>>(batchesId);

        static ecs::filter<position, rotation, scale, mesh_filter, mesh_renderer> renderablesQuery{};

        auto& filters = renderablesQuery.get<mesh_filter>();
        auto& renderers = renderablesQuery.get<mesh_renderer>();

        {
            for (auto [material, models] : *batches)
                for (auto [model, batch] : models)
                {
                    batch.first.clear();
                    batch.second.clear();
                }
        }

        {
            std::vector<std::reference_wrapper<std::pair<std::vector<id_type>, std::vector<math::mat4>>>> batchList;
            for (size_type i = 0; i < renderablesQuery.size(); i++)
            {
                if (!renderablesQuery[i]->active)
                    continue;

                auto& batch = (*batches)[renderers[i].get().material][model_handle{ filters[i].get().shared_mesh.id() }];
                if (batch.first.empty())
                    batchList.push_back(std::ref(batch));
                batch.first.push_back(renderablesQuery[i]->id);
                batch.second.emplace_back();
            }

            std::vector<id_type> entityList;
            entityList.reserve(renderablesQuery.size());
            std::vector<std::reference_wrapper<math::mat4>> matrixList;
            matrixList.reserve(renderablesQuery.size());


            for (size_type i = 0; i < batchList.size(); i++)
            {
                auto& [entities, matrices] = batchList[i].get();
                if (entities.size() == 0)
                    continue;

                entityList.insert(entityList.end(), entities.begin(), entities.end());
                for (auto& mat : matrices)
                    matrixList.push_back(std::ref(mat));
            }

            if (entityList.size())
            {
                auto poolSize = (schd::Scheduler::jobPoolSize() + 1) * 2;
                size_type jobSize = math::iround(math::ceil(entityList.size() / static_cast<float>(poolSize)));

                queueJobs(poolSize, [&](id_type jobId)
                    {
                        auto start = jobId * jobSize;
                        auto end = start + jobSize;
                        if (end > entityList.size())
                            end = entityList.size();
                        for (size_type i = start; i < end; i++)
                            matrixList[i].get() = transform(ecs::Registry::getEntity(entityList[i]).get_component<transform>()).to_world_matrix();
                    }
                ).wait();
            }
        }
    }

    priority_type MeshBatchingStage::priority()
    {
        return setup_priority;
    }

}
