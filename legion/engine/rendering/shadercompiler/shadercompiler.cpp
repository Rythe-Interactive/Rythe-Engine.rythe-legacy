#include <lgnspre/gl_consts.hpp>

#include <rendering/shadercompiler/shadercompiler.hpp>
#include <rendering/util/settings.hpp>
#include <application/application.hpp>

namespace legion::rendering
{
    delegate<void(const std::string&, log::severity)> ShaderCompiler::m_callback;


    std::string ShaderCompiler::get_view_path(const fs::view& view, bool mustBeFile)
    {
        using severity = log::severity;

        fs::navigator navigator(view.get_virtual_path());
        auto solution = navigator.find_solution();
        if (solution.has_error())
        {
            m_callback(std::string("Shader processor error: ") + solution.error().what(), severity::error);
            return "";
        }

        auto s = solution.value();
        if (s.size() != 1)
        {
            m_callback("Shader processor error: invalid file, fs::view was not fully local", severity::error);
            return "";
        }

        fs::basic_resolver* resolver = dynamic_cast<fs::basic_resolver*>(s[0].first);
        if (!resolver)
        {
            m_callback("Shader processor error: invalid file, fs::view was not local", severity::error);
            return "";
        }

        resolver->set_target(s[0].second);

        if (!resolver->is_valid_path())
        {
            m_callback("Shader processor error: invalid path", severity::error);
            return "";
        }

        if (mustBeFile && !resolver->is_file())
        {
            m_callback("Shader processor error: not a file", severity::error);
            return "";
        }

        return resolver->get_absolute_path();
    }

    const std::string& ShaderCompiler::get_shaderlib_path()
    {
        static std::string libPath = get_view_path(fs::view("engine://shaderlib"), false);
        return libPath;
    }

    const std::string& ShaderCompiler::get_compiler_path()
    {
        static std::string compPath;
        if (compPath.empty())
            compPath = get_view_path(fs::view("engine://tools"), false) + fs::strpath_manip::separator() + "lgnspre" + fs::strpath_manip::separator() + "lgnspre";
        return compPath;
    }

    const std::string& ShaderCompiler::get_cachecleaner_path()
    {
        static std::string compPath;
        if (compPath.empty())
            compPath = get_view_path(fs::view("engine://tools"), false) + fs::strpath_manip::separator() + "lgnspre" + fs::strpath_manip::separator() + "lgncleancache";
        return compPath;
    }

