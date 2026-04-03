/*
	"ESB" is designed to create external native components for the 1C(tm) program.
	Copyright © 2023 IntUnsigned	(v8classes@gmail.com)
	"ESB" is free software under LGPLv2.1 license with essential restriction about binary part (view esb-license.txt)
	"ESB" IS DISTRIBUTED "AS-IS" WITHOUT ANY, EVEN THE IMPLIED, WARRANTY. IN NO EVENT SHALL THE AUTHORS OR THE COPYRIGHT OWNER BE LIABLE FOR ANY CLAIM, 
	DAMAGES OR OTHER LIABILITY ARISING OUT OF THE USE OF THIS SOFTWARE.
	THE AUTHOR DOES NOT GIVE ANY GUARANTEES AND DOES NOT BEAR ANY RESPONSIBILITY REGARDING THE USE OF 1C(tm) COMPONENTS PRODUCED USING THE ESB LIBRARY IN THE 1C(tm) ENVIRONMENT.
*/


#pragma once
#ifndef ESB_STL_VECTOR_H
#define ESB_STL_VECTOR_H

#include "esb_stl_base_container.h"

namespace esb 
{

    template<typename CppVectorTy, typename EsbCompatableTy>
    struct StlVectorWrapper;
    //
    template<typename CppVectorTy, typename EsbCompatableTy>
    using StlVectorValue = esb::_internal::ValueForExEntity<ESB_INTERFACE_INFO_T<StlVectorWrapper<CppVectorTy, EsbCompatableTy>>, StlVectorWrapper<CppVectorTy, EsbCompatableTy>>;
    //
    template<typename CppTy, typename StlContainerElemCompatableTag>
    struct wrapper_for_traits<std::vector<CppTy>, StlContainerElemCompatableTag> : std::type_identity< StlVectorWrapper<std::vector<CppTy>, StlContainerElemCompatableTag> > {};
    //
    //
    template<typename CppVectorTy, typename EsbCompatableTy>
    struct StlVectorWrapper : StlIndexedContainerWrapperBase<CppVectorTy, EsbCompatableTy>
    {
    private:
        using ExClassWrapperBase<CppVectorTy>::_;
        using base_t = StlIndexedContainerWrapperBase<CppVectorTy, EsbCompatableTy>;
        using base_t::_safe_index;
        using base_t::_convert_to_cpp_arg;
        using base_t::content_is_const;
    public:
        using typename base_t::EsbElemTypeIn;
        using typename base_t::EsbElemTypeOut;
        using typename base_t::SizeType;
        // наследуем конструкторы
        using base_t::base_t;
        //-----------------
    public:
        //Общее для всех вариантов вектора
        //GetCapacity - это RO-свойство Capacity(Емкость) , SetCapacity - это будет метод УстановитьЕмкость для не-конст вариантов
        esb::Numeric GetCapacity() const                                { return _.capacity(); }
        //Часть только для RW-вектора
        //NOTE  Мы опять полагаемся на "Lazy Instantiation" и то, что в интерфейсе эти методы трогать не будут
        void Add(const EsbElemTypeIn& value_)                           requires not content_is_const   { _.push_back(_convert_to_cpp_arg(value_)); }
        void Clear()                                                    requires not content_is_const   { _.clear(); }
        void Delete(const Numeric& index_)                              requires not content_is_const   { _.erase(_.begin() + _safe_index(index_)); }
        //TODO  в 1С value_ опциональный параметр. нужно сделать
        void Insert(const Numeric& index_, const EsbElemTypeIn& value_) requires not content_is_const   { _.insert(_.begin() + _safe_index(index_), _convert_to_cpp_arg(value_)); }
        void SetCapacity(const Numeric& capacity_)                      requires not content_is_const   { _.reserve(esb::convert_to<SizeType>(capacity_)); }
        //
        //TODO	Добавить методов вектора по вкусу
    };
    //
    //
    template<typename ObjectTy, typename DescriptorInfoTy>
    struct stl_interface_for_vector_ro_face
    {
        static ESB_META_INTERFACE_EVAL_MODE auto make_meta_interface_meth()
        {
            return std::tuple_cat
            (
                stl_interface_for_indexed_collection_r<ObjectTy>::make_meta_interface_meth(),
                ex_class_wrapper_interface_base<ObjectTy, DescriptorInfoTy>::meth_cref()
            );
        }
        static ESB_META_INTERFACE_EVAL_MODE auto make_meta_interface_prop()
        {
            return	esb::_internal::make_meta_interface_prop
            (
                ESB_META_INTERFACE_MAKE_PROP_RO(ObjectTy ::, Capacity, "Capacity", "Емкость")
            );
        }
    };
    template<typename ObjectTy, typename DescriptorInfoTy>
    struct stl_interface_for_vector_rw_face : stl_interface_for_vector_ro_face<ObjectTy, DescriptorInfoTy>
    {
        static ESB_META_INTERFACE_EVAL_MODE auto make_meta_interface_meth()
        {
            return std::tuple_cat
            (
                stl_interface_for_vector_ro_face<ObjectTy, DescriptorInfoTy>::make_meta_interface_meth(),
                stl_interface_for_indexed_collection_updatable<ObjectTy>::make_meta_interface_meth(),
                stl_interface_for_indexed_collection_resizable<ObjectTy>::make_meta_interface_meth(),
                ex_class_wrapper_interface_base<ObjectTy, DescriptorInfoTy>::meth_vref(),
                esb::_internal::make_meta_interface_meth
                (
                    ESB_META_INTERFACE_MAKE_METH(ObjectTy, SetCapacity, "УстановитьЕмкость")
                )
            );
        }
        //
        // Свойства полностью наследуются от stl_interface_for_vector_ro_face
    };

