#include <core/serialization/serializer/asset_serializer.hpp>
#pragma once
#include <core/serialization/serializationregistry.hpp>

#define EndReadPropagate(_result, _warnings, _view)                                                 \
        _warnings.insert(_warnings.end(), _result.warnings().begin(), _result.warnings().end());    \
        if (_result.has_error()) { _view.end_read(); return { _result.error(), _warnings }; }

#define EndObjectPropagate(_result, _warnings, _view)                                               \
        _warnings.insert(_warnings.end(), _result.warnings().begin(), _result.warnings().end());    \
        if (_result.has_error()) { _view.end_object(); return { _result.error(), _warnings }; }

namespace legion::core::serialization
{
    template<typename AssetType>
    inline common::result<void, fs_error> serializer<assets::asset<AssetType>>::serialize(const void* src, serializer_view& view, std::string_view name)
    {
        std::vector<std::string> warnings{};

        view.start_object(std::string(name));
        view.serialize<std::string>("type_name", nameOfType<assets::asset<AssetType>>());

        const assets::asset<AssetType>& ref = *static_cast<const assets::asset<AssetType>*>(src);

        view.serialize("name", ref.name());

        auto& path = ref.path();
        if (!path.empty())
        {
            view.serialize("loaded_from_file", true);
            view.serialize("path", path);

            auto typeId = typeHash<assets::import_settings<AssetType>>();

            auto _serializer = SerializerRegistry::getSerializer(typeId);
            if (_serializer)
            {
                auto result = _serializer->serialize(&ref.import_settings(), view, "import_settings");
                EndObjectPropagate(result, warnings, view);
            }
            else
            {
                warnings.push_back("Could not find existing serializer for " + std::string(nameOfType<assets::import_settings<AssetType>>()));
            }
        }
        else
        {
            view.serialize("loaded_from_file", false);
            auto typeId = typeHash<AssetType>();

            auto _serializer = SerializerRegistry::getSerializer(typeId);
            if (!_serializer)
            {
                warnings.push_back("Could not find existing serializer for " + std::string(nameOfType<AssetType>()));
                view.end_object();
                return { common::success, warnings };
            }

            auto result = _serializer->serialize(ref.ptr, view, "value");
            EndObjectPropagate(result, warnings, view);
        }

        view.end_object();
        return { common::success, warnings };
    }

    template<typename AssetType>
    inline common::result<void, fs_error> serializer<assets::asset<AssetType>>::deserialize(void* target, serializer_view& view, std::string_view name)
    {
        std::vector<std::string> warnings{};

        {
            auto result = view.start_read(std::string(name)); // Object
            PropagateErrors(result, warnings);
        }

        {
            auto result = view.deserialize<std::string>("type_name");
            EndReadPropagate(result, warnings, view);

            if (*result != nameOfType<assets::asset<AssetType>>())
            {
                view.end_read();
                return { legion_fs_error("Item of name " + std::string(name) + " is not of type " + std::string(nameOfType<assets::asset<AssetType>>()) + " but of type " + *result + "."), warnings };
            }
        }

        std::string assetName;

        {
            auto result = view.deserialize<std::string>("name");
            EndReadPropagate(result, warnings, view);
            assetName = *result;
        }

        bool loadedFromFile;

        {
            auto result = view.deserialize<bool>("loaded_from_file");
            EndReadPropagate(result, warnings, view);
            loadedFromFile = *result;
        }

        if (loadedFromFile)
        {
            std::string path;

            {
                auto result = view.deserialize<std::string>("path");
                EndReadPropagate(result, warnings, view);
                path = *result;
            }

            id_type typeId = typeHash<assets::import_settings<AssetType>>();
            auto _serializer = SerializerRegistry::getSerializer(typeId);

            byte_vec buffer{};
            buffer.resize(sizeof(assets::import_settings<AssetType>));
            auto settingsPtr = reinterpret_cast<assets::import_settings<AssetType>*>(buffer.data());
            {
                auto result = _serializer->deserialize(settingsPtr, view, "import_settings");

                warnings.insert(warnings.end(), result.warnings().begin(), result.warnings().end());

                if (result.has_error())
                {
                    new(settingsPtr) assets::import_settings<AssetType>();
                    result.mark_handled();
                }
            }

            {
                auto result = assets::load<AssetType>(assetName, fs::view(path), *settingsPtr);

                warnings.insert(warnings.end(), result.warnings().begin(), result.warnings().end());
                if (result.has_error())
                {
                    view.end_read();
                    auto& error = result.error();
                    return { fs_error(error.what(), error.file(), error.line(), error.func()), warnings};
                }

                new(target) assets::asset<AssetType>(*result);
            }
        }
        else
        {
            id_type typeId = typeHash<AssetType>();
            auto _serializer = SerializerRegistry::getSerializer(typeId);

            if (!_serializer)
            {
                warnings.push_back("Could not find existing serializer for " + std::string(nameOfType<AssetType>()));

                new(target) assets::asset<AssetType>(assets::create<AssetType>(assetName));

                view.end_read();
                return { common::success, warnings };
            }

            byte_vec buffer{};
            buffer.resize(sizeof(AssetType));
            AssetType* assetPtr = reinterpret_cast<AssetType*>(buffer.data());
            {
                auto result = _serializer->deserialize(assetPtr, view, "value");
                EndReadPropagate(result, warnings, view);
            }

            new(target) assets::asset<AssetType>(assets::create<AssetType>(assetName, *assetPtr));
        }

        view.end_read(); // Object
        return { common::success, warnings };
    }
}

#undef EndReadPropagate
#undef EndObjectPropagate
