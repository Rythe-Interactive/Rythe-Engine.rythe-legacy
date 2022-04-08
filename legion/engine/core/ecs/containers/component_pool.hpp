#pragma once
#include <core/platform/platform.hpp>
#include <core/types/primitives.hpp>
#include <core/containers/sparse_map.hpp>
#include <core/events/events.hpp>
#include <core/types/reflector.hpp>

#include <core/ecs/containers/component_container.hpp>
#include <core/ecs/handles/entity.hpp>
#include <core/ecs/filters/filterregistry.hpp>

/**
 * @file component_pool.hpp
 */

namespace legion::core::ecs
{
    /**@struct component_pool_base
     * @brief Common base class of all component pools that allows the use of component pools without knowing a component type at compile-time.
     */
    struct component_pool_base
    {
        /**@brief Increase the capacity of the component pool to a value that's greater or equal to the passed amount. If amount is greater than the current capacity, new storage is allocated, otherwise the function does nothing.
         */
        virtual void reserve(size_type amount) LEGION_PURE;

        /**@brief Erases all components from the pool.
         */
        virtual void clear() LEGION_PURE;

        /**@brief Creates a component attached to a certain entity.
         * @param target Entity ID to create the component for.
         * @return Pointer to the created component.
         */
        virtual pointer<void> create_component(entity target) LEGION_PURE;

        /**@brief Creates a component attached to a certain entity. (uses a prototype to serialize the component)
         * @param target Entity ID to create the component for.
         * @param src Pointer to component to copy from.
         * @return Pointer to the created component.
         */
        virtual pointer<void> create_component(entity target, const prototype& src) LEGION_PURE;
        virtual pointer<void> create_component(entity target, prototype&& src) LEGION_PURE;
        virtual pointer<void> create_component(entity target, pointer<const void> src) LEGION_PURE;

        /**@brief Check if a certain entity has the type of component managed by this pool.
         * @param target Entity ID of the entity to check for.
         * @return True if the specified entity is registered with this pool, false if not.
         */
        L_NODISCARD virtual bool contains(entity target) const LEGION_PURE;

        /**@brief Fetch the component attached to a certain entity.
         * @param target Entity ID of the entity the component is attached to.
         * @return Pointer to the component.
         */
        L_NODISCARD virtual pointer<void> get_component(entity target) LEGION_PURE;
        L_NODISCARD virtual pointer<const void> get_component(entity target) const LEGION_PURE;

        L_NODISCARD virtual reflector get_component_reflector(entity target) LEGION_PURE;
        L_NODISCARD virtual const reflector get_component_reflector(entity target) const LEGION_PURE;

        /**@brief Erase a component attached to a certain entity.
         * @param target Entity ID of the entity the component is attached to.
         */
        virtual void destroy_component(entity target) LEGION_PURE;

        virtual ~component_pool_base() = default;
    };

    /**@struct component_pool
     * @brief Storage container for all components of a certain type.
     * @tparam ComponentType Type of components to handle and store.
     */
    template<typename ComponentType>
    struct component_pool : public component_pool_base
    {
        sparse_map<id_type, ComponentType> m_components{ 125 };

        /**@brief Increase the capacity of the component pool to a value that's greater or equal to the passed amount. If amount is greater than the current capacity, new storage is allocated, otherwise the function does nothing.
         */
        virtual void reserve(size_type size);

        /**@brief Erases all components from the pool.
         */
        virtual void clear();

        /**@brief Creates a component attached to a certain entity.
         * @param target Entity ID to create the component for.
         * @return Pointer to the created component.
         */
        virtual pointer<void> create_component(entity target);

        template<typename... Args>
        ComponentType& create_component(entity target, Args&&... args);
        /**@brief Creates a component attached to a certain entity. (uses a prototype to serialize the component)
         * @param target Entity ID to create the component for.
         * @param src Pointer to component to copy from.
         * @return Pointer to the created component.
         */
        virtual pointer<void> create_component(entity target, const prototype& src);
        virtual pointer<void> create_component(entity target, prototype&& src);
        virtual pointer<void> create_component(entity target, pointer<const void> src);

        /**@brief Check if a certain entity has the type of component managed by this pool.
         * @param target Entity ID of the entity to check for.
         * @return True if the specified entity is registered with this pool, false if not.
         */
        L_NODISCARD virtual bool contains(entity target) const;

        /**@brief Fetch the component attached to a certain entity.
         * @param target Entity ID of the entity the component is attached to.
         * @return Pointer to the component.
         */
        L_NODISCARD virtual pointer<void> get_component(entity target);
        L_NODISCARD virtual pointer<const void> get_component(entity target) const;

        L_NODISCARD virtual reflector get_component_reflector(entity target);
        L_NODISCARD virtual const reflector get_component_reflector(entity target) const;

        /**@brief Erase a component attached to a certain entity.
         * @param target Entity ID of the entity the component is attached to.
         */
        virtual void destroy_component(entity target);

        void fill_container(component_container<ComponentType>& container, entity_set& entities);
    };
}
