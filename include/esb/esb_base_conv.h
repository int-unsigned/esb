/*
	"ESB" is designed to create external native components for the 1C(tm) program.
	Copyright © 2023 IntUnsigned	(v8classes@gmail.com)
	"ESB" is free software under LGPLv2.1 license with essential restriction about binary part (view esb-license.txt)
	"ESB" IS DISTRIBUTED "AS-IS" WITHOUT ANY, EVEN THE IMPLIED, WARRANTY. IN NO EVENT SHALL THE AUTHORS OR THE COPYRIGHT OWNER BE LIABLE FOR ANY CLAIM, 
	DAMAGES OR OTHER LIABILITY ARISING OUT OF THE USE OF THIS SOFTWARE.
	THE AUTHOR DOES NOT GIVE ANY GUARANTEES AND DOES NOT BEAR ANY RESPONSIBILITY REGARDING THE USE OF 1C(tm) COMPONENTS PRODUCED USING THE ESB LIBRARY IN THE 1C(tm) ENVIRONMENT.
*/


/********************************************************************
* Если один костыль хорошо забетонировать, то другие убрать можно.	*
* *******************************************************************
*	Сказано мудро это. Но осторожным быть должен ты.
*	Если костыль один в бетон замурован крепко, опорой он станет надежной. 
*		Однако помни: когда в камень превращается то, что двигаться должно, гибкость теряешь ты.
*		- Устойчивость обретешь ты, если фундамент крепок.
*		- Свободу потеряешь ты, если костыль этот с места сдвинуть не сможешь .
*	Тьма скрывает будущее, где другие опоры исчезли, а единственная — неподвижна.
*	Истинная сила не в бетоне, а в балансе Силы. 
*	Костыль уберешь — сам стоять научишься ли?
*	Правду ли говоришь ты о надежности, или иллюзия это, скрывающая нежелание идти своим путем?
*	-
*	*Приведенная информация носит справочный характер. 
*	*Если вам требуется медицинская консультация или постановка диагноза, обратитесь к специалисту...
*/


#pragma once
#ifndef ESB_BASE_CONV_H_
#define ESB_BASE_CONV_H_

#include "esb_core.h"
#include "esb_traits.h"
#include "esb_base_type.h"


namespace esb 
{

	struct converter_unavailable_tag {};
	//
	template<typename ToTy, typename... FromTs>
	struct converter_to : converter_unavailable_tag
	{	
		//NOTE	метод convert отсутствует, чтобы в случае если компилятор до сюда дойдет 
		//		он внятно сказал какой именно специализации ему не хватает
	};
	//
	template<typename ToTy, typename... FromTy>
	inline constexpr bool is_convert_to_avalable = ( not std::is_base_of_v< converter_unavailable_tag, converter_to<ToTy, FromTy...> > );


	template<typename ToTy, typename... FromTs>
	struct converter_to_arg : converter_unavailable_tag
	{	//NOTE	см. для converter_to
	};
	//
	template<typename ToTy, typename... FromTy>
	inline constexpr bool is_convert_to_avalable_for_arg_special = (not std::is_base_of_v< converter_unavailable_tag, converter_to_arg<ToTy, FromTy...> >);


	template<typename ToTy, typename... FromTs>
	struct converter_to_ret : converter_unavailable_tag
	{	//NOTE	см. для converter_to
	};
	//
	template<typename ToTy, typename... FromTy>
	inline constexpr bool is_convert_to_avalable_for_ret_special = (not std::is_base_of_v< converter_unavailable_tag, converter_to_ret<ToTy, FromTy...> >);
}



namespace esb
{
//NOTE	"Фундаментальное"
//		Любой есб-класс можно создать через check_and_make
//		По умолчанию разрешается только создание перемещением IValuePtr.
//		Для дисциплины. Обычно именно это и подразумевается, только std::move ленится...
//		Иногда, конечно, копирования не хватает, но это легко обходится вызовом c копией IValuePtr{..}
//TODO	Желательно, конечно, сделать converter_to_copy, чтобы не обходить костылями
//		тем более что check_and_make_copy есть
//
	template<typename ToEsbTy>
		requires is_esb_class<ToEsbTy>
	struct esb::converter_to<ToEsbTy, IValuePtr&&>
	{
		static ToEsbTy convert(IValuePtr&& value_interface_) {
			return check_and_make<ToEsbTy>(std::move(value_interface_));
		}
	};
//NOTE	У любого esb-класса можно получить IValuePtr
//		здесь разрешены перемещения - из временного есб-класса IValuePtr мувнется и останется тыква.. как и должно быть.
	template<typename FromTy>
		requires is_esb_class<std::remove_cvref_t<FromTy>>
	struct esb::converter_to<IValuePtr, FromTy>
	{
		static IValuePtr convert(auto&& value_) {
			return get_interface(std::forward<decltype(value_)>(value_));
		}
	};


//NOTE  Объявляет конвертатор для всех вариантов типа. 
//      Третьим параметром (...) ожидает конвертирующее выражение. 
//      Аргумент для конвертирования именован 'value_'
//			нр. ESB_DECLARE_CONVERT_TO_TYPE_FROM(String, std::string, String(convert_to_string_U(value_)) );
// 
//NOTE	Если тип позволяет конвертирование в любом случае, 
//		то лучше использовать этот вариант а не просто делать ..FROM.. const Type&
//		так как несмотря на то, что "const Type&" - конвертер может быть использован для всех вариантов типа,
//		НО! is_convert_to_available его не найдет !
//		будет static_assert, или нужно искать ручками конвертер (иногда целесообразно)
//
#define ESB_DECLARE_CONVERT_TO_TYPE_FROM(TYPE_TO_, TYPE_FROM_, ...)     \
template<typename FromTy>                                               \
	requires std::is_same_v<std::remove_cvref_t<FromTy>, TYPE_FROM_>    \
struct esb::converter_to<TYPE_TO_, FromTy>                              \
{                                                                       \
    static TYPE_TO_ convert(auto&& value_) { return __VA_ARGS__; }		\
}


#define ESB_DECLARE_CONVERT_TO_FOR_FUNDAMENTAL_TO_ESB(CPP_TY_, ESB_TY_)						\
	ESB_DECLARE_CONVERT_TO_TYPE_FROM(IValuePtr, CPP_TY_, get_interface(ESB_TY_(value_)))

#define ESB_DECLARE_CONVERT_TO_FOR_FUNDAMENTAL_TO_CPP(CPP_TY_, ESB_TY_, TO_CPP_CONVERTER_)	\
	template<>																				\
	struct esb::converter_to<CPP_TY_, IValuePtr&&>											\
	{																						\
		static CPP_TY_ convert(IValuePtr&& value_interface_) {								\
			ESB_TY_ value = check_and_make<ESB_TY_>(std::move(value_interface_));			\
			return TO_CPP_CONVERTER_<CPP_TY_>(value);										\
		}																					\
	}


#if defined(ESB_USE_AUTO_CONVERTION_VALUE_TO_CPP_TYPE) && ESB_USE_AUTO_CONVERTION_VALUE_TO_CPP_TYPE
#define ESB_DECLARE_CONVERT_TO_FOR_FUNDAMENTAL(CPP_TY_, ESB_TY_, TO_CPP_CONVERTER_)			\
	ESB_DECLARE_CONVERT_TO_FOR_FUNDAMENTAL_TO_ESB(CPP_TY_, ESB_TY_);						\
	ESB_DECLARE_CONVERT_TO_FOR_FUNDAMENTAL_TO_CPP(CPP_TY_, ESB_TY_, TO_CPP_CONVERTER_)
#else
#define ESB_DECLARE_CONVERT_TO_FOR_FUNDAMENTAL(CPP_TY_, ESB_TY_, TO_CPP_CONVERTER_)
#endif


#define ESB_DECLARE_CONVERT_TO_FOR_NUMERIC(CPP_NUMERIC_TYPE_)														\
		ESB_DECLARE_CONVERT_TO_TYPE_FROM(CPP_NUMERIC_TYPE_, Numeric, ToIntegerOrThrow<CPP_NUMERIC_TYPE_>(value_));	\
		ESB_DECLARE_CONVERT_TO_TYPE_FROM(Numeric, CPP_NUMERIC_TYPE_, Numeric(value_));								\
		ESB_DECLARE_CONVERT_TO_FOR_FUNDAMENTAL(CPP_NUMERIC_TYPE_, Numeric, ToIntegerOrThrow)

