#include <rendering/data/texture.hpp>

namespace legion::rendering
{
    void texture::to_resource(fs::basic_resource* resource, const texture& value)
    {
        resource->clear();
        appendBinaryData(&value.textureId, resource->get());
        appendBinaryData(&value.channels, resource->get());
        appendBinaryData(&value.type, resource->get());
    }

    void texture::from_resource(texture* value, const fs::basic_resource& resource)
    {
        byte_vec::const_iterator start = resource.begin();
        retrieveBinaryData(value->textureId, start);
        retrieveBinaryData(value->channels, start);
        retrieveBinaryData(value->type, start);
    }

    math::ivec2 texture::size() const
    {
        math::ivec2 texSize;
        glBindTexture(static_cast<GLenum>(type), textureId);
        glGetTexLevelParameteriv(static_cast<GLenum>(type), 0, GL_TEXTURE_WIDTH, &texSize.x);
        glGetTexLevelParameteriv(static_cast<GLenum>(type), 0, GL_TEXTURE_HEIGHT, &texSize.y);
        glBindTexture(static_cast<GLenum>(type), 0);
        return texSize;
    }

    void texture::resize(math::ivec2 newSize) const
    {
        if (immutable)
        {
            log::error("Can't resize immutable texture {}", name);
            return;
        }

        glBindTexture(static_cast<GLenum>(type), textureId);
        glTexImage2D(
            static_cast<GLenum>(type),
            0,
            static_cast<GLint>(format),
            newSize.x,
            newSize.y,
            0,
            components_to_format[static_cast<int>(channels)],
            channels_to_glenum[static_cast<uint>(fileFormat)],
            NULL);
        glBindTexture(static_cast<GLenum>(type), 0);
    }

    sparse_map<id_type, texture> TextureCache::m_textures;
    async::rw_spinlock TextureCache::m_textureLock;
    texture_handle TextureCache::m_invalidTexture;

    texture_data texture_handle::get_data() const
    {
        return TextureCache::get_data(id);
    }

    const texture& texture_handle::get_texture() const
    {
        return TextureCache::get_texture(id);
    }

    const texture& TextureCache::get_texture(id_type id)
    {
        if (m_invalidTexture.id == invalid_id)
            m_invalidTexture = create_texture("invalid texture", fs::view("engine://resources/invalid/missing"));

        async::readonly_guard guard(m_textureLock);
        if (id == invalid_id)
            return m_textures[nameHash("invalid texture")];
        return m_textures[id];
    }

    texture_data TextureCache::get_data(id_type id)
    {
        texture texture;
        if (m_invalidTexture.id == invalid_id)
            m_invalidTexture = create_texture("invalid texture", fs::view("engine://resources/invalid/missing"));

        {
            async::readonly_guard guard(m_textureLock);
            if (id == invalid_id)
                texture = m_textures[nameHash("invalid texture")];
            else
                texture = m_textures[id];
        }
        texture_data data{};
        math::ivec2 texSize = texture.size();
        data.size.x = texSize.x;
        data.size.y = texSize.y;
        data.type = texture.type;
        data.pixels.resize(data.size.x * data.size.y);
        glBindTexture(static_cast<GLenum>(data.type), texture.textureId);
        glGetTexImage(static_cast<GLenum>(data.type), 0, components_to_format[static_cast<int>(texture.channels)], GL_FLOAT, data.pixels.data());
        glBindTexture(static_cast<GLenum>(data.type), 0);
        return data;
    }

    void TextureCache::destroy_texture(const std::string& name)
    {
        id_type id = nameHash(name);
        async::readonly_guard guard(m_textureLock);
        m_textures.erase(id);
    }

    texture_handle TextureCache::create_texture_array(const std::string& name, const std::vector<fs::view>& files, texture_import_settings settings)
    {
        std::vector<assets::asset<image>> loadedImgs;
        for (auto& file : files)
            loadedImgs.push_back(assets::load<image>(file).value());

        auto result = create_texture_array(name, loadedImgs, settings);

        for (auto& img : loadedImgs)
            img.destroy();

        return result;
    }

