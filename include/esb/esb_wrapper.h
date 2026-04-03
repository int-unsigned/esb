/*
	"ESB" is designed to create external native components for the 1C(tm) program.
	Copyright © 2023 IntUnsigned	(v8classes@gmail.com)
	"ESB" is free software under LGPLv2.1 license with essential restriction about binary part (view esb-license.txt)
	"ESB" IS DISTRIBUTED "AS-IS" WITHOUT ANY, EVEN THE IMPLIED, WARRANTY. IN NO EVENT SHALL THE AUTHORS OR THE COPYRIGHT OWNER BE LIABLE FOR ANY CLAIM, 
	DAMAGES OR OTHER LIABILITY ARISING OUT OF THE USE OF THIS SOFTWARE.
	THE AUTHOR DOES NOT GIVE ANY GUARANTEES AND DOES NOT BEAR ANY RESPONSIBILITY REGARDING THE USE OF 1C(tm) COMPONENTS PRODUCED USING THE ESB LIBRARY IN THE 1C(tm) ENVIRONMENT.
*/


#pragma once
#ifndef ESB_WRAPPER_H
#define ESB_WRAPPER_H

#include "esb_base.h"
#include "esb_meta.h"
#include "esb_ext.h"

//TODO	 все в esb::_internal


namespace esb
{
	// Если оборачиваемый тип не содержит данных (нр. Енумератор для енума)
	template<typename T>
	struct ExClassWrapperBaseEmpty
	{ };

// "Магические" имена..
#define ESB_CONTAINED_DATA_NO_REF_T   PP_CAT(ESB_CONTAINED_DATA_T, NoRef)
#define ESB_CONTAINED_DATA_TO_MOV_T   PP_CAT(ESB_CONTAINED_DATA_T, ToMov)
#define ESB_CONTAINED_DATA_VALUE_T    ValueType

	template<typename T>
	struct ExClassWrapperBaseData
	{
		using ESB_CONTAINED_DATA_T			= T;
		using ESB_CONTAINED_DATA_NO_REF_T	= std::remove_reference_t<DataType>;
		using ESB_CONTAINED_DATA_TO_MOV_T	= std::conditional_t<std::is_const_v<DataTypeNoRef>, DataTypeNoRef&, DataTypeNoRef&&>;
		using ESB_CONTAINED_DATA_VALUE_T	= std::remove_cvref_t<DataType>;
		//NOTE  когда мы построены на обычной (не конст) ссылке, то из конст метода мы можем возвращать обычную ссылку, т.к. константность относится к нам, а не к нашей ссылке
		using DataTypeRef       = std::conditional_t<std::is_reference_v<DataType> && not std::is_const_v<DataTypeNoRef>, ValueType&, const ValueType&>;
    protected:
		DataType _; 
		// конструктор прикрытый, т.к. наши данным может требоваться "Owner" и это обслуживается наследниками
		//TODO	Не получается "прикрытый" конструктор - разнообразные ошибки construct_at
		//		Нужно обязательно конструктор публичный и именно вынести его через using в ExClassWrapperBaseNonOwner
		//		не получается переопределить вслепую... ошибки construct_at... не понимаю..
		//		разбираться надо...
	public:
		template<typename... ArgsTs>
            ExClassWrapperBaseData(ArgsTs&&... args_)
				noexcept( noexcept(DataType(std::forward<ArgsTs>(args_)...)) ) 
				: _{ std::forward<ArgsTs>(args_)... } 
			{} 
    public:
		DataTypeRef     Data() const&   { return _; }
		DataTypeNoRef&  Data()&         { return _; } 
		DataTypeToMov   Data()&&        { return std::forward<DataTypeToMov>(_); } 
	};


	template<typename T>
	struct ExClassWrapperBaseNonOwner : ExClassWrapperBaseData<T> //: ExOwnerHolderNone
	{ 
	public:
		using ExClassWrapperBaseData<T>::ExClassWrapperBaseData;

		//TODO	см.ExClassWrapperBaseData(ArgsTs&&... args_)
		// 
		//template<typename... ArgsTs>
		//ExClassWrapperBaseNonOwner(ArgsTs&&... args_)
		//	noexcept( noexcept(ExClassWrapperBaseData<T>(std::forward<ArgsTs>(args_)...)) )
		//	: ExClassWrapperBaseData<T>( std::forward<ArgsTs>(args_)... )
		//{ }

		//template<typename... ArgsTs>
		//ExClassWrapperBaseNonOwner(ArgsTs&&... args_)
		//	noexcept( noexcept(T(std::forward<ArgsTs>(args_)...)) )
		//	: ExClassWrapperBaseData<T>( std::forward<ArgsTs>(args_)... )
		//{ }
	};
    //
	template<typename T>
	struct ExClassWrapperBaseHasOwner : ExClassWrapperBaseData<T>, ExOwnerHolder
	{
		public: 
			template<typename... ArgsTs>
            ExClassWrapperBaseHasOwner(ESB_OWNER_HANDLER_T&& owner_ptr_, ArgsTs&&... args_)
			    noexcept( noexcept(ExClassWrapperBaseData<T>(std::forward<ArgsTs>(args_)...)) && noexcept(ExOwnerHolder(std::move(owner_ptr_))) )
			        : ExClassWrapperBaseData<T>(std::forward<ArgsTs>(args_)...), ExOwnerHolder(std::move(owner_ptr_))
			{} 
	};

// Возможно имена этих методов придется менять, чтобы с 1С-именами не конфликтовало
#define ESB_WRAPPER_STD_METH_GetReference			GetReference
#define ESB_WRAPPER_STD_METH_GetReference_Ru		"ПолучитьСсылку"
#define ESB_WRAPPER_STD_METH_GetConstReference		GetConstReference
#define ESB_WRAPPER_STD_METH_GetConstReference_Ru	"ПолучитьКонстСсылку"
//TODO	"Типовой" 1С-интерфейс для врапперов. 
//		Пока не до конца понят и не доделан...
// 		Делается "по-месту" руками там где просится..
//		Нужно бы в ExClassWrapperBase единым образом делать
//		..и решить окончательно - "Data" или "Value" (или и то и то, или в зависимости-от, или ...)
#define ESB_WRAPPER_STD_METH_GetData				GetData
#define ESB_WRAPPER_STD_METH_GetData_En				"GetData"
#define ESB_WRAPPER_STD_METH_GetData_Ru				"ПолучитьДанные"
#define ESB_WRAPPER_STD_METH_SetData				SetData
#define ESB_WRAPPER_STD_METH_SetData_En				"SetData"
#define ESB_WRAPPER_STD_METH_SetData_Ru				"УстановитьДанные"
#define ESB_WRAPPER_STD_PROP_Data					Data
#define ESB_WRAPPER_STD_PROP_Data_En				"Data"
#define ESB_WRAPPER_STD_PROP_Data_Ru				"Данные"
#define ESB_WRAPPER_STD_PROP_Value					Value
#define ESB_WRAPPER_STD_PROP_Value_En				"Value"
#define ESB_WRAPPER_STD_PROP_Value_Ru				"Значение"
#define ESB_WRAPPER_STD_METH_GetValue				GetValue
#define ESB_WRAPPER_STD_METH_SetValue				SetValue
#define ESB_WRAPPER_STD_METH_SetValue_En			"SetValue"
#define ESB_WRAPPER_STD_METH_SetValue_Ru			"УстановитьЗначение"


