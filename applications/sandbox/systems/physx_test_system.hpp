#pragma once
#include <core/core.hpp>
#include <rendering/rendering.hpp>
#include <physics/physics_helpers.hpp>

namespace lgn = legion;

struct ShootPhysXBox : public lgn::app::input_action<ShootPhysXBox> {};
struct ShootPhysXSphere : public lgn::app::input_action<ShootPhysXSphere> {};
struct ShootFrictionAndForceCubes : public lgn::app::input_action<ShootFrictionAndForceCubes> {};

struct MoveForward : public lgn::app::input_action<MoveForward> {};
struct MoveBackward : public lgn::app::input_action<MoveBackward> {};

struct MoveLeft : public lgn::app::input_action<MoveLeft> {};
struct MoveRight: public lgn::app::input_action<MoveRight> {};


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

        void setupBoxAndStackTestScene();

        void setupCharacterControllerTestScene();

        //------------------------ Rigidbody Shooter -------------------------------------------//

        void shootPhysXCubes(ShootPhysXBox& action);

        void shootPhysXSphere(ShootPhysXSphere& action);

        void shootFrictionTest(ShootFrictionAndForceCubes& action);

        void getCameraPositionAndDirection(math::vec3& cameraDirection, math::vec3& cameraPosition);


        //------------------------ Character Controls -------------------------------------------//

        void MoveCharacter(const math::vec3& displacement);

        void onPressForward(MoveForward& action)
        {
            if (action.pressed())
            {
                MoveCharacter(math::vec3(0, 0, 1));
            }
            
        }

        void onPresBackward(MoveBackward& action)
        {
            if (action.pressed())
            {
                MoveCharacter(math::vec3(0, 0, -1));
            }
        }

        void onPressLeft(MoveLeft& action)
        {
            if (action.pressed())
            {
                MoveCharacter(math::vec3(1, 0, 0));
            }
        }

        void onPressRight(MoveRight& action)
        {
            if (action.pressed())
            {
                MoveCharacter(math::vec3(-1, 0, 0));
            }
        }



        //-------------------------- Scene Setup Helpers ---------------------------------------//

        void initializeLitMaterial(rendering::material_handle& materialToInitialize, rendering::shader_handle& litShader,
            const fs::view& albedoFile, const fs::view& normalFile, const fs::view& roughnessFile);

        //entity,transform,and mesh
        ecs::entity createDefaultMeshEntity(math::vec3 position, rendering::model_handle cubeH,
            rendering::material_handle TextureH);

        ecs::entity createStaticColliderWall(math::vec3 position, rendering::material_handle TextureH, math::vec3 scale = math::vec3(1.0f),
            math::quat rot = math::quat(1,0,0,0));

        void suzzaneRainTick(legion::time::span deltaTime);

        void createCubeStack(const math::vec3& extents, size_t stackSize, const math::vec3& startPos);


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

        rendering::model_handle colaCanH;
        rendering::model_handle suzanneH;
        rendering::model_handle statueH;

        bool m_isRainingSuzanne = false;
        float m_currentInterval = 0.0f;

        float m_rainInterval = 1.5f;

        math::vec3 m_rainStartPos = math::vec3(10,12,-5);
        math::vec3 m_rainExtents = math::vec3(10,0,10);

        ecs::entity m_characterControllerEnt;
    };
}