	//
	// Конструкторы есть для short, int, long, long long + их unsigned варианты
	ESB_DECLARE_CONVERT_TO_FOR_NUMERIC(signed char);
	ESB_DECLARE_CONVERT_TO_FOR_NUMERIC(short);
	ESB_DECLARE_CONVERT_TO_FOR_NUMERIC(int);
	ESB_DECLARE_CONVERT_TO_FOR_NUMERIC(long);
	ESB_DECLARE_CONVERT_TO_FOR_NUMERIC(long long);
	ESB_DECLARE_CONVERT_TO_FOR_NUMERIC(unsigned char);
	ESB_DECLARE_CONVERT_TO_FOR_NUMERIC(unsigned short);
	ESB_DECLARE_CONVERT_TO_FOR_NUMERIC(unsigned int);
	ESB_DECLARE_CONVERT_TO_FOR_NUMERIC(unsigned long);
//NOTE	Это std::is_arithmetic, но НЕ is_standard_integer, а is_standard_character
//		они имеют право преобразовываться в-из Numeric, но иногда могут быть нюансы типа как с cmp_equal/cmp_greater
	ESB_DECLARE_CONVERT_TO_FOR_NUMERIC(char);
	ESB_DECLARE_CONVERT_TO_FOR_NUMERIC(wchar_t);
	ESB_DECLARE_CONVERT_TO_FOR_NUMERIC(char8_t);
	ESB_DECLARE_CONVERT_TO_FOR_NUMERIC(char16_t);
	ESB_DECLARE_CONVERT_TO_FOR_NUMERIC(char32_t);
	
//TODO	У 1С действительно есть конструктор create_numeric_value(unsigned __int64)
//		а вот оператор преобразования из Numeric в unsigned long long я сам придумал (из просто long long кастится)
//		по видимому нужно считать unsigned long long тоже "фундаментальным" типом
//		поскольку мы в него можем записать без исключения unsigned long long и прочитать тоже без исключения
//TODO	НУЖНО ПРОВЕРИТЬ на обрезание значения !!
	ESB_DECLARE_CONVERT_TO_FOR_NUMERIC(unsigned long long);
#undef ESB_DECLARE_CONVERT_TO_FOR_NUMERIC

	ESB_DECLARE_CONVERT_TO_TYPE_FROM(Boolean, bool, Boolean(value_));
	ESB_DECLARE_CONVERT_TO_TYPE_FROM(bool, Boolean, static_cast<bool>(value_));
	ESB_DECLARE_CONVERT_TO_FOR_FUNDAMENTAL(bool, Boolean, static_cast);


//TODO	встроенная конвертация double-Numeric - ЭТО НЕ ПРАВИЛЬНО
//		и дело даже не в том "умещается-ли", а в том что double может быть NaN !!!
	ESB_DECLARE_CONVERT_TO_TYPE_FROM(Numeric, double, Numeric(value_));
	ESB_DECLARE_CONVERT_TO_TYPE_FROM(double, Numeric, static_cast<double>(value_));

	//
	static_assert(is_convert_to_avalable<int64_t, const Numeric&> == true,			"unexpected is_convert_to_avalable==false");
	static_assert(is_convert_to_avalable<unsigned __int64, const Numeric&> == true, "unexpected is_convert_to_avalable==false");
	static_assert(is_convert_to_avalable<const int64_t&, const Numeric&> == false,	"unexpected is_convert_to_avalable==true");



	//TODO	Ниже поддержка конввертации для ext-типов у которых есть .Contained().Data()
	//		а это все определено в esb_ext.
	//		Непорядок..

// Используется во всяких requires и convert_to ( .ESB_CONTAINED_GET().ESB_CONTAINED_DATA() )
#define ESB_CONTAINED_GET	GetContained
#define ESB_CONTAINEDTYPE	ContainedType

	//TODO	А почему все это не в _internal ?
	template<typename T>
	inline constexpr bool is_esb_value_ext = requires
	{
		requires is_esb_value_based<T>;
		//
		typename T::descriptor_t;
		typename T::descriptor_t::esb_value_t;
		requires std::is_same_v<typename T::descriptor_t::esb_value_t, T>;
	};
	template<typename T>
	inline constexpr bool is_esb_value_ext_with_contained = requires(T x)
	{
		requires is_esb_value_ext<T>;
		x.ESB_CONTAINED_GET();
	};
	template<typename T>
	inline constexpr bool is_esb_value_ext_with_contained_data = requires(T x)
	{
		requires is_esb_value_ext_with_contained<T>;
		x.ESB_CONTAINED_GET().ESB_CONTAINED_DATA();
	};

	template<typename T>
	inline constexpr bool is_type_wrapper_contain_reftype = requires
	{
		typename T::ESB_CONTAINEDTYPE::ESB_CONTAINED_DATA_T;
		requires is_type_reftype<typename T::ESB_CONTAINEDTYPE::ESB_CONTAINED_DATA_T>;
	};

	// пока так примитивно. возможно в дальнейшем нужно будет спец-треитс.
	template<typename T>
	inline constexpr bool is_type_require_owner = is_type_wrapper_contain_reftype<T>;


	template<typename T, typename DataTy>	
	inline constexpr bool is_type_wrapper_contain_refence_to_type = requires
	{
		typename T::ESB_CONTAINEDTYPE::ESB_CONTAINED_DATA_T;
		requires std::is_reference_v<typename T::ESB_CONTAINEDTYPE::ESB_CONTAINED_DATA_T>;		
		requires std::is_same_v<typename T::ESB_CONTAINEDTYPE::ESB_CONTAINED_DATA_T, std::add_lvalue_reference_t<DataTy>>;
	};
	template<typename T, typename DataTy>
	inline constexpr bool is_type_wrapper_contain_refence_to = is_type_wrapper_contain_refence_to_type<T, std::remove_cvref_t< DataTy>> || is_type_wrapper_contain_refence_to_type<T, const std::remove_cvref_t< DataTy>>;


	//NOTE	Проверка ... -> std::convertible_to<ToTy> для конвертера может быть опасна, 
	//		т.к. конвертирование может осуществляться, например, конвертирующим конструктором с созданием нового объекта. 
	//		При этом если требуется вернуть его по ссылке, то получаем возврат висящей ссылки из функции.
	// 
	//		Для конвертера безопаснее использовать is_convert_to_contained_convertable_for_return, 
	//		которая разрешает преобразования только в пределах иерархии наследования. 
	//		Без конвертирующих конструкторов и операторов преобразования
	// 
	//		Или всегда возможно что-то свое ручками сделать для конкретного случая
	//
	template<typename ToTy, typename FromTy>
	inline constexpr bool is_convert_to_contained_convertable = requires(FromTy from_)
	{
		requires is_esb_value_ext_with_contained_data<std::remove_cvref_t<FromTy>>;
		{ from_.ESB_CONTAINED_GET().ESB_CONTAINED_DATA() } -> std::convertible_to<ToTy>;
	};


	//NOTE	std::is_same_v и компания не учитывают константность.
	//		Учитывайте отдельно
	template<typename ToTy, typename FromTy>
	inline constexpr bool is_same_or_base_of_or_derived_from = (std::is_same_v<ToTy, FromTy> || std::is_base_of_v<ToTy, FromTy> || std::is_base_of_v<FromTy, ToTy>);

