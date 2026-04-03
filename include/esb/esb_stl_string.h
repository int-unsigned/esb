/*
	"ESB" is designed to create external native components for the 1C(tm) program.
	Copyright © 2023 IntUnsigned	(v8classes@gmail.com)
	"ESB" is free software under LGPLv2.1 license with essential restriction about binary part (view esb-license.txt)
	"ESB" IS DISTRIBUTED "AS-IS" WITHOUT ANY, EVEN THE IMPLIED, WARRANTY. IN NO EVENT SHALL THE AUTHORS OR THE COPYRIGHT OWNER BE LIABLE FOR ANY CLAIM, 
	DAMAGES OR OTHER LIABILITY ARISING OUT OF THE USE OF THIS SOFTWARE.
	THE AUTHOR DOES NOT GIVE ANY GUARANTEES AND DOES NOT BEAR ANY RESPONSIBILITY REGARDING THE USE OF 1C(tm) COMPONENTS PRODUCED USING THE ESB LIBRARY IN THE 1C(tm) ENVIRONMENT.
*/


#pragma once
#ifndef ESB_STL_STRING_H
#define ESB_STL_STRING_H
#include <cassert>

#include "esb_stl_base_container.h"

#include "stringapiset.h"



namespace esb 
{
//TODO	Ниже шаблон строк. Нужно создать отдельный хедер esb_stl_string_conv.h 
// 
//TODO  Конвертацию нужно куда-то в само ЕСБ встроить типичную конвертацию A-W. ПС. вроде уже где-то должна быть для core::exception... ?
//		А еще лучше привязаться к платформе - у нее ICU и конвертация из коробки
inline esb::string_t convert_to_string_U(const std::string_view& astr_to_convert_)
{
    static_assert(sizeof(esb::char_t) == sizeof(WCHAR), "Expected sizeof(esb::char_t) == sizeof(WCHAR)");

    esb::string_t   ustr_converted;
    size_t          astr_to_convert_len = astr_to_convert_.length();
    assert(astr_to_convert_len < std::numeric_limits<int>::max());
    if (astr_to_convert_len) {
        int ustr_converted_len = MultiByteToWideChar(CP_ACP, 0, astr_to_convert_.data(), static_cast<int>(astr_to_convert_len), NULL, 0);
        //TODO  Can handle error (e.g., GetLastError())
        assert(ustr_converted_len > 0);
        if (ustr_converted_len > 0) {
            ustr_converted.resize(ustr_converted_len);
            int conversion_result_len = MultiByteToWideChar(CP_ACP, 0, astr_to_convert_.data(), static_cast<int>(astr_to_convert_len), (LPWSTR)ustr_converted.data(), ustr_converted_len);
            assert(conversion_result_len == ustr_converted_len);
        }
    }
    return ustr_converted;
}
inline esb::string_t convert_to_string_U(char achr_to_convert_)
{
    return convert_to_string_U(std::string_view(&achr_to_convert_, 1));
}

inline std::string convert_to_string_A(const esb::string_view_t& ustr_to_convert_)
{
    static_assert(sizeof(esb::char_t) == sizeof(WCHAR), "Expected sizeof(esb::char_t) == sizeof(WCHAR)");

    if (ustr_to_convert_.empty())
        return ""; 

    // Determine required buffer size for UTF-8 string
    int mb_ansi_string_size = WideCharToMultiByte(
        CP_ACP,                                                     // Code page to convert to 
        0,                                                          // No flags
        reinterpret_cast<const WCHAR*>(ustr_to_convert_.data()),    // Input UTF-16 string
        static_cast<int>(ustr_to_convert_.length()),                // Length of input string
        nullptr,                                                    // Output buffer (get size)
        0,                                                          // Output buffer size (get size)
        nullptr,                                                    // Default character
        nullptr                                                     // Used default character flag
    );

    //TODO  Can handle error (e.g., GetLastError())
    assert(mb_ansi_string_size > 0);
    if (mb_ansi_string_size <= 0)
        return "";

    std::string mb_ansi_string(mb_ansi_string_size, '\0');          // Create string with determined size

    int res_converted = WideCharToMultiByte(
        CP_ACP,
        0,
        reinterpret_cast<const WCHAR*>(ustr_to_convert_.data()),
        static_cast<int>(ustr_to_convert_.length()),
        &mb_ansi_string[0],       // Output buffer
        mb_ansi_string_size,
        nullptr,
        nullptr
    );

    assert(res_converted == mb_ansi_string_size);

    return mb_ansi_string;
}
inline std::string convert_to_string_A(esb::char_t uchr_to_convert_)
{
    return convert_to_string_A(esb::string_view_t(&uchr_to_convert_, 1));
}
template<typename CharToTy, typename CharFromTy>
inline CharToTy convert_to_string_char(CharFromTy x_char_)
{
    if constexpr (std::is_same_v<CharToTy, CharFromTy>)
        return x_char_;
    else
    {
        //TODO  здесь должна быть полноценная конвертация
        static_assert((std::is_same_v<CharToTy, char> && std::is_same_v<CharFromTy, esb::char_t>) 
                        || (std::is_same_v<CharToTy, esb::char_t> && std::is_same_v<CharFromTy, char>), "Unsupported string conversion.");
        //TODO  Во 1-х: конвертировать через создание строки расточительно - нужна нормальная конвертация
        //      Во 2-х: этой функции просится интерфейс std::expected (C++23). решение о исключении должен принимать вызывающий
        // 
        //TODO  нюанс с std::expected (C++23) ГОРАЗДО ШИРЕ
        //      это проблема у всей системы convert_to - КОНВЕРТАЦИИ МОГУТ БЫТЬ С РАНТАЙМ ОШИБКОЙ
        //      дополнительно ко всем convert(_to) НУЖНО делать try_convert_to, которая будет отдавать std::expected (C++23)
        //
        if constexpr (std::is_same_v<CharToTy, char>)
        {
            std::string str_a = convert_to_string_A(x_char_);
            if (str_a.size() != 1)
                ESL_THROW_ERROR_IN_STRING_CONVERSION();
            return str_a[0];
        }
        else
        {
            esb::string_t str_u = convert_to_string_U(x_char_);
            if (str_u.size() != 1)
                ESL_THROW_ERROR_IN_STRING_CONVERSION();
            return str_u[0];
        }
    }
}
//NOTE  Конструктор String от esb::string_view_t может быть объявлен explicit в зависимости от опции ESB_USE_EXPLICIT_STRING
//      поэтому мы его всегда здесь вызываем принудительно
ESB_DECLARE_CONVERT_TO_TYPE_FROM(String,        std::string,        String(convert_to_string_U(value_)) );
ESB_DECLARE_CONVERT_TO_TYPE_FROM(String,        std::string_view,   String(convert_to_string_U(value_)) );
//NOTE  Просто String::view() не для временных String&&. Но в этом случае это 'safe'
ESB_DECLARE_CONVERT_TO_TYPE_FROM(std::string,   String,             convert_to_string_A(value_.view_when_safe()) );


//NOTE  Конвертер в std::string_view мы не определяем, т.к. действительно 'unsafe' 
//          - конвертер вернет string_view для временного std::string - UB -
//      Безопасно это только в контексте вызова метода (вычисления полного выражения), когда компилятор продлевает жизнь
//      всех временных объектов созданных при вычислении полного выражения до конца вычисления оного (до ';', условно..)
//      В таких случаях конвертер в string_view можно доопределить. на в ограниченном namespace, 
//      т.к. иначи этот конвертер может попробовать использовать кто-то кому нельзя было..
//      Или лучше точечно использовать конвертацию в std::string если считаете что безопасно 
//      (string_view от string свободно конструируется)


//NOTE  Очень специализированный конвертер именно для аргументов.
//      Зачастую аргументы методов декларированы как 'std::string const&'
//      Стандартный конвертер без этой специализации (convert_to<std::string const&>(..)) 
//      и пытается вернуть std::string const&, т.е. ссылку да еще на временный объект.
//      Поэтому идем на хитрость - декларируем конвертер _to_ std::string const&,
//      а возвращаем std::string - by-val - она к ссылке привяжется 
//      и все будет Ок в рамках вызова какого-то метода
// 
//TODO  Возможно эту технику нужно расширить и углубить для других типов
//
template<typename EsbStringType>
    requires std::is_same_v<std::remove_cvref_t<EsbStringType>, String>
struct esb::converter_to_arg<std::string const&, EsbStringType>
{   
    // что бы там на входе не было, принимаем как конст-ссылку - привяжется, сконвертируем..
    static std::string convert(const String& value_)
    {          
        return convert_to_string_A(value_.view_when_safe());
    }
};


ESB_DECLARE_CONVERT_TO_TYPE_FROM(String,        esb::string_t,        String(value_)    );
ESB_DECLARE_CONVERT_TO_TYPE_FROM(String,        esb::string_view_t,   String(value_)    );
ESB_DECLARE_CONVERT_TO_TYPE_FROM(esb::string_t, String,               value_.string()   );
//В esb::string_view_t конвертера нет по причине указанной выше


//NOTE  По хорошему нужно у String сделать конструктор от char_t
//      НО! см.примечание у String
ESB_DECLARE_CONVERT_TO_TYPE_FROM(String, esb::char_t, String(esb::string_view_t(&value_, 1)));
ESB_DECLARE_CONVERT_TO_TYPE_FROM(String, char, String(convert_to_string_U(value_)));

} //namespace esb 


