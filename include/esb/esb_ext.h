/*
	"ESB" is designed to create external native components for the 1C(tm) program.
	Copyright © 2023 IntUnsigned	(v8classes@gmail.com)
	"ESB" is free software under LGPLv2.1 license with essential restriction about binary part (view esb-license.txt)
	"ESB" IS DISTRIBUTED "AS-IS" WITHOUT ANY, EVEN THE IMPLIED, WARRANTY. IN NO EVENT SHALL THE AUTHORS OR THE COPYRIGHT OWNER BE LIABLE FOR ANY CLAIM, 
	DAMAGES OR OTHER LIABILITY ARISING OUT OF THE USE OF THIS SOFTWARE.
	THE AUTHOR DOES NOT GIVE ANY GUARANTEES AND DOES NOT BEAR ANY RESPONSIBILITY REGARDING THE USE OF 1C(tm) COMPONENTS PRODUCED USING THE ESB LIBRARY IN THE 1C(tm) ENVIRONMENT.
*/


#pragma once
#ifndef ESB_EXT_H_
#define ESB_EXT_H_
#include <cassert>

#include "esb_core.h"
#include "esb_meta.h"
#include "esb/afx/afx_tuple.h"

#include <array>
#include <tuple>
#include <span>


namespace esb 
{
	namespace _internal
	{
		using esb::make;	//TODO	тоже в _internal ее


		//INFO	TypeDescriptorValueSimple и TypeDescriptorValueObject это структуры-интерфейсы для esbhlp, которые описывают какие-то типы
		//		собственно описателями этих типов будут наследники этих структур. строго по одному классу-дескриптору на каждый тип.
		//		Этим классом-дескриптором (наследником от ...) мы специализируем ExtTypeDef и он создает инстанс дескриптора.
		template<TypeDescriptorConcept ExtTypeDescriptorT>
		struct ExtTypeDef : public TypeDef 
		{
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
				TypeDef::on_runtime_init(descriptor_t::InitTypeInfoInterface(DescriptorInstance_));
				return true;
			}
			static const descriptor_t& GetTypeDescriptor() {
				return DescriptorInstance_;
			}

			template<typename... ArgsT>
			ext_value_t& CreateValueInstance(ArgsT&&... args_) const {
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

#define ESB_DECLARE_CONTAINED_DATA_RO(DATA_NAME_)									\
	const auto&	ESB_CONTAINED_DATA_ACCESSOR_NAME() const	{ return DATA_NAME_; }

#define ESB_DECLARE_CONTAINED_DATA_RW(DATA_NAME_)									\
	ESB_DECLARE_CONTAINED_DATA_RO(DATA_NAME_)										\
	auto& ESB_CONTAINED_DATA_ACCESSOR_NAME()				{ return DATA_NAME_; }	


		template<class T>
		constexpr bool is_contained_has_Data = (requires(T* v) { v->ESB_CONTAINED_DATA_ACCESSOR_NAME(); });

		template<bool, typename ContainedT>
		struct contained_data_accessor_const {
			using type	= std::remove_reference_t< decltype(std::declval<const ContainedT>().ESB_CONTAINED_DATA_ACCESSOR_NAME()) >;
		};
		template<typename ContainedT>
		struct contained_data_accessor_const<false, ContainedT> {
			using type = void;
		};
		template<typename ContainedT>
		using contained_data_accessor_const_t = contained_data_accessor_const<is_contained_has_Data<ContainedT>, ContainedT>::type;
		//
		template<bool, typename ContainedT>
		struct contained_data_accessor {
			using type = std::remove_reference_t< decltype(std::declval<ContainedT>().ESB_CONTAINED_DATA_ACCESSOR_NAME()) >;
		};
		template<typename ContainedT>
		struct contained_data_accessor<false, ContainedT> {
			using type = void;
		};
		template<typename ContainedT>
		using contained_data_accessor_t = contained_data_accessor<is_contained_has_Data<ContainedT>, ContainedT>::type;




		//TODO	Весь "механизм" авто-определеления iterable/indexable/etc.. опирается либбо на присутствие в самом классе соответствующих методов
		//		либо на то, что у класса есть "магический" .ESB_CONTAINED_DATA_ACCESSOR_NAME().
		//		Нужен третий, промежуточный и основной вариснт, когда в interface_info_t определены нужные методы
		//		как сделано для disp-интерфейса.
		//		ТО ЕСТЬ. Индексируемяй - это тот, у которого в интерфейсе есть "методы индексации", и ссылаться они должны на что-то в объекте
		//		И итерируемяй - это тот у кого есть соответствующий метод в интерфейсе (может реализовываться на базе итератора, индексируемого, т.п.)
		template<typename T>
		inline constexpr bool is_contained_indexable_r = is_contained_has_Data<T> && is_entity_indexable_r< contained_data_accessor_const_t<T> >;
		template<typename T>
		inline constexpr bool is_contained_indexable_w = is_contained_has_Data<T> && is_entity_indexable_w< contained_data_accessor_t<T> >;
		template<typename ContainedT>
		inline constexpr bool is_contained_iterable = is_contained_has_Data<ContainedT> && is_entity_iteratable< contained_data_accessor_const_t<ContainedT> >;


		template<typename InterfaceT>
		inline constexpr bool	is_interface_describe_object = is_interface_has_meth_meta<InterfaceT> || is_interface_has_prop_meta<InterfaceT>;
		//
		template<typename InterfaceT>
		inline constexpr bool	is_interface_describe_indexable_rw	=	is_contained_has_SetValue< interface_contained_t<InterfaceT>> ||
																		is_contained_indexable_w< interface_contained_t<InterfaceT>>;
		//
		template<typename InterfaceT>	//!is_interface_describe_indexable_rw - ВАЖНО, т.к. должен быть либо ..RW либо ..RO !
		inline constexpr bool	is_interface_describe_indexable_ro	=	(	is_contained_has_GetValue<interface_contained_t<InterfaceT>> || 
																			is_contained_indexable_r<interface_contained_t<InterfaceT>>
																		) &&
																		!is_interface_describe_indexable_rw<InterfaceT>;
		//
		template<typename InterfaceT>
		inline constexpr bool	is_interface_describe_iterable		=	is_contained_has_GetEnumerator<interface_contained_t<InterfaceT>> || 
																		is_contained_iterable<interface_contained_t<InterfaceT>>;





		// Value Conversion
		//////////////////////////////////////////////////////////////////////////////
		//
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
		inline constexpr bool ext_has_type_toStringConvert = requires(T & t) { to_ustring(t); };
		template<class T>
		inline constexpr bool ext_has_type_toString = ext_has_type_toStringCreate<T> || ext_has_type_toStringConvert<T>;


		template<class T, class TypeTo>
		inline constexpr bool ext_has_data_to = (is_contained_has_Data<T> && std::is_constructible_v<TypeTo, contained_data_accessor_t<T> >);

		template<class T>
		inline constexpr bool ext_has_data_toBoolean = ext_has_data_to<T, Boolean>;
		template<class T>
		inline constexpr bool ext_has_data_toNumeric = ext_has_data_to<T, Numeric>;
		template<class T>
		inline constexpr bool ext_has_data_toDate = ext_has_data_to<T, DateTime>;
		template<class T>
		inline constexpr bool ext_has_data_toStringCreate = ext_has_data_to<T, String>;
		template<class T>
		inline constexpr bool ext_has_data_toStringConvert = (is_contained_has_Data<T> && requires(T & t) { to_ustring(t.ESB_CONTAINED_DATA_ACCESSOR_NAME()); });
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
				return Boolean{ static_cast<bool>(value_.ESB_CONTAINED_DATA_ACCESSOR_NAME()) };	//error C2398: Элемент "1": преобразование из "RawDataT" в "bool" требует сужающего 
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
				return String{ to_ustring(value_) };
			else if constexpr (ext_has_data_toStringCreate<ExtValueT>)
				return String{ value_.ESB_CONTAINED_DATA_ACCESSOR_NAME() };
			else if constexpr (ext_has_data_toStringConvert<ExtValueT>)
				return String{ to_ustring(value_.ESB_CONTAINED_DATA_ACCESSOR_NAME()) };
			else
				static_assert(always_false_v<ExtValueT>, "Value not convertable to String any way!");
		}


