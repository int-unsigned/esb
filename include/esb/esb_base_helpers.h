/*
	"ESB" is designed to create external native components for the 1C(tm) program.
	Copyright © 2023 IntUnsigned	(v8classes@gmail.com)
	"ESB" is free software under LGPLv2.1 license with essential restriction about binary part (view esb-license.txt)
	"ESB" IS DISTRIBUTED "AS-IS" WITHOUT ANY, EVEN THE IMPLIED, WARRANTY. IN NO EVENT SHALL THE AUTHORS OR THE COPYRIGHT OWNER BE LIABLE FOR ANY CLAIM,
	DAMAGES OR OTHER LIABILITY ARISING OUT OF THE USE OF THIS SOFTWARE.
	THE AUTHOR DOES NOT GIVE ANY GUARANTEES AND DOES NOT BEAR ANY RESPONSIBILITY REGARDING THE USE OF 1C(tm) COMPONENTS PRODUCED USING THE ESB LIBRARY IN THE 1C(tm) ENVIRONMENT.
*/


#pragma once
#ifndef ESB_BASE_HELPERS_H_
#define ESB_BASE_HELPERS_H_

#include "esb_core.h"




//TODO	Всякие доп-макросы для "удобства"
//		припрятать куда-то надо.. (до ума довести)
// 	
//__attribute__((used))
//Критично заключать блок в {}. Иначе ; в конце трактуется как асм-комментарий и все что после нее игнорируется - если использовать макрос внутри другого макроса с переносом строки
#define VARIABLE_USED(VAR_) __asm {mov eax, VAR_}
// для декларации псевдо-енум-структоры ID методов
//TOBO: Нужно в макрос обернуть и саму декларацию структуры - struct MethId -> ESB_DECL_METHID(...), и в этот макрос аргументами передавать ESB_ID
//		`;` после ESB_ID добавить в макрос и убрать из текста.
//		тогда мы получим возможность "в зависимости от" менять способ представления диспатч-идентификаторов. Например для варианта динамической загрузки
//		можем сделать id не constexpr, а просто static и биндить их при startup. Малореально без reflection, но.. когда-то оно будет.
#define ESB_ID(NAME_, VAL_)			static constexpr int NAME_ = VAL_
// в макрос нельзя непосредственно подставить ClassMix<C1_, C2_> т.к. препроцессор воспринимает запятую как разделитель аргуменотов самого макроса. <>-для него не скобки.
#define ESB_MIX2(C1_, C2_)		ClassMix<C1_, C2_>


#define ESB_ARGPACK0()
#define ESB_ARGPACK1(_1)							{&_1}							
#define ESB_ARGPACK2(_1,_2)							{&_1,&_2}
#define ESB_ARGPACK3(_1,_2,_3)						{&_1,&_2,&_3}
#define ESB_ARGPACK4(_1,_2,_3,_4)					{&_1,&_2,&_3,&_4}
#define ESB_ARGPACK5(_1,_2,_3,_4,_5)				{&_1,&_2,&_3,&_4,&_5}
#define ESB_ARGPACK6(_1,_2,_3,_4,_5,_6)				{&_1,&_2,&_3,&_4,&_5,&_6}
#define ESB_ARGPACK7(_1,_2,_3,_4,_5,_6,_7)			{&_1,&_2,&_3,&_4,&_5,&_6,&_7}
#define ESB_ARGPACK8(_1,_2,_3,_4,_5,_6,_7,_8)		{&_1,&_2,&_3,&_4,&_5,&_6,&_7,&_8}
#define ESB_ARGPACK9(_1,_2,_3,_4,_5,_6,_7,_8,_9)	{&_1,&_2,&_3,&_4,&_5,&_6,&_7,&_8,&_9}



