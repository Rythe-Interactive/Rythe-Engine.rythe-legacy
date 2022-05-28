#pragma once
#include <unordered_map>
#include <vector>
#include <memory>

#include <core/platform/platform.hpp>
#include <core/filesystem/filesystem.hpp>
#include <core/common/result.hpp>
#include <core/async/async_operation.hpp>
#include <core/engine/engine.hpp>
#include <core/engine/enginesubsystem.hpp>
#include <core/types/meta.hpp>

#include <core/assets/asset.hpp>
#include <core/assets/assetloader.hpp>
#include <core/assets/import_settings.hpp>

namespace legion::core::assets
{
    namespace detail
    {
        struct asset_info
        {
            std::string name;
            std::string path;
            size_type assetLoader;
        };
    }

    template<typename AssetType>
    class AssetCache : public EngineSubSystem<AssetCache<AssetType>>
    {
        AllowPrivateOnShutdown;
        SubSystemInstance(AssetCache);

        template<typename Asset>
        friend class AssetLoader;

        template<typename Asset>
        friend struct asset;

        using base = EngineSubSystem<AssetCache<AssetType>>;
        using asset_ptr = asset<AssetType>;
        using import_cfg = import_settings<AssetType>;
        using loader_type = AssetLoader<AssetType>;
        using progress_type = async::async_progress<common::result<asset_ptr>>;

    private:
        std::unordered_map<id_type, AssetType> m_cache;
        std::unordered_map<id_type, detail::asset_info> m_info;
        std::unordered_map<id_type, import_cfg> m_importSettings;

        std::vector<std::unique_ptr<loader_type>> m_loaders;
        std::unordered_map<id_type, id_type> m_loaderIds;

        static void onShutdown();

        template<typename... Args>
        static asset_ptr createInternal(id_type nameHash, Args&&... args);

        static const detail::asset_info& info(id_type nameHash);
        static const import_cfg& importSettings(id_type nameHash);

        static common::result<asset_ptr> retryLoad(id_type previousLoader, id_type nameHash, const std::string& name, const fs::view& file, const import_cfg& settings);
        static common::result<asset_ptr> retryLoadAsync(id_type previousLoader, id_type nameHash, const std::string& name, const fs::view& file, const import_cfg& settings, progress_type& progress);

        static void asyncLoadJob(id_type nameHash, const std::string& name, const fs::view& file, const import_cfg& settings, const std::shared_ptr<progress_type>& progress);

    public:

        static byte reportAssetToEngine();

        template<typename LoaderType>
        static bool hasLoader();

        static bool hasLoaders() noexcept;

        static void addLoader(std::unique_ptr<loader_type>&& loader);
        template<typename LoaderType, typename... Arguments>
        static void addLoader(Arguments&&... args);

        template<typename LoaderType, typename... Arguments>
        static asset_ptr createAsLoader(id_type nameHash, const std::string& name, const std::string& path, Arguments&&... args);

        static common::result<asset_ptr> load(const fs::view& file);
        static common::result<asset_ptr> load(const fs::view& file, const import_cfg& settings);
        static common::result<asset_ptr> load(const std::string& name, const fs::view& file);
        static common::result<asset_ptr> load(const std::string& name, const fs::view& file, const import_cfg& settings);
        static common::result<asset_ptr> load(id_type nameHash, const std::string& name, const fs::view& file);
        static common::result<asset_ptr> load(id_type nameHash, const std::string& name, const fs::view& file, const import_cfg& settings);
        static async::async_operation<common::result<asset_ptr>> loadAsync(const fs::view& file);
        static async::async_operation<common::result<asset_ptr>> loadAsync(const fs::view& file, const import_cfg& settings);
        static async::async_operation<common::result<asset_ptr>> loadAsync(const std::string& name, const fs::view& file);
        static async::async_operation<common::result<asset_ptr>> loadAsync(const std::string& name, const fs::view& file, const import_cfg& settings);
        static async::async_operation<common::result<asset_ptr>> loadAsync(id_type nameHash, const std::string& name, const fs::view& file);
        static async::async_operation<common::result<asset_ptr>> loadAsync(id_type nameHash, const std::string& name, const fs::view& file, const import_cfg& settings);

        template<typename... Arguments, ::std::enable_if_t<element_at_is_same_as_v<0, ::std::string, remove_cvr_t<Arguments>...>, bool> = true>
        static asset_ptr create(id_type nameHash, Arguments&&... args);
        template<typename... Arguments>
        static asset_ptr create(const std::string& name, Arguments&&... args);
        template<typename... Arguments>
        static asset_ptr create(id_type nameHash, const std::string& name, Arguments&&... args);
        template<typename... Arguments>
        static asset_ptr createWithPath(id_type nameHash, const std::string& name, const std::string& path, Arguments&&... args);

        static bool has(const std::string& name);
        static bool has(id_type nameHash);

        static asset_ptr get(const std::string& name);
        static asset_ptr get(id_type nameHash);

        static std::vector<asset_ptr> getAll();

        static void destroy(const std::string& name);
        static void destroy(id_type nameHash);
        static void destroy(asset_ptr asset);
    };
}

#define ReportAssetType(Type) ANON_VAR(byte, CONCAT(_reportAsset_, Type)) = legion::core::assets::AssetCache< Type >::reportAssetToEngine();