		template<class ExtValueT>
		struct ExtValueConverter 
		{
			using value_t = ExtValueT;
			using data_t = value_t::ContainedDataType;	// ext_value_contained_data_t<ExtValueT>;
			static const data_t& data(ExValueBase& v_)			{ return static_cast<const value_t&>(v_).ContainedData_; }
			static Boolean	ValueToBoolean(ExValueBase& v_)		{ return ExtConvertToBoolean(data(v_)); }
			static Numeric	ValueToNumeric(ExValueBase& v_)		{ return ExtConvertToNumeric(data(v_)); }
			static String	ValueToString(ExValueBase& v_)		{ return ExtConvertToString(data(v_)); }
			static DateTime ValueToDate(ExValueBase& v_)		{ return ExtConvertToDate(data(v_)); }

			static constexpr ExValueToBooleanFn* PfnValueToBoolean()	{ if constexpr (ext_has_toBoolean<data_t>) return ValueToBoolean; else return nullptr; }
			static constexpr ExValueToNumericFn* PfnValueToNumeric()	{ if constexpr (ext_has_toNumeric<data_t>) return ValueToNumeric; else return nullptr; }
			static constexpr ExValueToStringFn* PfnValueToString()		{ if constexpr (ext_has_toString<data_t>) return ValueToString; else return nullptr; }
			static constexpr ExValueToDateTimeFn* PfnValueToDate()		{ if constexpr (ext_has_toDate<data_t>) return ValueToDate; else return nullptr; }
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
				string_t m_data;
				string_t& Data() { return m_data; }
				const string_t& Data() const { return m_data; }
			};
			constexpr bool _test_text_t_to_string = ext_has_data_toString<_test_text_t>;
			static_assert(_test_text_t_to_string, " ! ext_has_data_toString<_test_text_t> ");
		}
		// end Value Conversion
		/////////////////////////////////////////////////////////////////////////////////////



		template<class ExtTypeDescriptorT>
		struct ExtValueFactory {
			using descriptor_t = ExtTypeDescriptorT;
			using ext_value_t = typename descriptor_t::ext_value_t;
			using allocator_t = typename descriptor_t::allocator_t;
			using contained_t = typename descriptor_t::contained_t;
			//
			static void InstanceDelete(ExInstanceBaseForDeleter& base_) {
				ext_value_t* v = static_cast<ext_value_t*>(&base_);
				v->~ext_value_t();
				allocator_t::Free(v);
			}
			//
			template<typename... ArgsT>
			static ext_value_t& InstanceCreate(const descriptor_t& descriptor_, ArgsT&&... args_)
				noexcept(noexcept(allocator_t::Alloc(sizeof(ext_value_t))) && noexcept(std::construct_at(std::declval<contained_t*>(), std::forward<ArgsT>(args_)...)))/**/
			{
				void* pv = allocator_t::Alloc(sizeof(ext_value_t));
				assert(pv != nullptr || L"аллокатор не должен возвращать нулл!" == nullptr);

				ext_value_t& v = static_cast<ext_value_t&>(descriptor_.NewValueBaseInplace(pv));

				if constexpr (noexcept(std::construct_at(std::declval<contained_t*>(), std::forward<ArgsT>(args_)...))) {
					std::construct_at(&v.ContainedData_, std::forward<ArgsT>(args_) ...);
				}
				else {
					try {
						std::construct_at(&v.ContainedData_, std::forward<ArgsT>(args_) ...);
					}
					catch (...) {
						allocator_t::Free(pv);
						throw;
					}
				}
				return v;
			}
		};




