#pragma once
#ifndef __ESBLDR_H__
#define __ESBLDR_H__

// для их _tVariant (поле DATE) нужно дополнительно подключить wtypes, где DATE определен ка double. в режиме WIN32_LEAN_AND_MEAN это не подключается в esb_core
#include <wtypes.h>
//также их types.h в режиме WIN32_LEAN_AND_MEAN не видит что он под windows
#ifndef _WINDOWS
#define _WINDOWS
#endif // !1


// В 1С хедерах варнинги не давил и не правил. они в т.ч. и на гну завязаны, поэтому чтобы по-нормальному это делать нужно и там проверять.
#include "es1addin/ComponentBase.h"
#include "es1addin/IMemoryManager.h"
#include "es1addin/AddInDefBase.h"

#include <clocale>		//для SetLocale

#include "esb_core.h"
//-------------------------------------------------------------------------------------------------------------------------

#define WCHAR_T_STRING(WSTR_)		L ## WSTR_


template<typename WCHAR_T_TYPE_>
bool alloc_WCHAR_T(WCHAR_T_TYPE_*& target_, const std::wstring_view& source_, IMemoryManager& allocator_);

template<>
inline bool alloc_WCHAR_T(wchar_t*& target_, const std::wstring_view& source_, IMemoryManager& allocator_) {
	size_t source_len	= source_.length();
	size_t source_cb	= sizeof(wchar_t) * source_len;
	
	if (allocator_.AllocMemory((void**)&target_, source_cb + sizeof(wchar_t)) && target_) {	//+ L'\0'
		std::memcpy(target_, source_.data(), source_cb);
		target_[source_len] = L'\0';
		return true;
	}
	return false;
}

template<typename WCHAR_T_TYPE_>
WCHAR_T_TYPE_* alloc_WCHAR_T(const std::wstring_view& source_, IMemoryManager& allocator_);
template<>
inline wchar_t* alloc_WCHAR_T<wchar_t>(const std::wstring_view& source_, IMemoryManager& allocator_) {
	wchar_t* buffer = nullptr;
	if(alloc_WCHAR_T(buffer, source_, allocator_))
		return buffer;
	else
		return nullptr;
}




template<typename WCHAR_T_TYPE_>
void setlocale_from_WCHAR_T(int category_, const WCHAR_T_TYPE_* locale_name_);
template<>
void setlocale_from_WCHAR_T(int category_, const wchar_t* locale_name_) {
	_wsetlocale(category_, locale_name_);
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
	static const std::wstring_view	ComponentName_;
	

//ТУДУ: Все очень очень примитивно. По хорошему нужно делать дисп-интерфейс как для есб.
ESB_WARNING_SUPRESS(ESB_WARN_NO_DEFAULT_CTOR  ESB_WARN_NO_OPERATOR_ASSIGN_ANY)
	struct Meth {
		using Fn = bool(void);
		const wchar_t*	name_;
		const wchar_t*	code_;
		Fn&				func_;
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
	
	static constexpr Meth	Meth_[]		=	{	{L"Register",		L"REGISTER",		esb::AddinObject_Register },
												{L"RegisterAsSCOM", L"REGISTERASSCOM",	RegisterAsSCOM },
												{L"Unregister",		L"UNREGISTER",		esb::AddinObject_Revoke}	};
	static constexpr size_t MethSize_	= std::size(Meth_);
	static bool is_meth_ok(int meth_) {
		return (meth_ >= 0 && (unsigned)meth_ < MethSize_); 
	}
	struct MethComparator {
		constexpr int operator()(const Meth& a, const wchar_t* b) {
			return cstr_compare_nocase_xxcase(a.code_, b);
		}
	};
	static constexpr int meth_find(const WCHAR_T* name_) {
		esb::dispix_t ix = array_find_binary(Meth_, name_, MethComparator{}, ARRAY_FIND_BINARY_NOT_FOUND);
		return (ix == ARRAY_FIND_BINARY_NOT_FOUND) ? -1 : esb::as_dispid(ix);
	}


	static EsbComponent		Instance_;

	friend AppCapabilities	SetPlatformCapabilities(const AppCapabilities capabilities_);
	static AppCapabilities	setAppCapabilities(AppCapabilities capabilities_)	{ AppCapabilities_ = capabilities_; return eAppCapabilitiesLast; }
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
	virtual void ADDIN_API SetLocale(const WCHAR_T* loc_) override final							{ setlocale_from_WCHAR_T(LC_ALL, loc_);	}
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
public:
	static void DestroyInstance() {}
};

#endif //__ESBLDR_H__