    template<typename T>
    struct ExClassWrapperBase : std::conditional_t<esb::is_type_reftype<T>, ExClassWrapperBaseHasOwner<T>, ExClassWrapperBaseNonOwner<T>>
    {
		using mybase_t = std::conditional_t<esb::is_type_reftype<T>, ExClassWrapperBaseHasOwner<T>, ExClassWrapperBaseNonOwner<T>>;
		using mybase_t::mybase_t;

        template<typename ValueReferenceTy>
        ValueReferenceTy ESB_WRAPPER_STD_METH_GetReference(ESB_OWNER_HANDLER_T owner_ptr_) {
            return ValueReferenceTy::Create(std::move(owner_ptr_), this->Data());
        }
		template<> std::false_type ESB_WRAPPER_STD_METH_GetReference<std::false_type>(ESB_OWNER_HANDLER_T owner_ptr_);

        template<typename ValueConstReferenceTy>
        ValueConstReferenceTy ESB_WRAPPER_STD_METH_GetConstReference(ESB_OWNER_HANDLER_T owner_ptr_) const {
            return ValueConstReferenceTy::Create(std::move(owner_ptr_), this->Data());
        }
		template<> std::false_type ESB_WRAPPER_STD_METH_GetConstReference<std::false_type>(ESB_OWNER_HANDLER_T owner_ptr_) const;
    };



	// is_interface_defined_for "разрушающая" состояние проверка. 
	// ее нельзя делать в процессе самого определения интерфейса и после нее определить этот интерфейс уже нельзя - он считается определенным, даже если определения не было.
	template<template<typename...> typename WrapperTemplate, template<typename...> typename ValueTemplate, typename CppContainedTy, typename... ExtraTemplateArgs>
	using template_interface_descriptor_value_if_present_t =
		std::conditional_t<esb::_internal::is_interface_defined_for< WrapperTemplate<CppContainedTy, ExtraTemplateArgs...> >, ValueTemplate<CppContainedTy, ExtraTemplateArgs...>, std::false_type>;


	template<template<typename...> typename WrapperTemplate, template<typename...> typename ValueTemplate, typename CppContainedTy, typename... ExtraTemplateArgs>
	struct template_interface_descriptor_cref_value
	{
		// для всех вариантов (кроме конст-реф, которому специализация ниже) пытаемся получить валюе-для-конст-реф, если он определен
		using type = template_interface_descriptor_value_if_present_t<WrapperTemplate, ValueTemplate, const std::remove_cvref_t<CppContainedTy>&, ExtraTemplateArgs...>;
	};
	//
	template<template<typename...> typename WrapperTemplate, template<typename...> typename ValueTemplate, typename CppContainedTy, typename... ExtraTemplateArgs>
	struct template_interface_descriptor_cref_value<WrapperTemplate, ValueTemplate, const CppContainedTy&, ExtraTemplateArgs...>
	{
		// если мы const-ref, то возвращаем свой валюе-тип
		using type = ValueTemplate<const CppContainedTy&, ExtraTemplateArgs...>;
	};


	template<template<typename...> typename WrapperTemplate, template<typename...> typename ValueTemplate, typename CppContainedTy, typename... ExtraTemplateArgs>
	struct template_interface_descriptor_vref_value
	{
		// мы валюе-тип (для других специализация ниже). тип валюе-ссылки мы имеем, только если он был определен ранее
		using type = template_interface_descriptor_value_if_present_t<WrapperTemplate, ValueTemplate, CppContainedTy&, ExtraTemplateArgs...>;
	};
	template<template<typename...> typename WrapperTemplate, template<typename...> typename ValueTemplate, typename CppContainedTy, typename... ExtraTemplateArgs>
	struct template_interface_descriptor_vref_value<WrapperTemplate, ValueTemplate, CppContainedTy&, ExtraTemplateArgs...>
	{
		// мы реф-тип. возвращаем свой валюе-тип
		using type = ValueTemplate<CppContainedTy&, ExtraTemplateArgs...>;
	};
	// если мы любой другой конст, то делать mutable ссылку мы не можем. мы - std::false_type
	template<template<typename...> typename WrapperTemplate, template<typename...> typename ValueTemplate, typename CppContainedTy, typename... ExtraTemplateArgs>
	struct template_interface_descriptor_vref_value<WrapperTemplate, ValueTemplate, const CppContainedTy&, ExtraTemplateArgs...> 
		: std::type_identity_t<std::false_type> {};
	template<template<typename...> typename WrapperTemplate, template<typename...> typename ValueTemplate, typename CppContainedTy, typename... ExtraTemplateArgs>
	struct template_interface_descriptor_vref_value<WrapperTemplate, ValueTemplate, const CppContainedTy, ExtraTemplateArgs...> 
		: std::type_identity_t<std::false_type> {};