		/////////////////////////////////////////////////////////////////////////////////////////////
		// Дескрипторы
		ESB_WARNING_SUPRESS_NO_VIRTUAL_DTOR_ANY()
		//
		///////////////////////////
		using TypeInfoTupleAll = std::tuple<TypeInfoFor_IObject, TypeInfoFor_IxCollectionRW, TypeInfoFor_IxCollectionRO, TypeInfoFor_IEnumeratable>;
		//
		template<typename T, typename InterfaceT>
		struct TypeInfoTupleForInterfaceFilter;
		//
	#define DECLARE_TYPEINFO_TUPLE_FLT(TYPEINFO_T_, PRED_)	\
		template<typename InterfaceT>		struct TypeInfoTupleForInterfaceFilter<TYPEINFO_T_, InterfaceT> : std::bool_constant< PRED_<InterfaceT> > {}
		//
		DECLARE_TYPEINFO_TUPLE_FLT(TypeInfoFor_IObject,			is_interface_describe_object);
		DECLARE_TYPEINFO_TUPLE_FLT(TypeInfoFor_IxCollectionRW,	is_interface_describe_indexable_rw);
		DECLARE_TYPEINFO_TUPLE_FLT(TypeInfoFor_IxCollectionRO,	is_interface_describe_indexable_ro);
		DECLARE_TYPEINFO_TUPLE_FLT(TypeInfoFor_IEnumeratable,	is_interface_describe_iterable);
	#undef DECLARE_TYPEINFO_TUPLE_FLT
		//
		template<typename InterfaceT>
		using TypeInfoTupleForInterface = afx::tuple_filter_t<TypeInfoTupleAll, TypeInfoTupleForInterfaceFilter, InterfaceT>;


		template<typename ContainedT, typename TypeInfoTupleT>
		struct ex_entity_value_for;
		//
		template<typename ContainedT, typename... TypeInfoTs>
		struct ex_entity_value_for<ContainedT, std::tuple<TypeInfoTs...>> {
			using type = ExEntityFor<	ContainedT,
										TypeInfoFor	<
													std::conditional_t<std::is_same_v<ContainedT, void>, TypeInfoFor_IValueSingleton, TypeInfoFor_IValueInstance>,
													TypeInfoTs...
													>
								>;
		};
		//
		template<typename InterfaceT>
		struct ex_entity_value_for_interface {
			static_assert(is_interface_has_object_field<InterfaceT>, "InterfaceT not specialized");
			using type = ex_entity_value_for< interface_contained_t<InterfaceT>, TypeInfoTupleForInterface<InterfaceT> >::type;
		};
		template<typename InterfaceT>
		using ex_entity_value_for_interface_t = ex_entity_value_for_interface<InterfaceT>::type;
		//
		///////////////////////////


		// Это "есб-валюе" (которое унаследовано от esb::Value, фактически IValuePtr) для дескрипторов. 
		// они предоставляют "сервис" доступа к содержащимся "за IValuePtr" данным (ContainedT) - 
		// конвертации из ValueForExEntity в ExValue и получении .ContainedData_
		// (определение после будет, им надо тоже знать о своих дескрипторах)
		template<class InterfaceT, typename ContainedT>
		class ValueForExEntity;


		//TODO	Мы можем декларировать конструктор дескриптора constexpr, но из-за поля String TypeDescription
		//		он не удовлетворяет требованиям constexpr из-за деструктора
		//		Нужно сделать отдельный on_runtime_term список для деструкторов и TypeDescription делать placment-new/costom~dtor()
	#define TYPEDESCRIPTOR_CONSTRUCT_CONSTEXPR	constexpr

		template<typename InterfaceT, typename ContainedT>
		struct TypeDescriptorImplement_IObj
		{
			using meta_interface_t = InterfaceT;
			using contained_t = ContainedT;
			using ext_value_t = ex_entity_value_for_interface_t<meta_interface_t>;
			using disp_interface_t = dispinterface_object_t< meta_interface_t >;

			static constexpr size_t						PropCount(ExValueBase&)			{ return dispinterface_impl_prop_count<disp_interface_t>(); }
			static constexpr int						PropFind(ExValueBase&, const string_view_t& name_)	{ return dispinterface_impl_prop_find<disp_interface_t>(name_); }
			static constexpr const DispInfoMembProp*	Prop(ExValueBase&, int prop_)	{ return dispinterface_impl_memb_prop<disp_interface_t>(prop_); }

			static constexpr size_t						MethCount(ExValueBase&)			{ return dispinterface_impl_meth_count<disp_interface_t>(); }
			static constexpr int						MethFind(ExValueBase&, const string_view_t& name_)	{ return dispinterface_impl_meth_find<disp_interface_t>(name_); }
			static constexpr const DispInfoMembMeth*	Meth(ExValueBase&, int meth_)	{ return dispinterface_impl_memb_meth<disp_interface_t>(meth_); }

			TYPEDESCRIPTOR_CONSTRUCT_CONSTEXPR
			static void Construct(TypeInfoFor_IObject& it)
			{
				it.ObjPropCount_ = PropCount;
				it.ObjPropFind_ = PropFind;
				it.ObjProp_ = Prop;
				//
				it.ObjMethCount_ = MethCount;
				it.ObjMethFind_ = MethFind;
				it.ObjMeth_ = Meth;
			}
		};


		//TODO	Унаследовать/совместить _IxRO/_IxRW. Копипасты много!
		template<typename InterfaceT, typename ContainedT>
		struct TypeDescriptorImplement_IxRW
		{
			using meta_interface_t	= InterfaceT;
			using contained_t		= ContainedT;
			using ext_value_t		= ex_entity_value_for_interface_t<meta_interface_t>;