#define ESB_INVOKE_CTOR_(CLASS_)							return esb::_internal::create_value<CLASS_>
#define ESB_INVOKE_CTOR0(CLASS_)									ESB_INVOKE_CTOR_(CLASS_) (Type_)
#define ESB_INVOKE_CTOR1(CLASS_, _1)								ESB_INVOKE_CTOR_(CLASS_) (Type_, {&_1});							
#define ESB_INVOKE_CTOR2(CLASS_, _1,_2)								ESB_INVOKE_CTOR_(CLASS_) (Type_, {&_1,&_2});						
#define ESB_INVOKE_CTOR3(CLASS_, _1,_2,_3)							ESB_INVOKE_CTOR_(CLASS_) (Type_, {&_1,&_2,&_3});					
#define ESB_INVOKE_CTOR4(CLASS_, _1,_2,_3,_4)						ESB_INVOKE_CTOR_(CLASS_) (Type_, {&_1,&_2,&_3,&_4});				
#define ESB_INVOKE_CTOR5(CLASS_, _1,_2,_3,_4,_5)					ESB_INVOKE_CTOR_(CLASS_) (Type_, {&_1,&_2,&_3,&_4,&_5});			
#define ESB_INVOKE_CTOR6(CLASS_, _1,_2,_3,_4,_5,_6)					ESB_INVOKE_CTOR_(CLASS_) (Type_, {&_1,&_2,&_3,&_4,&_5,&_6});		
#define ESB_INVOKE_CTOR7(CLASS_, _1,_2,_3,_4,_5,_6,_7)				ESB_INVOKE_CTOR_(CLASS_) (Type_, {&_1,&_2,&_3,&_4,&_5,&_6,&_7});	
#define ESB_INVOKE_CTOR8(CLASS_, _1,_2,_3,_4,_5,_6,_7,_8)			ESB_INVOKE_CTOR_(CLASS_) (Type_, {&_1,&_2,&_3,&_4,&_5,&_6,&_7,&_8});

#define ESB_INVOKE_CTOR_PACK1(CLASS_, ARGS_V_)						ESB_INVOKE_CTOR_(CLASS_) (Type_, {&ARGS_V_ } );
#define ESB_INVOKE_CTOR_PACK2(CLASS_, _1V, ARGS_V_)					ESB_INVOKE_CTOR_(CLASS_) (Type_, {&_1V, &ARGS_V_ });


#define ESB_INVOKE_MEMB_FUNC0(RET_, METH_)					return check_and_make<RET_>(IObjectMethods_InvokeAsFunc( *get_context(*this), static_cast<dispix_t>((METH_)) ) )
#define ESB_INVOKE_MEMB_FUNC_(RET_, METH_, ARGS_)			return check_and_make<RET_>(IObjectMethods_InvokeAsFunc(* get_context(*this), static_cast<dispix_t>((METH_)) , ARGS_ ) )
#define ESB_INVOKE_MEMB_FUNC1(RET_, METH_, _1)				ESB_INVOKE_MEMB_FUNC_(PP_EXPAND_LATER(RET_), METH_, ESB_ARGPACK1(_1) )
#define ESB_INVOKE_MEMB_FUNC2(RET_, METH_, _1,_2)			ESB_INVOKE_MEMB_FUNC_(PP_EXPAND_LATER(RET_), METH_, ESB_ARGPACK2(_1,_2) )
#define ESB_INVOKE_MEMB_FUNC3(RET_, METH_, _1,_2,_3)		ESB_INVOKE_MEMB_FUNC_(PP_EXPAND_LATER(RET_), METH_, ESB_ARGPACK3(_1,_2,_3) )
#define ESB_INVOKE_MEMB_FUNC4(RET_, METH_, _1,_2,_3,_4)		ESB_INVOKE_MEMB_FUNC_(PP_EXPAND_LATER(RET_), METH_, ESB_ARGPACK4(_1,_2,_3,_4) )
#define ESB_INVOKE_MEMB_FUNC5(RET_, METH_, _1,_2,_3,_4,_5)	ESB_INVOKE_MEMB_FUNC_(PP_EXPAND_LATER(RET_), METH_, ESB_ARGPACK5(_1,_2,_3,_4,_5) )


#define ESB_INVOKE_MEMB_PROC0(METH_)						return IObjectMethods_InvokeAsProc( *get_context(*this), static_cast<dispix_t>((METH_))  );
#define ESB_INVOKE_MEMB_PROC_(METH_, ARGS_)					return IObjectMethods_InvokeAsProc( *get_context(*this), static_cast<dispix_t>((METH_)), ARGS_ );
#define ESB_INVOKE_MEMB_PROC1(METH_, _1)					ESB_INVOKE_MEMB_PROC_(METH_, ESB_ARGPACK1(_1) )
#define ESB_INVOKE_MEMB_PROC2(METH_, _1,_2)					ESB_INVOKE_MEMB_PROC_(METH_, ESB_ARGPACK2(_1,_2) )
#define ESB_INVOKE_MEMB_PROC3(METH_, _1,_2,_3)				ESB_INVOKE_MEMB_PROC_(METH_, ESB_ARGPACK3(_1,_2,_3) )
#define ESB_INVOKE_MEMB_PROC4(METH_, _1,_2,_3,_4)			ESB_INVOKE_MEMB_PROC_(METH_, ESB_ARGPACK4(_1,_2,_3,_4) )


