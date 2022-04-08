#include "autogen_prototype_filesystem_traits.hpp"
#include "../../core/filesystem/detail/traits.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::core::filesystem::filesystem_traits>(const legion::core::filesystem::filesystem_traits& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::core::filesystem::filesystem_traits>();
        prot.typeName = "legion::core::filesystem::filesystem_traits";
        return prot;
    }
}
