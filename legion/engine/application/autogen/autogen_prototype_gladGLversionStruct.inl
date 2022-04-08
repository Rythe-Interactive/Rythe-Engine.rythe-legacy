#include "autogen_prototype_gladGLversionStruct.hpp"
#include "../../application/context/detail/glad/glad.h"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<::gladGLversionStruct>(const ::gladGLversionStruct& obj)
    {
        prototype prot;
        prot.typeId = typeHash<::gladGLversionStruct>();
        prot.typeName = "gladGLversionStruct";
        return prot;
    }
}
