#pragma once
#include <rendering/data/postprocessingeffect.hpp>
#include <rendering/util/bindings.hpp>

namespace legion::rendering
{
    enum struct tonemapping_type
    {
        aces, reinhard, reinhard_jodie, legion, unreal3
    };

    class Tonemapping : public PostProcessingEffect<Tonemapping>
    {
    private:
        static std::atomic<id_type> m_currentShader;
        static std::atomic_bool m_autoExposure;
        static std::atomic<float> m_exposure;

    public:
        static void setAlgorithm(tonemapping_type type);

        static void setAutoExposure(bool value);
        static void setExposure(float value);

        void setup(app::window& context) override;

        void renderPass(framebuffer& fbo, RenderPipelineBase* pipeline, camera& cam, const camera::camera_input& camInput, time::span deltaTime);

    };


}
