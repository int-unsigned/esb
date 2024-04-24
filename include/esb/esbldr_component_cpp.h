#pragma once


#ifndef NDEBUG
// в режиме дебаг ассерты есбхлп передаются на этот esb_assert_handler
ESB_NORETURN void esb_assert_handler(uint32_t line_) {
	//												 4294967295
	wchar_t mbuf[] = L"Unexpected in esbhlp, line: ""          ";
	wchar_t* p = mbuf + std::size(mbuf) - 1;
	do {
		*--p = L'0' + line_ % 10;
		line_ /= 10;
	} while (line_ != 0);

	MessageBoxW(NULL, mbuf, L"Abort execution!", MB_TASKMODAL | MB_ICONSTOP | MB_OK | MB_SETFOREGROUND);
	// Тут как-бы надо обрушить все т.к. дальнейшее бессмысленно (std::terminate и аналоги), но корректно обрушить 1С из длл не получается
	// Что-то у нее внутрях сопротивляется - установлен terminate_handler, который пытается сделать "дамп памяти для анализа" и рушится сам.
	// Поэтому просто валимся в отладчик. Ремить его бессмысленно, т.к. при компиляции esbhlp плюсы знали что функция NORETURN и дополнительно
	// от греха насовали int3 после каждого ее вызова.
	__debugbreak();
}
#endif


//1C Компонента
//---------------------------------------------------------------------------
#include "esbldr.h"
const esb::string_view_t	EsbComponent::ComponentName_	= ESB_T("EsbComponent");
const WCHAR_T				EsbComponent::AddinName_[]		= ESB_T("1");		// L"esb";
IAddInDefBase*				EsbComponent::AppConnect_		= nullptr;
IMemoryManager*				EsbComponent::AppMemoryManager_ = nullptr;
AppCapabilities				EsbComponent::AppCapabilities_	= AppCapabilities::eAppCapabilitiesInvalid;
EsbComponent				EsbComponent::Instance_{};

//---------------------------------------------------------------------------//
AppCapabilities SetPlatformCapabilities(const AppCapabilities capabilities_) {
	return EsbComponent::setAppCapabilities(capabilities_);
}
const WCHAR_T* GetClassNames() {
	return EsbComponent::getAddinName();
}
long GetClassObject(const WCHAR_T* name_, IComponentBase** ppComponent) {
	if (ppComponent && *ppComponent==NULL && name_ && name_[0] == L'1' && name_[1] == L'\0') {
		*ppComponent = &EsbComponent::getInstance();
		return 1;
	}
	return 0;
}
long DestroyObject(IComponentBase** ppComponent) {
	if (ppComponent && *ppComponent && *ppComponent == &EsbComponent::getInstance()) {
		EsbComponent* pComponent = static_cast<EsbComponent*>(*ppComponent);
		pComponent->DestroyInstance();
		*ppComponent = NULL;
		return 0;
	}
	return -1;	
}
//---------------------------------------------------------------------------

