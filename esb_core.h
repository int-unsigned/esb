#pragma once
#ifndef ESB_CORE
#define ESB_CORE

#include "esb_app.h"


//затем "Unknwnbase.h" в комбинации с COM_NO_WINDOWS_H он windows.h не подключает, но все что надо подключает
#define COM_NO_WINDOWS_H
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <combaseapi.h>
#include "Unknwnbase.h"

#include <string>
#include <string_view>
#include <span>
#include <utility>

//
#include "esb_util.h"
#include "esb_errors.h"





// как аргумент шаблона допускается только struct: error C2993: "esb::FixedString": не является допустимым типом для параметра шаблона "TA", не являющегося типом.
// message : "ptr_" не является открытым, неизменяемым, нестатическим элементом данных.
// clang 14, 15 error: pointer to subobject of string literal is not allowed in a template argument
// mscl 19.29.30146 (vc 16.11.19) - OK, but IDE sow error E2645: недопустимый нетипизированный аргумент шаблона
//	
class FixedString {
	const wchar_t*	m_ptr;
	size_t			m_len;
public:
	template<std::size_t N>
	constexpr FixedString(const wchar_t(&arr_)[N]) : m_ptr(arr_), m_len(N - 1)
	{}
	constexpr FixedString(const wchar_t* ptr_, size_t len_) : m_ptr{ ptr_ }, m_len{ len_ }
	{}
	constexpr size_t len() const				{ return m_len; }
	//string view -ish
	constexpr size_t			size() const	{ return m_len; }
	constexpr const wchar_t*	data()	const	{ return m_ptr; }
};


struct FixedTerm {
	FixedString m_string[2];
public:
	constexpr FixedTerm(const FixedString& s1_, const FixedString& s2_) : m_string{ s1_, s2_ }
	{}
	template<size_t NEn, size_t NRu>
	constexpr FixedTerm(const wchar_t(&str1_)[NEn], const wchar_t(&str2_)[NRu]) : m_string{ str1_, str2_ }
	{}
	constexpr size_t size() const { 
		return 2; 
	}
	constexpr bool index_ok(int index_) const {
		return (index_ >= 0 && (unsigned)index_ < size());
	}
	constexpr const FixedString& operator[](int index_) const {
		assert(index_ok(index_));
		return m_string[index_];
	}
	constexpr const FixedString* at(int index_) const {
		return (index_ok(index_))? &m_string[index_] : nullptr;
	}
	constexpr const wchar_t* cstr_at(int index_) const {
		return (index_ok(index_)) ? m_string[index_].data() : nullptr;
	}
};


using UniqueTypeId = CLSID;
using UniqueTypeTerm = FixedTerm;




// COM-like объекты применяют иную технику наследования - аггрегирование. Соответственно политика относительно виртуального деструктора иная.
// (мы не создаем объекты через new и не уничтожаем их через delete. и вообще пока у нас нет ни наследования ни аггрегирования)
//
//warning C5204: "esb::IValue": класс содержит виртуальные функции, но его тривиальный деструктор не является виртуальным; 
//								деструктор для экземпляров производных от этого класса объектов может работать некорректно.
//это совершенно нормально для интерфейса. подавляем.
// 
//warning C4265 : "esb::TypeDescriptor" :	класс содержит виртуальные функции, но его нетривиальный деструктор не является виртуальным; 
//											деструктор экземпляров этого класса может работать некорректно.
//несколько иная ситуация (см.замечения для TypeDescriptor и TypeInfo). Классы не предназначены для какого-либо создания\копирования\т.п.
//кроме как статически при компиляции. подавляем.
//
ESB_WARNING_SUPRESS_NO_VIRTUAL_DTOR_ANY()



//InterfacePtr
namespace core 
{
	//TOBE:	Многие используемые esb объекты платформы являются COM-синглетонами. Т.е. создаются при старте, уничтожаются при выгрузке и Addref\Release
	//		подсчетом ссылок не занимаются.
	//		Поэтому вызывать Addref\Release им не нужно. И дело не в том, что это расходы 20 тактов процессора, а в том, что esbhlp кэширует у себя
	//		некоторые объекты не запрашивая их каждый раз у платформы. (нр. апи-провайдеров)
	//		И тогда возникает нехорошая ситуация с выгрузкой. Мы не контролируем очередность нашей выгрузки и можем случайно вызвать Release объекту
	//		dll которого была уже выгружена платформой. 
	//		Сейчас эта проблема решена способом OnComponentDoneCleanup, но лучше просто не вызывать.
	template<class InterfaceT>
	inline constexpr bool is_interface_singleton = false;


	// По мотивам CComPtr
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
		class NoAddRefReleaseT : public T {
		private:
			STDMETHOD_(ULONG, AddRef)() = 0;
			STDMETHOD_(ULONG, Release)() = 0;
		};
		NoAddRefReleaseT* operator->() const noexcept		{ ESB_ASSERT(p != NULL);	return (NoAddRefReleaseT*)p; }
		bool operator!() const noexcept						{ return (p == NULL);		}
		bool operator<(_In_opt_ T* pT) const noexcept		{ return p < pT;		}
		bool operator!=(_In_opt_ T* pT) const noexcept		{ return !operator==(pT);		}
		bool operator==(_In_opt_ T* pT) const noexcept		{ return p == pT;		}
	public:
		//TOBE	Во многих местах используются всякие init\term\on_init\do_init\do_term\... и т.п. методы
		//		Они все отражают тот факт, что многие объекты esb имеют отложенную инициализацию при OnComponentInitStartup 
		//		и досрочную очистку при OnComponentDoneCleanup.
		//		То есть esb-классы имеют некий мета-интерфейс инит\терм.
		//		Тогда этот метод должен называться do_term и быть у InterfacePtr, а не у InterfacePtrBase
		void reset() {
			T* local_p = p;
			p = nullptr;
			_do_release(local_p);
		}
	public:
		T* p;
	};


	//TOBE	InterfacePtr выписан так, чтобы никогда не быть nullptr. только в ограниченный исключительных случаях
	//		По хорошему core::InterfacePtrBase - это core::InterfacePtr, а core::InterfacePtr это esb::InterfaceRef
	template <class T>
	class InterfacePtr : public InterfacePtrBase<T>
	{
		using base_t = InterfacePtrBase<T>;
	public:
		//Этот магический типдеф нам дальше много где нужен
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
		//TOBE: для InterfacePtr можно сделать шаблонный operator U, который разрешен если сами интерфейсы унаследованы.
		//		то есть если некий IFaceDerived унаследовано от IFaceBase, то IFaceDerivedPtr может и должен легко перобразовываться в IFaceBasePtr
		//		(сейчас для с++ это абсолютно разные не связанные друг с другом классы, что иногда напрягает)
		//		Также можно сделать explicit cast вниз от IFaceBasePtr к IFaceDerivedPtr
	};


	template<class InterfaceT>
	struct template_argument_type<InterfacePtr<InterfaceT>> { using type = InterfaceT; };

	static_assert(std::is_same_v< IUnknown, template_argument_type_t<InterfacePtr<IUnknown>> > );


	template<typename>
	inline constexpr bool is_interface_ptr = false;
	template<typename T> 
	inline constexpr bool is_interface_ptr<InterfacePtr<T>> = true;

	static_assert(is_interface_ptr<int> == false);
	static_assert(is_interface_ptr<InterfacePtr<IUnknown>> == true);


	// через структуру, а не непосредственно через переменную _iid_of сделано чтобы иметь _has_iid
	// (запросить у компилятора наличие специализации структуры можно, а наличие специализации переменной не получается)
	// в компилированном виде лишних структур нет - компилятор их выбрасывает.
	template<class InterfaceT>
	struct _iid_of_t {};
	template<>
	struct _iid_of_t<IUnknown> {
		static inline constexpr const IID value_ = __uuidof(IUnknown);
	};

#define ESB_DECLARE_IID_OF(TYP_T_, IID_V_)	template<> struct _iid_of_t<TYP_T_> { static inline constexpr const IID value_ = IID_V_; }


	template<typename T>
	concept _has_iid = requires { _iid_of_t<T>::value_; };

	template<class InterfaceT>
	static inline constexpr const IID _iid_of = _iid_of_t<InterfaceT>::value_;

	static_assert(_has_iid<IUnknown> == true);
	static_assert(_has_iid<int> == false);



	template<class RawInterfaceT>
	inline InterfacePtr<RawInterfaceT> query_interface_or_die(IUnknown& source_) {
		static_assert(_has_iid<RawInterfaceT>, "_iid_of absent for RawInterfaceT. Make ESB_DECLARE_IID_OF for RawInterfaceT.");

		InterfacePtr<RawInterfaceT> ifptr{ nullptr };
		HRESULT hr = source_.QueryInterface(_iid_of<RawInterfaceT>, (void**)&ifptr);	//QueryInterface return with Addref!!

		assert(hr == S_OK && ifptr);
		if (hr == S_OK) {
			if (!ifptr)
				ESL_THROW_UNEXPECTED_QUERY_INTERFACE_RESULT();
			return ifptr;	//code-analis посоветовал вернуть без мув
		}
		ESL_THROW_UNEXPECTED_QUERY_INTERFACE_FAILED();
	}
	template<class InterfacePtrT>
	requires is_interface_ptr<InterfacePtrT>
	inline InterfacePtrT query_interface_or_die(IUnknown& source_) {
		return query_interface_or_die<template_argument_type_t<InterfacePtrT>>(source_);
	}


	template<class RawInterfaceT>
	inline InterfacePtr<RawInterfaceT> query_interface_opt(IUnknown& source_) {
		static_assert(_has_iid<RawInterfaceT>, "_iid_of absent for RawInterfaceT. Make ESB_DECLARE_IID_OF for RawInterfaceT.");
		InterfacePtr<RawInterfaceT> ifptr = nullptr;
		[[maybe_unused]] HRESULT hr = source_.QueryInterface(_iid_of<RawInterfaceT>, (void**)&ifptr);	//QueryInterface return with Addref!!
		assert(hr == S_OK || hr == E_NOINTERFACE || !FAILED(hr));
		return std::move(ifptr);
	}
	template<class InterfacePtrT>
	requires is_interface_ptr<InterfacePtrT>
	inline InterfacePtrT query_interface_opt(IUnknown& source_) {
		return query_interface_opt<template_argument_type_t<InterfacePtrT>>(source_);
	}

}	//namespace core InterfacePtr



//INFO	Класс ошибки нельзя полностью "спрятать". он должен быть явно объявлен и корректно определен. (см.coreall)
//		Реализация всех методов в esbhlp
// 
//INFO	Относительно исключений. Выбрасывание исключений через границу dll для с++ вообще плохая практика. 
//		Нужно быть ну очень уверенным что все dll скомпилированы одинаково (и компилятор и опции) в отношении модели исключений.
//		Но имеем, что имеем...
//		Платформа 1С предаставляет некоторое АПИ для выброса нужных нам исключений. (core::throw_wrong_parameter(...) и аналоги)
//		И правильным бы было использовать его.
//		Но нам исключения нужно не только бросать, но и перехварывать. Поэтому от объявления класса core::Exception все равно не уйти.
//		Получится что нужно будет поддерживать два механизма - и АПИ платформы и собственно класс Exception. 
//		Уж лучше что-то одно, но хорошо.
//
#ifdef ESB_USE_NATIVE
#include "esb_coreall.h"
#else 
namespace core {
#define ESB_CORE_EXCEPTION_SIZE		44
#define ESB_CORE_EXCEPTION_ALIGN	4

	class IExceptionObject;

	class alignas(ESB_CORE_EXCEPTION_ALIGN) Exception {
	protected:
		char	m_dummy_data[ESB_CORE_EXCEPTION_SIZE];
	public:
		Exception() = delete;
		~Exception();
		// не может быть noexcept - аллоцирует. у 1С тоже.
		Exception(const Exception&);
		Exception& operator=(const Exception&);
		Exception(Exception&&) noexcept;
		Exception& operator=(Exception&&) noexcept;
	public:
		Exception(const GUID& clsid_, const std::wstring_view& text_, core::IExceptionObject*);
	public:
		core::InterfacePtr<core::IExceptionObject> __thiscall	object(void) const;
		const /*byval*/ std::wstring_view						text() const;
	};
	static_assert(sizeof(Exception) == ESB_CORE_EXCEPTION_SIZE && alignof(Exception) == ESB_CORE_EXCEPTION_ALIGN);
#pragma detect_mismatch( "ESB_CORE_EXCEPTION_SIZE" , PP_STRINGIZE(ESB_CORE_EXCEPTION_SIZE) )
#pragma detect_mismatch( "ESB_CORE_EXCEPTION_ALIGN" , PP_STRINGIZE(ESB_CORE_EXCEPTION_ALIGN) )
}
#endif




// объявление минимального набора необходимых esb интерфейсов.
namespace esb {
	//
#ifdef ESB_USE_NATIVE
	//
#else
	class IValue : public IUnknown {};
	class IType : public IUnknown {};
	class IObject : public IUnknown {};

	class IVariable { 
		uintptr_t _; 
	};

	class IEnumValues : public IUnknown {};

	class IIxCollectionRO : public IUnknown {};
	class IIxCollectionRW : public IIxCollectionRO {};
	class IIxCollection : public IIxCollectionRW {};

	class IValuePair : public IUnknown {};
	class IAxCollectionRO : public IUnknown {};
	class IAxCollectionRW : public IAxCollectionRO {};
	class IAxCollection : public IAxCollectionRW {};
#endif


	template<class InterfaceT>
	using InterfacePtr = core::InterfacePtr<InterfaceT>;


	//TOBE	Проверку на is_esb_interface делаем не через специализацию, а через магический interface_t, т.к. дальше этот магический нам нужен
	//		Возможно нужно проверку делать специализацией, а требование на interface_t описать статик-ассертом там где он используется
	//		Или избавиться от магии и использовать template_argument_type
	template<class T>
	constexpr bool is_esb_interface = (requires {std::same_as<T, InterfacePtr<typename T::interface_t>>; });
	


	template<class InterfaceT>
	concept InterfaceConcept = is_esb_interface<InterfaceT>;

	using IValuePtr = InterfacePtr<IValue>;
	using IObjectPtr = InterfacePtr<IObject>;

	// приходящие к нам аргументы при вызове платформой наших объектов
	using argpack_t = std::span<IVariable* /*, dynamic_extent*/ >;
	// уходящие от нас аргументы при вызове нами методов объектов платформы.
	// (кроме всего прочего нам нужно чтобы они выстроились в массив для реализации dispatch-like интерфейса)
	using argarray_t = std::initializer_list<const IVariable*>;

	using IEnumValuesPtr = InterfacePtr<IEnumValues>;

	using IIxCollectionROPtr = InterfacePtr<IIxCollectionRO>;
	using IIxCollectionRWPtr = InterfacePtr<IIxCollectionRW>;
	using IIxCollectionPtr = InterfacePtr<IIxCollection>;

	using IValuePairPtr = InterfacePtr<IValuePair>;
	using IAxCollectionROPtr = InterfacePtr<IAxCollectionRO>;
	using IAxCollectionRWPtr = InterfacePtr<IAxCollectionRW>;
	using IAxCollectionPtr = InterfacePtr<IAxCollection>;
}


//TOBE	см.где объявлено is_interface_singleton
namespace esb {
	class IObjectSingleton : public IObject {};
	using IObjectSingletonPtr = InterfacePtr<IObjectSingleton>;
}
namespace core {
	template<>
	inline constexpr bool is_interface_singleton<esb::IObjectSingleton> = true;
}




namespace esb {
	// начальная инициализация библиотеки
	bool OnComponentInitStartup();
	void OnComponentDoneCleanup();

#define ESB_DECLARE_FRIEND_INITTERM()		\
	friend bool OnComponentInitStartup();	\
	friend void OnComponentDoneCleanup();


	//INFO	АПИ esbhlp реализовано по принципу чем "меньше, ниже и легче" тем лучше. Поэтому везде голые интерфейсы и примитивные типы.
	//		Но к сожалению часть АПИ зависит от строковых идентификаторов (нр. имена свойств или полей запроса)
	//		У 1С они естесственно построены на основе их проприетарной и самобытной строки.
	//		Единственный корректный способ реализовать такое АПИ не связываясь с их строкой - это использовать строку-объект языка 1С esb::String
	//		(Она, естесственно, тоже построена на основе их строки)
	//		Поэтому объявляем класс String самым первым прямо здесь.
	class String;
	//esbhlp
	//------------------------------------------ api --------------------------------------------------------------------------------------
	IValuePtr	create_undefined_value(void);
	IValuePtr	create_null_value(void);
	IValuePtr	create_boolean_value(bool);
	IValuePtr	create_numeric_value(short);
	IValuePtr	create_numeric_value(int);
	IValuePtr	create_numeric_value(long);
	IValuePtr	create_numeric_value(long long);
	IValuePtr	create_numeric_value(unsigned short);
	IValuePtr	create_numeric_value(unsigned int);
	IValuePtr	create_numeric_value(unsigned long);
	IValuePtr	create_numeric_value(unsigned long long);
	IValuePtr	create_numeric_value(double);
	//IValuePtr create_string_value(wchar_t const*);		//навязываем, чтобы не ленились длину высчитывать. это всегда можно сделать
	IValuePtr	create_string_value(const std::wstring_view&);
	
