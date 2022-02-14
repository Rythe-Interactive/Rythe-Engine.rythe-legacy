#pragma once
#include <core/engine/module.hpp>
#include <core/engine/module.inl>
#include <core/data/loaders/objmeshloader.hpp>
#include <core/data/loaders/gltfmeshloader.hpp>
#include <core/data/loaders/stbimageloader.hpp>
#include <core/filesystem/provider_registry.hpp>
#include <core/filesystem/basic_resolver.hpp>
#include <core/compute/context.hpp>
#include <core/serialization/serialization.hpp>
#include <core/ecs/serialization/entity_serializer.hpp>

/**
 * @file coremodule.hpp
 */
namespace legion::core
{
    /**@class CoreModule
     * @brief Custom module.
     */
    class CoreModule final : public Module
    {
    public:
        virtual void setup()
        {
            filesystem::provider_registry::domain_create_resolver<filesystem::basic_resolver>("assets://", "./assets");
            filesystem::provider_registry::domain_create_resolver<filesystem::basic_resolver>("engine://", "./engine");

            assets::AssetCache<mesh>::addLoader<ObjMeshLoader>();
            assets::AssetCache<mesh>::addLoader<GltfMeshLoader>();

            assets::AssetCache<image>::addLoader<GltfFauxImageLoader>();
            assets::AssetCache<image>::addLoader<StbImageLoader>();

            serialization::SerializerRegistry::registerSerializer<ecs::entity>();
            serialization::SerializerRegistry::registerSerializer<ecs::entity_data>();

            createProcessChain("Update");
        }

        virtual priority_type priority() override
        {
            return PRIORITY_MAX;
        }
    };
}
