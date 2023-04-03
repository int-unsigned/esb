/*
	"ESB" is designed to create external native components for the 1C(tm) program.
	Copyright © 2023 IntUnsigned	(v8classes@gmail.com)
	"ESB" is free software under LGPLv2.1 license with essential restriction about binary part (view esb-license.txt)
	"ESB" IS DISTRIBUTED "AS-IS" WITHOUT ANY, EVEN THE IMPLIED, WARRANTY. IN NO EVENT SHALL THE AUTHORS OR THE COPYRIGHT OWNER BE LIABLE FOR ANY CLAIM, 
	DAMAGES OR OTHER LIABILITY ARISING OUT OF THE USE OF THIS SOFTWARE.
	THE AUTHOR DOES NOT GIVE ANY GUARANTEES AND DOES NOT BEAR ANY RESPONSIBILITY REGARDING THE USE OF 1C(tm) COMPONENTS PRODUCED USING THE ESB LIBRARY IN THE 1C(tm) ENVIRONMENT.
*/


#pragma once
#ifndef ESB_EXT_H
#define ESB_EXT_H
#include <cassert>

#include "esb_core.h"
#include "esb_meta.h"

#include <array>
#include <tuple>
#include <span>


namespace esb 
{

	//INFO	TypeDescriptorValueSimple и TypeDescriptorValueObject это структуры-интерфейсы для esbhlp, которые описывают какие-то типы
	//		собственно описателями этих типов будут наследники этих структур. строго по одному классу-дескриптору на каждый тип.
	//		Этим классом-дескриптором (наследником от ...) мы специализируем ExtTypeDef и он создает инстанс дескриптора.
	template<TypeDescriptorConcept ExtTypeDescriptorT>
	struct ExtTypeDef : public TypeDef {
		using descriptor_t = ExtTypeDescriptorT;
		using ext_value_t = typename descriptor_t::ext_value_t;
	protected:
		static inline descriptor_t DescriptorInstance_{};
	public:
		ExtTypeDef() : TypeDef{ nullptr }
		{}
		constexpr bool operator==(const TypeDef& other_) const {
			return (this == &other_) ? true : TypeDef::operator==(other_);
		}
		bool init() {
			TypeDef::init(descriptor_t::InitDescriptor(DescriptorInstance_));
			return true;
		}
		static const descriptor_t& GetTypeDescriptor() {
			return DescriptorInstance_;
		}

		template<typename... ArgsT>
		ext_value_t& CreateValueInstance(ArgsT... args_) const {
			return descriptor_t::CreateValueInstance(DescriptorInstance_, std::forward<ArgsT>(args_) ...);
		}
	};




// Converter
// 1С-классы имеют встроенный интерфейс преобразования значений в примитивные типы 1С (Булево, Число, Строка, Дата)
// Для создаваемых в есб значений (объектов) мы также предоставляем возможность несколькими способами заявить для 1С, что они преобразуемы к примитивным типам.
// во-первых само примитивное значение с++ может быть естесственно преобразовано в примитивный тип 1С:
//		скажем int естесственно преобразуется в Булево, Число, Строку. (но не в Дату)
// во-вторых, если это некая сложная структура, но она может предатавляться примитивом, то у структуры можно объявить метод Data()
//		и если примитивное 1С значение конструируемо из типа данных возвращаемым этим методом Data(), то будет задействовано преобразование
// и-в-третьих Вы можете непосредственно в своей структуре объявить метод конвертера ToBoolean, ToNumeric, ToString, ToDate
//

#define ESB_CONTAINED_DATA_ACCESSOR_NAME			Data

	template<class T>
	constexpr bool ext_has_Data = (requires(T& v) { v.ESB_CONTAINED_DATA_ACCESSOR_NAME(); });

	template<bool, class T>
	struct ext_data_type_or_void			{ using type = decltype( std::declval<T>().ESB_CONTAINED_DATA_ACCESSOR_NAME() );	};
	template<class T>
	struct ext_data_type_or_void<false, T>	{ using type = void;	}; //если у типа Т нет мембера data, то вычислять какибы-то образом его нельзя - ошибка
	template<class T>
	using ext_data_t = ext_data_type_or_void<ext_has_Data<T>, T>::type;

