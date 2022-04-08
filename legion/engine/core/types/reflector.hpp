#pragma once
#include <core/platform/platform.hpp>
#include <core/types/meta.hpp>
#include <core/types/primitives.hpp>
#include <core/types/attributes.hpp>

#include <unordered_map>

namespace legion::core
{
    struct [[no_reflect]] member_reference;

    //a reference to data
    struct [[no_reflect]] reflector
    {
        using member_container = std::unordered_map<std::string_view, member_reference>;
        using attribute_container = std::vector<std::reference_wrapper<const attribute_base>>;

        id_type typeId;
        std::string_view typeName;
        member_container members;
        attribute_container attributes;

        void* data;

        reflector() = default;
        reflector(id_type id, std::string_view name, const member_container& _members, void* _adress)
            : typeId(id), typeName(name), members(_members), data(_adress) {}

        reflector(id_type id, std::string_view name, const member_container& _members, const void* _adress)
            : typeId(id), typeName(name), members(_members)
        {
            ptr_type adress = reinterpret_cast<ptr_type>(_adress);
            data = reinterpret_cast<void*>(adress);
        }

        reflector(const reflector& refl) : typeId(refl.typeId), typeName(refl.typeName), members(refl.members), data(refl.data) {}

        reflector& operator=(const reflector& rhs)
        {
            typeId = rhs.typeId;
            typeName = rhs.typeName;
            members = rhs.members;
            data = rhs.data;

            return *this;
        }
    };

    struct [[no_reflect]] primitive_reference
    {
        id_type typeId;
        void* data = nullptr;

        RULE_OF_5_NOEXCEPT(primitive_reference);
        primitive_reference(id_type id, void* address) noexcept : typeId(id), data(address) {}
        primitive_reference(id_type id, const void* address) noexcept : typeId(id)
        { ptr_type tmp = reinterpret_cast<ptr_type>(address); data = reinterpret_cast<void*>(tmp); }

        template<typename T>
        L_NODISCARD T* cast()
        {
            if (localTypeHash<T>() == typeId)
            {
                return reinterpret_cast<T*>(data);
            }
            return nullptr;
        }

        template<typename T>
        L_NODISCARD const T* cast() const
        {
            if (localTypeHash<T>() == typeId)
            {
                return reinterpret_cast<const T*>(data);
            }
            return nullptr;
        }
    };

    struct [[no_reflect]] member_reference
    {
        bool is_object;
        std::string_view name;
        std::vector<std::reference_wrapper<const attribute_base>> attributes;
        union
        {
            reflector object;
            primitive_reference primitive;
        };

        member_reference() noexcept : is_object(false), name(""), primitive() {}
        member_reference(std::string_view _name, const primitive_reference& _primitive) noexcept : is_object(false), name(_name), primitive(_primitive) {}
        member_reference(std::string_view _name, const reflector& refl) : is_object(true), name(_name), object(refl) {}
        member_reference(const member_reference& other)
        {
            is_object = other.is_object;
            name = other.name;
            if (is_object)
                new (&object) reflector(other.object);
            else
                new (&primitive) primitive_reference(other.primitive);
        }
        ~member_reference()
        {
            if (is_object)
                object.~reflector();
            else
                primitive.~primitive_reference();
        }

        member_reference& operator=(const member_reference& other)
        {
            if (is_object)
                object.~reflector();
            else
                primitive.~primitive_reference();

            is_object = other.is_object;
            name = other.name;
            if (is_object)
                new (&object) reflector(other.object);
            else
                new (&primitive) primitive_reference(other.primitive);

            return *this;
        }
    };

    template<typename T>
    L_NODISCARD extern auto make_reflector(T& obj)->std::conditional_t<std::is_const_v<T>, const reflector, reflector>;
}

#if !defined(L_AUTOGENACTIVE)
#include <core/autogen/autogen.hpp>
#endif