	bool		is_space(wchar_t);
	IValuePtr	create_error_value(const core::Exception& e_);

	// создает значение для самого объекта тип
	IValuePtr				IType_CreateTypeValueForType(IType&);
	// создает значение соответствующего типа
	IValuePtr				IType_CreateValue(IType& type_);
	IValuePtr				IType_CreateValue(IType& type_, const argarray_t& args_);
	IType*					IType_FromName(const strview_t& name_);		// //maybe nullptr if not found

	// Везде и всегда (вроде) должно возвращать !nullptr, вроде не бывает значений "без типа", и можно было-бы вернуть IType&
	// но у платформы апи такое и тогда мне все равно придется делать ESB_ASSERT в esbhlp. Уж лучше здесь.
	IType*					IValue_GetType(IValue& val_);	
	IValuePtr				IValue_ToString(IValue& val_);	//maybe nullptr if no conversion
	IValuePtr				IValue_ToNumeric(IValue& val_);	//maybe nullptr	if no conversion
	IValuePtr				IValue_ToBoolean(IValue& val_);	//maybe nullptr	if no conversion
	IValuePtr				IValue_ToDate(IValue& val_);	//maybe nullptr	if no conversion
	bool					IValue_IsEqualValue(IValue& value1_, IValue& value2_);

	void					IVariable_SetValue(IVariable& target_, IValue& val_);
	IValuePtr				IVariable_GetValue(const IVariable& source_);

	void					IObjectMethods_InvokeAsProc(IObject& obj_, dispid_t meth_);
	void					IObjectMethods_InvokeAsProc(IObject& obj_, dispid_t meth_, const argarray_t& args_);
	IValuePtr				IObjectMethods_InvokeAsFunc(IObject& obj_, dispid_t meth_);
	IValuePtr				IObjectMethods_InvokeAsFunc(IObject& obj_, dispid_t meth_, const argarray_t& args_);
	int						IObjectMethods_Find(IObject& obj_, const String& name_);
#if ESB_USE_OBJECT_DISPATCH_FIND_STRVIEW_HACK
	int						IObjectMethods_Find(IObject& obj_, const std::wstring_view& name_);
#endif // !ESB_USE_OBJECT_DISPATCH_FIND_STRVIEW_HACK

	IValuePtr				IObjectProperties_GetValue(IObject& obj_, dispid_t prop_);
	void					IObjectProperties_SetValue(IObject& obj_, dispid_t prop_, const IVariable& value_);
	bool					IObjectProperties_IsReadable(IObject& obj_, dispid_t prop_);
	bool					IObjectProperties_IsWritable(IObject& obj_, dispid_t prop_);
	inline IValuePtr IObjectProperties_GetValue(IObject& obj_, dispix_t prop_)							{ return IObjectProperties_GetValue(obj_, as_dispid(prop_)); }
	inline void		 IObjectProperties_SetValue(IObject& obj_, dispix_t prop_, const IVariable& value_) { return IObjectProperties_SetValue(obj_, as_dispid(prop_), value_); }
	inline bool		 IObjectProperties_IsReadable(IObject& obj_, dispix_t prop_)						{ return IObjectProperties_IsReadable(obj_, as_dispid(prop_)); }
	inline bool		 IObjectProperties_IsWritable(IObject& obj_, dispix_t prop_)						{ return IObjectProperties_IsWritable(obj_, as_dispid(prop_)); }
	int						IObjectProperties_Find(IObject& obj_, const String& name_);
#if ESB_USE_OBJECT_DISPATCH_FIND_STRVIEW_HACK
	int						IObjectProperties_Find(IObject& obj_, const std::wstring_view& name_);
#endif // !ESB_USE_OBJECT_DISPATCH_FIND_STRVIEW_HACK
	size_t					IObjectProperties_Size(IObject& obj_);

	IEnumValuesPtr			IEnumValues_Clone(IEnumValues& interface_);
	bool					IEnumValues_Next(IEnumValues& interface_, IVariable& out_value_);
	IValuePtr				IEnumValues_Next(IEnumValues& interface_);
	bool					IEnumValues_Reset(IEnumValues& interface_);
	bool					IEnumValues_Skip(IEnumValues& interface_, size_t n_skip_);

	// интерфейсы коллекций унаследованы друг от друга, поэтому проще принимать сам интерфейс, а не его Ptr 
	
	size_t					IxCollection_Size(IIxCollectionRO& interface_);
	IValuePtr				IxCollection_GetAt(IIxCollectionRO& interface_, size_t index_);
	void					IxCollection_SetAt(IIxCollectionRW& interface_, size_t index_, const IVariable& value_);
	void					IxCollection_Resize(IIxCollection& interface_, size_t new_size_);
	void					IxCollection_Insert(IIxCollection& interface_, size_t at_index_, const IVariable& value_);
	void					IxCollection_Remove(IIxCollection& interface_, size_t at_index_);
	void					IxCollection_Copy(IIxCollectionRO& collection_get_, size_t index_get_, IIxCollectionRW& collection_set_, size_t index_set_);
	void					IxCollection_Copy(IIxCollectionRW& collection_, size_t index_get_, size_t index_set_);
	void					IxCollection_Swap(IIxCollectionRW& collection_a_, size_t index_a_, IIxCollectionRW& collection_b_, size_t index_b_);
	void					IxCollection_Swap(IIxCollectionRW& collection_, size_t index_a_, size_t index_b_);

	IValuePtr				IValuePair_GetKey(IValuePair& interface_);
	IValuePtr				IValuePair_GetValue(IValuePair& interface_);

	size_t					AxCollection_Size(IAxCollectionRO& interface_);
	IValuePairPtr			AxCollection_Find(IAxCollectionRO& interface_, const IVariable& key_);
	void					AxCollection_Replace(IAxCollectionRW& interface_, const IVariable& key_, const IVariable& value_);
	void					AxCollection_Insert(IAxCollection& interface_, const IVariable& key_, const IVariable& value_);
	void					AxCollection_Remove(IAxCollection& interface_, const IVariable& key_);
	void					AxCollection_Clear(IAxCollection& interface_);

	String					FormatVar(const IVariable& Var_, const String& FmtString_);
	int						collator_compare(const String& s1_, const String& s2_);
}



namespace esb 
{	
	class Value;
	class Object;
	class Arbitrary;
	class Undef;
	template<class... EsbClassesT>	class ClassMix;
	template<class EsbClassT>		class VarArgumentByValOnThat;
	class String;

	template<class EsbClassT>
	class Var;

	template<typename>
	struct Invoker;


// Класс VarArgumentByValOnThat берет просто обычную константную ссылку на Value и вообще-то желательно давать это делать только если Value - lvalue
// так как иначе (rvalue) в каких-то случаях можно получить висящую ссылку (или Value, которое уже умерло). Но тогда пропадает легкость вызовов в виде 
// array.Add(array.Get(...)) - плюсы начинают запрещать такое. Это неприятно.
// При обычной работе никаких проблем с висящими ссылками быть не должно, т.к. VarArgumentByValOnThat спроектирован исключительно для передачи "на лету" вызова
// в платформу, и Value на котором он построен гарантировано будет существовать до конца этого вызова.
// Использование VarArgumentByValOnThat и прочих VarArgumentBy... каким-либо иным образом черевато...
// Это не является каким-то нарушением правил - все итераторы с++ фактически также построены и их также нельзя использовать если их контейнер умер.
// operator const VarArgumentByValOnThat<const CLASS_T_>() const &
// operator const VarArgumentByValOnThat<const Arbitrary>()&& = delete;

#define ESB_ARGBYVAL_OPERATOR(CLASS_T_)					\
	operator const VarArgumentByValOnThat<const CLASS_T_>

#define ESB_DECLARE_ARGBYVAL_OPERATOR(CLASS_T_)			\
	ESB_ARGBYVAL_OPERATOR(CLASS_T_) () const

#define ESB_IMPLEMENT_ARGBYVAL_OPERATOR_BODY(CLASS_T_)	\
	{ return VarArgumentByValOnThat<const CLASS_T_>(*this); }

#define ESB_IMPLEMENT_ARGBYVAL_OPERATOR(CLASS_T_)		\
	ESB_DECLARE_ARGBYVAL_OPERATOR(CLASS_T_)		ESB_IMPLEMENT_ARGBYVAL_OPERATOR_BODY(CLASS_T_)



	class Value
	{
	public:
		using interface_t = IValuePtr;
	private:
		IValuePtr	m_ivalue;
		Value() = delete;
		// сразу здесь тело написать не получается. то-ли consteval, то-ли class еще неопределен..
		template<class DerivedT>
		friend consteval size_t esb_value_based_value_offset();	
	protected:
		constexpr Value(std::nullptr_t) noexcept : m_ivalue{ nullptr }
		{}
		Value(IValuePtr&& ival_) noexcept : m_ivalue(std::move(ival_))
		{}

		friend const IValuePtr& get_interface(const Value&);
		friend IValuePtr& get_interface(Value& val_);
		friend IValuePtr&& get_interface(Value&&);

		bool is_null() const { 
			return (m_ivalue==nullptr); 
		}
		// для переустановки значения из наследников при вычислениях типа +=, ++, т.п.
		void set_interface(IValuePtr&& interface_) {
			ESB_ASSERT(interface_ && m_ivalue);
			m_ivalue = std::move(interface_);
		}
		// для инициализации значений при связывании библиотеки если изначально при загрузке длл установлено в nullptr
		void initialize(IValuePtr&& interface_) {
			ESB_ASSERT(interface_ && !m_ivalue);
			m_ivalue = std::move(interface_);
		}
		// мы проверяем только указатели на тождественность. унаследованные от нас могут сами определить свои методы сравнения
		bool operator==(const Value& other_) const noexcept {
			// мы постараемся все разработать так, чтобы у нас никогда после инициализации не было nullptr - значений.
			// единственное исключение ClassOpt, но он от нас не унаследован.
			ESB_ASSERT(m_ivalue && other_.m_ivalue);
			return (m_ivalue == other_.m_ivalue);
		}
	public:
		// все прочие конструкторы и операторы default - где копирование, там копирование, где перемещение там перемещение - ++ лучше знает
	public:
		// любой Value-based объект может тихо трактоваться как const Arbitrary или умирающий Arbitrary. Имплементация после Arbitrary
		// поскольку для с++ нетипизированный arbitrary-программинг не типичный режим, то сделаем операторы по-требованию (explicit)
		explicit operator const Arbitrary&() const&;
		explicit operator Arbitrary&& () &&;
		// как просто Arbitrary трактоваться не может, т.к. тогда сюда можно присвоить другой Arbitrary с иным типом
		operator Arbitrary& () & = delete;

		// любой Value-based объект может напрямую отдавать ArgByVal прокси типа Arbitrary
		inline ESB_DECLARE_ARGBYVAL_OPERATOR(Arbitrary);
	};


	// любой есб-класс может быть обернут в Var и там необходимо чтобы он был первым в наследовании.
	// но получается некоторое ограничение, что от esb-классов наследоваться надо публично или дать этому как-то этому френду добраться до Value
	template<class DerivedT>
	consteval size_t esb_value_based_value_offset() {
		return offsetof(DerivedT, m_ivalue);
	}
	template<class DerivedT>
	consteval bool is_esb_value_based_zero_offset() {
		return (esb_value_based_value_offset<DerivedT>() == 0);
	}
	// можно было-бы сразу переменную, но.. компилится-ок, а IDE не понимает... по-старинке - понимает
	template<class DerivedT, bool>
	struct is_esb_value_based_t {
		static inline constexpr bool value_ = false;
	};
	template<class DerivedT>
	struct is_esb_value_based_t<DerivedT, true> {
		static inline constexpr bool value_ = is_esb_value_based_zero_offset<DerivedT>();
	};
	
	template<typename EsbClassT>
	inline constexpr bool is_esb_value_based = is_esb_value_based_t<EsbClassT, std::is_base_of_v<esb::Value, EsbClassT>>::value_;


	//возможно единственность наследования можно проверить как-то по-другому.. но это работает.
	template<typename EsbClassT>
	inline constexpr bool is_esb_value_simple = (is_esb_value_based<EsbClassT> && sizeof(EsbClassT) == sizeof(Value));


	//NOTE	Это методы-"друзья" базовых классов возращающие их интерфейсы... много где используются... как-то так сознательно разрушаем инкапсуляцию чтобы...
	//		Но в публичном интерфейсе Value всяких "странных методов возвращающих какие-то интерфейсы" - нет. А все что маленькими буквами - то "детали реализации"!
	//TOBE	когда и если все прорисуется можно будет пробовать объявить друзьями сами классы где используются эти интерфейсы и восстановить инкапсуляцию
	//		только вряд-ли везде получится, но обращаться к этому периодически надо.
	inline const IValuePtr& get_interface(const Value& val_)	{ return val_.m_ivalue; }
	inline IValuePtr& get_interface(Value& val_)				{ return val_.m_ivalue; }
	inline IValuePtr&& get_interface(Value&& val_)				{ return std::move(val_.m_ivalue); }


	class Object : public Value
	{
	private:
		IObjectPtr	m_ipctx;
		Object() = delete;
	protected:
		Object(IValuePtr&& val_, IObjectPtr&& obj_) throw() : Value(std::move(val_)), m_ipctx(std::move(obj_))
		{}
		constexpr Object(nullptr_t) throw() : Value(nullptr), m_ipctx(nullptr)
		{}
		friend const IObjectPtr& get_context(const Object&);
	public:
		// все прочие конструкторы и операторы default - где копирование, там копирование, где перемещение там перемещение - ++ лучше знает
	};


	//NOTE	широко используется для получения контекста объекта во всех наследниках (ESB_IMPLEMENT_INVOKE_... ) и их примесях. Деталь реализации!
	//TOBE	в принципе наследникам можно дать протектед доступ, а примеси объявлять друзьями по месту использования... (см.get_interface(Value&))
	inline const IObjectPtr& get_context(const Object& obj_) { return obj_.m_ipctx; }


	template<typename EsbClassT>
	inline constexpr bool is_esb_object_based = (is_esb_value_based<EsbClassT> && std::is_base_of_v<esb::Object, EsbClassT>);

	template<typename EsbClassT>
	inline constexpr bool is_esb_object_simple = (is_esb_object_based<EsbClassT> && sizeof(EsbClassT) == sizeof(Object));

	template<typename EsbClassT>
	inline constexpr bool is_esb_class = is_esb_value_based<EsbClassT>;

	template<class EsbClassT>
	concept EsbValueSimpleConcept = is_esb_value_simple<EsbClassT>;

	template<class EsbClassT>
	concept EsbObjectAnyConcept = is_esb_object_based<EsbClassT>; 

	// "концептуально" любой унаследованный от Value
	template<class EsbClassT>
	concept EsbClassConcept = is_esb_class<EsbClassT>;



	template<class ObjExtraT>
	class ObjectDual : public Object, protected ObjExtraT {
	public:
		using dual_t = ObjExtraT;
		using dual_interface_t = typename dual_t::interface_t;
	protected:
		ObjectDual(IValuePtr&& val_, IObjectPtr&& obj_, dual_interface_t&& ext_) : Object(std::move(val_), std::move(obj_)), ObjExtraT(std::move(ext_))
		{}
		ObjectDual(nullptr_t) : Object(nullptr), ObjExtraT(nullptr)
		{}
	};
	//TOBE:	Здесь и для ObjectDual2 лучше сделать через специализацию структур нежли через наличие магического dual_t. 
	//		Но это используется в make и там завязано на dual_interface_t. Придется как-то менять и там. Выносить логику make из одного места и
	//		разбрасывать ее по классам ..Dual.. не хочется.
	//		Имеет смысл если кол-во интерфейсов будет увеличиваться и нужно будет переходить на конструкторы от <InterfacePtrT...> или еще как.
	template<class T>
	constexpr bool is_esb_object_dual = requires {				// проверять is_esb_object_based<T> излишне, т.к. ObjectDual прямо унаследован от Object
		typename T::dual_t;
		requires std::is_base_of_v<ObjectDual<T::dual_t>, T>;	// requires обязательно, чтобы проверило на true, без него проверяет только на корректность
	};


	template<class ObjExtraT1, class ObjExtraT2>
	class ObjectDual2 : public Object, protected ObjExtraT1, protected ObjExtraT2 {
	public:
		using dual1_t = ObjExtraT1;
		using dual1_interface_t = dual1_t::interface_t;
		using dual2_t = ObjExtraT2;
		using dual2_interface_t = dual2_t::interface_t;
	protected:
		ObjectDual2(IValuePtr&& val_, IObjectPtr&& obj_, dual1_interface_t&& ex1_, dual2_interface_t&& ex2_)
			: Object(std::move(val_), std::move(obj_)), ObjExtraT1(std::move(ex1_)), ObjExtraT2(std::move(ex2_))
		{}
		ObjectDual2(nullptr_t) : Object(nullptr), ObjExtraT1(nullptr), ObjExtraT2(nullptr)
		{}
	};
	template<class T>
	constexpr bool is_esb_object_dual2 = requires { 
		typename T::dual1_t; 
		typename T::dual2_t; 
		requires std::is_base_of_v<ObjectDual2<T::dual1_t, T::dual2_t>, T>;
	};


