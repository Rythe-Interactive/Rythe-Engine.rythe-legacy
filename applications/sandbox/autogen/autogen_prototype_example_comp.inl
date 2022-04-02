#include "autogen_prototype_example_comp.hpp"
#include "../../sandbox/systems/examplesystem.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<::example_comp>(const ::example_comp& obj)
    {
        prototype prot;
        prot.typeId = typeHash<::example_comp>();
        prot.typeName = "::example_comp";
        {
            static const reflectable_attribute reflectable_attr{};
            prot.attributes.push_back(std::cref(reflectable_attr));
        }
        prot.members.emplace("value", member_value("value", primitive_value{typeHash(obj.value), std::any(obj.value)}));
        return prot;
    }
}
