#include "autogen_reflector_audio_listener.hpp"
#include "../../audio/components/audio_listener.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::audio::audio_listener>(legion::audio::audio_listener& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::audio::audio_listener>();
        refl.typeName = "legion::audio::audio_listener";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::audio::audio_listener>(const legion::audio::audio_listener& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::audio::audio_listener>();
        refl.typeName = "legion::audio::audio_listener";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
