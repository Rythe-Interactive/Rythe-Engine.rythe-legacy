#include <rendering/pipeline/default/stages/particlebatchingstage.hpp>
#include <core/particles/particleemitter.hpp>

namespace legion::rendering
{
    void ParticleBatchingStage::setup(app::window& context)
    {
        create_meta< sparse_map < material_handle, sparse_map < model_handle, std::vector<float>>>>("particle batches");
        //particle_matrix_composer = fs::view("assets://kernels/particles.cl").load_as<compute::function>("particle_matrix_composer");
        //particle_matrix_composer.setLocalSize(16);
    }

    void ParticleBatchingStage::render(app::window& context, camera& cam, const camera::camera_input& camInput, time::span deltaTime)
    {
        static id_type batchesId = nameHash("particle batches");
        auto* batches = get_meta< sparse_map < material_handle, sparse_map < model_handle, std::vector<float>>>>(batchesId);
        static ecs::filter<particle_emitter> emitterFilter;

        //core::time::stopwatch watch;
        //watch.start();
        {
            for (auto [material, models] : *batches)
                for (auto [model, batch] : models)
                {
                    batch.clear();
                }
        }

        app::context_guard guard(context);
        if (!guard.contextIsValid())
        {
            abort();
            return;
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

            bool hasPosBuffer = emitter.has_buffer<math::vec4>(posBufferId);
            bool hasScaleBuffer = emitter.has_buffer<math::vec4>(scaleBufferId);
            bool hasRotBuffer = emitter.has_buffer<rotation>(rotBufferId);

            bool hasMeshFilter = emitter.has_uniform<mesh_filter>(meshFilterId);
            bool hasRenderer = emitter.has_uniform<mesh_renderer>(rendererId);

            if ((!hasPosBuffer && !hasScaleBuffer && !hasRotBuffer) || (!hasMeshFilter || !hasRenderer))
                continue;

            buffer* posBuffer = get_meta<buffer>("position buffer");
            if (hasPosBuffer && posBuffer)
                posBuffer->bufferData(emitter.get_buffer<math::vec4>(posBufferId));

            buffer* scaleBuffer = get_meta<buffer>("scale buffer");
            if (hasScaleBuffer && scaleBuffer)
                scaleBuffer->bufferData(emitter.get_buffer<math::vec4>(scaleBufferId));

            buffer* rotBuffer = get_meta<buffer>("orientation buffer");
            if (hasRotBuffer && rotBuffer)
                rotBuffer->bufferData(emitter.get_buffer<rotation>(rotBufferId));


            //auto compare = [&](position& a, position& b)
            //{
            //    return math::distance2(a, camInput.pos) < math::distance2(b, camInput.pos);
            //};

            std::vector<id_type> particleIds(emitter.size());
            for (size_type i = 0; i < emitter.size(); i++)
                particleIds[i] = i;

            //for (size_type i = 1; i < emitter.size(); i++)
            //{
            //    auto j = i;
            //    while (j > 0 && compare(posBuffer[particleIds[j - 1]], posBuffer[particleIds[j]]))
            //    {
            //        std::swap(particleIds[j], particleIds[j - 1]);
            //        j--;
            //    }
            //}

            auto filter = emitter.get_uniform<mesh_filter>(meshFilterId);
            auto renderer = emitter.get_uniform<mesh_renderer>(rendererId);

            auto& batch = (*batches)[renderer.material][model_handle{ filter.shared_mesh.id() }];
            //batch.first.insert(batch.first.end(), emitter.size(), math::mat4());
            if (emitter.has_buffer<float>(frameID))
            {
                auto& frameIDBuffer = emitter.get_buffer<float>(frameID);
                for (size_type i = 0; i < emitter.size(); i++)
                {
                    id_type particleId = particleIds[i];
                    batch.push_back(frameIDBuffer[particleId]);
                }
            }
            else
            {
                batch.insert(batch.end(), emitter.size(), 0);
            }

            //scale scal{ 1.f };
            //rotation rot;
            //position origin;
            //if (emitter.in_local_space())
            //{
            //    if (ent.has_component<scale>())
            //        scal = ent.get_component<scale>();
            //    if (ent.has_component<rotation>())
            //        rot = ent.get_component<rotation>();
            //    if (ent.has_component<position>())
            //        origin = ent.get_component<position>();
            //}

            //math::mat4 parentMat = math::compose(scal, rot, origin);

            //core::time::stopwatch jobsystemWatch;
            //jobsystemWatch.start();
            //queueJobs(emitter.size(), [&](id_type jobId)
            //    {
            //        id_type particleId = particleIds[jobId];
            //        core::time::stopwatch watch2;
            //        watch2.start();
            //        if (emitter.is_alive(particleId))
            //        {
            //            watch2.end();
            //            log::debug("Check if particle is alive time:  {}ms", watch2.elapsed_time().milliseconds());
            //            math::mat4 localMat = math::compose(hasScaleBuffer ? scaleBuffer[particleId] : scal, hasRotBuffer ? rotBuffer[particleId] : rot, hasPosBuffer ? math::vec3(posBuffer[particleId]) : origin);
            //            batch.first[jobId + start] = parentMat * localMat;
            //        }
            //    }).wait();
            //    jobsystemWatch.end();
            //log::debug("Job system elapsed time:  {}ms", jobsystemWatch.elapsed_time().milliseconds());

            //core::time::stopwatch computeWatch;
            //computeWatch.start();
            //auto paddedSize = math::max(((emitter.size() - 1) | 15) + 1, 16ull);
            //auto& livingBuffer = emitter.get_living_buffer();
            //std::vector<math::vec4> xAxis(emitter.capacity());
            //std::vector<math::vec4> yAxis(emitter.capacity());
            //std::vector<math::vec4> zAxis(emitter.capacity());
            //std::vector<math::vec4> translation(emitter.capacity());

            //particle_matrix_composer(paddedSize, compute::in(livingBuffer),
            //    compute::in(posBuffer), compute::in(rotBuffer), compute::in(scaleBuffer),
            //    compute::inout(xAxis), compute::inout(yAxis), compute::inout(zAxis), compute::inout(translation),
            //    compute::karg(parentMat[0], "parentX"), compute::karg(parentMat[1], "parentY"), compute::karg(parentMat[2], "parentZ"), compute::karg(parentMat[3], "parentW"));

            //for (size_type i = 0; i < emitter.size(); i++)
            //{
            //    batch.first[i][0] = xAxis[i];
            //    batch.first[i][1] = yAxis[i];
            //    batch.first[i][2] = zAxis[i];
            //    batch.first[i][3] = translation[i];
            //    //xAxis.push_back(std::ref(batch.first[i][0]));
            //    //yAxis.push_back(std::ref(batch.first[i][1]));
            //    //zAxis.push_back(std::ref(batch.first[i][2]));
            //    //translation.push_back(std::ref(batch.first[i][3]));
            //}
            //computeWatch.end();
            //log::debug("Compute elapsed time:  {}ms", computeWatch.elapsed_time().milliseconds());
        }
        //watch.end();
        //log::debug("Particle Batching Stage elapsed time:  {}ms", watch.elapsed_time().milliseconds());
    }

    priority_type ParticleBatchingStage::priority()
    {
        return setup_priority;
    }
}
