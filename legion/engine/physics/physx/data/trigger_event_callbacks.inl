#include <physx/PxPhysicsAPI.h>
#include <physics/physx/data/physx_wrapper.hpp>
#include <physics/events/events.hpp>

namespace legion::physics
{
    using namespace physx;

    template <class HitBuffer>
    static void processBuffer(const HitBuffer* buffer, PxU32 nbHits,
        ecs::entity controllerEntity, pointer<PhysxCharacterWrapper> characterWrapperPtr)
    {
        PxActor* self = characterWrapperPtr->characterController->getActor();

        std::vector<PxActor*>& prevFrameActors = characterWrapperPtr->actorsCollidedWith;

        for (size_t currentActorIdx = 0; currentActorIdx < nbHits; currentActorIdx++)
        {
            if (buffer[currentActorIdx].actor == self) { continue; }

            ecs::entity touchedEntity = { static_cast<ecs::entity_data*>(buffer[currentActorIdx].actor->userData) };
            collision_info info{ controllerEntity,touchedEntity };

            bool foundOnPrevAndCurrent = false;

            //check if we previously collided with this actor
            for (size_t actorPrevIdx = 0; actorPrevIdx < prevFrameActors.size(); actorPrevIdx++)
            {
                if (buffer[currentActorIdx].actor == prevFrameActors[actorPrevIdx])
                {
                    on_trigger_stay triggerStay;
                    triggerStay.collision = info;
                    log::debug("on trigger stay found");
                    events::EventBus::raiseEvent(triggerStay);

                    foundOnPrevAndCurrent = true;
                    break;
                }
            }

            if (foundOnPrevAndCurrent) { continue; }

            log::debug("on trigger enter found");
            
            on_trigger_enter triggerEnter;
            triggerEnter.collision = info;

            events::EventBus::raiseEvent(triggerEnter);
            //if we reached here on trigger enter
        }

        //check if there any actors that we previously collided with but are not colliding with now
        for (PxActor* prevActor : prevFrameActors)
        {
            bool inPrevButNotCurrent = true;

            for (size_t currentActorIdx = 0; currentActorIdx < nbHits; currentActorIdx++)
            {
                //find actor in current
                if (buffer[currentActorIdx].actor == prevActor)
                {
                    inPrevButNotCurrent = false;
                    break;
                }
            }

            if (inPrevButNotCurrent)
            {
                ecs::entity touchedEntity = { static_cast<ecs::entity_data*>(prevActor->userData) };

                collision_info info{ controllerEntity,touchedEntity };
                on_trigger_exit triggerExit;
                triggerExit.collision = info;
                log::debug("on trigger end found");
                events::EventBus::raiseEvent(triggerExit);
            }
            //if we reach here on trigger exit
        }

        prevFrameActors.clear();

        for (size_t currentActorIdx = 0; currentActorIdx < nbHits; currentActorIdx++)
        {
            if (buffer[currentActorIdx].actor != self)
            {
                prevFrameActors.push_back(buffer[currentActorIdx].actor);
            }
        }

        return false;
    }

    struct ControllerMoveSweepCallback : public PxSweepCallback
    {
        ControllerMoveSweepCallback(PhysxCharacterWrapper& characterWrapper, ecs::entity controllerEnt, PxSweepHit* aTouches, PxU32 aMaxNbTouches) :
            PxSweepCallback(aTouches, aMaxNbTouches), controllerEntity(controllerEnt), characterWrapperPtr{&characterWrapper}
        {
            
        }

        PxAgain processTouches(const PxSweepHit* buffer, PxU32 nbHits)
        {
            processBuffer(buffer, nbHits,
                controllerEntity, characterWrapperPtr);

            return false;
        }

        ecs::entity controllerEntity;
        pointer<PhysxCharacterWrapper> characterWrapperPtr{ nullptr };
    };

    struct ControllerOverlapCallback : public PxOverlapCallback
    {
        ControllerOverlapCallback(PhysxCharacterWrapper& characterWrapper, ecs::entity controllerEnt, PxOverlapHit* aTouches, PxU32 aMaxNbTouches) :
            PxOverlapCallback(aTouches, aMaxNbTouches), controllerEntity(controllerEnt), characterWrapperPtr{ &characterWrapper }
        {

        }

        PxAgain processTouches(const PxOverlapHit* buffer, PxU32 nbHits)
        {
            processBuffer(buffer, nbHits,
                controllerEntity, characterWrapperPtr);

            return false;
        }

        ecs::entity controllerEntity;
        pointer<PhysxCharacterWrapper> characterWrapperPtr{ nullptr };
    };

}
