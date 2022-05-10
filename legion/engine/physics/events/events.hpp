#pragma once
#include <core/events/event.hpp>
#include <physics/diviner/data/physics_manifold.hpp>

namespace legion::physics {


    template <class T>
    struct collision_event_base : public events::event<T>
    {
        collision_event_base(physics_manifold* m, float d) : manifold(m) , physics_delta(d) {}

        collision_event_base(const collision_event_base& other) = default;
        collision_event_base(collision_event_base&& other) noexcept = default;
        collision_event_base& operator=(const collision_event_base& other) = default;
        collision_event_base& operator=(collision_event_base&& other) noexcept = default;

        physics_manifold* manifold;


        /** @brief utility function to quickly get the participating entities
         *         of this collision.
         */

        L_NODISCARD std::pair<ecs::entity,ecs::entity> participants() const
        {
            return std::make_pair(
                manifold->entityA,
                manifold->entityB
            );
        }

        /** @brief gets the centers of the colliders in local space
         */
        L_NODISCARD std::pair<math::vec3,math::vec3> centers() const noexcept
        {
            return std::make_pair(
                manifold->colliderA->GetLocalCentroid(),
                manifold->colliderB->GetLocalCentroid()
            );
        }

        /** @brief gets the colliders themselves
         */
        L_NODISCARD std::pair<PhysicsCollider*,
                             PhysicsCollider*> colliders() const noexcept
        {
            return std::make_pair(manifold->colliderA, manifold->colliderB);
        }

        float physics_delta;
    };

    /** @class trigger_event
     *  @brief This event is raised when two triggers in the physics-system collide.
     *         It contains the collision event in the manifold and the delta time of
     *         the physics system
     */
    struct trigger_event : public collision_event_base<trigger_event>
    {
        using collision_event_base<trigger_event>::collision_event_base;
    };

    /** @class collision_event
     *  @brief This event is raised when two triggers in the physics-system collide.
     *         It contains the collision event in the manifold and the delta time of
     *         the physics system
     */
    struct collision_event : public collision_event_base<collision_event>
    {
        using collision_event_base<collision_event>::collision_event_base;
    };

    struct request_create_physics_material : public events::event<request_create_physics_material>
    {
        float newDynamicFriction;
        float newStaticFriction;
        float newRestitution;
        size_type newMaterialHash;

        request_create_physics_material(float dynamicFriction, float staticFriction,
            float restitution, size_type materialHash)
            : newDynamicFriction(dynamicFriction), newStaticFriction(staticFriction),
            newRestitution(restitution), newMaterialHash(materialHash)
        {

        }
    };

    struct collision_info
    {
        ecs::entity firstEntity;
        ecs::entity secondEntity;
    };

    struct on_trigger_enter : public events::event< on_trigger_enter>
    {
        collision_info collision;
    };

    struct on_trigger_stay : public events::event<on_trigger_stay>
    {
        collision_info collision;
    };

    struct on_trigger_exit : public events::event<on_trigger_exit>
    {
        collision_info collision;
    };

    //-------------------------------------------- Debugging Related ---------------------------------------------//

    struct request_flip_physics_continuous final : public events::event<request_flip_physics_continuous>
    {
        request_flip_physics_continuous(bool continuousState) : newContinuousState{ continuousState } {}

        bool newContinuousState;
    };

    struct request_single_physics_tick final : public events::event<request_single_physics_tick> { };
}
