#include "autogen_reflector_audio_import_settings.hpp"
#include "../../audio/data/audio_segment.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::audio::audio_import_settings>(legion::audio::audio_import_settings& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::audio::audio_import_settings>();
        refl.typeName = "legion::audio::audio_import_settings";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::audio::audio_import_settings>(const legion::audio::audio_import_settings& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::audio::audio_import_settings>();
        refl.typeName = "legion::audio::audio_import_settings";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