	template<class T, class TypeTo>
	inline constexpr bool ext_has_type_to = (std::is_constructible_v<TypeTo, T >);

	template<class T>
	inline constexpr bool ext_has_type_toBoolean = ext_has_type_to<T, Boolean>;
	template<class T>
	inline constexpr bool ext_has_type_toNumeric = ext_has_type_to<T, Numeric>;
	template<class T>
	inline constexpr bool ext_has_type_toDate = ext_has_type_to<T, DateTime>;
	template<class T>
	inline constexpr bool ext_has_type_toStringCreate = ext_has_type_to<T, String>;
	template<class T>
	inline constexpr bool ext_has_type_toStringConvert = requires(T& t) { std::to_wstring(t); };
	template<class T>
	inline constexpr bool ext_has_type_toString = ext_has_type_toStringCreate<T> || ext_has_type_toStringConvert<T>;


	template<class T, class TypeTo>
	inline constexpr bool ext_has_data_to = (ext_has_Data<T> && std::is_constructible_v<TypeTo, ext_data_t<T> >);

	template<class T>
	inline constexpr bool ext_has_data_toBoolean = ext_has_data_to<T, Boolean>;
	template<class T>
	inline constexpr bool ext_has_data_toNumeric = ext_has_data_to<T, Numeric>;
	template<class T>
	inline constexpr bool ext_has_data_toDate = ext_has_data_to<T, DateTime>;
	template<class T>
	inline constexpr bool ext_has_data_toStringCreate = ext_has_data_to<T, String>;
	template<class T>
	inline constexpr bool ext_has_data_toStringConvert = (ext_has_Data<T> && requires(T & t) { std::to_wstring(t.ESB_CONTAINED_DATA_ACCESSOR_NAME()); });
	template<class T>
	inline constexpr bool ext_has_data_toString = ext_has_data_toStringCreate<T> || ext_has_data_toStringConvert<T>;


	template<class T>
	inline constexpr bool  ext_has_toBoolean = ext_has_type_toBoolean<T> || has_meth_ToBoolean<T> || ext_has_data_toBoolean<T>;
	template<class T>
	inline constexpr bool  ext_has_toNumeric = ext_has_type_toNumeric<T> || has_meth_ToNumeric<T> || ext_has_data_toNumeric<T>;
	template<class T>
	inline constexpr bool  ext_has_toString = ext_has_type_toString<T> || has_meth_ToString<T> || ext_has_data_toString<T>;
	template<class T>
	inline constexpr bool  ext_has_toDate = ext_has_type_toDate<T> || has_meth_ToDate<T> || ext_has_data_toDate<T>;


	// во первых мы проверяем на наличие пользовательского метода конвертации ToXXX. Таким образом пользовательский способ "перекрывает" иные возможные.
	template<class ExtValueT>
	Boolean ExtConvertToBoolean(ExtValueT& value_) {
		if constexpr (has_meth_ToBoolean<ExtValueT>)
			return value_.ESB_CONVERTER_METH_ToBoolean();
		else if constexpr (ext_has_type_toBoolean<ExtValueT>)
			return Boolean{ static_cast<bool>(value_) };
		else if constexpr (ext_has_data_toBoolean<ExtValueT>)
			return Boolean{ static_cast<bool>( value_.ESB_CONTAINED_DATA_ACCESSOR_NAME() ) };	//error C2398: Элемент "1": преобразование из "RawDataT" в "bool" требует сужающего 
		else
			static_assert(always_false_v<ExtValueT>, "Value not convertable to Boolean any way!");
	}

	template<class ExtValueT>
	Numeric ExtConvertToNumeric(ExtValueT& value_) {
		if constexpr (has_meth_ToNumeric<ExtValueT>)
			return value_.ESB_CONVERTER_METH_ToNumeric();
		else if constexpr (ext_has_type_toNumeric<ExtValueT>)
			return Numeric{ value_ };
		else if constexpr (ext_has_data_toNumeric<ExtValueT>)
			return Numeric{ value_.ESB_CONTAINED_DATA_ACCESSOR_NAME() };
		else
			static_assert(always_false_v<ExtValueT>, "Value not convertable to Numeric any way!");
	}