	template<typename ToTy, typename FromTy>
	inline constexpr bool is_convert_to_contained_convertable_for_return = requires(FromTy from_)
	{
		requires is_esb_value_ext_with_contained_data<std::remove_cvref_t<FromTy>>;		
		//NOTE	Проверять ::is_convertible_v ( ::convertible_to) можно, но только проверять на конвертируемость ссылок, 
		//		т.к. если у класса вдруг удален конструктор от себя, то он будет НЕ-КОНВЕРТИРУЕМЫМ сам в себя! 
		//		Упс..
		//		std::is_convertible_v< operations_research::sat::Model, operations_research::sat::Model >	== false
		//		но
		//		std::is_convertible_v< operations_research::sat::Model&, operations_research::sat::Model& > == true
		//
		//		Для "ретурн" разрешаем только в пределах иерархии наследования, чтобы вякие конвертирующие операторы/конструкторы не учитывались
		//
		requires is_same_or_base_of_or_derived_from< std::remove_cvref_t<ToTy>, std::remove_cvref_t< decltype(from_.ESB_CONTAINED_GET().ESB_CONTAINED_DATA()) > >;
		// и отдельно проверяем константную целостность
		requires (std::is_const_v<std::remove_reference_t<ToTy>> || not std::is_const_v<std::remove_reference_t<FromTy>>);
	};



	template<typename ToTy, typename FromTy>
	inline constexpr bool is_convert_to_contained_cref = requires
	{
		requires is_const_reference_v<ToTy>;
		requires is_convert_to_contained_convertable<ToTy, FromTy>;
	};


	// Разрешаем по-умолчанию конвертирование енум-врапперов в содержащиеся в них енумы
	template<typename TYPE_TO_, typename TYPE_FROM_>
		requires (std::is_enum_v<TYPE_TO_> && esb::is_convert_to_contained_convertable<TYPE_TO_, TYPE_FROM_>)
	struct esb::converter_to<TYPE_TO_, TYPE_FROM_>
	{
		static TYPE_TO_	convert(TYPE_FROM_ value_)
		{
			return value_.GetContained().Data();
		}
	};


	// Разрешаем по-умолчанию конвертирование врапперов в то что они содержат. 
	// По-умолчанию разрешается только возврат ссылки на содержимое! Копии разрешать вручную!
	// в конст-ссылку можно конвертировать любой соответствующий враппер-тип в пределах иерархии наследования
	// БЕЗ выполнения операторов конвертации и конвертирующих конструкторов! см.is_convert_to_contained_convertable_for_return
	template<typename TYPE_TO_, typename TYPE_FROM_>
		requires (std::is_reference_v<TYPE_TO_> && is_convert_to_contained_convertable_for_return<TYPE_TO_, TYPE_FROM_> && not std::is_rvalue_reference_v<TYPE_FROM_>)
	struct esb::converter_to<TYPE_TO_, TYPE_FROM_>		
	{	static TYPE_TO_	convert(TYPE_FROM_ value_) 
		{
			return static_cast<TYPE_TO_>(value_.GetContained().Data());
		}
	};
	// Также разрешаем "естественное" конвертирование в указатели
	template<typename TYPE_TO_, typename TYPE_FROM_>
		requires (std::is_pointer_v<TYPE_TO_> && is_convert_to_contained_convertable_for_return<std::remove_pointer_t<TYPE_TO_>, TYPE_FROM_> && not std::is_rvalue_reference_v<TYPE_FROM_>)
	struct esb::converter_to<TYPE_TO_, TYPE_FROM_>
	{
		static TYPE_TO_	convert(TYPE_FROM_ value_)
		{
			return &value_.GetContained().Data();
		}
	};


	template<typename ToExtValueTy, typename FromCppTy>
	inline constexpr bool is_convert_to_create_move = requires(ToExtValueTy ext_value_)
	{
		requires is_esb_value_ext_with_contained_data<ToExtValueTy>;
		requires std::is_rvalue_reference_v<FromCppTy>;
		{ ext_value_.ESB_CONTAINED_GET().ESB_CONTAINED_DATA() } -> std::same_as< std::remove_reference_t<FromCppTy>& >;
	};

	template<typename ToExtValueTy, typename... FromCppTs>
	inline constexpr bool is_convert_to_create_any_way_avalable = requires()
	{
		//TODO	Нужно "удоскональть" что значит "contained". Наверное нужно ужесточить - это и Contained и ContainadDataType и .Data() и DataType
		requires is_esb_value_ext_with_contained_data<ToExtValueTy>;
		typename ToExtValueTy::ContainedDataType;
		// если наш вложенный может сконструироваться из переданных аргументов, то и наш враппер может сделать ::Create(...)
		requires std::is_constructible_v<typename ToExtValueTy::ContainedDataType, FromCppTs...>;		
	};


	//NOTE	!!! по-умолчанию разрешено только move-создание														!!!
	//		!!! любое прочее создание, в том числе создание копии, или что еще допускает конструктор объекта	!!!
	//		!!! декларировать отдельно своими руками															!!!
	// Разрешаем по-умолчанию естесственное для врапперов конвертирование-создание перемещением из содержащегося в них спп-типа
	// ( вроде convert_to<ArrayOfInteger64, std::vector<int64_t>>(std::vector<int64_t>&& value_) -> return ArrayOfInteger64::Create(std::move(value_)) )
	// Только для создания перемещением! для копирование указываем вручную ESB_DECLARE_CONVERT_TO_CREATE
	//
	// это для вал-типов.
	template<typename TYPE_TO_, typename TYPE_FROM_>
	requires (is_convert_to_create_move<TYPE_TO_, TYPE_FROM_> and not is_type_require_owner<TYPE_TO_>)
	struct esb::converter_to<TYPE_TO_, TYPE_FROM_>
	{
		static TYPE_TO_	convert(TYPE_FROM_ value_)
		{
			return TYPE_TO_::Create(std::move(value_));
		}
	};
	// для реф-типов
	template<typename TYPE_TO_, typename TYPE_FROM_>
		requires (is_convert_to_create_move<TYPE_TO_, TYPE_FROM_> and is_type_require_owner<TYPE_TO_>)
	struct esb::converter_to<TYPE_TO_, ESB_OWNER_HANDLER_T&&, TYPE_FROM_>
	{
		static TYPE_TO_	convert(ESB_OWNER_HANDLER_T&& owner_, TYPE_FROM_ value_)
		{
			return TYPE_TO_::Create(std::move(owner_), std::move(value_));
		}
	};



	//NOTE	IValuePtr&& должен быть первым параметром, т.к. FromTs может быть списком.
	//		Также по-умолчанию мы разрешаем только типичное преобразование от rvalue IValuePtr&&
	//		с иными, не типичными преобразованиями от IValuePtr, const IValuePtr& т.п. нужно разбираться/делать ручками
	template<typename ToTy, typename... FromTs>
	inline constexpr bool is_convert_to_avalable_with_owner = is_convert_to_avalable<ToTy, ESB_OWNER_HANDLER_T&&, FromTs...>;


	// Разрешить создание объекта-ссылки из соответствующего объекта или соответствующей ссылки 
	// т.е. создать CStringAnsiRef из std::string (привязка) или создать CStringAnsiRef из std::string& (копирование)
	//  - создавать ссылку из std::string&& НЕЛЬЗЯ - висящая
	//  - объект-ссылка требует владельца
	// 
	template<typename TYPE_TO_, typename TYPE_FROM_>
		requires ( not std::is_rvalue_reference_v<TYPE_FROM_> and is_type_wrapper_contain_refence_to_type<TYPE_TO_, std::remove_reference_t<TYPE_FROM_>> )
	struct esb::converter_to<TYPE_TO_, ESB_OWNER_HANDLER_T&&, TYPE_FROM_>
	{
		static TYPE_TO_	convert(ESB_OWNER_HANDLER_T&& owner_, TYPE_FROM_ value_)
		{
			return TYPE_TO_::Create(std::move(owner_), value_);
		}
	};
}


