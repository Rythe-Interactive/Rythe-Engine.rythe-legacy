#include "autogen_prototype_audio_listener.hpp"
#include "../../audio/components/audio_listener.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::audio::audio_listener>(const legion::audio::audio_listener& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::audio::audio_listener>();
        prot.typeName = "legion::audio::audio_listener";
        return prot;
    }
}
