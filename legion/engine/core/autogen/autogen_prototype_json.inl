#include "autogen_prototype_json.hpp"
#include "../../core/serialization/serializer_views/json.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::core::serialization::json>(const legion::core::serialization::json& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::core::serialization::json>();
        prot.typeName = "legion::core::serialization::json";
        return prot;
    }
}