namespace esb
{
	//TODO  Несколько специфическая конвертация, поэтому сделана отдельным конвертером
	//      Возможно нужно "вставить" ее в обычный converter_to
	//      Посмотрим..
	//      ПС. Поэтому общий шаблон <typename ToTy, typename... FromTs> не меняю
	//
	template<typename ToTy, typename... FromTs>
	struct converter_to_from_one_of : converter_unavailable_tag
	{	// метод без реализации чтобы компилятор здесь явно выругался какого именно convert он не нашел.
	};
	template<typename ToTy, typename T>
	struct esb::converter_to_from_one_of<ToTy, OneOfValue<T> >
	{
		// Ожидаем auto&&, но типа OneOfValue<T>
		static ToTy convert(auto&& value_)
		{
			static_assert(std::is_same_v < std::remove_cvref_t<decltype(value_)>, OneOf<T>>, "Expected OneOf<T>.");
			//NOTE	value_.make<T>() дает нам && (естесственно - make дает копию), соответственно convert_to на голом месте получает &&, 
			//		что не отражает действительного характера value_ - это только копия &&, но она копия value_, которая живет своей жизнью
			//		Поэтому, если value_ временный, то и преобразуем напрямую (от временной копии),
			//		иначе преобразуем через через промежуточный lvalue
			//NOTE	И здесь мы обычно ловим невозможность преобразование временного объекта в ссылку на его данные 
			//		то есть потенциальное создание висячей ссылки.
			//		Если получение ссылки на данные от временного объекта безопасно (в контексте вычисления полного выражения)
			//		то нужно это разрешать отдельно.
			if constexpr (std::is_rvalue_reference_v<decltype(value_)>)
				return convert_to<ToTy>(value_.make<T>());
			else
			{
				decltype(auto) v = value_.make<T>();
				return convert_to<ToTy>(v);
			}
		}
	};
	template<typename ToTy, typename T, typename... RestTy>
	struct esb::converter_to_from_one_of<ToTy, OneOfValue<T, RestTy...> >
	{
		// Ожидаем auto&&, но типа OneOfValue<T, RestTy...>
		static ToTy convert(auto&& value_)
		{
			static_assert(std::is_same_v < std::remove_cvref_t<decltype(value_)>, OneOf<T, RestTy...>>, "Expected OneOf<T, RestTy...>.");
			if (decltype(auto) t_val = value_.make_if<T>())
				return convert_to<ToTy>(*t_val);
			else
				return esb::converter_to_from_one_of<ToTy, OneOf<RestTy...> >::convert(static_cast_one_of_to_subset<OneOf<RestTy...>>(std::forward<decltype(value_)>(value_)));
		}
	};
}


namespace esb
{
	template<typename ToTy, typename T, typename... RestTy>
		requires (not std::is_same_v<T, Undef>)
	struct esb::converter_to<ToTy, OneOfValue<T, RestTy...> >
	{
		// Ожидаем auto&&, но типа OneOfValue<T, RestTy...>
		static ToTy convert(auto&& value_)
		{
			static_assert(std::is_same_v < std::remove_cvref_t<decltype(value_)>, OneOf<T, RestTy...>>, "Expected OneOf<T, RestTy...>.");
			return esb::converter_to_from_one_of<ToTy, OneOf<T, RestTy...> >::convert(std::forward<decltype(value_)>(value_));
		}
	};
}



namespace esb
{
	template<typename ToTy, typename... FromTs>
	ToTy convert_to(FromTs&&... values_) {
		return converter_to<ToTy, FromTs&&...>::convert(std::forward<FromTs>(values_)...);
	}
}


namespace esb
{
	// Для конвертирования "в-аргумент" разрешаем по-умолчанию все что может с++ конвертировать.
	// здесь мы не боимся временных значений (&&) и т.п., т.к. "в рамках вычисления полного выражения" все должно жить Ок.
	// ПС. При использовании не по назначению учитывайте!
	template<typename TYPE_TO_, typename TYPE_FROM_>
		requires (is_convert_to_contained_convertable<TYPE_TO_, TYPE_FROM_>)
	struct esb::converter_to_arg<TYPE_TO_, TYPE_FROM_>
	{
		static TYPE_TO_	convert(TYPE_FROM_ value_)
		{
			return static_cast<TYPE_TO_>(value_.GetContained().Data());
		}
	};


	// Также разрешаем "естественное" конвертирование в указатели
	// 
	//TODO	сделано на std::is_same_v, а не с учетом возможного статик-кастинга к базовым(наследникам)
	//		(там все плохо...)	плюс те ^^^ кастинги работают только с -contained-, а еще бывают и обычные типы - Numeric или тупо int !!!
	// 
	//NOTE	Пытаемся сохранять константность интерфейса
	//		- две специализации - не-конст и конст - в-не-конст от просто UndefOr<TYPE_FROM_>&, а в конст только от const UndefOr<TYPE_FROM_>&
	//		(это в общем с++ ненужно, т.к. не-конст указатель в конст и так преобразуется, но для наглядности нам надо. а то все за convert_to прячется..)
	//
	template<typename TYPE_TO_, typename TYPE_FROM_>
		requires (std::is_pointer_v<TYPE_TO_>&& std::is_same_v<std::remove_const_t<std::remove_pointer_t<TYPE_TO_>>, typename TYPE_FROM_::ContainedDataType>)
	struct esb::converter_to_arg<TYPE_TO_, UndefOr<TYPE_FROM_>>
	{
		static TYPE_TO_	convert(UndefOr<TYPE_FROM_>& value_) requires not std::is_const_v<std::remove_pointer_t<TYPE_TO_>>
		{
			return (value_.is<Undef>()) ? nullptr : &value_.as<TYPE_FROM_>().GetContained().Data();
		}
		static TYPE_TO_	convert(const UndefOr<TYPE_FROM_>& value_)  requires std::is_const_v<std::remove_pointer_t<TYPE_TO_>>
		{
			return (value_.is<Undef>()) ? nullptr : &value_.as<TYPE_FROM_>().GetContained().Data();
		}
	};


	//TODO	Полная копия converter_to_from_one_of, 
	//		за исключением что для преобразования элемента вызывается convert_to_arg, а не convert_to
	//		Когда "приработается" нужно будет "привести к одному знаменателю"..
	template<typename ToTy, typename... FromTs>
	struct converter_to_arg_from_one_of : converter_unavailable_tag	{ };
	template<typename ToTy, typename T>
	struct esb::converter_to_arg_from_one_of<ToTy, OneOfValue<T> >
	{
		static ToTy convert(auto&& value_)
		{
			static_assert(std::is_same_v < std::remove_cvref_t<decltype(value_)>, OneOf<T>>, "Expected OneOf<T>.");
			//NOTE	value_.make<T>() дает нам && (естесственно - make дает копию), соответственно convert_to на голом месте получает &&, 
			//		что не отражает действительного характера value_ - это только копия &&, но она копия value_, которая живет своей жизнью
			//		Поэтому, если value_ временный, то и преобразуем напрямую (от временной копии),
			//		иначе преобразуем через через промежуточный lvalue
			//NOTE	И здесь мы обычно ловим невозможность преобразование временного объекта в ссылку на его данные 
			//		то есть потенциальное создание висячей ссылки.
			//		Если получение ссылки на данные от временного объекта безопасно (в контексте вычисления полного выражения)
			//		то нужно это разрешать отдельно.
			if constexpr (std::is_rvalue_reference_v<decltype(value_)>)
				return convert_to_arg<ToTy>(value_.make<T>());
			else
			{
				decltype(auto) v = value_.make<T>();
				return convert_to_arg<ToTy>(v);
			}
		}
	};
	template<typename ToTy, typename T, typename... RestTy>
	struct esb::converter_to_arg_from_one_of<ToTy, OneOfValue<T, RestTy...> >
	{
		// Ожидаем auto&&, но типа OneOfValue<T, RestTy...>
		static ToTy convert(auto&& value_)
		{
			static_assert(std::is_same_v < std::remove_cvref_t<decltype(value_)>, OneOf<T, RestTy...>>, "Expected OneOf<T, RestTy...>.");
			if (decltype(auto) t_val = value_.make_if<T>())
				return convert_to_arg<ToTy>(*t_val);
			else
				return esb::converter_to_arg_from_one_of<ToTy, OneOf<RestTy...> >::convert(static_cast_one_of_to_subset<OneOf<RestTy...>>(std::forward<decltype(value_)>(value_)));
		}
	};
	template<typename ToTy, typename T, typename... RestTy>
		requires (not std::is_same_v<T, Undef>)
	struct esb::converter_to_arg<ToTy, OneOfValue<T, RestTy...> >
	{
		// Ожидаем auto&&, но типа OneOfValue<T, RestTy...>
		static ToTy convert(auto&& value_)
		{
			static_assert(std::is_same_v < std::remove_cvref_t<decltype(value_)>, OneOf<T, RestTy...>>, "Expected OneOf<T, RestTy...>.");
			return esb::converter_to_arg_from_one_of<ToTy, OneOf<T, RestTy...> >::convert(std::forward<decltype(value_)>(value_));
		}
	};