			static size_t		IxCollection_Size(const ExValueBase& obj_base_) {			
				if constexpr (is_contained_has_Size<contained_t>) {
					const contained_t& container = static_cast<const ext_value_t&>(obj_base_).ContainedData_;
					return container.ESB_CONTAINED_IXCOLLECTION_SIZE();
				}
				else {
					static_assert(is_contained_indexable_w<contained_t>, "ContainedData_ not indexable container.");
					const contained_t& contained = static_cast<const ext_value_t&>(obj_base_).ContainedData_;
					return contained.ESB_CONTAINED_DATA_ACCESSOR_NAME().size();
				}
			}
			static IValuePtr	IxCollection_GetValue(const ExValueBase& obj_base_, size_t index_) {
				if constexpr (is_contained_has_GetValue<contained_t>) {
					const contained_t& container = static_cast<const ext_value_t&>(obj_base_).ContainedData_;
					return container.ESB_CONTAINED_IXCOLLECTION_GETVALUE(index_);
				}
				else {
					static_assert(is_contained_indexable_w<contained_t>, "ContainedData_ not indexable container.");
					const contained_t& contained = static_cast<const ext_value_t&>(obj_base_).ContainedData_;
					if (index_ >= contained.ESB_CONTAINED_DATA_ACCESSOR_NAME().size())
						ESL_THROW_ARRAY_INDEX_ERR();
					return ex_get_value_interface(contained.ESB_CONTAINED_DATA_ACCESSOR_NAME()[index_]);
				}
			}
			static void			IxCollection_SetValue(ExValueBase& obj_base_, size_t index_, IValuePtr byval_value_) {
				contained_t& contained = static_cast<ext_value_t&>(obj_base_).ContainedData_;
				if constexpr (is_contained_has_SetValue<contained_t>) {
					contained_t& container = static_cast<ext_value_t&>(obj_base_).ContainedData_;
					return container.ESB_CONTAINED_IXCOLLECTION_SETVALUE(index_, std::move(byval_value_));
				}
				else {
					static_assert(is_contained_indexable_w<contained_t>, "ContainedData_ not indexable container.");
					contained_t& contained = static_cast<ext_value_t&>(obj_base_).ContainedData_;
					if (index_ >= contained.ESB_CONTAINED_DATA_ACCESSOR_NAME().size())
						ESL_THROW_ARRAY_INDEX_ERR();

					using contained_container_t = contained_data_accessor_t<contained_t>;
					using contained_container_value_t = contained_container_t::value_type;
					contained.ESB_CONTAINED_DATA_ACCESSOR_NAME()[index_] = ex_get_value_from_interface<contained_container_value_t>(std::move(byval_value_));
				}
			}

			TYPEDESCRIPTOR_CONSTRUCT_CONSTEXPR
			static void Construct(TypeInfoFor_IxCollectionRW& it)
			{
				it.PfnGetSize_ = IxCollection_Size;
				it.PfnGetValue_ = IxCollection_GetValue;
				it.PfnSetValue_ = IxCollection_SetValue;
			}
		};


		template<typename InterfaceT, typename ContainedT>
		struct TypeDescriptorImplement_IxRO
		{
			using meta_interface_t	= InterfaceT;
			using contained_t		= ContainedT;
			using ext_value_t		= ex_entity_value_for_interface_t<meta_interface_t>;
			//
			static size_t		IxCollection_Size(const ExValueBase& obj_base_) {
				if constexpr (is_contained_has_Size<contained_t>) {
					const contained_t& container = static_cast<const ext_value_t&>(obj_base_).ContainedData_;
					return container.ESB_CONTAINED_IXCOLLECTION_SIZE();
				}
				else {
					static_assert(is_contained_indexable_r<contained_t>, "ContainedData_ not indexable container.");
					const contained_t& contained = static_cast<const ext_value_t&>(obj_base_).ContainedData_;
					return contained.ESB_CONTAINED_DATA_ACCESSOR_NAME().size();
				}
			}
			static IValuePtr	IxCollection_GetValue(const ExValueBase& obj_base_, size_t index_) {
				if constexpr (is_contained_has_GetValue<contained_t>) {
					const contained_t& container = static_cast<const ext_value_t&>(obj_base_).ContainedData_;
					return container.ESB_CONTAINED_IXCOLLECTION_GETVALUE(index_);
				}
				else {
					static_assert(is_contained_indexable_r<contained_t>, "ContainedData_ not indexable container.");
					const contained_t& contained = static_cast<const ext_value_t&>(obj_base_).ContainedData_;
					if (index_ >= contained.ESB_CONTAINED_DATA_ACCESSOR_NAME().size())
						ESL_THROW_ARRAY_INDEX_ERR();
					return ex_get_value_interface(contained.ESB_CONTAINED_DATA_ACCESSOR_NAME()[index_]);
				}
			}
			//
			TYPEDESCRIPTOR_CONSTRUCT_CONSTEXPR
			static void Construct(TypeInfoFor_IxCollectionRO& it)
			{
				it.PfnGetSize_ = IxCollection_Size;
				it.PfnGetValue_ = IxCollection_GetValue;
			}

		};


		// Енумератор для объектов, которые можно итерировать через const_iterator от begin(container_) до end(container_)
		//TODO	НЕДОДЕЛАНО.	Для Reset/Skip/Clone. 
		//		Для Reset нужно держать и сам контейнер. 
		//		Для Skip нужны соответствующие итераторы (через m_end перескакивать нельзя!) 
		//		Для Clone это вообще не здесь надо делать, а там где нас создают
		//		Возможно и Reset нужно там же делать
		template<typename ContainerT>
		struct ExEnumeratorForContainer
		{
			using container_t		= ContainerT;
			using container_value_t = container_t::value_type;
			container_t::const_iterator m_1st;
			container_t::const_iterator m_end;
	#if defined(ESB_USE_OWNING_ENUMERATOR) && ESB_USE_OWNING_ENUMERATOR
			IEntityPtr					m_container_handler;
			ExEnumeratorForContainer(IEntityPtr&& entity_handler_, const container_t& container_)
				: m_container_handler{ std::move(entity_handler_) }, m_1st{ begin(container_) }, m_end{ end(container_) }
			{ }
	#else
			ExEnumeratorForContainer(const container_t& container_)
				: m_1st{ begin(container_) }, m_end{ end(container_) }
			{ }
	#endif
			IValuePtr	GetValueAndMoveNext() {
				if (m_1st == m_end)
					return { nullptr };

				const container_value_t& value = *m_1st;
				++m_1st;
				return esb::_internal::ex_get_value_interface( value );
			}
			bool		Reset() {						//TODO
				return false;
			}
			bool		Skip(size_t c_elem_to_skip_) {	//TODO
				//std::advance(m_1st, c_elem_to_skip_);
				return false;
			}
			IValuePtr	Clone() {						//TODO
				return { nullptr };
			}
		};


