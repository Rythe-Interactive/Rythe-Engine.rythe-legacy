#pragma once

#include <core/core.hpp>
#include <physics/mesh_splitter_utils/splittable_polygon.hpp>
#include <physics/mesh_splitter_utils/mesh_splitter_typedefs.hpp>
#include <rendering/components/renderable.hpp>


namespace legion::physics
{
    class PrimitiveMesh
    {
    public:
        PrimitiveMesh(ecs::entity pOriginalEntity, 
            std::vector<std::shared_ptr<SplittablePolygon>>& pPolygons,
            rendering::material_handle pOriginalMaterial);
            

        ecs::entity InstantiateNewGameObject();


    private:

        void populateMesh(mesh& mesh,const math::mat4& originalTransform,math::vec3& outOffset,math::vec3& scale);

        rendering::material_handle originalMaterial;

        std::vector<std::shared_ptr<SplittablePolygon>> polygons;

        ecs::entity originalEntity;

        static int count;

    };
}



