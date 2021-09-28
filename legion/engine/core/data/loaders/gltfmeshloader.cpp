#if !defined(DOXY_EXCLUDE)
#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define TINYGLTF_USE_CPP14
#include <tinygltf/tiny_gltf.h> 
#endif

#include <core/data/loaders/gltfmeshloader.hpp>

namespace legion::core
{
    namespace detail
    {
        static assets::asset<image> loadGLTFImage(const tinygltf::Image& img)
        {
            const auto hash = nameHash(img.name);

            auto handle = assets::get<image>(hash);
            if (handle)
                return handle;

            byte* imgData = new byte[img.image.size()]; // faux_gltf_image_loader will delete upon destruction.
            memcpy(imgData, img.image.data(), img.image.size());

            return assets::AssetCache<image>::createAsLoader<GltfFauxImageLoader>(hash, img.name, "",
                // Image constructor parameters.
                math::ivec2(img.width, img.height),
                img.pixel_type == TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE ? channel_format::eight_bit : img.pixel_type == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT ? channel_format::sixteen_bit : channel_format::float_hdr,
                img.component == 1 ? image_components::grey : img.component == 2 ? image_components::grey_alpha : img.component == 3 ? image_components::rgb : image_components::rgba,
                data_view<byte>{ imgData, img.image.size(), 0 });
        }

        /**
         * @brief Function to copy tinygltf buffer data into the correct mesh data vector
         *
         * @param buffer - The tinygltf::Buffer buffer containing the data
         * @param bufferView - the tinygltf::BufferView containing information about data size and offset
         * @param data - std::Vector<T> where the buffer is going to be copied into. The vector will be resized to vector.size()+(tinygltf data size)
         */
        template<typename T>
        static void handleGltfBuffer(const tinygltf::Model& model, const tinygltf::Accessor& accessor, std::vector<T>& data)
        {
            const tinygltf::BufferView& bufferView = model.bufferViews.at(static_cast<size_type>(accessor.bufferView));
            const tinygltf::Buffer& buffer = model.buffers.at(static_cast<size_type>(bufferView.buffer));
            const size_type bufferStart = bufferView.byteOffset + accessor.byteOffset;
            const size_type stride = static_cast<size_type>(accessor.ByteStride(bufferView));
            const size_type bufferEnd = bufferStart + accessor.count * stride;

            const size_type dataStart = data.size();
            data.reserve(dataStart + accessor.count);

            for (size_t i = bufferStart; i < bufferEnd; i += stride)
                data.push_back(*reinterpret_cast<const T*>(&buffer.data[i]));

            if (accessor.sparse.isSparse)
            {
                const auto& sparse = accessor.sparse;
                const auto& indices = sparse.indices;
                const auto& values = sparse.values;

                const auto& indexView = model.bufferViews.at(static_cast<size_type>(indices.bufferView));
                const auto& indexBuffer = model.buffers.at(static_cast<size_type>(indexView.buffer));
                const size_type indexStart = indexView.byteOffset + static_cast<size_type>(indices.byteOffset);
                const size_type indexStride = (indexView.byteStride == 0 ? sizeof(uint16) : indexView.byteStride);

                const auto& valueView = model.bufferViews.at(static_cast<size_type>(values.bufferView));
                const auto& valueBuffer = model.buffers.at(static_cast<size_type>(valueView.buffer));
                const size_type valueStart = valueView.byteOffset + static_cast<size_type>(values.byteOffset);
                const size_type valueStride = (valueView.byteStride == 0 ? sizeof(T) : valueView.byteStride);

                size_type indexPos = indexStart;
                size_type valuePos = valueStart;

                for (int i = 0; i < sparse.count; i++)
                {
                    const uint16* idx = reinterpret_cast<const uint16*>(&indexBuffer.data[indexPos]);
                    data.at(dataStart + *idx) = *reinterpret_cast<const T*>(&valueBuffer.data[valuePos]);

                    indexPos += indexStride;
                    valuePos += valueStride;
                }
            }
        }