		template<typename ExEnumeratorForContainerT>
		using ExEntityForEnumerator = ExEntityFor< ExEnumeratorForContainerT, TypeInfoForEnumerator >;
		//
		struct TypeDescriptorForEnumerateBase : public TypeInfoFor<TypeInfoFor_IEnumValues, TypeInfoFor_Instance>
		{
			ESB_DECLARE_NOCOPYMOVE_DTORDEFAULT(TypeDescriptorForEnumerateBase);
			//
			using mybase_t = TypeInfoFor<TypeInfoFor_IEnumValues, TypeInfoFor_Instance>;
			// для доступа из factory_t выносим в паблик
			using mybase_t::NewValueBaseInplace;

			static constexpr CLSID				TypeId_ = guid_from_hex("5A8AB656-6EF3-4FB1-9CE7-745806AE1E2B");
			static constexpr FixedTerm			TypeTerm_{ ESB_T("ConllectionEnumerator"), ESB_T("ЕнумераторКоллекции") };
			static constexpr strview_t			TypeDescriptionInit_{ ESB_T("Енумератор коллекции значений") };

			constexpr TypeDescriptorForEnumerateBase() :
				mybase_t(TypeId_, TypeTerm_, TypeDescriptionInit_)
			{ }
		};
		//
		template<class ExEnumeratorForContainerT>
		struct ExEntityForEnumeratorFactory
		{
			struct TypeDescriptorForEnumerate : public TypeDescriptorForEnumerateBase
			{			
				ESB_DECLARE_NOCOPYMOVE_DTORDEFAULT(TypeDescriptorForEnumerate);
				//
				using me_t = ExEntityForEnumeratorFactory<ExEnumeratorForContainerT>::TypeDescriptorForEnumerate;
				using mybase_t = TypeDescriptorForEnumerateBase;
				using contained_t = ExEnumeratorForContainerT;
				using ext_value_t = ExEntityForEnumerator< contained_t >;
				using allocator_t = ExtAllocator<alignof(ext_value_t)>;
				using factory_t = ExtValueFactory<me_t>;
				// для доступа из factory_t выносим в паблик
				using mybase_t::NewValueBaseInplace;

				static bool			IEnumValues_Reset(ExValueBase& obj_base_) {
					ext_value_t& obj = static_cast<ext_value_t&>(obj_base_);
					contained_t& obj_contained = obj.ContainedData_;
					//TODO
					return false;
				}
				static IValuePtr	IEnumValues_GetValueAndMoveNext(ExValueBase& obj_base_) {
					ext_value_t& obj = static_cast<ext_value_t&>(obj_base_);
					contained_t& obj_contained = obj.ContainedData_;
					return obj_contained.GetValueAndMoveNext();
				}
				static bool			IEnumValues_Skip(ExValueBase& obj_base_, size_t c_elem_to_skip_) {
					ext_value_t& obj = static_cast<ext_value_t&>(obj_base_);
					contained_t& obj_contained = obj.ContainedData_;
					//TODO
					return false;
				}
				static IValuePtr	IEnumValues_Clone(ExValueBase& obj_base_) {
					ext_value_t& obj = static_cast<ext_value_t&>(obj_base_);
					contained_t& obj_contained = obj.ContainedData_;
					//TODO
					return { nullptr };
				}

				template<typename ContainerT>
	#if defined(ESB_USE_OWNING_ENUMERATOR) && ESB_USE_OWNING_ENUMERATOR
				ext_value_t& CreateEnumerator(IEntityPtr&& entity_handler_, const ContainerT& container_) const {
					return factory_t::InstanceCreate(*this, std::move(entity_handler_), container_);
				}
	#else
				ext_value_t& CreateEnumerator(const ContainerT& container_) const {
					return factory_t::InstanceCreate(*this, container_);
				}
	#endif
				constexpr TypeDescriptorForEnumerate() : mybase_t{}
				{
					//не создаваем средствами 1С
					mybase_t::InstanceDeleter_ = factory_t::InstanceDelete;
					//
					TypeInfoFor_IEnumValues::PfnClone_ = IEnumValues_Clone;
					TypeInfoFor_IEnumValues::PfnGetValueAndMoveNext_ = IEnumValues_GetValueAndMoveNext;
					TypeInfoFor_IEnumValues::PfnReset_ = IEnumValues_Reset;
					TypeInfoFor_IEnumValues::PfnSkip_ = IEnumValues_Skip;
				}
			};
			//
			static inline const TypeDescriptorForEnumerate descriptor_instance_{};
			//
			template<typename ContainerT>
	#if defined(ESB_USE_OWNING_ENUMERATOR) && ESB_USE_OWNING_ENUMERATOR
			static TypeDescriptorForEnumerate::ext_value_t& CreateEnumerator(IEntityPtr&& entity_handler_, const ContainerT& container_) {
				return descriptor_instance_.CreateEnumerator(std::move(entity_handler_), container_);
			}
	#else
			static TypeDescriptorForEnumerate::ext_value_t& CreateEnumerator(const ContainerT& container_) {
				return descriptor_instance_.CreateEnumerator(container_);
			}
	#endif
		};
		//
#if defined(ESB_USE_OWNING_ENUMERATOR) && ESB_USE_OWNING_ENUMERATOR
		template<typename ContainerT>
		IEntityPtr ExEnumeratorForContainer_Create(esb::_internal::IEntityPtr&& enumeratable_entity_handler_, const ContainerT& container_)
		{
			using enumerator_t				= ExEnumeratorForContainer<ContainerT>;
			using enumerator_entity_t		= ExEntityForEnumerator<enumerator_t>;
			using enumer_entity_factory_t	= ExEntityForEnumeratorFactory<enumerator_t>;
			enumerator_entity_t& enumerator = enumer_entity_factory_t::CreateEnumerator(std::move(enumeratable_entity_handler_), container_);
			return IEntityPtr{ static_cast<IUnknown&>(enumerator) };
		}
#else
		template<typename ContainerT>
		IEntityPtr ExEnumeratorForContainer_Create(const ContainerT& container_)
		{
			using enumerator_t				= ExEnumeratorForContainer<ContainerT>;
			using enumerator_entity_t		= ExEntityForEnumerator<enumerator_t>;
			using enumer_entity_factory_t	= ExEntityForEnumeratorFactory<enumerator_t>;
			enumerator_entity_t& enumerator = enumer_entity_factory_t::CreateEnumerator(container_);
			return IEntityPtr{ static_cast<IUnknown&>(enumerator) };
		}
#endif


