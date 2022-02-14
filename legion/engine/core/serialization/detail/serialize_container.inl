#pragma once
#include <core/serialization/registry/serializationregistry.hpp>
#include <core/serialization/views/serializer_view.hpp>


#define EndContainerPropagate(_result, _warnings, _view)                                            \
        _warnings.insert(_warnings.end(), _result.warnings().begin(), _result.warnings().end());    \
        if (_result.has_error()) { _view.end_container(); return { _result.error(), _warnings }; }


#define EndReadPropagate(_result, _warnings, _view)                                                 \
        _warnings.insert(_warnings.end(), _result.warnings().begin(), _result.warnings().end());    \
        if (_result.has_error()) { _view.end_read(); return { _result.error(), _warnings }; }

namespace legion::core::serialization::detail
{
    template<typename type>
    inline common::result<void, fs_error> serialize_container(type&& container, serializer_view& s_view, std::string_view name)
    {
        using container_type = remove_cvr_t<type>;
        using value_type = remove_cvr_t<typename container_type::value_type>;

        s_view.start_container(std::string(name));

        std::vector<std::string> warnings{};
        size_type i = 0;
        for (auto it = container.begin(); it != container.end(); ++it, i++)
        {
            if constexpr (is_serializable_v<value_type>)
            {
                if (!s_view.serialize("", *it))
                {
                    s_view.end_container();
                    return { legion_fs_error("Type was not a primitive serializable type."), warnings };
                }
            }
            else if constexpr (is_container_v<value_type>)
            {
                auto result = detail::serialize_container(*it, s_view, "");
                EndContainerPropagate(result, warnings, s_view);
            }
            else
            {
                auto _serializer = SerializerRegistry::getSerializer<value_type>();

                auto result = _serializer->serialize(&(*it), s_view, "");
                EndContainerPropagate(result, warnings, s_view);
            }
        }

        s_view.end_container();

        return { common::success, warnings };
    }

    template<typename type>
    inline common::result<void, fs_error> deserialize_container(void* target, serializer_view& s_view, std::string_view name)
    {
        using container_type = remove_cvr_t<type>;
        using value_type = remove_cvr_t<typename container_type::value_type>;
        std::vector<std::string> warnings{};

        {
            auto result = s_view.start_read(std::string(name));
            PropagateErrors(result, warnings);
        }

        size_type size = s_view.current_item_size();

        std::vector<value_type> tempContainer{};
        tempContainer.reserve(size);

        byte buffer[sizeof(value_type)];
        value_type* itemPtr = reinterpret_cast<value_type*>(buffer);

        for (size_type i = 0; i < size; i++)
        {
            if constexpr (is_serializable_v<value_type>)
            {
                auto result = s_view.deserialize<value_type>("");
                EndReadPropagate(result, warnings, s_view);

                new(itemPtr) value_type(*result);
            }
            else if constexpr (is_container_v<value_type>)
            {
                auto result = detail::deserialize_container(*itemPtr, s_view, "");
                EndReadPropagate(result, warnings, s_view);
            }
            else
            {
                auto _serializer = SerializerRegistry::getSerializer<value_type>();

                auto result = _serializer->deserialize(buffer, s_view, "");
                EndReadPropagate(result, warnings, s_view);
            }

            tempContainer.push_back(*itemPtr);
        }

        s_view.end_read();

        using iterator = typename container_type::iterator;
        using const_iterator = typename container_type::const_iterator;
        using input_iterator = typename std::vector<value_type>::iterator;

        if constexpr (std::is_constructible_v<container_type, input_iterator, input_iterator>)
            new (target) container_type(tempContainer.begin(), tempContainer.end());
        else if constexpr (std::is_default_constructible_v<container_type>)
        {
            auto& container = *(new (target) container_type());

            if constexpr (has_insert_v<container_type, iterator(const_iterator, input_iterator, input_iterator)>)
                container.insert(container.begin(), tempContainer.begin(), tempContainer.end());
            else if constexpr (has_insert_v<container_type, void(input_iterator, input_iterator)>)
                container.insert(tempContainer.begin(), tempContainer.end());
            else if constexpr (has_size_v<container_type, size_type()> && has_at_v<container_type, value_type & (size_type)>)
            {
                for (size_type i = 0; i < tempContainer.size(); i++)
                    container[i] = tempContainer[i];
            }
            else
            {
                static_assert(
                    !has_insert_v<container_type, iterator(const_iterator, input_iterator, input_iterator)>
                    && !has_insert_v<container_type, void(input_iterator, input_iterator)>
                    && !(has_size_v<container_type, size_type()> && has_at_v<container_type, value_type& (size_type)>)
                    , "Incompatible container type");
            }
        }
        else
        {
            static_assert(
                !std::is_constructible_v<container_type, input_iterator, input_iterator>
                && !std::is_default_constructible_v<container_type>
                , "Incompatible container type");
        }

        return { common::success, warnings };
    }
}
#undef EndReadPropagate
#undef EndContainerPropagate
