#pragma once
#include <rendering/data/importers/texture_importers.hpp>
#include <rendering/systems/renderer.hpp>
#include <rendering/components/renderable.hpp>
#include <rendering/components/light.hpp>
#include <rendering/data/serializers/material_serializer.hpp>

namespace legion::rendering
{
    class RenderingModule : public Module
    {
    public:
        virtual void setup() override
        {
            for (cstring extension : stbi_texture_loader::extensions)
                fs::AssetImporter::reportConverter<stbi_texture_loader>(extension);

            srl::SerializerRegistry::registerSerializer<material_handle>();
            srl::SerializerRegistry::registerSerializer<mesh_renderer>();
            srl::SerializerRegistry::registerSerializer<skybox_renderer>();

            registerComponentType<camera>();
            registerComponentType<mesh_renderer>();
            registerComponentType<light>();
            reportSystem<Renderer>();
        }

        virtual priority_type priority() override
        {
            return 99;
        }

    };
}
