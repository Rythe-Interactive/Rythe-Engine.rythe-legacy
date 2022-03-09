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

        const math::vec3& getOffset() const  { return m_positionOffset; }
        const math::quat& getRotationOffset() const { return m_rotationOffset; }
        
    protected:

        ColliderData(bool isRead,const math::vec3& offset,const math::quat& rotation)
            : m_isRead(isRead),m_positionOffset(offset),m_rotationOffset(rotation) { }

        math::quat m_rotationOffset{ math::identity<math::quat>()};
        math::vec3 m_positionOffset{0};
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

        ConvexColliderData(const math::vec3& offset,const math::quat& rotation, const math::vec3& boxExtents,
            const std::bitset < physics_component_flag::pc_max>* modificationBitset);

        inline convex_type getConvexType() const { return m_convexType; }

        inline const math::vec3& getBoxExtents() const { return m_boxExtents; }

    private:

        math::vec3 m_boxExtents{0.0f};
        convex_type m_convexType = convex_type::not_set;
        const std::bitset < physics_component_flag::pc_max>* m_bitsetPtr;
    };
}