	template<typename ToTy, typename... FromTs>
	constexpr inline bool is_convert_to_avalable_for_arg = is_convert_to_avalable_for_arg_special<ToTy, FromTs...> || is_convert_to_avalable<ToTy, FromTs...>;

	template<typename ToTy>
	inline constexpr bool is_convert_to_avalable_for_arg_from_IValuePtr = is_convert_to_avalable_for_arg<ToTy, IValuePtr&&>;

	//NOTE	относительно 'decltype(auto)', а не 'ToTy'
	//		(и также почему не снимаются спецификаторы с ToTy)
	//		convert_to_arg может возвращать не строго то, что ему заказали, но приемлемое для аргумента
	//		(см. esb::converter_to_arg<std::string const&...> в esb_stl_string.h)
	template<typename ToTy, typename... FromTs>
	decltype(auto) convert_to_arg(FromTs&&... values_)
	{
		if constexpr (is_convert_to_avalable_for_arg_special<ToTy, std::remove_cvref_t<FromTs>...>)
			return converter_to_arg<ToTy, std::remove_cvref_t<FromTs>...>::convert(std::forward<FromTs>(values_)...);
		else
			return convert_to<ToTy>(std::forward<FromTs>(values_)...);
	}
}


namespace esb
{
	template<typename CppElemTy, typename EsbElemTy>
	std::vector<CppElemTy> convert_to_arg_vector(const Array& array_)
	{
		std::vector<CppElemTy> vector;
		vector.reserve(array_.Size());
		// Array - это фактически массив Arbitrary, поэтому здесь var - это Arbitrary (как-то получается...)
		for (const auto& array_elem_arbitrary : array_) {
			//TODO	Тупо и явно написано "для врапперов".
			//		Нужно доработать "для всех допустимых convert_to_arg", т.к. мы сами convert_to_arg
			vector.push_back(array_elem_arbitrary.make<EsbElemTy>().GetContained().Data());
		}
		return vector;
	}
}


namespace esb
{
	template<typename TYPE_TO_, typename... FromTs>
		requires (is_convert_to_create_any_way_avalable<TYPE_TO_, FromTs...>)
	struct esb::converter_to_ret<TYPE_TO_, FromTs...>
	{
		//NOTE	Этот шаблон нужен чтобы CreateArgsTs&& были действительно "форвард ссылками" и мы их могли сделать std::forward
		//		(потому как FromTs&& уже не будут "форвард ссылками")
		//TODO	Но для порядка надо убедиться, что CreateArgsTs... это то же самое, что и FromTs...
		//
		template<typename... CreateArgsTs>
		static TYPE_TO_	convert(CreateArgsTs&&... args_)
		{
			return TYPE_TO_::Create(std::forward<CreateArgsTs>(args_)...);
		}
	};

	//TODO	Сыро, не тестировано на всякие const/&&
	//		особенно обратить внимание при встраивании конвертирования OneOf<>, т.к. UndefOr - это OneOf<>
	//operations_research::sat::ConstraintProto* -> [this::]ConstraintProto
	template<typename TYPE_TO_, typename FromTyPtr>
		requires ( std::is_pointer_v<FromTyPtr> && std::is_same_v<typename TYPE_TO_::ContainedDataType, std::remove_pointer_t<FromTyPtr>> )
	struct esb::converter_to_ret<UndefOr<TYPE_TO_>, FromTyPtr>
	{
		static UndefOr<TYPE_TO_> convert(FromTyPtr arg_)
		{
			if (arg_)
				return make_undef_or<TYPE_TO_>(convert_to_ret<TYPE_TO_>(*arg_));
			else
				return make_undef_or<TYPE_TO_>();
		}
	};
	//NOTE	ESB_OWNER_HANDLER_T в специализации нужно проставлять именно без всяких &&
	//		т.к. convert_to_ret делает converter_to_ret<ToTy, FromTs...> т.е. именно от типа без всяких decltype(std::forward<>)...
	//		и is_convert_to_avalable_for_ret_special<ToTy, FromTs...> также вызывается.
	//		если в специализации сделать ESB_OWNER_HANDLER_T&& то она просто не находится
	template<typename TYPE_TO_, typename FromTyPtr>
		requires (std::is_pointer_v<FromTyPtr> && std::is_same_v<typename TYPE_TO_::ContainedDataType, std::remove_pointer_t<FromTyPtr>>)
	struct esb::converter_to_ret<UndefOr<TYPE_TO_>, ESB_OWNER_HANDLER_T, FromTyPtr>
	{
		static UndefOr<TYPE_TO_> convert(ESB_THIS_ENTITY_HANDLER_, FromTyPtr arg_)
		{
			if (arg_)
				return make_undef_or<TYPE_TO_>(convert_to_ret<TYPE_TO_>(ESB_MOVE_ENTITY_HANDLER_, *arg_));
			else
				return make_undef_or<TYPE_TO_>();
		}
	};


	//TODO	Возможно сделать еще converter_to_ret_from_one_of при возникновении необходимости.
	//		пока таковой не видится, т.к. дефолтный converter_to_from_one_of вроде все задачи закрывает

	template<typename ToTy, typename... FromTs>
	constexpr inline bool is_convert_to_avalable_for_ret = is_convert_to_avalable_for_ret_special<ToTy, FromTs...> || is_convert_to_avalable<ToTy, FromTs...>;

	//TODO	Допустимое конвертирование для возврата значения
	template<typename ToTy, typename... FromTs>
	ToTy convert_to_ret(FromTs&&... values_) {
		if constexpr (is_convert_to_avalable_for_ret_special<ToTy, FromTs...>)
			return converter_to_ret<ToTy, FromTs...>::convert(std::forward<FromTs>(values_)...);
		else
			return convert_to<ToTy>(std::forward<FromTs>(values_)...);
	}
}






namespace esb
{
	//TODO	namespace _internal {

	// нам нужно из IVariable* сделать приемлемый в с++ класс
	//	это
	//		1) если T это есб-класс, то делаем через check_and_make_from_var
	//		2) если Т это натив-с++ класс, то .. у нас должно быть зарегестрировано (фундаментальное)
	//			или как еще преобразование в натив-с++ класс-из-IValuePtr&& 
	//			т.к. от IVariable* мы через IVariable_GetValue получаем IValuePtr&&
	// вызывается это в контексте корверт-ту-арг
	//	то есть преобразование возможно если is_convert_to_avalable_for_arg<T, IValuePtr&&>
	template<class T>
	decltype(auto) check_and_make_from_var_to_arg(const IVariable* pvar_)
	{
		if constexpr (is_esb_class<T>) 
		{
			return check_and_make_from_var<T>(pvar_);
		}
		else
		{
			static_assert(is_convert_to_avalable_for_arg_from_IValuePtr<T>, "No conversion avalable for check_and_make_from_var_to_arg");
			
			ESB_ASSERT(pvar_);
			if(pvar_ == nullptr)
				ESL_THROW_UNEXPECTED_VARIABLE_RET_NULL();

			return convert_to_arg<T>(IVariable_GetValue(*pvar_));
		}
	}


	//NOTE	std::is_convertable проверяет только implicit conversion
	//		поэтому нам надо сделать проверку на explicit conversion отдельно
	//NOTE	-> std::convertible_to<ToTy> нужна для нюансов с константностью типа static_cast<const int>(Numeric) -> const int
	//		т.к. static_cast<const int>(Numeric) возвращает int, что не то же самое что и const int
	template<typename ToTy, typename FromTy>
	inline constexpr bool is_convertable_with_static_cast = requires(FromTy v)
	{
		{ static_cast<ToTy>(v) } -> std::convertible_to<ToTy>;
	};

