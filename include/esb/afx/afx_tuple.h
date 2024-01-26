#pragma once
#ifndef AFX_TUPLE_H_
#define AFX_TUPLE_H_

#include <tuple>


//утилитка для проверки наличия
template <typename, typename>
struct is_tuple_contain_t {};
template <typename... TupleElemTs, typename TupleElemToMatchT>
struct is_tuple_contain_t<std::tuple<TupleElemTs...>, TupleElemToMatchT>
	: std::bool_constant<(std::is_same_v<TupleElemTs, TupleElemToMatchT> || ...)>
{};
template<typename TupleT, typename TupleElemToMatchT>
inline constexpr bool is_tuple_contain = is_tuple_contain_t<TupleT, TupleElemToMatchT>::value;


namespace afx
{
	//TODO	Нужно отвязаться от поля Pred2<Ts, ArgsTs...>::value и попытаться принимать в том числе и consteval-предикаты
	//		(модет быть и алгоритм поменять как-то...?)
	////////////////////////////////////////////////////////////////////
	template <typename, template <typename...> class, typename... ArgsT>
	struct tuple_filter;
	//
	template<typename... Ts, template <typename...> class Pred2, typename... ArgsTs>
	struct tuple_filter<std::tuple<Ts...>, Pred2, ArgsTs...>
	{
		using type =
			decltype
					(std::tuple_cat
						(
							std::declval
							<
								std::conditional_t
								<
									Pred2<Ts, ArgsTs...>::value, std::tuple<Ts>, std::tuple<>
								>
							>()...
						)
				);
	};
	//
	template <typename Tpl, template <typename...> class Pred, class... ArgTs>
	using tuple_filter_t = typename tuple_filter<Tpl, Pred, ArgTs...>::type;
	//////////////////////////////////////////////////////////////////////////
	//
#ifndef NDEBUG
	using x_test_tuple_org_t = std::tuple<int, bool, char, void, void>;
	//
	template<typename TupleElemT, typename SameTypeT>
	struct x_test_tuple_filter_is_same {
		static constexpr bool value = std::is_same_v<TupleElemT, SameTypeT>;
	};
	using test_tuple_flt1_t = tuple_filter_t< x_test_tuple_org_t, x_test_tuple_filter_is_same, void>;
	constexpr size_t x_test_tuple_flt1_size = std::tuple_size_v< test_tuple_flt1_t>;
	static_assert(x_test_tuple_flt1_size==2);
	//
	template<typename TupleElemT>
	struct x_test_tuple_filter_not_void {
		static constexpr bool value = !std::is_same_v<TupleElemT, void>;
	};
	using test_tuple_flt2_t = tuple_filter_t< x_test_tuple_org_t, x_test_tuple_filter_not_void>;
	constexpr size_t x_test_tuple_flt2_size = std::tuple_size_v< test_tuple_flt2_t>;
	static_assert(x_test_tuple_flt2_size == 3);
	///////////////////////////////////
#endif
}

#endif	//AFX_TUPLE_H_
