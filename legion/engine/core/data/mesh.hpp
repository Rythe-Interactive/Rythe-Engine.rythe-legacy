#pragma once
#include <core/types/primitives.hpp>
#include <core/math/math.hpp>
#include <core/data/image.hpp>

/**
 * @file mesh.hpp
 */

namespace legion::core
{
    enum struct transparency_mode
    {
        Opaque,
        Mask,
        Blend
    };

    struct [[reflectable]] material_data
    {
        std::string name;

        transparency_mode transparencyMode;
        float alphaCutoff;
        bool doubleSided;

        math::color albedoValue;
        assets::asset<image> albedoMap;
        float metallicValue;
        assets::asset<image> metallicMap;
        float roughnessValue;
        assets::asset<image> roughnessMap;
        assets::asset<image> metallicRoughnessMap;
        math::color emissiveValue;
        assets::asset<image> emissiveMap;

        assets::asset<image> normalMap;
        assets::asset<image> aoMap;
        assets::asset<image> heightMap;
    };

    enum struct winding_order : byte
    {
        clockwise,
        counter_clockwise
    };

    /**@class sub_mesh
     * @brief Encapsulation of a sub-mesh with the offsets and sizes of the sub-mesh within the main mesh data.
     */
    struct [[reflectable]] sub_mesh
    {
        std::string name;
        size_type indexCount;
        size_type indexOffset;
        int32 materialIndex;
    };

    /**@class mesh
     * @brief Raw mesh representation.
     */
    struct [[reflectable]] mesh
    {
        std::vector<math::vec3> vertices;
        std::vector<math::color> colors;
        std::vector<math::vec3> normals;
        std::vector<math::vec2> uvs;
        std::vector<math::vec3> tangents;
        std::vector<uint> indices;
        std::vector<material_data> materials;

        winding_order windingOrder;

        std::vector<sub_mesh> submeshes;

        /**@brief Calculate the tangents from the triangles, vertices and normals of a certain mesh.
         */
        static void calculate_tangents(mesh* data);
    };

    ReportAssetType(mesh);

    template<>
    struct assets::import_settings<mesh>
    {
        bool triangulate = true;
        bool keepNativeCoords = false;
        bool flipVerticalTexcoords = true;
        winding_order windingOrder = winding_order::clockwise;
        math::mat4 transform = math::mat4(1.f);
    };
}