		template<typename InterfaceT, typename ContainedT>
		struct TypeDescriptorImplement_IEnu
		{
			using meta_interface_t	= InterfaceT;
			using contained_t		= ContainedT;
			using ext_value_t		= ex_entity_value_for_interface_t<meta_interface_t>;
			//
			static IEntityPtr IEnumeratable_GetEnumerator(ExValueBase& enumaratable_value_base_) {
				if constexpr (is_contained_has_GetEnumerator<contained_t>) {
					ext_value_t&	enumaratable_value = static_cast<ext_value_t&>(enumaratable_value_base_);
					contained_t&	enumaratable = enumaratable_value.ContainedData_;
	#if defined(ESB_USE_OWNING_ENUMERATOR) && ESB_USE_OWNING_ENUMERATOR
					return			enumaratable.IEnumeratable_GetEnumerator(query_entity_handler(enumaratable_value));
	#else
					return			enumaratable.IEnumeratable_GetEnumerator();
	#endif
				}
				else {
					static_assert(is_contained_iterable<contained_t>, "ContainedData_ not iterable container.");
					using contained_container_t = contained_data_accessor_const_t<contained_t>;	//std::vector<bool>
					ext_value_t&	enumaratable_value = static_cast<ext_value_t&>(enumaratable_value_base_);
					contained_t&	enumaratable = enumaratable_value.ContainedData_;
	#if defined(ESB_USE_OWNING_ENUMERATOR) && ESB_USE_OWNING_ENUMERATOR
					return ExEnumeratorForContainer_Create<contained_container_t>(query_entity_handler(enumaratable_value), enumaratable.ESB_CONTAINED_DATA_ACCESSOR_NAME());
	#else
					return ExEnumeratorForContainer_Create<contained_container_t>(enumaratable.ESB_CONTAINED_DATA_ACCESSOR_NAME());
	#endif
				}
			}
			//		
			TYPEDESCRIPTOR_CONSTRUCT_CONSTEXPR
			static void Construct(TypeInfoFor_IEnumeratable& it)
			{
				it.PfnGetEnumerator_ = IEnumeratable_GetEnumerator;
			}
		};


		template<typename MetaInterfaceT, typename ContainedObjT, typename TypeInfosTupleT>
		struct TypeDescriptorImplementation;
		//
		template<typename MetaInterfaceT, typename ContainedObjT, typename... TypeInfosTupleTs>
		struct TypeDescriptorImplementation<MetaInterfaceT, ContainedObjT, std::tuple<TypeInfosTupleTs...>>
			: public TypeInfoFor< std::conditional_t<std::is_same_v<ContainedObjT, void>, TypeInfoFor_IValueSingleton, TypeInfoFor_IValueInstance>, TypeInfosTupleTs... >
		{
			using me_t = TypeDescriptorImplementation;
			using mybase_t = TypeInfoFor< std::conditional_t<std::is_same_v<ContainedObjT, void>, TypeInfoFor_IValueSingleton, TypeInfoFor_IValueInstance>, TypeInfosTupleTs... >;
			//
			using meta_interface_t = MetaInterfaceT;
			using contained_t = ContainedObjT;
			using ext_value_t = ex_entity_value_for_interface_t<meta_interface_t>;
			//
			using allocator_t = ExtAllocator<alignof(ext_value_t)>;
			using factory_t = ExtValueFactory<me_t>;
			using converter_t = ExtValueConverter<ext_value_t>;
			using esb_value_t = ValueForExEntity<meta_interface_t, contained_t>;
			using disp_interface_t = dispinterface_object_t< meta_interface_t >;
			//
			using mybase_t::NewValueBaseInplace;

			// мы единственный источник создания ext_value_t - и для TypeDef (из спп) и для ExtInvoker4CtorFromTypePack (из 1С)
			template<typename... ArgsT>
			static ext_value_t& CreateValueInstance(const me_t& td_, ArgsT&&... args_) {
				return factory_t::InstanceCreate(td_, std::forward<ArgsT>(args_) ...);
			}
			//
			static contained_t& GetContained(esb_value_t& value_) { return static_cast<ext_value_t&>(*get_interface(value_)).ContainedData_; }
			static const contained_t& GetContained(const esb_value_t& value_) { return static_cast<const ext_value_t&>(*get_interface(value_)).ContainedData_; }
			static contained_t&& GetContained(esb_value_t&& value_) { return std::move(static_cast<ext_value_t&&>(*get_interface(value_)).ContainedData_); }
			//
			TYPEDESCRIPTOR_CONSTRUCT_CONSTEXPR
				TypeDescriptorImplementation()
				: mybase_t(MetaInterfaceT::TypeId_, MetaInterfaceT::TypeTerm_, MetaInterfaceT::TypeDescriptionInit_)
			{	//instance
				mybase_t::InstanceCreatorFind_ = dispinterface_impl_ctor_find<disp_interface_t>();
				mybase_t::InstanceDeleter_ = factory_t::InstanceDelete;
				//conveter
				mybase_t::ToBoolean_ = converter_t::PfnValueToBoolean();
				mybase_t::ToNumeric_ = converter_t::PfnValueToNumeric();
				mybase_t::ToString_ = converter_t::PfnValueToString();
				mybase_t::ToDateTime_ = converter_t::PfnValueToDate();
				//
				if constexpr (is_interface_describe_object<meta_interface_t>) {
					TypeDescriptorImplement_IObj<meta_interface_t, contained_t>::Construct(*this);
				}
				if constexpr (is_interface_describe_indexable_rw<meta_interface_t>) {
					TypeDescriptorImplement_IxRW<meta_interface_t, contained_t>::Construct(*this);
				}
				if constexpr (is_interface_describe_indexable_ro<meta_interface_t>) {
					TypeDescriptorImplement_IxRO<meta_interface_t, contained_t>::Construct(*this);
				}
				if constexpr (is_interface_describe_iterable<meta_interface_t>) {
					TypeDescriptorImplement_IEnu<meta_interface_t, contained_t>::Construct(*this);
				}
			}
		};


