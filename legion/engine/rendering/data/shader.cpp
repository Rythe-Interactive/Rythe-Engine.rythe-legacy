#include <rendering/data/shader.hpp>
#include <rendering/util/bindings.hpp>
#include <algorithm>
#include <rendering/shadercompiler/shadercompiler.hpp>

namespace legion::rendering
{
    sparse_map<id_type, shader> ShaderCache::m_shaders;
    async::rw_spinlock ShaderCache::m_shaderLock;
    std::unordered_set<id_type> ShaderCache::m_checkedPaths;

    shader* ShaderCache::get_shader(id_type id)
    {
        async::readonly_guard guard(m_shaderLock);

#if defined(LEGION_VALIDATE)
        if (!m_shaders.count(id))
        {
            if (!m_shaders.contains(invalid_id) || (!m_shaders.at(invalid_id).has_variant(0)))
            {
                create_invalid_shader(fs::view("engine://shaders/invalid.shs"));
            }

            return &m_shaders.at(invalid_id);
        }
#endif

        return &m_shaders.at(id);
    }

    void ShaderCache::process_io(shader& shader, id_type id)
    {
        for (auto& [_, variant] : shader.m_variants)
        {
            // Clear all values.
            variant.uniforms.clear();
            variant.attributes.clear();

#pragma region uniforms
            // Find the number of active uniforms.
            GLint numActiveUniforms = 0;
            glGetProgramiv(variant.programId, GL_ACTIVE_UNIFORMS, &numActiveUniforms);

            GLint maxUniformNameLength = 0; // Get name buffer length.
            glGetProgramiv(variant.programId, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxUniformNameLength);

            GLchar* uniformNameBuffer = new GLchar[maxUniformNameLength]; // Create buffer with the right length.

            uint textureUnit = 1;

            for (int uniformId = 0; uniformId < numActiveUniforms; uniformId++)
            {
                GLint arraySize = 0; // Use this later for uniform arrays.
                GLenum type = 0;
                GLsizei nameLength = 0;
                glGetActiveUniform(variant.programId, uniformId, (GLsizei)maxUniformNameLength, &nameLength, &arraySize, &type, uniformNameBuffer);

                std::string_view name(uniformNameBuffer, nameLength + 1); // Get string_view of the actual name within the buffer.

                if (name.find('[') != std::string_view::npos) // We don't support uniform arrays yet.
                    continue;

                // Get location and create uniform object.
                app::gl_location location = glGetUniformLocation(variant.programId, uniformNameBuffer);
                shader_parameter_base* uniform = nullptr;
                switch (type)
                {
                case GL_SAMPLER_2D_ARRAY:
                    uniform = new rendering::uniform<texture_handle>(id, name, type, location, textureUnit);
                    textureUnit++;
                    break;
                case GL_SAMPLER_2D:
                    uniform = new rendering::uniform<texture_handle>(id, name, type, location, textureUnit);
                    textureUnit++;
                    break;
                case GL_UNSIGNED_INT:
                    uniform = new rendering::uniform<uint>(id, name, type, location);
                    break;
                case GL_FLOAT:
                    uniform = new rendering::uniform<float>(id, name, type, location);
                    break;
                case GL_FLOAT_VEC2:
                    uniform = new rendering::uniform<math::vec2>(id, name, type, location);
                    break;
                case GL_FLOAT_VEC3:
                    uniform = new rendering::uniform<math::vec3>(id, name, type, location);
                    break;
                case GL_FLOAT_VEC4:
                    uniform = new rendering::uniform<math::vec4>(id, name, type, location);
                    break;
                case GL_INT:
                    uniform = new rendering::uniform<int>(id, name, type, location);
                    break;
                case GL_INT_VEC2:
                    uniform = new rendering::uniform<math::ivec2>(id, name, type, location);
                    break;
                case GL_INT_VEC3:
                    uniform = new rendering::uniform<math::ivec3>(id, name, type, location);
                    break;
                case GL_INT_VEC4:
                    uniform = new rendering::uniform<math::ivec4>(id, name, type, location);
                    break;
                case GL_BOOL:
                    uniform = new rendering::uniform<bool>(id, name, type, location);
                    break;
                case GL_BOOL_VEC2:
                    uniform = new rendering::uniform<math::bvec2>(id, name, type, location);
                    break;
                case GL_BOOL_VEC3:
                    uniform = new rendering::uniform<math::bvec3>(id, name, type, location);
                    break;
                case GL_BOOL_VEC4:
                    uniform = new rendering::uniform<math::bvec4>(id, name, type, location);
                    break;
                case GL_FLOAT_MAT2:
                    uniform = new rendering::uniform<math::mat2>(id, name, type, location);
                    break;
                case GL_FLOAT_MAT3:
                    uniform = new rendering::uniform<math::mat3>(id, name, type, location);
                    break;
                case GL_FLOAT_MAT4:
                    uniform = new rendering::uniform<math::mat4>(id, name, type, location);
                    break;
                default:
                    continue;
                }

                // Insert uniform into the uniform list.
                auto hashid = nameHash(name);
                variant.uniforms[hashid] = std::unique_ptr<shader_parameter_base>(uniform);
                variant.idOfLocation[location] = hashid;
            }

            delete[] uniformNameBuffer; // Delete name buffer
#pragma endregion
#pragma region attributes
        // Find the number of active attributes.
            GLint numActiveAttribs = 0;
            glGetProgramiv(variant.programId, GL_ACTIVE_ATTRIBUTES, &numActiveAttribs);

            GLint maxAttribNameLength = 0; // Get name buffer length.
            glGetProgramiv(variant.programId, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxAttribNameLength);

            GLchar* attribNameBuffer = new GLchar[maxAttribNameLength]; // Create buffer with the right length.

            for (int attrib = 0; attrib < numActiveAttribs; ++attrib)
            {
                GLint arraySize = 0; // Use this later for attribute arrays.
                GLenum type = 0;
                GLsizei nameLength = 0;
                glGetActiveAttrib(variant.programId, attrib, (GLsizei)maxAttribNameLength, &nameLength, &arraySize, &type, attribNameBuffer);

                std::string_view name(attribNameBuffer, nameLength + 1); // Get string_view of the actual name within the buffer.

                if (name.find('[') != std::string_view::npos) // We don't support attribute arrays yet.
                    continue;

                // Get location and create attribute object
                GLint location = glGetAttribLocation(variant.programId, attribNameBuffer);
                variant.attributes[nameHash(name)] = std::unique_ptr<attribute>(new attribute(id, name, type, location));
            }

            delete[] attribNameBuffer;
#pragma endregion
        }
    }

