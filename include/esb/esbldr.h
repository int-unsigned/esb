/*
	"ESB" is designed to create external native components for the 1C(tm) program.
	Copyright © 2023 IntUnsigned	(v8classes@gmail.com)
	"ESB" is free software under LGPLv2.1 license with essential restriction about binary part (view esb-license.txt)
	"ESB" IS DISTRIBUTED "AS-IS" WITHOUT ANY, EVEN THE IMPLIED, WARRANTY. IN NO EVENT SHALL THE AUTHORS OR THE COPYRIGHT OWNER BE LIABLE FOR ANY CLAIM, 
	DAMAGES OR OTHER LIABILITY ARISING OUT OF THE USE OF THIS SOFTWARE.
	THE AUTHOR DOES NOT GIVE ANY GUARANTEES AND DOES NOT BEAR ANY RESPONSIBILITY REGARDING THE USE OF 1C(tm) COMPONENTS PRODUCED USING THE ESB LIBRARY IN THE 1C(tm) ENVIRONMENT.
*/


#pragma once
#ifndef __ESBLDR_H__
#define __ESBLDR_H__

#include "es1addin.h"

#include "esb_core.h"
#include "esb_meta.h"
//-------------------------------------------------------------------------------------------------------------------------


template<typename>
struct one_es_memory_manager_size_t_extractor;

template<typename RetT, typename ClassT, typename Arg1T, typename Arg2T>
struct one_es_memory_manager_size_t_extractor<RetT(__stdcall ClassT::*)(Arg1T, Arg2T)> {
	using size_t = Arg2T;
};

using one_es_memory_manager_size_t = one_es_memory_manager_size_t_extractor<decltype(&IMemoryManager::AllocMemory)>::size_t;


template<typename WCHAR_T_TYPE_>
bool alloc_WCHAR_T(WCHAR_T_TYPE_*& target_, const esb::strview_t& source_, IMemoryManager& allocator_);

template<>
inline bool alloc_WCHAR_T(WCHAR_T*& target_, const esb::strview_t& source_, IMemoryManager& allocator_) {
	static_assert(sizeof(WCHAR_T) == sizeof(esb::strchar_t), "WCHAR_T and esb char_t not compatable");

	size_t source_len		= source_.length();
	size_t source_txt_cb	= sizeof(WCHAR_T) * source_len;
	size_t source_str_cb	= source_txt_cb + sizeof(WCHAR_T);	//+ L'\0'

	if constexpr (std::cmp_greater(std::numeric_limits<size_t>::max(), std::numeric_limits<one_es_memory_manager_size_t>::max())) {
		if (std::cmp_greater(source_str_cb, std::numeric_limits<one_es_memory_manager_size_t>::max()))
			return false;
	}

	if (allocator_.AllocMemory((void**)&target_, static_cast<one_es_memory_manager_size_t>(source_str_cb)) && target_) {
		std::memcpy(target_, source_.data(), source_txt_cb);
		target_[source_len] = ESB_T('\0');
		return true;
	}
	return false;
}

template<typename WCHAR_T_TYPE_>
WCHAR_T_TYPE_* alloc_WCHAR_T(const esb::strview_t& source_, IMemoryManager& allocator_);
template<>
inline WCHAR_T* alloc_WCHAR_T<WCHAR_T>(const esb::strview_t& source_, IMemoryManager& allocator_) {
	WCHAR_T* buffer = nullptr;
	if(alloc_WCHAR_T(buffer, source_, allocator_))
		return buffer;
	else
		return nullptr;
}





template<typename DATA_T_>
void assign(tVariant& var_, const DATA_T_& val_);
template<>
inline void assign(tVariant& var_, const bool& val_) {
	var_.bVal = val_;
	var_.vt = VTYPE_BOOL;
}


class EsbComponent: public IComponentBase
{
	static IAddInDefBase*			AppConnect_;
	static IMemoryManager*			AppMemoryManager_;
	static AppCapabilities			AppCapabilities_;

	static const WCHAR_T			AddinName_[];
	static const esb::strview_t		ComponentName_;
	
//ТУДУ: Все очень очень примитивно. По хорошему нужно делать дисп-интерфейс как для есб.
ESB_WARNING_SUPRESS(ESB_WARN_NO_DEFAULT_CTOR  ESB_WARN_NO_OPERATOR_ASSIGN_ANY)
	struct Meth {
		using Fn = bool(void);
		const esb::strchar_t*	name_;
		const esb::strchar_t*	code_;
		Fn&						func_;
	};
ESB_WARNING_RESTORE()	//ESB_WARN_NO_DEFAULT_CTOR  ESB_WARN_NO_OPERATOR_ASSIGN_ANY

	static bool RegisterAsSCOM() {
#if defined(ESB_USE_DETOUR) && ESB_USE_DETOUR
		auto x = DetourOne<GetModuleFileNameW>([]([[maybe_unused]] HMODULE hModule, LPWSTR lpOutFilenameBuf, DWORD nSizeBufInTCHARS) -> DWORD {
				DWORD dw_result = 0;
				__try { 
					dw_result = DetourOne<GetModuleFileNameW>::invoke_real(esb::GetAddinModuleHandle(), lpOutFilenameBuf, nSizeBufInTCHARS);
					__leave;
				}
				__finally	
				{};
				return dw_result;
			});
#endif
		return esb::AddinObject_RegisterAsSCOM();
		//detour will release hook here
	}
	