	template<class ObjExtraT>
	class ValueDual : public Value, protected ObjExtraT {
	public:
		using dual_t = ObjExtraT;
		using dual_interface_t = dual_t::interface_t;
	protected:
		ValueDual(IValuePtr&& val_, dual_interface_t&& ext_) : Value(std::move(val_)), ObjExtraT(std::move(ext_))
		{}
		ValueDual(nullptr_t) : Value(nullptr), ObjExtraT(nullptr)
		{}
	};
	template<class T>
	constexpr bool is_esb_value_dual = requires { 
		typename T::dual_t;
		requires std::is_base_of_v<ValueDual<T::dual_t>, T>;
	};


	//INFO	Все запросы интерфейсов происходят в esbhlp. Библиотека экспортирует явные специализации query_interface_or_throw 
	//		для всех необходимых в реализации интерфейсов.
	//		Подчеркиваю - запросы делаются только для известных объектов и интерфейсов, а не с целью экзаменовать объекты на их наличие
	//		нуллптр не возвращается - нет интерфейса будет исключение.
	template<class InterfacePtrT>
	extern InterfacePtrT query_interface_or_throw(IUnknown& source_);
	//NOTE	DelegatToMeth использует query_interface_or_throw напрямую минуя query_interface<class EsbClassT, class InterfacePtrT>
	//		т.к. он не знает объекта к которому его прикрепляют. Для такого случая нужно непосредственно указать, что существует специализация
	//		query_interface_or_throw<IObjectPtr> и эта специализация extern. Иначи компилятор автоматом создает эту специализацию несмотря на
	//		то что шаблон помечен extern и без определения. И потом дает ошибку, что явная специализация уже создана.
	//		(extern у декларации самого template, говорит только что само его определение extern, а где инстансы не говорит)
	//		На всякий случай сделаем указание extern для всех интерфейсов используемых esb
#define ESB_DECLARE_EXTERN_QUERY_INTERFACE_OR_THROW(IFACE_PTR_)							\
	template<> IFACE_PTR_ query_interface_or_throw<IFACE_PTR_>(IUnknown& source_);		\
	extern template IFACE_PTR_ query_interface_or_throw<IFACE_PTR_>(IUnknown& source_)
	//->
	//TOBE	При расширении списка интерфейсов нужно делать .inc файл как для типов и т.п. (в esbhlp и coreall такая же портянка)
	ESB_DECLARE_EXTERN_QUERY_INTERFACE_OR_THROW(IValuePtr);
	ESB_DECLARE_EXTERN_QUERY_INTERFACE_OR_THROW(IObjectPtr);
	ESB_DECLARE_EXTERN_QUERY_INTERFACE_OR_THROW(IAxCollectionPtr);
	ESB_DECLARE_EXTERN_QUERY_INTERFACE_OR_THROW(IAxCollectionROPtr);
	ESB_DECLARE_EXTERN_QUERY_INTERFACE_OR_THROW(IAxCollectionRWPtr);
	ESB_DECLARE_EXTERN_QUERY_INTERFACE_OR_THROW(IValuePairPtr);
	ESB_DECLARE_EXTERN_QUERY_INTERFACE_OR_THROW(IIxCollectionPtr);
	ESB_DECLARE_EXTERN_QUERY_INTERFACE_OR_THROW(IIxCollectionROPtr);
	ESB_DECLARE_EXTERN_QUERY_INTERFACE_OR_THROW(IIxCollectionRWPtr);
	ESB_DECLARE_EXTERN_QUERY_INTERFACE_OR_THROW(IObjectSingletonPtr);
	ESB_DECLARE_EXTERN_QUERY_INTERFACE_OR_THROW(IEnumValuesPtr);
#undef ESB_DECLARE_EXTERN_QUERY_INTERFACE_OR_THROW
	

	//INFO	Поскольку чаще всего мы запрашиваем интерфейсы у известных нам объектов для которых мы
	//		абсолютно уверены в наличии этих интерфейсов, то проще, быстрее и безопаснее не запрашивать их через QueryInterface, а просто получить
	//		зная бинарную структуру этих объектов. Для примера такое сделано для Array. Для тех объектов и интерфейсов для кого такое не сделано
	//		уходим на стандартный запрос через QueryInterface.
	template<class EsbClassT, class InterfacePtrT>
	InterfacePtrT query_interface(IValue& source_) {
		return query_interface_or_throw<InterfacePtrT>(source_);
	}

	class Array;
#define ESB_DECLARE_EXTERN_QUERY_INTERFACE(ESB_CLASS_T_, IFACE_PTR_T_)							\
	template<> IFACE_PTR_T_ query_interface<ESB_CLASS_T_, IFACE_PTR_T_>(IValue& source_);		\
	extern template IFACE_PTR_T_ query_interface<ESB_CLASS_T_, IFACE_PTR_T_>(IValue& source_)
	//-->
	ESB_DECLARE_EXTERN_QUERY_INTERFACE(Array, IObjectPtr);
	ESB_DECLARE_EXTERN_QUERY_INTERFACE(Array, IIxCollectionRWPtr);
	ESB_DECLARE_EXTERN_QUERY_INTERFACE(Array, IEnumValuesPtr);
#undef ESB_DECLARE_EXTERN_QUERY_INTERFACE



	//NOTE	Если класс свои конструктор(nullptr) и/или is_null() прячет из публичного интерфейса, то должен объявить эту структуру другом.
	template<class T>
	struct is_can_be_nullable_type {
		static constexpr bool value_ = requires(T & t) {
			T{ nullptr };	//requires std::is_constructible_v<T, nullptr_t>; - до протекткед члена добраться не может :(
			{t.is_null()}->std::same_as<bool>;
		};
	};
	template<class T>
	inline constexpr bool is_can_be_nullable = is_can_be_nullable_type<T>::value_;



	template<class EsbClassT>
	inline EsbClassT make(IValuePtr&& val_);

	//TOBE	Nullable
	// для использования ptr-like класса необходимо чтобы у класса был доступен для нас конструктор от nullptr. 
	// конструкция такова, что и объект созданный от interface_t{nullptr} считается nullptr тоже
	// ptr-like гарантирует, что никто не получит доступ к nullptr-объекту
	template<class EsbClassT>
	class ClassOpt : private EsbClassT {
		static_assert(is_can_be_nullable<EsbClassT>, "EsbClassT not satisfy nullable requirements.");
		using me_t = ClassOpt<EsbClassT>;
	public:
		using class_t = EsbClassT;
	private:
		class_t& my_class()						{ return *static_cast<class_t*>(this); }
		const class_t& my_class() const			{ return *static_cast<const class_t*>(this); }
	public:
		//Конструктор от nullptr оставляю публичным. В нем нет специфики запроса разных интерфейсов и make не нужен, а работать приятнее 
		constexpr ClassOpt(nullptr_t) throw() : class_t{ nullptr }
		{}
		//NOTE	Конструкто по умолчанию не сделан намеренно, чтобы явно указывалось чего хочется.
	private:
		// Это конструктор для обычного создания Opt-EsbClassT класса через make. Мы слепо делегируем интерфейсы есб-классу.
		template<class EsbClassT>
		friend EsbClassT make<ClassOpt<EsbClassT>>(IValuePtr&&);
		template<InterfaceConcept... InterfaceT>
		ClassOpt(IValuePtr&& val_, InterfaceT&&... ifc_) throw() : class_t{ std::move(val_), std::move(ifc_)... }
		{}
		// этот конструктор для классов non-value-based есб-классов
		template<class ClassT, InterfaceConcept InterfaceConceptT>
		friend ClassOpt<ClassT> make_opt(InterfaceConceptT&& ifptr_);
		template<InterfaceConcept EsbInterfaceT> requires (!is_esb_value_based<EsbClassT>)
		ClassOpt(EsbInterfaceT&& interface_) throw() : class_t{ std::move(interface_) }
		{}
		//TOBE	Здесь еще нужен конструктор для любых is_nullable классов с полным слепым делегированием аргументов. По возможности/необходимости нужно сделать.
	public:
		bool is_null() const					{ return my_class().is_null(); }
		bool is_ok() const						{ return !is_null();  }
		explicit operator bool() const			{ return is_ok(); }
		//NOTE	От возможного nullptr мы здесь защищаемся ассертом, а не throw. Мы - указатель. Ваш алгоритм должен быть проверен в debug!
		class_t* operator->()					{ ESB_ASSERT(is_ok()); return &my_class(); }
		const class_t* operator->() const		{ ESB_ASSERT(is_ok()); return &my_class(); }
		class_t& operator*() &					{ ESB_ASSERT(is_ok()); return my_class(); }
		class_t&& operator*() &&				{ ESB_ASSERT(is_ok()); return std::move(my_class()); }
		const class_t& operator*() const &		{ ESB_ASSERT(is_ok()); return my_class(); }
		//NOTE	Оператор преобразования не делаем, т.к. указатель к объекту не преобразуется
	public:
		//NOTE	Защита от самоприсваивания - дело самого класса, а не наше. Для есб-классов основанных на InterfacePtr все ок - InterfacePtr проверяет.
		me_t& operator=(const class_t& other_) {
			my_class() = other_;
			return *this;
		}
		me_t& operator=(class_t&& other_) {
			my_class() = std::move(other_);
			return *this;
		}
		me_t& operator=(const nullptr_t&) {
			my_class() = class_t{ nullptr };
			return *this;
		}
	};

	template<class EsbClassT>	inline constexpr bool is_esb_class_opt = false;
	template<class EsbClassT>	inline constexpr bool is_esb_class_opt<ClassOpt<EsbClassT>> = true;


	//TOBE	Не могу определиться Nullable, Optional или пусть так остается? Nullable конкретно отражает суть ptr-like интерфейса. Opt как в с++, может OptNullable? Ptr?
	template<class T>
	using Nullable = ClassOpt<T>;


#define ESB_CLASS_DECLARE_NULLABLE(CLASS_T_)			\
	friend is_can_be_nullable_type<CLASS_T_>;			\
	friend is_can_be_nullable_type<const CLASS_T_>;		\
	friend ClassOpt<CLASS_T_>;							\
	friend ClassOpt<const CLASS_T_>

// make_opt создается с nullptr и он должен вызвать приватный конструктор целевого класса от nullptr.
// поэтому а) конструктор от nullptr_t должен быть и б) должен быть доступен для ClassOpt
// (пс. конструктор от nullptr_t должен быть строго приватным т.к. нигде более не предполагается, что интерфейс у класса может быть nullptr)
#define ESB_CLASS_IMPLEMENT_MAKE_OPT(CLASS_T_)			\
	ESB_CLASS_DECLARE_NULLABLE(CLASS_T_);				\
	using Value::is_null;								\
	CLASS_T_(nullptr_t) noexcept : base_t(nullptr) {}



	template<class EsbClassT>
	inline bool check_interface(const Value& value_);
	template<class EsbClassT>
	inline EsbClassT check_and_make_copy(const IValuePtr& value_);
	template<class EsbClassT>
	inline ClassOpt<EsbClassT> make_opt(const Value& val_);
	template<class EsbClassT, InterfaceConcept InterfaceConceptT>
	inline ClassOpt<EsbClassT> make_opt(InterfaceConceptT&& ifptr_);
	template<EsbValueSimpleConcept EsbClassT>
	inline EsbClassT& check_and_cast(Value& value_);
	template<EsbValueSimpleConcept EsbClassT>
	inline const EsbClassT& check_and_cast(const Value& value_);
	template<EsbValueSimpleConcept EsbClassT>
	inline EsbClassT&& check_and_cast(Value&& value_);



	class Arbitrary : public Value {
		friend Arbitrary make<Arbitrary>(IValuePtr&&);
		Arbitrary(IValuePtr&& ival_) : Value(std::move(ival_))
		{}
	public:
		template<class EsbClassT>					bool				is()					{ return check_interface<EsbClassT>(*this); } 
		// Поскольку мы базируемся только на Value, представиться мы можем только им.
		template<EsbValueSimpleConcept EsbClassT>	EsbClassT&			as_value() &			{ return check_and_cast<EsbClassT>(*this);	}
		template<EsbValueSimpleConcept EsbClassT>	const EsbClassT&	as_value() const &		{ return check_and_cast<EsbClassT>(*this);	}
		template<EsbValueSimpleConcept EsbClassT>	EsbClassT&&			as_value()&&			{ return check_and_cast<EsbClassT>(std::move(*this));	}
		// создать копию можем любого значения\объекта
		template<EsbClassConcept EsbClassT>			EsbClassT			make() const &			{ return check_and_make_copy<EsbClassT>(get_interface(*this));	}
		template<EsbClassConcept EsbClassT>			EsbClassT			make() &&				{ return check_and_make<EsbClassT>( get_interface(std::move(*this)) ); }
		template<class EsbClassT>					ClassOpt<EsbClassT>	make_if() const			{ return make_opt<EsbClassT>(*this); }
#if ESB_USE_RUSSIAN
		template<class EsbClassT>					bool				Это()					{ return check_interface<EsbClassT>(*this); }
		template<EsbValueSimpleConcept EsbClassT>	EsbClassT&			КакЗначение() &			{ return check_and_cast<EsbClassT>(*this);	}
		template<EsbValueSimpleConcept EsbClassT>	const EsbClassT&	КакЗначение() const &	{ return check_and_cast<EsbClassT>(*this);	}
		template<EsbValueSimpleConcept EsbClassT>	EsbClassT&&			КакЗначение()&&			{ return check_and_cast<EsbClassT>(std::move(*this));	}
		template<EsbClassConcept EsbClassT>			EsbClassT			Сделать() const&		{ return check_and_make_copy<EsbClassT>(get_interface(*this)); }
		template<EsbClassConcept EsbClassT>			EsbClassT			Сделать()&&				{ return check_and_make<EsbClassT>(get_interface(std::move(*this))); }
		template<class EsbClassT>					ClassOpt<EsbClassT>	СделатьЕсли() const		{ return make_opt<EsbClassT>(*this); }
#endif
	};

	template<class EsbClassT>
	constexpr bool is_esb_arbitrary = std::is_same_v< std::remove_cvref_t<EsbClassT>, Arbitrary>;

	inline Value::operator const Arbitrary& () const&	{ return static_cast<const Arbitrary&>(*this); }
	inline Value::operator Arbitrary&& () &&			{ return static_cast<Arbitrary&&>(*this); }

	// Имплементируется для тех классов у которых есть Default_ (см. после Undef)
	template<class EsbClassT>
	Arbitrary MakeArbitraryDefault() = delete;
	// реализация после Undef
	template<> Arbitrary MakeArbitraryDefault<Undef>();


	template<class... EsbClasses>
	class ClassMix : public Value{
		friend ClassMix<EsbClasses...> make<ClassMix<EsbClasses...>>(IValuePtr&&);
		ClassMix(IValuePtr&& ival_) : Value(std::move(ival_))
		{}
	public:
		using Types = typepack<EsbClasses...>;
	public:
		template<class EsbClassT>	requires is_type_in_list<EsbClassT, EsbClasses...> 
		bool				is() const			{ return check_interface<EsbClassT>(*this);}
		template<class EsbClassT> 	requires is_type_in_list<EsbClassT, EsbClasses...>
		EsbClassT			make() const		{ return check_and_make_copy<EsbClassT>(get_interface(*this)); }
		template<class EsbClassT> 	requires is_type_in_list<EsbClassT, EsbClasses...>
		ClassOpt<EsbClassT>	make_if() const		{ return make_opt<EsbClassT>(*this); }
		template<class EsbClassT>
		requires is_esb_value_simple<EsbClassT> && is_type_in_list<EsbClassT, EsbClasses...>
		EsbClassT& as() &						{ return check_and_cast<EsbClassT>(*this);		}
		template<class EsbClassT>
		requires is_esb_value_simple<EsbClassT> && is_type_in_list<EsbClassT, EsbClasses...>
		const EsbClassT& as() const &			{ return check_and_cast<EsbClassT>(*this);		}
		template<class EsbClassT>
		requires is_esb_value_simple<EsbClassT> && is_type_in_list<EsbClassT, EsbClasses...>
		EsbClassT&& as()&&						{ return check_and_cast<EsbClassT>(std::move(*this));	}
	};

	template<typename>
	inline constexpr bool is_esb_mix = false;
	template<typename... EsbClasses>
	inline constexpr bool is_esb_mix<ClassMix<EsbClasses...>> = true;

