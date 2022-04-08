#include "autogen_reflector_inputmap.hpp"
#include "../../application/input/inputmap.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::application::inputmap>(legion::application::inputmap& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::application::inputmap>();
        refl.typeName = "legion::application::inputmap";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::application::inputmap>(const legion::application::inputmap& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::application::inputmap>();
        refl.typeName = "legion::application::inputmap";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