    app::gl_id ShaderCache::compile_shader(GLuint shaderType, cstring source, GLint sourceLength)
    {
        // Create and compile shader.
        app::gl_id shaderId = glCreateShader(shaderType);
        glShaderSource(shaderId, 1, &source, &sourceLength);
        glCompileShader(shaderId);

        // Fetch compile status.
        GLint compilerResult = GL_FALSE;
        glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compilerResult);

        // Check for errors.
        if (!compilerResult)
        {
            // Create message buffer and fetch message.
            GLint infoLogLength;
            glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLogLength);
            const char* errorMessage;
            if (infoLogLength > 0 && infoLogLength < 65536)
            {
                char* temp = new char[infoLogLength + 1];
                glGetShaderInfoLog(shaderId, infoLogLength, nullptr, temp);
                temp[infoLogLength] = '\0';
                errorMessage = temp;
            }
            else
            {
                errorMessage = "Unknown error, OpenGL context might not have been made current?";
            }
            cstring shaderTypename;
            switch (shaderType)
            {
            case GL_FRAGMENT_SHADER:
                shaderTypename = "fragment";
                break;
            case GL_VERTEX_SHADER:
                shaderTypename = "vertex";
                break;
            case GL_GEOMETRY_SHADER:
                shaderTypename = "geometry";
                break;
            case GL_TESS_CONTROL_SHADER:
                shaderTypename = "tessellation control";
                break;
            case GL_TESS_EVALUATION_SHADER:
                shaderTypename = "tessellation evaluation";
                break;
            case GL_COMPUTE_SHADER:
                shaderTypename = "compute";
                break;
            default:
                shaderTypename = "unknown type";
                break;
            }

            log::error("Error compiling {} shader:\n\t{}", shaderTypename, errorMessage);
            if (infoLogLength > 0 && infoLogLength < 65536)
                delete[] errorMessage; // Delete message.

