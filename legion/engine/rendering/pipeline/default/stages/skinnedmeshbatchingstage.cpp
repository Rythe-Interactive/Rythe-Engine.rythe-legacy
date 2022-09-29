#include <rendering/pipeline/default/stages/skinnedmeshbatchingstage.hpp>

namespace  legion::rendering
{
    void SkinnedMeshBatchingStage::setup(app::window& context)
    {
        create_meta<sparse_map<material_handle, sparse_map<model_handle, std::pair<std::vector<id_type>, std::vector<skinned_mesh_data>>>>>("skinned mesh batches");
    }

    void SkinnedMeshBatchingStage::render(app::window& context, camera& cam, const camera::camera_input& camInput, time::span deltaTime)
    {
        (void)deltaTime;
        (void)camInput;
        (void)cam;
        (void)context;

        static id_type batchesId = nameHash("skinned mesh batches");
        auto* batches = get_meta<sparse_map<material_handle, sparse_map<model_handle, std::pair<std::vector<id_type>, std::vector<skinned_mesh_data>>>>>(batchesId);

        static ecs::filter<position, rotation, scale, mesh_filter, skinned_mesh_renderer> renderablesQuery{};

        auto& filters = renderablesQuery.get<mesh_filter>();
        auto& renderers = renderablesQuery.get<skinned_mesh_renderer>();

        {
            for (auto [material, models] : *batches)
                for (auto [model, batch] : models)
                {
                    batch.first.clear();
                    batch.second.clear();
                }
        }

        {
            std::vector<std::reference_wrapper<std::pair<std::vector<id_type>, std::vector<skinned_mesh_data>>>> batchList;
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
            std::vector<std::reference_wrapper<skinned_mesh_data>> skinnedMeshData;
            skinnedMeshData.reserve(renderablesQuery.size());


            for (size_type i = 0; i < batchList.size(); i++)
            {
                auto& [entities, meshDatas] = batchList[i].get();
                if (entities.size() == 0)
                    continue;

                entityList.insert(entityList.end(), entities.begin(), entities.end());
                for (auto& sknmData : meshDatas)
                    skinnedMeshData.push_back(std::ref(sknmData));
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
                        {
                            auto ent = ecs::Registry::getEntity(entityList[i]);
                            skinnedMeshData[i].get().rootMat = transform(ent.get_component<transform>()).to_world_matrix();
                            auto mesh = ent.get_component<mesh_filter>()->shared_mesh;
                            skinnedMeshData[i].get().jointIndeces = std::vector<math::vec3>(mesh->jointIDs);
                            skinnedMeshData[i].get().weights = std::vector<math::vec3>(mesh->weights);
                            auto skinnedRenderer = ent.get_component<skinned_mesh_renderer>();
                            assets::asset<skeleton> skeleton = skinnedRenderer->m_skeleton;
                            skinnedMeshData[i].get().jointTransforms = std::vector<math::mat4>(skeleton->rootJoint.get_joint_transforms());
                        }
                    }
                ).wait();
            }
        }
    }

    priority_type SkinnedMeshBatchingStage::priority()
    {
        return setup_priority;
    }

}