        static void handleGltfBuffer(const tinygltf::Model& model, const tinygltf::Accessor& accessor, std::vector<math::vec3>& data, const math::mat4 transform, bool normal = false)
        {
            const tinygltf::BufferView& bufferView = model.bufferViews.at(static_cast<size_type>(accessor.bufferView));
            const tinygltf::Buffer& buffer = model.buffers.at(static_cast<size_type>(bufferView.buffer));
            const size_type bufferStart = bufferView.byteOffset + accessor.byteOffset;
            const size_type stride = static_cast<size_type>(accessor.ByteStride(bufferView));
            const size_type bufferEnd = bufferStart + accessor.count * stride;

            const size_type dataStart = data.size();
            data.reserve(dataStart + accessor.count);

            if (normal)
            {
                for (size_t i = bufferStart; i < bufferEnd; i += stride)
                {
                    const float* x = reinterpret_cast<const float*>(&buffer.data[i]);
                    const float* y = reinterpret_cast<const float*>(&buffer.data[i + sizeof(float)]);
                    const float* z = reinterpret_cast<const float*>(&buffer.data[i + 2 * sizeof(float)]);
                    data.push_back(math::normalize((transform * math::vec4(*x, *y, *z, 0)).xyz()));
                }
            }
            else
            {
                for (size_t i = bufferStart; i < bufferEnd; i += stride)
                {
                    const float* x = reinterpret_cast<const float*>(&buffer.data[i]);
                    const float* y = reinterpret_cast<const float*>(&buffer.data[i + sizeof(float)]);
                    const float* z = reinterpret_cast<const float*>(&buffer.data[i + 2 * sizeof(float)]);
                    data.push_back((transform * math::vec4(*x, *y, *z, 1)).xyz());
                }
            }

            if (accessor.sparse.isSparse)
            {
                const auto& sparse = accessor.sparse;
                const auto& indices = sparse.indices;
                const auto& values = sparse.values;

                const auto& indexView = model.bufferViews.at(static_cast<size_type>(indices.bufferView));
                const auto& indexBuffer = model.buffers.at(static_cast<size_type>(indexView.buffer));
                const size_type indexStart = indexView.byteOffset + static_cast<size_type>(indices.byteOffset);
                const size_type indexStride = (indexView.byteStride == 0 ? sizeof(uint16) : indexView.byteStride);

                const auto& valueView = model.bufferViews.at(static_cast<size_type>(values.bufferView));
                const auto& valueBuffer = model.buffers.at(static_cast<size_type>(valueView.buffer));
                const size_type valueStart = valueView.byteOffset + static_cast<size_type>(values.byteOffset);
                const size_type valueStride = (valueView.byteStride == 0 ? 3 * sizeof(float) : valueView.byteStride);

                size_type indexPos = indexStart;
                size_type valuePos = valueStart;
                if (normal)
                {
                    for (int i = 0; i < sparse.count; i++)
                    {
                        const uint16* idx = reinterpret_cast<const uint16*>(&indexBuffer.data[indexPos]);
                        const float* x = reinterpret_cast<const float*>(&valueBuffer.data[valuePos]);
                        const float* y = reinterpret_cast<const float*>(&valueBuffer.data[valuePos + sizeof(float)]);
                        const float* z = reinterpret_cast<const float*>(&valueBuffer.data[valuePos + 2 * sizeof(float)]);
                        data.at(dataStart + *idx) = math::normalize((transform * math::vec4(*x, *y, *z, 0)).xyz());

                        indexPos += indexStride;
                        valuePos += valueStride;
                    }
                }
                else
                {
                    for (int i = 0; i < sparse.count; i++)
                    {
                        const uint16* idx = reinterpret_cast<const uint16*>(&indexBuffer.data[indexPos]);
                        const float* x = reinterpret_cast<const float*>(&valueBuffer.data[valuePos]);
                        const float* y = reinterpret_cast<const float*>(&valueBuffer.data[valuePos + sizeof(float)]);
                        const float* z = reinterpret_cast<const float*>(&valueBuffer.data[valuePos + 2 * sizeof(float)]);
                        data.at(dataStart + *idx) = (transform * math::vec4(*x, *y, *z, 1)).xyz();

                        indexPos += indexStride;
                        valuePos += valueStride;
                    }
                }
            }
        }

