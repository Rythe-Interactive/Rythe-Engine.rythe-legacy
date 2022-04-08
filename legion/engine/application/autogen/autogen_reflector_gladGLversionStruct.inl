#include "autogen_reflector_gladGLversionStruct.hpp"
#include "../../application/context/detail/glad/glad.h"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<::gladGLversionStruct>(::gladGLversionStruct& obj)
    {
        reflector refl;
        refl.typeId = typeHash<::gladGLversionStruct>();
        refl.typeName = "gladGLversionStruct";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const ::gladGLversionStruct>(const ::gladGLversionStruct& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<::gladGLversionStruct>();
        refl.typeName = "gladGLversionStruct";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
