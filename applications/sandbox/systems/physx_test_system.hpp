#pragma once
#include <core/core.hpp>
#include <rendering/components/renderable.hpp>

namespace lgn = legion;

namespace legion::physics
{
    class PhysXTestSystem : public System<PhysXTestSystem>
    {
    public:

        virtual void setup();

    private:

        //wide block, 1 normal cube on the center, 1 rotated default cube on top of it
        void setupCubeWorldTestScene();

        //-------------------------- Scene Setup Helpers ---------------------------------------//

        void initializeLitMaterial(rendering::material_handle& materialToInitialize, rendering::shader_handle& litShader,
            const fs::view& albedoFile, const fs::view& normalFile, const fs::view& roughnessFile);

        //--------------------------- Rendering Variables ---------------------------------------//

        legion::rendering::shader_handle litShader;

        rendering::material_handle legionLogoMat;
        rendering::material_handle tileMat;
        rendering::material_handle concreteMat;

    };
}
