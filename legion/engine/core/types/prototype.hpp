#pragma once
#include <core/platform/platform.hpp>
#include <core/types/meta.hpp>
#include <core/types/primitives.hpp>
#include <core/types/attributes.hpp>

#include <any>
#include <unordered_map>

namespace legion::core
{
    struct [[no_reflect]] member_value;
    //a copy of data
    struct [[no_reflect]] prototype
    {
        using member_container = std::unordered_map<std::string_view, member_value>;
        using attribute_container = std::vector<std::reference_wrapper<const attribute_base>>;

        id_type typeId;
        std::string_view typeName;
        member_container members;
        attribute_container attributes;

        prototype() = default;
        prototype(id_type id, std::string_view name, const member_container& _members) : typeId(id), typeName(name), members(_members) {}
        prototype(const prototype& prot) : typeId(prot.typeId), typeName(prot.typeName), members(prot.members) {}

        prototype& operator=(const prototype& rhs)
        {
            typeId = rhs.typeId;
            typeName = rhs.typeName;
            members = rhs.members;

            return *this;
        }
    };

    struct [[no_reflect]] primitive_value
    {
        id_type typeId;
        std::any data;

        template<typename T>
        L_NODISCARD T* cast()
        {
            return std::any_cast<T>(&data);
        }

        template<typename T>
        L_NODISCARD const T* cast() const
        {
            return std::any_cast<T>(&data);
        }
    };

    struct [[no_reflect]] member_value
    {
        bool is_object;
        std::string_view name;
        std::vector<std::reference_wrapper<const attribute_base>> attributes;

        union
        {
            prototype object;
            primitive_value primitive;
        };

        member_value() : is_object(false), name(""), primitive() {}
        member_value(std::string_view _name, const primitive_value& _primitive) : is_object(false), name(_name), primitive(_primitive) {}
        member_value(std::string_view _name, const prototype& prot) : is_object(true), name(_name), object(prot) {}
        member_value(const member_value& other)
        {
            is_object = other.is_object;
            name = other.name;
            if (is_object)
                new (&object) prototype(other.object);
            else
                new (&primitive) primitive_value(other.primitive);
        }

        ~member_value()
        {
            if (is_object)
                object.~prototype();
            else
                primitive.~primitive_value();
        }

        member_value& operator=(const member_value& other)
        {
            if (is_object)
                object.~prototype();
            else
                primitive.~primitive_value();

            is_object = other.is_object;
            name = other.name;
            if (is_object)
                new (&object) prototype(other.object);
            else
                new (&primitive) primitive_value(other.primitive);

            return *this;
        }
    };

    template<typename T>
    L_NODISCARD extern prototype make_prototype(const T& obj);
}

#if !defined(L_AUTOGENACTIVE)
#include <core/autogen/autogen.hpp>
#endif