	//}	//namespace esb-_internal


	namespace _internal
	{
		inline void variable_set_value(IVariable& ret_, IValuePtr&& res_) {
			IVariable_SetValue(ret_, std::move(res_));
		}
		template<class RetT>
		inline void variable_set_value(IVariable& ret_, RetT&& res_) {
			variable_set_value(ret_, get_interface(std::move(res_)));
		}
		template<class RetT>
		inline void variable_set_value(IVariable* pret_, RetT&& res_) {
			assert(pret_);
			if (pret_) 
			{
				if constexpr (is_esb_class<std::remove_cvref_t<RetT>>)
					return variable_set_value(*pret_, std::move(res_));
				else
				{
					static_assert(is_convert_to_avalable_for_ret<IValuePtr, decltype(res_)>, "No conversion avalable for IVariable_SetValue");
					return variable_set_value(*pret_, convert_to_ret<IValuePtr>(std::forward<RetT>(res_)));
				}
			}
				
			ESL_THROW_UNEXPECTED_VARIABLE_RET_NULL();
		}



		//TODO	механизмы преобазования в-из с++ типов нужно как-то формализовать и расширить
		//		наверное нужно взять за основу модель std::swap и сделать T esb::convert_to<T>(IValuePtr&&) с подмножеством convert_to<IValuePtr>
		// 
		// Данный метод используется для получения элемента контейнера в есб-виде, т.е. унаследованном от Value 
		// При этом в контейнере может храниться как собственно Value-унаследованный тип (нр. Numeric), так и некие compatable_cpp_type_t - типы
		// для которых известно, что от них можно создать Value-унаследованный тип 
		// (нр. int, но мы знаем что ему соответствует Numeric и у Numeric есть конструктор от int)



//NOTE	Это не правильно вслепую назначать элементу, которому нужен владелец, владельцем коллекцию где он находится
//		Мало ли ссылки на какие данные содержит элемент и при чем здесь с++ контейнер куда его добавили
//		Если с++ элементу нужен владелец для того чтобы гарантировано удерживать свои данные, 
//TODO	-> то должны быть какие-то прозрачные механизмы это сделать
//
#define ESB_ALLOW_COLLECTION_BLIND_OWNER	0


		// Вызывается из esb_extr::ExEnumeratorForContainer::GetValueAndMoveNext
		//TODO	ExEnumeratorForContainer совершенно не знает есб-валюе контейнер, который енумерует
		//		он знает только с++ тип контейнера и IEntityPtr	m_container_handler
		//		поэтому он может делать только "слепое" преобразование с++-тип -> IValuePtr
		//		на основе предопределнных convert_to
		//		Правильно было-бы, чтобы ExEnumeratorForContainer знал о враппер-типах
		//		как TypeDescriptorImplement_IxRO/RW знают о возможном переопределении враппером 
		//		ESB_CONTAINED_IXCOLLECTION_GETVALUE/ESB_CONTAINED_IXCOLLECTION_SETVALUE
		//		и в этом случае вызывал типа ESB_CONTAINED_IXCOLLECTION_ENUM_GetValueAndMoveNext
		template<typename T>
		IValuePtr convert_to_ret_for_Enumerator_GetValue(const T& value_, const ESB_OWNER_HANDLER_T& owner_) {
			if constexpr (is_esb_class<T>)
				return get_interface(value_);
			else
			{
				if constexpr (is_convert_to_avalable_with_owner<IValuePtr, const T&>)
				{
#if !defined(ESB_ALLOW_COLLECTION_BLIND_OWNER) || !ESB_ALLOW_COLLECTION_BLIND_OWNER
					static_assert(false, "Not allowed use collection as blind owner for element.");
#endif
					return convert_to<IValuePtr>(ESB_OWNER_HANDLER_T{ owner_ }, value_);
				}
				else
				{
					static_assert(is_convert_to_avalable_for_ret<IValuePtr, const T&>, "No conversion in convert_to_ret_for_Enumerator_GetValue");
					return convert_to_ret<IValuePtr>(value_);
				}
			}
		}
		//NOTE	Специализация просто для примера, что в отношении некоторых типов можно специализировать
		template<>
		inline IValuePtr convert_to_ret_for_Enumerator_GetValue<bool>(const bool& value_, const ESB_OWNER_HANDLER_T& owner_) {
			return create_boolean_value(value_);
		}





		// вызывается из IxCollection_GetValue для типов-контейнеров
		// которым не указано через ESB_CONTAINED_IXCOLLECTION_GETVALUE 
		// как преобразовывать тип содержащийся в контейнере в 1С-IValuePtr
		// (в слепую перебираем возможные варианты преобразования)
		template<typename ExValueTy, typename CollectionElemTy>
		IValuePtr convert_to_ret_for_IxCollection_GetValue(CollectionElemTy&& value_, ExValueBase& owner_) 
		{
			if constexpr (is_esb_class<CollectionElemTy>)
				return get_interface(value_);
			else
			{
				if constexpr (is_convert_to_avalable_with_owner<IValuePtr, decltype(value_)>)
				{
#if !defined(ESB_ALLOW_COLLECTION_BLIND_OWNER) || !ESB_ALLOW_COLLECTION_BLIND_OWNER
					static_assert(false, "Not allowed use collection as blind owner for element.");
#endif
					return convert_to<IValuePtr>(query_owner_handler<ExValueTy>(owner_), value_);
				}
				else 
				{
					static_assert(is_convert_to_avalable_for_ret<IValuePtr, decltype(value_)>, "No conversion in convert_to_ret_for_IxCollection_GetValue");
					return convert_to_ret<IValuePtr>(value_);
				}
			}
		}

		// вызовется из IxCollection_SetValue
		// 
		// принимает IValuePtr из которого нужно попробовать получить тип T
		// то есть это либо 
		// - если тип Т это есб-тип (унаследованный от Value), 
		//		то пытаемся создать этот объект (будет рантайм проверка типа в check_and_make<T>)
		// - иначе значит нас просят преобразоваться в некоторый с++ тип T
		//		у нас или должно быть зарегистрировано преобразование convert_to
		//		или тип должен быть зарегистрирован как compatable_cpp_type_t
		//			и мы пытаемся создать соответствующий совместимый есб-класс 
		//			и пытаемся static_cast преобразовать его в требуемый Т
		template<typename ToTy>
		decltype(auto) convert_to_arg_for_IxCollection_SetValue(IValuePtr&& value_interface_)
		{
			if constexpr (is_esb_class<ToTy>)
				return check_and_make<ToTy>(std::move(value_interface_));
			else 
			{
				static_assert(is_convert_to_avalable_for_arg_from_IValuePtr<ToTy>, "No conversion in convert_to_arg_for_IxCollection_SetValue");
				return convert_to_arg<ToTy>(std::move(value_interface_));
			}				
		}

	}	//namespace esb-_internal
}


