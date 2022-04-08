#include "autogen_prototype_file_traits.hpp"
#include "../../core/filesystem/detail/traits.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::core::filesystem::file_traits>(const legion::core::filesystem::file_traits& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::core::filesystem::file_traits>();
        prot.typeName = "legion::core::filesystem::file_traits";
        return prot;
    }
}
