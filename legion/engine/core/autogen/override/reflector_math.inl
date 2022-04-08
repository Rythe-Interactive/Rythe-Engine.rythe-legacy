#include "reflector_math.hpp"
#include "../../../core/math/math.hpp"

namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<math::vec2>(math::vec2& obj)
    {
        reflector refl;
        refl.typeId = typeHash<math::vec2>();
        refl.typeName = "legion::core::math::vec2";
        {
            static const reflectable_attribute reflectable_attr{};
            refl.attributes.push_back(std::cref(reflectable_attr));
        }
        refl.members.emplace("x", member_reference("x", primitive_reference{ typeHash<float>(), &obj.x }));
        refl.members.emplace("y", member_reference("y", primitive_reference{ typeHash<float>(), &obj.y }));
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const math::vec2>(const math::vec2& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<math::vec2>();
        refl.typeName = "legion::core::math::vec2";
        {
            static const reflectable_attribute reflectable_attr{};
            refl.attributes.push_back(std::cref(reflectable_attr));
        }
        refl.members.emplace("x", member_reference("x", primitive_reference{ typeHash<float>(), &obj.x }));
        refl.members.emplace("y", member_reference("y", primitive_reference{ typeHash<float>(), &obj.y }));
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
    template<>
    L_NODISCARD reflector make_reflector<math::vec3>(math::vec3& obj)
    {
        reflector refl;
        refl.typeId = typeHash<math::vec3>();
        refl.typeName = "legion::core::math::vec3";
        {
            static const reflectable_attribute reflectable_attr{};
            refl.attributes.push_back(std::cref(reflectable_attr));
        }
        refl.members.emplace("x", member_reference("x", primitive_reference{ typeHash<float>(), &obj.x }));
        refl.members.emplace("y", member_reference("y", primitive_reference{ typeHash<float>(), &obj.y }));
        refl.members.emplace("z", member_reference("z", primitive_reference{ typeHash<float>(), &obj.z }));
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const math::vec3>(const math::vec3& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<math::vec3>();
        refl.typeName = "legion::core::math::vec3";
        {
            static const reflectable_attribute reflectable_attr{};
            refl.attributes.push_back(std::cref(reflectable_attr));
        }
        refl.members.emplace("x", member_reference("x", primitive_reference{ typeHash<float>(), &obj.x }));
        refl.members.emplace("y", member_reference("y", primitive_reference{ typeHash<float>(), &obj.y }));
        refl.members.emplace("z", member_reference("z", primitive_reference{ typeHash<float>(), &obj.z }));
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
    template<>
    L_NODISCARD reflector make_reflector<math::vec4>(math::vec4& obj)
    {
        reflector refl;
        refl.typeId = typeHash<math::vec4>();
        refl.typeName = "legion::core::math::vec4";
        {
            static const reflectable_attribute reflectable_attr{};
            refl.attributes.push_back(std::cref(reflectable_attr));
        }
        refl.members.emplace("x", member_reference("x", primitive_reference{ typeHash<float>(), &obj.x }));
        refl.members.emplace("y", member_reference("y", primitive_reference{ typeHash<float>(), &obj.y }));
        refl.members.emplace("z", member_reference("z", primitive_reference{ typeHash<float>(), &obj.z }));
        refl.members.emplace("w", member_reference("w", primitive_reference{ typeHash<float>(), &obj.w }));
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const math::vec4>(const math::vec4& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<math::vec4>();
        refl.typeName = "legion::core::math::vec4";
        {
            static const reflectable_attribute reflectable_attr{};
            refl.attributes.push_back(std::cref(reflectable_attr));
        }
        refl.members.emplace("x", member_reference("x", primitive_reference{ typeHash<float>(), &obj.x }));
        refl.members.emplace("y", member_reference("y", primitive_reference{ typeHash<float>(), &obj.y }));
        refl.members.emplace("z", member_reference("z", primitive_reference{ typeHash<float>(), &obj.z }));
        refl.members.emplace("w", member_reference("w", primitive_reference{ typeHash<float>(), &obj.w }));
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
    template<>
    L_NODISCARD reflector make_reflector<math::mat4>(math::mat4& obj)
    {
        reflector refl;
        refl.typeId = typeHash<math::mat4>();
        refl.typeName = "legion::core::math::mat4";
        {
            static const reflectable_attribute reflectable_attr{};
            refl.attributes.push_back(std::cref(reflectable_attr));
        }
        refl.members.emplace("col0", member_reference("col0", make_reflector(obj[0])));
        refl.members.emplace("col1", member_reference("col1", make_reflector(obj[1])));
        refl.members.emplace("col2", member_reference("col2", make_reflector(obj[2])));
        refl.members.emplace("col3", member_reference("col3", make_reflector(obj[3])));
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const math::mat4>(const math::mat4& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<math::mat4>();
        refl.typeName = "legion::core::math::mat4";
        {
            static const reflectable_attribute reflectable_attr{};
            refl.attributes.push_back(std::cref(reflectable_attr));
        }
        refl.members.emplace("col0", member_reference("col0", make_reflector(obj[0])));
        refl.members.emplace("col1", member_reference("col1", make_reflector(obj[1])));
        refl.members.emplace("col2", member_reference("col2", make_reflector(obj[2])));
        refl.members.emplace("col3", member_reference("col3", make_reflector(obj[3])));
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
