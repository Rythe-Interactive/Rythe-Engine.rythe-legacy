#include <core/types/type_info.hpp>
#pragma once

namespace legion::core
{
    template<typename ObjectType>
    template<typename ChildType>
    void type_info<ObjectType>::add_child()
    {
        auto typeId = make_hash<ChildType>().id();
        if (std::count(children.begin(), children.end(), typeId) > 0)
            return;
        children.push_back(typeId);
        auto& child = *TypeInfoRegistry::getTypeInfo<ChildType>();
        child.add_parent<ObjectType>();
    }

    template<typename ObjectType>
    void type_info<ObjectType>::add_child(id_type typeId)
    {
        if (std::count(children.begin(), children.end(), typeId) > 0)
            return;
        children.push_back(typeId);
        TypeInfoRegistry::getTypeInfo(typeId)->add_parent<ObjectType>();
    }

    template<typename ObjectType>
    template<typename ParentType>
    void type_info<ObjectType>::add_parent()
    {
        auto typeId = make_hash<ParentType>().id();
        if (std::count(parents.begin(), parents.end(), typeId) > 0)
            return;
        parents.push_back(typeId);
        auto& parent = *TypeInfoRegistry::getTypeInfo<ParentType>();
        parent.add_child<ObjectType>();
    }

    template<typename ObjectType>
    void type_info<ObjectType>::add_parent(id_type typeId)
    {
        if (std::count(parents.begin(), parents.end(), typeId) > 0)
            return;
        parents.push_back(typeId);
        TypeInfoRegistry::getTypeInfo(typeId)->add_child<ObjectType>();
    }

    template<typename ObjectType>
    template<typename MemberType>
    void type_info<ObjectType>::add_member(const std::string& name)
    {
        auto nameHash = make_hash(name).value();
        id_type typeId = make_hash<MemberType>().id();
        members.try_emplace(nameHash, typeId);
    }

    template<typename ObjectType>
    void type_info<ObjectType>::add_member(const std::string& name, id_type typeId)
    {
        auto nameHash = make_hash(name).value();
        members.try_emplace(nameHash, typeId);
    }

    template<typename ObjectType>
    template<typename MemberType>
    pointer<type_info<MemberType>> type_info<ObjectType>::get_member(const std::string& name)
    {
        auto nameHash = make_hash(name).value();
        if (members.count(nameHash))
            return TypeInfoRegistry::getTypeInfo<MemberType>();
        return { nullptr };
    }

    template<typename ObjectType>
    pointer<type_info_base> type_info<ObjectType>::get_member(const std::string& name)
    {
        auto nameHash = make_hash(name).value();
        if (members.count(nameHash))
            return TypeInfoRegistry::getTypeInfo(members[nameHash]);
        return { nullptr };
    }

    template<typename ObjectType>
    inline pointer<type_info<ObjectType>> TypeInfoRegistry::registerType()
    {
        auto [iterator, emplaced] = registry.try_emplace(make_hash<ObjectType>().id(), std::make_unique<type_info<ObjectType>>());
        return { dynamic_cast<type_info<ObjectType>*>(iterator->second.get()) };
    }

    template<typename ObjectType>
    inline pointer<type_info<ObjectType>> TypeInfoRegistry::getTypeInfo()
    {
        static const id_type typeId = make_hash<ObjectType>().id();
        if (registry.count(typeId))
            return { dynamic_cast<type_info<ObjectType>*>(registry.at(typeId).get()) };
        return { nullptr };
    }

    inline pointer<type_info_base> TypeInfoRegistry::getTypeInfo(id_type typeId)
    {
        if (registry.count(typeId))
            return { registry.at(typeId).get() };
        return { nullptr };
    }
}
