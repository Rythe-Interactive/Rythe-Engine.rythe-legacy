#include <rendering/pipeline/default/stages/particlebatchingstage.hpp>
#include <core/particles/particleemitter.hpp>

namespace legion::rendering
{
    void ParticleBatchingStage::setup(app::window& context)
    {
        create_meta<sparse_map<material_handle, sparse_map<model_handle, std::pair<std::vector<math::mat4>, std::vector<uint>>>>>("particle batches");
    }

    void ParticleBatchingStage::render(app::window& context, camera& cam, const camera::camera_input& camInput, time::span deltaTime)
    {
        static id_type batchesId = nameHash("particle batches");
        auto* batches = get_meta<sparse_map<material_handle, sparse_map<model_handle, std::pair<std::vector<math::mat4>, std::vector<uint>>>>>(batchesId);
        static ecs::filter<particle_emitter> emitterFilter;

        {
            for (auto [material, models] : *batches)
                for (auto [model, batch] : models)
                {
                    batch.first.clear();
                    batch.second.clear();
                }
        }

        for (auto& ent : emitterFilter)
        {
            auto& emitter = ent.get_component<particle_emitter>().get();

            static id_type posBufferId = nameHash("posBuffer");
            static id_type scaleBufferId = nameHash("scaleBuffer");
            static id_type rotBufferId = nameHash("rotBuffer");
            static id_type meshFilterId = nameHash("mesh_filter");
            static id_type rendererId = nameHash("renderer");
            static id_type frameID = nameHash("frameID");

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
                return math::distance2(a, camInput.pos) < math::distance2(b, camInput.pos);
            };

            std::vector<id_type> particleIds(emitter.size());
            for (size_type i = 0; i < emitter.size(); i++)
                particleIds[i] = i;

            for (size_type i = 1; i < emitter.size(); i++)
            {
                auto j = i;
                while (j > 0 && compare(posBuffer[particleIds[j - 1]], posBuffer[particleIds[j]]))
                {
                    std::swap(particleIds[j], particleIds[j - 1]);
                    j--;
                }
            }

            auto filter = emitter.get_uniform<mesh_filter>(meshFilterId);
            auto renderer = emitter.get_uniform<mesh_renderer>(rendererId);

            auto& batch = (*batches)[renderer.material][model_handle{ filter.shared_mesh.id() }];
            auto start = batch.first.size();
            batch.first.insert(batch.first.end(), emitter.size(), math::mat4());
            if (emitter.has_buffer<uint>(frameID))
            {
                auto& frameIDBuffer = emitter.get_buffer<uint>(frameID);
                for (size_type i = 0; i < emitter.size(); i++)
                {
                    id_type particleId = particleIds[i];
                    batch.second.push_back(frameIDBuffer[particleId]);
                }
            }
            else
            {
                batch.second.insert(batch.second.end(), emitter.size(), 0);
            }

            scale scal{ 1.0f };
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
                    id_type particleId = particleIds[jobId];
                    if (emitter.is_alive(particleId))
                    {
                        math::mat4 localMat = math::compose(hasScaleBuffer ? scaleBuffer[particleId] : scal, hasRotBuffer ? rotBuffer[particleId] : rot, hasPosBuffer ? posBuffer[particleId] : origin);
                        batch.first[jobId + start] = parentMat * localMat;
                    }
                }).wait();
        }
    }

    priority_type ParticleBatchingStage::priority()
    {
        return setup_priority;
    }
}