		//TODO	дескриптор для описателя интерфейса, который НЕ содержт данных (ContainedT := void)
		//		НЕДОДЕЛАНО.
		//		У такого объекта тоже могут быть все интерфейсы и конвертация-в-...
		//		Пример - собственно AddinObject со статическим объектным интерфейсом
		//		или интерфейс для enum class и т.п.
		//		дескриптор должен полагаться только на методы описателя интерфейса, а не contained_t
		template<typename MetaInterfaceT, typename... TypeInfosTupleTs>
		struct TypeDescriptorImplementation<MetaInterfaceT, void, std::tuple<TypeInfosTupleTs...>>
			: public TypeInfoFor< TypeInfoFor_IValueSingleton, TypeInfosTupleTs... >
		{
			using me_t = TypeDescriptorImplementation;
			using mybase_t = TypeInfoFor< TypeInfoFor_IValueSingleton, TypeInfosTupleTs... >;
			//
			using meta_interface_t = MetaInterfaceT;
			using contained_t = void;
			using ext_value_t = ex_entity_value_for_interface_t<meta_interface_t>;
			//
			using allocator_t = ExtAllocator<alignof(ext_value_t)>;
			using factory_t = ExtValueFactory<me_t>;
			//using converter_t = ExtValueConverter<ext_value_t>;
			using esb_value_t = ValueForExEntity<meta_interface_t, contained_t>;
			using disp_interface_t = dispinterface_object_t< meta_interface_t >;
			//
			using mybase_t::NewValueBaseInplace;

			// мы единственный источник создания ext_value_t - и для TypeDef (из спп) и для ExtInvoker4CtorFromTypePack (из 1С)
			template<typename... ArgsT>
			static ext_value_t& CreateValueInstance(const me_t& td_, ArgsT&&... args_) {
				return factory_t::InstanceCreate(td_, std::forward<ArgsT>(args_) ...);
			}
			//
			TYPEDESCRIPTOR_CONSTRUCT_CONSTEXPR
				TypeDescriptorImplementation()
				: mybase_t(MetaInterfaceT::TypeId_, MetaInterfaceT::TypeTerm_, MetaInterfaceT::TypeDescriptionInit_)
			{	//instance, даже для void возможно создание - всегда одного экземпляра. уничтожение не предусмотрено.
				mybase_t::InstanceCreatorFind_ = dispinterface_impl_ctor_find<disp_interface_t>();
				//conveter. TODO коенвертирование единственного экземпляра возможно - надо сделать!
				//mybase_t::ToBoolean_ = converter_t::PfnValueToBoolean();
				//mybase_t::ToNumeric_ = converter_t::PfnValueToNumeric();
				//mybase_t::ToString_ = converter_t::PfnValueToString();
				//mybase_t::ToDateTime_ = converter_t::PfnValueToDate();
				//
				if constexpr (is_interface_describe_object<meta_interface_t>) {
					TypeDescriptorImplement_IObj<meta_interface_t, contained_t>::Construct(*this);
				}
				if constexpr (is_interface_describe_indexable_rw<meta_interface_t>) {
					TypeDescriptorImplement_IxRW<meta_interface_t, contained_t>::Construct(*this);
				}
				if constexpr (is_interface_describe_indexable_ro<meta_interface_t>) {
					TypeDescriptorImplement_IxRO<meta_interface_t, contained_t>::Construct(*this);
				}
				if constexpr (is_interface_describe_iterable<meta_interface_t>) {
					TypeDescriptorImplement_IEnu<meta_interface_t, contained_t>::Construct(*this);
				}
			}
		};
		//
		template<typename InterfaceT>
		using TypeDescriptorForInterface = TypeDescriptorImplementation<InterfaceT, interface_contained_t<InterfaceT>, TypeInfoTupleForInterface<InterfaceT> >;
		//
		template<typename InterfaceSpecT>
		using TypeDescriptor = TypeDescriptorForInterface<interface_info_t<InterfaceSpecT>>;


		template<class InterfaceT, typename ContainedT>
		class ValueForExEntity : public esb::Value
		{
			using me_t = ValueForExEntity<InterfaceT, ContainedT>;
		public:
			using contained_t		= ContainedT;
			using descriptor_t		= interface_descriptor_t<InterfaceT>;
			using typedef_t			= esb::_internal::ExtTypeDef<descriptor_t>;
			// для красоты семантики: PseudoArrayOfInt::ContainedDataType& vector_of_int = array_.GetContained().Data();
			using ContainedType		= contained_t;
			using ContainedDataType = contained_data_accessor_t<contained_t>;
		private:
			ESB_CLASS_IMPLEMENT_MAKE_VAL(ValueForExEntity)
			ESB_CLASS_IMPLEMENT_MAKE_OPT(ValueForExEntity)
			static inline typedef_t						TypeObj_{};
			static inline const RuntimeRegistratorInit	on_init_{ MemberMethInvoker<&TypeObj_, &typedef_t::init>::invoke };
		public:
			static inline const TypeDef& Type_{ TypeObj_ };
		public:
			template<typename... ArgsT>
			static me_t Create(ArgsT&&... args_) {
				return make<me_t>(IValuePtr{ TypeObj_.CreateValueInstance(std::forward<ArgsT>(args_) ...) });
			}
		public:
			contained_t& GetContained()&					{ return descriptor_t::GetContained(*this); }
			const contained_t& GetContained() const&		{ return descriptor_t::GetContained(*this); }
			contained_t&& GetContained()&&					{ return descriptor_t::GetContained(*this); }

