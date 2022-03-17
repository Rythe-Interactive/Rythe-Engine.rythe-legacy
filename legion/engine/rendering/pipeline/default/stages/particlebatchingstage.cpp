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
        auto* batches = get_meta<sparse_map<material_handle, sparse_map<model_handle, std::pair<std::vector<ecs::entity>,std::vector<math::mat4>>>>>(batchesId);
        static ecs::filter<particle_emitter> emitterFilter;

        std::vector<std::reference_wrapper<std::pair<std::vector<ecs::entity>, std::vector<math::mat4>>>> batchList;

        for (auto& ent : emitterFilter)
        {
            OPTICK_EVENT("[ParticleBatchingStage] Emitter");
            auto& emitter = ent.get_component<particle_emitter>().get();

            static id_type posBufferId = nameHash("posBuffer");
            static id_type scaleBufferId = nameHash("scaleBuffer");
            static id_type rotBufferId = nameHash("rotBuffer");
            static id_type meshFilterId = nameHash("mesh_filter");
            static id_type rendererId = nameHash("renderer");

            bool hasPosBuffer = emitter.has_buffer<position>(posBufferId);
            bool hasScaleBuffer = emitter.has_buffer<scale>(scaleBufferId);
            bool hasRotBuffer = emitter.has_buffer<rotation>(rotBufferId);

            bool hasMeshFilter = emitter.has_uniform<mesh_filter>(meshFilterId);
            bool hasRenderer = emitter.has_uniform<mesh_renderer>(rendererId);

            if ((!hasPosBuffer && !hasScaleBuffer && !hasRotBuffer) || (!hasMeshFilter || !hasRenderer))
                continue;

            particle_buffer<position> posBuffer;
            if (hasPosBuffer)
                posBuffer = emitter.get_buffer<position>(posBufferId);

            particle_buffer<scale> scaleBuffer;
            if (hasScaleBuffer)
                scaleBuffer = emitter.get_buffer<scale>(scaleBufferId);

            particle_buffer<rotation> rotBuffer;
            if (hasRotBuffer)
                rotBuffer = emitter.get_buffer<rotation>(rotBufferId);


            auto compare = [&](position& a, position& b)
            {
                OPTICK_EVENT("[ParticleBatchingStage] Compare Lamda");
                return math::distance2(a, camInput.pos) < math::distance2(b, camInput.pos);
            };

            //{
            //    OPTICK_EVENT("[ParticleBatchingStage] Z Sorting");
            //    for (size_type i = 1; i < emitter.size(); i++)
            //    {
            //        auto j = i;
            //        while (j > 0 && compare(posBuffer[j - 1], posBuffer[j]))
            //        {
            //            emitter.swap(j, j - 1);
            //            j--;
            //        }
            //    }
            //}

            auto filter = emitter.get_uniform<mesh_filter>(meshFilterId);
            auto renderer = emitter.get_uniform<mesh_renderer>(rendererId);

            auto& batch = (*batches)[renderer.material][model_handle{ filter.shared_mesh.id() }];
            auto start = batch.second.size();
            batch.second.insert(batch.second.end(), emitter.size(), math::mat4());

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


            queueJobs(emitter.size(), [&](id_type jobId)
                {
                    if (emitter.is_alive(jobId))
                    {
                        math::mat4 localMat = math::compose(hasScaleBuffer ? scaleBuffer[jobId] : scal, hasRotBuffer ? rotBuffer[jobId] : rot, hasPosBuffer ? posBuffer[jobId] : origin);
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
