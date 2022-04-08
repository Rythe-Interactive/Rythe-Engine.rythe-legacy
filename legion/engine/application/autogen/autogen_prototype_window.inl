#include "autogen_prototype_window.hpp"
#include "../../application/window/window.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::application::window>(const legion::application::window& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::application::window>();
        prot.typeName = "legion::application::window";
        return prot;
    }
}
