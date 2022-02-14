#include <core/assets/serialization/asset_serializer.hpp>
#pragma once

#define EndReadPropagate(_result, _warnings, _view)                                                 \
        _warnings.insert(_warnings.end(), _result.warnings().begin(), _result.warnings().end());    \
        if (_result.has_error()) { _view.end_read(); return { _result.error(), _warnings }; }


namespace legion::core::serialization
{
    template<typename AssetType>
    inline common::result<void, fs_error> serializer<assets::asset<AssetType>>::serialize(const void* serializable, serializer_view& s_view, std::string_view name)
    {
        const assets::asset<AssetType>& asset = *reinterpret_cast<const assets::asset<AssetType>*>(serializable);

        if (!asset)
            return legion_fs_error("Invalid asset.");

        std::vector<std::string> warnings{};

        s_view.start_object(std::string(name));
        s_view.serialize("type_name", std::string(nameOfType<assets::asset<AssetType>>()));

        s_view.serialize("name", asset.name());
        s_view.serialize("id", asset.id());

        if (auto& path = asset.path(); !path.empty())
            s_view.serialize("path", path);
        else
        {
            auto serializer = SerializerRegistry::getSerializer<AssetType>();
            if (!serializer)
                serializer = SerializerRegistry::registerSerializer<AssetType>();

            auto result = serializer->serialize(asset.ptr, s_view, "data");
            warnings.insert(warnings.end(), result.warnings().begin(), result.warnings().end());
            if (result.has_error())
            {
                s_view.end_object();
                return { result.error(), warnings };
            }
        }

        s_view.end_object();
        return { common::success, warnings };
    }

    template<typename AssetType>
    inline common::result<void, fs_error> serializer<assets::asset<AssetType>>::deserialize(void* target, serializer_view& s_view, std::string_view name)
    {
        std::vector<std::string> warnings{};

        {
            auto result = s_view.start_read(std::string(name));
            PropagateErrors(result, warnings);
        }

        {
            auto result = s_view.deserialize<std::string>("type_name");
            EndReadPropagate(result, warnings, s_view);

            if (*result != nameOfType<assets::asset<AssetType>>())
            {
                s_view.end_read();
                return { legion_fs_error("Item of name " + std::string(name) + " is not of type " + std::string(nameOfType<assets::asset<AssetType>>()) + " but of type " + *result + "."), warnings };
            }
        }

        auto nameRes = s_view.deserialize<std::string>("name");
        EndReadPropagate(nameRes, warnings, s_view);

        auto idRes = s_view.deserialize<id_type>("id");
        EndReadPropagate(idRes, warnings, s_view);

        if (s_view.has_item("path"))
        {
            auto pathRes = s_view.deserialize<std::string>("path");
        }
        else
        {
            byte buffer[sizeof(AssetType)];
            AssetType* data = reinterpret_cast<AssetType*>(&buffer[0]);

            auto serializer = SerializerRegistry::getSerializer<AssetType>();
            if (!serializer)
                serializer = SerializerRegistry::registerSerializer<AssetType>();

            auto result = serializer->deserialize(data, s_view, "data");
            EndReadPropagate(result, warnings, s_view);


        }

        return { common::success, warnings };
    }
}

#undef EndReadPropagate