	template<class ExtValueT>
	DateTime ExtConvertToDate(ExtValueT& value_) {
		if constexpr (has_meth_ToDate<ExtValueT>)
			return value_.ESB_CONVERTER_METH_ToDate();
		else if constexpr (ext_has_type_toDate<ExtValueT>)
			return DateTime{ value_ };
		else if constexpr (ext_has_data_toDate<ExtValueT>)
			return DateTime{ value_.ESB_CONTAINED_DATA_ACCESSOR_NAME() };
		else
			static_assert(always_false_v<ExtValueT>, "Value not convertable to DateTime any way!");
	}

	template<class ExtValueT>
	String ExtConvertToString(ExtValueT& value_) {
		if constexpr (has_meth_ToString<ExtValueT>)
			return value_.ESB_CONVERTER_METH_ToString();
		else if constexpr (ext_has_type_toStringCreate<ExtValueT>)
			return String{ value_ };
		else if constexpr (ext_has_type_toStringConvert<ExtValueT>)
			return String{ std::to_wstring(value_) };
		else if constexpr (ext_has_data_toStringCreate<ExtValueT>)
			return String{ value_.ESB_CONTAINED_DATA_ACCESSOR_NAME() };
		else if constexpr (ext_has_data_toStringConvert<ExtValueT>)
			return String{ std::to_wstring(value_.ESB_CONTAINED_DATA_ACCESSOR_NAME()) };
		else
			static_assert(always_false_v<ExtValueT>, "Value not convertable to String any way!");
	}


	template<class ExtValueT>
	struct ExtValueConverter {
		using value_t = ExtValueT;
		using data_t = ext_value_contained_data_t<ExtValueT>;
		static const data_t& data(const ExtValueBase& v_)			{ return static_cast<const value_t&>(v_).ContainedValueData_; }
		static Boolean	ValueToBoolean(const ExtValueBase& v_)		{ return ExtConvertToBoolean(data(v_)); }
		static Numeric	ValueToNumeric(const ExtValueBase& v_)		{ return ExtConvertToNumeric(data(v_)); }
		static String	ValueToString(const ExtValueBase& v_)		{ return ExtConvertToString(data(v_)); }
		static DateTime ValueToDate(const ExtValueBase& v_)			{ return ExtConvertToDate(data(v_)); }

		static constexpr ExtValueToBooleanFn*	PfnValueToBoolean() { if constexpr (ext_has_toBoolean<data_t>) return ValueToBoolean; else return nullptr; }
		static constexpr ExtValueToNumericFn*	PfnValueToNumeric() { if constexpr (ext_has_toNumeric<data_t>) return ValueToNumeric; else return nullptr; }
		static constexpr ExtValueToStringFn*	PfnValueToString()	{ if constexpr (ext_has_toString<data_t>) return ValueToString; else return nullptr; }
		static constexpr ExtValueToDateTimeFn*	PfnValueToDate()	{ if constexpr (ext_has_toDate<data_t>) return ValueToDate; else return nullptr; }
	};
	