namespace esb
{

template<typename T, typename EsbCompatableTy >
struct StlStringWrapper;
//
template<typename CppStringTy, typename EsbCompatableTy >
using StlStringValue = esb::_internal::ValueForExEntity<ESB_INTERFACE_INFO_T<StlStringWrapper<CppStringTy, EsbCompatableTy>>, StlStringWrapper<CppStringTy, EsbCompatableTy>>;
//
template<typename CppCharTy>
struct wrapper_for_traits<std::basic_string<CppCharTy>> : std::type_identity< StlStringWrapper<std::basic_string<CppCharTy>, stl_compatable_types<String>> > {};



//

template< typename CppStringTy, typename EsbCompatableTy >
struct StlStringWrapper : StlIndexedContainerWrapperBase<CppStringTy, EsbCompatableTy>
{
private:
    using base_t = StlIndexedContainerWrapperBase<CppStringTy, EsbCompatableTy>;
    using base_t::contained_container_t;
    using base_t::_;
    using base_t::CppElemType;
    using base_t::_safe_index;
    using base_t::content_is_const;
public:
    // наследуем конструкторы
    using base_t::base_t;
    //TODO  А почему именно из && ??
    //      плюс еще видимо из своих ссылок можно.. и т.п.
    StlStringWrapper(esb::String&& arg_) requires(not std::is_reference_v<CppStringTy>)
        : base_t{ convert_to<std::remove_cvref_t<base_t::ESB_CONTAINED_DATA_T>>(std::move(arg_)) }
    { }