        /**
         * @brief Function to handle vertex color of tinygltf
         *
         * @param buffer - tinygltf::Buffer containing the mesh data
         * @param bufferView - tinygltf::BufferView containing information about the buffer (data size/data offset)
         * @param accessorType - tinygltf accessorType, Vertex color is expected to come in vec3 or vec4 - will be handled by the function
         * @param componentType - tinygltf componentType, Vertex color is expected to come in float, unsigned byte or unsigned short - will be handled by the function
         * @param data - std::vector<color> the destination of the data copy. The vector will be resized to vector.size()+(tinygltf vertex color size)
         */
        static common::result<void, void> handleGltfVertexColor(const tinygltf::Model& model, const tinygltf::Accessor& accessor, int accessorType, int componentType, std::vector<math::color>& data)
        {
            const tinygltf::BufferView& bufferView = model.bufferViews.at(static_cast<size_type>(accessor.bufferView));
            const tinygltf::Buffer& buffer = model.buffers.at(static_cast<size_type>(bufferView.buffer));
            const size_type bufferStart = bufferView.byteOffset + accessor.byteOffset;
            const size_type stride = static_cast<size_type>(accessor.ByteStride(bufferView));
            const size_type bufferEnd = bufferStart + accessor.count * stride;

            const size_type dataStart = data.size();
            data.reserve(dataStart + accessor.count);

            std::vector<std::string> warnings;
            //colors in glft are in vec3/vec4 float/unsigned byte/unsigned short
            for (size_type i = bufferStart; i < bufferEnd; i += stride)
            {
                if (accessorType == TINYGLTF_TYPE_VEC3)
                {
                    // Copy the vertex colors into the vector, keeping in my mind that the vertex color data is only r,g,b
                    switch (componentType)
                    {
                    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
                    {
                        // Vertex colors in unsigned byte
                        const float r = static_cast<float>(buffer.data[i]) / 255.f;
                        const float g = static_cast<float>(buffer.data[i + sizeof(byte)]) / 255.f;
                        const float b = static_cast<float>(buffer.data[i + 2 * sizeof(byte)]) / 255.f;
                        data.emplace_back(r, g, b);
                    }
                    break;
                    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
                    {
                        // Vertex colors in unsigned short
                        // Currently not supported
                        warnings.emplace_back("Vert colors for UNSIGNED SHORT not implemented");
                    }
                    break;
                    case TINYGLTF_COMPONENT_TYPE_FLOAT:
                    {
                        // Vertex colors in float

                        const float* r = reinterpret_cast<const float*>(&buffer.data[i]);
                        const float* g = reinterpret_cast<const float*>(&buffer.data[i + sizeof(float)]);
                        const float* b = reinterpret_cast<const float*>(&buffer.data[i + 2 * sizeof(float)]);
                        data.emplace_back(*r, *g, *b);
                    }
                    break;
                    default:
                        warnings.emplace_back("Vert colors were not stored as UNSIGNED BYTE/SHORT or float, skipping");
                    }
                }
                else if (accessorType == TINYGLTF_TYPE_VEC4)
                {
                    // Copy the vertex colors into the vector, keeping in my mind that the vertex color data is r,g,b,a
                    switch (componentType)
                    {
                    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
                    {
                        // Vertex colors in unsigned byte
                        const float r = static_cast<float>(buffer.data[i]) / 255.f;
                        const float g = static_cast<float>(buffer.data[i + sizeof(byte)]) / 255.f;
                        const float b = static_cast<float>(buffer.data[i + 2 * sizeof(byte)]) / 255.f;
                        const float a = static_cast<float>(buffer.data[i + 3 * sizeof(byte)]) / 255.f;
                        data.emplace_back(r, g, b, a);
                    }
                    break;
                    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
                    {
                        // Vertex colors in unsigned short
                        // Currently not supported
                        warnings.emplace_back("Vert colors for UNSIGNED SHORT not implemented");
                    }
                    break;
                    case TINYGLTF_COMPONENT_TYPE_FLOAT:
                    {
                        // Vertex colors in float
                        const float* r = reinterpret_cast<const float*>(&buffer.data[i]);
                        const float* g = reinterpret_cast<const float*>(&buffer.data[i + sizeof(float)]);
                        const float* b = reinterpret_cast<const float*>(&buffer.data[i + 2 * sizeof(float)]);
                        const float* a = reinterpret_cast<const float*>(&buffer.data[i + 3 * sizeof(float)]);
                        data.emplace_back(*r, *g, *b, *a);
                    }
                    break;
                    default:
                        warnings.emplace_back("Vert colors were not stored as UNSIGNED BYTE/SHORT or float, skipping");
                    }
                }
                else
                    warnings.emplace_back("Vert colors were not vec3 or vec4, skipping colors");
            }


            if (accessor.sparse.isSparse)
            {
                const auto& sparse = accessor.sparse;
                const auto& indices = sparse.indices;
                const auto& values = sparse.values;

                const auto& indexView = model.bufferViews.at(static_cast<size_type>(indices.bufferView));
                const auto& indexBuffer = model.buffers.at(static_cast<size_type>(indexView.buffer));
                const size_type indexStart = indexView.byteOffset + static_cast<size_type>(indices.byteOffset);
                const size_type indexStride = (indexView.byteStride == 0 ? sizeof(uint16) : indexView.byteStride);

                const auto& valueView = model.bufferViews.at(static_cast<size_type>(values.bufferView));
                const auto& valueBuffer = model.buffers.at(static_cast<size_type>(valueView.buffer));
                const size_type valueStart = valueView.byteOffset + static_cast<size_type>(values.byteOffset);
                const size_type valueStride = (valueView.byteStride == 0 ? sizeof(uint16) : valueView.byteStride);

                size_type indexPos = indexStart;
                size_type valuePos = valueStart;

                for (int i = 0; i < sparse.count; i++)
                {
                    if (accessorType == TINYGLTF_TYPE_VEC3)
                    {
                        switch (componentType)
                        {
                        case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
                        {
                            const uint16* idx = reinterpret_cast<const uint16*>(&indexBuffer.data[indexPos]);
                            const float r = static_cast<float>(valueBuffer.data[valuePos]) / 255.f;
                            const float g = static_cast<float>(valueBuffer.data[valuePos + sizeof(byte)]) / 255.f;
                            const float b = static_cast<float>(valueBuffer.data[valuePos + 2 * sizeof(byte)]) / 255.f;
                            data.at(dataStart + *idx) = math::color(r, g, b);
                        }
                        break;
                        case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
                        {
                        }
                        break;
                        case TINYGLTF_COMPONENT_TYPE_FLOAT:
                        {
                            const uint16* idx = reinterpret_cast<const uint16*>(&indexBuffer.data[indexPos]);
                            const float* r = reinterpret_cast<const float*>(&valueBuffer.data[valuePos]);
                            const float* g = reinterpret_cast<const float*>(&valueBuffer.data[valuePos + sizeof(float)]);
                            const float* b = reinterpret_cast<const float*>(&valueBuffer.data[valuePos + 2 * sizeof(float)]);
                            data.at(dataStart + *idx) = math::color(*r, *g, *b);
                        }
                        break;
                        }
                    }
                    else if (accessorType == TINYGLTF_TYPE_VEC4)
                    {
                        switch (componentType)
                        {
                        case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
                        {
                            const uint16* idx = reinterpret_cast<const uint16*>(&indexBuffer.data[indexPos]);
                            const float r = static_cast<float>(valueBuffer.data[valuePos]) / 255.f;
                            const float g = static_cast<float>(valueBuffer.data[valuePos + sizeof(byte)]) / 255.f;
                            const float b = static_cast<float>(valueBuffer.data[valuePos + 2 * sizeof(byte)]) / 255.f;
                            const float a = static_cast<float>(valueBuffer.data[valuePos + 3 * sizeof(byte)]) / 255.f;
                            data.at(dataStart + *idx) = math::color(r, g, b, a);
                        }
                        break;
                        case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
                        {
                        }
                        break;
                        case TINYGLTF_COMPONENT_TYPE_FLOAT:
                        {
                            const uint16* idx = reinterpret_cast<const uint16*>(&indexBuffer.data[indexPos]);
                            const float* r = reinterpret_cast<const float*>(&valueBuffer.data[valuePos]);
                            const float* g = reinterpret_cast<const float*>(&valueBuffer.data[valuePos + sizeof(float)]);
                            const float* b = reinterpret_cast<const float*>(&valueBuffer.data[valuePos + 2 * sizeof(float)]);
                            const float* a = reinterpret_cast<const float*>(&valueBuffer.data[valuePos + 3 * sizeof(float)]);
                            data.at(dataStart + *idx) = math::color(*r, *g, *b, *a);
                        }
                        break;
                        }
                    }

                    indexPos += indexStride;
                    valuePos += valueStride;
                }
            }

            return { common::success, warnings };
        }

