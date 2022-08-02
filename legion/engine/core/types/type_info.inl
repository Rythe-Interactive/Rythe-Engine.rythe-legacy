#include <core/types/type_info.hpp>
#pragma once

namespace legion::core
{
    template<typename ObjectType>
    template<typename type>
    void type_info<ObjectType>::addChild()
    {

    }

    template<typename ObjectType>
    template<typename type>
    void type_info<ObjectType>::addParent()
    {

    }

    template<typename ObjectType>
    static pointer<type_info<ObjectType>> TypeInfoRegistry::registerType()
    {
        auto [iterator, emplaced] = serializers.try_emplace(make_hash<ObjectType>().id(), std::make_unique<serializer<ObjectType>>(std::forward<Args>(args)...));
        return { dynamic_cast<serializer<ObjectType>*>(iterator->second.get()) };
    }

    template<typename ObjectType>
    static pointer<type_info<ObjectType>> TypeInfoRegistry::getTypeInfo()
    {

    }
}