	inline void _test_ext_converter_() {
		static_assert(ext_has_toBoolean<long>);
		static_assert(ext_has_toNumeric<long>);
		static_assert(ext_has_toString<long>);
		static_assert(!ext_has_toDate<long>);

		struct _test_long_t {
			long m_data;
			Numeric ToNumeric() {
				return Numeric(m_data);
			}
		};
		static_assert(has_meth_ToNumeric<_test_long_t>);

		struct _test_text_t {
			std::wstring m_data;
			std::wstring& Data()				{ return m_data; }
			const std::wstring& Data() const	{ return m_data; }
		};
		constexpr bool _test_text_t_to_string = ext_has_data_toString<_test_text_t>;
		static_assert(_test_text_t_to_string, " ! ext_has_data_toString<_test_text_t> ");
	}


	
	template<class ExtTypeDescriptorT>
	struct ExtValueFactory {
		using descriptor_t = ExtTypeDescriptorT;
		using ext_value_t = typename descriptor_t::ext_value_t;
		using allocator_t = typename descriptor_t::allocator_t;
		using contained_t = ext_value_contained_data_t<ext_value_t>;
		//
		static void InstanceDelete(ExtValueBase& base_) {
			ext_value_t* v = static_cast<ext_value_t*>(&base_);
			v->~ext_value_t();
			allocator_t::Free(v);
		}
		//
		template<typename... ArgsT>
		static ext_value_t& InstanceCreate(const descriptor_t& descriptor_, ArgsT... args_)
			noexcept(noexcept(allocator_t::Alloc(sizeof(ext_value_t))) && noexcept(std::construct_at(std::declval<contained_t*>(), std::forward<ArgsT>(args_)...)))/**/
		{
			void* pv = allocator_t::Alloc(sizeof(ext_value_t));
			assert(pv != nullptr || L"аллокатор не должен возвращать нулл!" == nullptr);

			ext_value_t& v = static_cast<ext_value_t&>(descriptor_.NewValueBaseInplace(pv));

			if constexpr (noexcept(std::construct_at(std::declval<contained_t*>(), std::forward<ArgsT>(args_)...))) {
				std::construct_at(&v.ContainedValueData_, std::forward<ArgsT>(args_) ...);
			}
			else {
				try {
					std::construct_at(&v.ContainedValueData_, std::forward<ArgsT>(args_) ...);
				}
				catch (...) {
					allocator_t::Free(pv);
					throw;
				}
			}
			return v;
		}
	};





	template<class ExtObjectT>
	class ExtValueObjectValue;

	template<class ExtObjectT>
	class ExtValueSimpleValue;


ESB_WARNING_SUPRESS_NO_VIRTUAL_DTOR_ANY()
//
	template<class ExtContainedT, class ExtAllocatorT = ExtAllocator<alignof(ExtValueSimple<ExtContainedT>)>>
	struct TypeDescriptorValueSimpleAuto : public TypeDescriptorValueSimple
	{	ESB_DECLARE_NOCOPYMOVE_DTORDEFAULT(TypeDescriptorValueSimpleAuto);
		//
		using me_t = TypeDescriptorValueSimpleAuto<ExtContainedT, ExtAllocatorT>;
		using contained_t = ExtContainedT;
		using ext_value_t = ExtValueSimple<contained_t>;
		using allocator_t = ExtAllocatorT;
		using converter_t = ExtValueConverter<ext_value_t>;
		using factory_t = ExtValueFactory<me_t>;
		using esb_value_t = ExtValueSimpleValue<contained_t>;
		using interface_t = dispinterface_object_t< interface_info_t<contained_t> >;
		//для доступа из factory_t выносим в паблик
		using TypeDescriptorValueSimple::NewValueBaseInplace;
		//
		template<typename... ArgsT>
		static ext_value_t& CreateValueInstance(const me_t& td_,  ArgsT... args_) {
			return factory_t::InstanceCreate(td_, std::forward<ArgsT>(args_) ...);
		}

		static contained_t& GetContained(esb_value_t& value_)				{ return static_cast<ext_value_t&>(*get_interface(value_)).ContainedValueData_;	}
		static const contained_t& GetContained(const esb_value_t& value_)	{ return static_cast<const ext_value_t&>(*get_interface(value_)).ContainedValueData_;	}
		static contained_t&& GetContained(esb_value_t&& value_)				{ return std::move(static_cast<ext_value_t&&>(*get_interface(value_)).ContainedValueData_);	}

