#pragma once
#include <core/core.hpp>
#include <scripting/data/get_in.hpp>
#include <scripting/providers/provider_base.hpp>


namespace legion::scripting{
    class CSharpCoreEcsProvider : public CSharpProviderBase
    {
    public:

        static ecs::EcsRegistry* registry;

        bool onHostFXRRegister(HostFXRLoader* loader) override
        {
            classname = TO_CHAR_T("Entity");
            auto register_entity_functions = engineFunction(loader,TO_CHAR_T("RegisterEntityFns"));
            register_entity_functions.invoke<void>
            (
                &wrapped_validate_entity,
                &wrapped_get_parent,
                &wrapped_get_child_count,
                &wrapped_get_child,
                &wrapped_compare_tag,
                &wrapped_get_entities,
                &wrapped_create_entity
            );
            return true;

        }

    protected:

        static bool wrapped_validate_entity(id_type entity)
        {
            return ecs::entity_handle(entity).valid();
        }

        static id_type wrapped_get_parent(id_type entity)
        {
            return ecs::entity_handle(entity).get_parent().get_id();
        }

        static size_type wrapped_get_child_count(id_type entity)
        {
            return ecs::entity_handle(entity).child_count();
        }

        static id_type wrapped_get_child(id_type entity, index_type index)
        {
            return ecs::entity_handle(entity).get_child(index).get_id();
        }

        static bool wrapped_compare_tag(id_type entity, const char * tag_name)
        {
            ecs::entity_handle ent(entity);

            if(ent.has_component<tag>())
            {
                const tag t = ent.get_component_handle<tag>().read();
                return compare(t, tag(tag_name));
            }
            return false;
        }

        static get_in wrapped_get_entities()
        {
            std::pair<ecs::entity_set&, async::readonly_rw_spinlock&> refs = registry->getEntities();
            ecs::entity_set& set = std::get<0>(refs);
            async::readonly_rw_spinlock& lock = std::get<1>(refs);

            id_type* buffer;
            index_type i = 0;
            size_type size;
            {
                async::readonly_guard guard(lock);
                size = set.size();
                buffer = new id_type[size];
                for(ecs::entity_handle& entity : set)
                {
                    buffer[i] = entity.get_id();
                    ++i;
                }
            }
            return get_in{buffer,size * sizeof(id_type)};
        }
        static id_type wrapped_create_entity()
        {
            return registry->createEntity().get_id();
        }

    };
}

