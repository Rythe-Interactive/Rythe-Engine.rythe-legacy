#include <core/defaults/coremodule.hpp>
#include <core/engine/system.inl>
#include <core/engine/module.inl>
#include <core/data/loaders/objmeshloader.hpp>
#include <core/data/loaders/gltfmeshloader.hpp>
#include <core/data/loaders/stbimageloader.hpp>
#include <core/filesystem/provider_registry.hpp>
#include <core/filesystem/basic_resolver.hpp>
#include <core/compute/context.hpp>
#include <core/particles/particles.hpp>

namespace legion::core
{
    void CoreModule::setup()
    {
        filesystem::provider_registry::domain_create_resolver<filesystem::basic_resolver>("assets://", "./assets");
        filesystem::provider_registry::domain_create_resolver<filesystem::basic_resolver>("engine://", "./engine");

        assets::AssetCache<mesh>::addLoader<ObjMeshLoader>();
        assets::AssetCache<mesh>::addLoader<GltfMeshLoader>();

        assets::AssetCache<image>::addLoader<GltfFauxImageLoader>();
        assets::AssetCache<image>::addLoader<StbImageLoader>();

        createProcessChain("Update");

        reportSystem<ParticleSystem>();
    }

    priority_type CoreModule::priority()
    {
        return PRIORITY_MAX;
    }
}