    //DescriptorInfoTy - template_interface_descriptor_info_ex
    template<typename ObjectTy, typename DescriptorInfoTy>
    struct ex_class_wrapper_interface_base
    {
        using value_reference_t         = DescriptorInfoTy::my_value_reference_t;
        using value_const_reference_t   = DescriptorInfoTy::my_value_const_reference_t;
        //
        static ESB_META_INTERFACE_EVAL_MODE auto meth_cref()
        {
            if constexpr (std::is_same_v<value_const_reference_t, std::false_type>)
                return esb::_internal::make_meta_interface_meth();
            else
                return esb::_internal::make_meta_interface_meth
                (
					ESB_META_INTERFACE_MAKE_METH_TEMPLATE(ObjectTy, (value_const_reference_t), ESB_WRAPPER_STD_METH_GetConstReference,	ESB_WRAPPER_STD_METH_GetConstReference_Ru)
                );
        }
        static ESB_META_INTERFACE_EVAL_MODE auto meth_vref()
        {
            if constexpr (std::is_same_v<value_reference_t, std::false_type>)
                return esb::_internal::make_meta_interface_meth();
            else
                return esb::_internal::make_meta_interface_meth
                (
                    ESB_META_INTERFACE_MAKE_METH_TEMPLATE(ObjectTy, (value_reference_t),		ESB_WRAPPER_STD_METH_GetReference,		ESB_WRAPPER_STD_METH_GetReference_Ru)
                );
        }
    };


//NOTE	Стандартная методика создания врапперов имплементировання в этом хедере
//		предполагает создание классов-врапперов как специализацию шаблона ExClassWrapperFor
//		универсально для четырех вариантов данных 
//		- самого значения типа
//		- ссылки на значение типа
//		- конст ссылки на значение типа
//		- конст значения типа
// 	   Возможны и применяются иные способы создания врапперов (см. StlIndexedContainerWrapper, StlStringWrapper, т.п.)
// 	   (да хоть вообще вручную - враппер-это всего лишь класс с++)
// 	   И чтобы увязать это все в одну систему, чтобы можно было по-простому получить, скажем "враппер-для-конст-double"
// 		= WrapperFor<const double>
// 	   нужно сделать специализацию структуры wrapper_for_traits для соответствующего типа (типов, т.п.)
//
//		Ниже макрос ESB_WRAPPER декларирует специализации врапперов сделанные с его использованием.
//TODO	(ESB_WRAPPER_FOR_EMPTY - нет, это для енумов и стат-интерфейсов, как-то в общую схему не вписалось... пока... подумать...)
// 
// 	   Если враппер делается каким-то иным способом, то и специализировать wrapper_for_traits для него нужно отдельно.
//
	template<typename...>
	struct wrapper_for_traits {};
	//
#define ESB_DECLARE_WRAPPER_FOR_TRAITS_VVAL(...)        \
    template<> struct wrapper_for_traits<__VA_ARGS__> : std::type_identity< ExClassWrapperFor<__VA_ARGS__, __VA_ARGS__> > {};

#define ESB_DECLARE_WRAPPER_FOR_TRAITS_CVAL(...)        \
    template<> struct wrapper_for_traits<const __VA_ARGS__> : std::type_identity< ExClassWrapperFor<__VA_ARGS__, const __VA_ARGS__> > {};

#define ESB_DECLARE_WRAPPER_FOR_TRAITS_VREF(...)        \
    template<> struct wrapper_for_traits<__VA_ARGS__ &> : std::type_identity< ExClassWrapperFor<__VA_ARGS__, __VA_ARGS__ &> > {};

#define ESB_DECLARE_WRAPPER_FOR_TRAITS_CREF(...)        \
    template<> struct wrapper_for_traits<const __VA_ARGS__ &> : std::type_identity< ExClassWrapperFor<__VA_ARGS__, const __VA_ARGS__ &> > {};

#define ESB_DECLARE_WRAPPER_FOR_TRAITS_ALL(...)         \
    ESB_DECLARE_WRAPPER_FOR_TRAITS_VVAL(__VA_ARGS__)    \
    ESB_DECLARE_WRAPPER_FOR_TRAITS_CVAL(__VA_ARGS__)    \
    ESB_DECLARE_WRAPPER_FOR_TRAITS_VREF(__VA_ARGS__)    \
    ESB_DECLARE_WRAPPER_FOR_TRAITS_CREF(__VA_ARGS__)
	//
	template<typename CppTypeTy, typename... ExtraTs>
	using WrapperFor = wrapper_for_traits<CppTypeTy, ExtraTs...>::type;


// Все врапперы специализация этого шаблона, где
//	DataValueTy - тип без модификаторов (нр. int)
//	DataTy - конкретный тип данных, возможно с модификаторами (нр. const int&)
// обязаны соответствовать! 
// std::same_v<std::remove_cvref_t<DataTy>, DataValueTy>
	template<typename DataValueTy, typename DataTy>
	struct ExClassWrapperFor;


// Это тип ESB-Value (esb_ext:: class ValueForExEntity : public esb::Value)
// для враппера
	template<typename DataTy>
	using ValueForWrapper = esb::_internal::ValueFor< ExClassWrapperFor<std::remove_cvref_t<DataTy>, DataTy> >;



//... - CLASS_TY_, может быть шаблоном с аргументами через запятую. поэтому __VA_ARGS__ 
#define ESB_DECLARE_VALUE_FOR_WRAPPER_IMPL(VALUE_NAME_, VALUE_TEMPLATE_, ...)                   \
    using VALUE_NAME_ = VALUE_TEMPLATE_<__VA_ARGS__>

#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
#   define ESB_DECLARE_VALUE_FOR_WRAPPER_IMPL_RU(VALUE_NAME_RU_, VALUE_TEMPLATE_, ...)  ESB_DECLARE_VALUE_FOR_WRAPPER_IMPL(VALUE_NAME_RU_, VALUE_TEMPLATE_, __VA_ARGS__)
#else
#   define ESB_DECLARE_VALUE_FOR_WRAPPER_IMPL_RU(VALUE_NAME_RU_, VALUE_TEMPLATE_, ...)
#endif

#if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
#   define ESB_DECLARE_VALUE_FOR_WRAPPER_IMPL_EN(VALUE_NAME_EN_, VALUE_TEMPLATE_, ...)  ESB_DECLARE_VALUE_FOR_WRAPPER_IMPL(VALUE_NAME_EN_, VALUE_TEMPLATE_, __VA_ARGS__)
#else
#   define ESB_DECLARE_VALUE_FOR_WRAPPER_IMPL_EN(VALUE_NAME_EN_, VALUE_TEMPLATE_, ...)
#endif

//TODO	Большое TODO !!!
//		ESB_DECLARE_VALUE_FOR_WRAPPER принимает DATA_TY_ без учета что может быть шаблон с запятыми !!!
//		НУЖНО принимать DATA_SPEC_ или возможно __VA_ARGS__ последним параметром.
//		НО! оно уже много где используется и тут много кропотливой работы !!!
//
#define ESB_DECLARE_VALUE_FOR_WRAPPER(DATA_TY_, VALUE_NAME_EN_, VALUE_NAME_RU_)							\
    ESB_DECLARE_VALUE_FOR_WRAPPER_IMPL_EN(VALUE_NAME_EN_, ValueForWrapper, DATA_TY_);					\
    ESB_DECLARE_VALUE_FOR_WRAPPER_IMPL_RU(VALUE_NAME_RU_, ValueForWrapper, DATA_TY_)


//NOTE	**Последовательность реализации Враппера**
//TODO	Переписать (расписать) более толково
// 1) задать ESB_DECLARE_AS_VALUE_TYPE/_REFERENCE_TYPE для класса
// 2) Опт, задать ESB_DECLARE_VALUE_FOR_WRAPPER
// 		если используется в методах самого враппера 
// 		(если нет, то можно задать где-то после, или вообще не задавать если ненужно)
// 3) Собственно ESB_WRAPPER_BEGIN
// 		определить нужные методы/свойства
//		НЕ КОНСТАНТНЫЕ методы делать как ESB_DECLARE_TEMPLATE_METH_MUTABLE
//		закончить };
// 4) ESB_WRAPPER_INTERFACE(operations_research::ClosedInterval)
//		определить интерфейс (ESB_META_INTERFACE_MEMB_METH/ESB_META_INTERFACE_MEMB_PROP_RW_TEMPLATE, т.п.)
// 5) ESB_INTERFACEDEF_FOR_WRAPPER(operations_research::ClosedInterval (const, &, т.п.) - формы для класса, ГУИД, имена для 1С..
// 6) Опт, отложенная имплементация для методов при необходимости
// 7) convert_to SatClosedInterval <-> ..operations_research::ClosedInterval.. в том виде, как будет использоваться
// 8) convert_to IValuePtr <-> ..operations_research::ClosedInterval.. , если тип будет использоваться внутри контейнеров


//... - CPP_CLASS_TYPE_ - c++ тип без модификаторов. может быть шаблоном, поэтому __VA_ARGS__
#define ESB_WRAPPER_BEGIN(...)															\
    template<typename DataTy>                                                           \
    struct ExClassWrapperFor<__VA_ARGS__, DataTy> : ExClassWrapperBase<DataTy>          \
    {   /* проверяем, чтобы DataTy было формой того же типа */                          \
        static_assert(std::is_same_v<std::remove_cvref_t<DataTy>, __VA_ARGS__>, "Ops"); \
		static_assert(not std::is_empty_v<DataTy>, "Expected non-empty-type DataTy for ExClassWrapperBase-with-data-derived wrapper implementation.");	\
		using WrapType = __VA_ARGS__ ;													\
        using ExClassWrapperBase<DataTy>::ExClassWrapperBase;                           \
        private: using ExClassWrapperBase<DataTy>::_;                                   \
        public:

#define ESB_WRAPPER_BEGIN_EMPTY(WRAP_TYPE_, ...)										\
    template<typename DataTy>                                                           \
    struct ExClassWrapperFor<__VA_ARGS__, DataTy> : ExClassWrapperBaseEmpty<DataTy>     \
    {   /* проверяем, чтобы DataTy было формой того же типа */                          \
        static_assert(std::is_same_v<std::remove_cvref_t<DataTy>, __VA_ARGS__>, "Ops"); \
		static_assert(std::is_empty_v<DataTy>, "Expected empty-type DataTy for ExClassWrapperBaseEmpty-derived wrapper implementation.");	\
		using WrapType = WRAP_TYPE_;													\
        public:

//TODO	CLASS_TY_ должен быть (CLASS_TY_), т.к....
//		или должен быть ESB_WRAPPER_EX((CLASS_TY_)	...)
//		т.к. шаблоны !!!
//		ESB_DECLARE_VALUE_FOR_WRAPPER мешается !!!
#define ESB_WRAPPER(CLASS_TY_, VALUE_NAME_EN_, VALUE_NAME_RU_)							\
	ESB_DECLARE_WRAPPER_FOR_TRAITS_ALL(CLASS_TY_);										\
	ESB_DECLARE_VALUE_FOR_WRAPPER(CLASS_TY_, VALUE_NAME_EN_, VALUE_NAME_RU_);			\
	ESB_WRAPPER_BEGIN(CLASS_TY_)


//NOTE	Поскольку во враппере должен быть конкретный тип, для кого он враппер - WrapType
//		а empty-врапперы создаются для вспомогательных WrapperForStaticInterfaceTag или WrapperForEnumEnumeratorTag
//		который скрывают основной тип для кого враппер
//		то мы требуем, чтобы нам прямо передали WRAP_TY_
#define ESB_WRAPPER_FOR_EMPTY(WRAP_TYPE_, CLASS_TY_, VALUE_NAME_EN_, VALUE_NAME_RU_)	\
	ESB_DECLARE_VALUE_FOR_WRAPPER(CLASS_TY_, VALUE_NAME_EN_, VALUE_NAME_RU_);			\
	ESB_WRAPPER_BEGIN_EMPTY(WRAP_TYPE_, CLASS_TY_)



} //namespace esb


namespace esb
{

