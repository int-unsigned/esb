/*
	"ESB" is designed to create external native components for the 1C(tm) program.
	Copyright © 2023 IntUnsigned	(v8classes@gmail.com)
	"ESB" is free software under LGPLv2.1 license with essential restriction about binary part (view esb-license.txt)
	"ESB" IS DISTRIBUTED "AS-IS" WITHOUT ANY, EVEN THE IMPLIED, WARRANTY. IN NO EVENT SHALL THE AUTHORS OR THE COPYRIGHT OWNER BE LIABLE FOR ANY CLAIM, 
	DAMAGES OR OTHER LIABILITY ARISING OUT OF THE USE OF THIS SOFTWARE.
	THE AUTHOR DOES NOT GIVE ANY GUARANTEES AND DOES NOT BEAR ANY RESPONSIBILITY REGARDING THE USE OF 1C(tm) COMPONENTS PRODUCED USING THE ESB LIBRARY IN THE 1C(tm) ENVIRONMENT.
*/


#pragma once
#ifndef ESB_TRAITS_H
#define ESB_TRAITS_H

#include <type_traits>
#include <vector>

namespace esb 
{
	template<typename>
	inline constexpr bool always_false_v = false;


	template<typename T>
	inline constexpr bool is_const_reference_v = std::is_reference_v<T> && std::is_const_v<std::remove_reference_t<T>>;


	//TODO	Вспомогательное. 
	//		Недоделано для всего многообразия вариантов методов (см. std::is_member_function_pointer_v)
	template<typename T>
	struct is_const_member_function_pointer : std::false_type
	{
		static_assert(not std::is_same_v<T, std::nullptr_t>,	"nullptr is not member_function_pointer, to check aware nullptr use 'is_const_member_function_pointer_maybe_nullptr'.");
		static_assert(std::is_member_function_pointer_v<T>,		"Not a member_function_pointer, or implementation incomplete.");
	};
	template<typename RetTy, typename ClsTy, typename... ArgsTs>
	struct is_const_member_function_pointer<typename RetTy(ClsTy::*)(ArgsTs...) const> : std::true_type {};
	template<class R, class T, class... Args>
	struct is_const_member_function_pointer<R(T::*)(Args...) const&> : std::true_type {};
	template<class R, class T, class... Args>
	struct is_const_member_function_pointer<R(T::*)(Args...) const&&> : std::true_type {};
	template<class R, class T, class... Args>
	struct is_const_member_function_pointer<R(T::*)(Args..., ...) const> : std::true_type {};
	template<class R, class T, class... Args>
	struct is_const_member_function_pointer<R(T::*)(Args..., ...) const&> : std::true_type {};
	template<class R, class T, class... Args>
	struct is_const_member_function_pointer<R(T::*)(Args..., ...) const&&> : std::true_type {};
	//NOTE  remove_cv_t снимает модификаторы с самого указателя, не с сигнатуры метода (в std также делают)
	template<class T>
	inline constexpr bool is_const_member_function_pointer_v = is_const_member_function_pointer<std::remove_cv_t<T>>::value;

	template<typename T>
	struct is_const_member_function_pointer_maybe_nullptr : is_const_member_function_pointer<T> {};
	template<>
	struct is_const_member_function_pointer_maybe_nullptr<std::nullptr_t> : std::false_type {};
	//
	template<class T>
	inline constexpr bool is_const_member_function_pointer_maybe_nullptr_v = is_const_member_function_pointer_maybe_nullptr<std::remove_cv_t<T>>::value;


	template<typename T>
	struct member_function_retval
	{
		static_assert(std::is_member_function_pointer_v<T>, "Not a member_function_pointer, or implementation incomplete.");
	};
	template<typename R, typename T, typename... Args>
	struct member_function_retval<R(T::*)(Args...)> : std::type_identity<R> {};
	template<class R, class T, class... Args>
	struct member_function_retval<R(T::*)(Args...)&> : std::type_identity<R> {};
	template<class R, class T, class... Args>
	struct member_function_retval<R(T::*)(Args...)&&> : std::type_identity<R> {};
	template<class R, class T, class... Args>
	struct member_function_retval<R(T::*)(Args..., ...)> : std::type_identity<R> {};
	template<class R, class T, class... Args>
	struct member_function_retval<R(T::*)(Args..., ...)&> : std::type_identity<R> {};
	template<class R, class T, class... Args>
	struct member_function_retval<R(T::*)(Args..., ...)&&> : std::type_identity<R> {};
	//
	template<typename R, typename T, typename... Args>
	struct member_function_retval<R(T::*)(Args...) const> : std::type_identity<R> {};
	template<class R, class T, class... Args>
	struct member_function_retval<R(T::*)(Args...) const&> : std::type_identity<R> {};
	template<class R, class T, class... Args>
	struct member_function_retval<R(T::*)(Args...) const&&> : std::type_identity<R> {};
	template<class R, class T, class... Args>
	struct member_function_retval<R(T::*)(Args..., ...) const> : std::type_identity<R> {};
	template<class R, class T, class... Args>
	struct member_function_retval<R(T::*)(Args..., ...) const&> : std::type_identity<R> {};
	template<class R, class T, class... Args>
	struct member_function_retval<R(T::*)(Args..., ...) const&&> : std::type_identity<R> {};
	//
	template<typename T>
	using member_function_retval_t = typename member_function_retval<std::remove_cv_t<T>>::type;