	//TOBE: UndefOr используется для индикации опт-значения аргумента. Ввиду этого возможно целесообразно не просто алаис, а класс с дополнительным оператор-буул 
	//		если Undef:false Else:true плюс оператор * для не-Undef содержимого...
	//		Фактически ClassOpt получается... Только там invalid_state == nullptr, а здесь == Undef
	//INFO	Параметр в диспатч-лайк-интерфейсе считается опт-параметром если Undef первый. Т.е. ClassMix<Undef, Numeric> - опт, а ClassMix<Numeric, Undef> - не-опт.
	template<class EsbClassT>
	using UndefOr = ClassMix<Undef, EsbClassT>;



//NOTE	На создание в функцию make может подаваться как есб-класс напрямую, так и "завернутый" в класс-опт
//		(класс-опт имеет слепой делегирующий конструктор на реальный класс, который он за собой прячет)
//		поэтому нам надо выяснить что собственно нам заказали сделать
	template<class EsbClassT>
	struct esb_class_self_or_opt_inner_esb_class { using type = EsbClassT; };
	template<class EsbClassOptInnerT>
	struct esb_class_self_or_opt_inner_esb_class<ClassOpt<EsbClassOptInnerT>> { using type = EsbClassOptInnerT; };

//INFO	Функция make единственное место где создаются есб-объекты. Она ничего не проверяет. Ее задача запросить необходимые интерфейсы и вызвать конструктор.
//		Для попытки создания с проверкой типа того, что есть IValue и что нужно создать используются check_and_make
	template<class EsbClassT>
	EsbClassT make(IValuePtr&& val_) {
		using class_t = esb_class_self_or_opt_inner_esb_class<EsbClassT>::type;

		if constexpr (is_esb_object_dual2<class_t>) {
			IObjectPtr obj = query_interface<class_t, IObjectPtr>(*val_);
			typename class_t::dual1_interface_t ex1 = query_interface<class_t, class_t::dual1_interface_t>(*val_);
			typename class_t::dual2_interface_t ex2 = query_interface<class_t, class_t::dual2_interface_t>(*val_);
			return EsbClassT{ std::move(val_), std::move(obj), std::move(ex1), std::move(ex2) };
		}
		else if constexpr (is_esb_object_dual<class_t>) {
			IObjectPtr obj = query_interface<class_t, IObjectPtr>(*val_);
			typename class_t::dual_interface_t ext = query_interface<class_t, class_t::dual_interface_t>(*val_);
			return EsbClassT{ std::move(val_), std::move(obj), std::move(ext) };
		}
		else if constexpr (is_esb_object_simple<class_t>) {
			IObjectPtr obj = query_interface<class_t, IObjectPtr>(*val_);
			return EsbClassT{ std::move(val_), std::move(obj) };
		}
		else if constexpr (is_esb_value_dual<class_t>) {
			typename class_t::dual_interface_t ext = query_interface<class_t, class_t::dual_interface_t>(*val_);
			return EsbClassT{ std::move(val_), std::move(ext) };
		}
		else { //simple value or ClassMix - ClassMix просто унаследован от Value
			static_assert(is_esb_value_simple<class_t>);
			return EsbClassT{ std::move(val_) };
		}
	}

	template<class EsbClassT, InterfaceConcept SomeInterfaceT>
	EsbClassT make_from_dual(SomeInterfaceT&& ixx_) 
	{
		if constexpr (std::is_same_v<SomeInterfaceT, IValuePtr>)
			return make<EsbClassT>(std::move(ixx_));


		using class_t = esb_class_self_or_opt_inner_esb_class<EsbClassT>::type;
		IValuePtr val = query_interface_or_throw<IValuePtr>(*ixx_);

		auto qry_interface = [&ixx_, &val]<class IXPtrT>() -> IXPtrT {
			if constexpr (std::is_same_v<SomeInterfaceT, IXPtrT>)
				return std::move(ixx_);
			else
				return query_interface<class_t, IXPtrT>(*val);
		};
#define QRY_IPTR(IPTR_T_)	qry_interface.operator()<IPTR_T_>()

		if constexpr (is_esb_object_dual2<class_t>) {
			using ix1_t = typename class_t::dual1_interface_t;
			using ix2_t = typename class_t::dual2_interface_t;
			return EsbClassT{ std::move(val), QRY_IPTR(IObjectPtr), QRY_IPTR(ix1_t), QRY_IPTR(ix2_t) };
		}
		else if constexpr (is_esb_object_dual<class_t>) {
			using ixx_t = typename class_t::dual_interface_t;
			return EsbClassT{ std::move(val), QRY_IPTR(IObjectPtr), QRY_IPTR(ixx_t) };
		}
		else if constexpr (is_esb_object_simple<class_t>) {
			return EsbClassT{ std::move(val), QRY_IPTR(IObjectPtr) };
		}
		else if constexpr (is_esb_value_dual<class_t>) {
			using ixx_t = typename class_t::dual_interface_t;
			return EsbClassT{ std::move(val), QRY_IPTR(ixx_t) };
		}
		else { //simple value or ClassMix - ClassMix просто унаследован от Value
			static_assert(is_esb_value_simple<class_t>);
			return EsbClassT{ std::move(val) };
		}
#undef QRY_IPTR
	}


//NOTE	base_t используется в ESB_CLASS_IMPLEMENT_MAKE_OPT - он должен быть после ESB_CLASS_IMPLEMENT_MAKE...
//		(внутри класса используемое имя шаблона от которого наследуемся развернется в полную спецификацию, т.е. если Array : ObjectDual<IxCollectionRW<Arbitrary>>
//		то при using base_t = ObjectDual	base_t :-> ObjectDual<IxCollectionRW<Arbitrary>>)
//TOBE	Если расширять список возможных базовых классов\интерфейсов, то CLASS_BASE_T_ нужно передавать сюда и делать единый ESB_CLASS_IMPLEMENT_MAKE(CLASS_T_, CLASS_BASE_T_)
//		(список для вызова base_t(std::move(val_), std::move(obj_) ...) можно сделать доп.макросом с контактацией с CLASS_BASE_T_)
//		Но пока даже необходимость ObjectDual2 под вопросом т.к. пока везде функциональность AxCollection дублируется DynamicCollection и качественней.
#define ESB_CLASS_IMPLEMENT_MAKE_VAL(CLASS_)					\
	using base_t = Value;										\
	friend CLASS_ make<CLASS_>(IValuePtr&&);					\
	CLASS_(IValuePtr&& val_) noexcept : base_t(std::move(val_)) {}

#define ESB_CLASS_IMPLEMENT_MAKE_OBJ(CLASS_)					\
	using base_t = Object;										\
	friend CLASS_ make<CLASS_>(IValuePtr&&);					\
	CLASS_(IValuePtr&& val_, IObjectPtr&& obj_) noexcept : base_t(std::move(val_), std::move(obj_)) {}

#define ESB_CLASS_IMPLEMENT_MAKE_OBJ_DUAL(CLASS_)				\
	using base_t = ObjectDual;									\
	friend CLASS_ make<CLASS_>(IValuePtr&&);					\
	CLASS_(IValuePtr&& val_, IObjectPtr&& obj_, dual_interface_t&& ixx_) noexcept : base_t(std::move(val_), std::move(obj_), std::move(ixx_)) {}

#define ESB_CLASS_IMPLEMENT_MAKE_VAL_DUAL(CLASS_)				\
	using base_t = ValueDual;									\
	friend CLASS_ make<CLASS_>(IValuePtr&&);					\
	CLASS_(IValuePtr&& val_, dual_interface_t&& ixx_) noexcept : base_t(std::move(val_), std::move(ixx_)) {}

#define ESB_CLASS_IMPLEMENT_MAKE_OBJ_DUAL2(CLASS_)																\
	using base_t = ObjectDual2;																					\
	friend CLASS_ make<CLASS_>(IValuePtr&&);																	\
	CLASS_(IValuePtr&& val_, IObjectPtr&& obj_, dual1_interface_t&& ix1_, dual2_interface_t&& ix2_) noexcept	\
			: base_t(std::move(val_), std::move(obj_), std::move(ix1_), std::move(ix2_)) {}


	
	template<class EsbClassT>
	struct interface_checker {
		static bool check(IValue& value_) {
			return EsbClassT::Type_.check_value(value_);
		}
		static bool check(IType& value_type_) {
			return EsbClassT::Type_.check_value_type(value_type_);
		}
	};
	template<class... EsbClassesT>
	struct interface_checker<ClassMix<EsbClassesT...>> {
		static bool check(IValue& value_) {
			IType* pt = IValue_GetType(value_);
			ESB_ASSERT(pt);
			return check(*pt);
		}
		static bool check(IType& value_type_) {
			//NOTE	Внутри ClassMix<> могут быть еще ClassMix<> - н.р. ClassMix<Numeric, ClassMix<String, Undef>>, поэтому мы рекурсивно "углубляемся" в список типов 
			return (interface_checker<EsbClassesT>::check(value_type_) || ...);
		}
	};


	template<class EsbClassT>
	inline bool check_interface(IValue& value_) {
		return interface_checker<EsbClassT>::check(value_);
	}
	template<class EsbClassT>
	inline bool check_interface(const Value& value_) {
		return check_interface<EsbClassT>(*get_interface(value_));
	}

	template<class EsbClassT>
	inline void check_interface_or_throw(IValue& value_) {
		if constexpr (!is_esb_arbitrary<EsbClassT>) {
			if (!check_interface<EsbClassT>(value_)) ESL_THROW_VALUE_TYPE_MISMATCH();
		}
	}
	template<class EsbClassT>
	inline void check_interface_or_throw(IValue* pvalue_) {
		ESB_ASSERT(pvalue_);
		return check_interface_or_throw<EsbClassT>(*pvalue_);
	}
	template<class EsbClassT>
	inline void check_interface_or_throw(const Value& value_) {
		return check_interface_or_throw<EsbClassT>(*get_interface(value_));
	}
	template<class EsbClassT>
	EsbClassT check_and_make(IValuePtr&& value_) {
		check_interface_or_throw<EsbClassT>(*value_);
		return make<EsbClassT>(std::move(value_));
	}
	template<EsbClassConcept EsbClassT>
	EsbClassT check_and_make_from_var(const IVariable* pvar_) {
		ESB_ASSERT(pvar_);
		return check_and_make<EsbClassT>(IVariable_GetValue(*pvar_));
	}
	//	
	template<class EsbClassT>
	EsbClassT make_copy(const IValuePtr& val_) {
		return make<EsbClassT>(IValuePtr{ val_ });
	}
	template<class EsbClassT>
	EsbClassT check_and_make_copy(const IValuePtr& value_) {
		check_interface_or_throw<EsbClassT>(*value_);
		return make_copy<EsbClassT>(value_);
	}
	template<EsbValueSimpleConcept EsbClassT>
	EsbClassT& check_and_cast(Value& value_) {
		check_interface_or_throw<EsbClassT>(value_);
		return static_cast<EsbClassT&>(value_);
	}
	template<EsbValueSimpleConcept EsbClassT>
	const EsbClassT& check_and_cast(const Value& value_) {
		check_interface_or_throw<EsbClassT>(value_);
		return static_cast<const EsbClassT&>(value_);
	}
	template<EsbValueSimpleConcept EsbClassT>
	EsbClassT&& check_and_cast(Value&& value_) {
		check_interface_or_throw<EsbClassT>(value_);
		return std::move(static_cast<EsbClassT&&>(value_));
	}
	template<class EsbClassT>
	ClassOpt<EsbClassT> make_opt(const Value& val_) {
		const auto& value_ref = get_interface(val_);
		if (check_interface<EsbClassT>(*value_ref))
			return make_copy<ClassOpt<EsbClassT>>(value_ref);
		else
			return ClassOpt<EsbClassT>{nullptr};
	}
	template<class EsbClassT, InterfaceConcept InterfaceConceptT>
	ClassOpt<EsbClassT> make_opt(InterfaceConceptT&& ifptr_) {
		if (ifptr_)
			return ClassOpt<EsbClassT>{ std::move(ifptr_) };
		else
			return ClassOpt<EsbClassT>{nullptr};
	}

}



// теперь определяем типы для параметров методов 1С объектов и АПИ
// 
// Все приходящие от платформы значения (и уходящие от нас, соответственно) несут на себе интерфейс IVariable c виртуальными методами доступа к значению
// (он у них по другому называется, но, как мне кажется, мне удалось назвать эту сущность в точности с ее предназначением)
// Но для работы в с++ с собственно значениями никакого дополнительного "виртуального" интерфейса не надо. он только мешается. компилятору лучше точно и безо всякого
// интерфейса-фасада знать с чем он работает.
// ЕСБ в первую очередь предназначена для получения данных от платформы и максимально эффективной обработки их методами с++. таскать с собой этот самый IVariable
// внутри с++ алгоритмов - это просто деградация и черевато.
// Поэтому мы, при получении данных, с них этот IVariable снимаем, внутри работаем напрямую, а при передаче назад в платформу одеваем IVariable назад.
// В такой методике нехорошо то, что если алгоритм требует активного обмена с платформой, то снять-одеть-снять-одеть... это тоже деградация.
// Для решения этой проблемы разработан шаблон Var<>, который имеет и IVariable интерфейс и публично унаследован от нужного типа.
// То есть при использовании Var с++ работает со своим видением данных, а 1С со своим. Постоянного снять-одеть нет и возможен прирост производительности до 10-20%.
// Но подчеркиваю - активный обмен с платформой - это не может быть основной стратегией использования ЕСБ. Деградация на самом диспатч-протоколе передачи аргументов
// угробит всякую производительность. Но возможность оптимизации используя Var<> у Вас есть.
// 
// У реализации IVariable от 1С нет никакого понятия о константности аргумента. Если смотреть только на интерфейс, то можно опасаться, что платформа Вам может хоть
// индекс массива прямо посередь цикла поменять. Поэтому мы сразу и строго разделяем IVariable на разные классы 
// IVariableByVal - гарантировано константная имплементация интерфейса и IVariableByRef - имплементация разрешающая платформе переустановить у нас значение.
// И дальше строго следим чтобы все параметры объявлялись или ..ByVal (const терминах c++) или ..ByRef (RefParam, в терминах с++ `&` - возможность изменения)
// и никогда просто IVariable с невнятной константностью.
// (пс. разбираться где конст\не-конст приходится фактически вручную, по хелпу, интуиции и тестам. технической документации нет)
// 
namespace esb {
	template<class EsbClassT>
	class IVariableByRef : public IVariable {};

	template<class EsbClassT>
	class IVariableByVal : public IVariable {};


	template<class EsbClassT>
	class Var;
#if ESB_USE_RUSSIAN
	template<class EsbClassT>	using Перем = Var<EsbClassT>;
#endif
	template<class EsbClassT>
	struct template_argument_type<Var<EsbClassT>> { using type = EsbClassT; };


	template<class T>
	using ConstPara = const IVariableByVal<const T>&;
	using ConstParaAny = ConstPara<Arbitrary>;

	template<class T>
	using RefPara = const IVariableByRef<T>&;
	using RefParaAny = RefPara<Arbitrary>;
}



// теперь имплементацию классов аргументов
#ifdef ESB_USE_NATIVE
#include "esb_core_native.h"
#else
namespace esb {
//
	
// 
//INFO	esbhlp и имплементация IVariable
//		Структуры var_layout_ вообще не предназначены для создания\копирования\тп - только для проверки того, что esb и esbhlp правильно понимают друг друга
//		здесь, в "пользовательском" коде всевозможные VarArgument.. создаются, а в esbhlp им назначается правильная vtbl для IVariable. 
//		такая вот вынужденная "баня - через дорогу раздевалка" ввиду того, что что такое "IVariable" недокументировано
//		(можно быдл бы вообще void* использовать, но со структурами надежнее. убедился)
//		(ESB_WARN_NO_DEFAULT_CTOR потому что у Value нет конструктора по-умолчанию. он только для конкретных наследников, а esbhlp об этих наследниках ничего не знает
//		но твердо знает, что они все унаследованы от Value)
//		все var_layout_...::init_variable... определены в esbhlp. внутри своих структур они сделаны с целью не загромождать область видимости деталями реализации.
ESB_WARNING_SUPRESS( ESB_WARN_NO_OPERATOR_ASSIGN_ANY  ESB_WARN_NO_DEFAULT_CTOR )
//
// Поскольку esbhlp уже скомпилирована с именно такими структурами, а в процессе дальнейшей (пере,раз)работки есб что-то может меняться
// мы метим библиотеку esbhlp detect_mismatch директивами линкера с теми параметрами на которые esbhlp расчитывает.
// (ниже у каждый VarArgument...Impl проверяет себя на соответствие со своей структурой)
	struct var_layout_on_this : public IVariable, public Value {
		static void init_variable(var_layout_on_this&);
	};
#define ESB_VAR_LAYOUT_ON_THIS_VALUE_OFFSET		ESB_POINTER_SIZE
ESB_CHECK_AND_DETECT_MISMATCH(ESB_VAR_LAYOUT_ON_THIS_VALUE_OFFSET, ( esb_value_based_value_offset<var_layout_on_this>() ))

//NOTE	Поля val_ структур (и соответствующие им поля классов VarArgument...Impl) декларированы как указатель на Value, 
//		хотя по сути это ссылки, которые никогда не должны быть nullptr	(не-нулл для соответствующих VarArgument...Impl обеспечивается конструктором)
//		Но возникла проблема с offsetof для ссылочных полей - компилятор не дает их получить (*доступ к хранилищу среды исполнения и т.п.*)
//		Поэтому пришлось сделать указателями.
	struct var_layout_on_that_byval : public IVariable { 
		const Value* val_;
		static void init_variable_ByValOnThat(var_layout_on_that_byval&);
		static void init_variable_ByRefOnThatIgnore(var_layout_on_that_byval&);
	};
#define ESB_VAR_LAYOUT_ON_THAT_VALUE_OFFSET		ESB_POINTER_SIZE
ESB_CHECK_AND_DETECT_MISMATCH(ESB_VAR_LAYOUT_ON_THAT_VALUE_OFFSET, ( offsetof(var_layout_on_that_byval, val_) ))

