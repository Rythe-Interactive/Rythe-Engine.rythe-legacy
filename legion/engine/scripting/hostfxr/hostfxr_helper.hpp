#pragma once
#include <unordered_map>
#include <cstring>

#include <dotnet/coreclr_delegates.h>
#include <dotnet/hostfxr.h>

static constexpr int WIN_VALUE = 1;
static constexpr int LNX_VALUE = 2;


#ifdef _WIN32
#include <windows.h>
#include "linux_stubs.h"
static constexpr int OS_VALUE = WIN_VALUE;
using module_t = HMODULE;
#else
#include <dlfcn.h>
#include <limits.h>
#include <windows_stubs.h>
static constexpr int OS_VALUE = LNX_VALUE;
using module_t = void*;
#endif

namespace legion::scripting {
namespace detail
{
	inline module_t load_library(const char* path)
	{
		if constexpr (OS_VALUE == WIN_VALUE)
		{
			return ::LoadLibraryA(path);
		}
		else 
		{
			return dlopen(path, RTLD_LAZY | RTLD_LOCAL);
		}
	}
	inline void* get_proc(module_t mod, const char* name)
	{
		if constexpr (OS_VALUE == WIN_VALUE)
		{
			return ::GetProcAddress(mod, name);
		}
		else
		{
			return dlsym(mod,name);
		}
	}

	template <class T>
	struct ft_helper;

	template <class Ret,class ... Args>
	struct ft_helper<Ret(Args...)>
	{
		using type = Ret(*)(Args...);
	};
	
	class Module
	{
	public:
		Module(const char* path)
		{
			m_module = load_library(path);
		}

		template <class T>
		typename ft_helper<T>::type GetFn(const char * name)
		{
			using fn_type = typename ft_helper<T>::type;
			
			if(!m_module) fn_type(nullptr);

			else if(const auto itr = m_pointers.find(name);
					itr != m_pointers.end())
			{
				return fn_type(itr->second);
			}
			else
			{
				auto ptr = m_pointers[name] = get_proc(m_module,name);
				return fn_type(ptr);
			}
		}

		template <class T>
		T GetFnRaw(const char * name)
		{
			using fn_type = T;
			
			if(!m_module) fn_type(nullptr);

			else if(const auto itr = m_pointers.find(name);
					itr != m_pointers.end())
			{
				return fn_type(itr->second);
			}
			
			
			auto ptr = m_pointers[name] = get_proc(m_module,name);
			return fn_type(ptr);
		}

		bool isValid() const
		{
			return m_module != nullptr;
		}

	private:
		module_t m_module;
		std::unordered_map<std::string_view,void*> m_pointers;
	};

	template <size_t CurrOffs>
	struct cpy_to_buffer_helper
	{
		template <class Elem,class... Elems>
		static void cpy(unsigned char * buffer, const Elem& e, Elems&&... elems)
		{
			memcpy(buffer+CurrOffs,&e,sizeof(Elem));
			if constexpr(sizeof...(Elems) != 0)
			{
				cpy_to_buffer_helper<CurrOffs+sizeof(Elem)>::cpy(buffer,elems...);
			}
			
		}
		template <class Elem,class... Elems>
		static void cpy(unsigned char * buffer, Elem&& e, Elems&&... elems)
		{
			memcpy(buffer+CurrOffs,&e,sizeof(Elem));
			if constexpr (sizeof...(Elems) != 0)
			{
				cpy_to_buffer_helper<CurrOffs+sizeof(Elem)>::cpy(buffer,elems...);
			}
		}
	};
	
}



class Assembly
{
public:

    Assembly(nullptr_t) : m_func(nullptr){}
	Assembly(component_entry_point_fn fn) : m_func(fn){}
	template <class Ret,class... Args>
	Ret invoke(Args&&... args)
	{
		if(m_func == nullptr)
		{
			throw std::exception("Assembly not loaded!");
		}
		using fn_type = Ret (CORECLR_DELEGATE_CALLTYPE*)(Args...);
		return fn_type(m_func)(args...);
	}

	
private:
	component_entry_point_fn m_func;
};

class HostFXRLoader
{
public:

	HostFXRLoader(const char * path) : m_module(path)
	{

		//check if module initialized successfully
		if(m_module.isValid())
		{

			//load init function
			m_init_fn = m_module
			.GetFnRaw<hostfxr_initialize_for_runtime_config_fn>
		    ("hostfxr_initialize_for_runtime_config");

			//load get delegate function
			m_get_delegate_fn = m_module
			.GetFnRaw<hostfxr_get_runtime_delegate_fn>
			("hostfxr_get_runtime_delegate");

			//load close function
			m_close_fn = m_module
			.GetFnRaw<hostfxr_close_fn>
			("hostfxr_close");
		}
	}

	void initHostFXR(const char_t* config_path)
	{
		hostfxr_handle ctx;

		//initialize hostfxr library
		const int init_rc = m_init_fn(config_path,nullptr,&ctx);
		if (init_rc != 0 || ctx == nullptr)
		{
			m_close_fn(ctx);
			return;
		}

		//load the "load assembly" delegate
		const int delegate_rc = m_get_delegate_fn
		(
			ctx,
			hdt_load_assembly_and_get_function_pointer,
			reinterpret_cast<void**>(&m_load_asm_delegate)
		);

		m_close_fn(ctx);	
	}

	Assembly loadAssembly(
			const char_t* path,
			const char_t* fully_qualified_class_name,
			const char_t* function_name,
			const char_t* fully_qualified_matching_delegate_name)
	{
		component_entry_point_fn func = nullptr;
		int rc = m_load_asm_delegate(
			path,
			fully_qualified_class_name,
			function_name,
			fully_qualified_matching_delegate_name,
			nullptr,
			reinterpret_cast<void**>(&func));

		return Assembly(func);
	}

private:

	detail::Module m_module;
	
	hostfxr_initialize_for_runtime_config_fn m_init_fn;
	hostfxr_get_runtime_delegate_fn m_get_delegate_fn;
	hostfxr_close_fn m_close_fn;
	load_assembly_and_get_function_pointer_fn m_load_asm_delegate;
};
}
