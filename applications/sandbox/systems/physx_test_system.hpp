#pragma once
#include <core/core.hpp>
#include <rendering/rendering.hpp>

namespace lgn = legion;

struct ShootPhysXBox : public lgn::app::input_action<ShootPhysXBox> {};
struct ShootPhysXSphere : public lgn::app::input_action<ShootPhysXSphere> {};

struct self_destruct_component
{
    float selfDestructTimer = 0.0f;
};

struct self_destruct_component
{
    float selfDestructTimer = 0.0f;
};

namespace legion::physics
{
    class PhysXTestSystem : public System<PhysXTestSystem>
    {
    public:

        virtual void setup();

        void update(legion::time::span deltaTime);

    private:

        //wide block, 1 normal cube on the center, 1 rotated default cube on top of it
        void setupCubeWorldTestScene();
        void shootPhysXCubes(ShootPhysXBox& action);

        //------------------------ Rigidbody Shooter -------------------------------------------//

        void shootPhysXCubes(ShootPhysXBox& action);

        void shootPhysXSphere(ShootPhysXSphere& action);

        void getCameraPositionAndDirection(math::vec3& cameraDirection, math::vec3& cameraPosition);

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
        rendering::model_handle sphereH;
        rendering::model_handle directionalLightH; 
    };
}
