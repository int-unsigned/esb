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
#include "esb_traits.h"
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


		//TODO	Весь "механизм" авто-определеления iterable/indexable/etc.. опирается либбо на присутствие в самом классе соответствующих методов
		//		либо на то, что у класса есть "магический" .ESB_CONTAINED_DATA().
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
		inline constexpr bool	is_interface_describe_indexable_rw = is_contained_has_SetValue< interface_contained_t<InterfaceT>> ||
			is_contained_indexable_w< interface_contained_t<InterfaceT>>;
		//
		template<typename InterfaceT>	//!is_interface_describe_indexable_rw - ВАЖНО, т.к. должен быть либо ..RW либо ..RO !
		inline constexpr bool	is_interface_describe_indexable_ro = (is_contained_has_GetValue<interface_contained_t<InterfaceT>> ||
			is_contained_indexable_r<interface_contained_t<InterfaceT>>
			) &&
			!is_interface_describe_indexable_rw<InterfaceT>;
		//
		template<typename InterfaceT>
		inline constexpr bool	is_interface_describe_iterable = is_contained_has_GetEnumerator<interface_contained_t<InterfaceT>> ||
			is_contained_iterable<interface_contained_t<InterfaceT>>;





		template<class ExtValueT>
		struct ExtValueConverter
		{
			using value_t = ExtValueT;
			using data_t = value_t::ContainedDataType;	// ext_value_contained_data_t<ExtValueT>;
			static const data_t& data(ExValueBase& v_)		{ return static_cast<const value_t&>(v_).ContainedData_; }
			static Boolean	ValueToBoolean(ExValueBase& v_) { return ExtConvertToBoolean(data(v_)); }
			static Numeric	ValueToNumeric(ExValueBase& v_) { return ExtConvertToNumeric(data(v_)); }
			static String	ValueToString(ExValueBase& v_)	{ return ExtConvertToString(data(v_)); }
			static DateTime ValueToDate(ExValueBase& v_)	{ return ExtConvertToDate(data(v_)); }

			static constexpr ExValueToBooleanFn*	PfnValueToBoolean() { if constexpr (is_wrapper_interface_may_ToBoolean<data_t>)	return ValueToBoolean;	else return nullptr; }
			static constexpr ExValueToNumericFn*	PfnValueToNumeric() { if constexpr (is_wrapper_interface_may_ToNumeric<data_t>)	return ValueToNumeric;	else return nullptr; }
			static constexpr ExValueToStringFn*		PfnValueToString()	{ if constexpr (is_wrapper_interface_may_ToString<data_t>)	return ValueToString;	else return nullptr; }
			static constexpr ExValueToDateTimeFn*	PfnValueToDate()	{ if constexpr (is_wrapper_interface_may_ToDate<data_t>)	return ValueToDate;		else return nullptr; }
		};

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
		DECLARE_TYPEINFO_TUPLE_FLT(TypeInfoFor_IObject, is_interface_describe_object);
		DECLARE_TYPEINFO_TUPLE_FLT(TypeInfoFor_IxCollectionRW, is_interface_describe_indexable_rw);
		DECLARE_TYPEINFO_TUPLE_FLT(TypeInfoFor_IxCollectionRO, is_interface_describe_indexable_ro);
		DECLARE_TYPEINFO_TUPLE_FLT(TypeInfoFor_IEnumeratable, is_interface_describe_iterable);
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

			static constexpr size_t						PropCount(ExValueBase&)								{ return dispinterface_impl_prop_count<disp_interface_t>(); }
			static constexpr int						PropFind(ExValueBase&, const string_view_t& name_)	{ return dispinterface_impl_prop_find<disp_interface_t>(name_); }
			static constexpr const DispInfoMembProp*	Prop(ExValueBase&, int prop_)						{ return dispinterface_impl_memb_prop<disp_interface_t>(prop_); }

			static constexpr size_t						MethCount(ExValueBase&)								{ return dispinterface_impl_meth_count<disp_interface_t>(); }
			static constexpr int						MethFind(ExValueBase&, const string_view_t& name_)	{ return dispinterface_impl_meth_find<disp_interface_t>(name_); }
			static constexpr const DispInfoMembMeth*	Meth(ExValueBase&, int meth_)						{ return dispinterface_impl_memb_meth<disp_interface_t>(meth_); }

			TYPEDESCRIPTOR_CONSTRUCT_CONSTEXPR
				static void Construct(TypeInfoFor_IObject& it)
			{
				it.ObjPropCount_	= PropCount;
				it.ObjPropFind_		= PropFind;
				it.ObjProp_			= Prop;
				//
				it.ObjMethCount_	= MethCount;
				it.ObjMethFind_		= MethFind;
				it.ObjMeth_			= Meth;
			}
		};


		//TODO	Унаследовать/совместить _IxRO/_IxRW. Копипасты много!
		template<typename InterfaceT, typename ContainedT>
		struct TypeDescriptorImplement_IxRW
		{
			using meta_interface_t = InterfaceT;
			using contained_t = ContainedT;
			using ext_value_t = ex_entity_value_for_interface_t<meta_interface_t>;

			static size_t		IxCollection_Size(const ExValueBase& obj_base_) {
				if constexpr (is_contained_has_Size<contained_t>) {
					const contained_t& container = static_cast<const ext_value_t&>(obj_base_).ContainedData_;
					return container.ESB_CONTAINED_IXCOLLECTION_SIZE();
				}
				else {
					static_assert(is_contained_indexable_w<contained_t>, "ContainedData_ not indexable container.");
					const contained_t& contained = static_cast<const ext_value_t&>(obj_base_).ContainedData_;
					return contained.ESB_CONTAINED_DATA().size();
				}
			}
			static IValuePtr	IxCollection_GetValue(const ExValueBase& obj_base_const_TODO_, size_t index_)
			{
				//TODO	Нужно перекомпилировать esbhlp! Мы вынуждены снимать конст с ExValueBase т.к. не можем создать объект-mutable-ссылку на элемент контейнера!
				ExValueBase& obj_base_ = const_cast<ExValueBase&>(obj_base_const_TODO_);
				//
				contained_t& container = static_cast<ext_value_t&>(obj_base_).ContainedData_;
				//
				if constexpr (is_contained_has_GetValue<contained_t>) {
					
					return container.ESB_CONTAINED_IXCOLLECTION_GETVALUE(index_);
				}
				else if constexpr (is_contained_has_GetValueExt<contained_t>) {
					
					return container.template ESB_CONTAINED_IXCOLLECTION_GETVALUE_EXT<ext_value_t>(obj_base_, index_);
				}
				else {
					static_assert(is_contained_indexable_w<contained_t>, "ContainedData_ not indexable container.");					
					if (index_ >= container.ESB_CONTAINED_DATA().size())
						ESL_THROW_ARRAY_INDEX_ERR();
					return convert_to_ret_for_IxCollection_GetValue<ext_value_t>(container.ESB_CONTAINED_DATA()[index_], obj_base_);
				}
			}
			static void			IxCollection_SetValue(ExValueBase& obj_base_, size_t index_, IValuePtr byval_value_) 
			{
				contained_t& contained = static_cast<ext_value_t&>(obj_base_).ContainedData_;
				//				
				if constexpr (is_contained_has_SetValue<contained_t>) {					
					contained_t& container = static_cast<ext_value_t&>(obj_base_).ContainedData_;
					return container.ESB_CONTAINED_IXCOLLECTION_SETVALUE(index_, std::move(byval_value_));
				}
				else {
					static_assert(is_contained_indexable_w<contained_t>, "ContainedData_ not indexable container.");
					contained_t& contained = static_cast<ext_value_t&>(obj_base_).ContainedData_;
					if (index_ >= contained.ESB_CONTAINED_DATA().size())
						ESL_THROW_ARRAY_INDEX_ERR();

					using contained_container_t			= contained_data_accessor_t<contained_t>;
					using contained_container_value_t	= contained_container_t::value_type;
					//contained.ESB_CONTAINED_DATA()[index_] = ex_get_value_from_interface<contained_container_value_t>(std::move(byval_value_));
					contained.ESB_CONTAINED_DATA()[index_] = convert_to_arg_for_IxCollection_SetValue<contained_container_value_t>(std::move(byval_value_));
				}
			}

			TYPEDESCRIPTOR_CONSTRUCT_CONSTEXPR
				static void Construct(TypeInfoFor_IxCollectionRW& it)
			{
				it.PfnGetSize_	= IxCollection_Size;
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
					return contained.ESB_CONTAINED_DATA().size();
				}
			}
			static IValuePtr	IxCollection_GetValue(const ExValueBase& obj_base_const_TODO_, size_t index_) 
			{
				//TODO	Нужно перекомпилировать esbhlp! Мы вынуждены снимать конст с ExValueBase т.к. не можем создать объект-mutable-ссылку на элемент контейнера!
				ExValueBase& obj_base_ = const_cast<ExValueBase&>(obj_base_const_TODO_);
				//
				contained_t& container = static_cast<ext_value_t&>(obj_base_).ContainedData_;
				//
				if constexpr (is_contained_has_GetValue<contained_t>) 
					return container.ESB_CONTAINED_IXCOLLECTION_GETVALUE(index_);
				else if constexpr (is_contained_has_GetValueExt<contained_t>)
					return container.template ESB_CONTAINED_IXCOLLECTION_GETVALUE_EXT<ext_value_t>(obj_base_, index_);
				else {
					static_assert(is_contained_indexable_r<contained_t>, "ContainedData_ not indexable container.");
					contained_t& container = static_cast<ext_value_t&>(obj_base_).ContainedData_;
					if (index_ >= container.ESB_CONTAINED_DATA().size())
						ESL_THROW_ARRAY_INDEX_ERR();
					return convert_to_ret_for_IxCollection_GetValue<ext_value_t>(container.ESB_CONTAINED_DATA()[index_], obj_base_);
				}
			}
			//
			TYPEDESCRIPTOR_CONSTRUCT_CONSTEXPR
				static void Construct(TypeInfoFor_IxCollectionRO& it)
			{
				it.PfnGetSize_	= IxCollection_Size;
				it.PfnGetValue_ = IxCollection_GetValue;
			}

		};


		// Енумератор для объектов, которые можно итерировать через const_iterator от begin(container_) до end(container_)
		//TODO	НЕДОДЕЛАНО.	Для Reset/Skip/Clone. 
		//		Для Reset нужно держать и сам контейнер. 
		//		Для Skip нужны соответствующие итераторы (через m_end перескакивать нельзя!) 
		//		Для Clone это вообще не здесь надо делать, а там где нас создают
		//		Возможно и Reset нужно там же делать
		// 
		//TODO	Проблема. Енумератор ничего не знает о враппере в котором находится контейнер
		//		Поэтому он "в слепую" преобразовывает значение (скажем int64_t) в объект понятный 1С
		//		полагаясь на некоторые общие настройки конвертирования (::convert_to_ivalue_for_enum)
		//
		template<typename ContainerT>
		struct ExEnumeratorForContainerDefault
		{
			using container_t		= ContainerT;
			using container_value_t = container_t::value_type;
			container_t::const_iterator m_1st;
			container_t::const_iterator m_end;
#if defined(ESB_USE_OWNING_ENUMERATOR) && ESB_USE_OWNING_ENUMERATOR
			IEntityPtr					m_container_handler;
			ExEnumeratorForContainerDefault(IEntityPtr&& entity_handler_, const container_t& container_)
				: m_container_handler{ std::move(entity_handler_) }, m_1st{ begin(container_) }, m_end{ end(container_) }
			{ }
#else
			ExEnumeratorForContainerDefault(const container_t& container_)
				: m_1st{ begin(container_) }, m_end{ end(container_) }
			{ }
#endif
			IValuePtr	GetValueAndMoveNext() {
				if (m_1st == m_end)
					return { nullptr };

				//TODO	Нужно подумать над целесообразностью возвращать всегда только конст-значение из контейнера
				//		Может можно и значение-ссылку если контейнер позволяет и объект-значение-ссылка есть?
				const container_value_t& value = *m_1st;
				++m_1st;
				return esb::_internal::convert_to_ret_for_Enumerator_GetValue(value, m_container_handler);
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





		template<typename ContainerWrapperTy>
		struct ExEnumeratorForContainerWrapper
		{
			using container_wrapper_t	= ContainerWrapperTy;
			//NOTE	Тип контейнера мы определяем из возвращаемого значения метода .Data()
			using container_t			= contained_data_accessor_t<container_wrapper_t>;				// (std::vector<bool>)
			using container_iterator_t	= std::conditional_t<std::is_const_v<container_t>, typename container_t::const_iterator, typename container_t::iterator>;
			
			container_wrapper_t&		m_container_wrapper_;
			container_iterator_t		m_1st;
			container_iterator_t		m_end;
#if defined(ESB_USE_OWNING_ENUMERATOR) && ESB_USE_OWNING_ENUMERATOR
			IEntityPtr					m_container_handler;
			ExEnumeratorForContainerWrapper(IEntityPtr&& entity_handler_, container_wrapper_t& container_wrapper_)
				: m_container_handler{ std::move(entity_handler_) }, m_container_wrapper_{ container_wrapper_ }, 
					m_1st{ begin(container_wrapper_.ESB_CONTAINED_DATA()) }, m_end{ end(container_wrapper_.ESB_CONTAINED_DATA()) }
			{ }
#else
			ExEnumeratorForContainerWrapper(const container_wrapper_t& container_wrapper_)
				: m_1st{ begin(container_) }, m_end{ end(container_) }
			{ }
#endif
			IValuePtr	GetValueAndMoveNext() 
			{
				//NOTE	Мы здесь ничего не инкрементируем и ничего не проверяем - все это задача custom имплементации метода контейнера
				return m_container_wrapper_.ESB_CONTAINED_GETVALUEANDMOVENEXT(m_1st, m_end, m_container_handler);
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
				using me_t			= ExEntityForEnumeratorFactory<ExEnumeratorForContainerT>::TypeDescriptorForEnumerate;
				using mybase_t		= TypeDescriptorForEnumerateBase;
				using contained_t	= ExEnumeratorForContainerT;
				using ext_value_t	= ExEntityForEnumerator< contained_t >;
				using allocator_t	= ExtAllocator<alignof(ext_value_t)>;
				using factory_t		= ExtValueFactory<me_t>;
				// для доступа из factory_t выносим в паблик
				using mybase_t::NewValueBaseInplace;

				static bool			IEnumValues_Reset(ExValueBase& obj_base_) {
					ext_value_t& obj			= static_cast<ext_value_t&>(obj_base_);
					contained_t& obj_contained	= obj.ContainedData_;
					//TODO
					return false;
				}
				static IValuePtr	IEnumValues_GetValueAndMoveNext(ExValueBase& obj_base_) {
					ext_value_t& obj			= static_cast<ext_value_t&>(obj_base_);
					contained_t& obj_contained	= obj.ContainedData_;
					return obj_contained.GetValueAndMoveNext();
				}
				static bool			IEnumValues_Skip(ExValueBase& obj_base_, size_t c_elem_to_skip_) {
					ext_value_t& obj			= static_cast<ext_value_t&>(obj_base_);
					contained_t& obj_contained	= obj.ContainedData_;
					//TODO
					return false;
				}
				static IValuePtr	IEnumValues_Clone(ExValueBase& obj_base_) {
					ext_value_t& obj			= static_cast<ext_value_t&>(obj_base_);
					contained_t& obj_contained	= obj.ContainedData_;
					//TODO
					return { nullptr };
				}

				template<typename ContainerT>
#if defined(ESB_USE_OWNING_ENUMERATOR) && ESB_USE_OWNING_ENUMERATOR
				ext_value_t& CreateEnumerator(IEntityPtr&& entity_handler_, ContainerT& container_) const {
					return factory_t::InstanceCreate(*this, std::move(entity_handler_), container_);
				}
#else
				ext_value_t& CreateEnumerator(ContainerT& container_) const {
					return factory_t::InstanceCreate(*this, container_);
				}
#endif
				constexpr TypeDescriptorForEnumerate() : mybase_t{}
				{
					//не создаваем средствами 1С
					mybase_t::InstanceDeleter_ = factory_t::InstanceDelete;
					//
					TypeInfoFor_IEnumValues::PfnClone_					= IEnumValues_Clone;
					TypeInfoFor_IEnumValues::PfnGetValueAndMoveNext_	= IEnumValues_GetValueAndMoveNext;
					TypeInfoFor_IEnumValues::PfnReset_					= IEnumValues_Reset;
					TypeInfoFor_IEnumValues::PfnSkip_					= IEnumValues_Skip;
				}
			};
			//
			static inline const TypeDescriptorForEnumerate descriptor_instance_{};
			//
			template<typename ContainerT>
#if defined(ESB_USE_OWNING_ENUMERATOR) && ESB_USE_OWNING_ENUMERATOR
			static TypeDescriptorForEnumerate::ext_value_t& CreateEnumerator(IEntityPtr&& entity_handler_, ContainerT& container_) {
				return descriptor_instance_.CreateEnumerator(std::move(entity_handler_), container_);
			}

#else
			static TypeDescriptorForEnumerate::ext_value_t& CreateEnumerator(ContainerT& container_) {
				return descriptor_instance_.CreateEnumerator(container_);
			}
#endif
		};
		//

#if defined(ESB_USE_OWNING_ENUMERATOR) && ESB_USE_OWNING_ENUMERATOR
		template<template<typename ContainerTy> typename EnumeratorForContainerTt, typename ContainerTy>
		IEntityPtr ExEnumeratorForContainer_Create(esb::_internal::IEntityPtr&& enumeratable_entity_handler_, ContainerTy& container_)
		{
			using enumerator_t				= EnumeratorForContainerTt<ContainerTy>;
			using enumerator_entity_t		= ExEntityForEnumerator<enumerator_t>;
			using enumer_entity_factory_t	= ExEntityForEnumeratorFactory<enumerator_t>;
			enumerator_entity_t& enumerator = enumer_entity_factory_t::CreateEnumerator(std::move(enumeratable_entity_handler_), container_);
			return IEntityPtr{ static_cast<IUnknown&>(enumerator) };
		}

#else
		template<typename ContainerT>
		IEntityPtr ExEnumeratorForContainer_Create(ContainerTy& container_)
		{
			using enumerator_t				= EnumeratorForContainerTt<ContainerTy>;
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
					ext_value_t& enumaratable_value = static_cast<ext_value_t&>(enumaratable_value_base_);
					contained_t& enumaratable		= enumaratable_value.ContainedData_;
#if defined(ESB_USE_OWNING_ENUMERATOR) && ESB_USE_OWNING_ENUMERATOR
					return			enumaratable.ESB_CONTAINED_GETENUMERATOR( query_entity_handler(enumaratable_value) );
#else
					return			enumaratable.IEnumeratable_GetEnumerator();
#endif
				}
				else {
					static_assert(is_contained_iterable<contained_t>, "ContainedData_ not iterable container.");					
					using contained_container_t		= contained_data_accessor_const_t<contained_t>;				// (std::vector<bool>)
					ext_value_t& enumaratable_value = static_cast<ext_value_t&>(enumaratable_value_base_);		// value которое содержит wrapper on (std::vector<bool>)								
					contained_t& enumaratable		= enumaratable_value.ContainedData_;						// wrapper on (std::vector<bool>)
#if defined(ESB_USE_OWNING_ENUMERATOR) && ESB_USE_OWNING_ENUMERATOR
					return ExEnumeratorForContainer_Create<ExEnumeratorForContainerDefault, contained_container_t>(query_entity_handler(enumaratable_value), enumaratable.ESB_CONTAINED_DATA());
#else
					return ExEnumeratorForContainer_Create<ExEnumeratorForContainerDefault, contained_container_t>(enumaratable.ESB_CONTAINED_DATA());
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
				//converter
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
			using ESB_CONTAINEDTYPE = contained_t;
			using ContainedDataType = contained_data_accessor_t<contained_t>;
		private:
			ESB_CLASS_IMPLEMENT_MAKE_VAL(ValueForExEntity)
				ESB_CLASS_IMPLEMENT_MAKE_OPT(ValueForExEntity)
				static inline typedef_t						TypeObj_{};
			static inline const RuntimeRegistratorInit	on_init_{ MemberMethInvoker<&TypeObj_, &typedef_t::init>::invoke };
		public:
			static inline const TypeDef& Type_{ TypeObj_ };
		public:
			template<typename... ArgsTs>
			static me_t Create(ArgsTs&&... args_) {
				static_assert(std::is_constructible_v<contained_t, ArgsTs...>, "Contained object not constructable with this arguments.");
				return make<me_t>(IValuePtr{ TypeObj_.CreateValueInstance(std::forward<ArgsTs>(args_) ...) });
			}
		public:
			ESB_IMPLEMENT_ARGBYVAL_OPERATOR(me_t)
		public:
			contained_t& ESB_CONTAINED_GET()&					{ return descriptor_t::GetContained(*this); }
			const contained_t& ESB_CONTAINED_GET() const&		{ return descriptor_t::GetContained(*this); }
			contained_t&& ESB_CONTAINED_GET()&&					{ return descriptor_t::GetContained(std::move(*this)); }

			explicit operator contained_t& ()&					{ return descriptor_t::GetContained(*this); }
			explicit operator const contained_t& () const&		{ return descriptor_t::GetContained(*this); }
			explicit operator contained_t && ()&&				{ return descriptor_t::GetContained(*this); }
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

	} //namespace _internal 


	////TODO	А почему все это не в _internal ?
	//template<typename T>
	//inline constexpr bool is_esb_value_ext = requires
	//{
	//	requires is_esb_value_based<T>;
	//	//
	//	typename T::descriptor_t;
	//	typename T::descriptor_t::esb_value_t;
	//	requires std::is_same_v<typename T::descriptor_t::esb_value_t, T>;
	//};
	//template<typename T>
	//inline constexpr bool is_esb_value_ext_with_contained = requires(T x)
	//{
	//	requires is_esb_value_ext<T>;
	//	x.ESB_CONTAINED_GET();
	//};
	//template<typename T>
	//inline constexpr bool is_esb_value_ext_with_contained_data = requires(T x)
	//{
	//	requires is_esb_value_ext_with_contained<T>;
	//	x.ESB_CONTAINED_GET().ESB_CONTAINED_DATA();
	//};

	//template<typename T>
	//inline constexpr bool is_contain_refence_type = requires
	//{
	//	typename T::ContainedType::DataType;
	//	requires is_reference_type_v<typename T::ESB_CONTAINEDTYPE::ESB_CONTAINED_DATA_T>;
	//};

	// конец Дескрипторы
	/////////////////////////////////////////////////////////////////////////////////////////////



	namespace _internal 
	{

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


//... - CLASS_T_ (c++ класс или специализация шаблона на котором построен интерфейс для 1С)
#define ESB_INTERFACEDEF_VALUE_T(...)				ESB_INTERFACEDEF_DESCRIPTOR_T(__VA_ARGS__)::esb_value_t
#define ESB_INTERFACEDEF_FACTORY_T(...)				esb::_internal::ExtTypeFactoryImplementation< ESB_INTERFACEDEF_VALUE_T(__VA_ARGS__) :: typedef_t >
#define ESB_INTERFACEDEF_FACTORY_INSTANCE(...)		inline ESB_INTERFACEDEF_FACTORY_T(__VA_ARGS__) ESB_INTERFACEDEF_FACTORY_T(__VA_ARGS__)::Instance_{}


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

//ESB_ENTITY_VALUE_T_
#define ESB_INTERFACEDEF_REGISTRATOR_T(...)										\
    esb::_internal::ExtEntityRegistrator<__VA_ARGS__>

//ESB_ENTITY_VALUE_T_
#define ESB_INTERFACEDEF_REGISTRATOR_INSTANCE(...)								\
    inline const ESB_INTERFACEDEF_REGISTRATOR_T(__VA_ARGS__) ESB_INTERFACEDEF_REGISTRATOR_T(__VA_ARGS__)::Instance_{}


#define ESB_INTERFACEDEF_DONE_WITH_VALUE_AND_FACTORY(CLASS_T_, ESB_NAME_)		\
	ESB_INTERFACEDEF_DONE(CLASS_T_);											\
	using ESB_NAME_ =  ESB_INTERFACEDEF_VALUE_T(CLASS_T_);						\
	ESB_INTERFACEDEF_FACTORY_INSTANCE(CLASS_T_)

#define ESB_INTERFACEDEF_DONE_WITH_VALUE_AND_FACTORY_REG(CLASS_T_, ESB_NAME_)	\
	ESB_INTERFACEDEF_DONE_WITH_VALUE_AND_FACTORY(CLASS_T_, ESB_NAME_);			\
	ESB_INTERFACEDEF_REGISTRATOR_INSTANCE(ESB_NAME_)


//... - CLASS_T_ (c++ класс или специализация шаблона на котором построен интерфейс для 1С)
#define ESB_INTERFACEDEF_FACTORY_INSTANCE_AND_REG(...)							\
    ESB_INTERFACEDEF_FACTORY_INSTANCE(__VA_ARGS__);								\
    ESB_INTERFACEDEF_REGISTRATOR_INSTANCE(ESB_INTERFACEDEF_VALUE_T(__VA_ARGS__))


#define ESB_VALUEDEF(ESB_NAME_, CLASS_T_, GUID_STR_, NAME_PAIR_, TEXT_)			\
	ESB_INTERFACEDEF_VALUE(CLASS_T_, GUID_STR_, NAME_PAIR_, TEXT_);				\
	using ESB_NAME_ =  ESB_INTERFACEDEF_VALUE_T(CLASS_T_)

#define ESB_VALUEDEF_WITH_FACTORY(ESB_NAME_, CLASS_T_, GUID_STR_, NAME_PAIR_, TEXT_)		\
	ESB_VALUEDEF(ESB_NAME_, CLASS_T_, GUID_STR_, NAME_PAIR_, TEXT_);						\
	ESB_INTERFACEDEF_FACTORY_INSTANCE(CLASS_T_)

#define ESB_VALUEDEF_WITH_FACTORY_AND_REG(ESB_NAME_, CLASS_T_, GUID_STR_, NAME_PAIR_, TEXT_)		\
	ESB_VALUEDEF_WITH_FACTORY(ESB_NAME_, CLASS_T_, GUID_STR_, NAME_PAIR_, TEXT_);					\
	ESB_INTERFACEDEF_REGISTRATOR_INSTANCE(ESB_NAME_)


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


//...CLASS_WRAPPER_T_
#define ESB_VALUE_FOR_CONTAINED_T(...)		                                                    \
    esb::_internal::ValueForExEntity<ESB_INTERFACEDEF_T(__VA_ARGS__), __VA_ARGS__>

	template<typename ContainedTy>
	using ValueFor = esb::_internal::ValueForExEntity<ESB_INTERFACEDEF_T(ContainedTy), ContainedTy>;

	

#define ESB_INTERFACEDEF_DONE_WITH_VALUE(CLASS_T_, ESB_NAME_)		\
	ESB_INTERFACEDEF_DONE(CLASS_T_);								\
	using ESB_NAME_ =  ESB_INTERFACEDEF_VALUE_T(CLASS_T_)


// образует имя для intermediate-wrapper класса. 
// сейчас имя класа 'value-для-1с' (тот который унаследован от ValueForExEntity) плюс суффикс 'Object'. 
// возможно будет лучше суффикс 'Wrapper'
// возможно лучше в будущем от этого класса вообще избавиться т.к. класс 'value-для-1С' сам по себе является враппером, матрешка, TODO, подумать..
//TODO	Во многих местах, макросах и т.п. ожидается непосредственно имя уже с суффиксом.
//		НУЖНО везде рефакторить и менять чтобы это исподнее белье наружу не вылезало.
//		тогда может быть и можно будет попробовать избавиться от этого 'intermediate-wrapper класса'
#define ESB_CLASS_WRAPPER_NAME(CLASS_VALUE_)									\
	CLASS_VALUE_##Object

//NOTE	Если сюда непосредственно передавать WRAPPED_TYPE_ и делать проверку noexcept( noexcept(WRAPPED_TYPE_(...
//		то если тип с конст: нр. ClosedInterval const, то ошибка парсинга от МС: непредвиденный токен const
//		а если тот-же через псевдоним (одним токеном), то ош нет.
#define ESB_CLASS_WRAPPER_CTOR(CLASS_WRAPPER_, WRAPPED_TYPE_, WRAPPED_DATA_)	\
        template<typename... ArgsTs>                                            \
        CLASS_WRAPPER_(ArgsTs&&... args_) noexcept( noexcept(ESB_CONTAINED_DATA_T(std::forward<ArgsTs>(args_)...)) ) : WRAPPED_DATA_{ std::forward<ArgsTs>(args_)... } {}

#define ESB_CLASS_WRAPPER_HEADER(DATA_T_, CLASS_WRAPPER_)													\
	ESB_DECLARE_CONTAINED_DATA_RW(DATA_T_, MyData_)															\
	ESB_CLASS_WRAPPER_CTOR(CLASS_WRAPPER_, DATA_T_, MyData_)


#define ESB_CLASS_WRAPPER_OLD(DATA_T_, CLASS_VALUE_)													  \
    struct  ESB_CLASS_WRAPPER_NAME(CLASS_VALUE_);                                                     \
    using   CLASS_VALUE_ = ESB_VALUE_FOR_CONTAINED_T(ESB_CLASS_WRAPPER_NAME(CLASS_VALUE_));           \
    struct ESB_CLASS_WRAPPER_NAME(CLASS_VALUE_)                                                       \
    {                                                                                                 \
        ESB_CLASS_WRAPPER_HEADER(DATA_T_, ESB_CLASS_WRAPPER_NAME(CLASS_VALUE_))






	} //_internal
} //namespace esb-ext



#endif	//ESB_EXT_H_

