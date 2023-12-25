
/*
	"ESB" is designed to create external native components for the 1C(tm) program.
	Copyright � 2023 IntUnsigned	(v8classes@gmail.com)
	"ESB" is free software under LGPLv2.1 license with essential restriction about binary part (view esb-license.txt)
	"ESB" IS DISTRIBUTED "AS-IS" WITHOUT ANY, EVEN THE IMPLIED, WARRANTY. IN NO EVENT SHALL THE AUTHORS OR THE COPYRIGHT OWNER BE LIABLE FOR ANY CLAIM,
	DAMAGES OR OTHER LIABILITY ARISING OUT OF THE USE OF THIS SOFTWARE.
	THE AUTHOR DOES NOT GIVE ANY GUARANTEES AND DOES NOT BEAR ANY RESPONSIBILITY REGARDING THE USE OF 1C(tm) COMPONENTS PRODUCED USING THE ESB LIBRARY IN THE 1C(tm) ENVIRONMENT.
*/

#pragma once
#ifndef ESB_CORE_IFPTR_H_
#define ESB_CORE_IFPTR_H_

#include "esb_config.h"

// ��� ������ ����� IUnkown � IClassFactory (� HRESULTs)
// �� �������� ��� �� ��������� �������� windows.h ����������...
// ��������� �������
#define COM_NO_WINDOWS_H
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
// ��� ��������� � <windows.h> ��� ������ ����������� ��������
#define NOATOM
#define NOGDI
#define NOGDICAPMASKS
#define NOMETAFILE
#define NOMINMAX
#define NOMSG
#define NOOPENFILE
#define NORASTEROPS
#define NOSCROLL
#define NOSOUND
#define NOSYSMETRICS
#define NOTEXTMETRIC
#define NOWH
#define NOCOMM
#define NOKANJI
#define NOCRYPT
#define NOMCX

ESB_WARNING_SUPRESS(ESB_WARN_DEFINED_IN_MACRO)
// combaseapi ���������� "Unknwnbase.h"
#include <combaseapi.h>
//#include "Unknwnbase.h"
ESB_WARNING_RESTORE()	//ESB_WARN_DEFINED_IN_MACRO


//InterfacePtr
namespace core
{
	//TOBE:	������ ������������ esb ������� ��������� �������� COM-������������. �.�. ��������� ��� ������, ������������ ��� �������� � Addref\Release
	//		��������� ������ �� ����������.
	//		������� �������� Addref\Release �� �� �����. � ���� �� � ���, ��� ��� ������� 20 ������ ����������, � � ���, ��� esbhlp �������� � ����
	//		��������� ������� �� ���������� �� ������ ��� � ���������. (��. ���-�����������)
	//		� ����� ��������� ��������� �������� � ���������. �� �� ������������ ����������� ����� �������� � ����� �������� ������� Release �������
	//		dll �������� ���� ��� ��������� ����������. 
	//		������ ��� �������� ������ �������� OnComponentDoneCleanup, �� ����� ������ �� ��������.
	template<class InterfaceT>
	inline constexpr bool is_interface_singleton = false;


