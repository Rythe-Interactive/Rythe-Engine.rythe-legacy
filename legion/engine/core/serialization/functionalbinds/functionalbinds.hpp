#pragma once
#include <core/common/result.hpp>
#include <core/serialization/serializer/serializer.hpp>
#include <core/serialization/registry/serializationregistry.hpp>

namespace legion::core::serialization
{
    template<typename ViewType, typename Type>
    common::result<void, fs_error> serialize(ViewType& s_view, Type&& data, std::string_view name);

    template<typename ViewType, typename Type>
    common::result<void, fs_error> write(fs::view file, Type&& data, std::string_view name);

    template<typename ViewType, typename Type>
    common::result<void, fs_error> write(Type&& data, fs::view file, std::string_view name);

    template<typename Type, typename ViewType>
    common::result<Type, fs_error> deserialize(ViewType& s_view, std::string_view name);

    template<typename ViewType, typename Type>
    common::result<Type, fs_error> load(const fs::view& file, std::string_view name);

    template<typename ViewType, typename Type>
    common::result<Type, fs_error> load(const byte_vec& data, std::string_view name);

    template<typename ViewType, typename Iterator, typename Type>
    common::result<Type, fs_error> load(Iterator begin, Iterator end, std::string_view name);
}