    void ShaderCompiler::extract_state(std::string_view source, shader_state& state)
    {
        std::string_view rest = source;
        std::vector<std::pair<std::string, std::string>> stateInput;
        while (!rest.empty())
        {
            auto seperator = rest.find_first_not_of('\n');
            seperator = rest.find_first_of('\n', seperator);

            if (seperator == std::string::npos)
                seperator = rest.size();

            auto line = rest.substr(0, seperator);
            auto space = line.find(' ');
            if (space != std::string::npos)
            {
                auto param = line.substr(space + 1);
                auto paramSpace = param.find(' ');
                stateInput.push_back(std::make_pair(std::string(line.substr(0, space)), std::string(param.substr(0, paramSpace))));
            }
            else
                stateInput.push_back(std::make_pair(std::string(line.substr(0, space)), std::string()));

            rest = rest.substr(seperator);
            seperator = rest.find_first_not_of('\n');
            if (seperator == std::string::npos)
                break;
            else
                rest = rest.substr(seperator);
        }

        // Create lookup table for the OpenGL function types that can be changed by the shader state.
        static std::unordered_map<std::string, GLenum> funcTypes;
        static bool funcTypesInitialized = false;
        if (!funcTypesInitialized)
        {
            funcTypesInitialized = true;
            funcTypes["DEPTH"] = GL_DEPTH_TEST;
            funcTypes["DEPTH_WRITE"] = GL_DEPTH_WRITEMASK;
            funcTypes["CULL"] = GL_CULL_FACE;
            funcTypes["ALPHA_SOURCE"] = GL_BLEND_SRC;
            funcTypes["ALPHA_DEST"] = GL_BLEND_DST;
            funcTypes["ALPHA"] = GL_BLEND;
            funcTypes["BLEND_SOURCE"] = GL_BLEND_SRC;
            funcTypes["BLEND_DEST"] = GL_BLEND_DST;
            funcTypes["BLEND"] = GL_BLEND;
            funcTypes["DITHER"] = GL_DITHER;
        }

        for (auto& [func, par] : stateInput)
        {
            if (!funcTypes.count(func)) // If the function type is unsupported or unknown we also want to fail compilation.
                continue;

            GLenum funcType = funcTypes.at(func); // Fetch the function type without risking editing the lookup table.
            GLenum param = GL_FALSE;

            switch (funcType)
            {
            case GL_DEPTH_TEST:
            {
                static std::unordered_map<std::string, GLenum> params; // Initialize parameter lookup table.
                static bool initialized = false;
                if (!initialized)
                {
                    initialized = true;
                    params["OFF"] = GL_FALSE;
                    params["NEVER"] = GL_NEVER;
                    params["LESS"] = GL_LESS;
                    params["EQUAL"] = GL_EQUAL;
                    params["LEQUAL"] = GL_LEQUAL;
                    params["GREATER"] = GL_GREATER;
                    params["NOTEQUAL"] = GL_NOTEQUAL;
                    params["GEQUAL"] = GL_GEQUAL;
                    params["ALWAYS"] = GL_ALWAYS;
                }

                if (!params.count(par))
                    continue;

                param = params.at(par);
            }
            break;
            case GL_DEPTH_WRITEMASK:
            {
                static std::unordered_map<std::string, GLenum> params; // Initialize parameter lookup table.
                static bool initialized = false;
                if (!initialized)
                {
                    initialized = true;
                    params["OFF"] = GL_FALSE;
                    params["ON"] = GL_TRUE;
                }

                if (!params.count(par))
                    continue;

                param = params.at(par);
            }
            break;
            case GL_CULL_FACE:
            {
                static std::unordered_map<std::string, GLenum> params; // Initialize parameter lookup table.
                static bool initialized = false;
                if (!initialized)
                {
                    initialized = true;
                    params["FRONT"] = GL_FRONT;
                    params["BACK"] = GL_BACK;
                    params["FRONT_AND_BACK"] = GL_FRONT_AND_BACK;
                    params["OFF"] = GL_FALSE;
                }

                if (!params.count(par))
                    continue;

                param = params.at(par);
            }
            break;
            case GL_BLEND:
            case GL_BLEND_SRC:
            case GL_BLEND_DST:
            {
                static std::unordered_map<std::string, GLenum> params; // Initialize parameter lookup table.
                static bool initialized = false;
                if (!initialized)
                {
                    initialized = true;
                    params["ZERO"] = GL_ZERO;
                    params["ONE"] = GL_ONE;
                    params["SRC_COLOR"] = GL_SRC_COLOR;
                    params["ONE_MINUS_SRC_COLOR"] = GL_ONE_MINUS_SRC_COLOR;
                    params["DST_COLOR"] = GL_DST_COLOR;
                    params["ONE_MINUS_DST_COLOR"] = GL_ONE_MINUS_DST_COLOR;
                    params["SRC_ALPHA"] = GL_SRC_ALPHA;
                    params["ONE_MINUS_SRC_ALPHA"] = GL_ONE_MINUS_SRC_ALPHA;
                    params["DST_ALPHA"] = GL_DST_ALPHA;
                    params["ONE_MINUS_DST_ALPHA"] = GL_ONE_MINUS_DST_ALPHA;
                    params["CONSTANT_COLOR"] = GL_CONSTANT_COLOR;
                    params["ONE_MINUS_CONSTANT_COLOR"] = GL_ONE_MINUS_CONSTANT_COLOR;
                    params["CONSTANT_ALPHA"] = GL_CONSTANT_ALPHA;
                    params["ONE_MINUS_CONSTANT_ALPHA"] = GL_ONE_MINUS_CONSTANT_ALPHA;
                    params["SRC_ALPHA_SATURATE"] = GL_SRC_ALPHA_SATURATE;
                    params["OFF"] = GL_FALSE;
                    params["OPAQUE"] = GL_FALSE;
                    params["TRANSPARENT"] = GL_INTERNALFORMAT_PREFERRED;
                }

                if (!params.count(par))
                    continue;

                param = params.at(par);

                if (param == GL_INTERNALFORMAT_PREFERRED)
                {
                    state[GL_BLEND_SRC] = GL_SRC_ALPHA;
                    state[GL_BLEND_DST] = GL_ONE_MINUS_SRC_ALPHA;
                    continue;
                }
            }
            break;
            case GL_DITHER:
            {
                static std::unordered_map<std::string, GLenum> params; // Initialize parameter lookup table.
                static bool initialized = false;
                if (!initialized)
                {
                    initialized = true;
                    params["OFF"] = GL_FALSE;
                    params["ON"] = GL_TRUE;
                    params["FALSE"] = GL_FALSE;
                    params["TRUE"] = GL_TRUE;
                }

                if (!params.count(par))
                    continue;

                param = params.at(par);
            }
            break;
            default:
                param = GL_FALSE;
                break;
            }

            state[funcType] = param;
        }
    }

