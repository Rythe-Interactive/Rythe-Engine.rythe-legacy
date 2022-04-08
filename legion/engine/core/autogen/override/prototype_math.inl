#include "prototype_math.hpp"
#include "../../../core/math/math.hpp"
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<math::vec2>(const math::vec2& obj)
    {
        prototype prot;
        prot.typeId = typeHash<math::vec2>();
        prot.typeName = "legion::core::math::vec2";
        {
            static const reflectable_attribute reflectable_attr{};
            prot.attributes.push_back(std::cref(reflectable_attr));
        }
        prot.members.emplace("x", member_value("x", primitive_value{ typeHash<float>(), ::std::any(obj.x) }));
        prot.members.emplace("y", member_value("y", primitive_value{ typeHash<float>(), ::std::any(obj.y) }));
        return prot;
    }
    template<>
    L_NODISCARD prototype make_prototype<math::vec3>(const math::vec3& obj)
    {
        prototype prot;
        prot.typeId = typeHash<math::vec3>();
        prot.typeName = "legion::core::math::vec3";
        {
            static const reflectable_attribute reflectable_attr{};
            prot.attributes.push_back(std::cref(reflectable_attr));
        }
        prot.members.emplace("x", member_value("x", primitive_value{ typeHash<float>(), ::std::any(obj.x) }));
        prot.members.emplace("y", member_value("y", primitive_value{ typeHash<float>(), ::std::any(obj.y) }));
        prot.members.emplace("z", member_value("z", primitive_value{ typeHash<float>(), ::std::any(obj.z) }));
        return prot;
    }
    template<>
    L_NODISCARD prototype make_prototype<math::vec4>(const math::vec4& obj)
    {
        prototype prot;
        prot.typeId = typeHash<math::vec4>();
        prot.typeName = "legion::core::math::vec4";
        {
            static const reflectable_attribute reflectable_attr{};
            prot.attributes.push_back(std::cref(reflectable_attr));
        }
        prot.members.emplace("x", member_value("x", primitive_value{ typeHash<float>(), ::std::any(obj.x) }));
        prot.members.emplace("y", member_value("y", primitive_value{ typeHash<float>(), ::std::any(obj.y) }));
        prot.members.emplace("z", member_value("z", primitive_value{ typeHash<float>(), ::std::any(obj.z) }));
        prot.members.emplace("w", member_value("w", primitive_value{ typeHash<float>(), ::std::any(obj.w) }));
        return prot;
    }
    template<>
    L_NODISCARD prototype make_prototype<math::mat4>(const math::mat4& obj)
    {
        prototype prot;
        prot.typeId = typeHash<math::mat4>();
        prot.typeName = "legion::core::math::mat4";
        {
            static const reflectable_attribute reflectable_attr{};
            prot.attributes.push_back(std::cref(reflectable_attr));
        }
        prot.members.emplace("col0", member_value("col0", make_prototype(obj[0])));
        prot.members.emplace("col1", member_value("col1", make_prototype(obj[1])));
        prot.members.emplace("col2", member_value("col2", make_prototype(obj[2])));
        prot.members.emplace("col3", member_value("col3", make_prototype(obj[3])));
        return prot;
    }
}
