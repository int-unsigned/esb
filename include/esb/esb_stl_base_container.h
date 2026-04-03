/*
	"ESB" is designed to create external native components for the 1C(tm) program.
	Copyright © 2023 IntUnsigned	(v8classes@gmail.com)
	"ESB" is free software under LGPLv2.1 license with essential restriction about binary part (view esb-license.txt)
	"ESB" IS DISTRIBUTED "AS-IS" WITHOUT ANY, EVEN THE IMPLIED, WARRANTY. IN NO EVENT SHALL THE AUTHORS OR THE COPYRIGHT OWNER BE LIABLE FOR ANY CLAIM, 
	DAMAGES OR OTHER LIABILITY ARISING OUT OF THE USE OF THIS SOFTWARE.
	THE AUTHOR DOES NOT GIVE ANY GUARANTEES AND DOES NOT BEAR ANY RESPONSIBILITY REGARDING THE USE OF 1C(tm) COMPONENTS PRODUCED USING THE ESB LIBRARY IN THE 1C(tm) ENVIRONMENT.
*/


#pragma once
#ifndef ESB_STL_BASE_CONTAINER_H
#define ESB_STL_BASE_CONTAINER_H

#include "esb_meta.h"
#include "esb_stl_base.h"

//TODO	 все в esb::_internal


namespace esb 
{

    template<typename CppContainerTy>
    inline constexpr bool is_stl_container_content_const = std::is_const_v<std::remove_reference_t<CppContainerTy>> ||
        std::is_const_v<std::remove_reference_t< esb::element_or_value_type_t<std::remove_reference_t<CppContainerTy>> > >;


    template<typename CppContainerTy, typename EsbCompatableTy>
    struct StlIndexedContainerWrapperBase : ExClassWrapperBase<CppContainerTy>
    {
        using ExClassWrapperBase<CppContainerTy>::ExClassWrapperBase;
        using ExClassWrapperBase<CppContainerTy>::_;
        //
        using contained_container_t             = std::remove_reference_t<CppContainerTy>;
        using contained_container_iterator_t    = std::conditional_t< std::is_const_v<contained_container_t>, typename contained_container_t::const_iterator, typename contained_container_t::iterator>;
        //
        using SizeType              = typename contained_container_t::size_type;    
        using CppElemType           = element_or_value_type_t<contained_container_t>;
        // CppElemType НЕ МОжет быть ссылкой - Вектор/Спан на ссылке не строят (хотя про Спан не уверен.. наткнемся посмотрим..)
        //TODO!!!   span на ссылках МОЖНО!! строить!!!
        //TODO!!!   вообще не помню зачем это ограничение сделал!
        static_assert(not std::is_reference_v<CppElemType>, "Unexpected !");

        using esb_compatable_val_t  = typename EsbCompatableTy::value_t;
        using esb_compatable_ref_t  = std::conditional_t<std::is_same_v<typename EsbCompatableTy::reference_t, void>,       esb_compatable_val_t, typename EsbCompatableTy::reference_t>;
        using esb_compatable_cval_t = std::conditional_t<std::is_same_v<typename EsbCompatableTy::const_value_t, void>,     esb_compatable_val_t, typename EsbCompatableTy::const_value_t>;
        using esb_compatable_cref_t = std::conditional_t<std::is_same_v<typename EsbCompatableTy::const_reference_t, void>, esb_compatable_val_t, typename EsbCompatableTy::const_reference_t>;

        // Принимать как элемент мы можем любой из совместимых типов
        using EsbElemTypeIn         = OneOf< typepack_unique_t<typepack<esb_compatable_val_t, esb_compatable_ref_t, esb_compatable_cval_t, esb_compatable_cref_t>> >;
        // Возвращать мы должны либо конст-ссылку на элемент если он конст, либо просто ссылку если мы или элементы не константа
        // (общее для вектора, который константа если сам константа и спан у которго константами должны быть элементы)
        static constexpr bool content_is_const  = is_stl_container_content_const<CppContainerTy>;
        using EsbElemTypeOut                    = std::conditional_t<content_is_const, esb_compatable_cref_t, esb_compatable_ref_t>;
	    //
        SizeType _safe_index(SizeType index_value_) const {
            if (index_value_ < this->Data().size())
                return index_value_;
            ESL_THROW_ARRAY_INDEX_ERR();
        }
	    SizeType _safe_index(const Numeric& index_value_) const {
            return _safe_index(convert_to<SizeType>(index_value_));
	    }
        //NOTE  _convert_to_cpp_ везде используется для конвертации "в-аргумент", поэтому делаем convert_to_arg
        static CppElemType _convert_to_cpp_arg(const EsbElemTypeIn& value_) {
            return convert_to_arg<CppElemType>(value_);
        }
        //
        EsbElemTypeOut _get(SizeType index_) {
            return convert_to<EsbElemTypeOut>(_[index_]);
        }
        EsbElemTypeOut _get(ESB_OWNER_HANDLER_T owner_, SizeType index_) {
            return convert_to<EsbElemTypeOut>(std::move(owner_), _[index_]);
        }
        //
        // Собственно методы индексируемой коллекции для 1С (общие для вектора и спан и прочих)
        esb::Numeric Count() const 
        { 
            return esb::convert_to<Numeric>(_.size()); 
        }
        esb::Numeric UBound() const
        {
            return (_.empty()) ? Numeric::Value__1_ /*-1*/ : esb::convert_to<Numeric>(_.size() - 1);
        }

