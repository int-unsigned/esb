#pragma once
#ifndef AFX_META_FIELDS_H_
#define AFX_META_FIELDS_H_

#include <tuple>
#include "afx_pp.h"
#include "afx_base.h"


template<typename>
struct data_member_type;
//
template<typename ClassT, typename DataT>
struct data_member_type<typename DataT ClassT::*> {
	using class_t = ClassT;
	using data_t = DataT;
};
template<typename ClassT, typename DataT>
struct data_member_type<typename DataT ClassT::* const> {
	using class_t = ClassT;
	using data_t = DataT;
};
template<typename T>
using data_member_t = data_member_type<T>::data_t;


template<typename>
struct meta_fields_to_data_tuple;
//
template<typename... Ts>
struct meta_fields_to_data_tuple< const std::tuple<Ts...> >
{
	using type = std::tuple< data_member_t<Ts> ...>;
};
template<typename... Ts>
struct meta_fields_to_data_tuple< std::tuple<Ts...> >
{
	using type = std::tuple< data_member_t<Ts> ...>;
};
//
template<typename T>
using meta_fields_to_data_tuple_t = meta_fields_to_data_tuple<T>::type;


template<typename> struct meta_fields : std::false_type
{};
//

#define META_DESCRIBE_TYPE_REST													\
		using fields_t		= std::remove_const_t< decltype(fields_) >;			\
		using data_tuple_t	= meta_fields_to_data_tuple_t<fields_t>;			\
		static constexpr size_t fields_count_ = std::tuple_size_v<fields_t>;	\
		static_assert(sizeof(type_t) == sizeof(data_tuple_t), "somewhat wrong");\
	}

#define PP_LIST_OF_ADDR1(T_, _1)				&T_::_1
#define PP_LIST_OF_ADDR2(T_, _1,_2)				&T_::_1,&T_::_2
#define PP_LIST_OF_ADDR3(T_, _1,_2,_3)			&T_::_1,&T_::_2,&T_::_3
#define PP_LIST_OF_ADDR4(T_, _1,_2,_3,_4)		&T_::_1,&T_::_2,&T_::_3,&T_::_4
#define PP_LIST_OF_ADDR5(T_, _1,_2,_3,_4,_5)	&T_::_1,&T_::_2,&T_::_3,&T_::_4,&T_::_5

#define META_DESCRIBE_TYPE_FIELDS_ADDR(_1)		&type_t::_1
#define META_DESCRIBE_TYPE_FIELDS_LIST(...)		PP_VARIADIC_APPLY(META_DESCRIBE_TYPE_FIELDS_ADDR, __VA_ARGS__)
#define META_DESCRIBE_TYPE_FIELDS(...)			\
	static constexpr auto	fields_ = std::make_tuple( META_DESCRIBE_TYPE_FIELDS_LIST(__VA_ARGS__) )


#define META_DESCRIBE_TYPE_FIELDS_LIST1(_1)		&type_t::_1
#define META_DESCRIBE_TYPE_FIELDS_LIST2(_1,_2)	PP_LIST_OF_ADDR ## 2 (type_t, _1,_2)

#define META_DESCRIBE_TYPE_FIELDS2(_1,_2)										\
	static constexpr auto	fields_ = std::make_tuple( META_DESCRIBE_TYPE_FIELDS_LIST2(_1,_2) )

#define META_DESCRIBE_TYPE_HEAD(TYP_)											\
	template<> struct meta_fields<TYP_> : std::true_type {						\
		using type_t = TYP_

#define META_DESCRIBE_TYPE(TYP_, ...)										\
	META_DESCRIBE_TYPE_HEAD(TYP_);											\
	META_DESCRIBE_TYPE_FIELDS(__VA_ARGS__);									\
	META_DESCRIBE_TYPE_REST

#define META_DESCRIBE_TYPE2(TYP_, _1,_2)	META_DESCRIBE_TYPE(TYP_, _1,_2)						

template<typename T>
inline constexpr bool has_meta_fields = meta_fields<T>::value;




#endif	//AFX_META_FIELDS_H_