	//TODO	Не буду пока как-то утрясать это 'шаблоны', template_interface_descriptor и т.п.
	//		Нужно немного больше шаблонов и практики использования
	/*
	template<typename T>
	struct eval;                     // primary template
	template<template<typename, typename...> class TT, typename T1, typename... Rest>
	struct eval<TT<T1, Rest...>> {}; // partial specialization of eval
	*/
// Базовый шаблон дескриптора интерфейса для врапперов
// для каждого враппера с++ типа должна быть своя специализация
// у которой должно быть три метода:
//	::make_meta_interface_meth()
//	::make_meta_interface_prop()
//	::make_meta_interface_ctor()
//
	template<typename>
	struct template_interface_descriptor;
	//template<template<typename> typename TT, typename T>
	//struct template_interface_descriptor<TT<T>> 
	//{
	//};


	// Фильтры используемые в template_interface_descriptor_base
	template<typename MetaInterfaceMethTy>
	struct template_interface_filter_not_retval_false {
	    using ret_t = member_function_retval_t<typename MetaInterfaceMethTy::meth_t>;
	    static constexpr bool is_ignore_by_retval = std::is_same_v<ret_t, std::false_type>;
	    static constexpr bool value = not is_ignore_by_retval;
	};
	template<typename MetaInterfaceMethTy>
	struct template_interface_filter_not_retval_false_and_only_const
	{
	    using ret_t = member_function_retval_t<typename MetaInterfaceMethTy::meth_t>;
	    static constexpr bool is_ignore_by_retval = std::is_same_v<ret_t, std::false_type>;
	    static constexpr bool value /*true to include in interface*/ = not is_ignore_by_retval and is_const_member_function_pointer_v<typename MetaInterfaceMethTy::meth_t>;
	};


	template<typename>
	struct template_interface_filter_all : std::true_type {};
	template<typename FuncTy>
	struct template_interface_filter_all_not_nullptr : std::bool_constant<not std::is_same_v<FuncTy, std::nullptr_t>> {};

	//TODO используется в stl_string - оно от descriptor_base не унаследовано
	template<typename MetaInterfaceMethTy>
	struct template_interface_filter_meth_only_const
	{
		static constexpr bool value /*true to include in interface*/ = is_const_member_function_pointer_v<typename MetaInterfaceMethTy::meth_t>;
	};

	template<typename FuncTy>
	struct template_interface_filter_func_only_const_if_not_nullptr
	{
		// дла свойств, у них указатель может быть nullptr
		static constexpr bool value /*true to include in interface*/ = is_const_member_function_pointer_maybe_nullptr_v<FuncTy>;
	};


