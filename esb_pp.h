#pragma once
#ifndef ESB_PP_H
#define ESB_PP_H



#define PP_EXPAND(X_)			X_


#define PP_CAT(A_, B_)			PP_CAT_IMPL(A_,B_)
#define PP_CAT_IMPL(A_, B_)		A_##B_


#define PP_STRINGIZE(X_)		PP_STRINGIZE_IMPL(X_)
#define PP_STRINGIZE_IMPL(X_)	#X_


#ifdef  _MSC_VER // Microsoft compilers
#define PP_VARIADIC_SIZE(...)			PP_VARIADIC_SIZE_IMPL(PP_VA_SIZE_ARGS(__VA_ARGS__))
#if !defined(_MSVC_TRADITIONAL) || _MSVC_TRADITIONAL
#define PP_VA_SIZE_ARGS(...)	_dummy_,  __VA_ARGS__
#else
#define PP_VA_SIZE_ARGS(...)	_dummy_   __VA_OPT__(,) __VA_ARGS__
#endif
#define PP_VARIADIC_SIZE_IMPL(...)		PP_EXPAND( PP_VARIADIC_SIZE_COUNTER(__VA_ARGS__,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0) )
#define PP_VARIADIC_SIZE_COUNTER(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,COUNT_,...)	COUNT_
#else
//#define PP_VA_SIZE(...)			PP_VA_SIZE_COUNTER(_dummy_,##__VA_ARGS__,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0)
//#define PP_VA_SIZE_COUNTER(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15_,_16,COUNT_,...)	COUNT_
#define PP_VARIADIC_SIZE(...)			PP_VA_SIZE_COUNTER(_dummy_ __VA_OPT__(,) __VA_ARGS__,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0)
#define PP_VA_SIZE_COUNTER(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15_,_16,COUNT_,...)	COUNT_
#endif

static_assert(PP_VARIADIC_SIZE() == 0, "!!! PP_VARIADIC_SIZE !!!!");
static_assert(PP_VARIADIC_SIZE(_1) == 1, "!!! PP_VARIADIC_SIZE !!!!");
static_assert(PP_VARIADIC_SIZE(_1, _2) == 2, "!!! PP_VARIADIC_SIZE !!!!");
static_assert(PP_VARIADIC_SIZE(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15) == 15, "!!! PP_VARIADIC_SIZE !!!!");



#define PP_VARIADIC_APPLY_0( M_)
#define PP_VARIADIC_APPLY_1( M_, _1)																																	  M_(_1)
#define PP_VARIADIC_APPLY_2( M_, _1,_2)														PP_VARIADIC_APPLY_1( M_, _1)												, M_(_2)
#define PP_VARIADIC_APPLY_3( M_, _1,_2,_3)													PP_VARIADIC_APPLY_2( M_, _1,_2)												, M_(_3)
#define PP_VARIADIC_APPLY_4( M_, _1,_2,_3,_4)												PP_VARIADIC_APPLY_3( M_, _1,_2,_3)											, M_(_4)
#define PP_VARIADIC_APPLY_5( M_, _1,_2,_3,_4,_5)											PP_VARIADIC_APPLY_4( M_, _1,_2,_3,_4)										, M_(_5)
#define PP_VARIADIC_APPLY_6( M_, _1,_2,_3,_4,_5,_6)											PP_VARIADIC_APPLY_5( M_, _1,_2,_3,_4,_5)									, M_(_6)
#define PP_VARIADIC_APPLY_7( M_, _1,_2,_3,_4,_5,_6,_7)										PP_VARIADIC_APPLY_6( M_, _1,_2,_3,_4,_5,_6)									, M_(_7)
#define PP_VARIADIC_APPLY_8( M_, _1,_2,_3,_4,_5,_6,_7,_8)									PP_VARIADIC_APPLY_7( M_, _1,_2,_3,_4,_5,_6,_7)								, M_(_8)
#define PP_VARIADIC_APPLY_9( M_, _1,_2,_3,_4,_5,_6,_7,_8,_9)								PP_VARIADIC_APPLY_8( M_, _1,_2,_3,_4,_5,_6,_7,_8)							, M_(_9)
#define PP_VARIADIC_APPLY_10(M_, _1,_2,_3,_4,_5,_6,_7,_8,_9,_10)							PP_VARIADIC_APPLY_9( M_, _1,_2,_3,_4,_5,_6,_7,_8,_9)						, M_(_10)
#define PP_VARIADIC_APPLY_11(M_, _1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11)						PP_VARIADIC_APPLY_10(M_, _1,_2,_3,_4,_5,_6,_7,_8,_9,_10)					, M_(_11)
#define PP_VARIADIC_APPLY_12(M_, _1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12)					PP_VARIADIC_APPLY_11(M_, _1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11)				, M_(_12)
#define PP_VARIADIC_APPLY_13(M_, _1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13)				PP_VARIADIC_APPLY_12(M_, _1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12)			, M_(_13)
#define PP_VARIADIC_APPLY_14(M_, _1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14)			PP_VARIADIC_APPLY_13(M_, _1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13)		, M_(_14)
#define PP_VARIADIC_APPLY_15(M_, _1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15)		PP_VARIADIC_APPLY_14(M_, _1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14)	, M_(_15)
#define PP_VARIADIC_APPLY_16(M_, _1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16)	PP_VARIADIC_APPLY_15(M_, _1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15), M_(_16)

#define PP_VARIADIC_APPLY(MACRO_, ...)	PP_EXPAND(  PP_CAT(PP_VARIADIC_APPLY_, PP_VARIADIC_SIZE(__VA_ARGS__))	(MACRO_, __VA_ARGS__ )  )

namespace {
#define XPLUS1(X_) (X_ + 1)
	constexpr const int va_test[] = { PP_VARIADIC_APPLY(XPLUS1, 1,2,3) };
	static_assert(std::size(va_test) == 3, "! PP_VARIADIC_APPLY !");
	static_assert(va_test[0] == 2, "! PP_VARIADIC_APPLY !");
	static_assert(va_test[1] == 3, "! PP_VARIADIC_APPLY !");
	static_assert(va_test[2] == 4, "! PP_VARIADIC_APPLY !");
#undef XPLUS1
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------








#endif	//ESB_PP_H