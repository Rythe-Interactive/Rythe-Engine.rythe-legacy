#include <rendering/pipeline/default/defaultpipeline.hpp>
#include <rendering/pipeline/default/stages/clearstage.hpp>
#include <rendering/pipeline/default/stages/framebufferresizestage.hpp>
#include <rendering/pipeline/default/stages/lightbufferstage.hpp>
#include <rendering/pipeline/default/stages/particlebatchingstage.hpp>
#include <rendering/pipeline/default/stages/particlerenderstage.hpp>
#include <rendering/pipeline/default/stages/meshbatchingstage.hpp>
#include <rendering/pipeline/default/stages/meshrenderstage.hpp>
#include <rendering/pipeline/default/stages/transparencyrenderstage.hpp>
#include <rendering/pipeline/default/stages/debugrenderstage.hpp>
#include <rendering/pipeline/default/stages/postprocessingstage.hpp>
#include <rendering/pipeline/default/stages/submitstage.hpp>
#include <rendering/pipeline/default/stages/skybox.hpp>
#include <rendering/pipeline/default/postfx/tonemapping.hpp>
#include <rendering/pipeline/default/postfx/fxaa.hpp>
#include <rendering/pipeline/default/postfx/bloom.hpp>
#include <rendering/pipeline/default/postfx/depthoffield.hpp>
#include <rendering/data/buffer.hpp>


namespace legion::rendering
{
    void DefaultPipeline::setup(app::window& context)
    {
        attachStage<ClearStage>();
        attachStage<Skybox>();
        attachStage<FramebufferResizeStage>();
        attachStage<LightBufferStage>();
        attachStage<MeshBatchingStage>();
        attachStage<MeshRenderStage>();
        attachStage<ParticleBatchingStage>();
        attachStage<ParticleRenderStage>();
        attachStage<TransparencyRenderStage>();
        attachStage<DebugRenderStage>();
        attachStage<PostProcessingStage>();
        attachStage<SubmitStage>();

        PostProcessingStage::addEffect<Bloom>(-10);
        PostProcessingStage::addEffect<DepthOfField>(-15);
        PostProcessingStage::addEffect<Tonemapping>(-32);
        PostProcessingStage::addEffect<FXAA>(-64);


        buffer modelMatrixBuffer;
        buffer entityIdBuffer;
        buffer flipbookBuffer;

        {
            app::context_guard guard(context);
            addFramebuffer("main");
            modelMatrixBuffer = buffer(GL_ARRAY_BUFFER, sizeof(math::mat4) * 1024, nullptr, GL_DYNAMIC_DRAW);
            entityIdBuffer = buffer(GL_ARRAY_BUFFER, sizeof(id_type) * 1024, nullptr, GL_DYNAMIC_DRAW);
            flipbookBuffer = buffer(GL_ARRAY_BUFFER, sizeof(float) * 1024, nullptr, GL_DYNAMIC_DRAW);
        }

        create_meta<buffer>("model matrix buffer", modelMatrixBuffer);
        create_meta<buffer>("entity id buffer", entityIdBuffer);
        create_meta<buffer>("flipbook frame buffer", flipbookBuffer);
    }

}
