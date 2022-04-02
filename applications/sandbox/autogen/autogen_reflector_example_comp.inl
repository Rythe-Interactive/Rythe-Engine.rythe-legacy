#include "autogen_reflector_example_comp.hpp"
#include "../../sandbox/systems/examplesystem.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<::example_comp>(::example_comp& obj)
    {
        reflector refl;
        refl.typeId = typeHash<::example_comp>();
        refl.typeName = "::example_comp";
        {
            static const reflectable_attribute reflectable_attr{};
            refl.attributes.push_back(std::cref(reflectable_attr));
        }
        refl.members.emplace("value", member_reference("value", primitive_reference{typeHash(obj.value), &obj.value}));
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const ::example_comp>(const ::example_comp& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<::example_comp>();
        refl.typeName = "::example_comp";
        {
            static const reflectable_attribute reflectable_attr{};
            refl.attributes.push_back(std::cref(reflectable_attr));
        }
        refl.members.emplace("value", member_reference("value", primitive_reference{typeHash(obj.value), &obj.value}));
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
