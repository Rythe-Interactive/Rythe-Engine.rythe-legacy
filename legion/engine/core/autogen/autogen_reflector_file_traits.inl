#include "autogen_reflector_file_traits.hpp"
#include "../../core/filesystem/detail/traits.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::core::filesystem::file_traits>(legion::core::filesystem::file_traits& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::core::filesystem::file_traits>();
        refl.typeName = "legion::core::filesystem::file_traits";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::core::filesystem::file_traits>(const legion::core::filesystem::file_traits& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::core::filesystem::file_traits>();
        refl.typeName = "legion::core::filesystem::file_traits";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