    //  Методы получения/установки значения из 1С 
    //TODO  Перенести в WrapperBase для всех типов, которые "esb_compatable", а TODO которые (convert_to) могут конвертироваться из-ту esb-типов
    esb::String ESB_WRAPPER_STD_METH_GetData() const
    {
        return convert_to<esb::String>(this->Data());
    }
    void ESB_WRAPPER_STD_METH_SetData(const esb::String& s)                      requires not content_is_const
    { 
        this->Data() = convert_to<base_t::ESB_CONTAINED_DATA_NO_REF_T>(s);
    }
    //NOTE  Переопределяем методы присваивания значения элементу контейнера (и Find)
    //      У StlIndexedContainerWrapperBase они используют convert_to_arg<CppElemType>(EsbElemTypeIn)
    //      что в нашем случаем означает convert_to_arg<char|char16_t>(String)
    //      делать глобальный конвертер из String-в-char не красиво и неправильно
    //      т.к. например не понятно как в чар конвертировать пустую строку и что это значит
    //      (пс. преобразование char-в-String сделано глобально и это нормально)
    //
    // Пока делаем "жесткую" логику - только строка из одниго символа иначе исключение
    //
    //TODO  Мы можем принимать OneOf<String, Numeric> и непосредственно трактовать Numeric как код символа (или ош)
    //
    static CppElemType _get_first_char(const String& string_) {
        if (string_.length() != 1)
            ESL_THROW_EXPECTED_ONE_CHAR_STRING();

        return convert_to_string_char<CppElemType>(string_.view()[0]);
    }
    void ESB_CONTAINED_IXCOLLECTION_SETVALUE(size_t index_, IValuePtr&& value_) requires not content_is_const
    {
        _[_safe_index(index_)] = _get_first_char(convert_to_arg<String>(std::move(value_)));
    }
    void Set(const Numeric& index_, const String& value_) requires not content_is_const
    {
        _[_safe_index(index_)] = _get_first_char(value_);
    }
    esb::UndefOr<Numeric> Find(const String& value_) const
    {
        //TODO  Здесь можно сделать логику strstr, то есть позиция подстроки (или лучше отдельным методом?)
        auto ch     = _get_first_char(value_);
        auto ch_pos = _.find_first_of(ch);
        if(ch_pos == contained_container_t::npos)
            return make_undef_or<Numeric>();
        else
            return make_undef_or<Numeric>(ch_pos);
    }
};


template<typename ObjectTy, typename DescriptorInfoTy>
struct stl_interface_for_string_ro_face //: esb::_internal::interface_empty_face_prop
{
    static ESB_META_INTERFACE_EVAL_MODE auto make_meta_interface_meth()
    {
        return std::tuple_cat
        (
            stl_interface_for_indexed_collection_r<ObjectTy>::make_meta_interface_meth(),
            ex_class_wrapper_interface_base<ObjectTy, DescriptorInfoTy>::meth_cref(),
            esb::_internal::make_meta_interface_meth
            (
                ESB_META_INTERFACE_MAKE_METH_EX(&ObjectTy::ESB_WRAPPER_STD_METH_GetData, ESB_WRAPPER_STD_METH_GetData_En, ESB_WRAPPER_STD_METH_GetData_Ru)
            )
        );
    }
    static ESB_META_INTERFACE_EVAL_MODE auto make_meta_interface_prop()
    {
        return	esb::_internal::make_meta_interface_prop
        (
            //TODO  ESB_WRAPPER_STD_PROP_Data здесь использовать нельзя, 
            //      т.к. ESB_META_INTERFACE_MAKE_PROP_RO к нему Get подставляет и GetESB_WRAPPER_STD_PROP_Data получается
            //      Нужно каклй-то ..EX как для методов
            ESB_META_INTERFACE_MAKE_PROP_RO(ObjectTy ::, Data, ESB_WRAPPER_STD_PROP_Data_En, ESB_WRAPPER_STD_PROP_Data_Ru)
        );
    }
};
//
template<typename ObjectTy, typename DescriptorInfoTy>
struct stl_interface_for_string_rw_face : stl_interface_for_string_ro_face<ObjectTy, DescriptorInfoTy>
{
    static ESB_META_INTERFACE_EVAL_MODE auto make_meta_interface_meth()
    {
        return std::tuple_cat
        (            
            stl_interface_for_string_ro_face<ObjectTy, DescriptorInfoTy>::make_meta_interface_meth(),
            stl_interface_for_indexed_collection_updatable<ObjectTy>::make_meta_interface_meth(),
            ex_class_wrapper_interface_base<ObjectTy, DescriptorInfoTy>::meth_vref(),
            esb::_internal::make_meta_interface_meth
            (
                ESB_META_INTERFACE_MAKE_METH_EX(&ObjectTy::ESB_WRAPPER_STD_METH_SetData, ESB_WRAPPER_STD_METH_SetData_En, ESB_WRAPPER_STD_METH_SetData_Ru)
            )
        );
    }
    //NOTE  Мы здесь переопределяем а не доопределяем make_meta_interface_prop из -ro-интерфейса, 
    //      т.к. там ESB_META_INTERFACE_MAKE_PROP_RO, а здесь нужно ESB_META_INTERFACE_MAKE_PROP_RW
    //      (так устроено...)
    static ESB_META_INTERFACE_EVAL_MODE auto make_meta_interface_prop()
    {
        return	esb::_internal::make_meta_interface_prop
        (
            ESB_META_INTERFACE_MAKE_PROP_RW(ObjectTy ::, Data, ESB_WRAPPER_STD_PROP_Data_En, ESB_WRAPPER_STD_PROP_Data_Ru)
        );
    }
};
//
//TODO конструкторы из 1С отсутствуют
template<typename CppStringTy, typename ObjectTy, typename DescriptorInfoTy>
struct stl_interface_for_string_ro : stl_interface_for_string_ro_face<ObjectTy, DescriptorInfoTy>, esb::_internal::interface_empty_ctor
{ };
template<typename CppStringTy, typename ObjectTy, typename DescriptorInfoTy>
struct stl_interface_for_string_rw : stl_interface_for_string_rw_face<ObjectTy, DescriptorInfoTy>
{ 
    static ESB_META_INTERFACE_EVAL_MODE auto make_meta_interface_ctor()
    {
        if constexpr (std::is_reference_v<CppStringTy>)
            return esb::_internal::make_meta_interface_ctor();      //TODO у ссылок конструкторы из 1С отсутствуют
        else
            return	esb::_internal::make_meta_interface_ctor
            (
                ESB_META_CTOR(),
                ESB_META_CTOR(String)
            );
    }
};
//
template<typename CppStringTy, typename EsbCompatableTy>
struct template_interface_descriptor< StlStringWrapper<CppStringTy, EsbCompatableTy> >
    : stl_interface_descriptor<StlStringWrapper, StlStringValue, CppStringTy, EsbCompatableTy, stl_interface_for_string_ro, stl_interface_for_string_rw>
{ };


#define ESB_WRAPPER_FOR_STL_STRING_EX(STRING_TY_, COMPATABLE_TY_, CLASS_VALUE_EN_, CLASS_VALUE_RU_, GUID_, DESCRIPTION_)				\
	ESB_CLASS_WRAPPER_FROM_TEMPLATE((StlStringWrapper< PP_EXTRACT STRING_TY_, stl_compatable_types<PP_EXTRACT COMPATABLE_TY_> >), CLASS_VALUE_EN_, CLASS_VALUE_RU_, GUID_, DESCRIPTION_)

#define ESB_WRAPPER_FOR_STL_STRING(STRING_TY_, CLASS_VALUE_EN_, CLASS_VALUE_RU_, GUID_, DESCRIPTION_)							        \
    ESB_WRAPPER_FOR_STL_STRING_EX(STRING_TY_, (String), CLASS_VALUE_EN_, CLASS_VALUE_RU_, GUID_, DESCRIPTION_)



} //namespace esb 


#endif //ESB_STL_STRING_H