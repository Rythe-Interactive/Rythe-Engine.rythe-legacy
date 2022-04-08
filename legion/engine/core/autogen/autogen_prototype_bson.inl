#include "autogen_prototype_bson.hpp"
#include "../../core/serialization/serializer_views/bson.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::core::serialization::bson>(const legion::core::serialization::bson& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::core::serialization::bson>();
        prot.typeName = "legion::core::serialization::bson";
        return prot;
    }
}