    texture_handle TextureCache::create_texture_array(const std::string& name, const std::vector<assets::asset<image>>& imgs, texture_import_settings settings)
    {
        if (m_invalidTexture.id == invalid_id)
            m_invalidTexture = create_texture("invalid texture", fs::view("engine://resources/invalid/missing"));

        id_type id = nameHash(name);
        {
            async::readonly_guard guard(m_textureLock);
            if (m_textures.contains(id))
                return { id };
        }

        if (!settings.immutable)
        {
            log::error("We do not support mutable texture arrays.");
            return invalid_texture_handle;
        }

        texture texture{};
        texture.type = settings.type;

        auto glTexType = static_cast<GLenum>(settings.type);

        // Allocate and bind the texture.
        glGenTextures(1, &texture.textureId);
        glBindTexture(glTexType, texture.textureId);

        glTexParameteri(glTexType, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(settings.min));
        glTexParameteri(glTexType, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(settings.mag));
        glTexParameteri(glTexType, GL_TEXTURE_BASE_LEVEL, 0);

        // Handle wrapping behavior.
        glTexParameteri(glTexType, GL_TEXTURE_WRAP_R, static_cast<GLint>(settings.wrapR));
        glTexParameteri(glTexType, GL_TEXTURE_WRAP_S, static_cast<GLint>(settings.wrapS));
        glTexParameteri(glTexType, GL_TEXTURE_WRAP_T, static_cast<GLint>(settings.wrapT));

        texture.channels = imgs[0]->components();
        texture.fileFormat = imgs[0]->format();
        texture.name = name;

        glTexStorage3D(glTexType,
            1,
            static_cast<GLint>(settings.intendedFormat),
            imgs[0]->resolution().x, imgs[0]->resolution().y, imgs.size());

        for (size_type idx = 0; idx < imgs.size(); idx++)
        {
            auto res = imgs[idx]->resolution();
            glTexSubImage3D(glTexType,
                0,
                0, 0, idx,
                res.x, res.y, 1,
                components_to_format[static_cast<int>(texture.channels)],
                channels_to_glenum[static_cast<uint>(texture.fileFormat)],
                imgs[idx]->data());
        }

        glBindTexture(glTexType, 0);

        log::debug("Created texture array from texture collection {}", texture.name);

        {
            async::readwrite_guard guard(m_textureLock);
            m_textures.insert(id, std::move(texture));
            m_textures.at(id).name = name;
        }

        return { id };
    }

    texture_handle TextureCache::create_texture(const std::string& name, const fs::view& file, texture_import_settings settings)
    {
        if (m_invalidTexture.id == invalid_id)
        {
            m_invalidTexture.id = 1;
            m_invalidTexture = create_texture("invalid texture", fs::view("engine://resources/invalid/missing"));
        }

        id_type id = nameHash(name);

        {
            async::readonly_guard guard(m_textureLock);
            if (m_textures.contains(id))
                return { id };
        }

        if (!file.is_valid() || !file.file_info().is_file)
            return invalid_texture_handle;

        auto result = fs::AssetImporter::tryLoad<texture>(file, settings);

        if (result != common::valid)
            return invalid_texture_handle;

        {
            texture t = result.value();
            t.path = file.get_virtual_path();
            async::readwrite_guard guard(m_textureLock);
            m_textures.insert(id, t);
            m_textures.at(id).name = name;
        }
        log::debug("Created texture {} with file: {}", name, file.get_filename().value());

        return { id };
    }

    texture_handle TextureCache::create_texture(const fs::view& file, texture_import_settings settings)
    {
        return create_texture(*file.get_filename(), file, settings);
    }

    texture_handle TextureCache::create_texture(const std::string& name, math::ivec2 size, texture_import_settings settings)
    {
        id_type id = nameHash(name);
        {
            async::readonly_guard guard(m_textureLock);
            if (m_textures.contains(id))
                return { id };
        }

        texture texture{};
        texture.type = settings.type;
        texture.name = name;

        auto glTexType = static_cast<GLenum>(settings.type);

        // Allocate and bind the texture.
        glGenTextures(1, &texture.textureId);
        glBindTexture(glTexType, texture.textureId);

        // Handle mips
        glTexParameteri(glTexType, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(settings.min));
        glTexParameteri(glTexType, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(settings.mag));
        glTexParameteri(glTexType, GL_TEXTURE_BASE_LEVEL, 0);

        // Handle wrapping behavior.
        glTexParameteri(glTexType, GL_TEXTURE_WRAP_R, static_cast<GLint>(settings.wrapR));
        glTexParameteri(glTexType, GL_TEXTURE_WRAP_S, static_cast<GLint>(settings.wrapS));
        glTexParameteri(glTexType, GL_TEXTURE_WRAP_T, static_cast<GLint>(settings.wrapT));

        texture.channels = settings.components;
        texture.format = settings.intendedFormat;
        texture.fileFormat = settings.fileFormat;

        // Construct the texture using the loaded data.
        texture.immutable = settings.immutable;
        if (settings.immutable)
        {
            texture.mipCount = settings.mipCount ? settings.mipCount : (settings.generateMipmaps ? math::log2(math::max(size.x, size.y)) : 1);
            glTexParameteri(glTexType, GL_TEXTURE_MAX_LEVEL, texture.mipCount);
            glTexStorage2D(
                glTexType,
                static_cast<GLint>(texture.mipCount),
                static_cast<GLint>(settings.intendedFormat),
                size.x,
                size.y);
        }
        else
        {
            texture.mipCount = settings.generateMipmaps ? math::log2(math::max(size.x, size.y)) : 1;
            glTexParameteri(glTexType, GL_TEXTURE_MAX_LEVEL, texture.mipCount);
            glTexImage2D(
                glTexType,
                0,
                static_cast<GLint>(settings.intendedFormat),
                size.x,
                size.y,
                0,
                components_to_format[static_cast<int>(settings.components)],
                channels_to_glenum[static_cast<uint>(settings.fileFormat)],
                nullptr);
        }

        glBindTexture(glTexType, 0);

        {
            async::readwrite_guard guard(m_textureLock);
            m_textures.insert(id, texture);
        }

        log::debug("Created blank texture of size {}", size);

        return { id };

    }

