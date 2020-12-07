#pragma once

#include <core/core.hpp>
#include <scripting/providers/provider_base.hpp>

namespace legion::scripting {

    class CSharpCoreEventBusProvider : public CSharpProviderBase
    {
    public:

        CSharpCoreEventBusProvider(std::vector<std::pair<std::string,id_type>> evs) : m_registered_events(std::move(evs)){}

        bool onHostFXRRegister(HostFXRLoader* loader) override
        {
            classname = TO_CHAR_T("EventBus"); 

            engineFunction(loader, TO_CHAR_T("RegisterGetPendingDataFunc")).invoke<void>(&wrapped_get_pending_data);
            

            for (auto&[name,id_] : m_registered_events)
            {
                data = name.c_str();
                id = id_;
                engineFunction(loader, TO_CHAR_T("OnPendingData")).invoke<void>();  
            }

            return true;
        }

    private:
        static get_in wrapped_get_pending_data()
        {
            get_in ctr;
            ctr.size = strlen(data)+1+sizeof(id_type);
            char* ptr = new char[ctr.size];
            memset(ptr,0,ctr.size);
            memcpy(ptr,data,ctr.size-1-sizeof(id_type));
            memcpy(ptr+static_cast<long long>(ctr.size-sizeof(id_type)),&id,sizeof(id_type));

            ctr.ptr = ptr;
            return ctr;
        }

        std::vector<std::pair<std::string,id_type>> m_registered_events;
        inline static const char * data;
        inline static id_type id;
        
    };
}
