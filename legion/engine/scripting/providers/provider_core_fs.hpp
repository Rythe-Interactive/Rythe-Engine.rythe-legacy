#pragma once
#include <core/core.hpp>
#include <core/common/result.hpp>
#include <scripting/data/get_in.hpp>
#include <scripting/providers/provider_base.hpp>

namespace legion::scripting {

    class CSharpCoreFilesystemProvider : public CSharpProviderBase
    {
    public:
        bool onHostFXRRegister(HostFXRLoader* loader) override
        {
            classname = TO_CHAR_T("AssetView");
            auto register_filesystem_functions =  engineFunction(loader, TO_CHAR_T("RegisterReadFileFns"));

            register_filesystem_functions.invoke<void>
            (
                &wrapped_readfile,
                &wrapped_clean_array,
                &wrapped_ls
            );
            return true;
        }

    protected:


        static get_in wrapped_readfile(const char* str)
        {
            get_in ret;
            auto result = fs::view(str).get();
            if(result != core::common::valid)
            {
                return {nullptr,0};
            }
            auto buf = result.decay();

            ret.size = buf.size();
            ret.ptr = new byte[ret.size];
            memcpy(ret.ptr,buf.data(),ret.size);
            return ret;
        }

        //TODO(algo-ryth-mix) this is hella broken
        //for some reason the identifier of the found 
        static  const char* wrapped_ls(const char* path)
        {   
            auto result = fs::view(path).ls_strings();
            std::string retval = "";
            if(result == core::common::valid)
            {
                for(auto & v: result.decay())
                {
                    retval += v + "\n";
                }
            }
            char * buffer = new char[retval.size()+1];
            memset(buffer + retval.size(),0,1);
            memcpy(buffer,retval.data(),retval.size());
            return buffer;
        }

        static void wrapped_clean_array(byte* ptr)
        {
            delete[] ptr;
        }

    };
}