	using	var_byref_assigner_fn = void(IVariable::*)(IValue*);
	struct	var_layout_on_that_byref : public IVariable { 
		/*const*/ Value* val_; 
		var_byref_assigner_fn assigner_; 
		static void	init_variable(var_layout_on_that_byref&);
	};
	static_assert(offsetof(var_layout_on_that_byref, val_) == ESB_VAR_LAYOUT_ON_THAT_VALUE_OFFSET);
// мы вынуждены задавать просто цифрами т.к. препроцессор ничего не вычисляет, а подключать буст не хочется
#define ESB_VAR_LAYOUT_ON_THAT_ASSIGN_OFFSET	8
	ESB_CHECK_AND_DETECT_MISMATCH(ESB_VAR_LAYOUT_ON_THAT_ASSIGN_OFFSET, ( offsetof(var_layout_on_that_byref, assigner_) ))
//
ESB_WARNING_RESTORE()	//ESB_WARN_NO_OPERATOR_ASSIGN_ANY  ESB_WARN_NO_DEFAULT_CTOR


	template<class VarT>
	class VarArgumentByValOnThisImpl : public IVariableByVal<const template_argument_type_t<VarT>> {
		using me_t = VarArgumentByValOnThisImpl<VarT>;
	protected:
		VarArgumentByValOnThisImpl(VarT& var_self_)
		{
			static_assert(	std::is_base_of_v<me_t, VarT> &&												// то что нам дали как VarT как-то унаследовано от нас
							std::is_base_of_v<Value, VarT> &&												// также оно как-то унаследовано от Value
							esb_value_based_value_offset<VarT>() == ESB_VAR_LAYOUT_ON_THIS_VALUE_OFFSET,	// и Value находится там где esbhlp ожидает
							"VarT incompatable.");															// значит можно смело преобразоваться
			var_layout_on_this::init_variable(static_cast<var_layout_on_this&>(static_cast<IVariable&>(var_self_)));
		}
	public:
		// мы не даем преобразования к operator IVariableByVal<[const] Arbitrary>& - это, при необходимости, может сделать наш наследник - ему виднее
	};



// Классы ...OnThat... by-desin предназначены исключительно для операторов преобразования к IVariable* для передачи агрументов платфоме
// поэтому делаем их максимально not-user-friendly (ESB_DECLARE_NOCOPYMOVE)
ESB_WARNING_SUPRESS(ESB_WARN_NO_OPERATOR_ASSIGN_ANY ESB_WARN_NO_DEFAULT_CTOR)
//
	template<class EsbClassT>
	class VarArgumentByValOnThatImpl: public IVariableByVal<EsbClassT> 
	{	ESB_DECLARE_NOCOPYMOVE(VarArgumentByValOnThatImpl);
		const Value* m_value;
	protected:
		VarArgumentByValOnThatImpl(const Value& val_) : m_value(&val_)
		{
			static_assert(offsetof(var_layout_on_that_byval, val_) == offsetof(VarArgumentByValOnThatImpl<EsbClassT>, m_value), "incompatable var_layout_on_that_byval.");
			var_layout_on_that_byval::init_variable_ByValOnThat( static_cast<var_layout_on_that_byval&>( static_cast<IVariable&>(*this) ) );
		}
	};

	
	template<class EsbClassT>
	class VarArgumentByRefOnThatAssignImpl : public IVariableByRef<EsbClassT> 
	{	ESB_DECLARE_NOCOPYMOVE(VarArgumentByRefOnThatAssignImpl);
		Value*					m_value;
		var_byref_assigner_fn	m_assiner;
		void assign(IValue* pv_) {
			esb::check_interface_or_throw<EsbClassT>(pv_);
			static_cast<EsbClassT&>(*m_value) = esb::make<EsbClassT>(IValuePtr{ *pv_ });
		}
	public:
		VarArgumentByRefOnThatAssignImpl(Value& val_) : m_value(&val_), m_assiner(static_cast<var_byref_assigner_fn>(&VarArgumentByRefOnThatAssignImpl<EsbClassT>::assign))
		{
			static_assert(	offsetof(var_layout_on_that_byref, val_) == offsetof(VarArgumentByRefOnThatAssignImpl<EsbClassT>, m_value) &&
							offsetof(var_layout_on_that_byref, assigner_) == offsetof(VarArgumentByRefOnThatAssignImpl<EsbClassT>, m_assiner),
							"incompatable var_layout_on_that_byref.");
			var_layout_on_that_byref::init_variable( static_cast<var_layout_on_that_byref&>(static_cast<IVariable&>(*this)) );
		}
	};


	template<class EsbClassT>
	class VarArgumentByRefOnThatIgnoreImpl: public IVariableByRef<EsbClassT> 
	{	ESB_DECLARE_NOCOPYMOVE(VarArgumentByRefOnThatIgnoreImpl);
		const Value* m_value;
	public:
		VarArgumentByRefOnThatIgnoreImpl(const Value& val_)  throw() : m_value(&val_)
		{
			static_assert(offsetof(var_layout_on_that_byval, val_) == offsetof(VarArgumentByRefOnThatIgnoreImpl<EsbClassT>, m_value), "incompatable var_layout_on_that_byval.");
			var_layout_on_that_byval::init_variable_ByRefOnThatIgnore( static_cast<var_layout_on_that_byval&>(static_cast<IVariable&>(*this)) );
		}
	};
//
ESB_WARNING_RESTORE()	//disable : 4626 5027


//Наши VarArgument никогда не бывают не-in с точки зраения 1С и никогда не бывают out с точки зрения 1С (то есть мы не делаем _SetValue для IVariable аргументов)
//см. примечание о аргументах в esb_ext. Здесь как заметку чтобы не забыть где это испельзуется оставлю константы.
#define ESB_ARGUMENT_ALWAIS_IN_TRUE		true
#define ESB_ARGUMENT_ALWAIS_OUT_FALSE	false

}	//namespace esb
#endif	//#ifdef ESB_USE_NATIVE



namespace esb 
{

ESB_WARNING_SUPRESS(ESB_WARN_NO_OPERATOR_ASSIGN_ANY ESB_WARN_NO_DEFAULT_CTOR)
//
	template<class EsbClassT>
	class VarArgumentByValOnThat : public VarArgumentByValOnThatImpl<const EsbClassT> {
	public:
		// мы можем создаваться для любого EsbClassT, если он не Arbitrary. Если Arbitrary, то срабатывает оператор преобразования и получается зацикливание
		VarArgumentByValOnThat(const EsbClassT& val_) throw() requires (!std::same_as<EsbClassT, const Arbitrary>) : VarArgumentByValOnThatImpl<const EsbClassT>(val_) {}
		// если EsbClassT==Arbitrary, то мы создаемся от любого Value
		VarArgumentByValOnThat(const Value& val_) throw() requires (std::same_as<EsbClassT, const Arbitrary>) : VarArgumentByValOnThatImpl<const EsbClassT>(val_) {}
		// также для любого EsbClassT мы можем создаться из Undef (это для ArgDefaultUndef). 
		// Для Undef этот конструктор выбирается вместо EsbClassT& поскольку он менее ограниченный (вообще без ограничений)
		VarArgumentByValOnThat(const Undef& val_) throw() : VarArgumentByValOnThatImpl<const EsbClassT>(val_) {}
	};


	template<class EsbClassT>
	using VarArgumentByRefOnThatAssign = VarArgumentByRefOnThatAssignImpl<EsbClassT>;


	template<class EsbClassT>
	class VarArgumentByRefOnThatIgnore : public VarArgumentByRefOnThatIgnoreImpl<EsbClassT>
	{
	public:
		VarArgumentByRefOnThatIgnore(const EsbClassT& val_) throw() : VarArgumentByRefOnThatIgnoreImpl<EsbClassT>(val_) {}
		VarArgumentByRefOnThatIgnore(const Undef& val_) throw() : VarArgumentByRefOnThatIgnoreImpl<EsbClassT>(val_) {}
	};
//
ESB_WARNING_RESTORE()	//ESB_WARN_NO_OPERATOR_ASSIGN_ANY ESB_WARN_NO_DEFAULT_CTOR



	inline Value::ESB_DECLARE_ARGBYVAL_OPERATOR(Arbitrary) ESB_IMPLEMENT_ARGBYVAL_OPERATOR_BODY(Arbitrary)


	template<class EsbClassT>
	using ArgDefaultByRef = VarArgumentByRefOnThatIgnore<EsbClassT>;

	template<class EsbClassT>
	using VarArgumentByValDefault = VarArgumentByValOnThat<EsbClassT>;
	template<class EsbClassT>
	using VarArgumentByRefDefault = VarArgumentByRefOnThatIgnore<EsbClassT>;

	template<class EsbClassT>
	class ArgDefaultUndefObj {
	public:
		operator const VarArgumentByValDefault<const EsbClassT>& () const {
			static const VarArgumentByValDefault<const EsbClassT> def_undef{ Undef::Value_ };
			return def_undef;
		}
		operator const VarArgumentByRefDefault<EsbClassT>& () const {
			static const VarArgumentByRefDefault<EsbClassT>  def_undef{ Undef::Value_ };
			return def_undef;
		}
	};

	template<class EsbClassT>
	inline const ArgDefaultUndefObj<EsbClassT> DefUndef{};


// Компилятор предупреждает, что this переданный в конструктор базового класса сырой, неинициализированный, и любая попытка использовать его чтобы что-то
// получить выдаст мусор. Это так и есть, но мы используем этот this для того чтобы прописать правильный vtbl. То есть это by-design нам надо. Роботает.
// Конечно был вариант в самом базовом классе использовать его this, но как-то не получилось. Детально не разбирался.
ESB_WARNING_SUPRESS(ESB_WARN_THIS_FOR_INIT_BASE)
//
	template<class EsbClassT>
	class Var : public VarArgumentByValOnThisImpl<Var<EsbClassT>>, public EsbClassT {
		static_assert(is_esb_value_based<EsbClassT>, "EsbClassT must be Value-based class!");
		// любой EsbClass имеет операторы преобразования к byval-аргументу своего типа и byval-аргументу типа Arbitrary
		// мы их скрываем из публичного интерфейса дабы использовалось прямое преобразование из базового класса без промежуточных объектов
		using EsbClassT::ESB_ARGBYVAL_OPERATOR(EsbClassT);
		using EsbClassT::Value::ESB_ARGBYVAL_OPERATOR(Arbitrary);
	public:
		// мы должны уметь создаваться также как и EsbClassT
		template<typename... ArgsT>	
		requires std::is_constructible_v<EsbClassT, ArgsT...>
		Var(ArgsT... args_) throw() : VarArgumentByValOnThisImpl<Var<EsbClassT>>{*this}, EsbClassT{ std::forward<ArgsT>(args_)... }
		{}
	public:
		// Имплементация VarArgumentByValOnThisImpl такова, что позволяет нам (поскольку мы унаследованы от Value-based класса) представляться 
		// переменной содержащей произвольное значение.
		operator IVariableByVal<const Arbitrary>& () & {
			IVariable* pv = static_cast<IVariable*>(this);
			return *static_cast<IVariableByVal<const Arbitrary>*>(pv);
		}
		operator const IVariableByVal<const Arbitrary>& () const &  {
			const IVariable* pv = static_cast<const IVariable*>(this);
			return *static_cast<const IVariableByVal<const Arbitrary>*>(pv);
		}
		// К && мы не преобразовываемся. Это не требуется, или требует изучения.
		operator IVariableByVal<const Arbitrary>& () && = delete;
	};


	//Var<Arbitrary> требует другой имплементации: 
	//using EsbClassT::ESB_ARGBYVAL_OPERATOR(EsbClassT) дает дублирование декларации и operator IVariableByVal<const Arbitrary>& не нужен.
	template<>
	class Var<Arbitrary> : public VarArgumentByValOnThisImpl<Var<Arbitrary>>, public Arbitrary {
		using Arbitrary::Value::ESB_ARGBYVAL_OPERATOR(Arbitrary);
	public:
		// для удобства создания Var<Arbitrary> для работы с платформой пусть будут конструкторы от Undef и пустой тоже Undef (имплементация после Undef)
		Var(const Undef&) throw();
		Var() throw();
		// ну и если начали делать свои конструкторы, то методы по-умолчанию для Arbitrary компилятор удалил :(. Восстанавливаем.
		Var(const Arbitrary& val_) : VarArgumentByValOnThisImpl<Var<Arbitrary>>{ *this }, Arbitrary{ val_ } 		{}
		Var(Arbitrary&& val_) : VarArgumentByValOnThisImpl<Var<Arbitrary>>{ *this }, Arbitrary{ std::move(val_) }	{}
		Var<Arbitrary>& operator= (const Arbitrary& other_)	{ static_cast<Arbitrary&>(*this) = other_;				return *this; }
		Var<Arbitrary>& operator= (Arbitrary&& other_)		{ static_cast<Arbitrary&>(*this) = std::move(other_);	return *this; }
		
		template<class EsbClassT>
		Var(const EsbClassT& val_) : Var(static_cast<const Arbitrary&>(val_)) {}
		template<class EsbClassT>
		Var(EsbClassT&& val_) : Var( static_cast<Arbitrary&&>(std::move(val_)) ) {}
	};
//
ESB_WARNING_RESTORE()	//ESB_WARN_THIS_FOR_INIT_BASE


	template<class EsbClassT>
	struct ByRefMaker {
		using argument_t = VarArgumentByRefOnThatAssign<EsbClassT>;
		static VarArgumentByRefOnThatAssign<EsbClassT> make(EsbClassT& it) { return VarArgumentByRefOnThatAssign<EsbClassT>{ it }; }
	};
	template<class EsbClassT>
	struct ByRefMaker<Var<EsbClassT>> {	// специализацию нужно делать ПОСЛЕ определения Var!! а то не видит
		using argument_t = VarArgumentByRefOnThatAssign<EsbClassT>;
		static VarArgumentByRefOnThatAssign<EsbClassT> make(EsbClassT& it) { return VarArgumentByRefOnThatAssign<EsbClassT>{ it }; }
	};
	template<class EsbClassT>
	ByRefMaker<EsbClassT>::argument_t ByRef(EsbClassT& it) {
		return ByRefMaker<EsbClassT>::make(it);
	}


	template<class T>
	inline constexpr bool is_arg_byval = false;
	template<class T>
	inline constexpr bool is_arg_byval<VarArgumentByValOnThat<const T>> = true;
	template<class T>
	inline constexpr bool is_arg_byval<IVariableByVal<const T>> = true;
	template<class T>
	inline constexpr bool is_arg_byref = false;
	template<class T>
	inline constexpr bool is_arg_byref<IVariableByRef<T>> = true;

	template<class T>
	concept ArgConcept = (is_arg_byval<T> || is_arg_byref<T>);



	// В некоторых случаях с++ не понимает что от нее хотят, как что преобразовывать и какой оператор вызывать.
	// (в случаях ArgsT... args). в частности у Делегата при вызове Invoke напрямую без класса-прокладки (см. Делегат)
	// поэтому можно принудительно показать что вызывать
	template<class T>
	struct forward_as_arg_t {	// invoke operator and return result byval! Для передаче методу принимающему его как const& и уже далее преобразующему в const IVariable*
		// для всех классов (за исключением явных специализаций ниже) мы навязываем вызов оператора преобразования к VarArgumentByValOnThat
		// который унаследован от IVariableByVal<const T>. Само IVariableByVal мы ссылкой вернуть не можем, т.к. оператор именно создает этот объект
		// ссылку нельзя привязать к временному значению. Но при сипользовании для передачи параметру, который декларирован как const IVariable&
		// компилятор имплисит кастит его и продлевает жизнь этому временному объекту до конца вызова.
		static const VarArgumentByValOnThat<const T> forward(const T& t)					{ return static_cast<const VarArgumentByValOnThat<const T>>(t); }
	};
	template<class T>
	struct forward_as_arg_t<Var<T>> {		//VarArgumentByValOnThis
		static const IVariableByVal<const T>& forward(const Var<T>& t)						{ return t; }	/*implicit cast to base*/
	};
	template<class T>
	struct forward_as_arg_t<VarArgumentByRefOnThatAssign<T>> {
		static const IVariableByRef<T>& forward(const VarArgumentByRefOnThatAssign<T>& t)	{ return t; }	/*implicit cast to base*/
	};
	template<class T>
	struct forward_as_arg_t<VarArgumentByRefOnThatIgnore<T>> {
		static const IVariableByRef<T>& forward(const VarArgumentByRefOnThatIgnore<T>& t)	{ return t; }	/*implicit cast to base*/
	};


	template<class T>
	auto forward_as_arg(const T& t) {
		return forward_as_arg_t<std::remove_cvref_t<T>>::forward(t);
	}
} // namespace esb



namespace esb {
	//
	class TypeValue;

	class TypeDef {
		friend bool OnComponentInitStartup();
	protected:
		mutable IType* m_ptype;
		IType& myType() const { ESB_ASSERT(m_ptype); return *m_ptype; }
		TypeDef(const nullptr_t&) : m_ptype(nullptr)
		{}
		void init(IType& typ_) {
			assert(m_ptype == nullptr);
			m_ptype = &typ_;
		}
	protected:
		IValuePtr invoke_CreateValue() const							{ return IType_CreateValue(myType()); }
		IValuePtr invoke_CreateValue(const argarray_t& args_) const		{ return IType_CreateValue(myType(), args_); }
		bool check_value_type(const IType& type_) const					{ return (&type_ == &myType()); }
		bool check_value(IValue& val_) const							{ IType* pt = IValue_GetType(val_); ESB_ASSERT(pt); return check_value_type(*pt); }
		template<class EsbClassT> friend struct interface_checker;
	public:
		TypeDef(IType& itype_) : m_ptype(&itype_)
		{}
		constexpr bool operator==(const TypeDef& other_) const { return m_ptype == other_.m_ptype; }
		//
		TypeValue Value() const; // в esb.h после TypeValue
	};


