#pragma once
#include "engine_include.hpp"

using namespace lgn;

static inline void drawOrientedCube(math::vec3 min, math::vec3 max, math::mat4 worldMat, math::color color = math::colors::white, float width = 1.f, float time = 0, bool ignoreDepth = false)
{
    std::pair<math::vec3, math::vec3> edges[] = {
        std::make_pair(min, math::vec3(min.x, min.y, max.z)),
        std::make_pair(math::vec3(min.x, min.y, max.z), math::vec3(max.x, min.y, max.z)),
        std::make_pair(math::vec3(max.x, min.y, max.z), math::vec3(max.x, min.y, min.z)),
        std::make_pair(math::vec3(max.x, min.y, min.z), min),

        std::make_pair(max, math::vec3(max.x, max.y, min.z)),
        std::make_pair(math::vec3(max.x, max.y, min.z), math::vec3(min.x, max.y, min.z)),
        std::make_pair(math::vec3(min.x, max.y, min.z), math::vec3(min.x, max.y, max.z)),
        std::make_pair(math::vec3(min.x, max.y, max.z), max),

        std::make_pair(min, math::vec3(min.x, max.y, min.z)),
        std::make_pair(math::vec3(min.x, min.y, max.z), math::vec3(min.x, max.y, max.z)),
        std::make_pair(math::vec3(max.x, min.y, max.z), math::vec3(max.x, max.y, max.z)),
        std::make_pair(math::vec3(max.x, min.y, min.z), math::vec3(max.x, max.y, min.z))
    };

    for (auto& edge : edges)
        debug::drawLine((worldMat * math::vec4(edge.first.x, edge.first.y, edge.first.z, 1.f)).xyz(), (worldMat * math::vec4(edge.second.x, edge.second.y, edge.second.z, 1.f)).xyz(), color, width, time, ignoreDepth);
}

static inline void drawOrientedSphere(float diameter, math::mat4 worldMat, math::color color = math::colors::white, float width = 1.f, float time = 0, bool ignoreDepth = false)
{
    math::vec3 vertices[] = {
       diameter * math::vec3(0.f, -0.5f, -0.f),
       diameter * math::vec3(0.212662f, -0.425327f, 0.154505f),
       diameter * math::vec3(-0.081228f, -0.425327f, 0.249997f),
       diameter * math::vec3(0.361804f, -0.22361f, 0.262862f),
       diameter * math::vec3(0.425324f, -0.262868f, -0.f),
       diameter * math::vec3(-0.262865f, -0.425326f, -0.f),
       diameter * math::vec3(-0.081228f, -0.425327f, -0.249998f),
       diameter * math::vec3(0.212662f, -0.425327f, -0.154506f),
       diameter * math::vec3(0.475529f, -0.f, 0.154507f),
       diameter * math::vec3(-0.138194f, -0.22361f, 0.425324f),
       diameter * math::vec3(0.131435f, -0.262869f, 0.404506f),
       diameter * math::vec3(0.f, -0.f, 0.5f),
       diameter * math::vec3(-0.447213f, -0.223608f, -0.f),
       diameter * math::vec3(-0.344095f, -0.262868f, 0.249998f),
       diameter * math::vec3(-0.475529f, -0.f, 0.154507f),
       diameter * math::vec3(-0.138194f, -0.22361f, -0.425325f),
       diameter * math::vec3(-0.344095f, -0.262868f, -0.249999f),
       diameter * math::vec3(-0.293893f, 0.f, -0.404509f),
       diameter * math::vec3(0.361804f, -0.22361f, -0.262863f),
       diameter * math::vec3(0.131435f, -0.262869f, -0.404506f),
       diameter * math::vec3(0.293893f, 0.f, -0.404509f),
       diameter * math::vec3(0.293893f, -0.f, 0.404509f),
       diameter * math::vec3(-0.293893f, -0.f, 0.404509f),
       diameter * math::vec3(-0.475529f, 0.f, -0.154507f),
       diameter * math::vec3(0.f, 0.f, -0.5f),
       diameter * math::vec3(0.475529f, 0.f, -0.154507f),
       diameter * math::vec3(0.138194f, 0.22361f, 0.425325f),
       diameter * math::vec3(0.344095f, 0.262868f, 0.249999f),
       diameter * math::vec3(0.081228f, 0.425327f, 0.249998f),
       diameter * math::vec3(-0.361804f, 0.22361f, 0.262863f),
       diameter * math::vec3(-0.131435f, 0.262869f, 0.404506f),
       diameter * math::vec3(-0.212662f, 0.425327f, 0.154506f),
       diameter * math::vec3(-0.361804f, 0.22361f, -0.262862f),
       diameter * math::vec3(-0.425324f, 0.262868f, 0.f),
       diameter * math::vec3(-0.212662f, 0.425327f, -0.154505f),
       diameter * math::vec3(0.138194f, 0.22361f, -0.425324f),
       diameter * math::vec3(-0.131435f, 0.262869f, -0.404506f),
       diameter * math::vec3(0.081228f, 0.425327f, -0.249997f),
       diameter * math::vec3(0.447213f, 0.223608f, 0.f),
       diameter * math::vec3(0.344095f, 0.262868f, -0.249998f),
       diameter * math::vec3(0.262865f, 0.425326f, 0.f),
       diameter * math::vec3(0.f, 0.5f, 0.f)
    };

    std::pair<size_type, size_type> indices[]{
        {0,1},{1,2},{2,0},{3,1},{1,4},{4,3},{2,5},{5,0},{5,6},{6,0},{6,7},{7,0},
        {4,8},{8,3},{9,10},{10,11},{11,9},{12,13},{13,14},{14,12},{15,16},{16,17},
        {17,15},{18,19},{19,20},{20,18},{8,21},{21,3},{11,22},{22,9},{14,23},
        {23,12},{17,24},{24,15},{20,25},{25,18},{26,27},{27,28},{28,26},{29,30},
        {30,31},{31,29},{32,33},{33,34},{34,32},{35,36},{36,37},{37,35},{38,39},
        {39,40},{40,38},{40,37},{37,41},{41,40},{39,37},{39,35},{37,34},{34,41},
        {36,34},{36,32},{34,31},{31,41},{33,31},{33,29},{31,28},{28,41},{30,28},
        {30,26},{28,40},{27,40},{27,38},{25,39},{38,25},{20,39},{20,35},{24,36},
        {35,24},{17,36},{17,32},{23,33},{32,23},{14,33},{14,29},{22,30},{29,22},
        {11,30},{11,26},{21,27},{26,21},{8,27},{8,38},{20,24},{19,24},{19,15},
        {17,23},{16,23},{16,12},{14,22},{13,22},{13,9},{11,21},{10,21},{10,3},
        {8,25},{4,25},{4,18},{7,19},{18,7},{6,19},{6,15},{6,16},{5,16},{5,12},
        {5,13},{2,13},{2,9},{4,7},{1,7},{2,10},{1,10} };

    for (auto& [start, end] : indices)
        debug::drawLine((worldMat * math::vec4(vertices[start].x, vertices[start].y, vertices[start].z, 1.f)).xyz(), (worldMat * math::vec4(vertices[end].x, vertices[end].y, vertices[end].z, 1.f)).xyz(), color, width, time, ignoreDepth);
}