#define ESB_INVOKE_PROPERTY_GET(TYPE_, PROP_)		return check_and_make<TYPE_>(IObjectProperties_GetValue(*get_context(*this), (PROP_) ))
#define ESB_INVOKE_PROPERTY_SET(PROP_, VAL_)		return IObjectProperties_SetValue(*get_context(*this), (PROP_) , VAL_)


#if defined(ESB_USE_PROPERTIES) && ESB_USE_PROPERTIES
#	define ESB_PROPERTY_FIELD_RO_FUNC(FUNC_, TYPE_, NAME_)					__declspec(property(get = FUNC_))						TYPE_ NAME_;
#	define ESB_PROPERTY_FIELD_WO_FUNC(FUNC_, TYPE_, NAME_)					__declspec(property(put = FUNC_))						TYPE_ NAME_;
#	define ESB_PROPERTY_FIELD_RW_FUNC(FUNC_GET_, FUNC_SET_, TYPE_, NAME_)	__declspec(property(get = FUNC_GET_, put = FUNC_SET_))	TYPE_ NAME_;

#	define ESB_PROPERTY_ARRAY_RO_FUNC(FUNC_, TYPE_, NAME_)					__declspec(property(get = FUNC_))						TYPE_ NAME_[];
#	define ESB_PROPERTY_ARRAY_WO_FUNC(FUNC_, TYPE_, NAME_)					__declspec(property(put = FUNC_))						TYPE_ NAME_[];
#	define ESB_PROPERTY_ARRAY_RW_FUNC(FUNC_GET_, FUNC_SET_, TYPE_, NAME_)	__declspec(property(get = FUNC_GET_, put = FUNC_SET_))	TYPE_ NAME_[];

#else
#	define ESB_PROPERTY_FIELD_RO_FUNC(FUNC_, TYPE_, NAME_)				
#	define ESB_PROPERTY_FIELD_WO_FUNC(FUNC_, TYPE_, NAME_)				
#	define ESB_PROPERTY_FIELD_RW_FUNC(FUNC_GET_, FUNC_SET_, TYPE_, NAME_)

#	define ESB_PROPERTY_ARRAY_RO_FUNC(FUNC_, TYPE_, NAME_)				
#	define ESB_PROPERTY_ARRAY_WO_FUNC(FUNC_, TYPE_, NAME_)				
#	define ESB_PROPERTY_ARRAY_RW_FUNC(FUNC_GET_, FUNC_SET_, TYPE_, NAME_)
#endif

//TODO	Где используются эти "безымянные" ен свойства - сделать ен+ру
#define ESB_PROPERTY_FIELD_RO(TYPE_, NAME_)			ESB_PROPERTY_FIELD_RO_FUNC(Get##NAME_, PP_EXPAND_LATER(TYPE_), NAME_)
#define ESB_PROPERTY_FIELD_WO(TYPE_, NAME_)			ESB_PROPERTY_FIELD_WO_FUNC(Set##NAME_, PP_EXPAND_LATER(TYPE_), NAME_)
#define ESB_PROPERTY_FIELD_RW(TYPE_, NAME_)			ESB_PROPERTY_FIELD_RW_FUNC(Get##NAME_, Set##NAME_, PP_EXPAND_LATER(TYPE_), NAME_)

#define ESB_PROPERTY_ARRAY_RO(TYPE_, NAME_)			ESB_PROPERTY_ARRAY_RO_FUNC(Get##NAME_, PP_EXPAND_LATER(TYPE_), NAME_)
#define ESB_PROPERTY_ARRAY_WO(TYPE_, NAME_)			ESB_PROPERTY_ARRAY_WO_FUNC(Set##NAME_, PP_EXPAND_LATER(TYPE_), NAME_)
#define ESB_PROPERTY_ARRAY_RW(TYPE_, NAME_)			ESB_PROPERTY_ARRAY_RW_FUNC(Get##NAME_, Set##NAME_, PP_EXPAND_LATER(TYPE_), NAME_)