	class TypeDefStatic : public TypeDef 
	{
		friend bool OnComponentInitStartup();
	protected:
		const CLSID& m_clsid;
		bool init() const;
		using TypeDef::myType;
	public:	
		// Методы публичные чтобы наши классы могли к ним обратиться. В публичном интерфейсе их нет т.к. TypeDefStatic сами протектед член класса.
		template<EsbClassConcept EsbClassT>
		EsbClassT CreateValue() const {
			return make<EsbClassT>(TypeDef::invoke_CreateValue());
		}
		template<EsbClassConcept EsbClassT>
		EsbClassT CreateValue(const argarray_t& args_) const {
			return make<EsbClassT>(TypeDef::invoke_CreateValue(args_));
		}
	public:
		// Экземпляр класса исключительно единственен для каждого типа. Тип-Инфо.
		ESB_DECLARE_NOCOPYMOVE_DTORDEFAULT_NOCTOR(TypeDefStatic);
		//
		TypeDefStatic(const CLSID& clsid_) : TypeDef{ nullptr }, m_clsid(clsid_)
		{}
		TypeDefStatic(const CLSID& clsid_, IType& itype_) : TypeDef{ itype_ }, m_clsid(clsid_)
		{}
		constexpr bool operator==(const TypeDefStatic& other_) const	{ return this == &other_; }
		constexpr bool operator==(const TypeDef& other_) const			{ return (this == &other_) ? true : TypeDef::operator==(other_); }
	};

	//TOBE	Имеется определенная проблема с инициализацие библиотеки при (возможном) ее расширении большим количеством поддерживаемых объектов 1С
	//		Сейчас все типы и значения нумераторов инициализируются при OnCompinentInitStartup. Но пока их всего меньше сотни и это не составляет проблемы.
	//		Но если "расширяться", то это будет составлять оперделенную проблему. У 1С объектов больше тысячи. И вопрос даже не столько в длительной загрузке,
	//		но и также с выгрузкой и необходимостью "терминировать" какие-нибудь объекты, т.к. мы не контролируем порядок нашей выгрузки. 
	//		(Если мы связались с чем-нибудь скажем из inet.dll, а core нас выгружает позже inet.dll, a мы делаем .Release() этому-из-inet.dll - то крах)
	//		Просить ползователя самому указать что ему нужно для работы не вариант - или все укажут или не укажут и крахи будут.
	//		Сделал для апи-провайдеров интересный вариант с помощью compiler-linker-magic подсматривать из esbhlp за тем, что люди используют
	//		и инициализировать им только то что нужно. Если будет стабильно работать, то нужно пробовать перенести эту методику и на инициализацию типов/нумераторов.
	//TOBE	Для этого нужно каждый TypeObj_ сделать отдельным инстациированием шаблона TypeDefStatic<EsbClassT>, ну и соответственно везде в ESB_DECLARE_TYPEINFO 
	//		проставить имя класса к которому этот TypeObj_ относится. Сейчас не везде. Поэтому ...
	//TOBE	Также, возможно, стоит Type_ переименовать в Type. Красивей смотреться будет - нр. Boolean::Type
	//		и не будет компилятор поругиваться, что имя параметра Type_ перекрывает поле класса Type_, хотя это и не столь важно.
#define ESB_DECLARE_TYPEINFO(...)									\
	ESB_DECLARE_FRIEND_INITTERM()									\
	protected:	static const TypeDefStatic TypeObj_;				\
	public:		static constexpr const TypeDef& Type_{ static_cast<const TypeDef&>(TypeObj_) };



	using RuntimeInitFn = bool(void);
	using RuntimeTermFn = void(void);
	//
	class RuntimeRegistratorInit : private StaticMethListBase<RuntimeRegistratorInit, RuntimeInitFn> {
		using base_t = StaticMethListBase<RuntimeRegistratorInit, RuntimeInitFn>;
		friend base_t;	// базовый у нас private и для того чтобы базовый мог привести свой this к нам делаем его другом
		static inline const RuntimeRegistratorInit* root_ = nullptr;
	public:
		// мы инициализируем элемент списка ссылкой, поэтому в дальнейшем можем не бояться что там нулл
		RuntimeRegistratorInit(RuntimeInitFn& fn_) : base_t{ fn_ }
		{}
		// делаем себя некопируемым, т.к. нам не нужны двойные инит
		ESB_DECLARE_NOCOPYMOVE_DTORDEFAULT_NOCTOR(RuntimeRegistratorInit);
		// проход по всему списку инициализации прерывается если только хоть один метод неуспешен
		static bool do_all() {
			const RuntimeRegistratorInit* it = root_;
			while (it) {
				if (it->meth_())
					it = it->next_;
				else
					return false;
			}
			return true;
		}
	};
	//
	class RuntimeRegistratorTerm : private StaticMethListBase<RuntimeRegistratorTerm, RuntimeTermFn> {
		using base_t = StaticMethListBase<RuntimeRegistratorTerm, RuntimeTermFn>;
		friend base_t;	// базовый у нас private и для того чтобы базовый мог привести свой this к нам делаем его другом
		static inline const RuntimeRegistratorTerm* root_ = nullptr;
	public:
		// мы инициализируем элемент списка ссылкой, поэтому в дальнейшем можем не бояться что там нулл
		RuntimeRegistratorTerm(RuntimeTermFn& fn_) : base_t{ fn_ }
		{}
		// делаем себя некопируемым, т.к. ... нужно еще везде анализировать необходимость и безопасность двойного терм
		ESB_DECLARE_NOCOPYMOVE_DTORDEFAULT_NOCTOR(RuntimeRegistratorTerm);
		// список терм выполняется всегда весь до конца т.к. метод терм безусловен и не возвращает нам свой результат
		static void do_all() {
			const RuntimeRegistratorTerm* it = root_;
			while (it) {
				it->meth_();
				it = it->next_;
			}
		}
	};




	template<class EnumClassType>
	class TypeDefStaticEnum : public TypeDefStatic {
		friend bool OnComponentInitStartup();
		bool init() const;
	public:
		TypeDefStaticEnum(const CLSID& clsid_) : TypeDefStatic(clsid_)
		{}
	};


	template<class EnumObjectT, class EnumValueT, size_t NItems>
	class EnumObjectImpl;

	template<class EnumT, size_t NItems>
	class EnumValueImpl : public Value {
		using enum_value_t = EnumValueImpl<EnumT, NItems>;
		friend enum_value_t make<enum_value_t>(IValuePtr&&);
		EnumValueImpl(IValuePtr&& val_) throw() : Value(std::move(val_)) {}
		EnumValueImpl& operator=(EnumValueImpl&) = delete;
		constexpr EnumValueImpl() noexcept : Value{ nullptr }
		{}
		void initialize(IValuePtr&& new_value_) { return Value::initialize(std::move(new_value_)); }
		//friend EnumT;													// для статической инициализации в нуллптр...
		friend class EnumObjectImpl<EnumT, enum_value_t, NItems>;		// ...сам EnumT нам не друг, а друг его базовая часть... ?
		friend class TypeDefStaticEnum<EnumT>;							// для вызова initialize при бинд
	public:
		ESB_DECLARE_TYPEINFO()
		ESB_IMPLEMENT_ARGBYVAL_OPERATOR(enum_value_t)
	public:
		// полагаемся на то, что равенство значений енумератора есть равенство указателей. 
		// То есть что все значения у нас и в платформе уникальны и созданы одним способом и один раз. (мы при биндинге просто получаем значения от платформы)
		bool operator==(const EnumValueImpl& other_) const { return Value::operator==(other_); }
		bool operator!=(const EnumValueImpl& other_) const { return !operator==(other_); }
	};


	template<class EnumObjectT, class EnumValueT, size_t NItems>
	class EnumObjectImpl {
		ESB_DECLARE_FRIEND_INITTERM()
	protected:
		static constexpr size_t		_items_count = NItems;
		static inline EnumValueT	_items[_items_count]{};
		friend class TypeDefStaticEnum<EnumObjectT>;
		static const TypeDefStaticEnum<EnumObjectT>	TypeObj_;
	};


	// используется в Numeric, поэтому предварительно определен явно. остальные в enums.inc
	class RoundModeEnum;
	using RoundModeValue = EnumValueImpl<RoundModeEnum, 2>;
	class RoundModeEnum : public EnumObjectImpl<RoundModeEnum, RoundModeValue, 2> {
	public:
		static constexpr const RoundModeValue& Round15as10{ _items[0] };
		static constexpr const RoundModeValue& Round15as20{ _items[1] };
	};
	//TOBE
	// Не до конца понятно как лучше - чтобы без суффикса Enum был енумератор - тогда упрощается передача аргументов, но тогда с суффиксом Value будет
	// значения енумератора и усложнится декларирование переменных и интерфейса.
	// Или енумератор с суффиксом, а значения без суффиксов.. Тогда интерфейс декларировать проще, но передача аргументов сложнее
	// Пусть будет однозначно понятно "по-Соломонову" - в интерфейсе ..Value, при выборе ..Enum
	// При желании каждый себе может сделать псевдонимы по вкусу
	//		using RoundMode = RoundModeEnum;	
	// или
	//		using RoundMode = RoundModeValue;

	//
	//TOBE: Нужно проанализировать целесообразность (и возможность) Default_ значения для енумераторов
	//

	//
}	//namespace esb




namespace esb	// String. Дескриптор зависит от String поэтому выносим ее определение сюда 
{	
	class String : public Value {
		ESB_CLASS_IMPLEMENT_MAKE_VAL(String)
	protected:
		ESB_CLASS_IMPLEMENT_MAKE_OPT(String)
	public:
		ESB_DECLARE_TYPEINFO()
		ESB_IMPLEMENT_ARGBYVAL_OPERATOR(String)
		static const String& Empty_;
		static const String& Default_; // = Empty_;
	public:
		ESB_EXPLICIT_STRING
		String(const std::wstring_view& val_) throw() : Value(create_string_value(val_))
		{}
		//template<std::size_t N>
		//ESB_EXPLICIT_STRING
		//String(const wchar_t(&arr_)[N]) : Value(create_string_value(std::wstring_view{arr_, N - 1}))
		//{}
	public:
		const wchar_t*		c_str() const &;	// если мы rvalue, то можем уничтожиться и уничтожить c_str, поэтому возвращать указатель мы можем только если мы lvalue
		std::size_t			length() const;
		std::wstring		string() const				{ return std::wstring{ c_str(), length() }; }
		const std::wstring_view	view() const &			{ return std::wstring_view{ c_str(), length() }; }
		const std::wstring_view	view_when_safe() const	{ return std::wstring_view{ c_str(), length() }; }
		bool				empty() const				{ return (length() == 0); }
		//using collator
		int					compare(const String& other_) const;
		bool operator==(const String& other_) const		{ return (Value::operator==(other_))? true : (compare(other_) == 0); }
		bool operator<(const String& other_) const		{ return (Value::operator==(other_))? false : (compare(other_) < 0); }
		bool operator>(const String& other_) const		{ return (Value::operator==(other_)) ? false : (compare(other_) > 0); }
	};

} //namespace esb // String.



namespace esb	// disp support
{


// все классы построения дисп-интерфейса не предполагают наличие конструкторов по умолчанию. Но мы не можем явно в них показать =delete
// т.к. они при этом становятся не-литеральными и не пригодными для констевал вычислений. Поэтому подавляем предупреждения.
// также они могут содержать ссылки by-design и ввиду этого некопируемы. Подавляем.
// Также, при развертывании шаблонов иногда появляется паддинг (типа std::array<wchar_t, NSize>	code_ при NSize==2). Подавляем.
ESB_WARNING_SUPRESS(ESB_WARN_NO_DEFAULT_CTOR  ESB_WARN_NO_OPERATOR_ASSIGN_ANY)
//
	// В этой структуре мы сохраняем указатель на строку-имя и подготавливаем массив для constexpr создания из этой строки уникального представления этой строки (code_)
	// для того чтобы в рантайме этим не заниматься каждый раз. 
	// (пока просто to_nocase(), но можно и какой-нибудь md5 считать, вопрос эффективности алгоритма сравнения в рантайме)
	//TOBE	По какой-то причине компилятор не дает обработку\переработку строки в конструкторе - *доступ к хранилищу среды исполнения*.
	//		это реализовано в методах make_MetaInterfaceMeth (см. esb_meta.h)
	template<size_t NSize>
	struct MetaNameData {
		static constexpr size_t		len_ = NSize - 1;
		const wchar_t*				text_;
		std::array<wchar_t, NSize>	code_;
		constexpr MetaNameData(const wchar_t(&str1_)[NSize]) : text_{ str1_ }, code_{}
		{}
	};

	// Структура представляет одно имя метода (свойства) в диспатч-лайк массиве методов (свойств) объекта. 
	// поле text_ - это оргинальное имя, поле code_ - преобразованное для упрощения алгоритма поиска по имени
	struct DispName {
		size_t						len_;
		const wchar_t*				text_;
		const wchar_t*				code_;
		template<size_t NSize>
		constexpr DispName(const MetaNameData<NSize>& data_) : len_(data_.len_), text_(data_.text_), code_(data_.code_.data())
		{}
		constexpr std::wstring_view	text_view() const { return { text_, len_ }; }
	};

	// 1С двуязычная (как минимум) соответственно каждый метод (свойство) определяется двумя именами (текстовыми идентификаторами)
#define ESB_DISP_TERM_NAMES_COUNT 2
	struct DispTerm {
		// у нас два и только два имени! это жестко учитывается в make_interface_names_array_sorted
		static constexpr size_t		names_count_ = ESB_DISP_TERM_NAMES_COUNT;
		DispName					names_[names_count_];
		constexpr size_t			names_count() const						{ return names_count_; }
		constexpr const DispName&	name(size_t index_) const				{ assert(index_ < names_count());		return names_[index_]; }
		constexpr std::wstring_view	name_text(size_t index_) const			{ return name(index_).text_view(); }
		constexpr const wchar_t*	name_code_cstr(size_t index_) const		{ return name(index_).code_;	}
	};
ESB_DETECT_MISMATCH(ESB_DISP_TERM_NAMES_COUNT)


	// Базовая структура для 1С-лайк-значений создаваемых в есб. (см.ниже в блоке ext)
	// для вызова методов-экземпляров классов представляет инстанс класса. каждый класс знает как из "базы" получить себя.
	struct ExtValueObjectBase;

	using DispInvokeMembMethFn		= void(ExtValueObjectBase&, IVariable*, const argpack_t&);
	using DispInvokeMembPropGetFn	= void(const ExtValueObjectBase&, IVariable&);
	using DispInvokeMembPropSetFn	= void(ExtValueObjectBase&, const IVariable&);

	using DispInvokeStatMethFn		= void(IVariable*, const argpack_t&);
	using DispInvokeStatPropGetFn	= void(IVariable&);
	using DispInvokeStatPropSetFn	= void(const IVariable&);


	struct DispMethInfo {
		using bitpack_t = uint64_t;
		bitpack_t	data_;
		//    6         5         4         3         2         1
		// 3210987654321098765432109876543210987654321098765432109876543210
		//                   opts-in-reverse-order                  R*count
#define ESB_METHINFO_DATA_BITS		64
#define ESB_METHINFO_SIZE_BITS		6
		// может быть всего 63 параметра из которых последние 57 могут быть опт. или для метода со всеми опт параметрами возмажно максимум 57 параметров
		// (возможные варианты 15:32767-48; 7:127-56; 6:63-57)
ESB_WARNING_SUPRESS(ESB_WARN_DETECT_MISMATCH_IN_SCOPE)
ESB_CHECK_AND_DETECT_MISMATCH(ESB_METHINFO_DATA_BITS, (sizeof(bitpack_t)* CHAR_BIT))
ESB_DETECT_MISMATCH(ESB_METHINFO_SIZE_BITS)
ESB_WARNING_RESTORE() //ESB_WARN_DETECT_MISMATCH_IN_SCOPE
		static constexpr bitpack_t	ESB_METHINFO_MASK_SIZE	= ((1 << ESB_METHINFO_SIZE_BITS) - 1);
		static constexpr bitpack_t	ESB_METHINFO_MASK_RETV	= ESB_METHINFO_MASK_SIZE + 1;
		static constexpr bitpack_t	ESB_METHINFO_MASK_OPTX	= (ESB_METHINFO_MASK_RETV << 1);
		static constexpr size_t		MAX_PARA_COUNT			= ESB_METHINFO_MASK_SIZE;
		static constexpr size_t		MAX_PARA_LAST_OPT_COUNT = (ESB_METHINFO_DATA_BITS - ESB_METHINFO_SIZE_BITS - 1);

