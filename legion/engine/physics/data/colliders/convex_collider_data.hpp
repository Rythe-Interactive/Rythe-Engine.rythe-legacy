#pragma once
#include <core/core.hpp>
#include <physics/data/component_flags.hpp>
#include <bitset>

namespace legion::physics
{
    class ColliderData
    {
    public:

        inline void setDataRead(bool isRead) { m_isRead = isRead; }
        inline bool isDataRead() const { return m_isRead; }
        
    protected:

        ColliderData(bool isRead) : m_isRead(isRead) { }

        bool m_isRead = false;
    };

    enum class convex_type
    {
        box,
        quickhull,
        not_set
    };

    class ConvexColliderData : public ColliderData
    {
    public:

        ConvexColliderData(const math::vec3& boxExtents,
            const std::bitset < physics_component_flag::pc_max>* modificationBitset);

        inline convex_type getConvexType() const { return m_convexType; }

        inline const math::vec3& getBoxExtents() const { return m_boxExtents; }

    private:

        math::vec3 m_boxExtents{0.0f};
        convex_type m_convexType = convex_type::not_set;
        const std::bitset < physics_component_flag::pc_max>* m_bitsetPtr;
    };
}