    //TODO  При декларации конструктора тип нужно указывать без всяких const/ref
    //      т.к. ExtInvoker4CtorFromTypePack специализирует check_and_make_from_var_ex<std::remove_cvref_t<ParaTypesPackT>>
    //      и, соответственно, также без всяких const/ref должен быть определен конструктор (иначе std::construct_at не находится)
    //TODO  Видимо это можно как-то доработать, чтобы конструктор можно было определять свободнее - как const Type& и Type&&
    //
    //TODO  !! Для конст-вектора должен быть какой-то не-пустой конструктор! А как иначе его создать?? !!
    //
    template<typename CppVectorTy, typename ObjectTy, typename DescriptorInfoTy>
    struct stl_interface_for_vector_ro : stl_interface_for_vector_ro_face<ObjectTy, DescriptorInfoTy>
    {
        static ESB_META_INTERFACE_EVAL_MODE auto make_meta_interface_ctor()
        {
            if constexpr (std::is_reference_v<CppVectorTy>)
                return esb::_internal::make_meta_interface_ctor();      //TODO конструкторы из 1С отсутствуют
            else
                return esb::_internal::make_meta_interface_ctor();      //TODO конструкторы из 1С отсутствуют
        }
    };
    template<typename CppVectorTy, typename ObjectTy, typename DescriptorInfoTy>
    struct stl_interface_for_vector_rw : stl_interface_for_vector_rw_face<ObjectTy, DescriptorInfoTy>
    {
        static ESB_META_INTERFACE_EVAL_MODE auto make_meta_interface_ctor()
        {
            if constexpr (std::is_reference_v<CppVectorTy>)
                return esb::_internal::make_meta_interface_ctor();      //TODO конструкторы из 1С отсутствуют
            else
                return	esb::_internal::make_meta_interface_ctor
                (
                    ESB_META_CTOR()
                );
        }
    };
    

    template<typename CppVectorTy, typename EsbCompatableTy>
    struct template_interface_descriptor< StlVectorWrapper< CppVectorTy, EsbCompatableTy> >
        : stl_interface_descriptor<StlVectorWrapper, StlVectorValue, CppVectorTy, EsbCompatableTy, stl_interface_for_vector_ro, stl_interface_for_vector_rw>
    { };


#define ESB_WRAPPER_FOR_STL_VECTOR(VECTOR_TY_, COMPATABLE_TY_, CLASS_VALUE_EN_, CLASS_VALUE_RU_, GUID_, DESCRIPTION_)							\
	ESB_CLASS_WRAPPER_FROM_TEMPLATE((StlVectorWrapper< PP_EXTRACT VECTOR_TY_, stl_compatable_types<PP_EXTRACT COMPATABLE_TY_> >), CLASS_VALUE_EN_, CLASS_VALUE_RU_, GUID_, DESCRIPTION_)

} //namespace esb 


#endif //ESB_STL_VECTOR_H