	static constexpr Meth	Meth_[]		=	{	{ESB_T("Register"),			ESB_T("REGISTER"),			esb::AddinObject_Register },
												{ESB_T("RegisterAsSCOM"),	ESB_T("REGISTERASSCOM"),	RegisterAsSCOM },
												{ESB_T("Unregister"),		ESB_T("UNREGISTER"),		esb::AddinObject_Revoke}	
											};
	static constexpr size_t MethSize_	= std::size(Meth_);

	static bool is_meth_ok(int meth_) {
		return (meth_ >= 0 && (unsigned)meth_ < MethSize_); 
	}
	static constexpr esb::dispid_t meth_find(const WCHAR_T* name_) {
		constexpr auto comparator = [](const Meth& a, const WCHAR_T* b) constexpr -> int {
			return afx_ustring::cstr_compare(a.code_, afx_ustring::CStrIteratorNoCase(b));
		};
		return esb::disparray_find(Meth_, name_, comparator);
	}



	static EsbComponent		Instance_;

	friend AppCapabilities	SetPlatformCapabilities(const AppCapabilities capabilities_);
	static AppCapabilities	setAppCapabilities(AppCapabilities capabilities_)	{ AppCapabilities_ = capabilities_;		return eAppCapabilitiesLast; }
public:
	IMemoryManager*			getAppMemoryManager() const							{ return AppMemoryManager_; }
	IAddInDefBase*			getAppConnect() const								{ return AppConnect_;	}
	static EsbComponent&	getInstance()										{ return Instance_; }
	static const WCHAR_T*	getAddinName()										{ return AddinName_; }
	
public:
	virtual bool ADDIN_API Init(void* pv_) override final {
		AppConnect_ = (IAddInDefBase*)pv_;
		if (!AppConnect_) return false;

		if(esb::OnComponentInitStartup())
			return true;

		// NOTE
		// если инициализация есб не удалась, то мы принудительно почистим за собой то, что получилось сделать и вернем неуспех.
		// (АПИ esbhlp устроено по принципу as-min-as-possible с целью возможной корректировки логики поведения в приложении.)
		esb::OnComponentDoneCleanup();
		return false;
	}
	virtual bool ADDIN_API setMemManager(void* pv_) override final {
		AppMemoryManager_ = (IMemoryManager*)pv_;
		return (AppMemoryManager_ != nullptr);
	}
	virtual long ADDIN_API GetInfo() override {
		// Component should put supported component technology version This component supports 2.0 version
		return 2000;
	}
	virtual void ADDIN_API Done() override final {
		esb::OnComponentDoneCleanup();
		return;
	}
	// ILanguageExtenderBase (используется при  = Новый("AddIn.EsCls.EsClsExtension"); 
	virtual bool ADDIN_API RegisterExtensionAs(WCHAR_T** ppwszExtensionNameOut) override final {
		return (ppwszExtensionNameOut && AppMemoryManager_)? alloc_WCHAR_T<WCHAR_T>(*ppwszExtensionNameOut, ComponentName_, *AppMemoryManager_) : false;
	}
	// LocaleBase
	virtual void ADDIN_API SetLocale(const WCHAR_T* loc_) override final							{ SetAddinLocale(LC_ALL, loc_);	}
	// ILanguageExtenderBase - context dummy impl as private
	long ADDIN_API GetNProps() override final														{ return 0;	}
	const WCHAR_T* ADDIN_API GetPropName(long, long) override final									{ return nullptr; }
	bool ADDIN_API IsPropReadable(const long) override final										{ return false; }
	bool ADDIN_API IsPropWritable(const long) override final										{ return false; }
	long ADDIN_API GetNMethods() override final														{ return MethSize_; }
	const WCHAR_T* ADDIN_API GetMethodName(const long meth_, const long) override final				{ 
		return (is_meth_ok(meth_) && AppMemoryManager_)? alloc_WCHAR_T<WCHAR_T>(Meth_[meth_].name_, *AppMemoryManager_)  : nullptr;
	}
	long ADDIN_API GetNParams(const long) override final											{ return 0;	}
	bool ADDIN_API HasRetVal(const long) override final												{ return true;}
	virtual bool ADDIN_API GetPropVal(const long, tVariant*) override final							{ return false; }
	virtual bool ADDIN_API SetPropVal(const long, tVariant*) override final							{ return false; }
	virtual long ADDIN_API FindProp(const WCHAR_T*) override final									{ return -1; }
	virtual long ADDIN_API FindMethod(const WCHAR_T* name_) override final							{ return meth_find( name_ ); }
	virtual bool ADDIN_API GetParamDefValue(const long, const long, tVariant*) override final		{ return false; }
	virtual bool ADDIN_API CallAsProc(const long, tVariant*, const long) override final				{ return false; }
	virtual bool ADDIN_API CallAsFunc(const long meth_, tVariant* retv_, tVariant* aargs_, const long cargs_) override final 	{
		if(!is_meth_ok(meth_) || retv_ == nullptr || aargs_ || cargs_ > 0)
			return false; 

		assign(*retv_, Meth_[(unsigned)meth_].func_());
		return true;
	}
#if ESB_VER >= ESB_VER_v8311
	void ADDIN_API SetUserInterfaceLanguageCode(const WCHAR_T* lang) override final
	{
		//m_userLang.assign(lang);
	}
#endif
public:
	static void DestroyInstance() {}
};


#endif //__ESBLDR_H__