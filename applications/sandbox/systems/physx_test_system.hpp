#pragma once
#include <core/core.hpp>
#include <rendering/rendering.hpp>

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

        //entity,transform,and mesh
        ecs::entity createDefaultMeshEntity(math::vec3 position, rendering::model_handle cubeH,
            rendering::material_handle TextureH);

        //--------------------------- Rendering Variables ---------------------------------------//

        legion::rendering::shader_handle litShader;

        //materials
        rendering::material_handle legionLogoMat;
        rendering::material_handle tileMat;
        rendering::material_handle concreteMat;

        rendering::material_handle directionalLightMH;
        
        //models
        rendering::model_handle cubeH;
        rendering::model_handle directionalLightH; 
    };
}