			explicit operator contained_t& ()&				{ return descriptor_t::GetContained(*this); }
			explicit operator const contained_t& () const&	{ return descriptor_t::GetContained(*this); }
			explicit operator contained_t && ()&&			{ return descriptor_t::GetContained(*this); }
		};


		template<class InterfaceT>
		class ValueForExEntity<InterfaceT, void> : public esb::Value
		{
			using me_t = ValueForExEntity<InterfaceT, void>;
		public:
			using contained_t = void;
			using descriptor_t = interface_descriptor_t<InterfaceT>;
			using typedef_t = esb::_internal::ExtTypeDef<descriptor_t>;
		private:
			ESB_CLASS_IMPLEMENT_MAKE_VAL(ValueForExEntity)
			ESB_CLASS_IMPLEMENT_MAKE_OPT(ValueForExEntity)
			static inline typedef_t						TypeObj_{};
			static inline const RuntimeRegistratorInit	on_init_{ MemberMethInvoker<&TypeObj_, &typedef_t::init>::invoke };
		public:
			static inline const TypeDef& Type_{ TypeObj_ };
		public:
			// у войд-объекта не может быть аргументов конструктора
			static me_t Create() {
				return make<me_t>(IValuePtr{ TypeObj_.CreateValueInstance() });
			}
		};

		// конец Дескрипторы
		/////////////////////////////////////////////////////////////////////////////////////////////




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
			STDMETHOD_(ULONG, AddRef)() throw() override final { return 1; }
			STDMETHOD_(ULONG, Release)() throw() override final { return 1; }
		public:
			virtual HRESULT STDMETHODCALLTYPE CreateInstance(IUnknown* pUnkOuter, REFIID riid, void** ppvObject) override final {
				if (ppvObject == nullptr) return E_POINTER;
				if (pUnkOuter != nullptr) return CLASS_E_NOAGGREGATION;
				IType& t = const_cast<IType&>(static_cast<const IType&>(ExtTypeDefT::GetTypeDescriptor()));
				return t.QueryInterface(riid, ppvObject);
			}
			virtual HRESULT STDMETHODCALLTYPE LockServer([[maybe_unused]] BOOL fLock) override final { return 0; }
		public:
			static ExtTypeFactoryImplementation<ExtTypeDefT> Instance_;		//not const due to IClassFactory api
		public:
			static bool RegisterType()	{ return esb::_internal::ExTypeRegister(ExtTypeDefT::GetTypeDescriptor(), Instance_); }
			static bool RevokeType()	{ return esb::_internal::ExTypeRevoke(ExtTypeDefT::GetTypeDescriptor()); }
		};
		//
		ESB_WARNING_RESTORE()	//ESB_WARNING_SUPRESS_NO_VIRTUAL_DTOR_ANY



#define ESB_INTERFACEDEF_VALUE_T(CLASS_T_)				ESB_INTERFACEDEF_DESCRIPTOR_T(CLASS_T_)::esb_value_t
#define ESB_INTERFACEDEF_FACTORY_T(CLASS_T_)			esb::_internal::ExtTypeFactoryImplementation< ESB_INTERFACEDEF_VALUE_T(CLASS_T_) :: typedef_t >
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
			// наши инстансы on_runtime_... static inline, но компилятор не хочет их делать если как-то не его не попросить. 
			// просим сделать фейковую инстанс этого класса
			// (оптимизатор ее все равно вырезает, а static inline on_runtime_... остаются.
			static const ExtEntityRegistrator Instance_;
		};

#define ESB_VALUEDEF_WITH_FACTORY_AND_REG(ESB_NAME_, CLASS_T_, GUID_STR_, NAME_PAIR_, TEXT_)		\
	ESB_VALUEDEF_WITH_FACTORY(ESB_NAME_, CLASS_T_, GUID_STR_, NAME_PAIR_, TEXT_);					\
	inline const esb::_internal::ExtEntityRegistrator<ESB_NAME_>	esb::_internal::ExtEntityRegistrator<ESB_NAME_>::Instance_{}


#define ESB_INTERFACEDEF_DONE_WITH_VALUE_AND_FACTORY(CLASS_T_, ESB_NAME_)		\
	ESB_INTERFACEDEF_DONE(CLASS_T_);											\
	using ESB_NAME_ =  ESB_INTERFACEDEF_VALUE_T(CLASS_T_);						\
	ESB_INTERFACEDEF_FACTORY_INSTANCE(CLASS_T_)

#define ESB_INTERFACEDEF_DONE_WITH_VALUE_AND_FACTORY_REG(CLASS_T_, ESB_NAME_)	\
	ESB_INTERFACEDEF_DONE_WITH_VALUE_AND_FACTORY(CLASS_T_, ESB_NAME_);			\
	inline const esb::_internal::ExtEntityRegistrator<ESB_NAME_>	esb::_internal::ExtEntityRegistrator<ESB_NAME_>::Instance_{}


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
			ExtEntityRegistrator() :
				on_runtime_init_{ RuntimeRegistratorInit::last(), TypeDefDelegatToMeth::ExtRegisterType },
				on_runtime_term_{ VoidifyInvoker<TypeDefDelegatToMeth::ExtRevokeType>::invoke }
			{}
			static const ExtEntityRegistrator Instance_;
		};
#define ESB_REGISTER_AUTO_DELEGAT_TO_METH()	\
	inline const esb::_internal::ExtEntityRegistrator<DelegatToMeth> esb::_internal::ExtEntityRegistrator<DelegatToMeth>::Instance_{}

	} //_internal
} //namespace esb-ext



#endif	//ESB_EXT_H_

