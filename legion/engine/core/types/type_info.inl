#include <core/types/type_info.hpp>
#pragma once

namespace legion::core
{
    template<typename ObjectType>
    template<typename ChildType>
    void type_info<ObjectType>::addChild()
    {
        children.push_back(make_hash<ChildType>);
        auto& child = *TypeInfoRegistry::getTypeInfo<ChildType>();
        child.addParent<ObjectType>();
    }

    template<typename ObjectType>
    void type_info<ObjectType>::addChild(id_type typeId)
    {
        children.push_back(typeId);
        TypeInfoRegistry::getTypeInfo(typeId)->addParent<ObjectType>();
    }

    template<typename ObjectType>
    template<typename ParentType>
    void type_info<ObjectType>::addParent()
    {
        parents.push_back(make_hash<ParentType>());
        auto* parent = *TypeInfoRegistry::getTypeInfo<ParentType>();
        parent.addChild<ObjectType>();
    }

    template<typename ObjectType>
    void type_info<ObjectType>::addParent(id_type typeId)
    {
        parents.push_back(typeId);
        TypeInfoRegistry::getTypeInfo(typeId)->addChild<ObjectType>();
    }

    template<typename ObjectType>
    template<typename MemberType>
    void type_info<ObjectType>::addMember(const std::string& name)
    {
        auto nameHash = make_hash(name).value();
        auto typeId = make_hash<MemberType>().id();
        members.try_emplace(nameHash, typeId);
    }

    template<typename ObjectType>
    void type_info<ObjectType>::addMember(const std::string& name, id_type typeId)
    {
        auto nameHash = make_hash(name).value();
        members.try_emplace(nameHash, typeId);
    }

    template<typename ObjectType>
    template<typename MemberType>
    pointer<type_info<MemberType>> type_info<ObjectType>::getMember(const std::string& name)
    {
        auto nameHash = make_hash(name).value();
        if (members.count(nameHash))
            return TypeInfoRegistry::getTypeInfo<MemberType>() ;
        return { nullptr };
    }

    template<typename ObjectType>
    pointer<type_info_base> type_info<ObjectType>::getMember(const std::string& name)
    {
        auto nameHash = make_hash(name).value();
        if (members.count(nameHash))
            return TypeInfoRegistry::getTypeInfo(members[nameHash]) ;
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