    bool ShaderCompiler::extract_ilo(const std::string& variant, std::string_view source, uint64 shaderType, shader_ilo& ilo)
    {
        using severity = log::severity;

        GLuint glShaderType = detail::get_gl_type(shaderType);
        auto lgnShaderType = detail::get_lgn_type(shaderType);

        if (shaderType != GL_LGN_VERTEX_SHADER &&
            shaderType != GL_LGN_FRAGMENT_SHADER &&
            shaderType != GL_LGN_GEOMETRY_SHADER &&
            shaderType != GL_LGN_TESS_EVALUATION_SHADER &&
            shaderType != GL_LGN_TESS_CONTROL_SHADER)
        {
            switch (lgnShaderType)
            {
            case LGN_VERTEX_SHADER:
                glShaderType = GL_VERTEX_SHADER;
                break;
            case LGN_FRAGMENT_SHADER:
                glShaderType = GL_FRAGMENT_SHADER;
                break;
            case LGN_GEOMETRY_SHADER:
                glShaderType = GL_GEOMETRY_SHADER;
                break;
            case LGN_TESS_EVALUATION_SHADER:
                glShaderType = GL_TESS_EVALUATION_SHADER;
                break;
            case LGN_TESS_CONTROL_SHADER:
                glShaderType = GL_TESS_CONTROL_SHADER;
                break;
            default:
                m_callback("Shader processor error: unkown shader type", severity::error);
                return false;
                break;
            }
        }

        auto versionIdx = source.find("#version");

        if (versionIdx == std::string::npos)
        {
            m_callback("Shader processor error: no shader version", severity::error);
            return false;
        }

        auto versionEnd = source.find_first_of('\n', versionIdx) + 1;

        auto start = source.substr(0, versionEnd);
        auto rest = source.substr(versionEnd);

        ilo[variant].emplace_back(glShaderType, std::string(start) + "#extension GL_ARB_gpu_shader_int64 : enable\n" + std::string(rest));
        return true;
    }

    std::string ShaderCompiler::invoke_compiler(const fs::view& file, bitfield8 compilerSettings, const std::vector<std::string>& defines, const std::vector<std::string>& additionalIncludes)
    {
        using severity = log::severity;

        std::string filepath = get_view_path(file, true);
        if (filepath.empty())
            return "";

        auto folderEnd = filepath.find_last_of("\\/");
        std::string folderPath(filepath.c_str(), folderEnd);

        std::string definesString = " -D LEGION";
        if (compilerSettings & shader_compiler_options::debug)
            definesString += " -D DEBUG";
        else
            definesString += " -D RELEASE";

        if (compilerSettings & shader_compiler_options::low_power)
            definesString += " -D LOW_POWER";
        else
            definesString += " -D HIGH_PERFORMANCE";

        bitfield8 apiSet = 0;
        if (compilerSettings & shader_compiler_options::api_opengl)
        {
            apiSet |= rendering_api::opengl;
            definesString += " -D API_OPENGL";
        }

        if (compilerSettings & shader_compiler_options::api_vulkan)
        {
            if (apiSet)
            {
                if (apiSet & rendering_api::opengl)
                    m_callback("Shader processor warning: both OpenGL and Vulkan were set as graphics api, OpenGL is assumed", severity::warn);
            }
            else
            {
                apiSet |= rendering_api::vulkan;
                definesString += " -D API_VULKAN";
            }
        }

        if (!apiSet)
        {
            m_callback("Shader processor warning: no api was set, OpenGL is assumed", severity::warn);
            definesString += " -D API_UNKNOWN -D API_OPENGL";
        }

        for (auto def : defines)
        {
            definesString += " -D " + def;
        }

        std::string includeString = " -I \"" + folderPath + "\" -I \"" + get_shaderlib_path() + "\"";

        for (auto incl : additionalIncludes)
        {
            includeString += " -I \"" + incl + "\"";
        }

        std::string command = "\"" + get_compiler_path() + "\" \"" + filepath + "\"" + definesString + includeString + " -f 1file -o stdout";

        std::string out, err;

        if (!ShellInvoke(command, out, err))
        {
            m_callback("Shader processor error: " + err, severity::error);
            return "";
        }

        if (!err.empty())
            m_callback("Shader processor warning: " + err, severity::warn);

        out.erase(std::remove(out.begin(), out.end(), '\r'), out.end());

        return out;
    }

