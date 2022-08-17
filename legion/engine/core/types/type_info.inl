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
    template<typename TemplateParameterType>
    void type_info<ObjectType>::add_template_parameter(const std::string& name)
    {
        id_type typeId = make_hash<TemplateParameterType>().id();
        templateParameters.try_emplace(name, typeId);
    }

    template<typename ObjectType>
    void type_info<ObjectType>::add_template_parameter(const std::string& name, id_type typeId)
    {
        templateParameters.try_emplace(name, typeId);
    }

    template<typename ObjectType>
    template<typename AttributeType>
    void type_info<ObjectType>::add_attribute(const std::string& name)
    {
        id_type typeId = make_hash<AttributeType>().id();
        attributes.try_emplace(name, typeId);
    }

    template<typename ObjectType>
    void type_info<ObjectType>::add_attribute(const std::string& name, id_type typeId)
    {
        attributes.try_emplace(name, typeId);
    }

    template<typename ObjectType>
    template<typename MemberType>
    void type_info<ObjectType>::add_member(const std::string& name)
    {
        id_type typeId = make_hash<MemberType>().id();
        members.try_emplace(name, typeId);
    }

    template<typename ObjectType>
    void type_info<ObjectType>::add_member(const std::string& name, id_type typeId)
    {
        members.try_emplace(name, typeId);
    }

    template<typename ObjectType>
    template<typename FunctionType>
    void type_info<ObjectType>::add_function(const std::string& name)
    {
        id_type typeId = make_hash<FunctionType>().id();
        functions.try_emplace(name, typeId);
    }

    template<typename ObjectType>
    void type_info<ObjectType>::add_function(const std::string& name, id_type typeId)
    {
        functions.try_emplace(name, typeId);
    }

    template<typename ObjectType>
    template<typename ChildType >
    pointer<type_info<ChildType>> type_info<ObjectType>::get_child() const noexcept
    {
        if (std::count(children.begin(), children.end(), make_hash<ChildType>().id()) > 0)
            return { nullptr };
        else
            return TypeInfoRegistry::getTypeInfo<ChildType>();
    }

    template<typename ObjectType>
    pointer<type_info_base> type_info<ObjectType>::get_child(id_type typeId) const noexcept
    {
        if (std::count(children.begin(), children.end(), typeId) > 0)
            return { nullptr };
        else
            return TypeInfoRegistry::getTypeInfo(typeId);
    }

    template<typename ObjectType>
    template<typename ParentType >
    pointer<type_info<ParentType>> type_info<ObjectType>::get_parent() const noexcept
    {
        if (std::count(parents.begin(), parents.end(), make_hash<ParentType>().id()) > 0)
            return { nullptr };
        else
            return TypeInfoRegistry::getTypeInfo<ParentType>();
    }

    template<typename ObjectType>
    pointer<type_info_base> type_info<ObjectType>::get_parent(id_type typeId) const noexcept
    {
        if (std::count(parents.begin(), parents.end(), typeId) > 0)
            return { nullptr };
        else
            return TypeInfoRegistry::getTypeInfo(typeId);
    }


    template<typename ObjectType>
    template<typename TemplateParameterType>
    pointer<type_info<TemplateParameterType>> type_info<ObjectType>::get_template_parameter(const std::string& name) const noexcept
    {
        if (templateParameters.count(name))
            return TypeInfoRegistry::getTypeInfo<TemplateParameterType>();
        return { nullptr };
    }

    template<typename ObjectType>
    pointer<type_info_base> type_info<ObjectType>::get_template_parameter(const std::string& name) const noexcept
    {
        if (templateParameters.count(name))
            return TypeInfoRegistry::getTypeInfo(templateParameters[name]);
        return { nullptr };
    }

    template<typename ObjectType>
    template<typename AttributeType>
    pointer<type_info<AttributeType>> type_info<ObjectType>::get_attribute(const std::string& name) const noexcept
    {
        if (attributes.count(name))
            return TypeInfoRegistry::getTypeInfo<AttributeType>();
        return { nullptr };
    }

    template<typename ObjectType>
    pointer<type_info_base> type_info<ObjectType>::get_attribute(const std::string& name) const noexcept
    {
        if (attributes.count(name))
            return TypeInfoRegistry::getTypeInfo(attributes[name]);
        return { nullptr };
    }

    template<typename ObjectType>
    template<typename MemberType>
    pointer<type_info<MemberType>> type_info<ObjectType>::get_member(const std::string& name) const noexcept
    {
        if (members.count(name))
            return TypeInfoRegistry::getTypeInfo<MemberType>();
        return { nullptr };
    }

    template<typename ObjectType>
    pointer<type_info_base> type_info<ObjectType>::get_member(const std::string& name) const noexcept
    {
        if (members.count(name))
            return TypeInfoRegistry::getTypeInfo(members[name]);
        return { nullptr };
    }

    template<typename ObjectType>
    template<typename FunctionType>
    pointer<type_info<FunctionType>> type_info<ObjectType>::get_function(const std::string& name) const noexcept
    {
        if (functions.count(name))
            return TypeInfoRegistry::getTypeInfo<FunctionType>();
        return { nullptr };
    }

    template<typename ObjectType>
    pointer<type_info_base> type_info<ObjectType>::get_function(const std::string& name) const noexcept
    {
        if (functions.count(name))
            return TypeInfoRegistry::getTypeInfo(functions[name]);
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