//NOTE	Ниже функциональность относящаяся к дефолтной конвертации для 1С В-Булево, В-Число, В-Строку, В-Дату
//		Пытаемся "любыми не запрещенными законом" способами предоставить эту функциональность
//TODO	В 1c_api_native.h есть методы эмулирующие 1С ToBoolean, ToNumeric, ToString, ToDate
//		их реализация несколько устарела - нужно доработать и опереться сюда.
//
namespace esb::_internal 
{

//TODO	Этот блок относится к "правильному" конструированию врапперов и заслуживает выделения в отдельный esb_base_wrap.h
//		который должен подключаться ДО esb_base_conv.h т.к. мы выше тоже много опираемся на ESB_CONTAINED_DATA и т.п.
//		НУЖНО переразобрать..
//
	template<class T>
	constexpr bool is_contained_has_Data = (requires(T * v) { v->ESB_CONTAINED_DATA(); });

#define ESB_DECLARE_CONTAINED_DATA_FIELD(DATA_TYPE_, DATA_FIELD_NAME_)										\
	DATA_TYPE_ DATA_FIELD_NAME_;																			\
	public:																									\
	using ESB_CONTAINED_DATA_T = DATA_TYPE_;


#define ESB_DECLARE_CONTAINED_DATA_ACCESSOR_RO(DATA_FIELD_NAME_)											\
	const ESB_CONTAINED_DATA_T&	ESB_CONTAINED_DATA() const&		{ return DATA_FIELD_NAME_; }

#define ESB_DECLARE_CONTAINED_DATA_ACCESSOR_RW(DATA_FIELD_NAME_)											\
	ESB_DECLARE_CONTAINED_DATA_ACCESSOR_RO(DATA_FIELD_NAME_)												\
	ESB_CONTAINED_DATA_T&  ESB_CONTAINED_DATA() &				{ return DATA_FIELD_NAME_; }				\
	ESB_CONTAINED_DATA_T&& ESB_CONTAINED_DATA() &&				{ return std::move(DATA_FIELD_NAME_); }


#define ESB_DECLARE_CONTAINED_DATA_RO(DATA_TYPE_, DATA_FIELD_NAME_)											\
	ESB_DECLARE_CONTAINED_DATA_FIELD(DATA_TYPE_, DATA_FIELD_NAME_)											\
	ESB_DECLARE_CONTAINED_DATA_ACCESSOR_RO(DATA_FIELD_NAME_)

#define ESB_DECLARE_CONTAINED_DATA_RW(DATA_TYPE_, DATA_FIELD_NAME_)											\
	ESB_DECLARE_CONTAINED_DATA_FIELD(DATA_TYPE_, DATA_FIELD_NAME_)											\
	ESB_DECLARE_CONTAINED_DATA_ACCESSOR_RW(DATA_FIELD_NAME_)


	template<bool, typename ContainedT>
	struct contained_data_accessor_const {
		using type = std::remove_reference_t< decltype(std::declval<const ContainedT>().ESB_CONTAINED_DATA()) >;
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
		using type = std::remove_reference_t< decltype(std::declval<ContainedT>().ESB_CONTAINED_DATA()) >;
	};
	template<typename ContainedT>
	struct contained_data_accessor<false, ContainedT> {
		using type = void;
	};
	template<typename ContainedT>
	using contained_data_accessor_t = contained_data_accessor<is_contained_has_Data<ContainedT>, ContainedT>::type;

	template<bool, typename ContainedT>
	struct contained_data_data_type {};
	template<typename ContainedT>
	struct contained_data_data_type<true, ContainedT> {	using type = typename ContainedT::DataType;	};
	template<typename ContainedT>
	using contained_data_data_type_t = typename contained_data_data_type<is_contained_has_Data<ContainedT>, ContainedT>::type;
//
//TODO	end-of esb_base_wrap.h
//////////////////////////////////////////////////////////////////////////////


//NOTE	Конвертацию-по-умолчанию-в-интерфейсе класса можно принудительно отключить для какого либо типа
//		объявив специализацию этой структуры (или лучше использовать один из макросов)
//NOTE	Запрет касается и custom методов ESB_CONVERTER_METH_ToBoolean и т.п.
//		Разрешено только то, что не запрещено..
//		ПС. custom-методы могут быть в шаблонах и быть не желательны как-то индивидуально..
//
//NOTE	Запрет не относится к реализации методов ExtConvertToBoolean и т.п.
//		- только запрет их использования в интерфейсе объекта (см. esb_ext.h)
//
	template<typename DataTy, typename ToEsbTy>
	struct wrapper_conversion_auto_deny : std::false_type {};
	//
	template<typename DataTy>
	inline constexpr bool is_wrapper_conversion_auto_deny_ToBoolean = wrapper_conversion_auto_deny<DataTy, Boolean>::value;
	template<typename DataTy>
	inline constexpr bool is_wrapper_conversion_auto_deny_ToNumeric = wrapper_conversion_auto_deny<DataTy, Numeric>::value;
	template<typename DataTy>
	inline constexpr bool is_wrapper_conversion_auto_deny_ToString	= wrapper_conversion_auto_deny<DataTy, String>::value;
	template<typename DataTy>
	inline constexpr bool is_wrapper_conversion_auto_deny_ToDate	= wrapper_conversion_auto_deny<DataTy, DateTime>::value;

//NOTE	Запрет работает "на индивидуальной основе" для каждого типа-с-модификатором отдельно
//		то есть для 'int' и 'int&' можно запретить, а для 'const int&' оставить
//		чтобы запретить все варианты типа нужно сделать все специализации
// 
#define ESB_WRAPPER_CONVERSION_AUTO_DENY_FOR_(DATA_TY_, TO_ESB_TY_)		\
	template<> struct wrapper_conversion_auto_deny<DATA_TY_, TO_ESB_TY_> : std::true_type {}

#define ESB_WRAPPER_CONVERSION_AUTO_DENY_FOR_ALL(DATA_TY_, TO_ESB_TY_)	\
	ESB_WRAPPER_CONVERSION_AUTO_DENY_FOR_(DATA_TY_, TO_ESB_TY_)			\
	ESB_WRAPPER_CONVERSION_AUTO_DENY_FOR_(DATA_TY_ &, TO_ESB_TY_)		\
	ESB_WRAPPER_CONVERSION_AUTO_DENY_FOR_(const DATA_TY_, TO_ESB_TY_)	\
	ESB_WRAPPER_CONVERSION_AUTO_DENY_FOR_(const DATA_TY_ &, TO_ESB_TY_)

#define ESB_WRAPPER_CONVERSION_AUTO_DENY_FOR_BOOLEAN(DATA_TY_)		ESB_WRAPPER_CONVERSION_AUTO_DENY_FOR_ALL(DATA_TY_, Boolean)
#define ESB_WRAPPER_CONVERSION_AUTO_DENY_FOR_NUMERIC(DATA_TY_)		ESB_WRAPPER_CONVERSION_AUTO_DENY_FOR_ALL(DATA_TY_, Numeric)
#define ESB_WRAPPER_CONVERSION_AUTO_DENY_FOR_STRING(DATA_TY_)		ESB_WRAPPER_CONVERSION_AUTO_DENY_FOR_ALL(DATA_TY_, String)
#define ESB_WRAPPER_CONVERSION_AUTO_DENY_FOR_DATE(DATA_TY_)			ESB_WRAPPER_CONVERSION_AUTO_DENY_FOR_ALL(DATA_TY_, DateTime)


#define ESB_CONVERTER_METH_ToBoolean	ToBoolean
#define ESB_CONVERTER_METH_ToNumeric	ToNumeric
#define ESB_CONVERTER_METH_ToString		ToString
#define ESB_CONVERTER_METH_ToDate		ToDate

//NOTE	We don't check return type - if we have meth with magic name return type MUST BE corresponding or ERROR. 
	template<class T>
	inline constexpr bool  has_meth_ToBoolean = requires(T & t)		{ t.ESB_CONVERTER_METH_ToBoolean(); };
	template<class T>
	inline constexpr bool  has_meth_ToNumeric = (requires(T & t)	{ t.ESB_CONVERTER_METH_ToNumeric(); });
	template<class T>
	inline constexpr bool  has_meth_ToString = requires(T & t)		{ t.ESB_CONVERTER_METH_ToString(); };
	template<class T>
	inline constexpr bool  has_meth_ToDate = requires(T & t)		{ t.ESB_CONVERTER_METH_ToDate(); };


	// Value Conversion
	//////////////////////////////////////////////////////////////////////////////
	//
	template<class T, class EsbTypeTo>
	inline constexpr bool ext_has_type_to = (std::is_constructible_v<EsbTypeTo, T >);

	template<class T>
	inline constexpr bool ext_has_type_toBoolean = ext_has_type_to<T, Boolean>;
	template<class T>
	inline constexpr bool ext_has_type_toNumeric = ext_has_type_to<T, Numeric>;
	template<class T>
	inline constexpr bool ext_has_type_toDate = ext_has_type_to<T, DateTime>;
	template<class T>
	inline constexpr bool ext_has_type_toStringCreate = ext_has_type_to<T, String>;
	template<class T>
	inline constexpr bool ext_has_type_toStringConvert = (requires(T & t) { to_ustring(t); });
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
	inline constexpr bool ext_has_data_toStringConvert = (is_contained_has_Data<T> && requires(T & t) { to_ustring(t.ESB_CONTAINED_DATA()); });
	template<class T>
	inline constexpr bool ext_has_data_toString = ext_has_data_toStringCreate<T> || ext_has_data_toStringConvert<T>;