	template<typename T>
	struct is_type_valtype_traits : std::bool_constant< std::is_fundamental_v<T> || std::is_enum_v<T> || std::is_empty_v<T> > {};
	template<typename CharTy>
	struct is_type_valtype_traits<std::basic_string<CharTy>> : std::bool_constant< true > {};
	template<typename T>
	struct is_type_valtype_traits<std::vector<T>> : std::bool_constant< true > {};
	//TODO дополнить
#define ESB_DECLARE_AS_VALUE_TYPE(...)						\
	template<>												\
	struct is_type_valtype_traits<__VA_ARGS__> : std::bool_constant< true > {};
#define ESB_DECLARE_AS_VALUE_TYPE_TEMPLATE(TEMPLATE_, ...)	\
	template<typename T>									\
	struct is_type_valtype_traits<TEMPLATE_<T>> : std::bool_constant< true > {};
	//
	template<typename T>
	inline constexpr bool is_type_valtype = is_type_valtype_traits<std::remove_const_t<T>>::value;
	//
	//
	template<typename T>
	struct is_type_reftype_traits : std::bool_constant< std::is_reference_v<T> && not is_type_valtype<T>  >
	{
		// сюда мы попадаем когда explicit специализации is_type_reftype_traits нет.
		//  - это допустимо если тип является ссылкой - тогда мы true
		//  - или он определяется как is_type_valtype - если определяется как is_type_valtype - мы false
		// иначе мы ругаемся и требуем чтобы явно было указано к какой категории относится этот тип
		static_assert(std::is_reference_v<T> || is_type_valtype<T>, "Not well known value types must be explicity declared as is_value_type or is_type_reftype_traits.");
	};
	template<typename CharTy>
	struct is_type_reftype_traits<std::basic_string_view<CharTy>> : std::bool_constant< true > {};
	template<typename Ty>
	struct is_type_reftype_traits<std::span<Ty>> : std::bool_constant< true > {};
	//TODO дополнить 
#define ESB_DECLARE_AS_REFERENCE_TYPE(...)					\
	template<>												\
	struct is_type_reftype_traits<__VA_ARGS__> : std::bool_constant< true > {};
#define ESB_DECLARE_AS_REFERENCE_TYPE_TEMPLATE(TEMPLATE_)	\
	template<typename T>									\
	struct is_type_reftype_traits<TEMPLATE_<T>> : std::bool_constant< true > {};
	//
	//NOTE	Специально названо несколько коряво т.к. варианты is_type_reftype_traits начинают на глаз путаться с std::is_reference_v
	template<typename T>
	inline constexpr bool is_type_reftype = is_type_reftype_traits< std::remove_const_t<T> >::value;


	//NOTE	У вектора точный тип элемента value_type = T (element_type нет), а у std-Спан точный тип элемента element_type = T (а value_type = std::remove_cv_t<T>)
	//		поэтому для получения точного типа элемента в контейнере мы сначала проверяем element_type
	template<typename Ty>
	inline constexpr bool has_element_type = requires { typename Ty::element_type; };
	//
	template<typename Ty, bool HasElementType>
	struct element_or_value_type : std::type_identity<typename Ty::element_type> {};
	template<typename Ty>
	struct element_or_value_type<Ty, false> : std::type_identity<typename Ty::value_type> {};
	template<typename Ty>
	using element_or_value_type_t = typename element_or_value_type<Ty, has_element_type<Ty>>::type;


	template <class Ty, class... Ts>
	constexpr bool is_same_as_any_of = (std::is_same_v<Ty, Ts> || ...);

	//NOTE	Как-бы в std такого треитса как "standard_integer" нет, 
	//		а по факту есть - все cmp_equal/cmp_greater и т.п. работают только с "standard_integer"
	//		ПС. Реализация честно содрана из ms-stl
	//		ЗДЕСЬ НЕТ - bool, (plain)char, char16_t, char32_t, char8_t и плавающих
	template <class Ty>
	inline constexpr bool is_standard_integer = is_same_as_any_of<std::remove_cv_t<Ty>, signed char, short, int, long, long long,
		unsigned char, unsigned short, unsigned int, unsigned long, unsigned long long>;

	//NOTE	Ну соответственно
	template <class Ty>
	inline constexpr bool is_standard_character = is_same_as_any_of< std::remove_cv_t<Ty>, char, wchar_t, char8_t, char16_t, char32_t>;
	using standard_character_min_t = char;
	using standard_character_max_t = char32_t;

} //namespace esb 

#endif //ESB_TRAITS_H