		constexpr TypeDescriptorValueSimpleAuto()
			: TypeDescriptorValueSimple{ interface_t::TypeId_, interface_t::TypeTerm_, interface_t::TypeDescriptionInit_, factory_t::InstanceDelete }
		{
			TypeDescriptorValue::ToBoolean_ =		converter_t::PfnValueToBoolean();
			TypeDescriptorValue::ToNumeric_ =		converter_t::PfnValueToNumeric();
			TypeDescriptorValue::ToString_ =		converter_t::PfnValueToString();
			TypeDescriptorValue::ToDateTime_ =	converter_t::PfnValueToDate();

			TypeDescriptorValue::InstanceCreatorFind_ = dispinterface_impl_ctor_find<interface_t>();
		}
	};

	
	template<class ExtContainedT, class ExtAllocatorT = ExtAllocator<alignof(ExtValueObject<ExtContainedT>)>>
	struct TypeDescriptorValueObjectStable : public TypeDescriptorValueObject 
	{	ESB_DECLARE_NOCOPYMOVE_DTORDEFAULT(TypeDescriptorValueObjectStable);
		//
		using me_t = TypeDescriptorValueObjectStable<ExtContainedT, ExtAllocatorT>;
		using contained_t = ExtContainedT;
		using ext_value_t = ExtValueObject<contained_t>;
		using allocator_t = ExtAllocatorT;
		using factory_t = ExtValueFactory<me_t>;
		using converter_t = ExtValueConverter<ext_value_t>;
		using esb_value_t = ExtValueObjectValue<contained_t>;
		using interface_t = dispinterface_object_t< interface_info_t<contained_t> >;
		// для доступа из factory_t выносим в паблик
		using TypeDescriptorValueObject::NewValueBaseInplace;

		static constexpr size_t						PropCount(const ExtValueObjectBase&)								{ return dispinterface_impl_prop_count<interface_t>(); }
		static constexpr int						PropFind(const ExtValueObjectBase&, const std::wstring_view& name_)	{ return dispinterface_impl_prop_find<interface_t>(name_); }
		static constexpr const DispInfoMembProp*	Prop(const ExtValueObjectBase&, int prop_)							{ return dispinterface_impl_memb_prop<interface_t>(prop_); }

		static constexpr size_t						MethCount(const ExtValueObjectBase&)								{ return dispinterface_impl_meth_count<interface_t>(); }
		static constexpr int						MethFind(const ExtValueObjectBase&, const std::wstring_view& name_)	{ return dispinterface_impl_meth_find<interface_t>(name_); }
		static constexpr const DispInfoMembMeth*	Meth(const ExtValueObjectBase&, int meth_)							{ return dispinterface_impl_memb_meth<interface_t>(meth_); }
		// мы единственный источник создания ext_value_t - и для TypeDef (из спп) и для ExtInvoker4CtorFromTypePack (из 1С)
		template<typename... ArgsT>
		static ext_value_t& CreateValueInstance(const me_t& td_, ArgsT... args_) {
			return factory_t::InstanceCreate(td_, std::forward<ArgsT>(args_) ...);
		}
		
		static contained_t& GetContained(esb_value_t& value_)				{ return static_cast<ext_value_t&>(*get_interface(value_)).ContainedValueData_;	}
		static const contained_t& GetContained(const esb_value_t& value_)	{ return static_cast<const ext_value_t&>(*get_interface(value_)).ContainedValueData_;	}
		static contained_t&& GetContained(esb_value_t&& value_)				{ return std::move(static_cast<ext_value_t&&>(*get_interface(value_)).ContainedValueData_);	}

		constexpr TypeDescriptorValueObjectStable() :
			TypeDescriptorValueObject(interface_t::TypeId_, interface_t::TypeTerm_, interface_t::TypeDescriptionInit_, factory_t::InstanceDelete)
		{
			TypeDescriptorValue::ToBoolean_ = converter_t::PfnValueToBoolean();
			TypeDescriptorValue::ToNumeric_ = converter_t::PfnValueToNumeric();
			TypeDescriptorValue::ToString_ = converter_t::PfnValueToString();
			TypeDescriptorValue::ToDateTime_ = converter_t::PfnValueToDate();

			ObjPropCount_ = PropCount;
			ObjPropFind_ = PropFind;
			ObjProp_ = Prop;

			ObjMethCount_ = MethCount;
			ObjMethFind_ = MethFind;
			ObjMeth_ = Meth;
			
			TypeDescriptorValue::InstanceCreatorFind_ = dispinterface_impl_ctor_find<interface_t>();
		}
	};
//
ESB_WARNING_RESTORE()	//ESB_WARNING_SUPRESS_NO_VIRTUAL_DTOR_ANY