        /**
         * @brief Function to copy tinygltf indices data into std::vector
         *
         * @param buffer - The tinygltf::Buffer containting mesh data
         * @param bufferView - the tinygltf::BufferView containting data about the buffer (data size/data offset)
         * @param offset - The mesh Indices offset. ( e.g. For the first submesh 0, for the second submesh submesh[0].indices.size() )
         * @param data - The std::vector to copy the indices data into
         */
        static common::result<void, void> handleGltfIndices(const tinygltf::Model& model, const tinygltf::Accessor& accessor, size_type offset, std::vector<uint>& data)
        {
            const tinygltf::BufferView& bufferView = model.bufferViews.at(static_cast<size_type>(accessor.bufferView));
            const tinygltf::Buffer& buffer = model.buffers.at(static_cast<size_type>(bufferView.buffer));
            const size_type bufferStart = bufferView.byteOffset + accessor.byteOffset;
            const size_type stride = static_cast<size_type>(accessor.ByteStride(bufferView));
            const size_type bufferEnd = bufferStart + accessor.count * stride;

            const size_type dataStart = data.size();
            data.reserve(dataStart + accessor.count);

            std::vector<std::string> warnings;

            switch (accessor.componentType)
            {
            case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
            {
                for (size_type i = bufferStart; i < bufferEnd; i += stride)
                    data.push_back(static_cast<uint>(buffer.data[i] + offset));
            }
            break;
            case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
            {
                for (size_type i = bufferStart; i < bufferEnd; i += stride)
                    data.push_back(static_cast<uint>(*reinterpret_cast<const uint16*>(&buffer.data[i]) + offset));
            }
            break;
            case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
            {
                for (size_type i = bufferStart; i < bufferEnd; i += stride)
                    data.push_back(*reinterpret_cast<const uint*>(&buffer.data[i]) + static_cast<uint>(offset));
            }
            break;
            default:
                warnings.push_back("Index component type not supported.");
            }

            if (accessor.sparse.isSparse)
            {
                const auto& sparse = accessor.sparse;
                const auto& indices = sparse.indices;
                const auto& values = sparse.values;

                const auto& indexView = model.bufferViews.at(static_cast<size_type>(indices.bufferView));
                const auto& indexBuffer = model.buffers.at(static_cast<size_type>(indexView.buffer));
                const size_type indexStart = indexView.byteOffset + static_cast<size_type>(indices.byteOffset);
                const size_type indexStride = (indexView.byteStride == 0 ? sizeof(uint16) : indexView.byteStride);

                const auto& valueView = model.bufferViews.at(static_cast<size_type>(values.bufferView));
                const auto& valueBuffer = model.buffers.at(static_cast<size_type>(valueView.buffer));
                const size_type valueStart = valueView.byteOffset + static_cast<size_type>(values.byteOffset);
                const size_type valueStride = (valueView.byteStride == 0 ? sizeof(uint16) : valueView.byteStride);

                size_type indexPos = indexStart;
                size_type valuePos = valueStart;

                for (int i = 0; i < sparse.count; i++)
                {
                    switch (accessor.componentType)
                    {
                    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
                    {
                        const uint16* idx = reinterpret_cast<const uint16*>(&indexBuffer.data[indexPos]);

                        data.at(dataStart + *idx) = static_cast<uint>(valueBuffer.data[valuePos] + offset);
                    }
                    break;
                    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
                    {
                        const uint16* idx = reinterpret_cast<const uint16*>(&indexBuffer.data[indexPos]);

                        data.at(dataStart + *idx) = static_cast<uint>(*reinterpret_cast<const uint16*>(&valueBuffer.data[valuePos]) + offset);
                    }
                    break;
                    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
                    {
                        const uint16* idx = reinterpret_cast<const uint16*>(&indexBuffer.data[indexPos]);

                        data.at(dataStart + *idx) = *reinterpret_cast<const uint*>(&valueBuffer.data[valuePos]) + static_cast<uint>(offset);
                    }
                    break;
                    }

                    indexPos += indexStride;
                    valuePos += valueStride;
                }
            }

            return { common::success, warnings };
        }