		constexpr size_t	para_count() const				{ return (data_ & ESB_METHINFO_MASK_SIZE); }
		constexpr bool		has_retval() const				{ return ((data_ & ESB_METHINFO_MASK_RETV) != 0); }
		constexpr bool		para_is_opt(int n_para_) const	{
			ESB_ASSERT(n_para_ >= 0 && (unsigned)n_para_ < para_count());
			if (n_para_ >= ((int)para_count() - (int)MAX_PARA_LAST_OPT_COUNT)) {	//выражение может быть отрицательным поэтому нам важно чтобы вычислялос все в int!
				size_t n_shift = para_count() - n_para_ - 1;
				return ((data_ & (ESB_METHINFO_MASK_OPTX << n_shift)) != 0);
			}
			else
				return false;
		}
		//см. make_DispMethInfo(.). Для consteval-friendly все делаем не в конструкторе, а отдельно.
		//(также есть стратегия отделить все raw-структуры от шаблонов чтобы esbhlp поменьше видел, думал и инлайнил)
		static consteval bitpack_t make_para_bit(size_t para_count_, size_t n_para_, bool is_para_opt_) {
			assert(n_para_ < para_count_);			
			if (is_para_opt_) {
				assert((int)n_para_ >= ((int)para_count_ - (int)MAX_PARA_LAST_OPT_COUNT));
				size_t n_shift = para_count_ - n_para_ - 1;
				return (ESB_METHINFO_MASK_OPTX << n_shift);
			}
			else
				return 0;
		}
	};


	//NOTE	Структурам DispInfo... нельзя выделять базовый - тривиальность теряют и проблемы начинаются
	struct DispInfoStatMeth  {
		const DispTerm				term_;
		const DispMethInfo			info_;
		DispInvokeStatMethFn&		invoke_;
	};

	struct DispInfoMembMeth {
		DispTerm					term_;
		const DispMethInfo			info_;
		DispInvokeMembMethFn&		invoke_;
	};

	struct DispInfoStatProp {
		DispTerm					term_;
		DispInvokeStatPropGetFn*	invoke_get_;
		DispInvokeStatPropSetFn*	invoke_set_;
	};

	struct DispInfoMembProp {
		DispTerm					term_;
		DispInvokeMembPropGetFn*	invoke_get_;
		DispInvokeMembPropSetFn*	invoke_set_;
	};

	struct DispInfoCtor;		//после TypeDescriptor
//
ESB_WARNING_RESTORE()	//ESB_WARN_NO_DEFAULT_CTOR  ESB_WARN_NO_OPERATOR_ASSIGN_ANY
}	//namespace esb disp support



// esb ext support
namespace esb {
//
	class Boolean;
	class Numeric;
	class DateTime;


// Данная структура описывает наше (1С) тип-инфо и не предназначена ни для какого использования кроме как статического создания в единственном экземпляре на этапе компиляции.
// Адрес этой структуры (указатель на экземпляр этой структуры) уникален для каждого типа и каждый тип однозначно знает где она находится с момента своей компиляции.
	struct TypeInfo 
	{	ESB_DECLARE_NOCOPYMOVE_DTORDEFAULT_NOCTOR(TypeInfo);

		const CLSID&				TypeId_;
		const FixedTerm&			TypeTerm_;
		const std::wstring_view&	TypeDescriptionInit_;
		//NOTE	Какое либо поле (и лучше это) обязано быть mutable. Тогда компилятор эту constexpr структуру укладывает в секцию data (rw),
		//		а не в rodata. Т.е. структура вся constexpr, но можно что-то переписывать. 
		//		(Если структура в rodata, то при попытке переписать крах)
		//		Это нам важно для наших наследников - TypeDescriptor... Мы их в рантайме правильной vtbl инициализируем.
		mutable ClassOpt<String>	TypeDescription_{ nullptr };
	public:
		constexpr TypeInfo(const CLSID& type_id_, const FixedTerm& type_term_, const std::wstring_view& type_description_)
													: TypeId_(type_id_), TypeTerm_(type_term_), TypeDescriptionInit_(type_description_)
		{}	
	public:
		static void Init(const TypeInfo& type_) {
			//множественная инициализаци дескриптора не катострофична, но все равно свидетельствует о какой-то ошибке
			assert(!type_.TypeDescription_);
			if (!type_.TypeDescription_)
				type_.TypeDescription_ = String(type_.TypeDescriptionInit_);
		}
		static void Term(const TypeInfo& type_) {
			type_.TypeDescription_ = nullptr;
		}
	};


	//NOTE	По историческим причинам часть классов, структур, методов и т.п. имеют приставку Ext.
	//		Ранее возможность создавать свои классы для 1С было неким "расширением"

	struct TypeDescriptor;
	struct TypeDescriptorValue;
	struct TypeDescriptorValueSimple;
	struct TypeDescriptorValueObject;


	//esbhlp
	//INFO	Регистрировать типы в платформе нужно только если Вы хотите создавать их в коде 1С с помощью Новый(Тип(...))
	//		Но это достаточно не удобно т.к. 1С пытается проверять имя типа и ее приходится обманывать
	//		Также при таком создании неудобно ограниченно передавать параметры конструктору.
	//		На мой взгляд удобнее сделать к своему типу XXXX метод CreateXXX в интерфейсе есб-addin-object.
	//		Для такого создания регистрировать ничего не надо, и, от-регистрировать тоже. И с параметрами проще.
	//		Поэтому эти методы "на любителя" Вы можете вызывать сами. Есб их не использует.
	bool ExtRegisterType(const TypeDescriptorValue& descriptor_, IClassFactory& factory_);
	bool ExtRevokeType(const TypeDescriptorValue& descriptor_);


	//INFO	Базовый класс для создаваемых в есб 1С-значений. Инициализация правильной vtbl в esbhlp
	//		Описание как имплементировать ту или иную функциональность задается структурами TypeDescriptor...
	//		Ввиду того, что vtbl задается не компилятором класс объявлен не-копируемым (а то еще и операторы копирования\перемещения делать надо)
	//		Копируемость ему не нужна т.к. он создается единожды, сам себя уничтожает, а данными мы оперируем через указатель на IValue.
	struct __declspec(novtable) ExtValueBase : public IValue {
		//hidden uintptr_t vft_
		ESB_DECLARE_NOCOPYMOVE(ExtValueBase);
	protected:
		const TypeDescriptor*		m_type = nullptr;
		refcounter_t				m_refs = 0;
		// implementation
	};
	

	using ExtValueSimpleBase = ExtValueBase;

	template<class ExtDataT>
	struct ExtValueSimple : public ExtValueSimpleBase {
		//Указываем что выравнивание поля для ExtDataT должно быть точно таким же, как если бы мы не паковали его в структуру
		alignas(ExtDataT) 
		ExtDataT ContainedValueData_;
		
	};


	struct ExtValueObjectBase : public ExtValueBase, public IObject {
		// implementation
		template<class ExtDataT>
		inline ExtDataT& GetContainedData();
		template<class ExtDataT>
		inline const ExtDataT& GetContainedData() const;
	};
	template<class ExtDataT>
	struct ExtValueObject : public ExtValueObjectBase {
		//Указываем что выравнивание поля для ExtDataT должно быть точно таким же, как если бы мы не паковали его в структуру
		alignas(ExtDataT) 
		ExtDataT ContainedValueData_;
	};


	template<class ExtDataT>
	inline ExtDataT& ExtValueObjectBase::GetContainedData() {
		return static_cast<ExtValueObject<ExtDataT>*>(this)->ContainedValueData_;
	}
	template<class ExtDataT>
	inline const ExtDataT& ExtValueObjectBase::GetContainedData() const {
		return static_cast<const ExtValueObject<ExtDataT>*>(this)->ContainedValueData_;
	}


	template<class ExtValueT>
	struct ext_value_contained_data;
	template<class ExtDataT>
	struct ext_value_contained_data<ExtValueSimple<ExtDataT>> {
		using type = ExtDataT;
	};
	template<class ExtDataT>
	struct ext_value_contained_data<ExtValueObject<ExtDataT>> {
		using type = ExtDataT;
	};
	template<class ExtValueT>
	using ext_value_contained_data_t = typename ext_value_contained_data<ExtValueT>::type;


	using ExtAllocatorAllocFn = void* (size_t);
	using ExtAllocatorFreeFn = void(void*);

	
	//NOTE	заглушка. настоящий интерфейс ставится в esbhlp
	class __declspec(novtable) TypeDescriptorInterfaceImpl : public IType {
		STDMETHOD(QueryInterface)(REFIID, void**) throw() override	{ return E_NOTIMPL; }
		STDMETHOD_(ULONG, AddRef)() throw() override				{ return 1; }
		STDMETHOD_(ULONG, Release)() throw() override				{ return 1; }
	};

	//Static
	//Fixed
	//Dynamic
	//Volatile
	//Stable
	//Unstable
	//Mutable

	// Диспатч-интерфейс реализуется двумя массивами (для Prop и Meth) и тремя методами для каждого массива - Size, Find, Item (Prop или Meth)
	// Интерфейс может быть стабильным - т.е. в рантайме ничего не меняется (т.е. каждому из методов не нужен параметр экземпляра)
	// и динамическим - в рантайме может измениться кол-во свойств/методов/их-имена/реализация - тогда нужена ссылка на экземпляр в параметрах
	// (интерфейс - статичный/динамический, методы - статические/экземпляра)
	using DispStableSizeFn		= size_t(void);
	using DispStableFindFn		= dispid_t(const strview_t&);
	// Для статических методов не нужен экземпляр объекта относительно которого они вызываются
	using DispStatPropFn		= const DispInfoStatProp*(dispid_t);
	using DispStatMethFn		= const DispInfoStatMeth*(dispid_t);
	using DispStatMethParaDefFn = bool(dispid_t meth_, dispid_t para_, IVariable* out_def_value_);

	using DispMutableSizeFn		= size_t(const ExtValueObjectBase&);
	using DispMutableFindFn		= dispid_t(const ExtValueObjectBase&, const strview_t&);
	// Для методов экземпляра обязателен параметр экземпляра объекта относительно которого вызываются
	// (реализация дисп-интерфейса в том числе позволяет привязывать статические методы к классу тихо "глотая" инстанс экземпляра. см.ext)
	using DispMembPropFn		= const DispInfoMembProp* (const ExtValueObjectBase&, dispid_t);
	using DispMembMethFn		= const DispInfoMembMeth* (const ExtValueObjectBase&, dispid_t);
	using DispMembMethParaDefFn = bool(const ExtValueObjectBase&, dispid_t meth_, dispid_t para_, IVariable* out_def_value_);


	using ExtInstanceCreatorFn = IValuePtr(const TypeDescriptorValue&, const argpack_t&);
	using ExtInstanceDeleterFn = void(ExtValueBase&);
	using ExtInstanceCreatorFindFn = ExtInstanceCreatorFn* (size_t);

	struct DispInfoCtor {
		size_t					param_count_;
		ExtInstanceCreatorFn*	func_;
	};


	using ExtValueToBooleanFn = Boolean(const ExtValueBase&);
	using ExtValueToNumericFn = Numeric(const ExtValueBase&);
	using ExtValueToStringFn = String(const ExtValueBase&);
	using ExtValueToDateTimeFn = DateTime(const ExtValueBase&);
	using ExtValueIsEqualDataFn = bool(const ExtValueBase&, const ExtValueBase&);
	using ExtValueDataHashFn = long(const ExtValueBase&);


	//INFO	Фактически "дескриптор" - это некий аналог vtbl и typeinfo, только своими руками.
	//		Минусом такого решения конечно есть отсутствие поддержки компилятором. все ручками.
	//		Плюсом намного большая гибкость. А при consteval в

#define ESB_CONSTEXPR_TYPEDESCRIPTOR	constexpr


	// совершенно пустой дескриптор для типов, которые сами знают как создаваться\удаляться\работать (делегат например) - не лучший пример..
	struct TypeDescriptor : public TypeDescriptorInterfaceImpl, public TypeInfo {
		ESB_DECLARE_NOCOPYMOVE_DTORDEFAULT_NOCTOR(TypeDescriptor);

		ESB_CONSTEXPR_TYPEDESCRIPTOR
		TypeDescriptor(const CLSID & id_, const FixedTerm & term_, const std::wstring_view & description_)
			: TypeDescriptorInterfaceImpl(), TypeInfo(id_, term_, description_)
		{}
	protected:
		// имплементация IType в esbhlp расчитана на некоторое 1С-Value. Поэтому все методы Init\Term реализованы в TypeDescriptorValue
	};



	struct TypeDescriptorValue : public TypeDescriptor 
	{	ESB_DECLARE_NOCOPYMOVE_DTORDEFAULT_NOCTOR(TypeDescriptorValue);
	private:
		//esbhlp
		static IType& InitDescriptorInterface(TypeDescriptorValue& typ_);
	public:
		ExtInstanceDeleterFn&		InstanceDeleter_;				//в данной модели делетер экземпляра обязателен! Возможна иная реализация дескриптора для синглтонов
		ExtInstanceCreatorFindFn*	InstanceCreatorFind_ = nullptr;	//которые ссылки не считают и не удалаяются - иная ветка иерархии дескрипторов. не надо здесь делать "удобства"
																	//в виде "а если удалятора нет, то мы синглтон и ссылки считать не будем...".. а хочется...

		ExtValueToBooleanFn*		ToBoolean_		= nullptr;
		ExtValueToNumericFn*		ToNumeric_		= nullptr;
		ExtValueToStringFn*			ToString_		= nullptr;
		ExtValueToDateTimeFn*		ToDateTime_		= nullptr;
		ExtValueIsEqualDataFn*		IsEqualData_	= nullptr;
		ExtValueDataHashFn*			DataHash_		= nullptr;
	public:
		ESB_CONSTEXPR_TYPEDESCRIPTOR
		TypeDescriptorValue(const CLSID& id_, const FixedTerm& term_, const std::wstring_view& description_, ExtInstanceDeleterFn& deleter_)
			: TypeDescriptor(id_, term_, description_), InstanceDeleter_(deleter_)
		{}
	protected:
		// собственно после вызова этого метода наш дескриптор готов работать как IType для 1С
		// метод протектед, т.к. конкретными 1С-Value являются примитивное-валюе и объектное-валюе
		// (см.ниже их дескрипторы) Они уже вызывают эти методы.
		static IType& Init(TypeDescriptorValue& typ_) {
			TypeInfo::Init(typ_);
			return InitDescriptorInterface(typ_);
		}
		static void Term(TypeDescriptorValue& typ_) {
			TypeInfo::Term(typ_);
			// делать какой-то терм интерфейсу не нужно - там ничего не требует очистки.
		}
	};


	struct TypeDescriptorValueSimple : public TypeDescriptorValue {
		ESB_DECLARE_NOCOPYMOVE_DTORDEFAULT_NOCTOR(TypeDescriptorValueSimple);

		ESB_CONSTEXPR_TYPEDESCRIPTOR
		TypeDescriptorValueSimple(const CLSID& id_, const FixedTerm& term_, const std::wstring_view& description_, ExtInstanceDeleterFn& deleter_)
			: TypeDescriptorValue(id_, term_, description_, deleter_)
		{}
	public:
		static IType& InitDescriptor(TypeDescriptorValueSimple& typ_) {
			return TypeDescriptorValue::Init(typ_);
		}
	protected:
		ExtValueSimpleBase& NewValueBaseInplace(void* pv_) const noexcept;
	};


	struct TypeDescriptorValueObject : public TypeDescriptorValue
	{	ESB_DECLARE_NOCOPYMOVE_DTORDEFAULT_NOCTOR(TypeDescriptorValueObject);
		//
		DispMutableSizeFn*	ObjPropCount_	= nullptr;
		DispMutableFindFn*	ObjPropFind_	= nullptr;
		DispMembPropFn*		ObjProp_		= nullptr;

		DispMutableSizeFn*	ObjMethCount_	= nullptr;
		DispMutableFindFn*	ObjMethFind_	= nullptr;
		DispMembMethFn*		ObjMeth_		= nullptr;
		// if info-bit-opt is set, but ObjMethParaDefault_==nullptr assume default Undef
		DispMembMethParaDefFn*	ObjMethParaDefault_ = nullptr;

	public:
		ESB_CONSTEXPR_TYPEDESCRIPTOR
		TypeDescriptorValueObject(const CLSID& id_, const FixedTerm& term_, const std::wstring_view& description_, ExtInstanceDeleterFn& deleter_)
			: TypeDescriptorValue(id_, term_, description_, deleter_)
		{}
	public:
		static IType& InitDescriptor(TypeDescriptorValueObject& typ_) {
			return TypeDescriptorValue::Init(typ_);
		}
	protected:
		ExtValueObjectBase& NewValueBaseInplace(void* pv_) const noexcept;
	};


	template<class T>
	inline constexpr bool is_type_descriptor_value = std::is_base_of_v<TypeDescriptorValueSimple, T>;
	template<class T>
	inline constexpr bool is_type_descriptor_object = std::is_base_of_v<TypeDescriptorValueObject, T>;

	template<class T>
	inline constexpr bool is_type_descriptor = (is_type_descriptor_value<T> || is_type_descriptor_object<T>);
	template<class T>
	concept TypeDescriptorConcept = is_type_descriptor<T>;

	// дальше см. esb_ext.h
}	// esb ext support



