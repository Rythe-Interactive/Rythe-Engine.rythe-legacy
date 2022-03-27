#pragma once
#include <rendering/data/model.hpp>
#include <rendering/data/material.hpp>

namespace legion::rendering
{
    struct [[reflectable]] skybox_renderer
    {
        material_handle material = invalid_material_handle;

        skybox_renderer() = default;
        explicit skybox_renderer(const material_handle& src) { material = src; }
    };

    struct [[reflectable]] mesh_renderer
    {
    private:
        model_handle m_tempHandle = invalid_model_handle;
    public:
        mesh_renderer() = default;
        explicit mesh_renderer(const material_handle& src) { material = src; }
        mesh_renderer(const material_handle& src, const model_handle& model) { material = src; m_tempHandle = model; }
        static void init(mesh_renderer& src, ecs::entity owner)
        {
            if (!owner.has_component<mesh_filter>())
            {
                owner.add_component<mesh_filter>(mesh_filter(src.m_tempHandle.get_mesh()));
            }
        }

        material_handle material = invalid_material_handle;
    };

    struct mesh_renderable : public ecs::archetype<mesh_filter, mesh_renderer>
    {
        using base = ecs::archetype<mesh_filter, mesh_renderer>;

        mesh_renderable() = default;
        mesh_renderable(const base::handle_group& handles) : base(handles) {}

        model_handle get_model()
        {
            id_type id = get<mesh_filter>().shared_mesh.id();
            if (id == invalid_id)
                return { invalid_id };
            return ModelCache::create_model(id);
        }

        material_handle get_material()
        {
            return get<mesh_renderer>().material;
        }
    };
}
