
#pragma once


//----------------------------
//


//TODO	Алгоритм ну даже пробела в начале строки не терпит..
//		По хорошему нужно улучшать подключая pcre (где-то оно у Свиг есть)
%esb_insert_module_macro
%{#define ESB_WRAPPER_PROTO_PROP_OPT_GET(ESB_DT_, CPP_DT_, NAME_, PROP_)			\
    ESB_WRAPPER_METH_CONST((UndefOr<ESB_DT_>),   Get##NAME_)						\
	{																				\
		if ( _.has_ ## PROP_() )													\
			return make_undef_or<ESB_DT_>(convert_to_ret<ESB_DT_>(_.PROP_()));		\
		else																		\
			return make_undef_or<ESB_DT_>();										\
	}
%}
%esb_insert_module_macro 
%{#define ESB_WRAPPER_PROTO_PROP_OPT_SET(ESB_DT_, CPP_DT_, NAME_, PROP_)			\
    ESB_WRAPPER_METH_MUTABLE((void),    Set##NAME_, const UndefOr<ESB_DT_>& value_)	\
	{																				\
		if(value_.is<Undef>())														\
			_.clear_ ## PROP_();													\
		else																		\
			_.set_ ## PROP_(convert_to_arg<CPP_DT_>(value_.as<ESB_DT_>()));			\
	}
%}



%define %esb_ortools_proto_prop(CLASS_, CPP_NAME_, ESB_NAME_, ESB_NAME_RU_)
	%esb_prop(CLASS_ ##::## CPP_NAME_, ESB_NAME_);
	%esb_prop_decl(CLASS_ ##::## CPP_NAME_, "ESB_WRAPPER_PROTO_PROP_OPT_GET", #CPP_NAME_);
	%esb_name_local(ESB_NAME_RU_)	CLASS_ ##::## CPP_NAME_;
	%esb_prop(CLASS_ ##::## set_ ## CPP_NAME_, ESB_NAME_);
	%esb_prop_decl(CLASS_ ##::## set_ ## CPP_NAME_, "ESB_WRAPPER_PROTO_PROP_OPT_SET", #CPP_NAME_);
	%esb_name_local(ESB_NAME_RU_)	CLASS_ ##::## set_ ## CPP_NAME_
%enddef

%define %esb_ortools_proto_prop_decl(CPP_TYPE_, NAME_)
	CPP_TYPE_ NAME_ () const;
	void set_ ## NAME_(CPP_TYPE_ value_)
%enddef



%define %esb_ortools_proto_list(CLASS_, LIST_NAME_, RU_SIZE_, RU_CLEAR_, RU_GET_, RU_SET_, RU_ADD_)
	%esb_name_local(RU_SIZE_)	CLASS_ ##::## LIST_NAME_ ## _size;
	%esb_name_local(RU_CLEAR_)	CLASS_ ##::## clear_ ## LIST_NAME_;
	%esb_name_local(RU_GET_)	CLASS_ ##::## LIST_NAME_;
	%esb_name_local(RU_SET_)	CLASS_ ##::## set_ ## LIST_NAME_;
	%esb_name_local(RU_ADD_)	CLASS_ ##::## add_ ## LIST_NAME_
%enddef

%define %esb_ortools_proto_list_decl(LIST_NAME_, ELEM_TY_)
		int 				LIST_NAME_ ## _size() const;
		void 				clear_ ## LIST_NAME_();
		ELEM_TY_ 			LIST_NAME_ (int index) const;
		void 				set_ ## LIST_NAME_(int index, ELEM_TY_ value);
		void 				add_ ## LIST_NAME_(ELEM_TY_ value)
%enddef


//
//----------------------------