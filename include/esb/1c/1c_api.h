/*
	"ESB" is designed to create external native components for the 1C(tm) program.
	Copyright � 2023 IntUnsigned	(v8classes@gmail.com)
	"ESB" is free software under LGPLv2.1 license with essential restriction about binary part (view esb-license.txt)
	"ESB" IS DISTRIBUTED "AS-IS" WITHOUT ANY, EVEN THE IMPLIED, WARRANTY. IN NO EVENT SHALL THE AUTHORS OR THE COPYRIGHT OWNER BE LIABLE FOR ANY CLAIM,
	DAMAGES OR OTHER LIABILITY ARISING OUT OF THE USE OF THIS SOFTWARE.
	THE AUTHOR DOES NOT GIVE ANY GUARANTEES AND DOES NOT BEAR ANY RESPONSIBILITY REGARDING THE USE OF 1C(tm) COMPONENTS PRODUCED USING THE ESB LIBRARY IN THE 1C(tm) ENVIRONMENT.
*/


#pragma once
#ifndef ESB_1�_MODULE_API_H_
#define ESB_1�_MODULE_API_H_

#include "esb/esb_base.h"

#if ESB_API_INCLUDE_NATIVE
#	include "esb/1c/1c_api_native.h"
#endif


#include ESB_INCLUDE_1C_MODULE(es1_api.h)


//NOTE: �� ������ � ������ �������� ��������� ��-��������� ��������������� ��� DefUndef<...> � ��� ������ � ������ ������� ��� ���������.
//		��!! ��� ��������� ������� ��������� ��-��������� ����� ��������� ���������. ������:
//		������� ��������(������_ ��� ������, ���������������_ ��� ������, �����������������_ ��� �����������������, ����������������_ ��� �����, ��������������_ ��� �����) ���������� �����
//		inline Numeric StrFind(ConstPara<String> Line_, ConstPara<String> SearchSubstring_, ConstPara<SearchDirection> SearchDirection_ = SearchDirectionEnum::FromBegin, ConstPara<Numeric> StartIndex_ = DefUndef<Numeric>, ConstPara<Numeric> EntryNumber_ = Numeric::Value_1_) { ESB_INVOKE_API_FUNC5(Numeric, core, 4, Line_, SearchSubstring_, SearchDirection_, StartIndex_, EntryNumber_); }
//		�.�. ������ SearchDirection_ = SearchDirectionEnum::FromBegin, � �� SearchDirection_ = DefUndef<SearchDirection> � �.�.
//		!!�������� ��� ������� ������� ������ ����� ��� ���������, �������� ������, ������ � ���� � ������� �������. ������������ ��������� ���!!
//���..
//
namespace esb //API part
{
#if ESB_API_PROVIDER_INCLUDE(core83)
#if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
	template<EsbClassConcept ... EsbClassArgsT>
	requires (sizeof...(EsbClassArgsT) <= 10)
	inline String StrTemplate(ConstParam<String> Template_, ConstParam< EsbClassArgsT>... TemplateArguments1_10_) { ESB_INVOKE_API_FUNC1X(String, core83, StrTemplate, Template_, TemplateArguments1_10_); }
#endif
#if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
	template<EsbClassConcept ... EsbClassArgsT>
	requires (sizeof...(EsbClassArgsT) <= 10)
	inline ������ ���������(����������<������> ������_, ����������<EsbClassArgsT>... ����������������1_10_) { ESB_INVOKE_API_FUNC1X(String, core83, StrTemplate, ������_, ����������������1_10_); }
#endif

#endif //
	// <<< end from providers manual -----------------------------------------------------
}	// namespace esb (api part)


#endif	// ESB_1�_MODULE_API_H_