    void ShaderCompiler::cleanCache(const fs::view& path)
    {
        using severity = log::severity;
        std::string out, err;

        std::string command = "\"" + get_cachecleaner_path() + "\" \"" + get_view_path(path, false) + "\" -I \"" + get_shaderlib_path() + "\" ./ --filter=shil";

        if (!ShellInvoke(command, out, err))
        {
            m_callback("Shader processor error: " + err, severity::error);
        }
    }

    bool ShaderCompiler::process(const fs::view& file, bitfield8 compilerSettings, shader_ilo& ilo, std::unordered_map<std::string, shader_state>& state)
    {
        std::vector<std::string> temp;
        return process(file, compilerSettings, ilo, state, temp, temp);
    }

    bool ShaderCompiler::process(const fs::view& file, bitfield8 compilerSettings, shader_ilo& ilo, std::unordered_map<std::string, shader_state>& state, const std::vector<std::string>& defines)
    {
        std::vector<std::string> temp;
        return process(file, compilerSettings, ilo, state, defines, temp);
    }

    bool ShaderCompiler::process(const fs::view& file, bitfield8 compilerSettings, shader_ilo& ilo, std::unordered_map<std::string, shader_state>& state, const std::vector<std::string>& defines, const std::vector<std::string>& additionalIncludes)
    {
        using severity = log::severity;

        log::info("Compiling shader: {}", file.get_virtual_path());

        auto result = invoke_compiler(file, compilerSettings, defines, additionalIncludes);

        if (result.empty())
            return false;

        auto start = result.find("=========== BEGIN SHADER CODE ===========\n") + 42;
        start = result.find_first_not_of('\n', start);
        auto end = result.find("============ END SHADER CODE ============");

        auto rest = std::string_view(result.data() + start, end - start);

        while (!rest.empty())
        {
            auto seperator = rest.find_first_not_of('\n');
            if (seperator == std::string::npos)
                break;

            seperator = rest.find_first_of('\n', seperator) + 1;
            if (rest.empty())
                break;
            auto shaderType = std::stoull(std::string(rest.substr(0, seperator)));
            rest = rest.substr(seperator);
            if (rest.empty())
                break;

            seperator = rest.find_first_of('\n') + 1;
            auto sourceLength = std::stoull(std::string(rest.substr(0, seperator)));
            if (sourceLength == 0)
                continue;

            rest = rest.substr(seperator);
            if (rest.empty())
                break;

            seperator = rest.substr(0, sourceLength).find_last_of('\n');
            auto source = rest.substr(0, seperator);            
            rest = rest.substr(seperator);

            seperator = source.find_first_of('\n');
            std::string variant(source.substr(0, seperator));
            source = source.substr(seperator+1);

            if (shaderType == 0)
            {
                // Default shader state in case nothing was specified by the shader.
                shader_state& currentVariantState = state[variant];
                currentVariantState[GL_DEPTH_TEST] = GL_LEQUAL;
                currentVariantState[GL_DEPTH_WRITEMASK] = GL_TRUE;
                currentVariantState[GL_CULL_FACE] = GL_BACK;
                currentVariantState[GL_BLEND] = GL_FALSE;
                currentVariantState[GL_DITHER] = GL_FALSE;
                currentVariantState[GL_DEPTH_WRITEMASK] = GL_TRUE;
                extract_state(source, currentVariantState);
            }
            else if (!extract_ilo(variant, source, shaderType, ilo))
            {
                return false;
            }
            if (rest.empty())
                break;

            seperator = rest.find_first_not_of('\n');
            if (seperator == std::string::npos)
                break;
        }

        if (ilo.empty())
        {
            m_callback("Shader processor error: shader was empty", severity::error);
            return false;
        }
        return true;
    }
}
