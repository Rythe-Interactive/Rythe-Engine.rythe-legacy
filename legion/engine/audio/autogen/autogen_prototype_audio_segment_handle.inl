#include "autogen_prototype_audio_segment_handle.hpp"
#include "../../audio/data/audio_segment.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::audio::audio_segment_handle>(const legion::audio::audio_segment_handle& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::audio::audio_segment_handle>();
        prot.typeName = "legion::audio::audio_segment_handle";
        return prot;
    }
}
