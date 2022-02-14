#pragma once
#include <core/serialization/serializer/serializer.hpp>
#include <core/serialization/registry/serializationregistry.hpp>
#include <core/serialization/functionalbinds/functionalbinds.hpp>

#include <core/serialization/views/json.hpp>
#include <core/serialization/views/bson.hpp>
#include <core/serialization/views/yaml.hpp>

#include <core/serialization/serializer/serializer.inl>
#include <core/serialization/registry/serializationregistry.inl>
#include <core/serialization/functionalbinds/functionalbinds.inl>

namespace legion::core
{
    namespace srl = serialization;
}