        //NOTE  В случае если элементы контейнера не сравнимы на равенство (нет оператора '==', not EqualityComparable)
        //      мы выбрасываем исключение
        // 
        //TODO  Возможны варианты возвращать Undef, -1, просто убрать Find из интерфейса, т.п.
        //      Нужно посмотреть что 1С в этих случаях делает
        //TODO  Можно еще добавить метод поиска с предикатом (делегатом) - FindWithPredicate (НайтиСПредикатом)
        //
        esb::UndefOr<Numeric> Find(const EsbElemTypeIn& value_) const
        {
            if constexpr (not std::equality_comparable< CppElemType>)
                ESL_THROW_CONTAINER_FIND_NOT_SUPPORTED_NO_EQ();
            else
            {
                auto it_1st = _.begin();
                auto it_end = _.end();
                auto it_find = std::find(it_1st, it_end, _convert_to_cpp_arg(value_));
                if (it_find == it_end)
                    return make_undef_or<Numeric>();
                else
                    return make_undef_or<Numeric>(convert_to<Numeric>(std::distance(it_1st, it_find)));
            }
        }   

        //NOTE  Метод Get сделан НЕ-КОНСТ
        //      т.к. в конст-методе мы никак не можем вернуть объект-обычную-ссылку на наш элемент
        //      (наш вектор становится константным и инициализировать не-конст ссылку невозможно)
        //      "константность" нашего вектора определяется через EsbElemTypeOut, который нам дали
        //      для просто-вектора нам должны дать ..Ref, а для конст-вектора должны дать ..CRef
        //      Также
        //      Метод сделан не-шаблоном т.к. он должен присутствовать во всех вариантах интерфейса и фильтровать его не надо
        EsbElemTypeOut Get(ESB_OWNER_HANDLER_T owner_, const Numeric& index_)
        {
            if constexpr (is_type_require_owner<EsbElemTypeOut>)
                return _get(std::move(owner_), _safe_index(index_));
            else
                return _get(_safe_index(index_));
        }
        //NOTE  Метод не валиден для const-вариантов, но в виду "Lazy Instantiation" 
        //      компилятор не будет его реализовывать, если его не вызывают (не берут указатель на метода)
        //      То есть мы полагаемся, что при реализации интерфейса его не будут трогать
        //      (ПС. requires прописан на всякий случай. и без него работает, но с ним спокойнее..)
        void Set(const Numeric& index_, const EsbElemTypeIn& value_) requires not content_is_const
        {
            _[_safe_index(index_)] = _convert_to_cpp_arg(value_);
        }
        // Кастомная имплементация индексированной коллекции 
        //  (кастомная потому что мы точно знаем во что должны преобразовываться наши элементы, 
        //  в отличие о имплементации по-умолчанию, которая опирается на зарегистрированные преобразования по-умолчанию)
        template<typename ExtValueTy>
        IValuePtr ESB_CONTAINED_IXCOLLECTION_GETVALUE_EXT(esb::_internal::ExValueBase& obj_base_, size_t index_)
        {
            if constexpr (is_type_require_owner<EsbElemTypeOut>)
                return get_interface( _get(esb::_internal::query_owner_handler<ExtValueTy>(obj_base_), index_) );
            else
                return get_interface( _get(index_) );
        }
        //NOTE  Это requires очень важно. На основании наличия у класса метода _SETVALUE дескриптор определяет - это RO коллекция или RW
        //      и создает соответствующий интерфейс (см. esb::_internal::is_interface_describe_indexable_rw)
        void ESB_CONTAINED_IXCOLLECTION_SETVALUE(size_t index_, IValuePtr&& value_) requires not content_is_const
        {       
            //TODO  Не совсем хорошо получилось. check_and_make для создания OneOf один раз проверяет типинфо,
            //      а _convert для преобразования второй раз проверяет типинфо
            //      Желательно бы придумать как эту проверку делать один раз.
            //      Здесь для этого достаточно сделать конвертер специализируемый typepack, а не OneOf
            //      НО! В методах .Add, .Set, .Insert та же проблема, но OneOf-value нам там делает инвокер,
            //          который понятия не имеет о наших "конверт" и т.п.
            //          Там сложнее намного, даже не понимаю как..
            //          Видимо нужно разрешить/научить инвокера сигнатуре метода с "сырым" IValuePtr-аргументом,
            //          чтобы в этом случае он не занимался конвертацией, а просто отдавал аргумент нам.
            //      Хотя бы здесь сделать для начала. Здесь можно.
            _[_safe_index(index_)] = _convert_to_cpp_arg(check_and_make<EsbElemTypeIn>(std::move(value_)));
        }
        // Реализация кастомного интерфейса енумератора
        esb::_internal::IEntityPtr ESB_CONTAINED_GETENUMERATOR(esb::_internal::IEntityPtr&& my_entity_ptr_) 
        {
            return esb::_internal::ExEnumeratorForContainer_Create<esb::_internal::ExEnumeratorForContainerWrapper>(std::move(my_entity_ptr_), *this);
        }
        //NOTE  ExEnumeratorForContainerWrapper фактически не делает ничего, а только хранит итераторы и наш container_handler_
        //      Нам он итераторы передает по ссылке и мы вольны с ними делать что хотим. Гибко, но нужно пользоваться этим аккуратно.
        IValuePtr ESB_CONTAINED_GETVALUEANDMOVENEXT(contained_container_iterator_t& current_, contained_container_iterator_t& end_, const esb::_internal::IEntityPtr& container_handler_)
        {
            if (current_ == end_)
                return { nullptr };

            if constexpr (is_type_require_owner<EsbElemTypeOut>)
                return get_interface(esb::convert_to<EsbElemTypeOut>(esb::_internal::IEntityPtr{ container_handler_ }, *current_++));
            else
                return get_interface(esb::convert_to<EsbElemTypeOut>(*current_++));
        }
    };