        static math::mat4 getGltfNodeTransform(const tinygltf::Node& node)
        {
            if (node.matrix.size() == 16u)
            {

                // Use matrix attribute
                return math::mat4(
                    -static_cast<float>(node.matrix[0]), -static_cast<float>(node.matrix[1]), -static_cast<float>(node.matrix[2]), static_cast<float>(node.matrix[3]),
                    -static_cast<float>(node.matrix[4]), -static_cast<float>(node.matrix[5]), -static_cast<float>(node.matrix[6]), static_cast<float>(node.matrix[7]),
                    static_cast<float>(node.matrix[8]), static_cast<float>(node.matrix[9]), static_cast<float>(node.matrix[10]), static_cast<float>(node.matrix[11]),
                    static_cast<float>(node.matrix[12]), static_cast<float>(node.matrix[13]), static_cast<float>(node.matrix[14]), static_cast<float>(node.matrix[15]));
            }
            else
            {
                math::vec3 pos{ 0.f, 0.f, 0.f };
                math::quat rot{ 1.f, 0.f, 0.f, 0.f };
                math::vec3 scale{ 1.f, 1.f, 1.f };

                // Assume Trans x Rotate x Scale order
                if (node.scale.size() == 3)
                    scale = math::vec3(static_cast<float>(node.scale[0]), static_cast<float>(node.scale[1]), static_cast<float>(node.scale[2]));

                if (node.rotation.size() == 4)
                    rot = math::quat(static_cast<float>(node.rotation[3]), static_cast<float>(node.rotation[0]), static_cast<float>(node.rotation[1]), static_cast<float>(node.rotation[2]));

                if (node.translation.size() == 3)
                    pos = math::vec3(static_cast<float>(node.translation[0]), static_cast<float>(node.translation[1]), static_cast<float>(node.translation[2]));

                auto result = math::compose(scale, rot, pos);
                result[0] = -result[0];
                result[1] = -result[1];
                return result;
            }
        }

