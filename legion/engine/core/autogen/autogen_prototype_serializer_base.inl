#include "autogen_prototype_serializer_base.hpp"
#include "../../core/serialization/serializer/serializer_base.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::core::serialization::serializer_base>(const legion::core::serialization::serializer_base& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::core::serialization::serializer_base>();
        prot.typeName = "legion::core::serialization::serializer_base";
        return prot;
    }
}