	//TOBE: ExtValueSimpleValue & ExtValueObjectValue (вроде) отличаются только базовыми классами
	//		Устаканится нужно сливать в один ExtValueImpl<Base>
	template<class ExtValueDataT>
	class ExtValueSimpleValue : public esb::Value
	{
		using me_t = ExtValueSimpleValue<ExtValueDataT>;
	public:
		using contained_t = ExtValueDataT;
		using descriptor_t = interface_info_t<contained_t>::descriptor_t;
		using typedef_t = ExtTypeDef<descriptor_t>;
	private:
		ESB_CLASS_IMPLEMENT_MAKE_VAL(ExtValueSimpleValue)
		ESB_CLASS_IMPLEMENT_MAKE_OPT(ExtValueSimpleValue)
		static inline typedef_t						TypeObj_{};
		static inline const RuntimeRegistratorInit	on_init_{ MemberMethInvoker<&TypeObj_, &typedef_t::init>::invoke };
	public:
		static inline const TypeDef&				Type_{ TypeObj_ };
	public:
		template<typename... ArgsT>
		static me_t Create(ArgsT... args_) {
			return make<me_t>( IValuePtr{ TypeObj_.CreateValueInstance( std::forward<ArgsT>(args_) ... ) } );
		}
	public:
		contained_t& GetContained() &					{ return descriptor_t::GetContained(*this); }
		const contained_t& GetContained() const &		{ return descriptor_t::GetContained(*this); }
		contained_t&& GetContained() &&					{ return descriptor_t::GetContained(*this); }

		explicit operator contained_t& () &				{ return descriptor_t::GetContained(*this); }
		explicit operator const contained_t& () const & { return descriptor_t::GetContained(*this); }
		explicit operator contained_t&& () &&			{ return descriptor_t::GetContained(*this); }
	};

	template<class T>
	inline constexpr bool is_contained_has_DataType = requires {typename T::DataType; };
	template<class T, bool>
	struct ext_contained_DataType {
		using type = void;
	};
	template<class T>
	struct ext_contained_DataType<T, true> {
		using type = T::DataType;
	};
	template<class T>
	using ext_contained_DataType_t = ext_contained_DataType<T, is_contained_has_DataType<T>>::type;

	template<class ExtObjectT>
	class ExtValueObjectValue : public esb::Object
	{
		using me_t = ExtValueObjectValue<ExtObjectT>;
	public:
		using contained_t = ExtObjectT;
		using descriptor_t = interface_info_t<contained_t>::descriptor_t;
		using typedef_t = ExtTypeDef<descriptor_t>;
		using ContainedDataType = ext_contained_DataType_t<contained_t>;
	private:
		ESB_CLASS_IMPLEMENT_MAKE_OBJ(ExtValueObjectValue)
		ESB_CLASS_IMPLEMENT_MAKE_OPT(ExtValueObjectValue)
		static inline typedef_t						TypeObj_{};
		static inline const RuntimeRegistratorInit	on_init_{ MemberMethInvoker<&TypeObj_, &typedef_t::init>::invoke };
	public:
		static inline const TypeDef&				Type_{ TypeObj_ };
	public:
		template<typename... ArgsT>
		static me_t Create(ArgsT... args_) {
			return make<me_t>(IValuePtr{ TypeObj_.CreateValueInstance(std::forward<ArgsT>(args_) ...) });
		}
	public:
		contained_t& GetContained() &					{ return descriptor_t::GetContained(*this); }
		const contained_t& GetContained() const &		{ return descriptor_t::GetContained(*this); }
		contained_t&& GetContained() &&					{ return descriptor_t::GetContained(*this); }

