#include "autogen_reflector_FractureParams.hpp"
#include "../../physics/diviner/data/fractureparams.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::physics::FractureParams>(legion::physics::FractureParams& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::physics::FractureParams>();
        refl.typeName = "legion::physics::FractureParams";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::physics::FractureParams>(const legion::physics::FractureParams& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::physics::FractureParams>();
        refl.typeName = "legion::physics::FractureParams";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