            glDeleteShader(shaderId); // Delete shader.
            return -1;
        }
        return shaderId;
    }

    bool ShaderCache::load_precompiled(const fs::view& file, shader_ilo& ilo, std::unordered_map<std::string, shader_state>& state)
    {
        log::info("Loading precompiled shader: {}", file.get_virtual_path());
        auto result = file.get();
        if (result != common::valid)
            return false;

        auto resource = result.value();

        if (resource.size() <= 22)
            return false;

        byte_vec data = resource.get();

        std::string_view magic(reinterpret_cast<char*>(data.data()), 19);
        if (magic != "\xabLEGION SHADER\xbb\r\n\x13\n")
            return false;

        auto start = data.cbegin() + 19;
        auto end = data.cend();

        while (start != end)
        {
            GLenum shaderType;
            retrieveBinaryData(shaderType, start);

            std::string shaderVariant;
            retrieveBinaryData(shaderVariant, start);

            switch (shaderType)
            {
            case 0:
            {
                std::vector<GLenum> rawState;
                retrieveBinaryData(rawState, start);
                if (rawState.size() % 2 != 0)
                    return false;

                shader_state& variantState = state[shaderVariant];
                for (int i = 0; i < rawState.size(); i += 2)
                {
                    variantState[rawState[i]] = rawState[i + 1];
                }
            }
            break;
            case GL_VERTEX_SHADER:
            case GL_FRAGMENT_SHADER:
            case GL_GEOMETRY_SHADER:
            {
                std::string source;
                retrieveBinaryData(source, start);

                ilo[shaderVariant].emplace_back(shaderType, source);
            }
            break;
            default:
                return false;
            }
        }
        return true;
    }

    void ShaderCache::store_precompiled(const fs::view& file, const shader_ilo& ilo, const std::unordered_map<std::string, shader_state>& state)
    {
        auto result = file.get_extension();
        if (result != common::valid)
            return;

        fs::view precompiled("");
        if (result.value() == ".shil" || result.value().empty())
            precompiled = file;
        else
            precompiled = file / ".." / (file.get_filestem().value() + ".shil");

        if (precompiled.is_valid(true) && precompiled.file_info().can_be_written)
        {
            fs::basic_resource resource(nullptr);
            byte_vec& data = resource.get();

            std::string magic = "\xabLEGION SHADER\xbb\r\n\x13\n";
            for (auto item : magic)
                data.push_back(item);

            std::vector<GLenum> rawState;
            for (auto& [variant, variantState] : state)
            {
                GLenum stateType = 0;
                appendBinaryData(&stateType, data);
                appendBinaryData(&variant, data);
                rawState.clear();
                for (auto& [key, value] : variantState)
                {
                    rawState.push_back(key);
                    rawState.push_back(value);
                }
                appendBinaryData(&rawState, data);
            }

            for (auto& [shaderVariant, variantSource] : ilo)
                for (auto& [shaderType, source] : variantSource)
                {
                    appendBinaryData(&shaderType, data);
                    appendBinaryData(&shaderVariant, data);
                    appendBinaryData(&source, data);
                }

            precompiled.set(resource).except([](fs_error err)
                {
                    log::error("error occurred in {} at {} line {}: {}", err.file(), err.func(), err.file(), err.what());
                });
        }

    }

    shader_handle ShaderCache::create_invalid_shader(const fs::view& file, shader_import_settings settings)
    {
        ShaderCompiler::setErrorCallback([](const std::string& errormsg, log::severity severity)
            {
                log::println(severity, errormsg);
            });

        clear_modified_from_cache(file.parent());

        std::unordered_map<std::string, shader_state> state;
        shader_ilo shaders;

        auto result = file.get_extension();
        if (result != common::valid)
            return invalid_shader_handle;

        bool compiledFromScratch = false;

        if (result.value().empty() || result.value() == ".shil")
        {
            if (!load_precompiled(file, shaders, state))
                return invalid_shader_handle;
        }
        else
        {
            do
            {
                if (settings.usePrecompiledIfAvailable) // Clean up this monster function...
                {
                    auto precompiled = file / ".." / (file.get_filestem().value() + ".shil");

                    if (precompiled.is_valid(true))
                    {
                        auto traits = precompiled.file_info();
                        if (traits.is_file && traits.can_be_read)
                        {
                            if (load_precompiled(precompiled, shaders, state))
                                break;
                        }
                    }
                }

                byte compilerSettings = 0;
                compilerSettings |= settings.api;
                if (settings.debug)
                    compilerSettings |= shader_compiler_options::debug;
                if (settings.low_power)
                    compilerSettings |= shader_compiler_options::low_power;

                if (!ShaderCompiler::process(file, compilerSettings, shaders, state, detail::get_default_defines()))
                    return invalid_shader_handle;

                compiledFromScratch = true;

            } while (false);
        }

        if (shaders.empty())
            return invalid_shader_handle;

        shader shader;
        shader.name = "invalid";
        for (auto& [variant, variantState] : state)
        {
            shader.m_variants[nameHash(variant)].state = variantState;
        }

        for (auto& [shaderVariant, variantSource] : shaders)
        {
            id_type variantId = nameHash(shaderVariant);
            shader_variant& variant = shader.m_variants[variantId];
            variant.name = shaderVariant;
            GLenum blendSrc = GL_FALSE, blendDst = GL_FALSE;

            for (auto& [func, param] : variant.state)
            {
                switch (func)
                {
                case GL_DEPTH_TEST:
                {
                    if (param == GL_FALSE)
                    {
                        glDisable(func);
                        continue;
                    }

                    glEnable(func);
                    glDepthFunc(param);
                }
                break;
                case GL_DEPTH_WRITEMASK:
                {
                    glDepthMask(param);
                }
                break;
                case GL_CULL_FACE:
                {
                    if (param == GL_FALSE)
                    {
                        glDisable(func);
                        continue;
                    }

                    glEnable(func);
                    glCullFace(param);
                }
                break;
                case GL_BLEND:
                {
                    if (blendSrc == GL_FALSE)
                        blendSrc = param;
                    if (blendDst == GL_FALSE)
                        blendDst = param;
                }
                break;
                case GL_BLEND_SRC:
                {
                    if (blendSrc == GL_FALSE)
                        blendSrc = param;
                }
                break;
                case GL_BLEND_DST:
                {
                    if (blendDst == GL_FALSE)
                        blendDst = param;
                }
                break;
                case GL_DITHER:
                {
                    if (param == GL_TRUE)
                        glEnable(GL_DITHER);
                    else
                        glDisable(GL_DITHER);
                }
                break;
                default:
                    break;
                }
            }

            if (blendSrc == GL_FALSE || blendDst == GL_FALSE)
            {
                glDisable(GL_BLEND);
            }
            else
            {
                glEnable(GL_BLEND);
                glBlendFunc(blendSrc, blendDst);
            }

            variant.programId = glCreateProgram();

            std::vector<app::gl_id> shaderIds;

            for (auto& [shaderType, shaderIL] : variantSource)
            {
                auto shaderId = compile_shader(shaderType, shaderIL.c_str(), shaderIL.size());

                if (shaderId == (app::gl_id)-1)
                {
                    log::error("Error occurred in shader: invalid\n");

                    for (auto id : shaderIds)
                    {
                        glDetachShader(variant.programId, id);
                        glDeleteShader(id);
                    }

                    glDeleteProgram(variant.programId);
                    shader.m_variants.erase(variantId);
                    continue;
                }

                glAttachShader(variant.programId, shaderId);
                shaderIds.push_back(shaderId);
            }

            glLinkProgram(variant.programId);

            GLint linkStatus;
            glGetProgramiv(variant.programId, GL_LINK_STATUS, &linkStatus);

            if (!linkStatus)
            {
                // Create message buffer and fetch message.
                GLint infoLogLength;
                glGetProgramiv(variant.programId, GL_INFO_LOG_LENGTH, &infoLogLength);

                const char* errorMessage;
                if (infoLogLength > 0 && infoLogLength < 65536)
                {
                    char* temp = new char[infoLogLength + 1];
                    glGetProgramInfoLog(variant.programId, infoLogLength, nullptr, temp);
                    temp[infoLogLength] = '\0';
                    errorMessage = temp;
                }
                else
                {
                    errorMessage = "Unknown error, OpenGL context might not have been made current?";
                }

                log::error("Error linking invalid shader:\n\t{}", errorMessage);
                if (infoLogLength > 0 && infoLogLength < 65536)
                    delete[] errorMessage; // Delete message.

                for (auto& id : shaderIds)
                {
                    glDetachShader(variant.programId, id);
                    glDeleteShader(id);
                }

                glDeleteProgram(variant.programId);
                shader.m_variants.erase(variantId);
                continue;
            }

            glValidateProgram(variant.programId);

            GLint validationStatus;
            glGetProgramiv(variant.programId, GL_VALIDATE_STATUS, &validationStatus);

            if (!validationStatus)
            {
                // Create message buffer and fetch message.
                GLint infoLogLength;
                glGetProgramiv(variant.programId, GL_INFO_LOG_LENGTH, &infoLogLength);

                const char* errorMessage;
                if (infoLogLength > 0 && infoLogLength < 65536)
                {
                    char* temp = new char[infoLogLength + 1];
                    glGetProgramInfoLog(variant.programId, infoLogLength, nullptr, temp);
                    temp[infoLogLength] = '\0';
                    errorMessage = temp;
                }
                else
                {
                    errorMessage = "Unknown error, OpenGL context might not have been made current?";
                }

                log::error("Error validating invalid shader:\n\t{}", errorMessage);
                if (infoLogLength > 0 && infoLogLength < 65536)
                    delete[] errorMessage; // Delete message.

                for (auto& shaderId : shaderIds)
                {
                    glDetachShader(variant.programId, shaderId);
                    glDeleteShader(shaderId);
                }

                glDeleteProgram(variant.programId);

                shader.m_variants.erase(variantId);
            }
        }

        if (shader.m_variants.size() == 0)
            return invalid_shader_handle;

        process_io(shader, invalid_id);

        {
            async::readwrite_guard guard(m_shaderLock);
            m_shaders[invalid_id] = std::move(shader);
            m_shaders[invalid_id].configure_variant(0);
        }

        if (compiledFromScratch && settings.storePrecompiled)
            store_precompiled(file, shaders, state);

        return { invalid_id };
    }

    void ShaderCache::clear_modified_from_cache(const fs::view& path)
    {
        auto pathHash = nameHash(path.get_virtual_path());
        if (m_checkedPaths.count(pathHash))
            return;
        else
            m_checkedPaths.insert(pathHash);

        ShaderCompiler::cleanCache(path);
    }

    void ShaderCache::clear_checked_paths()
    {
        m_checkedPaths.clear();
    }

    void ShaderCache::reload_shaders()
    {
        std::vector<shader> toReload;

        {
            async::readonly_guard guard(m_shaderLock);

            for (auto [id, shaderProgram] : m_shaders)
            {
                if (id != invalid_id)
                    toReload.emplace_back(std::move(shaderProgram));
            }

            for (auto& item : toReload)
            {
                m_shaders.erase(item.m_id);
            }
        }

        glUseProgram(0);

        for (auto& item : toReload)
        {
            for (auto& [id, variant] : item.m_variants)
                glDeleteProgram(variant.programId);

            create_shader(item.name, fs::view(item.path), item.m_importSettings);
        }
    }

    void ShaderCache::delete_shader(const std::string& name)
    {
        log::debug("Destroyed shader {}", name);
        auto id = nameHash(name);

        async::readonly_guard guard(m_shaderLock);
        if (m_shaders.contains(id))
        {
            shader temp = std::move(m_shaders.at(id));
            for (auto& [id, variant] : temp.m_variants)
                glDeleteProgram(variant.programId);
            m_shaders.erase(id);
        }
    }

    void ShaderCache::delete_shader(id_type id)
    {
        log::debug("Destroyed shader with id: {}", id);
        async::readonly_guard guard(m_shaderLock);
        if (m_shaders.contains(id))
        {
            shader temp = std::move(m_shaders.at(id));
            for (auto& [id, variant] : temp.m_variants)
                glDeleteProgram(variant.programId);
            m_shaders.erase(id);
        }
    }

    bool ShaderCache::has_shader(const std::string& name)
    {
        return has_shader(nameHash(name));
    }

    bool ShaderCache::has_shader(id_type id)
    {
        async::readonly_guard guard(m_shaderLock);

        return m_shaders.contains(id);
    }

    shader_handle ShaderCache::create_shader(const std::string& name, const fs::view& file, shader_import_settings settings)
    {
        static id_type defaultId = nameHash("default");

        // Get the id of the new shader.
        id_type id = nameHash(name);

        { // Check if the shader already exists.
            async::readonly_guard guard(m_shaderLock);

            if (!m_shaders.contains(invalid_id) || (!m_shaders.at(invalid_id).has_variant(0)))
            {
                create_invalid_shader(fs::view("engine://shaders/invalid.shs"));
            }

            if (m_shaders.contains(id))
            {
                log::debug("Shader {} already exists, existing shader will be returned instead.", name);
                return { id };
            }
        }

        clear_modified_from_cache(file.parent());

        std::unordered_map<std::string, shader_state> state;
        shader_ilo shaders;

        auto result = file.get_extension();
        if (result != common::valid)
            return invalid_shader_handle;

        bool compiledFromScratch = false;

        if (result.value().empty() || result.value() == ".shil")
        {
            if (!load_precompiled(file, shaders, state))
                return invalid_shader_handle;
        }
        else
        {
            do
            {
                if (settings.usePrecompiledIfAvailable)
                {
                    auto precompiled = file / ".." / (file.get_filestem().value() + ".shil");

                    if (precompiled.is_valid(true))
                    {
                        auto traits = precompiled.file_info();
                        if (traits.is_file && traits.can_be_read)
                        {
                            if (load_precompiled(precompiled, shaders, state))
                                break;
                        }
                    }
                }

                byte compilerSettings = 0;
                compilerSettings |= settings.api;
                if (settings.debug)
                    compilerSettings |= shader_compiler_options::debug;
                if (settings.low_power)
                    compilerSettings |= shader_compiler_options::low_power;

                if (!ShaderCompiler::process(file, compilerSettings, shaders, state, detail::get_default_defines()))
                    return invalid_shader_handle;

                compiledFromScratch = true;

            } while (false);
        }

        if (shaders.empty())
            return invalid_shader_handle;

        shader shader;

        for (auto& [variant, variantState] : state)
        {
            shader.m_variants[nameHash(variant)].state = variantState;
        }

        for (auto& [shaderVariant, variantSource] : shaders)
        {
            id_type variantId = nameHash(shaderVariant);
            shader_variant& variant = shader.m_variants[variantId];
            variant.name = shaderVariant;

            GLenum blendSrc = GL_FALSE, blendDst = GL_FALSE;

            for (auto& [func, param] : variant.state)
            {
                switch (func)
                {
                case GL_DEPTH_TEST:
                {
                    if (param == GL_FALSE)
                    {
                        glDisable(func);
                        continue;
                    }

                    glEnable(func);
                    glDepthFunc(param);
                }
                break;
                case GL_DEPTH_WRITEMASK:
                {
                    glDepthMask(param);
                }
                break;
                case GL_CULL_FACE:
                {
                    if (param == GL_FALSE)
                    {
                        glDisable(func);
                        continue;
                    }

                    glEnable(func);
                    glCullFace(param);
                }
                break;
                case GL_BLEND:
                {
                    if (blendSrc == GL_FALSE)
                        blendSrc = param;
                    if (blendDst == GL_FALSE)
                        blendDst = param;
                }
                break;
                case GL_BLEND_SRC:
                {
                    if (blendSrc == GL_FALSE)
                        blendSrc = param;
                }
                break;
                case GL_BLEND_DST:
                {
                    if (blendDst == GL_FALSE)
                        blendDst = param;
                }
                break;
                case GL_DITHER:
                {
                    if (param == GL_TRUE)
                        glEnable(GL_DITHER);
                    else
                        glDisable(GL_DITHER);
                }
                break;
                default:
                    break;
                }
            }

            if (blendSrc == GL_FALSE || blendDst == GL_FALSE)
            {
                glDisable(GL_BLEND);
            }
            else
            {
                glEnable(GL_BLEND);
                glBlendFunc(blendSrc, blendDst);
            }

            variant.programId = glCreateProgram();

            std::vector<app::gl_id> shaderIds;
            for (auto& [shaderType, shaderIL] : variantSource)
            {
                auto shaderId = compile_shader(shaderType, shaderIL.c_str(), shaderIL.size());

                if (shaderId == (app::gl_id)-1)
                {
                    log::error("Error occurred in shader: {}", name);

                    for (auto id : shaderIds)
                    {
                        glDetachShader(variant.programId, id);
                        glDeleteShader(id);
                    }

                    glDeleteProgram(variant.programId);
                    shader.m_variants.erase(variantId);
                    continue;
                }

                glAttachShader(variant.programId, shaderId);
                shaderIds.push_back(shaderId);
            }

            glLinkProgram(variant.programId);

            GLint linkStatus;
            glGetProgramiv(variant.programId, GL_LINK_STATUS, &linkStatus);

            if (!linkStatus)
            {
                // Create message buffer and fetch message.
                GLint infoLogLength;
                glGetProgramiv(variant.programId, GL_INFO_LOG_LENGTH, &infoLogLength);
                const char* errorMessage;
                if (infoLogLength > 0 && infoLogLength < 65536)
                {
                    char* temp = new char[infoLogLength + 1];
                    glGetProgramInfoLog(variant.programId, infoLogLength, nullptr, temp);
                    temp[infoLogLength] = '\0';
                    errorMessage = temp;
                }
                else
                {
                    errorMessage = "Unknown error, OpenGL context might not have been made current?";
                }

                log::error("Error linking {} shader:\n\t{}", name, errorMessage);
                if (infoLogLength > 0 && infoLogLength < 65536)
                    delete[] errorMessage; // Delete message.

                for (auto& id : shaderIds)
                {
                    glDetachShader(variant.programId, id);
                    glDeleteShader(id);
                }

                glDeleteProgram(variant.programId);
                shader.m_variants.erase(variantId);
                continue;
            }

            glValidateProgram(variant.programId);

            GLint validationStatus;
            glGetProgramiv(variant.programId, GL_VALIDATE_STATUS, &validationStatus);

            if (!validationStatus)
            {
                // Create message buffer and fetch message.
                GLint infoLogLength;
                glGetProgramiv(variant.programId, GL_INFO_LOG_LENGTH, &infoLogLength);
                const char* errorMessage;
                if (infoLogLength > 0 && infoLogLength < 65536)
                {
                    char* temp = new char[infoLogLength + 1];
                    glGetProgramInfoLog(variant.programId, infoLogLength, nullptr, temp);
                    temp[infoLogLength] = '\0';
                    errorMessage = temp;
                }
                else
                {
                    errorMessage = "Unknown error, OpenGL context might not have been made current?";
                }

                log::error("Error validating {} shader:\n\t{}", name, errorMessage);
                if (infoLogLength > 0 && infoLogLength < 65536)
                    delete[] errorMessage; // Delete message.

                for (auto& shaderId : shaderIds)
                {
                    glDetachShader(variant.programId, shaderId);
                    glDeleteShader(shaderId);
                }

                glDeleteProgram(variant.programId);

                shader.m_variants.erase(variantId);
                continue;
            }

            for (auto& shaderId : shaderIds)
            {
                glDetachShader(variant.programId, shaderId);
                glDeleteShader(shaderId);
            }
        }

        if (shader.m_variants.empty() || !shader.m_variants.count(defaultId))
        {
            shader.name = name;
            shader.path = file.get_virtual_path();
            shader.m_importSettings = settings;
            shader.m_id = id;

            {
                async::readwrite_guard guard(m_shaderLock);
                m_shaders.insert(id, std::move(shader));
            }

            return { id };
        }

        process_io(shader, id);

        shader.name = name;
        shader.path = file.get_virtual_path();
        shader.m_importSettings = settings;
        shader.m_id = id;

        {
            async::readwrite_guard guard(m_shaderLock);
            m_shaders.insert(id, std::move(shader));
            m_shaders[id].configure_variant(0);
        }

        if (compiledFromScratch && settings.storePrecompiled)
            store_precompiled(file, shaders, state);

        return { id };
    }

    shader_handle ShaderCache::create_shader(const fs::view& file, shader_import_settings settings)
    {
        return create_shader(*file.get_filename(), file, settings);
    }

    shader_handle ShaderCache::get_handle(const std::string& name)
    {
        id_type id = nameHash(name);

        async::readonly_guard guard(m_shaderLock);

        if (!m_shaders.contains(invalid_id) || (!m_shaders[invalid_id].has_variant(0)))
        {
            create_invalid_shader(fs::view("engine://shaders/invalid.shs"));
        }

        if (!m_shaders.contains(id))
            return invalid_shader_handle;
        else
            return { id };
    }

    shader_handle ShaderCache::get_handle(id_type id)
    {
        async::readonly_guard guard(m_shaderLock);

        if (!m_shaders.contains(invalid_id) || (!m_shaders[invalid_id].has_variant(0)))
        {
            create_invalid_shader(fs::view("engine://shaders/invalid.shs"));
        }

        if (!m_shaders.contains(id))
            return invalid_shader_handle;
        else
            return { id };
    }

    std::vector<shader_handle> ShaderCache::get_all()
    {
        std::vector<shader_handle> shaders;
        async::readonly_guard guard(m_shaderLock);
        shaders.reserve(m_shaders.size());

        for (auto [id, shader] : m_shaders)
            shaders.push_back(shader_handle{ id });

        return shaders;
    }

    shader_variant& shader_handle::get_variant(id_type variantId)
    {
        return ShaderCache::get_shader(id)->get_variant(variantId);
    }

    shader_variant& shader_handle::get_variant(const std::string& variant)
    {
        return ShaderCache::get_shader(id)->get_variant(variant);
    }

    const shader_variant& shader_handle::get_variant(id_type variantId) const
    {
        return ShaderCache::get_shader(id)->get_variant(variantId);
    }

    const shader_variant& shader_handle::get_variant(const std::string& variant) const
    {
        return ShaderCache::get_shader(id)->get_variant(variant);
    }

    GLuint shader_handle::get_uniform_block_index(const std::string& name) const
    {
        return ShaderCache::get_shader(id)->get_uniform_block_index(name);
    }

    void shader_handle::bind_uniform_block(GLuint uniformBlockIndex, GLuint uniformBlockBinding) const
    {
        ShaderCache::get_shader(id)->bind_uniform_block(uniformBlockIndex, uniformBlockBinding);
    }

    std::string shader_handle::get_name() const
    {
        return ShaderCache::get_shader(id)->name;
    }

    std::string shader_handle::get_path() const
    {
        return ShaderCache::get_shader(id)->path;
    }

    std::unordered_map<id_type, std::vector<std::tuple<std::string, GLint, GLenum>>> shader_handle::get_uniform_info() const
    {
        auto* shader = ShaderCache::get_shader(id);
        std::unordered_map<id_type, std::vector<std::tuple<std::string, GLint, GLenum>>> info;

        for (auto& [variantId, variant] : shader->m_variants)
        {
            info[variantId] = variant.get_uniform_info();
        }

        return info;
    }

    std::vector<std::tuple<std::string, GLint, GLenum>> shader_handle::get_uniform_info(id_type variantId) const
    {
        return ShaderCache::get_shader(id)->get_variant(variantId).get_uniform_info();
    }

    std::vector<std::tuple<std::string, GLint, GLenum>> shader_handle::get_uniform_info(const std::string& variant) const
    {
        return ShaderCache::get_shader(id)->get_variant(nameHash(variant)).get_uniform_info();
    }

    attribute shader_handle::get_attribute(const std::string& name)
    {
        return ShaderCache::get_shader(id)->get_attribute(name);
    }

    attribute shader_handle::get_attribute(id_type attributeId)
    {
        return ShaderCache::get_shader(id)->get_attribute(attributeId);
    }

    void shader_handle::destroy()
    {
        ShaderCache::delete_shader(id);
    }

    bool shader_handle::is_valid() const
    {
        return ShaderCache::has_shader(id);
    }

    void shader_handle::bind() const
    {
        ShaderCache::get_shader(id)->bind();
    }

    void shader_handle::release()
    {
        glUseProgram(0);
    }

    bool shader::has_variant(id_type variantId) const
    {
        if (variantId == 0)
        {
            static id_type defaultId = nameHash("default");
            return m_variants.count(defaultId);
        }
        return m_variants.count(variantId);
    }

    bool shader::has_variant(const std::string& variant) const
    {
        std::string variantName = variant;
        std::replace(variantName.begin(), variantName.end(), ' ', '_');
        return m_variants.count(nameHash(variantName));
    }

    void shader::configure_variant(id_type variantId) const
    {
        if (variantId == 0)
        {
            static id_type defaultId = nameHash("default");
            m_currentShaderVariant = &m_variants.at(defaultId);
        }
        else if (m_variants.count(variantId))
            m_currentShaderVariant = &m_variants.at(variantId);
    }

    void shader::configure_variant(const std::string& variant) const
    {
        std::string variantName = variant;
        std::replace(variantName.begin(), variantName.end(), ' ', '_');
        id_type variantId = nameHash(variantName);
        if (m_variants.count(variantId))
            m_currentShaderVariant = &m_variants.at(variantId);
    }

    shader_variant& shader::get_variant(id_type variantId)
    {
        if (variantId == 0)
        {
            static id_type defaultId = nameHash("default");

#if defined(LEGION_VALIDATE)
            if (!m_variants.count(defaultId))
                return const_cast<shader_variant&>(invalid_shader_handle.get_variant(0));
#endif
            return m_variants.at(defaultId);
        }

#if defined(LEGION_VALIDATE)
        if (!m_variants.count(variantId))
            return const_cast<shader_variant&>(invalid_shader_handle.get_variant(0));
#endif
        return m_variants.at(variantId);
    }

    shader_variant& shader::get_variant(const std::string& variant)
    {
        return m_variants.at(nameHash(variant));
    }

    const shader_variant& shader::get_variant(id_type variantId) const
    {
#if defined(LEGION_VALIDATE)
        if (!m_variants.count(variantId))
            return invalid_shader_handle.get_variant(0);
#endif
        if (variantId == 0)
        {
            static id_type defaultId = nameHash("default");
            return m_variants.at(defaultId);
        }
        return m_variants.at(variantId);
    }

    const shader_variant& shader::get_variant(const std::string& variant) const
    {
        return m_variants.at(nameHash(variant));
    }

    void shader::bind()
    {
        if (!m_currentShaderVariant)
        {
            log::warn("No current shader variant configured for shader {}", name);
            configure_variant(0);
        }

        GLenum blendSrc = GL_FALSE, blendDst = GL_FALSE;

        for (auto& [func, param] : m_currentShaderVariant->state)
        {
            switch (func)
            {
            case GL_DEPTH_TEST:
            {
                if (param == GL_FALSE)
                {
                    glDisable(func);
                    continue;
                }

                glEnable(func);
                glDepthFunc(param);
            }
            break;
            case GL_DEPTH_WRITEMASK:
            {
                glDepthMask(param);
            }
            break;
            case GL_CULL_FACE:
            {
                if (param == GL_FALSE)
                {
                    glDisable(func);
                    continue;
                }

                glEnable(func);
                glCullFace(param);
            }
            break;
            case GL_BLEND:
            {
                if (blendSrc == GL_FALSE)
                    blendSrc = param;
                if (blendDst == GL_FALSE)
                    blendDst = param;
            }
            break;
            case GL_BLEND_SRC:
            {
                if (blendSrc == GL_FALSE)
                    blendSrc = param;
            }
            break;
            case GL_BLEND_DST:
            {
                if (blendDst == GL_FALSE)
                    blendDst = param;
            }
            break;
            case GL_DITHER:
            {
                if (param == GL_TRUE)
                    glEnable(GL_DITHER);
                else
                    glDisable(GL_DITHER);
            }
            break;
            default:
                break;
            }
        }

        if (blendSrc == GL_FALSE || blendDst == GL_FALSE)
        {
            glDisable(GL_BLEND);
        }
        else
        {
            glEnable(GL_BLEND);
            glBlendFunc(blendSrc, blendDst);
        }

        glUseProgram(m_currentShaderVariant->programId);
    }

    void shader::release()
    {
        glUseProgram(0);
    }

    GLuint shader::get_uniform_block_index(const std::string& name) const
    {
        if (!m_currentShaderVariant)
        {
            log::warn("No current shader variant configured for shader {}", name);
            configure_variant(0);
        }

        return glGetUniformBlockIndex(m_currentShaderVariant->programId, name.c_str());
    }

    void shader::bind_uniform_block(GLuint uniformBlockIndex, GLuint uniformBlockBinding) const
    {
        if (!m_currentShaderVariant)
        {
            log::warn("No current shader variant configured for shader {}", name);
            configure_variant(0);
        }

        glUniformBlockBinding(m_currentShaderVariant->programId, uniformBlockIndex, uniformBlockBinding);
    }

    attribute shader::get_attribute(const std::string& name)
    {
        if (!m_currentShaderVariant)
        {
            log::error("No current shader variant configured for shader {}", name);
            return invalid_attribute;
        }

        id_type id = nameHash(name);
        if (m_currentShaderVariant->attributes.count(id))
            return *(m_currentShaderVariant->attributes[id].get());

        log::error("Shader {} does not contain attribute {}", this->name, name);
        return invalid_attribute;
    }

    attribute shader::get_attribute(id_type id)
    {
        if (!m_currentShaderVariant)
        {
            log::error("No current shader variant configured for shader {}", name);
            return invalid_attribute;
        }

        if (m_currentShaderVariant->attributes.count(id))
            return *(m_currentShaderVariant->attributes[id].get());
        log::error("Shader {} does not contain attribute with id {}", this->name, id);
        return invalid_attribute;
    }

    std::vector<std::tuple<std::string, GLint, GLenum>> shader_variant::get_uniform_info()
    {
        std::vector<std::tuple<std::string, GLint, GLenum>> info;
        for (auto& [_, uniform] : uniforms)
            info.push_back(std::make_tuple(uniform->get_name(), uniform->get_location(), uniform->get_type()));
        return info;
    }

    bool shader_handle::has_variant(id_type variantId) const
    {
        return ShaderCache::get_shader(id)->has_variant(variantId);
    }

    bool shader_handle::has_variant(const std::string& variant) const
    {
        return ShaderCache::get_shader(id)->has_variant(variant);
    }

    void shader_handle::configure_variant(id_type variantId)
    {
        ShaderCache::get_shader(id)->configure_variant(variantId);
    }

    void shader_handle::configure_variant(const std::string& variant)
    {
        ShaderCache::get_shader(id)->configure_variant(variant);
    }

}