        static common::result<void, void> handleGltfMesh(mesh& meshData, const tinygltf::Model& model, const tinygltf::Mesh& mesh, const math::mat4& transform)
        {
            std::vector<std::string> warnings;

            sub_mesh m;
            m.name = mesh.name;
            m.indexOffset = meshData.indices.size();

            if (mesh.primitives.size())
                m.materialIndex = mesh.primitives[0].material;

            size_type primitiveIndex = 0;
            for (auto primitive : mesh.primitives)
            {
                // Loop through all primitives in the mesh
                // Primitives can be vertex position, normal, texcoord (uv) and vertex colors

                const size_type vertexOffset = meshData.vertices.size();

                for (auto& attrib : primitive.attributes)
                {
                    // Loop through the attributes of the primitive
                    // Depending on the attribute the data is copied into a different std::vector in meshData

                    const tinygltf::Accessor& accessor = model.accessors.at(static_cast<size_type>(attrib.second));
                    if (attrib.first.compare("POSITION") == 0)
                    {
                        // Position data
                        detail::handleGltfBuffer(model, accessor, meshData.vertices, transform);
                    }
                    else if (attrib.first.compare("NORMAL") == 0)
                    {
                        // Normal data
                        detail::handleGltfBuffer(model, accessor, meshData.normals, transform, true);
                    }
                    else if (attrib.first.compare("TEXCOORD_0") == 0)
                    {
                        // UV data
                        detail::handleGltfBuffer(model, accessor, meshData.uvs);
                    }
                    else if (attrib.first.compare("COLOR_0") == 0)
                    {
                        // Vertex color data
                        auto result = detail::handleGltfVertexColor(model, accessor, accessor.type, accessor.componentType, meshData.colors);
                        warnings.insert(warnings.end(), result.warnings().begin(), result.warnings().end());
                    }
                    else
                    {
                        warnings.push_back("More data to be found in gltf. Data can be accesed through: " + attrib.first);
                    }
                }


                const size_type smallestBufferSize = std::min(meshData.normals.size(), std::min(meshData.uvs.size(), meshData.colors.size()));
                const size_type vertexCount = meshData.vertices.size();
                meshData.normals.reserve(vertexCount);
                meshData.uvs.reserve(vertexCount);
                meshData.colors.reserve(vertexCount);

                for (size_type i = smallestBufferSize; i < vertexCount; ++i)
                {
                    if (meshData.normals.size() == i)
                        meshData.normals.push_back(math::vec3::up);

                    if (meshData.uvs.size() == i)
                        meshData.uvs.push_back(math::vec2(0, 0));

                    if (meshData.colors.size() == i)
                        meshData.colors.push_back(core::math::colors::white);
                }

                // Find the indices of our mesh and copy them into meshData.indices
                const size_type index = static_cast<size_type>(primitive.indices);

                if (index > model.accessors.size())
                {
                    warnings.push_back("Invalid index accessor index in primitive: " + std::to_string(primitiveIndex));
                    return { common::error, warnings };
                }

                if (primitive.indices >= 0)
                {
                    const tinygltf::Accessor& indexAccessor = model.accessors.at(index);
                    auto result = detail::handleGltfIndices(model, indexAccessor, vertexOffset, meshData.indices);
                    warnings.insert(warnings.end(), result.warnings().begin(), result.warnings().end());
                }
                else
                {
                    for (uint i = static_cast<uint>(vertexOffset); i < meshData.vertices.size(); i++)
                        meshData.indices.push_back(i);
                }
                primitiveIndex++;
            }

            // Calculate size of submesh
            m.indexCount = meshData.indices.size() - m.indexOffset;
            meshData.submeshes.push_back(m);

            return { common::success, warnings };
        }