namespace esb // esb addin & register support
{
	// Дескриптор объекта есб-компоненты. Имеет упрощенный и статический объектный интерфейс. 
	// Instance_ статический, один и в esbhlp. Тип и фактори там же.
	// Нужно только задать соответствующие методы диспатч-лайк интерфейса. см. esb_meta.h, esb_ext.h
	struct AddinDescriptor : public TypeInfo
	{	ESB_DECLARE_NOCOPYMOVE_DTORDEFAULT_NOCTOR(AddinDescriptor);
	public:
		DispStableSizeFn*		ObjPropCount_	= nullptr;
		DispStableFindFn*		ObjPropFind_	= nullptr;
		DispStatPropFn*			ObjProp_		= nullptr;

		DispStableSizeFn*		ObjMethCount_	= nullptr;
		DispStableFindFn*		ObjMethFind_	= nullptr;
		DispStatMethFn*			ObjMeth_		= nullptr;

		DispStatMethParaDefFn*	ObjMethParaDefault_ = nullptr;
	public:
		constexpr AddinDescriptor(const CLSID& type_id_, const FixedTerm& type_term_, const std::wstring_view& type_description_,
									DispStableSizeFn* prop_count_, DispStableFindFn* prop_find_, DispStatPropFn* prop_item_,
									DispStableSizeFn* meth_count_, DispStableFindFn* meth_find_, DispStatMethFn* meth_item_)
			: TypeInfo(type_id_, type_term_, type_description_),
			ObjPropCount_(prop_count_), ObjPropFind_(prop_find_), ObjProp_(prop_item_),
			ObjMethCount_(meth_count_), ObjMethFind_(meth_find_), ObjMeth_(meth_item_)
		{}
		
		// Для инициализации через шаблонный конструктор через описание интерфейса (interface_info_t) нужно подключать esb_meta - определено там
		template<typename InterfaceT>
		constexpr AddinDescriptor(std::type_identity<InterfaceT>);
	public:
		static const AddinDescriptor Instance_;
	};


	//esbhlp
	bool AddinObject_Register();
	bool AddinObject_Revoke();
	bool AddinObject_RegisterAsSCOM();


	inline HMODULE GetAddinModuleHandle() {
		HMODULE hLibAddin = NULL;
		[[maybe_unused]] BOOL b = GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, 
																									(LPCWSTR)&AddinDescriptor::Instance_, &hLibAddin);
		// проверяемся просто для отладки. при загрузке проверяется NULL и в этом случае загрузка отменяется.
		assert(b && hLibAddin);
		return hLibAddin;
	}

	inline constexpr size_t ADDIN_MODULE_PATH_MAX = 0x400;	//1C do so
	//
}	// esb addin & register support




namespace esb // DelagatToMeth support
{
	//INFO	DelagatToMeth
	//		Сделан не через "интерфейсы", а с прямой поддержкой в esbhlp. Поэтому здесь, а не в ext. esbhlp всякие шаблоны видеть не должна (хотя видит еще..)
	//		а в ext не должно быть никаких упоминаний о hlp - только с++
	//		А делегату нужен аллокатор. Поэтому и аллокатор здесь :(
	//		Делагат по-простому сам себе валюе, дескриптор и фактори (типдеф)

	template<size_t NAlign, bool = /*IsDefaultAlignment*/ (NAlign <= __STDCPP_DEFAULT_NEW_ALIGNMENT__)>
	requires (is_power_of_2(NAlign))
	struct ExtAllocator;
	struct ExtAllocatorBase {
		static size_t _check_cb(size_t cb_) {
			if (cb_ == 0) {		// мы не ожидаем 0. это какая-то ошибка
				assert(L"Попытка выделения памяти нулевого размера" == 0);
				ESL_THROW_NIL_ALLOC();
			}
			return cb_;
		}
		static void* _check_alloc(void* ptr_) {
			if (ptr_ == nullptr)
				ESL_THROW_BAD_ALLOC();

			return ptr_;
		}
		static void* _check_free_ptr(void* ptr_) {
			assert(ptr_); // Видимо где-то ошибка
			return ptr_;
		}
	};

	template<size_t NAlign>
	struct ExtAllocator<NAlign, /*IsDefaultAlignment == */ true> : private ExtAllocatorBase {
		static void*	Alloc(size_t cb_) noexcept(false)		{ return _check_alloc( ::operator new( _check_cb(cb_) , std::nothrow) ); }
		static void		Free(void* ptr_) noexcept				{ return ::operator delete( _check_free_ptr(ptr_) ); }
	};
	template<size_t NAlign>
	struct ExtAllocator<NAlign, /*IsDefaultAlignment == */ false> : private ExtAllocatorBase {
		static void*	Alloc(size_t cb_) noexcept(false)		{ return _check_alloc( ::operator new( _check_cb(cb_) , std::align_val_t{ NAlign }, std::nothrow) ); }
		static void		Free(void* ptr_) noexcept				{ return ::operator delete( _check_free_ptr(ptr_), std::align_val_t{ NAlign } ); }
	};



	class TypeDefDelegatToMeth : public TypeDef {
		friend class DelegatToMeth;
	protected:
		struct DelegatToMethObjectValue : public IValue, public IObject {
			refcounter_t	m_refs;
			IObjectPtr		m_object;
			dispid_t		m_item;
			DelegatToMethObjectValue(IObjectPtr&& object_, dispid_t item_) : m_refs(0), m_object(std::move(object_)), m_item(item_)
			{}
		public:
			IValuePtr	InvokeAsFunc() const							{ return IObjectMethods_InvokeAsFunc(*m_object, m_item); }
			IValuePtr	InvokeAsFunc(const argarray_t& args_) const		{ return IObjectMethods_InvokeAsFunc(*m_object, m_item, args_);	}
			void		InvokeAsProc() const							{ return IObjectMethods_InvokeAsProc(*m_object, m_item); }
			void		InvokeAsProc(const argarray_t& args_) const		{ return IObjectMethods_InvokeAsProc(*m_object, m_item, args_); }
			// esbhlp
			static DelegatToMethObjectValue& NewValueInplace(void*, IObjectPtr&& object_, dispix_t item_) noexcept;
			static IType& GetType() noexcept;
		};
		using ext_value_t = DelegatToMethObjectValue;
		using allocator_t = ExtAllocator< alignof(ext_value_t) >;
		
		ESB_DEFINE_GUID(static inline constexpr const CLSID	TypeId_, 0xbf925800, 0x160e, 0x4219, 0xb7, 0x46, 0x6e, 0x2e, 0x26, 0x34, 0x42, 0x8b);
		static inline constexpr const FixedTerm				TypeTerm_{ L"DelegatToMeth", L"ДелегатМетода" };
		static inline constexpr const std::wstring_view		TypeTextInit_{ L"Делегат вызова метода объекта" };
		static inline /*constexpr*/ Nullable<String>		TypeText_{ nullptr };

		static ext_value_t& ValueInstanceCreate(IObjectPtr&& obj_, dispix_t meth_) {
			void* pv = allocator_t::Alloc(sizeof(ext_value_t));
			assert(pv != nullptr || L"аллокатор не должен возвращать нулл!" == nullptr);
			return ext_value_t::NewValueInplace(pv, std::move(obj_), meth_);
		}
		static void ValueInstanceDelete(ext_value_t& value_) {
			value_.~ext_value_t();
			allocator_t::Free(&value_);
		}

		static ext_value_t& CreateValueInstance(IObjectPtr&& obj_, dispid_t meth_) {
			if (!is_valid_dispid(meth_))
				ESL_THROW_OBJECT_METH_NOT_FOUND();
			return ValueInstanceCreate(std::move(obj_), as_dispix(meth_));
		}
#if ESB_USE_OBJECT_DISPATCH_FIND_STRVIEW_HACK
		static ext_value_t& CreateValueInstance(IObjectPtr&& obj_, const strview_t& name_)	{ return CreateValueInstance(std::move(obj_), IObjectMethods_Find(*obj_, name_));}
#endif	//ESB_USE_OBJECT_DISPATCH_FIND_STRVIEW_HACK
		static ext_value_t& CreateValueInstance(IObjectPtr&& obj_, const String& name_)		{ return CreateValueInstance(std::move(obj_), IObjectMethods_Find(*obj_, name_));}
		// это вызывается из esbhlp при создании делегата в 1С коде.
		static ext_value_t& CreateValueInstance(const IVariable* obj_var_, const IVariable* vname_) {
			ESB_ASSERT(obj_var_);
			IValuePtr obj_val = IVariable_GetValue(*obj_var_);
			IObjectPtr obj = query_interface_or_throw<IObjectPtr>(*obj_val);
			String name = check_and_make_from_var<String>(vname_);	//TODO
			return CreateValueInstance(std::move(obj), name);
		}
	public:
		TypeDefDelegatToMeth() : TypeDef{ nullptr }
		{}
		bool init() {
			TypeText_ = String(TypeTextInit_);
			TypeDef::init(DelegatToMethObjectValue::GetType());
			return true;
		}
		void term() {
			if (TypeText_)
				TypeText_ = nullptr;
		}
	};


	//esbhlp
	//	Это функции для регистрации типа делегат-на-метод в платформе 1С для возможности создавать делегаты в коде 1С с помощью Новый(...)
	//	(используются по желанию)
	extern bool ExtRegisterDelegatToMeth();
	extern bool ExtRevokeDelegatToMeth();


	class DelegatToMeth : public esb::Object
	{
		friend DelegatToMeth make<DelegatToMeth>(IValuePtr&&);
		DelegatToMeth(IValuePtr&& val_, IObjectPtr obj_) throw() : Object(std::move(val_), std::move(obj_))
		{}
		static inline TypeDefDelegatToMeth			TypeObj_{};
		static inline const RuntimeRegistratorInit	on_init_{ MemberMethInvoker<&TypeObj_, &TypeDefDelegatToMeth::init>::invoke };
		static inline const RuntimeRegistratorTerm	on_term_{ MemberMethInvoker<&TypeObj_, &TypeDefDelegatToMeth::term>::invoke };
		using ext_value_t = TypeDefDelegatToMeth::DelegatToMethObjectValue;
		//TOBE: здесь (и много где) делается *get_interface(*this), а * проверяет на нуллптр, а в случае валидного this его там быть никогда не должно
		//		Лишняя проверка, актуально только для non-release build. Но esbhlp даже в релизе - это assertion-build.
		//		Возможно нужен какой-то ESB_ASSERT_IN_DEBUG_ONLY или DEBUG_LEVEL или ... какое еще решение. Сам get_interface - это не хорошее решение.
		const ext_value_t& me() const { 
			return static_cast<const ext_value_t&>(static_cast<const IValue&>( *get_interface(*this) )); 
		}
		//
		template<class RetT, ArgConcept... ArgsT> 
		RetT invoke_impl(const ArgsT&... args_) const			{
			if constexpr (std::is_same_v<RetT, void>)
				return me().InvokeAsProc({ &args_ ... });
			else
				return check_and_make<RetT>(me().InvokeAsFunc({ &args_ ... }));
		}
		template<class RetT>	RetT invoke_impl() const		{ return check_and_make<RetT>(me().InvokeAsFunc()); }
		template<>				void invoke_impl<void>() const	{ return me().InvokeAsProc(); }
	public:
		static inline const TypeDef&				Type_{ TypeObj_ };
	public:
		static DelegatToMeth Create(const Object& obj_, const String& meth_) {
			IObjectPtr obj = get_context(obj_);	//copy
			return make<DelegatToMeth>(IValuePtr{ TypeObj_.CreateValueInstance(std::move(obj) , meth_) });
		}
#if ESB_USE_OBJECT_DISPATCH_FIND_STRVIEW_HACK
		static DelegatToMeth Create(const Object& obj_, const strview_t& meth_) {
			IObjectPtr obj = get_context(obj_);	//copy
			return make<DelegatToMeth>(IValuePtr{ TypeObj_.CreateValueInstance(std::move(obj) , meth_) });
		}
#endif //#if ESB_USE_OBJECT_DISPATCH_FIND_STRVIEW_HACK
	//protected:	TODO
	public:
		//INFO	Метод не публичный чтобы навязать использование какого-то конкретного класса-наследника в котором Invoke определен с конкретной необходимой сигнаурой. 
		//		К примеру для предикатк сортировки чисел что-то типа:
		//		class ComparatorForNumeric : public DelegatToMeth { bool operator()(const Numeric&, const Numeric&) { return DelegatToMeth::Invoke(...);  }  };
		template<class RetT, class... ArgsT>
		RetT Invoke(const ArgsT&... args_) const {
			return invoke_impl<RetT>(forward_as_arg(args_) ...);
		}
	};

}// namespace esb // DelagatToMeth support
ESB_WARNING_RESTORE()	//ESB_WARNING_SUPRESS_NO_VIRTUAL_DTOR_ANY




// Методы АПИ предоставляются разными длл и для вызовов из встроенного языка тихо по-за-сценой собираются в кучу. При этом эти самые апи-провайдеры
// инициализируются той или иной политикой использования. По хорошему нам тоже надо с какой-то политикой инициализироваться. 
// Но какой? Для нас политики не существует. Мы для платформы вообще никто.
// Поэтому, исходя из целей компилирования существующего 1С кода, самым разумным нахожу просто переиспользовать апи-провайдеров инициализированных
// для текущей конфигурации платформы. В конечном итоге компилированный код должен работать в точности так как и не компилированный.
// 
namespace esb_api 
{
	//TOBE: сделать обработку синглетонов
	enum class ProviderId {
		core,		//ДА-синглетон
		addin,		//ДА
		mngui,		//НЕТ!!
		backend,	//НЕТ!!
		entext2,	//НЕТ!!
		extui,		//НЕТ!!
		frntend,	//НЕТ!!
		mngbase,	//НЕТ!!
		mngcore,	//НЕТ!!
		odata,		//НЕТ!!
		xml2,		//ДА
		ext,		//НЕТ!!
	};

	using dispid_t = esb::dispid_t;

	//определение в esbhlp. если по какой-то причине будет неудача, то вернется nullptr. и метод инит должен вернуть false прервав инициализацию
	template<ProviderId PID>
	esb::IObjectPtr get_provider_context();		

	//instantiated only when compiler instantiate some method of some provider
	template<ProviderId PID>
	inline bool provider_in_use() { 
		return true; 
	}
	//linker magic
	//in esbhlp-obj build: in esbhlp defined		bool provider_no_use() { return false; }		which mapped as default thru #pragma comment(linker, "/alternatename: ***
	//in esbhlp-lib build: provider_in_use explicity instantiate for return false and linker use it only if it not defined in obj-files
	//
	//At OnComponentInitStartup invoked Provider::init only for providers which used in code
	//At OnComponentDoneCleanup invoked Provider::term for this providers
	
	template<ProviderId PID>
	struct Provider {
		static inline esb::IObjectPtr m_interface = nullptr;
#ifdef _LIB		// для esbhlp вообще не показываем реализацию методов, чтобы он не реализовывал и не инлайнил их
		static bool init();
		static void term();
#else
		static bool init() { m_interface = get_provider_context<PID>(); return (m_interface != nullptr); }
		static void term() { m_interface.reset(); }
#endif
	public:
#define PID_INUSE() (void)provider_in_use<PID>()	
		static void invoke_proc(dispid_t meth_) 											{ PID_INUSE(); return esb::IObjectMethods_InvokeAsProc(*m_interface, meth_);	}
		static void invoke_proc(dispid_t meth_, const esb::argarray_t& args_) 				{ PID_INUSE(); return esb::IObjectMethods_InvokeAsProc(*m_interface, meth_, args_); }
		static esb::IValuePtr invoke_func(dispid_t meth_) 									{ PID_INUSE(); return esb::IObjectMethods_InvokeAsFunc(*m_interface, meth_);	}
		static esb::IValuePtr invoke_func(dispid_t meth_, const esb::argarray_t& args_) 	{ PID_INUSE(); return esb::IObjectMethods_InvokeAsFunc(*m_interface, meth_, args_); }
		static esb::IValuePtr invoke_prop_get(dispid_t prop_) 								{ PID_INUSE(); return esb::IObjectProperties_GetValue(*m_interface, prop_); }
#undef PID_INUSE
	};

	// а в приложении explicit инстанциируем методы init\term
	// таким образом в процедуре инициализации (в lib) проверяется что кто-то где-то использовал АПИ (в объектном файле появилась provider_in_use возвращающая true)
	// и для этих провайдеров вызывается процедура init\term которые тоже находится под контролем пользователя библиотеки.
	// (сам вызов (void)provider_in_use<PID>() из методов invoke... вырезается оптимизатором)
#ifndef _LIB
	// методы нельзя делать инлайн, т.к. компилятор их тогда просто не делает - он видит, что в спп их никто не вызывает.
	// а если их делать не-инлайн, то он их делает с week (т.к. он от шаблонных классов) и в разных спп они не конфликтуют.
	// TOBE: немного не доделано, т.к. лишние (которые для не-используемого АПИ) все равно присутствуют и вяжутся с lib. Но не вызываются.
	//		 нужно еще подумать, поиграться... 		
	#define ESBLIB_API_PROVIDER(P_)									\
	template /*inline*/ bool Provider<ProviderId::##P_##>::init();	\
	template /*inline*/ void Provider<ProviderId::##P_##>::term();
	#include "esblib_api_providers.inc"
#endif
}	//namespace esb_api




//
#endif //ESB_CORE