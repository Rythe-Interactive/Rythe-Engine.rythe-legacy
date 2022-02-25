#include <core/particles/particlebuffer.hpp>

namespace legion::core
{
    void particle_buffer_base::swap(size_type idx1, size_type idx2)
    {
        std::iter_swap(buffer.begin() + idx1, buffer.begin() + idx2);
    }

    size_type particle_buffer_base::size()
    {
        return buffer.size();
    }
}
