#pragma once

#include <core/core.hpp>
#include <scripting/providers/provider_base.hpp>

namespace legion::scripting {

    class CSharpCoreEventBusProvider : public CSharpProviderBase
    {
    public:

        CSharpCoreEventBusProvider(std::vector<std::pair<std::string, id_type>> evs,events::EventBus* evbus) : m_registered_events(std::move(evs)) {}

        bool onHostFXRRegister(HostFXRLoader* loader) override
        {
            m_loader = loader;
            classname = TO_CHAR_T("EventBus");

            engineFunction(loader,TO_CHAR_T("RegisterEmitEventImplFn")).invoke<void>(&wrapped_emit);
            engineFunction(loader, TO_CHAR_T("RegisterGetPendingDataFunc")).invoke<void>(&wrapped_get_pending_data);
            m_pending_data_fn = engineFunction(loader, TO_CHAR_T("OnPendingData"));

            for (auto& [name, id_] : m_registered_events)
            {
                m_data = name.c_str();
                m_id = id_;
                m_payload_type = 0xFF;
                m_pending_data_fn.invoke<void>();
            }

            return true;
        }

        static void onEvent(events::event_base* base)
        {
            m_event = base;
            m_id = base->get_id();
            m_payload_type = 0;
            m_pending_data_fn.invoke<void>();
        }
    private:

        struct Payload {
            size_type type;
            size_type payload_length;
            void* ptr;
        };


        struct MessageRegisterEvent
        {
            id_type id;
            const char* msg;
        };

        struct MessageEmitEvent
        {
            id_type id;
            const void* data;
        };


        static get_in wrapped_get_pending_data()
        {
            Payload* payload = nullptr;

            if(m_payload_type == 0)
            {
                payload= new Payload
                {
                    m_payload_type,
                    sizeof(MessageEmitEvent),
                    new MessageEmitEvent
                    {
                        m_id,
                        m_event
                    }
                };
            }
            if(m_payload_type == 0xFF)
            {
                payload= new Payload
                {
                    m_payload_type,
                    sizeof(MessageRegisterEvent),
                    new MessageRegisterEvent
                    {
                        m_id,
                        m_data
                    }
                };
            }
            return get_in{payload,sizeof(Payload)};
        }

        static void wrapped_emit(void* event, id_type id)
        {
            

        }

        std::vector<std::pair<std::string, id_type>> m_registered_events;

        inline static Assembly m_pending_data_fn = nullptr;
        inline static const void* m_event;
        inline static const char* m_data;
        inline static id_type m_id;
        inline static int m_payload_type;
        inline static HostFXRLoader* m_loader;
        inline static events::EventBus* m_eventBus;

    };
}
