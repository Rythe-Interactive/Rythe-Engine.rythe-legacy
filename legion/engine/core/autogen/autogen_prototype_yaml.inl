#include "autogen_prototype_yaml.hpp"
#include "../../core/serialization/serializer_views/yaml.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::core::serialization::yaml>(const legion::core::serialization::yaml& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::core::serialization::yaml>();
        prot.typeName = "legion::core::serialization::yaml";
        return prot;
    }
}
