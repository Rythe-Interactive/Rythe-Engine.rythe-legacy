#include "autogen_prototype_audio_segment.hpp"
#include "../../audio/data/audio_segment.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::audio::audio_segment>(const legion::audio::audio_segment& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::audio::audio_segment>();
        prot.typeName = "legion::audio::audio_segment";
        return prot;
    }
}
