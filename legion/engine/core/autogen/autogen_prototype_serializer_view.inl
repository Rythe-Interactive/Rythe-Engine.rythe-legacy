#include "autogen_prototype_serializer_view.hpp"
#include "../../core/serialization/serializer_views/serializer_view.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::core::serialization::serializer_view>(const legion::core::serialization::serializer_view& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::core::serialization::serializer_view>();
        prot.typeName = "legion::core::serialization::serializer_view";
        return prot;
    }
}