        static common::result<void, void> handleGltfNode(mesh& meshData, const tinygltf::Model& model, const tinygltf::Node& node, const math::mat4& parentTransf)
        {
            std::vector<std::string> warnings;
            const auto transf = parentTransf * detail::getGltfNodeTransform(node);

            const size_type meshIdx = static_cast<size_type>(node.mesh);

            if (node.mesh >= 0 && meshIdx < model.meshes.size())
            {
                const math::mat4 rhToLhMat{
                     -1.f, 0.f, 0.f, 0.f,
                     0.f, -1.f, 0.f, 0.f,
                     0.f, 0.f, 1.f, 0.f,
                     0.f, 0.f, 0.f, 1.f
                };

                auto result = handleGltfMesh(meshData, model, model.meshes[meshIdx], transf * rhToLhMat);
                warnings.insert(warnings.end(), result.warnings().begin(), result.warnings().end());
            }

            for (auto& nodeIdx : node.children)
            {
                const size_type idx = static_cast<size_type>(nodeIdx);

                if (nodeIdx < 0 && idx >= model.nodes.size())
                {
                    warnings.emplace_back("invalid node in GLTF");
                    continue;
                }

                auto result = handleGltfNode(meshData, model, model.nodes[idx], transf);
                warnings.insert(warnings.end(), result.warnings().begin(), result.warnings().end());
            }

            return { common::success, warnings };
        }
    }

    using base = assets::AssetLoader<mesh>;
    using asset_ptr = base::asset_ptr;
    using import_cfg = base::import_cfg;
    using progress_type = base::progress_type;

