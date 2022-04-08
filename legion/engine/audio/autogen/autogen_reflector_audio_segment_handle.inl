#include "autogen_reflector_audio_segment_handle.hpp"
#include "../../audio/data/audio_segment.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::audio::audio_segment_handle>(legion::audio::audio_segment_handle& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::audio::audio_segment_handle>();
        refl.typeName = "legion::audio::audio_segment_handle";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::audio::audio_segment_handle>(const legion::audio::audio_segment_handle& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::audio::audio_segment_handle>();
        refl.typeName = "legion::audio::audio_segment_handle";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
