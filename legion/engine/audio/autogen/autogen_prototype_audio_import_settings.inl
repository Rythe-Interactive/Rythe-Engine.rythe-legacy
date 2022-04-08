#include "autogen_prototype_audio_import_settings.hpp"
#include "../../audio/data/audio_segment.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::audio::audio_import_settings>(const legion::audio::audio_import_settings& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::audio::audio_import_settings>();
        prot.typeName = "legion::audio::audio_import_settings";
        return prot;
    }
}