    common::result<asset_ptr> GltfMeshLoader::loadImpl(id_type nameHash, const fs::view& file, progress_type* progress)
    {
        namespace tg = tinygltf;

        tg::Model model;
        tg::TinyGLTF loader;
        std::string err;
        std::string warn;

        std::string baseDir = "";
        std::vector<std::string> warnings;
        auto contextPath = file.parent().get_virtual_path();

        filesystem::navigator navigator(contextPath);
        auto solution = navigator.find_solution();
        if (solution.has_error())
            warnings.push_back(std::string("Invalid gltf context path, ") + solution.error().what());
        else
        {
            auto s = solution.value();
            if (s.size() != 1)
                warnings.emplace_back("Invalid gltf context path, fs::view was not fully local");
            else
            {
                filesystem::basic_resolver* resolver = dynamic_cast<filesystem::basic_resolver*>(s[0].first);
                if (!resolver)
                    warnings.emplace_back("Invalid gltf context path, fs::view was not local");
                else
                {
                    resolver->set_target(s[0].second);

                    if (!resolver->is_valid_path())
                        warnings.emplace_back("Invalid gltf context path");
                    else
                        baseDir = resolver->get_absolute_path();
                }
            }
        }

        auto result = file.get();

        warnings.insert(warnings.end(), result.warnings().begin(), result.warnings().end());
        if (!result)
            return { legion_exception_msg(result.error().what()), warnings };

        auto extension = file.get_extension();
        if (!extension)
            return { legion_exception_msg(extension.error().what()), warnings };


        if (progress)
            progress->advance_progress(5.f);

        if (extension.value() == ".gltf")
        {
            auto text = result->to_string();
            if (!loader.LoadASCIIFromString(&model, &err, &warn, text.c_str(), static_cast<uint>(text.size()), baseDir))
            {
                auto parserWarnings = common::split_string_at<'\n'>(warn);
                warnings.insert(warnings.end(), parserWarnings.begin(), parserWarnings.end());
                return { legion_exception_msg("Failed to parse GLTF: " + err), warnings };
            }
        }
        else if (extension.value() == ".glb")
        {
            if (!loader.LoadBinaryFromMemory(&model, &err, &warn, result->data(), static_cast<uint>(result->size()), baseDir))
            {
                auto parserWarnings = common::split_string_at<'\n'>(warn);
                warnings.insert(warnings.end(), parserWarnings.begin(), parserWarnings.end());
                return { legion_exception_msg("Failed to parse GLTF: " + err), warnings };
            }
        }
        else
            return { legion_exception_msg("File was not recognised as a gltf file."), warnings };

        if (progress)
            progress->advance_progress(55.f);

        if (!err.empty())
        {
            auto parserErrors = common::split_string_at<'\n'>(err);
            warnings.insert(warnings.end(), parserErrors.begin(), parserErrors.end());
        }
        if (!warn.empty())
        {
            auto parserWarnings = common::split_string_at<'\n'>(warn);
            warnings.insert(warnings.end(), parserWarnings.begin(), parserWarnings.end());
        }

        core::mesh meshData;

        const float percentagePerMat = 25.f / static_cast<float>(model.materials.size());

        for (auto& srcMat : model.materials)
        {
            auto& material = meshData.materials.emplace_back();
            auto& pbrData = srcMat.pbrMetallicRoughness;

            material.name = srcMat.name;
            material.opaque = srcMat.alphaMode != "BLEND";
            material.alphaCutoff = static_cast<float>(srcMat.alphaCutoff);
            material.doubleSided = srcMat.doubleSided;

            material.albedoValue = math::color(
                static_cast<float>(pbrData.baseColorFactor[0]),
                static_cast<float>(pbrData.baseColorFactor[1]),
                static_cast<float>(pbrData.baseColorFactor[2]),
                static_cast<float>(pbrData.baseColorFactor[3]));

            if (pbrData.baseColorTexture.index >= 0)
                material.albedoMap = detail::loadGLTFImage(
                    model.images[
                        static_cast<size_type>(model.textures[
                            static_cast<size_type>(pbrData.baseColorTexture.index)
                        ].source)
                    ]);

            material.metallicValue = static_cast<float>(pbrData.metallicFactor);
            material.roughnessValue = static_cast<float>(pbrData.roughnessFactor);

            if (pbrData.metallicRoughnessTexture.index >= 0)
                material.metallicRoughnessMap = detail::loadGLTFImage(
                    model.images[
                        static_cast<size_type>(model.textures[
                            static_cast<size_type>(pbrData.metallicRoughnessTexture.index)
                        ].source)
                    ]);

            material.emissiveValue = math::color(
                static_cast<float>(srcMat.emissiveFactor[0]),
                static_cast<float>(srcMat.emissiveFactor[1]),
                static_cast<float>(srcMat.emissiveFactor[2]));

            if (srcMat.emissiveTexture.index >= 0)
                material.emissiveMap = detail::loadGLTFImage(
                    model.images[
                        static_cast<size_type>(model.textures[
                            static_cast<size_type>(srcMat.emissiveTexture.index)
                        ].source)
                    ]);

            if (srcMat.normalTexture.index >= 0)
                material.normalMap = detail::loadGLTFImage(
                    model.images[
                        static_cast<size_type>(model.textures[
                            static_cast<size_type>(srcMat.normalTexture.index)
                        ].source)
                    ]);

            if (srcMat.occlusionTexture.index >= 0)
                material.aoMap = detail::loadGLTFImage(
                    model.images[
                        static_cast<size_type>(model.textures[
                            static_cast<size_type>(srcMat.occlusionTexture.index)
                        ].source)
                    ]);

            material.heightMap = assets::invalid_asset<image>;

            if (progress)
                progress->advance_progress(percentagePerMat);
        }

        if (!model.scenes.size())
        {
            return { legion_exception_msg("GLTF model contained 0 scenes"), warnings };
        }

        const size_type sceneToLoad = model.defaultScene < 0 ? 0 : static_cast<size_type>(model.defaultScene);

        const float percentagePerNode = 10.f / static_cast<float>(model.scenes[sceneToLoad].nodes.size());

        const math::mat4 rootMat{
                 -1.f, 0.f, 0.f, 0.f,
                 0.f, -1.f, 0.f, 0.f,
                 0.f, 0.f, 1.f, 0.f,
                 0.f, 0.f, 0.f, 1.f
        };

        for (auto& nodeIdx : model.scenes[sceneToLoad].nodes)
        {
            if (nodeIdx < 0)
                continue;

            const size_type idx = static_cast<size_type>(nodeIdx);
            detail::handleGltfNode(meshData, model, model.nodes[idx], rootMat);

            if (progress)
                progress->advance_progress(percentagePerNode);
        }

        mesh::calculate_tangents(&meshData);

        return { create(nameHash, meshData), warnings };
    }

    bool GltfMeshLoader::canLoad(const fs::view& file)
    {
        auto result = file.get_extension();
        if (!result)
            return false;

        return result.value() == ".gltf" || result.value() == ".glb";
    }

    common::result<asset_ptr> GltfMeshLoader::load(id_type nameHash, const fs::view& file, L_MAYBEUNUSED const import_cfg& settings)
    {
        OPTICK_EVENT();
        return loadImpl(nameHash, file, nullptr);
    }

    common::result<asset_ptr> GltfMeshLoader::loadAsync(id_type nameHash, const fs::view& file, L_MAYBEUNUSED const import_cfg& settings, progress_type& progress)
    {
        OPTICK_EVENT();
        return loadImpl(nameHash, file, &progress);
    }
}