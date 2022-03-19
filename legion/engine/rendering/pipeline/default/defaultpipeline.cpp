#include <rendering/pipeline/default/defaultpipeline.hpp>
#include <rendering/pipeline/default/stages/clearstage.hpp>
#include <rendering/pipeline/default/stages/framebufferresizestage.hpp>
#include <rendering/pipeline/default/stages/lightbufferstage.hpp>
#include <rendering/pipeline/default/stages/meshbatchingstage.hpp>
#include <rendering/pipeline/default/stages/meshrenderstage.hpp>
#include <rendering/pipeline/default/stages/transparencyrenderstage.hpp>
#include <rendering/pipeline/default/stages/debugrenderstage.hpp>
#include <rendering/pipeline/default/stages/postprocessingstage.hpp>
#include <rendering/pipeline/default/stages/submitstage.hpp>
#include <rendering/pipeline/default/postfx/tonemapping.hpp>
#include <rendering/pipeline/default/postfx/fxaa.hpp>
#include <rendering/pipeline/default/postfx/bloom.hpp>
#include <rendering/pipeline/default/postfx/depthoffield.hpp>
#include <rendering/pipeline/default/postfx/skybox.hpp>
#include <rendering/data/buffer.hpp>


namespace legion::rendering
{
    void DefaultPipeline::setup(app::window& context)
    {
        attachStage<ClearStage>();
        attachStage<FramebufferResizeStage>();
        attachStage<LightBufferStage>();
        attachStage<MeshBatchingStage>();
        attachStage<MeshRenderStage>();
        attachStage<TransparencyRenderStage>();
        attachStage<DebugRenderStage>();
        attachStage<PostProcessingStage>();
        attachStage<SubmitStage>();

        PostProcessingStage::addEffect<Skybox>();
        PostProcessingStage::addEffect<DepthOfField>(-10);
        PostProcessingStage::addEffect<Bloom>(-15);
        PostProcessingStage::addEffect<Tonemapping>(-32);
        PostProcessingStage::addEffect<FXAA>(-64);


        buffer modelMatrixBuffer;

        {
            app::context_guard guard(context);
            addFramebuffer("main");
            modelMatrixBuffer = buffer(GL_ARRAY_BUFFER, sizeof(math::mat4) * 1024, nullptr, GL_DYNAMIC_DRAW);
        }

        create_meta<buffer>("model matrix buffer", modelMatrixBuffer);
    }

}
