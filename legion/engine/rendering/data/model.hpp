#pragma once
#include <application/application.hpp>
#include <rendering/util/bindings.hpp>
#include <rendering/data/buffer.hpp>
#include <rendering/data/vertexarray.hpp>
#include <rendering/data/material.hpp>

#include <vector>
#include <unordered_map>
#include <set>

/**
 * @file model.hpp
 */

namespace legion::rendering
{
    /**@class model
     * @brief Struct containing all the buffer ids and the VAO needed for rendering.
     */
    struct model
    {
        bool buffered;
        vertexarray vertexArray;
        buffer vertexBuffer;
        buffer colorBuffer;
        buffer normalBuffer;
        buffer uvBuffer;
        buffer tangentBuffer;
        buffer indexBuffer;

        std::vector<material_handle> materials;

        std::vector<sub_mesh> submeshes;
    };

    /**@class model_handle
     * @brief Save to pass around handle to a model in the model cache.
     */
    struct model_handle
    {
        id_type id;
        
        bool operator==(const model_handle& other) const { return id == other.id; }
        bool is_buffered() const;
        void buffer_data(const buffer& matrixBuffer, const buffer& entityBuffer, const buffer& flipbookBuffer) const;
        void overwrite_buffer(buffer& newBuffer, uint bufferID, bool perInstance = false) const;

        assets::asset<mesh> get_mesh() const;
        const model& get_model() const;
    };

    constexpr model_handle invalid_model_handle { invalid_id };

    class ModelCache
    {
        friend class renderer;
        friend struct model_handle;
    private:
        static sparse_map<id_type, model> m_models;
        static async::rw_spinlock m_modelLock;

        static async::rw_spinlock m_modelNameLock;
        static std::unordered_map<id_type, std::string> m_modelNames;

        static const model& get_model(id_type id);

    public:
        static std::string get_model_name(id_type id);

        static void overwrite_buffer(id_type id, buffer& newBuffer, uint bufferID, bool perInstance = false);
        static void buffer_model(id_type id, const buffer& matrixBuffer, const buffer& entityBuffer, const buffer& flipbookBuffer);
        static model_handle create_model(const std::string& name, const fs::view& file, assets::import_settings<mesh> settings = {});
        static model_handle create_model(const std::string& name);
        static model_handle create_model(const std::string& name, id_type meshId);
        static model_handle create_model(id_type meshId);
        static model_handle create_model(const std::string& name, assets::asset<mesh> mesh);
        static model_handle create_model(assets::asset<mesh> mesh);
        static model_handle get_handle(const std::string& name);
        static model_handle get_handle(id_type id);
        static sparse_map<id_type, model> get_all_models();
        static assets::asset<mesh> get_mesh(const std::string& name);
        static assets::asset<mesh> get_mesh(id_type id);

        static void destroy_model(const std::string& name);

    };
}

#if !defined(DOXY_EXCLUDE)
namespace std
{
    template<>
    struct hash<legion::rendering::model_handle>
    {
        std::size_t operator()(legion::rendering::model_handle const& handle) const noexcept
        {
            return static_cast<std::size_t>(handle.id);
        }
    };
}
#endif