	template<template<typename...> typename WrapperTemplate, template<typename...> typename ValueTemplate, typename CppDataTy, typename... ExtraTemplateArgs>
	struct template_interface_descriptor_info_ex
	{
		static constexpr bool   is_reference	= std::is_reference_v<CppDataTy>;
		static constexpr bool   is_const		= std::is_const_v<std::remove_reference_t<CppDataTy>>;
		static constexpr bool   is_mutable		= !is_const;
		//
		using                   my_value_reference_t		= template_interface_descriptor_vref_value<WrapperTemplate, ValueTemplate, CppDataTy, ExtraTemplateArgs...>::type;
		using                   my_value_const_reference_t	= template_interface_descriptor_cref_value<WrapperTemplate, ValueTemplate, CppDataTy, ExtraTemplateArgs...>::type;
		//
		template<typename MetaInterfaceMethTy>
		using methods_filter_t = std::conditional_t<is_mutable,
			template_interface_filter_not_retval_false<MetaInterfaceMethTy>,
			template_interface_filter_not_retval_false_and_only_const<MetaInterfaceMethTy>>;
		//TODO  Фильтр методов опирается всю структуру MetaInterfaceMeth, а фильтр свойств только на тип указателя на функцию (гет или сет)
		//      разумно и фильтр методов тоже сделать только от типа указаьеля на функцию для универсальности и простоты.
		//      (Хотя "в общем и в будущем" мы можем цеплять к структурам MetaInterfaceMeth/MetaInterfaceProp 
		//       еще какие-нибудь аттрибуты и использовать при фильтрации еще и их.. Тогда можно оставить и так.. для примера)
		template<typename FuncTy>
		using prop_filter_get_t = template_interface_filter_all_not_nullptr<FuncTy>;
		template<typename FuncTy>
		using prop_filter_set_t = std::conditional_t<is_mutable, template_interface_filter_all<FuncTy>, template_interface_filter_func_only_const_if_not_nullptr<FuncTy>>;
	};


	template<typename DataTy>
	struct template_interface_descriptor_info
	{
		using type_t = std::remove_cvref_t<DataTy>;
		template<typename DataTy>
		using wrapper_t = ExClassWrapperFor<type_t, DataTy>;
		template<typename DataTy>
		using value_t = esb::_internal::ValueFor<wrapper_t<DataTy>>;	//esb_ext
		//
		using info_t = template_interface_descriptor_info_ex<wrapper_t, value_t, DataTy>;
	};

