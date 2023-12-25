/*
	"ESB" is designed to create external native components for the 1C(tm) program.
	Copyright � 2023 IntUnsigned	(v8classes@gmail.com)
	"ESB" is free software under LGPLv2.1 license with essential restriction about binary part (view esb-license.txt)
	"ESB" IS DISTRIBUTED "AS-IS" WITHOUT ANY, EVEN THE IMPLIED, WARRANTY. IN NO EVENT SHALL THE AUTHORS OR THE COPYRIGHT OWNER BE LIABLE FOR ANY CLAIM,
	DAMAGES OR OTHER LIABILITY ARISING OUT OF THE USE OF THIS SOFTWARE.
	THE AUTHOR DOES NOT GIVE ANY GUARANTEES AND DOES NOT BEAR ANY RESPONSIBILITY REGARDING THE USE OF 1C(tm) COMPONENTS PRODUCED USING THE ESB LIBRARY IN THE 1C(tm) ENVIRONMENT.
*/

#pragma once
#ifndef ESB_ES1ADDIN_H_
#define ESB_ES1ADDIN_H_

#include "esb_config.h"


// ��� �� _tVariant (���� DATE) ����� ������������� ���������� wtypes, ��� DATE ��������� �� double. � ������ WIN32_LEAN_AND_MEAN ��� �� ������������ � esb_core
#define COM_NO_WINDOWS_H
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <wtypes.h>
//����� �� types.h � ������ WIN32_LEAN_AND_MEAN �� ����� ��� �� ��� windows
#ifndef _WINDOWS
#define _WINDOWS
#endif // !1


// � 1� ������� �������� �� ����� � �� ������. ��� � �.�. � �� ��� ��������, ������� ����� ��-����������� ��� ������ ����� � ��� ���������.
#if ESB_VER < ESB_VER_v8311
#include "es1addin_v8300/ComponentBase.h"
#include "es1addin_v8300/IMemoryManager.h"
#include "es1addin_v8300/AddInDefBase.h"
#elif defined ESB_VER_v8311
#include "es1addin_v8311/ComponentBase.h"
#include "es1addin_v8311/IMemoryManager.h"
#include "es1addin_v8311/AddInDefBase.h"
static_assert(std::is_same_v<WCHAR_T, esb::strchar_t>, "conflict esb version and 1C version");
#endif

#include <clocale>		//��� SetLocale




template<typename WCHAR_T_TYPE_>
void SetAddinLocale(int category_, const WCHAR_T_TYPE_* locale_name_);
template<>
inline void SetAddinLocale(int category_, const wchar_t* locale_name_) {
	_wsetlocale(category_, locale_name_);
}
template<>
inline void SetAddinLocale(int category_, const char16_t* locale_name_) {
	static_assert(sizeof(wchar_t) == sizeof(char16_t));
	_wsetlocale(category_, (wchar_t*)locale_name_);
}

//TODO linux etc..
//inline void SetAddinLocale(int category_, const WCHAR_T* loc_) {
//#if !defined( __linux__ ) && !defined(__APPLE__)
//	static_assert(sizeof(wchar_t) == sizeof(char16_t));
//	_wsetlocale(category_, (wchar_t*)loc_);
//#else
//	//TODO
//#	error not-implemented!
//	//We convert in char* char_locale
//	//also we establish locale
//	//setlocale(LC_ALL, char_locale);
//#endif
//}

#endif	//ESB_ES1ADDIN_H_