#define ESB_API_PROVIDER(TAG_)		esb_api::Provider<ESB_API_PROVIDER_TAG_T(TAG_)>
#define ESB_API_METHID(P_, M_)		ESB_DISPINFO_METH(ESB_API_PROVIDER_TAG_T(P_))::M_
#define ESB_API_PROPID(P_, M_)		ESB_DISPINFO_PROP(ESB_API_PROVIDER_TAG_T(P_))::M_
//
#define ESB_INVOKE_API_FUNC0(T_, P_, M_)						return check_and_make<T_>( ESB_API_PROVIDER(P_)::invoke_func( ESB_API_METHID(P_, M_)) )
#define ESB_INVOKE_API_FUNC1(T_, P_, M_, _1)					return check_and_make<T_>( ESB_API_PROVIDER(P_)::invoke_func( ESB_API_METHID(P_, M_), {&_1} ))
#define ESB_INVOKE_API_FUNC2(T_, P_, M_, _1,_2)					return check_and_make<T_>( ESB_API_PROVIDER(P_)::invoke_func( ESB_API_METHID(P_, M_), {&_1,&_2} ))
#define ESB_INVOKE_API_FUNC3(T_, P_, M_, _1,_2,_3)				return check_and_make<T_>( ESB_API_PROVIDER(P_)::invoke_func( ESB_API_METHID(P_, M_), {&_1,&_2,&_3} ))
#define ESB_INVOKE_API_FUNC4(T_, P_, M_, _1,_2,_3,_4)			return check_and_make<T_>( ESB_API_PROVIDER(P_)::invoke_func( ESB_API_METHID(P_, M_), {&_1,&_2,&_3,&_4} ))
#define ESB_INVOKE_API_FUNC5(T_, P_, M_, _1,_2,_3,_4,_5)		return check_and_make<T_>( ESB_API_PROVIDER(P_)::invoke_func( ESB_API_METHID(P_, M_), {&_1,&_2,&_3,&_4,&_5} ))
#define ESB_INVOKE_API_FUNC6(T_, P_, M_, _1,_2,_3,_4,_5,_6)		return check_and_make<T_>( ESB_API_PROVIDER(P_)::invoke_func( ESB_API_METHID(P_, M_), {&_1,&_2,&_3,&_4,&_5,&_6} ))
#define ESB_INVOKE_API_FUNCX(T_, P_, M_, _X)					return check_and_make<T_>( ESB_API_PROVIDER(P_)::invoke_func( ESB_API_METHID(P_, M_), {&_X...} ))
#define ESB_INVOKE_API_FUNC1X(T_, P_, M_, _1,_X)					return check_and_make<T_>( ESB_API_PROVIDER(P_)::invoke_func( ESB_API_METHID(P_, M_), {&_1,&_X...} ))

#define ESB_INVOKE_API_PROC0(P_, M_)							return ESB_API_PROVIDER(P_)::invoke_proc(ESB_API_METHID(P_, M_))
#define ESB_INVOKE_API_PROC1(P_, M_, _1)						return ESB_API_PROVIDER(P_)::invoke_proc(ESB_API_METHID(P_, M_), {&_1} )
#define ESB_INVOKE_API_PROC2(P_, M_, _1,_2)						return ESB_API_PROVIDER(P_)::invoke_proc(ESB_API_METHID(P_, M_), {&_1,&_2} )
#define ESB_INVOKE_API_PROC3(P_, M_, _1,_2,_3)					return ESB_API_PROVIDER(P_)::invoke_proc(ESB_API_METHID(P_, M_), {&_1,&_2,&_3} )
#define ESB_INVOKE_API_PROC4(P_, M_, _1,_2,_3,_4)				return ESB_API_PROVIDER(P_)::invoke_proc(ESB_API_METHID(P_, M_), {&_1,&_2,&_3,&_4} )
#define ESB_INVOKE_API_PROC5(P_, M_, _1,_2,_3,_4,_5)			return ESB_API_PROVIDER(P_)::invoke_proc(ESB_API_METHID(P_, M_), {&_1,&_2,&_3,&_4,&_5} )
#define ESB_INVOKE_API_PROC6(P_, M_, _1,_2,_3,_4,_5,_6)			return ESB_API_PROVIDER(P_)::invoke_proc(ESB_API_METHID(P_, M_), {&_1,&_2,&_3,&_4,&_5,&_6} )
#define ESB_INVOKE_API_PROC7(P_, M_, _1,_2,_3,_4,_5,_6,_7)		return ESB_API_PROVIDER(P_)::invoke_proc(ESB_API_METHID(P_, M_), {&_1,&_2,&_3,&_4,&_5,&_6,&_7} )

#define ESB_INVOKE_API_PROP_GET(T_, P_, M_)						return check_and_make<T_>( ESB_API_PROVIDER(P_)::invoke_prop_get(ESB_API_PROPID(P_, M_)) )


#endif