	struct wrapper_interface_descriptor_base : esb::_internal::interface_empty
	{
	protected:
		static constexpr auto make_meta_interface_meth_mutable()
		{
			return esb::_internal::make_meta_interface_meth();
		}
	};
//...CPP_TYPE_
#define ESB_WRAPPER_INTERFACE(...)													\
    template<typename DataTy>														\
    struct template_interface_descriptor< ExClassWrapperFor<__VA_ARGS__, DataTy> >  \
		: wrapper_interface_descriptor_base											\
    {                                                                               \
        using ESB_INTERFACE_OBJECT_FIELD = ExClassWrapperFor<__VA_ARGS__, DataTy>;  \
        using me = typename template_interface_descriptor_info<DataTy>::info_t;


//TODO	Желательно здесь сослаться на ex_class_wrapper_interface_base, а не дублировать макросы
#define ESB_WRAPPER_INTERFACE_MEMB_METH_STD_VREF  ESB_META_INTERFACE_MEMB_METH_TEMPLATE_OPT(me::my_value_reference_t,        ESB_WRAPPER_STD_METH_GetReference,				ESB_WRAPPER_STD_METH_GetReference_Ru)
#define ESB_WRAPPER_INTERFACE_MEMB_METH_STD_CREF  ESB_META_INTERFACE_MEMB_METH_TEMPLATE_OPT(me::my_value_const_reference_t,  ESB_WRAPPER_STD_METH_GetConstReference,		ESB_WRAPPER_STD_METH_GetConstReference_Ru)
#define ESB_WRAPPER_INTERFACE_MEMB_METH_STD       ESB_WRAPPER_INTERFACE_MEMB_METH_STD_VREF, ESB_WRAPPER_INTERFACE_MEMB_METH_STD_CREF

#define ESB_WRAPPER_INTERFACE_METH_MUTABLE(...)										\
    static constexpr auto make_meta_interface_meth_mutable()						\
    {																				\
    if constexpr (me::is_mutable)													\
        return esb::_internal::make_meta_interface_meth( __VA_ARGS__ );				\
    else																			\
        return esb::_internal::make_meta_interface_meth();							\
    }

#define ESB_WRAPPER_INTERFACE_METH(...)												\
    static constexpr auto make_meta_interface_meth()								\
    {																				\
        return std::tuple_cat														\
        (																			\
            make_meta_interface_meth_mutable(),										\
            esb::_internal::make_meta_interface_meth_if<me::methods_filter_t>		\
				(ESB_WRAPPER_INTERFACE_MEMB_METH_STD __VA_OPT__(,) __VA_ARGS__ )    \
        );																			\
    }

#define ESB_WRAPPER_INTERFACE_METH_STAT(...)										\
    static constexpr auto make_meta_interface_meth()								\
    {																				\
        return esb::_internal::make_meta_interface_meth (__VA_ARGS__ );				\
    }


//NOTE	Метод осуществляет фильтрацию переданных MetaInterfaceProp согласно переданным мета-фильтрам
//		(т.е. например для конст-вариантов класса из интерфейса убираются set-методы)
//		информация о методах "вынимается" из указателей на них
//		когда с++ компилятор берет указатель на метод, он всегда помечает его "к-имплементации"
// 		даже если он потом будет отфильтрован и не будет использоваться
//		(выбросить то что не будет использоваться уже задача оптимизатора)
//		НО! для начала компилятор метод для которого попросили указатель попытыется имплементировать
//			соответственно попытка имплементации не-конст метода для конст-класса приводит к ошибке
//		Здесь используется следующее решение этой проблемы:
//		методы требующие фильтрацию (скажем не-конст) объявляются шаблонами типа 
//			template<bool>	SetFoo(Foo foo_);		- !!! без имплементации !!! - просто для получения сигнатуры
//			template<>		SetFoo<true>(Foo foo_)	{ имплементация-установки-фуу }
//		макро-метод ESB_WRAPPER_INTERFACE_MEMB_PROP... запрашивает у компилятора указатель на метод 
//		передавая шаблонный параметор me::is_mutable вычисленный для конкретного варианта класса ( == not is_const)
//		и, соответственно, для конст-варианта получает указатель на заглушку без имплементации
//		make_meta_interface_prop_if смотрит что метод не-конст и отфильтровывает его не вставляя указатель в интерфейс
//		(а компилятор не делает имплементацию потому как для SetFoo<false> ее просто нет)
//		для mutable (не-конст вариантов класса) метод остается в интерфейсе и компилятор делает имплементацию для SetFoo<true>
//		Все Ок!
//		
//		Применительно к ESB_WRAPPER_INTERFACE_PROP это все означает, что если _PROP только _RO (т.е. только для чтения, конст-метод)
//		его можно объявлять без всяких выкрутасов с шаблонами как обычный метод и запрашивать обычным макро-методом 
//		ESB_META_INTERFACE_MEMB_PROP_RO (ESB_META_INTERFACE_STAT_PROP_RO) 
//		-конст существует во всех вариантах класса, не будет отфильтровываться и имплементация его не приводит к ошибкам
//		А для _RW (_WO) свойств обязательно нужно объявлять методы Get/Set как шаблоны 
//		(макросы ESB_WRAPPER_PROP_GET/_SET или ESB_WRAPPER_FIELD_GET/_SET или что-то подобное ручками)
//		и включать в интерфейс макросом ESB_WRAPPER_INTERFACE_MEMB_PROP_RW(_WO или _RO для общности)
// 
//		т.е. еще раз:	"Get -свойство"		- "просто-метод-Get"			-> просто ESB_META_INTERFACE_MEMB_PROP_RO
//						"Get+Set -свойство" - "ESB_WRAPPER_PROP_GET/_SET"	-> ESB_WRAPPER_INTERFACE_MEMB_PROP_RW
// 
//		ПС. Для методов ESB_WRAPPER_INTERFACE_METH сделано чуть проще, методов-шаблонов не надо, 
//			нужно не-конст методы помещать в отдельную секцию ESB_WRAPPER_INTERFACE_METH_MUTABLE, 
//			которая не трогается вообще для не-конст вариантов
// 
//		ППС. Ждем с++26 с рефлексией, которая все-все-все упростит.
//
#define ESB_WRAPPER_INTERFACE_PROP(...)												\
    static constexpr auto make_meta_interface_prop()								\
    {  return esb::_internal::make_meta_interface_prop_if<me::prop_filter_get_t, me::prop_filter_set_t> (__VA_ARGS__); }



//TODO	Конструкторы для ссылок мы создавать пока не умеем..
//		Теоретически конструктор для ссылки может принимать только объект-свое-значение и еще установить его себе владельцем...
//		и если это будет работать, то такие конструкторы должны генериться автоматически 
//		(если соответствующие объекты-значения определены)
// 
//		ПС. Пока ссылки можно делать создав объект-значение и вызвав у него метод .Получить[Конст]Ссылку()
//		->
//		Может это и есть самый правильный и логичный подход ?
//		Зачем делать какой-то-конструктор-для-ссылки-отдельно, 
//		если по определению ссылка должна получаться от какого-то конкретного объекта !
//
#define ESB_WRAPPER_INTERFACE_CTOR(...)												\
    static constexpr auto make_meta_interface_ctor()								\
    {																				\
        if constexpr (me::is_reference)												\
            return esb::_internal::make_meta_interface_ctor();						\
        else																		\
			return esb::_internal::make_meta_interface_ctor(__VA_ARGS__);			\
    }


//TODO  _CTOR - это описание как создать объект из 1С
//      сейчас мы разрешаем создание из 1С только value-типов (me::is_mutable && not me::is_ref)
//      НО!
//      мы могли-бы разрешать создание ref-типов, если нам дают аргумент соответсвующего value/ref типа
//      Также мы можем разрешить создание объекта от аргумента, который можно преобразовать в наши данные
//      ТАКЖЕ это особенн важно для const-value типа, который нельзы менять после создания - ему НУЖЕН источник из чего его создавать
#define ESB_WRAPPER_INTERFACE_CTOR_DEFAULT()										\
    static constexpr auto make_meta_interface_ctor()								\
    {																				\
        if constexpr (me::is_mutable && not me::is_reference)						\
            return esb::_internal::make_meta_interface_ctor( ESB_META_CTOR() );		\
        else																		\
            return esb::_internal::make_meta_interface_ctor();						\
    }



//NOTE  Мы не можем сделать частичную специализацию для всех вариантов template_interface_descriptor< object_t >
//      т.к. interface_info_t в то числе имеет поля GUID_, NAME_EN_, NAME_RU_, DESCR_, 
//      которые нужно задавать индивидульно для каждого варианта шаблона 
//      НО!
//      Декларировать interface_info_t нужно сразу для всех четырех вариантов, т.к. они друг с другом связаны
//      (или точнее value-вариант + ссылка + конст-ссылка или конст-валюе + конст-ссылка)
//
#define ESB_INTERFACEDEF_FOR_TEMPLATE_IMPL(INTERFACE_OBJECT_FIELD_)																				\
	static constexpr auto ESB_INTERFACE_METH_META_FIELD = template_interface_descriptor<INTERFACE_OBJECT_FIELD_>::make_meta_interface_meth();	\
	static constexpr auto ESB_INTERFACE_PROP_META_FIELD = template_interface_descriptor<INTERFACE_OBJECT_FIELD_>::make_meta_interface_prop();	\
	static constexpr auto ESB_INTERFACE_CTOR_META_FIELD = template_interface_descriptor<INTERFACE_OBJECT_FIELD_>::make_meta_interface_ctor()


#define ESB_INTERFACEDEF_FOR_WRAPPER(CLASS_T_, GUID_STR_, NAME_EN_, NAME_RU_, DESCRIPTION_)														\
    ESB_INTERFACEDEF_EX((ExClassWrapperFor<std::remove_cvref_t<CLASS_T_>, CLASS_T_>), GUID_STR_, (#NAME_EN_, #NAME_RU_), DESCRIPTION_)			\
    ESB_INTERFACEDEF_FOR_TEMPLATE_IMPL(ESB_INTERFACE_OBJECT_FIELD);																				\
    ESB_INTERFACEDEF_DONE(ExClassWrapperFor<std::remove_cvref_t<CLASS_T_>, CLASS_T_>);															\
    ESB_DECLARE_VALUE_FOR_WRAPPER(CLASS_T_, NAME_EN_, NAME_RU_)


// для деклараций методов для последующей фильтрации в make_meta_interface_meth_if/make_meta_interface_prop_if
#define ESB_WRAPPER_METH_CONST(RETV_, NAME_, ...)				\
    template<bool>												\
    PP_EXTRACT RETV_ NAME_(__VA_ARGS__) const

#define ESB_WRAPPER_METH_MUTABLE(RETV_, NAME_, ...)				\
    template<bool>												\
    PP_EXTRACT RETV_ NAME_(__VA_ARGS__);						\
    template<>													\
    PP_EXTRACT RETV_ NAME_<true>(__VA_ARGS__)


#define ESB_WRAPPER_PROP_GET(ESB_DT_, CPP_DT_, NAME_, GET_METH_)				\
    ESB_WRAPPER_METH_CONST((ESB_DT_),   Get##NAME_)								\
		{ return convert_to_ret<ESB_DT_>( _. GET_METH_ () ); }

#define ESB_WRAPPER_PROP_SET(ESB_DT_, CPP_DT_, NAME_, SET_METH_)				\
    ESB_WRAPPER_METH_MUTABLE((void),    Set##NAME_, const ESB_DT_& value_)		\
		{ _. SET_METH_( convert_to_arg<CPP_DT_>(value_) ); }

#define ESB_WRAPPER_PROP(ESB_DT_, CPP_DT_, NAME_, GET_METH_, SET_METH_)         \
    ESB_WRAPPER_PROP_GET(ESB_DT_, CPP_DT_, NAME_, GET_METH_)                    \
    ESB_WRAPPER_PROP_SET(ESB_DT_, CPP_DT_, NAME_, SET_METH_)


#define ESB_WRAPPER_FIELD_GET(ESB_DT_, CPP_DT_, NAME_, FIELD_)                  \
    ESB_WRAPPER_METH_CONST((ESB_DT_),   Get##NAME_)								\
		{ return convert_to_ret<ESB_DT_>(_. FIELD_); }

//NOTE	При присвоении значения полю мы используем convert_to_arg, 
//		т.к. присвоение значения ('=') это тоже выражение и в рамках его выполнения до ';'
//		все будут живы
#define ESB_WRAPPER_FIELD_SET(ESB_DT_, CPP_DT_, NAME_, FIELD_)                  \
    ESB_WRAPPER_METH_MUTABLE((void),    Set##NAME_, const ESB_DT_& value_)		\
		{ _. FIELD_ = convert_to_arg<CPP_DT_>(value_); }

#define ESB_WRAPPER_FIELD(ESB_DT_, CPP_DT_, NAME_, FIELD_)                      \
    ESB_WRAPPER_FIELD_GET(ESB_DT_, CPP_DT_, NAME_, FIELD_)						\
    ESB_WRAPPER_FIELD_SET(ESB_DT_, CPP_DT_, NAME_, FIELD_)


// для включения фильтруемых методов в интерфейс (ESB_WRAPPER_INTERFACE_METH(...))
#define ESB_WRAPPER_INTERFACE_MEMB_METH_MUTABLE(NAME_EN_, TEXT_RU_)				\
    ESB_META_INTERFACE_MEMB_METH_TEMPLATE_OPT(me::is_mutable, NAME_EN_, TEXT_RU_)

#define ESB_WRAPPER_INTERFACE_MEMB_PROP_RW(NAME_EN_, NAME_RU_)						\
    ESB_META_INTERFACE_MEMB_PROP_RW_TEMPLATE_OPT(me::is_mutable, NAME_EN_, NAME_RU_)

#define ESB_WRAPPER_INTERFACE_MEMB_PROP_RO(NAME_EN_, NAME_RU_)						\
    ESB_META_INTERFACE_MEMB_PROP_RO_TEMPLATE_OPT(me::is_mutable, NAME_EN_, NAME_RU_)

#define ESB_WRAPPER_INTERFACE_MEMB_PROP_WO(NAME_EN_, NAME_RU_)						\
    ESB_META_INTERFACE_MEMB_PROP_WO_TEMPLATE_OPT(me::is_mutable, NAME_EN_, NAME_RU_)

// Пост-имплементация "для всех вариантов DataTy", то есть для const-методов и для mutable-методов
// (mutable-методы для const-типов отсеиваются на уровне декларации интерфейса и компилятор их не реализует)
#define ESB_WRAPPER_METH_IMPL_RETV(RETV_T_, ...) RETV_T_
// ...RETV_T_
// возвращаемый тип нужно или явно передать вторым параметром, или будет auto и нужно указать -> RETV_T_ в конце
#define ESB_WRAPPER_METH_IMPL(CLASS_T_, ...)									\
    template<typename DataTy> inline ESB_WRAPPER_METH_IMPL_RETV(__VA_ARGS__ __VA_OPT__(,) auto)  ExClassWrapperFor<CLASS_T_, DataTy>




// для регистрации создания объекта из 1С (Новый(Типа(..))
// ...	- это с++ тип, если необходимо с квалификаторами
//		т.е. ESB_REGISTER_WRAPPER_FOR(double) зарегисирирует в системе типов 1С ESB::Double(Действительное)
//		а ESB_REGISTER_WRAPPER_FOR(const double) зарегисирирует в системе типов 1С ESB::DoubleConst(ДействительноеКонст)
#define ESB_REGISTER_WRAPPER_FOR(...)											\
	ESB_INTERFACEDEF_FACTORY_INSTANCE_AND_REG( WrapperFor<__VA_ARGS__> )


// Для енум-типов.
// Для енум-типов нам нужно два враппера - один собственно для значения енума, 
// а второй для объекта-перечислителя допустимых значений.

// Енум-Валюе
//TODO	Преобразования к числу и строке для 1С сделано explicit через методы конвертации
//		так как конверторы ExtConvertToNumeric и ExtConvertToString этого не умеют 
//		(см. примечания там)
//		НО! Если оставлять конвертацию здесь, то не дело с макросами играться 
//			- нужно отдельный базовый класс для енум-врапперов делать !!!
#define ESB_WRAPPER_FOR_ENUM(ENUM_, NAME_EN_, NAME_RU_, GUID_, DESCR_)                  \
    ESB_WRAPPER(ENUM_, NAME_EN_, NAME_RU_)                                              \
        Numeric ESB_CONVERTER_METH_ToNumeric() const                                    \
        {                                                                               \
            return Numeric(static_cast<std::underlying_type_t<ENUM_>>(_));              \
        }                                                                               \
        String ESB_CONVERTER_METH_ToString() const                                      \
        {                                                                               \
            return String(to_ustring(static_cast<std::underlying_type_t<ENUM_>>(_)));   \
        }                                                                               \
    };                                                                                  \
    ESB_WRAPPER_INTERFACE(ENUM_)                                                        \
    };                                                                                  \
    ESB_INTERFACEDEF_FOR_WRAPPER(ENUM_, GUID_, NAME_EN_, NAME_RU_, DESCR_)


// Для создания енумератора для енума мы используем всю ту же "машинерию" врапперов, а она требует базироваться на каком-то типе
// на самом типе-енума у нас базируется енум-валюе, ну а для типа енумератора приходится создавать фактически "фейковый" пустой тип - шоб було...
//TODO	можно просто нахально std::type_identity<> использовать чтобы сущности не плодить.. Или лучше плодить??
template<typename EnumTy>
struct WrapperForEnumEnumeratorTag
{
	using enum_t = EnumTy;
};

template<typename EnumTy>
using ValueForWrapperForEnumerator = ValueForWrapper<WrapperForEnumEnumeratorTag<EnumTy>>;

// макрос для енум-енумератора, "все-в-одном" - и враппер, и интерфейс, и интерфейс-деф
// в конце __VA_ARGS__ параметров - туда список ESB_WRAPPER_ENUM_VALUE(_EX) (см. ниже)
// для создания списка статических свойств представляющих значения енума.
// закончить );
#define ESB_WRAPPER_FOR_ENUM_ENUMERATOR(ENUM_, NAME_EN_, NAME_RU_, GUID_, DESCR_, ...)                      \
    ESB_WRAPPER_FOR_EMPTY(ENUM_, WrapperForEnumEnumeratorTag<ENUM_>, NAME_EN_, NAME_RU_)					\
        template<ENUM_ EnumValue_>                                                                          \
        static ValueForWrapper<ENUM_> GetEnumValue()                                                        \
        {                                                                                                   \
            static ValueForWrapper<ENUM_> value = ValueForWrapper<ENUM_>::Create(EnumValue_);               \
            return value;                                                                                   \
        }                                                                                                   \
    };                                                                                                      \
    ESB_WRAPPER_INTERFACE(WrapperForEnumEnumeratorTag<ENUM_>)                                               \
		using enum_t = ENUM_;                                                                               \
		ESB_WRAPPER_INTERFACE_PROP                                                                          \
		(                                                                                                   \
			__VA_ARGS__                                                                                     \
		)                                                                                                   \
    };                                                                                                      \
    ESB_INTERFACEDEF_FOR_WRAPPER(WrapperForEnumEnumeratorTag<ENUM_>, GUID_, NAME_EN_, NAME_RU_, DESCR_)


//NOTE	макросы ESB_WRAPPER_ENUM_VALUE(_EX) ожидают, что будут разворачиваться в контексте где есть enum_t
// 
//NOTE	метод енумератора называется GetEnumValue, но мы передаем EnumValue<enum_t::VALUE_>, 
//		т.к. Get подставляет ESB_META_INTERFACE_MAKE_PROP_RO

// тут если токен-значение-енумератора "непотребное" (типа DecisionStrategyProto_VariableSelectionStrategy_CHOOSE_FIRST)
// то тут токен-значение отдельно, а имя-ен для 1С отдельно
#define ESB_WRAPPER_ENUM_VALUE_EX(VALUE_, NAME_EN_, NAME_RU_)	\
		ESB_META_INTERFACE_MAKE_PROP_RO(ESB_INTERFACE_OBJECT_FIELD::template, EnumValue<enum_t::VALUE_>, NAME_EN_, NAME_RU_)

// тут если имя константы енума допустимо и как имя для 1С
#define ESB_WRAPPER_ENUM_VALUE(NAME_EN_, NAME_RU_)				\
	ESB_WRAPPER_ENUM_VALUE_EX(NAME_EN_, NAME_EN_, NAME_RU_)


template<typename TypeTy>
struct WrapperForStaticInterfaceTag
{
	using type_t = TypeTy;
};

#define ESB_WRAPPER_FOR_STATICS(WRAP_TYPE_, NAME_EN_, NAME_RU_)							\
    ESB_WRAPPER_FOR_EMPTY(WRAP_TYPE_, WrapperForStaticInterfaceTag<WRAP_TYPE_>, NAME_EN_, NAME_RU_)

#define ESB_WRAPPER_INTERFACE_FOR_STATICS(TYPE_SPEC_)                                               \
    ESB_WRAPPER_INTERFACE(WrapperForStaticInterfaceTag<PP_EXTRACT TYPE_SPEC_>)

#define ESB_INTERFACEDEF_FOR_WRAPPER_STATICS(TYPE_SPEC_, GUID_, NAME_EN_, NAME_RU_, DESCR_)         \
    ESB_INTERFACEDEF_FOR_WRAPPER(WrapperForStaticInterfaceTag<PP_EXTRACT TYPE_SPEC_>, GUID_, NAME_EN_, NAME_RU_, DESCR_)

//TODO	Из имени метода ну никак не видно, что он 'STAT' и иногда это ставит в тупик
//		Лучше было бы что-то типа GetStat##NAME_
//		Но тогда усложняется с ESB_META_INTERFACE_xxxx_PROP_RO - их _EX делать придется 
//		- метод GetStatFOO отдельно, а имя свойства FOO отдельно, т.к. в 1С-интерфейсе наши префиксы 'Stat' никчему.
#define ESB_WRAPPER_METH_IMPL_FREE_GET_CREATE_STATIC(TYPE_, NAME_)		\
    TYPE_ Get##NAME_()													\
    {																	\
        static TYPE_ value = TYPE_::Create();							\
        return value;													\
    }

#define ESB_WRAPPER_METH_IMPL_STAT_GET_CREATE_STATIC(TYPE_, NAME_)		\
    static ESB_WRAPPER_METH_IMPL_FREE_GET_CREATE_STATIC(TYPE_, NAME_)


//TODO  До конца еще недодумано-недоделано. Только один аргумент, и вообще..
//      Надеюсь до с++26 с рефлексией в таком виде доживет..
// 
//NOTE  Мы по-умолчанию всегда декларируем "вызываемое" как value-type, хотя возможны сценарии захвата данных по ссылке. 
//      Но это тогда вручную делать.
//NOTE  Здесь всегда используется обычный ESB_WRAPPER для типов с данными. 
//      Это правильно, т.к. даже если нет захвата данных, то мы всегда будем содержать сам DelegatToMeth
//NOTE	В интерфейсе для 1С здесь мы метод "Вызвать" не делаем - это для с++ "вызываемое"
//		(собственно "делегат" на метод объекта платформы - это немного другое - без оператора и плюс "Вызвать")
//NOTE  Если CLASS_TY_ слишком длинны и/или с запятыми используйте типдеф.
//
#define ESB_WRAPPER_FOR_INVOCABLE(CLASS_TY_, RET_TY_, ARG_TY_, NAME_EN_, NAME_RU_, GUID_, DESCR_)   \
    ESB_DECLARE_AS_VALUE_TYPE(CLASS_TY_);                                                           \
    ESB_WRAPPER(CLASS_TY_, NAME_EN_, NAME_RU_)					                                    \
        void operator()(PP_EXTRACT ARG_TY_ value_) const                                            \
        {                                                                                           \
            return _.Invoke<PP_EXTRACT RET_TY_>(value_);                                            \
        }                                                                                           \
    };                                                                                              \
    ESB_WRAPPER_INTERFACE(CLASS_TY_)                                                                \
    };                                                                                              \
    ESB_INTERFACEDEF_FOR_WRAPPER(CLASS_TY_, GUID_, NAME_EN_, NAME_RU_, DESCR_)




} // namespace esb

#endif //ESB_WRAPPER_H