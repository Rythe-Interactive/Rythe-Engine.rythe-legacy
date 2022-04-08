#include "autogen_prototype_audio_source.hpp"
#include "../../audio/components/audio_source.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::audio::audio_source>(const legion::audio::audio_source& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::audio::audio_source>();
        prot.typeName = "legion::audio::audio_source";
        return prot;
    }
}
