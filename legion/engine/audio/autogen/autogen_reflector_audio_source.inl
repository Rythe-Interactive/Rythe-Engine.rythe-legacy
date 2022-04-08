#include "autogen_reflector_audio_source.hpp"
#include "../../audio/components/audio_source.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::audio::audio_source>(legion::audio::audio_source& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::audio::audio_source>();
        refl.typeName = "legion::audio::audio_source";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::audio::audio_source>(const legion::audio::audio_source& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::audio::audio_source>();
        refl.typeName = "legion::audio::audio_source";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
