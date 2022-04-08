#include "autogen_reflector_channel_data.hpp"
#include "../../audio/data/importers/audio_importers.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::audio::detail::channel_data>(legion::audio::detail::channel_data& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::audio::detail::channel_data>();
        refl.typeName = "legion::audio::detail::channel_data";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::audio::detail::channel_data>(const legion::audio::detail::channel_data& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::audio::detail::channel_data>();
        refl.typeName = "legion::audio::detail::channel_data";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