	// �� ������� CComPtr
	template <class T>
	class InterfacePtrBase
	{
		static inline __declspec(nothrow) void _call_release(IUnknown& x_)	{ x_.Release(); }
		static inline __declspec(nothrow) void _call_addref(IUnknown& x_)	{ x_.AddRef();	}
	protected:
		static void _do_addref(T& x_) noexcept {
			if constexpr (is_interface_singleton<T>)	{	/*do nothing*/ }
			else										{  _call_addref(x_); }
		}
		static void _do_addref(T* p_) noexcept {
			if constexpr (is_interface_singleton<T>)	{	/*do nothing*/	}
			else										{	if (p_) _call_addref(*p_);	}
		}
		static void _do_release(T* p_) noexcept {
			if constexpr (is_interface_singleton<T>)	{	/*do nothing*/	}
			else										{ if (p_) _call_release(*p_);	}
		}	
		static void _do_assign(T*& this_p_, T* other_p_) {
			if constexpr (is_interface_singleton<T>)	{ /* no addref\release needed*/	} 
			else										{ 
				_do_addref(other_p_);
				_do_release(this_p_);
			}
			this_p_ = other_p_;
		}
		constexpr InterfacePtrBase(nullptr_t) noexcept							: p { NULL }	{}
		constexpr InterfacePtrBase(_Inout_opt_ T* lp) noexcept					: p{ lp }		{ _do_addref(p); }
		constexpr InterfacePtrBase(T& x_) noexcept								: p{ &x_ }		{ _do_addref(x_); }
		//5
		constexpr InterfacePtrBase(InterfacePtrBase<T>&& other_) noexcept		: p{ other_.p } { other_.p = NULL; }
		constexpr InterfacePtrBase(const InterfacePtrBase<T>& other_) noexcept	: p{ other_.p } { _do_addref(p); }
		InterfacePtrBase<T>& operator=(const InterfacePtrBase<T>& other_) noexcept	{
			if (this->p != other_.p)
				_do_assign(this->p, other_.p);
			return *this;
		}
		InterfacePtrBase<T>& operator=(InterfacePtrBase<T>&& other_) noexcept		{
			if (this->p != other_.p) {
				_do_release(this->p);
				this->p = other_.p;
				other_.p = NULL;
			}
			return *this;
		}
	public:
		~InterfacePtrBase() noexcept						{ _do_release(p); }
		operator T* () const noexcept						{ return p;	}
		T& operator*() const noexcept						{ ESB_ASSERT(p != NULL);	return *p;	}
		//The assert on operator& usually indicates a bug.  If this is really
		//what is needed, however, take the address of the p member explicitly.
		T** operator&() noexcept							{ ESB_ASSERT(p == NULL);	return &p; }
	ESB_WARNING_SUPRESS(ESB_WARN_NO_VIRTUAL_TRIVIAL_DTOR)
		class NoAddRefReleaseT : public T {
		private:
			STDMETHOD_(ULONG, AddRef)() = 0;
			STDMETHOD_(ULONG, Release)() = 0;
		};
	ESB_WARNING_RESTORE()	//ESB_WARN_NO_VIRTUAL_TRIVIAL_DTOR
		NoAddRefReleaseT* operator->() const noexcept		{ ESB_ASSERT(p != NULL);	return (NoAddRefReleaseT*)p; }
		bool operator!() const noexcept						{ return (p == NULL);		}
		bool operator<(_In_opt_ T* pT) const noexcept		{ return p < pT;		}
		bool operator!=(_In_opt_ T* pT) const noexcept		{ return !operator==(pT);		}
		bool operator==(_In_opt_ T* pT) const noexcept		{ return p == pT;		}
	public:
		//TOBE	�� ������ ������ ������������ ������ init\term\on_init\do_init\do_term\... � �.�. ������
		//		��� ��� �������� ��� ����, ��� ������ ������� esb ����� ���������� ������������� ��� OnComponentInitStartup 
		//		� ��������� ������� ��� OnComponentDoneCleanup.
		//		�� ���� esb-������ ����� ����� ����-��������� ����\����.
		//		����� ���� ����� ������ ���������� do_term � ���� � InterfacePtr, � �� � InterfacePtrBase
		void reset() {
			T* local_p = p;
			p = nullptr;
			_do_release(local_p);
		}
	public:
		T* p;
	};


	//TOBE	InterfacePtr ������� ���, ����� ������� �� ���� nullptr. ������ � ������������ �������������� �������
	//		�� �������� core::InterfacePtrBase - ��� core::InterfacePtr, � core::InterfacePtr ��� esb::InterfaceRef
	template <class T>
	class InterfacePtr : public InterfacePtrBase<T>
	{
		using base_t = InterfacePtrBase<T>;
	public:
		//���� ���������� ������ ��� ������ ����� ��� �����
		using interface_t = T;

		constexpr InterfacePtr(nullptr_t) noexcept : base_t(nullptr) {}
		constexpr InterfacePtr(T& p_) noexcept : base_t(p_) {}
		//5
		constexpr InterfacePtr(const InterfacePtr<T>& lp) noexcept	: base_t(lp.p) {}
		constexpr InterfacePtr(InterfacePtr<T>&& other_) noexcept	: base_t(std::move(other_)) {}
		constexpr InterfacePtr<T>& operator=(const InterfacePtr<T>& other_) noexcept	{ return static_cast<InterfacePtr<T>&>(base_t::operator=(other_)); }
		constexpr InterfacePtr<T>& operator=(InterfacePtr<T>&& other_) noexcept			{ return static_cast<InterfacePtr<T>&>(base_t::operator=(std::move(other_))); }
		~InterfacePtr() noexcept = default;
	public:
		//TOBE: ��� InterfacePtr ����� ������� ��������� operator U, ������� �������� ���� ���� ���������� ������������.
		//		�� ���� ���� ����� IFaceDerived ������������ �� IFaceBase, �� IFaceDerivedPtr ����� � ������ ����� ����������������� � IFaceBasePtr
		//		(������ ��� �++ ��� ��������� ������ �� ��������� ���� � ������ ������, ��� ������ ���������)
		//		����� ����� ������� explicit cast ���� �� IFaceBasePtr � IFaceDerivedPtr
	};

}


#endif //ESB_CORE_IFPTR_H_