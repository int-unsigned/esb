/*
	"ESB" is designed to create external native components for the 1C(tm) program.
	Copyright © 2023 IntUnsigned	(v8classes@gmail.com)
	"ESB" is free software under LGPLv2.1 license with essential restriction about binary part (view esb-license.txt)
	"ESB" IS DISTRIBUTED "AS-IS" WITHOUT ANY, EVEN THE IMPLIED, WARRANTY. IN NO EVENT SHALL THE AUTHORS OR THE COPYRIGHT OWNER BE LIABLE FOR ANY CLAIM, 
	DAMAGES OR OTHER LIABILITY ARISING OUT OF THE USE OF THIS SOFTWARE.
	THE AUTHOR DOES NOT GIVE ANY GUARANTEES AND DOES NOT BEAR ANY RESPONSIBILITY REGARDING THE USE OF 1C(tm) COMPONENTS PRODUCED USING THE ESB LIBRARY IN THE 1C(tm) ENVIRONMENT.
*/


#pragma once
#ifndef ESB_STL_BASE_H
#define ESB_STL_BASE_H

#include "esb_meta.h"
#include "esb_wrapper.h"

//TODO	 все в esb::_internal

namespace esb 
{
}//namespace esb 


//TODO	Возможно все что касается ..Wrapper нужно перенести в отдельный esb_wrappers.h
//

namespace esb
{


} //namespace esb



namespace esb
{







//TODO	мои stl-шаблоны НЕ ЯВЛЯЮТСЯ специализациями ExClassWrapperFor
//		поэтому для них все как-то отдельно. почему?
	template<typename TypeTy>
	using ValueForWrapperOf = esb::_internal::ValueFor< TypeTy >;

#define ESB_DECLARE_VALUE_FOR_WRAPPER_TEMPLATE(TEMPLATE_TY_, VALUE_NAME_EN_, VALUE_NAME_RU_)			\
    ESB_DECLARE_VALUE_FOR_WRAPPER_IMPL_EN(VALUE_NAME_EN_, ValueForWrapperOf, PP_EXTRACT TEMPLATE_TY_);	\
    ESB_DECLARE_VALUE_FOR_WRAPPER_IMPL_RU(VALUE_NAME_RU_, ValueForWrapperOf, PP_EXTRACT TEMPLATE_TY_)



#define ESB_INTERFACEDEF_FOR_TEMPLATE(TEMPLATE_SPEC_T_, GUID_, NAME_EN_, NAME_RU_, DESCR_)																\
	ESB_INTERFACEDEF_EX(TEMPLATE_SPEC_T_, GUID_, (NAME_EN_, NAME_RU_), DESCR_)																			\
	ESB_INTERFACEDEF_FOR_TEMPLATE_IMPL(ESB_INTERFACE_OBJECT_FIELD);																						\
	ESB_INTERFACEDEF_DONE(PP_EXTRACT TEMPLATE_SPEC_T_)


// TEMPLATE_TY_ must be in form (StlVectorWrapper< std::vector<int64_t>, Numeric >)
#define ESB_CLASS_WRAPPER_FROM_TEMPLATE(TEMPLATE_TY_, CLASS_VALUE_EN_, CLASS_VALUE_RU_, GUID_, DESCRIPTION_)									\
	ESB_DECLARE_VALUE_FOR_WRAPPER_TEMPLATE(TEMPLATE_TY_, CLASS_VALUE_EN_, CLASS_VALUE_RU_);														\
    ESB_INTERFACEDEF_FOR_TEMPLATE(TEMPLATE_TY_, GUID_, #CLASS_VALUE_EN_, #CLASS_VALUE_RU_, DESCRIPTION_)



	template<typename ValueTy, typename ReferenceTy = ValueTy, typename ConstValueTy = ValueTy, typename ConstReferenceTy = ValueTy>
	struct stl_compatable_types
	{
		using value_t			= ValueTy;
		using reference_t		= ReferenceTy;
		using const_value_t		= ConstValueTy;
		using const_reference_t = ConstReferenceTy;
	};

//TODO	видимо все это должно быть перенесено в esb_wrapper.h и stl_compatable_types как-то переназван wrapper_compatable_types что-ли, хз..
#define ESB_WRAPPER_FOR_TEMPLATE(TEMPLATE_, TEMPLATE_ARG_, COMPATABLE_TY_, CLASS_VALUE_EN_, CLASS_VALUE_RU_, GUID_, DESCRIPTION_)							\
    ESB_CLASS_WRAPPER_FROM_TEMPLATE((TEMPLATE_< PP_EXTRACT TEMPLATE_ARG_, stl_compatable_types<PP_EXTRACT COMPATABLE_TY_> >), CLASS_VALUE_EN_, CLASS_VALUE_RU_, GUID_, DESCRIPTION_)

} //namespace esb



namespace esb
{



} //namespace esb



namespace esb 
{





	



	





} //namespace esb 

#endif //ESB_STL_BASE_H