	template<class T>
	inline constexpr bool  ext_has_toBoolean = ext_has_type_toBoolean<T> || has_meth_ToBoolean<T> || ext_has_data_toBoolean<T> || is_convert_to_avalable_for_ret<Boolean, T>;
	template<class T>
	inline constexpr bool  ext_has_toNumeric = ext_has_type_toNumeric<T> || has_meth_ToNumeric<T> || ext_has_data_toNumeric<T> || is_convert_to_avalable_for_ret<Numeric, T>;
	template<class T>
	inline constexpr bool  ext_has_toString = ext_has_type_toString<T> || has_meth_ToString<T> || ext_has_data_toString<T> || is_convert_to_avalable_for_ret<String, T>;
	template<class T>
	inline constexpr bool  ext_has_toDate = ext_has_type_toDate<T> || has_meth_ToDate<T> || ext_has_data_toDate<T> || is_convert_to_avalable_for_ret<DateTime, T>;


	// во первых мы проверяем на наличие пользовательского метода конвертации ToXXX. Таким образом пользовательский способ "перекрывает" иные возможные.
	template<class ExtValueT>
	Boolean ExtConvertToBoolean(ExtValueT& value_) {
		if constexpr (has_meth_ToBoolean<ExtValueT>)
			return value_.ESB_CONVERTER_METH_ToBoolean();
		else if constexpr (is_convert_to_avalable_for_ret<Boolean, decltype(value_)>)
			return convert_to_ret<Boolean>(value_);
		else if constexpr (ext_has_type_toBoolean<ExtValueT>)
			return Boolean{ static_cast<bool>(value_) };
		else if constexpr (ext_has_data_toBoolean<ExtValueT>)
			return Boolean{ static_cast<bool>(value_.ESB_CONTAINED_DATA()) };	//error C2398: Элемент "1": преобразование из "RawDataT" в "bool" требует сужающего 
		else
			static_assert(always_false_v<ExtValueT>, "Value not convertable to Boolean any way!");
	}


	//NOTE	Чтобы ToNumeric работал его нужно объявлять const - Numeric ToNumeric() const
	// 
	//TODO	Не помню зачем сделано это ограничение, но по сути оно верное - примитивная конвертация не должна менять объект
	//		Но! 
	//		Тогда нам нужна еще одна конвертация при случае "сложных врапперов", которые могут хотеть кэшировать результат конвертации
	//TODO	Тут с "константностью и проч.ссылками" конь не валялся !!!
	//		1) Все ext_has_... рассчитыват на "чисто-тип" и ничего не понимают за модификаторы
	//		2) ExtValueConverter спрашивает за "чисто-тип", а на конвертацию подает const data_t
	//			(он еще НЕ ЗНАЕТ, что ContainedDataType может быть типа 'const string&')
	//		ДОРАБОТАТЬ надо чтобы вся эта машинерия с модификаторами корректно работала !!!
	//		И ДА.	custom-ToXXX должен мочь быть НЕ-КОНСТ и 
	//		И		ExtValueConverter должен сначала проверять наличие НЕ-КОНСТ-конвертера, а уж потом КОНСТ
	// 
	//TODO	ТАКЖЕ. Нужно примитивный магический дефайн ToNumeric сделать посложнее - типа IConverter_ToNumeric
	//		Или еще лучше IConverterTo<..> со специализациями Numeric, String и т.п. возможными
	// 
	//NOTE	Наличие конвертации к Numeric для 1С не означает возможность конвертации к Строке
	//		Если хочется чтобы объект-типа-число и к строке конвертировался, то нужно делать отдельно ToString
	//
	//TODO	Сейчас здесь значения енумов автоматом ни в число, ни в строку не преобразуются (по всем правилам с++)
	//		а во враппере над значениями енумов объявлены методы explisit преобразования ESB_CONVERTER_METH_ToNumeric и ESB_CONVERTER_METH_ToString
	//		Возможно нужно добавить здесь авто-преобразования, а там убрать. 
	//		Или такое поведение сделать настраиваемым через ESB-опции компиляции.
	//		ПС. Там преобразуются только Врапперы, а здесь будет преобразовываться Все и Всегда. Что лучше?
	//
	template<class ExtValueT>
	Numeric ExtConvertToNumeric(ExtValueT& value_) {
		if constexpr (has_meth_ToNumeric<ExtValueT>)
			return value_.ESB_CONVERTER_METH_ToNumeric();
		else if constexpr (is_convert_to_avalable_for_ret<Numeric, decltype(value_)>)
			return convert_to_ret<Numeric>(value_);
		else if constexpr (ext_has_type_toNumeric<ExtValueT>)
			return Numeric{ value_ };
		else if constexpr (ext_has_data_toNumeric<ExtValueT>)
			return Numeric{ value_.ESB_CONTAINED_DATA() };
		else
			static_assert(always_false_v<ExtValueT>, "Value not convertable to Numeric any way!");
	}

	template<class ExtValueT>
	DateTime ExtConvertToDate(ExtValueT& value_) {
		if constexpr (has_meth_ToDate<ExtValueT>)
			return value_.ESB_CONVERTER_METH_ToDate();
		else if constexpr (is_convert_to_avalable_for_ret<DateTime, decltype(value_)>)
			return convert_to_ret<DateTime>(value_);
		else if constexpr (ext_has_type_toDate<ExtValueT>)
			return DateTime{ value_ };
		else if constexpr (ext_has_data_toDate<ExtValueT>)
			return DateTime{ value_.ESB_CONTAINED_DATA() };
		else
			static_assert(always_false_v<ExtValueT>, "Value not convertable to DateTime any way!");
	}

	template<class ExtValueT>
	String ExtConvertToString(ExtValueT& value_) {
		if constexpr (has_meth_ToString<ExtValueT>)
			return value_.ESB_CONVERTER_METH_ToString();
		else if constexpr (is_convert_to_avalable_for_ret<String, decltype(value_)>)
			return convert_to_ret<String>(value_);
		else if constexpr (ext_has_type_toStringCreate<ExtValueT>)
			return String{ value_ };
		else if constexpr (ext_has_type_toStringConvert<ExtValueT>)
			return String{ to_ustring(value_) };
		else if constexpr (ext_has_data_toStringCreate<ExtValueT>)
			return String{ value_.ESB_CONTAINED_DATA() };
		else if constexpr (ext_has_data_toStringConvert<ExtValueT>)
			return String{ to_ustring(value_.ESB_CONTAINED_DATA()) };
		else
			static_assert(always_false_v<ExtValueT>, "Value not convertable to String any way!");
	}

	template<typename DataTy>
	inline constexpr bool is_wrapper_interface_may_ToBoolean	= (not is_wrapper_conversion_auto_deny_ToBoolean<DataTy> and ext_has_toBoolean<DataTy>);
	template<typename DataTy>
	inline constexpr bool is_wrapper_interface_may_ToNumeric	= (not is_wrapper_conversion_auto_deny_ToNumeric<DataTy> and ext_has_toNumeric<DataTy>);
	template<typename DataTy>
	inline constexpr bool is_wrapper_interface_may_ToString		= (not is_wrapper_conversion_auto_deny_ToString<DataTy> and ext_has_toString<DataTy>);
	template<typename DataTy>
	inline constexpr bool is_wrapper_interface_may_ToDate		= (not is_wrapper_conversion_auto_deny_ToDate<DataTy> and ext_has_toDate<DataTy>);


// end Value Conversion
/////////////////////////////////////////////////////////////////////////////////////


	//TODO	Тесты всякие куда-то отдельно надо. и побольше..
	//
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

} //namespace esb::_internal


#endif // !ESB_BASE_CONV_H_
