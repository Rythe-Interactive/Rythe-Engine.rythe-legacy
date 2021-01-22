#include <core/scenemanagement/components/scene.hpp>
#include <core/serialization/serializationUtil.hpp>
#include <core/logging/logging.hpp>
#include <core/common/string_extra.hpp>
#include <core/defaults/defaultcomponents.hpp>
//#include <rendering/components/camera.hpp>


namespace legion::core::scenemanagement
{
    int SceneManager::sceneCount;
    std::string SceneManager::currentScene = "Main";
    std::unordered_map < id_type, std::string> SceneManager::sceneNames;
    std::unordered_map<id_type, ecs::component_handle<scene>> SceneManager::sceneList;
    std::atomic_bool SceneManager::doNotCreateEntities{ false };

    ecs::entity_handle SceneManager::create_scene_entity()
    {
        ecs::entity_handle sceneEntity;
        static ecs::EntityQuery sceneEntities = m_ecs->createQuery<scenemanagement::scene>();
        sceneEntities.queryEntities();
        if (sceneEntities.size() == 0)
        {
            log::debug("Creating a Scene Entity");
            sceneEntity = m_ecs->createEntity();
            sceneEntity.add_component<hierarchy>();
            sceneEntity.add_component<scene>();
            std::vector<ecs::entity_handle> children;
            for (size_type i = 0; i < m_ecs->world.child_count(); i++)
            {
                children.push_back(m_ecs->world.get_child(i));
            }
            log::debug("Child count {}",m_ecs->world.child_count());
            for (auto child : children)
            {
                if (child == sceneEntity)
                    continue;
                child.set_parent(sceneEntity,true);
            }
            m_ecs->world.read_component<hierarchy>().children.clear();
        }
        else
        {
            sceneEntity = sceneEntities[0];
        }
        return sceneEntity;
    }

    bool SceneManager::create_scene(const std::string& name)
    {
        ecs::entity_handle sceneEntity = create_scene_entity();

        if (!get_scene(name))
        {
            scene s;
            s.id = nameHash(name);
            sceneNames.emplace(s.id, name);
            sceneList.emplace(nameHash(name), sceneEntity);
        }
        return SceneManager::save_scene(name, sceneEntity);
    }

    bool SceneManager::create_scene(const std::string& name, ecs::entity_handle& ent)
    {
        if (!ent.has_component<scene>())
        {
            scene s;
            s.id = nameHash(name);
            sceneNames.emplace(s.id, name);
            auto sceneHandle = ent.add_component<scene>(s);
            sceneList.emplace(nameHash(name), sceneHandle);
            sceneCount++;
            //true if entity does not have the scene component
            return save_scene(name, ent);
        }
        //false if it doesn't
        return false;
    }

    bool SceneManager::load_scene(const std::string& name)
    {

        std::string filename = name;
        if (!common::ends_with(filename, ".cornflake")) filename += ".cornflake";

        std::ifstream inFile("assets/scenes/" + filename);

        log::debug("Child Count Before: {}", m_ecs->world.child_count());
        doNotCreateEntities = true;
        while (m_ecs->world.child_count() > 0)
        {
            log::debug("children remaining {}", m_ecs->world.child_count());
            m_ecs->world.get_child(m_ecs->world.child_count() - 1).destroy(true);
        }
        m_ecs->world.read_component<hierarchy>().children.clear();

        log::debug("Child Count After: {}", m_ecs->world.child_count());

        auto sceneEntity = serialization::SerializationUtil::JSONDeserialize<ecs::entity_handle>(inFile);
        currentScene = name;

        doNotCreateEntities = true;
        //SceneManager::saveScene(name, sceneEntity);
        //log::debug("........Done saving scene");
        return true;
    }

    bool SceneManager::save_scene(const std::string& name, ecs::entity_handle& ent)
    {
        std::ofstream outFile("assets/scenes/" + name + ".cornflake");
        serialization::SerializationUtil::JSONSerialize<ecs::entity_handle>(outFile, ent);
        return true;
    }

    ecs::component_handle<scene> SceneManager::get_scene(std::string name)
    {
        return sceneList[nameHash(name)];
    }

    ecs::entity_handle SceneManager::get_scene_entity(std::string name)
    {
        return sceneList[nameHash(name)].entity;
    }
}
