#include "autogen_prototype_channel_data.hpp"
#include "../../audio/data/importers/audio_importers.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::audio::detail::channel_data>(const legion::audio::detail::channel_data& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::audio::detail::channel_data>();
        prot.typeName = "legion::audio::detail::channel_data";
        return prot;
    }
}