		explicit operator contained_t& () &				{ return descriptor_t::GetContained(*this); }
		explicit operator const contained_t& () const & { return descriptor_t::GetContained(*this); }
		explicit operator contained_t&& () &&			{ return descriptor_t::GetContained(*this); }
	};



ESB_WARNING_SUPRESS_NO_VIRTUAL_DTOR_ANY()
//
	template<typename ExtTypeDefT>
	class ExtTypeFactoryImplementation : public IClassFactory
	{
	public:
		STDMETHOD(QueryInterface)(_In_ REFIID iid, _Deref_out_ void** ppvObject) throw() override final {
			if (ppvObject == NULL)							return E_POINTER;
			else if (is_equal_guid(IID_IClassFactory, iid))	*ppvObject = static_cast<IClassFactory*>(this);
			else if (is_equal_guid(IID_IUnknown, iid))		*ppvObject = static_cast<IUnknown*>(static_cast<IClassFactory*>(this));
			else											return E_NOINTERFACE;

			return S_OK;
		}
		STDMETHOD_(ULONG, AddRef)() throw() override final		{ return 1; }
		STDMETHOD_(ULONG, Release)() throw() override final		{ return 1; }
	public:
		virtual HRESULT STDMETHODCALLTYPE CreateInstance(IUnknown* pUnkOuter, REFIID riid, void** ppvObject) override final {
			if (ppvObject == nullptr) return E_POINTER;
			if (pUnkOuter != nullptr) return CLASS_E_NOAGGREGATION;
			IType& t = const_cast<IType&>( static_cast<const IType&>( ExtTypeDefT::GetTypeDescriptor() ) );
			return t.QueryInterface(riid, ppvObject);
		}
		virtual HRESULT STDMETHODCALLTYPE LockServer([[maybe_unused]] BOOL fLock) override final		{ return 0; }
	public:
		static ExtTypeFactoryImplementation<ExtTypeDefT> Instance_;		//not const due to IClassFactory api
	public:
		static bool RegisterType()	{ return ExtRegisterType(ExtTypeDefT::GetTypeDescriptor(), Instance_); }
		static bool RevokeType()	{ return ExtRevokeType(ExtTypeDefT::GetTypeDescriptor()); }
	};
//
ESB_WARNING_RESTORE()	//ESB_WARNING_SUPRESS_NO_VIRTUAL_DTOR_ANY



#define ESB_INTERFACEDEF_VALUE_T(CLASS_T_)				ESB_INTERFACEDEF_DESCRIPTOR_T(CLASS_T_)::esb_value_t
#define ESB_INTERFACEDEF_FACTORY_T(CLASS_T_)			ExtTypeFactoryImplementation< ESB_INTERFACEDEF_VALUE_T(CLASS_T_) :: typedef_t >
#define ESB_INTERFACEDEF_FACTORY_INSTANCE(CLASS_T_)		inline ESB_INTERFACEDEF_FACTORY_T(CLASS_T_) ESB_INTERFACEDEF_FACTORY_T(CLASS_T_)::Instance_{}


#define ESB_VALUEDEF(ESB_NAME_, CLASS_T_, GUID_STR_, NAME_PAIR_, TEXT_)		\
	ESB_INTERFACEDEF_VALUE(CLASS_T_, GUID_STR_, NAME_PAIR_, TEXT_);			\
	using ESB_NAME_ =  ESB_INTERFACEDEF_VALUE_T(CLASS_T_)


#define ESB_VALUEDEF_WITH_FACTORY(ESB_NAME_, CLASS_T_, GUID_STR_, NAME_PAIR_, TEXT_)		\
	ESB_VALUEDEF(ESB_NAME_, CLASS_T_, GUID_STR_, NAME_PAIR_, TEXT_);						\
	ESB_INTERFACEDEF_FACTORY_INSTANCE(CLASS_T_)


template<class EsbExtEntityT>
class ExtEntityRegistrator {
	using entity_t = EsbExtEntityT;
	using typedef_t = entity_t::typedef_t;
	using factory_t = ExtTypeFactoryImplementation<typedef_t>;
	//NOTE	Поскольку сам тип в среде 1С должен быть зарегисирирован ПОСЛЕ инициализации его дескриптора, а порядок инициализации статических
	//		переменных у с++ неопределен, то инит дескриптора помещяется в голову списка, а регистратор в хвост. Таким образом всегда,
	//		при любой последовательности инициализации статик-регистраторов инит-дескриптора предшествует регистрации типа в платформе
	static inline const RuntimeRegistratorInit on_runtime_init_{ RuntimeRegistratorInit::last(), factory_t::RegisterType };
	//NOTE	Нам можно не увязывать дерегистрацию с успешностью регистрации, т.к. если список регистрации не будет успешно
	//		выполнен до конца, то загрузка будет просто прервана.
	//		А процедура дерегистрации (OnComponentInitCleanup) выписана так чтобы быть no-error - учитывает варианты аварийной выгрузки.
	//		дерегистрация ни с чем не связана, поэтому просто кладем в голову.
	static inline const RuntimeRegistratorTerm on_runtime_term_{ VoidifyInvoker<factory_t::RevokeType>::invoke };
	// наши инстансы on_runtime_... static inline, но компилятор не хочет их делать если как-то не его не попросить. просим сделать фейковую инстанс этого класса
	// (оптимизатор ее все равно вырезает, а static inline on_runtime_... остаются.
	static const ExtEntityRegistrator Instance_;
};

#define ESB_VALUEDEF_WITH_FACTORY_AND_REG(ESB_NAME_, CLASS_T_, GUID_STR_, NAME_PAIR_, TEXT_)		\
	ESB_VALUEDEF_WITH_FACTORY(ESB_NAME_, CLASS_T_, GUID_STR_, NAME_PAIR_, TEXT_);					\
	inline const ExtEntityRegistrator<ESB_NAME_> ExtEntityRegistrator<ESB_NAME_>::Instance_{}


#define ESB_INTERFACEDEF_DONE_WITH_VALUE_AND_FACTORY(CLASS_T_, ESB_NAME_)	\
	ESB_INTERFACEDEF_DONE(CLASS_T_);	\
	using ESB_NAME_ =  ESB_INTERFACEDEF_VALUE_T(CLASS_T_);	\
	ESB_INTERFACEDEF_FACTORY_INSTANCE(CLASS_T_)

#define ESB_INTERFACEDEF_DONE_WITH_VALUE_AND_FACTORY_REG(CLASS_T_, ESB_NAME_)	\
	ESB_INTERFACEDEF_DONE_WITH_VALUE_AND_FACTORY(CLASS_T_, ESB_NAME_);	\
	inline const ExtEntityRegistrator<ESB_NAME_> ExtEntityRegistrator<ESB_NAME_>::Instance_{}


// Специфический авто-типа-регистратор для DelegatToMeth
// Поскольку это уже полностью специализированный тип мы не можем делать поля on_runtime_init_/on_runtime_term_ static inline
// т.к. компилятор их тогда инициализирует сразу без нашего ведома.
// Мы сделаем эти поля полями экземпляра и будем при необходимости делать inline Instance_ этого класса
// Тогда on_runtime_init_/on_runtime_term_ будут проинициализированы в конструкторе этой Instance_
// Обращаю внимание, что и конструктор и Instance_ приватны - можно создать только один экземпляр этой специализации 
// (RuntimeRegistratorInit/RuntimeRegistratorTerm некопируемы)
template<>
class ExtEntityRegistrator<DelegatToMeth> {
	const RuntimeRegistratorInit on_runtime_init_;
	const RuntimeRegistratorTerm on_runtime_term_;
	ExtEntityRegistrator() : on_runtime_init_{ RuntimeRegistratorInit::last(), TypeDefDelegatToMeth::ExtRegisterType },
		on_runtime_term_{ VoidifyInvoker<TypeDefDelegatToMeth::ExtRevokeType>::invoke }
	{}
	static const ExtEntityRegistrator Instance_;
};
#define ESB_REGISTER_AUTO_DELEGAT_TO_METH()	\
	inline const ExtEntityRegistrator<DelegatToMeth> ExtEntityRegistrator<DelegatToMeth>::Instance_{}


} //namespace esb-ext



#endif	//ESB_EXT_H

