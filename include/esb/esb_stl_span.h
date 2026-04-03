/*
	"ESB" is designed to create external native components for the 1C(tm) program.
	Copyright © 2023 IntUnsigned	(v8classes@gmail.com)
	"ESB" is free software under LGPLv2.1 license with essential restriction about binary part (view esb-license.txt)
	"ESB" IS DISTRIBUTED "AS-IS" WITHOUT ANY, EVEN THE IMPLIED, WARRANTY. IN NO EVENT SHALL THE AUTHORS OR THE COPYRIGHT OWNER BE LIABLE FOR ANY CLAIM, 
	DAMAGES OR OTHER LIABILITY ARISING OUT OF THE USE OF THIS SOFTWARE.
	THE AUTHOR DOES NOT GIVE ANY GUARANTEES AND DOES NOT BEAR ANY RESPONSIBILITY REGARDING THE USE OF 1C(tm) COMPONENTS PRODUCED USING THE ESB LIBRARY IN THE 1C(tm) ENVIRONMENT.
*/


#pragma once
#ifndef ESB_STL_SPAN_H
#define ESB_STL_SPAN_H

#include "esb_stl_base_container.h"

namespace esb 
{

    template<typename CppSpanTy, typename EsbCompatableTy>
    struct StlSpanWrapper;
    //
    template<typename CppSpanTy, typename EsbCompatableTy>
    using StlSpanValue = esb::_internal::ValueForExEntity<ESB_INTERFACE_INFO_T<StlSpanWrapper<CppSpanTy, EsbCompatableTy>>, StlSpanWrapper<CppSpanTy, EsbCompatableTy>>;
    //
    //TODO  StlSpanWrapper - это враппер для всех span-compatable типов, а не только std::span
    //      он как-то вроде подходит и для absl::Span (или нет??)
    //      но работает...
    //      НО!
    //      Как-то нехорошо использовать StlSpanWrapper предварительно не уведомив нас о том, что absl::Span "совместим" с std::span
    //TODO  Нужен како-то корректный механизм переиспользования шаблона интерфейса для иного типа
    template<typename CppTy, typename StlContainerElemCompatableTag>
    struct wrapper_for_traits<std::span<CppTy>, StlContainerElemCompatableTag> : std::type_identity< StlSpanWrapper<std::span<CppTy>, StlContainerElemCompatableTag> > {};
    //
    //
    template<typename CppSpanTy, typename EsbCompatableTy>
    struct StlSpanWrapper : StlIndexedContainerWrapperBase<CppSpanTy, EsbCompatableTy>
    {
    private:
        using base_t = StlIndexedContainerWrapperBase<CppSpanTy, EsbCompatableTy>;
    public:
        // наследуем конструкторы
        using base_t::base_t;
        //
        //TODO  Добавить методы Спан по вкусу
    };
    //
    template<typename ObjectTy, typename DescriptorInfoTy>
    struct stl_interface_for_span_ro_face : esb::_internal::interface_empty_face_prop
    {
        static ESB_META_INTERFACE_EVAL_MODE auto make_meta_interface_meth()
        {
            return std::tuple_cat
            (
                stl_interface_for_indexed_collection_r<ObjectTy>::make_meta_interface_meth(),
                ex_class_wrapper_interface_base<ObjectTy, DescriptorInfoTy>::meth_cref()
            );
        }
    };
    //
    template<typename ObjectTy, typename DescriptorInfoTy>
    struct stl_interface_for_span_rw_face : stl_interface_for_span_ro_face<ObjectTy, DescriptorInfoTy>
    {
        static ESB_META_INTERFACE_EVAL_MODE auto make_meta_interface_meth()
        {
            return std::tuple_cat
            (
                stl_interface_for_span_ro_face<ObjectTy, DescriptorInfoTy>::make_meta_interface_meth(),
                stl_interface_for_indexed_collection_updatable<ObjectTy>::make_meta_interface_meth(),
                ex_class_wrapper_interface_base<ObjectTy, DescriptorInfoTy>::meth_vref()
            );
        }
        //
        // Свойства полностью наследуются от stl_interface_for_span_ro_face
    };
    //
    //TODO конструкторы из 1С отсутствуют
    template<typename CppSpanTy, typename ObjectTy, typename DescriptorInfoTy>
    struct stl_interface_for_span_ro : stl_interface_for_span_ro_face<ObjectTy, DescriptorInfoTy>, esb::_internal::interface_empty_ctor
    { };
    template<typename CppSpanTy, typename ObjectTy, typename DescriptorInfoTy>
    struct stl_interface_for_span_rw : stl_interface_for_span_rw_face<ObjectTy, DescriptorInfoTy>, esb::_internal::interface_empty_ctor
    { };
    //
    template<typename CppSpanTy, typename EsbCompatableTy>
    struct template_interface_descriptor< StlSpanWrapper<CppSpanTy, EsbCompatableTy> >
        : stl_interface_descriptor<StlSpanWrapper, StlSpanValue, CppSpanTy, EsbCompatableTy, stl_interface_for_span_ro, stl_interface_for_span_rw>
    { };



//#define ESB_WRAPPER_FOR_STL_SPAN(VECTOR_TY_, COMPATABLE_TY_, CLASS_VALUE_EN_, CLASS_VALUE_RU_, GUID_, DESCRIPTION_)							\
//	ESB_CLASS_WRAPPER_FROM_TEMPLATE((StlSpanWrapper< PP_EXTRACT VECTOR_TY_, stl_compatable_types<PP_EXTRACT COMPATABLE_TY_> >), CLASS_VALUE_EN_, CLASS_VALUE_RU_, GUID_, DESCRIPTION_)
#define ESB_WRAPPER_FOR_STL_SPAN(SPAN_TY_, COMPATABLE_TY_, CLASS_VALUE_EN_, CLASS_VALUE_RU_, GUID_, DESCRIPTION_)							\
	ESB_WRAPPER_FOR_TEMPLATE(StlSpanWrapper, SPAN_TY_, COMPATABLE_TY_, CLASS_VALUE_EN_, CLASS_VALUE_RU_, GUID_, DESCRIPTION_)


} //namespace esb 


#endif //ESB_STL_SPAN_H