    template<typename ObjectTy>
    struct stl_interface_for_indexed_collection_r
    {
        static ESB_META_INTERFACE_EVAL_MODE auto make_meta_interface_meth()
        {
            return esb::_internal::make_meta_interface_meth
            (
                ESB_META_INTERFACE_MAKE_METH(ObjectTy,  Count,           "Количество"),
                ESB_META_INTERFACE_MAKE_METH(ObjectTy,  UBound,          "ВГраница"),
                ESB_META_INTERFACE_MAKE_METH(ObjectTy,  Find,            "Найти"),
                //
                ESB_META_INTERFACE_MAKE_METH(ObjectTy,  Get,             "Получить")
            );
        }
    };
    template<typename ObjectTy>
    struct stl_interface_for_indexed_collection_updatable
    {
        static ESB_META_INTERFACE_EVAL_MODE auto make_meta_interface_meth()
        {
            return esb::_internal::make_meta_interface_meth
            (
                ESB_META_INTERFACE_MAKE_METH(ObjectTy,  Set,             "Установить")
            );
        }
    };
    template<typename ObjectTy>
    struct stl_interface_for_indexed_collection_resizable
    {
        static ESB_META_INTERFACE_EVAL_MODE auto make_meta_interface_meth()
        {
            return esb::_internal::make_meta_interface_meth
            (
                ESB_META_INTERFACE_MAKE_METH(ObjectTy,  Clear,           "Очистить"),
                ESB_META_INTERFACE_MAKE_METH(ObjectTy,  Add,             "Добавить"),
                ESB_META_INTERFACE_MAKE_METH(ObjectTy,  Delete,          "Удалить"),
                ESB_META_INTERFACE_MAKE_METH(ObjectTy,  Insert,          "Вставить")
            );
        }
    };


    template<typename CppVectorTy, typename ObjectTy, typename DescriptorInfoTy, template<typename...> typename FaceRoTemplate, template<typename...> typename FaceRwTemplate>
    using stl_interface_for_t = std::conditional_t
        <    
            is_stl_container_content_const<CppVectorTy>,
            FaceRoTemplate<CppVectorTy, ObjectTy, DescriptorInfoTy>,
            FaceRwTemplate<CppVectorTy, ObjectTy, DescriptorInfoTy>
        >;


    template<   template<typename...> typename WrapperTemplate, 
                template<typename...> typename ValueTemplate, 
                typename CppDataTy, 
                typename EsbCompatableTy, 
                template<typename CppTy, typename ObjTy, typename InfTy> typename FaceRoTemplate,
                template<typename CppTy, typename ObjTy, typename InfTy> typename FaceRwTemplate>
    struct stl_interface_descriptor 
        : stl_interface_for_t <
                                CppDataTy, 
                                WrapperTemplate<CppDataTy, EsbCompatableTy>,
                                template_interface_descriptor_info_ex<WrapperTemplate, ValueTemplate, CppDataTy, EsbCompatableTy>,
                                FaceRoTemplate, FaceRwTemplate
                              >
    {
        using ESB_INTERFACE_OBJECT_FIELD = WrapperTemplate<CppDataTy, EsbCompatableTy>;
    };

} //namespace esb


#endif //ESB_STL_BASE_CONTAINER_H