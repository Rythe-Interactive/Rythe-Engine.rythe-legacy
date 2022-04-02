#pragma once
#include <core/common/hash.hpp>
#include <core/containers/pointer.hpp>
#include <core/serialization/serializer/serializer.hpp>

#include <unordered_map>
#include <memory>

namespace legion::core::serialization
{
    class SerializerRegistry
    {
    private:
        static std::unordered_map<id_type, std::unique_ptr<serializer_base>> serializers;
    public:
        template<typename ObjectType, typename... Args>
        static pointer<serializer<ObjectType>> registerSerializer(Args&&... args);

        template<typename type>
        static pointer<serializer<type>> getSerializer();
        static pointer<serializer_base> getSerializer(id_type typeId);
    };
}

#include <core/serialization/serializer/serializer.inl>
#include <core/serialization/serializationregistry.inl>