    texture_handle TextureCache::create_texture_from_image(const std::string& name, texture_import_settings settings)
    {
        if (m_invalidTexture.id == invalid_id)
            m_invalidTexture = create_texture("invalid texture", fs::view("engine://resources/invalid/missing"));

        assets::asset<image> img = assets::get<image>(name);
        if (img == assets::invalid_asset<image>)
        {
            log::warn("Image {} doesn't exist.", name);
            return invalid_texture_handle;
        }

        return create_texture_from_image(img, settings);
    }

    texture_handle TextureCache::create_texture_from_image(assets::asset<image> img, texture_import_settings settings)
    {
        if (m_invalidTexture.id == invalid_id)
            m_invalidTexture = create_texture("invalid texture", fs::view("engine://resources/invalid/missing"));

        if (img == assets::invalid_asset<image>)
        {
            log::warn("Tried to create a texture with an invalid image");
            return invalid_texture_handle;
        }

        id_type id = img.id();

        {
            async::readonly_guard guard(m_textureLock);
            if (m_textures.contains(id))
                return { id };
        }

        texture texture{};
        texture.type = settings.type;

        auto glTexType = static_cast<GLenum>(settings.type);

        // Allocate and bind the texture.
        glGenTextures(1, &texture.textureId);
        glBindTexture(glTexType, texture.textureId);

        // Handle mips
        glTexParameteri(glTexType, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(settings.min));
        glTexParameteri(glTexType, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(settings.mag));
        glTexParameteri(glTexType, GL_TEXTURE_BASE_LEVEL, 0);

        // Handle wrapping behavior.
        glTexParameteri(glTexType, GL_TEXTURE_WRAP_R, static_cast<GLint>(settings.wrapR));
        glTexParameteri(glTexType, GL_TEXTURE_WRAP_S, static_cast<GLint>(settings.wrapS));
        glTexParameteri(glTexType, GL_TEXTURE_WRAP_T, static_cast<GLint>(settings.wrapT));

        auto& res = img->resolution();

        texture.channels = img->components();
        texture.name = img.name();


        // Construct the texture using the loaded data.
        texture.immutable = settings.immutable;
        if (settings.immutable)
        {
            texture.mipCount = settings.mipCount ? settings.mipCount : (settings.generateMipmaps ? math::log2(math::max(res.x, res.y)) : 1);
            glTexParameteri(glTexType, GL_TEXTURE_MAX_LEVEL, texture.mipCount);
            glTexStorage2D(
                glTexType,
                static_cast<GLint>(texture.mipCount),
                static_cast<GLint>(settings.intendedFormat),
                res.x,
                res.y);

            glTexSubImage2D(
                glTexType,
                0,
                0,
                0,
                res.x,
                res.y,
                components_to_format[static_cast<int>(img->components())],
                channels_to_glenum[static_cast<uint>(img->format())],
                img->data());
        }
        else
        {
            texture.mipCount = settings.generateMipmaps ? math::log2(math::max(res.x, res.y)) : 1;
            glTexParameteri(glTexType, GL_TEXTURE_MAX_LEVEL, texture.mipCount);
            glTexImage2D(
                glTexType,
                0,
                static_cast<GLint>(settings.intendedFormat),
                res.x,
                res.y,
                0,
                components_to_format[static_cast<int>(img->components())],
                channels_to_glenum[static_cast<uint>(img->format())],
                img->data());
        }

        // Generate mips.
        if (settings.generateMipmaps)
            glGenerateMipmap(glTexType);

        glBindTexture(glTexType, 0);

        log::debug("Created texture from image {}", texture.name);

        {
            async::readwrite_guard guard(m_textureLock);
            m_textures.insert(id, std::move(texture));
        }

        return { id };
    }

    texture_handle TextureCache::get_handle(const std::string& name)
    {
        if (m_invalidTexture.id == invalid_id)
            m_invalidTexture = create_texture("invalid texture", fs::view("engine://resources/invalid/missing"));

        id_type id = nameHash(name);
        async::readonly_guard guard(m_textureLock);
        if (m_textures.contains(id))
            return { id };
        return invalid_texture_handle;
    }

    texture_handle TextureCache::get_handle(id_type id)
    {
        if (m_invalidTexture.id == invalid_id)
            m_invalidTexture = create_texture("invalid texture", fs::view("engine://resources/invalid/missing"));

        async::readonly_guard guard(m_textureLock);
        if (m_textures.contains(id))
            return { id };
        return invalid_texture_handle;
    }

    std::vector<texture_handle> TextureCache::get_all()
    {
        std::vector<texture_handle> textures;

        async::readonly_guard guard(m_textureLock);
        textures.reserve(m_textures.size());

        for (auto [id, texture] : m_textures)
            textures.push_back(texture_handle{ id });

        return textures;
    }

}
