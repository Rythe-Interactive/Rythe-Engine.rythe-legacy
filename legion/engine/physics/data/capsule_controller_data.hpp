#pragma once
#include <core/core.hpp>
#include <physics/data/component_flags.hpp>
#include <physics/data/controller_presets.hpp>
#include <bitset>

namespace legion::physics
{
    struct controller_preset
    {
        size_type hash;

        union preset_specifics
        {
            constexpr preset_specifics() : dummy{ false } {}
            bool dummy;

            gravity_preset gravityPreset;
            rigidbody_force_feedback forcePreset;
        };

        preset_specifics specifics;
    };

    class CapsuleControllerData
    {
    public:

        L_ALWAYS_INLINE void moveTo(const math::vec3& displacement)
        {
            m_modificationFlags.set(capsule_character_flag::cc_move_to, true);
            m_currentDisplacement += displacement;
        }

        L_ALWAYS_INLINE void setRadius(float radius)
        {
            m_modificationFlags.set(capsule_character_flag::cc_set_radius, true);
            m_radius = radius;
        }

        L_ALWAYS_INLINE float getRadius() const noexcept { return m_radius; }

        L_ALWAYS_INLINE void setHeight(float height)
        {
            m_modificationFlags.set(capsule_character_flag::cc_set_height, true);
            m_height = height;
        }

        L_ALWAYS_INLINE float getHeight() const noexcept { return m_height; }

        L_ALWAYS_INLINE const math::vec3& getCurrentDisplacement() const noexcept { return m_currentDisplacement; }

        L_ALWAYS_INLINE void resetDisplacement() { m_currentDisplacement = math::vec3(0); }

        L_ALWAYS_INLINE const std::bitset<capsule_character_flag::cc_max>& getModificationFlags() const noexcept { return m_modificationFlags; }

        L_ALWAYS_INLINE void resetModificationFlags() { m_modificationFlags.reset(); }

        template<class preset>
        pointer<const preset> getPreset() const noexcept
        {
            static size_type searchHash = typeHash<preset>();

            for (const controller_preset& contPreset : m_presets)
            {
                if (searchHash == contPreset.hash)
                {
                    const void* specificsPtr = &contPreset.specifics;
                    return { static_cast<const preset*>(specificsPtr) };
                }
            }

            return { nullptr };
        }

        template<class preset>
        pointer<preset> getPreset() noexcept
        {
            static size_type searchHash = typeHash<preset>();

            for (controller_preset& contPreset : m_presets)
            {
                if (searchHash == contPreset.hash)
                {
                    void* specificsPtr = &contPreset.specifics;
                    return { static_cast<preset*>(specificsPtr) };
                }
            }

            return { nullptr };
        }

        template<class preset>
        pointer<preset> addPreset(preset& presetSpecifics)
        {
            static_assert(std::is_standard_layout<preset>());
            static size_type searchHash = typeHash<preset>();

            for (controller_preset& contPreset : m_presets)
            {
                if (searchHash == contPreset.hash)
                {
                    void* specificsPtr = &contPreset.specifics;
                    return { static_cast<preset*>(specificsPtr) };
                }
            }

            controller_preset newPreset;
            newPreset.hash = searchHash;

            static_assert(std::is_same<gravity_preset, preset>::value
                || std::is_same<rigidbody_force_feedback, preset>::value, "The template parameter used to call CapsuleControllerData::addPreset is not a recognized preset");

            if constexpr (std::is_same<gravity_preset, preset>::value)
                newPreset.specifics.gravityPreset = presetSpecifics;
            else
                newPreset.specifics.forcePreset = presetSpecifics;

            m_presets.push_back(newPreset);

            void* specificsPtr = &m_presets[m_presets.size() - 1].specifics;
            return { static_cast<preset*>(specificsPtr) };
        }

        L_ALWAYS_INLINE std::vector<controller_preset>& getControllerPresets() noexcept { return m_presets; }

        L_ALWAYS_INLINE const std::vector<controller_preset>& getControllerPresets() const noexcept { return m_presets; }

    private:

        math::vec3 m_currentDisplacement;
        math::vec3 m_currentlTeleportation;

        std::vector<controller_preset> m_presets;

        std::bitset<capsule_character_flag::cc_max> m_modificationFlags;

        float m_radius = 1.0f;
        float m_height = 2.0f;;
        float m_speed = 0.0f;
    };
}
