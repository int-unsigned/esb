/*
	"ESB" is designed to create external native components for the 1C(tm) program.
	Copyright © 2023 IntUnsigned	(v8classes@gmail.com)
	"ESB" is free software under LGPLv2.1 license with essential restriction about binary part (view esb-license.txt)
	"ESB" IS DISTRIBUTED "AS-IS" WITHOUT ANY, EVEN THE IMPLIED, WARRANTY. IN NO EVENT SHALL THE AUTHORS OR THE COPYRIGHT OWNER BE LIABLE FOR ANY CLAIM, 
	DAMAGES OR OTHER LIABILITY ARISING OUT OF THE USE OF THIS SOFTWARE.
	THE AUTHOR DOES NOT GIVE ANY GUARANTEES AND DOES NOT BEAR ANY RESPONSIBILITY REGARDING THE USE OF 1C(tm) COMPONENTS PRODUCED USING THE ESB LIBRARY IN THE 1C(tm) ENVIRONMENT.
*/


#pragma once
#ifndef ESB_ES1_MAIN_H_
#define ESB_ES1_MAIN_H_

#include "esb/esb_base.h"
#include "esdisp_main.h"




namespace esb 
{
	//class ManagedForm;
	using ManagedForm = Arbitrary;
#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
	using УправляемаяФорма = ManagedForm;
#endif
}

namespace esb 
{
	class AddInType;
	class AllowedLength;
	class AllowedSign;
	class ColorType;
	class ControlBorderType;
	class BorderType;
	class DateFractions;
	class FontType;
	class MessageStatus;
	class PictureFormat;
	class PictureType;
	class QuestionDialogMode;
	class SearchDirection;
	class SortDirection;
	class StringEncodingMethod;
	class CompareValues;
	class NumberQualifiers;
	class StringQualifiers;
	class DateQualifiers;
	class BinaryDataQualifiers;
	class TypeDescription;
	class CommandInterfaceCommand;
	class CommonModule;
	class NotifyDescription;
	class Border;
	class Color;
	class Picture;
	class Font;
	class FormattedString;


#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
	using ТипВнешнейКомпоненты = AddInType;
	using ДопустимаяДлина = AllowedLength;
	using ДопустимыйЗнак = AllowedSign;
	using ВидЦвета = ColorType;
	using ТипРамкиЭлементаУправления = ControlBorderType;
	using ВидРамки = BorderType;
	using ЧастиДаты = DateFractions;
	using ВидШрифта = FontType;
	using СтатусСообщения = MessageStatus;
	using ФорматКартинки = PictureFormat;
	using ВидКартинки = PictureType;
	using РежимДиалогаВопрос = QuestionDialogMode;
	using НаправлениеПоиска = SearchDirection;
	using НаправлениеСортировки = SortDirection;
	using СпособКодированияСтроки = StringEncodingMethod;
	using СравнениеЗначений = CompareValues;
	using КвалификаторыЧисла = NumberQualifiers;
	using КвалификаторыСтроки = StringQualifiers;
	using КвалификаторыДаты = DateQualifiers;
	using КвалификаторыДвоичныхДанных = BinaryDataQualifiers;
	using ОписаниеТипов = TypeDescription;
	using КомандаКомандногоИнтерфейса = CommandInterfaceCommand;
	using ОбщийМодуль = CommonModule;
	using ОписаниеОповещения = NotifyDescription;
	using Рамка = Border;
	using Цвет = Color;
	using Картинка = Picture;
	using Шрифт = Font;
	using ФорматированнаяСтрока = FormattedString;
#endif	// #if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN

	// определено в _file.h
	class BinaryData;
#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
	using ДвоичныеДанные = BinaryData;
#endif



ESB_CLASS_ENUM(AddInType)
{	ESB_CLASS_IMPLEMENT_ENUM(AddInType)
public:
	ESB_CLASS_ENUM_ITEM(COM)
	ESB_CLASS_ENUM_ITEM(Native)
};


ESB_CLASS_ENUM(AllowedLength)
{	ESB_CLASS_IMPLEMENT_ENUM(AllowedLength)
public:
	ESB_CLASS_ENUM_ITEM(Fixed, Фиксированная)
	ESB_CLASS_ENUM_ITEM(Variable, Переменная)
};


ESB_CLASS_ENUM(AllowedSign)
{	ESB_CLASS_IMPLEMENT_ENUM(AllowedSign)
public:
	ESB_CLASS_ENUM_ITEM(Any, Любой)
	ESB_CLASS_ENUM_ITEM(Nonnegative, Неотрицательный)
};


ESB_CLASS_ENUM(ColorType)
{	ESB_CLASS_IMPLEMENT_ENUM(ColorType)
public:
	ESB_CLASS_ENUM_ITEM(Absolute, Абсолютный)
	ESB_CLASS_ENUM_ITEM(StyleItem, ЭлементСтиля)
	ESB_CLASS_ENUM_ITEM(WebColor, WebЦвет)
	ESB_CLASS_ENUM_ITEM(WindowsColor, WindowsЦвет)
	ESB_CLASS_ENUM_ITEM(AutoColor, АвтоЦвет)
};


ESB_CLASS_ENUM(ControlBorderType)
{	ESB_CLASS_IMPLEMENT_ENUM(ControlBorderType)
public:
	ESB_CLASS_ENUM_ITEM(WithoutBorder, БезРамки)
	ESB_CLASS_ENUM_ITEM(Double, Двойная)
	ESB_CLASS_ENUM_ITEM(Single, Одинарная)
	ESB_CLASS_ENUM_ITEM(Embossed, Выпуклая)
	ESB_CLASS_ENUM_ITEM(Indented, Вдавленная)
	ESB_CLASS_ENUM_ITEM(Underline, Подчеркивание)
	ESB_CLASS_ENUM_ITEM(DoubleUnderline, ДвойноеПодчеркивание)
	ESB_CLASS_ENUM_ITEM(Rounded, Скругленная)
	ESB_CLASS_ENUM_ITEM(Overline, ЧертаСверху)
};


ESB_CLASS_ENUM(DateFractions)
{	ESB_CLASS_IMPLEMENT_ENUM(DateFractions)
public:
	ESB_CLASS_ENUM_ITEM(Date, Дата)
	ESB_CLASS_ENUM_ITEM(Time, Время)
	ESB_CLASS_ENUM_ITEM(DateTime, ДатаВремя)
};


ESB_CLASS_ENUM(FontType)
{	ESB_CLASS_IMPLEMENT_ENUM(FontType)
public:
	ESB_CLASS_ENUM_ITEM(Absolute, Абсолютный)
	ESB_CLASS_ENUM_ITEM(StyleItem, ЭлементСтиля)
	ESB_CLASS_ENUM_ITEM(WindowsFont, WindowsШрифт)
	ESB_CLASS_ENUM_ITEM(AutoFont, АвтоШрифт)
};


ESB_CLASS_ENUM(MessageStatus)
{	ESB_CLASS_IMPLEMENT_ENUM(MessageStatus)
public:
	ESB_CLASS_ENUM_ITEM(WithoutStatus, БезСтатуса)
	ESB_CLASS_ENUM_ITEM(Ordinary, Обычное)
	ESB_CLASS_ENUM_ITEM(Information, Информация)
	ESB_CLASS_ENUM_ITEM(Attention, Внимание)
	ESB_CLASS_ENUM_ITEM(Important, Важное)
	ESB_CLASS_ENUM_ITEM(VeryImportant, ОченьВажное)
};


ESB_CLASS_ENUM(PictureFormat)
{	ESB_CLASS_IMPLEMENT_ENUM(PictureFormat)
public:
	ESB_CLASS_ENUM_ITEM(UnknownFormat, НеизвестныйФормат)
	ESB_CLASS_ENUM_ITEM(BMP)
	ESB_CLASS_ENUM_ITEM(GIF)
	ESB_CLASS_ENUM_ITEM(Icon)
	ESB_CLASS_ENUM_ITEM(JPEG)
	ESB_CLASS_ENUM_ITEM(PNG)
	ESB_CLASS_ENUM_ITEM(TIFF)
	ESB_CLASS_ENUM_ITEM(EMF)
	ESB_CLASS_ENUM_ITEM(WMF)
	ESB_CLASS_ENUM_ITEM(SVG)
};


ESB_CLASS_ENUM(PictureType)
{	ESB_CLASS_IMPLEMENT_ENUM(PictureType)
public:
	ESB_CLASS_ENUM_ITEM(Empty, Пустая)
	ESB_CLASS_ENUM_ITEM(FromLib, ИзБиблиотеки)
	ESB_CLASS_ENUM_ITEM(Absolute, Абсолютная)
};


ESB_CLASS_ENUM(QuestionDialogMode)
{	ESB_CLASS_IMPLEMENT_ENUM(QuestionDialogMode)
public:
	ESB_CLASS_ENUM_ITEM(OK, ОК)
	ESB_CLASS_ENUM_ITEM(OKCancel, ОКОтмена)
	ESB_CLASS_ENUM_ITEM(AbortRetryIgnore, ПрерватьПовторитьПропустить)
	ESB_CLASS_ENUM_ITEM(YesNoCancel, ДаНетОтмена)
	ESB_CLASS_ENUM_ITEM(YesNo, ДаНет)
	ESB_CLASS_ENUM_ITEM(RetryCancel, ПовторитьОтмена)
};


ESB_CLASS_ENUM(StringEncodingMethod)
{	ESB_CLASS_IMPLEMENT_ENUM(StringEncodingMethod)
public:
	ESB_CLASS_ENUM_ITEM(URLEncoding, КодировкаURL)
	ESB_CLASS_ENUM_ITEM(URLInURLEncoding, URLВКодировкеURL)
};


ESB_CLASS_ENUM(BorderType)
{	ESB_CLASS_IMPLEMENT_ENUM(BorderType)
public:
	ESB_CLASS_ENUM_ITEM(Absolute, Абсолютная)
	ESB_CLASS_ENUM_ITEM(StyleItem, ЭлементСтиля)
};




ESB_CLASS_OBJECT(CompareValues)
{
	ESB_CLASS_IMPLEMENT_OBJECT(CompareValues)
#if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
public:
	static CompareValues Create() { ESB_INVOKE_CTOR0(CompareValues); }
public:
	// Функция Сравнить(Значение1_ Как Произвольный, Значение2_ Как Произвольный) Возвращает Число
	Number Compare(ConstParam<Arbitrary> Value1_, ConstParam<Arbitrary> Value2_) const { ESB_INVOKE_MEMB_FUNC2(esb::Number, MethId::Compare, Value1_, Value2_); };
#endif	// #if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
public:
	static СравнениеЗначений Создать() { ESB_INVOKE_CTOR0(СравнениеЗначений); }
public:
	// Function Compare(Arbitrary Value1_, Arbitrary Value2_) -> Number
	Число Сравнить(КонстПарам<Произвольный> Значение1_, КонстПарам<Произвольный> Значение2_) const { ESB_INVOKE_MEMB_FUNC2(esb::Число, MethId::Compare, Значение1_, Значение2_); };
#endif	// #if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
};


ESB_CLASS_OBJECT(NumberQualifiers)
{
	ESB_CLASS_IMPLEMENT_OBJECT(NumberQualifiers)
#if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
public:
	static NumberQualifiers Create() { ESB_INVOKE_CTOR0(NumberQualifiers); }  // no information about this constructor, but present in disp-info.
	static NumberQualifiers Create(ConstPara<Numeric> NumberOfDigits_) { ESB_INVOKE_CTOR1(NumberQualifiers, NumberOfDigits_); }
	static NumberQualifiers Create(ConstPara<Numeric> NumberOfDigits_, ConstPara<Numeric> NumberOfDigitsAfterTheDecimalPoint_) { ESB_INVOKE_CTOR2(NumberQualifiers, NumberOfDigits_, NumberOfDigitsAfterTheDecimalPoint_); }
	static NumberQualifiers Create(ConstPara<Numeric> NumberOfDigits_, ConstPara<Numeric> NumberOfDigitsAfterTheDecimalPoint_, ConstPara<AllowedSign> AllowedSign_) { ESB_INVOKE_CTOR3(NumberQualifiers, NumberOfDigits_, NumberOfDigitsAfterTheDecimalPoint_, AllowedSign_); }

public:
	// Свойство [[ТолькоЧтение]] ДопустимыйЗнак Как ДопустимыйЗнак
	AllowedSign GetAllowedSign() const { ESB_INVOKE_PROPERTY_GET(esb::AllowedSign, PropId::AllowedSign); };
	ESB_PROPERTY_FIELD_RO(AllowedSign, AllowedSign)
		// Свойство [[ТолькоЧтение]] Разрядность Как Число
		Number GetDigits() const { ESB_INVOKE_PROPERTY_GET(esb::Number, PropId::Digits); };
	ESB_PROPERTY_FIELD_RO(Number, Digits)
		// Свойство [[ТолькоЧтение]] РазрядностьДробнойЧасти Как Число
		Number GetFractionDigits() const { ESB_INVOKE_PROPERTY_GET(esb::Number, PropId::FractionDigits); };
	ESB_PROPERTY_FIELD_RO(Number, FractionDigits)
#endif	// #if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
public:
	static КвалификаторыЧисла Создать() { ESB_INVOKE_CTOR0(КвалификаторыЧисла); }  // no information about this constructor, but present in disp-info.
	static КвалификаторыЧисла Создать(КонстПарам<Число> ЧислоРазрядов_ = ПоУмолчаниюНеопределеноИли<esb::Число>, КонстПарам<Число> ЧислоРазрядовДробнойЧасти_ = ПоУмолчаниюНеопределеноИли<esb::Число>, КонстПарам<ДопустимыйЗнак> ДопустимыйЗнак_ = ПоУмолчаниюНеопределеноИли<esb::ДопустимыйЗнак>) { ESB_INVOKE_CTOR3(КвалификаторыЧисла, ЧислоРазрядов_, ЧислоРазрядовДробнойЧасти_, ДопустимыйЗнак_); }
public:
	// Property [[ReadOnly]] AllowedSign -> AllowedSign
	ДопустимыйЗнак ПолучитьДопустимыйЗнак() const { ESB_INVOKE_PROPERTY_GET(esb::ДопустимыйЗнак, PropId::AllowedSign); };
	ESB_PROPERTY_FIELD_RO(ДопустимыйЗнак, ДопустимыйЗнак)
		// Property [[ReadOnly]] Digits -> Number
		Число ПолучитьРазрядность() const { ESB_INVOKE_PROPERTY_GET(esb::Число, PropId::Digits); };
	ESB_PROPERTY_FIELD_RO(Число, Разрядность)
		// Property [[ReadOnly]] FractionDigits -> Number
		Число ПолучитьРазрядностьДробнойЧасти() const { ESB_INVOKE_PROPERTY_GET(esb::Число, PropId::FractionDigits); };
	ESB_PROPERTY_FIELD_RO(Число, РазрядностьДробнойЧасти)
#endif	// #if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
};


ESB_CLASS_OBJECT(StringQualifiers)
{
	ESB_CLASS_IMPLEMENT_OBJECT(StringQualifiers)
#if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
public:
	static StringQualifiers Create() { ESB_INVOKE_CTOR0(StringQualifiers); }  // no information about this constructor, but present in disp-info.
	static StringQualifiers Create(ConstParam<OneOf<Number,Undef>> StringLength_, ConstParam<AllowedLength> AllowedLength_ = DefUndef<esb::AllowedLength>) { ESB_INVOKE_CTOR2(StringQualifiers, StringLength_, AllowedLength_); }
public:
	// Свойство [[ТолькоЧтение]] ДопустимаяДлина Как ДопустимаяДлина
	AllowedLength GetAllowedLength() const { ESB_INVOKE_PROPERTY_GET(esb::AllowedLength, PropId::AllowedLength); };
	ESB_PROPERTY_FIELD_RO(AllowedLength, AllowedLength)
		// Свойство [[ТолькоЧтение]] Длина Как Число
		Number GetLength() const { ESB_INVOKE_PROPERTY_GET(esb::Number, PropId::Length); };
	ESB_PROPERTY_FIELD_RO(Number, Length)
#endif	// #if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
public:
	static КвалификаторыСтроки Создать() { ESB_INVOKE_CTOR0(КвалификаторыСтроки); }  // no information about this constructor, but present in disp-info.
	static КвалификаторыСтроки Создать(КонстПарам<Число> ДлинаСтроки_ = ПоУмолчаниюНеопределеноИли<esb::Число>, КонстПарам<ДопустимаяДлина> ДопустимаяДлина_ = ПоУмолчаниюНеопределеноИли<esb::ДопустимаяДлина>) { ESB_INVOKE_CTOR2(КвалификаторыСтроки, ДлинаСтроки_, ДопустимаяДлина_); }
public:
	// Property [[ReadOnly]] AllowedLength -> AllowedLength
	ДопустимаяДлина ПолучитьДопустимаяДлина() const { ESB_INVOKE_PROPERTY_GET(esb::ДопустимаяДлина, PropId::AllowedLength); };
	ESB_PROPERTY_FIELD_RO(ДопустимаяДлина, ДопустимаяДлина)
		// Property [[ReadOnly]] Length -> Number
		Число ПолучитьДлина() const { ESB_INVOKE_PROPERTY_GET(esb::Число, PropId::Length); };
	ESB_PROPERTY_FIELD_RO(Число, Длина)
#endif	// #if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
};


ESB_CLASS_OBJECT(DateQualifiers)
{
	ESB_CLASS_IMPLEMENT_OBJECT(DateQualifiers)
#if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
public:
	static DateQualifiers Create() { ESB_INVOKE_CTOR0(DateQualifiers); }  // no information about this constructor, but present in disp-info.
	static DateQualifiers Create(ConstParam<DateFractions> DateFractions_ = DefUndef<esb::DateFractions>) { ESB_INVOKE_CTOR1(DateQualifiers, DateFractions_); }
public:
	// Свойство [[ТолькоЧтение]] ЧастиДаты Как ЧастиДаты
	DateFractions GetDateFractions() const { ESB_INVOKE_PROPERTY_GET(esb::DateFractions, PropId::DateFractions); };
	ESB_PROPERTY_FIELD_RO(DateFractions, DateFractions)
#endif	// #if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
public:
	static КвалификаторыДаты Создать() { ESB_INVOKE_CTOR0(КвалификаторыДаты); }  // no information about this constructor, but present in disp-info.
	static КвалификаторыДаты Создать(КонстПарам<ЧастиДаты> ЧастиДаты_ = ПоУмолчаниюНеопределеноИли<esb::ЧастиДаты>) { ESB_INVOKE_CTOR1(КвалификаторыДаты, ЧастиДаты_); }
public:
	// Property [[ReadOnly]] DateFractions -> DateFractions
	ЧастиДаты ПолучитьЧастиДаты() const { ESB_INVOKE_PROPERTY_GET(esb::ЧастиДаты, PropId::DateFractions); };
	ESB_PROPERTY_FIELD_RO(ЧастиДаты, ЧастиДаты)
#endif	// #if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
};


ESB_CLASS_OBJECT(BinaryDataQualifiers)
{
	ESB_CLASS_IMPLEMENT_OBJECT(BinaryDataQualifiers)
#if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
public:
	static BinaryDataQualifiers Create() { ESB_INVOKE_CTOR0(BinaryDataQualifiers); }
	static BinaryDataQualifiers Create(ConstParam<Number> Length_ = DefUndef<esb::Number>, ConstParam<AllowedLength> AllowedLength_ = DefUndef<esb::AllowedLength>) { ESB_INVOKE_CTOR2(BinaryDataQualifiers, Length_, AllowedLength_); }
public:
	// Свойство [[ТолькоЧтение]] ДопустимаяДлина Как ДопустимаяДлина
	AllowedLength GetAllowedLength() const { ESB_INVOKE_PROPERTY_GET(esb::AllowedLength, PropId::AllowedLength); };
	ESB_PROPERTY_FIELD_RO(AllowedLength, AllowedLength)
		// Свойство [[ТолькоЧтение]] Длина Как Число
		Number GetLength() const { ESB_INVOKE_PROPERTY_GET(esb::Number, PropId::Length); };
	ESB_PROPERTY_FIELD_RO(Number, Length)
#endif	// #if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
public:
	static КвалификаторыДвоичныхДанных Создать() { ESB_INVOKE_CTOR0(КвалификаторыДвоичныхДанных); }
	static КвалификаторыДвоичныхДанных Создать(КонстПарам<Число> Длина_ = ПоУмолчаниюНеопределеноИли<esb::Число>, КонстПарам<ДопустимаяДлина> ДопустимаяДлина_ = ПоУмолчаниюНеопределеноИли<esb::ДопустимаяДлина>) { ESB_INVOKE_CTOR2(КвалификаторыДвоичныхДанных, Длина_, ДопустимаяДлина_); }
public:
	// Property [[ReadOnly]] AllowedLength -> AllowedLength
	ДопустимаяДлина ПолучитьДопустимаяДлина() const { ESB_INVOKE_PROPERTY_GET(esb::ДопустимаяДлина, PropId::AllowedLength); };
	ESB_PROPERTY_FIELD_RO(ДопустимаяДлина, ДопустимаяДлина)
		// Property [[ReadOnly]] Length -> Number
		Число ПолучитьДлина() const { ESB_INVOKE_PROPERTY_GET(esb::Число, PropId::Length); };
	ESB_PROPERTY_FIELD_RO(Число, Длина)
#endif	// #if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
};


ESB_CLASS_OBJECT(TypeDescription)
{
	ESB_CLASS_IMPLEMENT_OBJECT(TypeDescription)
#if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
public:
	static TypeDescription Create() { ESB_INVOKE_CTOR0(TypeDescription); }  // no information about this constructor, but present in disp-info.
	static TypeDescription Create(ConstParam<TypeDescription> SourceDescriptionOfTypes_ = DefUndef<esb::TypeDescription>, ConstParam<String> AddedTypes_ = DefUndef<esb::String>, ConstParam<String> RemovedTypes_ = DefUndef<esb::String>, ConstParam<NumberQualifiers> NumberQualifiers_ = DefUndef<esb::NumberQualifiers>, ConstParam<StringQualifiers> StringQualifiers_ = DefUndef<esb::StringQualifiers>, ConstParam<DateQualifiers> DateQualifiers_ = DefUndef<esb::DateQualifiers>, ConstParam<BinaryDataQualifiers> BinaryDataQualifiers_ = DefUndef<esb::BinaryDataQualifiers>) { ESB_INVOKE_CTOR7(TypeDescription, SourceDescriptionOfTypes_, AddedTypes_, RemovedTypes_, NumberQualifiers_, StringQualifiers_, DateQualifiers_, BinaryDataQualifiers_); }
	static TypeDescription Create(ConstParam<TypeDescription> SourceDescriptionOfTypes_ = DefUndef<esb::TypeDescription>, ConstParam<String> AddedTypes_ = DefUndef<esb::String>, ConstParam<Array> RemovedTypes_ = DefUndef<esb::Array>, ConstParam<NumberQualifiers> NumberQualifiers_ = DefUndef<esb::NumberQualifiers>, ConstParam<StringQualifiers> StringQualifiers_ = DefUndef<esb::StringQualifiers>, ConstParam<DateQualifiers> DateQualifiers_ = DefUndef<esb::DateQualifiers>, ConstParam<BinaryDataQualifiers> BinaryDataQualifiers_ = DefUndef<esb::BinaryDataQualifiers>) { ESB_INVOKE_CTOR7(TypeDescription, SourceDescriptionOfTypes_, AddedTypes_, RemovedTypes_, NumberQualifiers_, StringQualifiers_, DateQualifiers_, BinaryDataQualifiers_); }
	static TypeDescription Create(ConstParam<TypeDescription> SourceDescriptionOfTypes_ = DefUndef<esb::TypeDescription>, ConstParam<Array> AddedTypes_ = DefUndef<esb::Array>, ConstParam<String> RemovedTypes_ = DefUndef<esb::String>, ConstParam<NumberQualifiers> NumberQualifiers_ = DefUndef<esb::NumberQualifiers>, ConstParam<StringQualifiers> StringQualifiers_ = DefUndef<esb::StringQualifiers>, ConstParam<DateQualifiers> DateQualifiers_ = DefUndef<esb::DateQualifiers>, ConstParam<BinaryDataQualifiers> BinaryDataQualifiers_ = DefUndef<esb::BinaryDataQualifiers>) { ESB_INVOKE_CTOR7(TypeDescription, SourceDescriptionOfTypes_, AddedTypes_, RemovedTypes_, NumberQualifiers_, StringQualifiers_, DateQualifiers_, BinaryDataQualifiers_); }
	static TypeDescription Create(ConstParam<TypeDescription> SourceDescriptionOfTypes_ = DefUndef<esb::TypeDescription>, ConstParam<Array> AddedTypes_ = DefUndef<esb::Array>, ConstParam<Array> RemovedTypes_ = DefUndef<esb::Array>, ConstParam<NumberQualifiers> NumberQualifiers_ = DefUndef<esb::NumberQualifiers>, ConstParam<StringQualifiers> StringQualifiers_ = DefUndef<esb::StringQualifiers>, ConstParam<DateQualifiers> DateQualifiers_ = DefUndef<esb::DateQualifiers>, ConstParam<BinaryDataQualifiers> BinaryDataQualifiers_ = DefUndef<esb::BinaryDataQualifiers>) { ESB_INVOKE_CTOR7(TypeDescription, SourceDescriptionOfTypes_, AddedTypes_, RemovedTypes_, NumberQualifiers_, StringQualifiers_, DateQualifiers_, BinaryDataQualifiers_); }
	static TypeDescription Create(ConstParam<String> Types_, ConstParam<NumberQualifiers> NumberQualifiers_ = DefUndef<esb::NumberQualifiers>, ConstParam<StringQualifiers> StringQualifiers_ = DefUndef<esb::StringQualifiers>, ConstParam<DateQualifiers> DateQualifiers_ = DefUndef<esb::DateQualifiers>, ConstParam<BinaryDataQualifiers> BinaryDataQualifiers_ = DefUndef<esb::BinaryDataQualifiers>) { ESB_INVOKE_CTOR5(TypeDescription, Types_, NumberQualifiers_, StringQualifiers_, DateQualifiers_, BinaryDataQualifiers_); }
	static TypeDescription Create(ConstParam<Array> Types_, ConstParam<NumberQualifiers> NumberQualifiers_ = DefUndef<esb::NumberQualifiers>, ConstParam<StringQualifiers> StringQualifiers_ = DefUndef<esb::StringQualifiers>, ConstParam<DateQualifiers> DateQualifiers_ = DefUndef<esb::DateQualifiers>, ConstParam<BinaryDataQualifiers> BinaryDataQualifiers_ = DefUndef<esb::BinaryDataQualifiers>) { ESB_INVOKE_CTOR5(TypeDescription, Types_, NumberQualifiers_, StringQualifiers_, DateQualifiers_, BinaryDataQualifiers_); }
public:
	// Функция ПривестиЗначение([Значение_ Как Произвольный]) Возвращает Произвольный
	Arbitrary AdjustValue(ConstParam<Arbitrary> Value_ = DefUndef<esb::Arbitrary>) const { ESB_INVOKE_MEMB_FUNC1(esb::Arbitrary, MethId::AdjustValue, Value_); };
	// Функция СодержитТип(Тип_ Как Тип) Возвращает Булево
	Boolean ContainsType(ConstParam<TypeValue> type_) const { ESB_INVOKE_MEMB_FUNC1(esb::Boolean, MethId::ContainsType, type_); };
	// Функция Типы() Возвращает МассивИз<Тип>
	ArrayOf<TypeValue> Types() const { ESB_INVOKE_MEMB_FUNC0(esb::ArrayOf<TypeValue>, MethId::Types); };
public:
	// Свойство [[ТолькоЧтение]] КвалификаторыДвоичныхДанных Как КвалификаторыДвоичныхДанных
	BinaryDataQualifiers GetBinaryDataQualifiers() const { ESB_INVOKE_PROPERTY_GET(esb::BinaryDataQualifiers, PropId::BinaryDataQualifiers); };
	ESB_PROPERTY_FIELD_RO(BinaryDataQualifiers, BinaryDataQualifiers)
		// Свойство [[ТолькоЧтение]] КвалификаторыДаты Как КвалификаторыДаты
		DateQualifiers GetDateQualifiers() const { ESB_INVOKE_PROPERTY_GET(esb::DateQualifiers, PropId::DateQualifiers); };
	ESB_PROPERTY_FIELD_RO(DateQualifiers, DateQualifiers)
		// Свойство [[ТолькоЧтение]] КвалификаторыЧисла Как КвалификаторыЧисла
		NumberQualifiers GetNumberQualifiers() const { ESB_INVOKE_PROPERTY_GET(esb::NumberQualifiers, PropId::NumberQualifiers); };
	ESB_PROPERTY_FIELD_RO(NumberQualifiers, NumberQualifiers)
		// Свойство [[ТолькоЧтение]] КвалификаторыСтроки Как КвалификаторыСтроки
		StringQualifiers GetStringQualifiers() const { ESB_INVOKE_PROPERTY_GET(esb::StringQualifiers, PropId::StringQualifiers); };
	ESB_PROPERTY_FIELD_RO(StringQualifiers, StringQualifiers)
#endif	// #if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
public:
	static ОписаниеТипов Создать() { ESB_INVOKE_CTOR0(ОписаниеТипов); }  // no information about this constructor, but present in disp-info.
	static ОписаниеТипов Создать(КонстПарам<ОписаниеТипов> ИсходноеОписаниеТипов_ = ПоУмолчаниюНеопределеноИли<esb::ОписаниеТипов>, КонстПарам<Строка> ДобавляемыеТипы_ = ПоУмолчаниюНеопределеноИли<esb::Строка>, КонстПарам<Строка> ВычитаемыеТипы_ = ПоУмолчаниюНеопределеноИли<esb::Строка>, КонстПарам<КвалификаторыЧисла> КвалификаторыЧисла_ = ПоУмолчаниюНеопределеноИли<esb::КвалификаторыЧисла>, КонстПарам<КвалификаторыСтроки> КвалификаторыСтроки_ = ПоУмолчаниюНеопределеноИли<esb::КвалификаторыСтроки>, КонстПарам<КвалификаторыДаты> КвалификаторыДаты_ = ПоУмолчаниюНеопределеноИли<esb::КвалификаторыДаты>, КонстПарам<КвалификаторыДвоичныхДанных> КвалификаторыДвоичныхДанных_ = ПоУмолчаниюНеопределеноИли<esb::КвалификаторыДвоичныхДанных>) { ESB_INVOKE_CTOR7(ОписаниеТипов, ИсходноеОписаниеТипов_, ДобавляемыеТипы_, ВычитаемыеТипы_, КвалификаторыЧисла_, КвалификаторыСтроки_, КвалификаторыДаты_, КвалификаторыДвоичныхДанных_); }
	static ОписаниеТипов Создать(КонстПарам<ОписаниеТипов> ИсходноеОписаниеТипов_ = ПоУмолчаниюНеопределеноИли<esb::ОписаниеТипов>, КонстПарам<Строка> ДобавляемыеТипы_ = ПоУмолчаниюНеопределеноИли<esb::Строка>, КонстПарам<Массив> ВычитаемыеТипы_ = ПоУмолчаниюНеопределеноИли<esb::Массив>, КонстПарам<КвалификаторыЧисла> КвалификаторыЧисла_ = ПоУмолчаниюНеопределеноИли<esb::КвалификаторыЧисла>, КонстПарам<КвалификаторыСтроки> КвалификаторыСтроки_ = ПоУмолчаниюНеопределеноИли<esb::КвалификаторыСтроки>, КонстПарам<КвалификаторыДаты> КвалификаторыДаты_ = ПоУмолчаниюНеопределеноИли<esb::КвалификаторыДаты>, КонстПарам<КвалификаторыДвоичныхДанных> КвалификаторыДвоичныхДанных_ = ПоУмолчаниюНеопределеноИли<esb::КвалификаторыДвоичныхДанных>) { ESB_INVOKE_CTOR7(ОписаниеТипов, ИсходноеОписаниеТипов_, ДобавляемыеТипы_, ВычитаемыеТипы_, КвалификаторыЧисла_, КвалификаторыСтроки_, КвалификаторыДаты_, КвалификаторыДвоичныхДанных_); }
	static ОписаниеТипов Создать(КонстПарам<ОписаниеТипов> ИсходноеОписаниеТипов_ = ПоУмолчаниюНеопределеноИли<esb::ОписаниеТипов>, КонстПарам<Массив> ДобавляемыеТипы_ = ПоУмолчаниюНеопределеноИли<esb::Массив>, КонстПарам<Строка> ВычитаемыеТипы_ = ПоУмолчаниюНеопределеноИли<esb::Строка>, КонстПарам<КвалификаторыЧисла> КвалификаторыЧисла_ = ПоУмолчаниюНеопределеноИли<esb::КвалификаторыЧисла>, КонстПарам<КвалификаторыСтроки> КвалификаторыСтроки_ = ПоУмолчаниюНеопределеноИли<esb::КвалификаторыСтроки>, КонстПарам<КвалификаторыДаты> КвалификаторыДаты_ = ПоУмолчаниюНеопределеноИли<esb::КвалификаторыДаты>, КонстПарам<КвалификаторыДвоичныхДанных> КвалификаторыДвоичныхДанных_ = ПоУмолчаниюНеопределеноИли<esb::КвалификаторыДвоичныхДанных>) { ESB_INVOKE_CTOR7(ОписаниеТипов, ИсходноеОписаниеТипов_, ДобавляемыеТипы_, ВычитаемыеТипы_, КвалификаторыЧисла_, КвалификаторыСтроки_, КвалификаторыДаты_, КвалификаторыДвоичныхДанных_); }
	static ОписаниеТипов Создать(КонстПарам<ОписаниеТипов> ИсходноеОписаниеТипов_ = ПоУмолчаниюНеопределеноИли<esb::ОписаниеТипов>, КонстПарам<Массив> ДобавляемыеТипы_ = ПоУмолчаниюНеопределеноИли<esb::Массив>, КонстПарам<Массив> ВычитаемыеТипы_ = ПоУмолчаниюНеопределеноИли<esb::Массив>, КонстПарам<КвалификаторыЧисла> КвалификаторыЧисла_ = ПоУмолчаниюНеопределеноИли<esb::КвалификаторыЧисла>, КонстПарам<КвалификаторыСтроки> КвалификаторыСтроки_ = ПоУмолчаниюНеопределеноИли<esb::КвалификаторыСтроки>, КонстПарам<КвалификаторыДаты> КвалификаторыДаты_ = ПоУмолчаниюНеопределеноИли<esb::КвалификаторыДаты>, КонстПарам<КвалификаторыДвоичныхДанных> КвалификаторыДвоичныхДанных_ = ПоУмолчаниюНеопределеноИли<esb::КвалификаторыДвоичныхДанных>) { ESB_INVOKE_CTOR7(ОписаниеТипов, ИсходноеОписаниеТипов_, ДобавляемыеТипы_, ВычитаемыеТипы_, КвалификаторыЧисла_, КвалификаторыСтроки_, КвалификаторыДаты_, КвалификаторыДвоичныхДанных_); }
	static ОписаниеТипов Создать(КонстПарам<Строка> Типы_, КонстПарам<КвалификаторыЧисла> КвалификаторыЧисла_ = ПоУмолчаниюНеопределеноИли<esb::КвалификаторыЧисла>, КонстПарам<КвалификаторыСтроки> КвалификаторыСтроки_ = ПоУмолчаниюНеопределеноИли<esb::КвалификаторыСтроки>, КонстПарам<КвалификаторыДаты> КвалификаторыДаты_ = ПоУмолчаниюНеопределеноИли<esb::КвалификаторыДаты>, КонстПарам<КвалификаторыДвоичныхДанных> КвалификаторыДвоичныхДанных_ = ПоУмолчаниюНеопределеноИли<esb::КвалификаторыДвоичныхДанных>) { ESB_INVOKE_CTOR5(ОписаниеТипов, Типы_, КвалификаторыЧисла_, КвалификаторыСтроки_, КвалификаторыДаты_, КвалификаторыДвоичныхДанных_); }
	static ОписаниеТипов Создать(КонстПарам<Массив> Типы_, КонстПарам<КвалификаторыЧисла> КвалификаторыЧисла_ = ПоУмолчаниюНеопределеноИли<esb::КвалификаторыЧисла>, КонстПарам<КвалификаторыСтроки> КвалификаторыСтроки_ = ПоУмолчаниюНеопределеноИли<esb::КвалификаторыСтроки>, КонстПарам<КвалификаторыДаты> КвалификаторыДаты_ = ПоУмолчаниюНеопределеноИли<esb::КвалификаторыДаты>, КонстПарам<КвалификаторыДвоичныхДанных> КвалификаторыДвоичныхДанных_ = ПоУмолчаниюНеопределеноИли<esb::КвалификаторыДвоичныхДанных>) { ESB_INVOKE_CTOR5(ОписаниеТипов, Типы_, КвалификаторыЧисла_, КвалификаторыСтроки_, КвалификаторыДаты_, КвалификаторыДвоичныхДанных_); }
public:
	// Function AdjustValue(Arbitrary Value_ = <default>) -> Arbitrary
	Произвольный ПривестиЗначение(КонстПарам<Произвольный> Значение_ = ПоУмолчаниюНеопределеноИли<esb::Произвольный>) const { ESB_INVOKE_MEMB_FUNC1(esb::Произвольный, MethId::AdjustValue, Значение_); };
	// Function ContainsType(Type Type_) -> Boolean
	Булево СодержитТип(КонстПарам<Тип> Тип_) const { ESB_INVOKE_MEMB_FUNC1(esb::Булево, MethId::ContainsType, Тип_); };
	// Function Types() -> ArrayOf<Type>
	МассивИз<Тип> Типы() const { ESB_INVOKE_MEMB_FUNC0(esb::МассивИз<Тип>, MethId::Types); };
public:
	// Property [[ReadOnly]] BinaryDataQualifiers -> BinaryDataQualifiers
	КвалификаторыДвоичныхДанных ПолучитьКвалификаторыДвоичныхДанных() const { ESB_INVOKE_PROPERTY_GET(esb::КвалификаторыДвоичныхДанных, PropId::BinaryDataQualifiers); };
	ESB_PROPERTY_FIELD_RO(КвалификаторыДвоичныхДанных, КвалификаторыДвоичныхДанных)
		// Property [[ReadOnly]] DateQualifiers -> DateQualifiers
		КвалификаторыДаты ПолучитьКвалификаторыДаты() const { ESB_INVOKE_PROPERTY_GET(esb::КвалификаторыДаты, PropId::DateQualifiers); };
	ESB_PROPERTY_FIELD_RO(КвалификаторыДаты, КвалификаторыДаты)
		// Property [[ReadOnly]] NumberQualifiers -> NumberQualifiers
		КвалификаторыЧисла ПолучитьКвалификаторыЧисла() const { ESB_INVOKE_PROPERTY_GET(esb::КвалификаторыЧисла, PropId::NumberQualifiers); };
	ESB_PROPERTY_FIELD_RO(КвалификаторыЧисла, КвалификаторыЧисла)
		// Property [[ReadOnly]] StringQualifiers -> StringQualifiers
		КвалификаторыСтроки ПолучитьКвалификаторыСтроки() const { ESB_INVOKE_PROPERTY_GET(esb::КвалификаторыСтроки, PropId::StringQualifiers); };
	ESB_PROPERTY_FIELD_RO(КвалификаторыСтроки, КвалификаторыСтроки)
#endif	// #if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
};


ESB_CLASS_OBJECT(CommandInterfaceCommand)
{
	ESB_CLASS_IMPLEMENT_OBJECT(CommandInterfaceCommand)
#if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
public:
	static CommandInterfaceCommand Create() { ESB_INVOKE_CTOR0(CommandInterfaceCommand); }  // no information about this constructor, but present in disp-info.
public:
	// Свойство [[ТолькоЧтение]] ЭтотОбъект Как КомандаКомандногоИнтерфейса
	CommandInterfaceCommand GetThisObject() const { ESB_INVOKE_PROPERTY_GET(esb::CommandInterfaceCommand, PropId::ThisObject); };
	ESB_PROPERTY_FIELD_RO(CommandInterfaceCommand, ThisObject)
#endif	// #if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
public:
	static КомандаКомандногоИнтерфейса Создать() { ESB_INVOKE_CTOR0(КомандаКомандногоИнтерфейса); }  // no information about this constructor, but present in disp-info.
public:
	// Property [[ReadOnly]] ThisObject -> CommandInterfaceCommand
	КомандаКомандногоИнтерфейса ПолучитьЭтотОбъект() const { ESB_INVOKE_PROPERTY_GET(esb::КомандаКомандногоИнтерфейса, PropId::ThisObject); };
	ESB_PROPERTY_FIELD_RO(КомандаКомандногоИнтерфейса, ЭтотОбъект)
#endif	// #if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
};


ESB_CLASS_OBJECT(CommonModule)
{
	ESB_CLASS_IMPLEMENT_OBJECT(CommonModule)
#if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
public:
	// Not Creatable
public:
	// !>NO-DISP-INFO: Void NameOfAProcedureOrAFunction()
public:
	// !>NO-DISP-INFO: Property [[ReadOnly]] ThisObject -> CommonModule
#endif	// #if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
public:
	// Not Creatable
public:
	// !>NO-DISP-INFO: Процедура ИмяПроцедурыИлиФункции()
public:
	// !>NO-DISP-INFO: Свойство [[ТолькоЧтение]] ЭтотОбъект Как ОбщийМодуль
#endif	// #if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
};


ESB_CLASS_OBJECT(NotifyDescription)
{
	ESB_CLASS_IMPLEMENT_OBJECT(NotifyDescription)
#if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
public:
	static NotifyDescription Create() { ESB_INVOKE_CTOR0(NotifyDescription); }
	static NotifyDescription Create(ConstParam<String> ProcedureName_, ConstParam<CommandInterfaceCommand> Modul_, ConstParam<Arbitrary> Parameters_ = DefUndef<esb::Arbitrary>, ConstParam<String> ErrorHandlerProcedureName_ = DefUndef<esb::String>, ConstParam<CommandInterfaceCommand> ErrorHandlerModule_ = DefUndef<esb::CommandInterfaceCommand>) { ESB_INVOKE_CTOR5(NotifyDescription, ProcedureName_, Modul_, Parameters_, ErrorHandlerProcedureName_, ErrorHandlerModule_); }
	static NotifyDescription Create(ConstParam<String> ProcedureName_, ConstParam<CommandInterfaceCommand> Modul_, ConstParam<Arbitrary> Parameters_ = DefUndef<esb::Arbitrary>, ConstParam<String> ErrorHandlerProcedureName_ = DefUndef<esb::String>, ConstParam<CommonModule> ErrorHandlerModule_ = DefUndef<esb::CommonModule>) { ESB_INVOKE_CTOR5(NotifyDescription, ProcedureName_, Modul_, Parameters_, ErrorHandlerProcedureName_, ErrorHandlerModule_); }
	static NotifyDescription Create(ConstParam<String> ProcedureName_, ConstParam<CommandInterfaceCommand> Modul_, ConstParam<Arbitrary> Parameters_ = DefUndef<esb::Arbitrary>, ConstParam<String> ErrorHandlerProcedureName_ = DefUndef<esb::String>, ConstParam<ManagedForm> ErrorHandlerModule_ = DefUndef<esb::ManagedForm>) { ESB_INVOKE_CTOR5(NotifyDescription, ProcedureName_, Modul_, Parameters_, ErrorHandlerProcedureName_, ErrorHandlerModule_); }
	static NotifyDescription Create(ConstParam<String> ProcedureName_, ConstParam<CommonModule> Modul_, ConstParam<Arbitrary> Parameters_ = DefUndef<esb::Arbitrary>, ConstParam<String> ErrorHandlerProcedureName_ = DefUndef<esb::String>, ConstParam<CommandInterfaceCommand> ErrorHandlerModule_ = DefUndef<esb::CommandInterfaceCommand>) { ESB_INVOKE_CTOR5(NotifyDescription, ProcedureName_, Modul_, Parameters_, ErrorHandlerProcedureName_, ErrorHandlerModule_); }
	static NotifyDescription Create(ConstParam<String> ProcedureName_, ConstParam<CommonModule> Modul_, ConstParam<Arbitrary> Parameters_ = DefUndef<esb::Arbitrary>, ConstParam<String> ErrorHandlerProcedureName_ = DefUndef<esb::String>, ConstParam<CommonModule> ErrorHandlerModule_ = DefUndef<esb::CommonModule>) { ESB_INVOKE_CTOR5(NotifyDescription, ProcedureName_, Modul_, Parameters_, ErrorHandlerProcedureName_, ErrorHandlerModule_); }
	static NotifyDescription Create(ConstParam<String> ProcedureName_, ConstParam<CommonModule> Modul_, ConstParam<Arbitrary> Parameters_ = DefUndef<esb::Arbitrary>, ConstParam<String> ErrorHandlerProcedureName_ = DefUndef<esb::String>, ConstParam<ManagedForm> ErrorHandlerModule_ = DefUndef<esb::ManagedForm>) { ESB_INVOKE_CTOR5(NotifyDescription, ProcedureName_, Modul_, Parameters_, ErrorHandlerProcedureName_, ErrorHandlerModule_); }
	static NotifyDescription Create(ConstParam<String> ProcedureName_, ConstParam<ManagedForm> Modul_, ConstParam<Arbitrary> Parameters_ = DefUndef<esb::Arbitrary>, ConstParam<String> ErrorHandlerProcedureName_ = DefUndef<esb::String>, ConstParam<CommandInterfaceCommand> ErrorHandlerModule_ = DefUndef<esb::CommandInterfaceCommand>) { ESB_INVOKE_CTOR5(NotifyDescription, ProcedureName_, Modul_, Parameters_, ErrorHandlerProcedureName_, ErrorHandlerModule_); }
	static NotifyDescription Create(ConstParam<String> ProcedureName_, ConstParam<ManagedForm> Modul_, ConstParam<Arbitrary> Parameters_ = DefUndef<esb::Arbitrary>, ConstParam<String> ErrorHandlerProcedureName_ = DefUndef<esb::String>, ConstParam<CommonModule> ErrorHandlerModule_ = DefUndef<esb::CommonModule>) { ESB_INVOKE_CTOR5(NotifyDescription, ProcedureName_, Modul_, Parameters_, ErrorHandlerProcedureName_, ErrorHandlerModule_); }
	static NotifyDescription Create(ConstParam<String> ProcedureName_, ConstParam<ManagedForm> Modul_, ConstParam<Arbitrary> Parameters_ = DefUndef<esb::Arbitrary>, ConstParam<String> ErrorHandlerProcedureName_ = DefUndef<esb::String>, ConstParam<ManagedForm> ErrorHandlerModule_ = DefUndef<esb::ManagedForm>) { ESB_INVOKE_CTOR5(NotifyDescription, ProcedureName_, Modul_, Parameters_, ErrorHandlerProcedureName_, ErrorHandlerModule_); }
public:
	// Свойство [[ТолькоЧтение]] ДополнительныеПараметры Как Произвольный
	Arbitrary GetAdditionalParameters() const { ESB_INVOKE_PROPERTY_GET(esb::Arbitrary, PropId::AdditionalParameters); };
	ESB_PROPERTY_FIELD_RO(Arbitrary, AdditionalParameters)
		// Свойство [[ТолькоЧтение]] МодульОбработкиОшибки Как ОдинИз<КомандаКомандногоИнтерфейса, ОбщийМодуль, УправляемаяФорма>
		OneOf<CommandInterfaceCommand, CommonModule, ManagedForm> GetErrorHandlerModule() const { ESB_INVOKE_PROPERTY_GET(ESB_ONEOF(esb::CommandInterfaceCommand, esb::CommonModule, esb::ManagedForm), PropId::ErrorHandlerModule); };
	ESB_PROPERTY_FIELD_RO(ESB_ONEOF(CommandInterfaceCommand, CommonModule, ManagedForm), ErrorHandlerModule)
		// Свойство [[ТолькоЧтение]] ИмяПроцедурыОбработкиОшибки Как Строка
		String GetErrorHandlerProcedureName() const { ESB_INVOKE_PROPERTY_GET(esb::String, PropId::ErrorHandlerProcedureName); };
	ESB_PROPERTY_FIELD_RO(String, ErrorHandlerProcedureName)
		// Свойство [[ТолькоЧтение]] Модуль Как ОдинИз<КомандаКомандногоИнтерфейса, ОбщийМодуль, УправляемаяФорма>
		OneOf<CommandInterfaceCommand, CommonModule, ManagedForm> GetModule() const { ESB_INVOKE_PROPERTY_GET(ESB_ONEOF(esb::CommandInterfaceCommand, esb::CommonModule, esb::ManagedForm), PropId::Module); };
	ESB_PROPERTY_FIELD_RO(ESB_ONEOF(CommandInterfaceCommand, CommonModule, ManagedForm), Module)
		// Свойство [[ТолькоЧтение]] ИмяПроцедуры Как Строка
		String GetProcedureName() const { ESB_INVOKE_PROPERTY_GET(esb::String, PropId::ProcedureName); };
	ESB_PROPERTY_FIELD_RO(String, ProcedureName)
#endif	// #if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
public:
	static ОписаниеОповещения Создать() { ESB_INVOKE_CTOR0(ОписаниеОповещения); }
	static ОписаниеОповещения Создать(КонстПарам<Строка> ИмяПроцедуры_, КонстПарам<КомандаКомандногоИнтерфейса> Модуль_, КонстПарам<Произвольный> ДополнительныеПараметры_ = ПоУмолчаниюНеопределеноИли<esb::Произвольный>, КонстПарам<Строка> ИмяПроцедурыОбработкиОшибки_ = ПоУмолчаниюНеопределеноИли<esb::Строка>, КонстПарам<КомандаКомандногоИнтерфейса> МодульОбработкиОшибки_ = ПоУмолчаниюНеопределеноИли<esb::КомандаКомандногоИнтерфейса>) { ESB_INVOKE_CTOR5(ОписаниеОповещения, ИмяПроцедуры_, Модуль_, ДополнительныеПараметры_, ИмяПроцедурыОбработкиОшибки_, МодульОбработкиОшибки_); }
	static ОписаниеОповещения Создать(КонстПарам<Строка> ИмяПроцедуры_, КонстПарам<КомандаКомандногоИнтерфейса> Модуль_, КонстПарам<Произвольный> ДополнительныеПараметры_ = ПоУмолчаниюНеопределеноИли<esb::Произвольный>, КонстПарам<Строка> ИмяПроцедурыОбработкиОшибки_ = ПоУмолчаниюНеопределеноИли<esb::Строка>, КонстПарам<ОбщийМодуль> МодульОбработкиОшибки_ = ПоУмолчаниюНеопределеноИли<esb::ОбщийМодуль>) { ESB_INVOKE_CTOR5(ОписаниеОповещения, ИмяПроцедуры_, Модуль_, ДополнительныеПараметры_, ИмяПроцедурыОбработкиОшибки_, МодульОбработкиОшибки_); }
	static ОписаниеОповещения Создать(КонстПарам<Строка> ИмяПроцедуры_, КонстПарам<КомандаКомандногоИнтерфейса> Модуль_, КонстПарам<Произвольный> ДополнительныеПараметры_ = ПоУмолчаниюНеопределеноИли<esb::Произвольный>, КонстПарам<Строка> ИмяПроцедурыОбработкиОшибки_ = ПоУмолчаниюНеопределеноИли<esb::Строка>, КонстПарам<УправляемаяФорма> МодульОбработкиОшибки_ = ПоУмолчаниюНеопределеноИли<esb::УправляемаяФорма>) { ESB_INVOKE_CTOR5(ОписаниеОповещения, ИмяПроцедуры_, Модуль_, ДополнительныеПараметры_, ИмяПроцедурыОбработкиОшибки_, МодульОбработкиОшибки_); }
	static ОписаниеОповещения Создать(КонстПарам<Строка> ИмяПроцедуры_, КонстПарам<ОбщийМодуль> Модуль_, КонстПарам<Произвольный> ДополнительныеПараметры_ = ПоУмолчаниюНеопределеноИли<esb::Произвольный>, КонстПарам<Строка> ИмяПроцедурыОбработкиОшибки_ = ПоУмолчаниюНеопределеноИли<esb::Строка>, КонстПарам<КомандаКомандногоИнтерфейса> МодульОбработкиОшибки_ = ПоУмолчаниюНеопределеноИли<esb::КомандаКомандногоИнтерфейса>) { ESB_INVOKE_CTOR5(ОписаниеОповещения, ИмяПроцедуры_, Модуль_, ДополнительныеПараметры_, ИмяПроцедурыОбработкиОшибки_, МодульОбработкиОшибки_); }
	static ОписаниеОповещения Создать(КонстПарам<Строка> ИмяПроцедуры_, КонстПарам<ОбщийМодуль> Модуль_, КонстПарам<Произвольный> ДополнительныеПараметры_ = ПоУмолчаниюНеопределеноИли<esb::Произвольный>, КонстПарам<Строка> ИмяПроцедурыОбработкиОшибки_ = ПоУмолчаниюНеопределеноИли<esb::Строка>, КонстПарам<ОбщийМодуль> МодульОбработкиОшибки_ = ПоУмолчаниюНеопределеноИли<esb::ОбщийМодуль>) { ESB_INVOKE_CTOR5(ОписаниеОповещения, ИмяПроцедуры_, Модуль_, ДополнительныеПараметры_, ИмяПроцедурыОбработкиОшибки_, МодульОбработкиОшибки_); }
	static ОписаниеОповещения Создать(КонстПарам<Строка> ИмяПроцедуры_, КонстПарам<ОбщийМодуль> Модуль_, КонстПарам<Произвольный> ДополнительныеПараметры_ = ПоУмолчаниюНеопределеноИли<esb::Произвольный>, КонстПарам<Строка> ИмяПроцедурыОбработкиОшибки_ = ПоУмолчаниюНеопределеноИли<esb::Строка>, КонстПарам<УправляемаяФорма> МодульОбработкиОшибки_ = ПоУмолчаниюНеопределеноИли<esb::УправляемаяФорма>) { ESB_INVOKE_CTOR5(ОписаниеОповещения, ИмяПроцедуры_, Модуль_, ДополнительныеПараметры_, ИмяПроцедурыОбработкиОшибки_, МодульОбработкиОшибки_); }
	static ОписаниеОповещения Создать(КонстПарам<Строка> ИмяПроцедуры_, КонстПарам<УправляемаяФорма> Модуль_, КонстПарам<Произвольный> ДополнительныеПараметры_ = ПоУмолчаниюНеопределеноИли<esb::Произвольный>, КонстПарам<Строка> ИмяПроцедурыОбработкиОшибки_ = ПоУмолчаниюНеопределеноИли<esb::Строка>, КонстПарам<КомандаКомандногоИнтерфейса> МодульОбработкиОшибки_ = ПоУмолчаниюНеопределеноИли<esb::КомандаКомандногоИнтерфейса>) { ESB_INVOKE_CTOR5(ОписаниеОповещения, ИмяПроцедуры_, Модуль_, ДополнительныеПараметры_, ИмяПроцедурыОбработкиОшибки_, МодульОбработкиОшибки_); }
	static ОписаниеОповещения Создать(КонстПарам<Строка> ИмяПроцедуры_, КонстПарам<УправляемаяФорма> Модуль_, КонстПарам<Произвольный> ДополнительныеПараметры_ = ПоУмолчаниюНеопределеноИли<esb::Произвольный>, КонстПарам<Строка> ИмяПроцедурыОбработкиОшибки_ = ПоУмолчаниюНеопределеноИли<esb::Строка>, КонстПарам<ОбщийМодуль> МодульОбработкиОшибки_ = ПоУмолчаниюНеопределеноИли<esb::ОбщийМодуль>) { ESB_INVOKE_CTOR5(ОписаниеОповещения, ИмяПроцедуры_, Модуль_, ДополнительныеПараметры_, ИмяПроцедурыОбработкиОшибки_, МодульОбработкиОшибки_); }
	static ОписаниеОповещения Создать(КонстПарам<Строка> ИмяПроцедуры_, КонстПарам<УправляемаяФорма> Модуль_, КонстПарам<Произвольный> ДополнительныеПараметры_ = ПоУмолчаниюНеопределеноИли<esb::Произвольный>, КонстПарам<Строка> ИмяПроцедурыОбработкиОшибки_ = ПоУмолчаниюНеопределеноИли<esb::Строка>, КонстПарам<УправляемаяФорма> МодульОбработкиОшибки_ = ПоУмолчаниюНеопределеноИли<esb::УправляемаяФорма>) { ESB_INVOKE_CTOR5(ОписаниеОповещения, ИмяПроцедуры_, Модуль_, ДополнительныеПараметры_, ИмяПроцедурыОбработкиОшибки_, МодульОбработкиОшибки_); }
public:
	// Property [[ReadOnly]] AdditionalParameters -> Arbitrary
	Произвольный ПолучитьДополнительныеПараметры() const { ESB_INVOKE_PROPERTY_GET(esb::Произвольный, PropId::AdditionalParameters); };
	ESB_PROPERTY_FIELD_RO(Произвольный, ДополнительныеПараметры)
		// Property [[ReadOnly]] ErrorHandlerModule -> OneOf<CommandInterfaceCommand, CommonModule, ManagedForm>
		ОдинИз<КомандаКомандногоИнтерфейса, ОбщийМодуль, УправляемаяФорма> ПолучитьМодульОбработкиОшибки() const { ESB_INVOKE_PROPERTY_GET(ESB_ONEOF(esb::КомандаКомандногоИнтерфейса, esb::ОбщийМодуль, esb::УправляемаяФорма), PropId::ErrorHandlerModule); };
	ESB_PROPERTY_FIELD_RO(ESB_ONEOF(КомандаКомандногоИнтерфейса, ОбщийМодуль, УправляемаяФорма), МодульОбработкиОшибки)
		// Property [[ReadOnly]] ErrorHandlerProcedureName -> String
		Строка ПолучитьИмяПроцедурыОбработкиОшибки() const { ESB_INVOKE_PROPERTY_GET(esb::Строка, PropId::ErrorHandlerProcedureName); };
	ESB_PROPERTY_FIELD_RO(Строка, ИмяПроцедурыОбработкиОшибки)
		// Property [[ReadOnly]] Module -> OneOf<CommandInterfaceCommand, CommonModule, ManagedForm>
		ОдинИз<КомандаКомандногоИнтерфейса, ОбщийМодуль, УправляемаяФорма> ПолучитьМодуль() const { ESB_INVOKE_PROPERTY_GET(ESB_ONEOF(esb::КомандаКомандногоИнтерфейса, esb::ОбщийМодуль, esb::УправляемаяФорма), PropId::Module); };
	ESB_PROPERTY_FIELD_RO(ESB_ONEOF(КомандаКомандногоИнтерфейса, ОбщийМодуль, УправляемаяФорма), Модуль)
		// Property [[ReadOnly]] ProcedureName -> String
		Строка ПолучитьИмяПроцедуры() const { ESB_INVOKE_PROPERTY_GET(esb::Строка, PropId::ProcedureName); };
	ESB_PROPERTY_FIELD_RO(Строка, ИмяПроцедуры)
#endif	// #if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
};


ESB_CLASS_OBJECT(Border)
{
	ESB_CLASS_IMPLEMENT_OBJECT(Border)
#if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
public:
	static Border Create(ConstParam<ControlBorderType> BorderType_, ConstParam<Number> Thickness_ = DefUndef<esb::Number>) { ESB_INVOKE_CTOR2(Border, BorderType_, Thickness_); }
public:
	// Свойство [[ТолькоЧтение]] ТипРамки Как ТипРамкиЭлементаУправления
	ControlBorderType GetBorderType() const { ESB_INVOKE_PROPERTY_GET(esb::ControlBorderType, PropId::BorderType); };
	ESB_PROPERTY_FIELD_RO(ControlBorderType, BorderType)
	// Свойство [[ТолькоЧтение]] Вид Как ВидРамки
	esb::BorderType GetType() const { ESB_INVOKE_PROPERTY_GET(esb::BorderType, PropId::Type); };
	ESB_PROPERTY_FIELD_RO(esb::BorderType, Type)
	// Свойство [[ТолькоЧтение]] Толщина Как Число
	Number GetWidth() const { ESB_INVOKE_PROPERTY_GET(esb::Number, PropId::Width); };
	ESB_PROPERTY_FIELD_RO(Number, Width)
#endif	// #if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
public:
	static Рамка Создать(КонстПарам<ТипРамкиЭлементаУправления> ТипРамки_, КонстПарам<Число> Толщина_ = ПоУмолчаниюНеопределеноИли<esb::Число>) { ESB_INVOKE_CTOR2(Рамка, ТипРамки_, Толщина_); }
public:
	// Property [[ReadOnly]] BorderType -> ControlBorderType
	ТипРамкиЭлементаУправления ПолучитьТипРамки() const { ESB_INVOKE_PROPERTY_GET(esb::ТипРамкиЭлементаУправления, PropId::BorderType); };
	ESB_PROPERTY_FIELD_RO(ТипРамкиЭлементаУправления, ТипРамки)
		// Property [[ReadOnly]] Type -> BorderType
		ВидРамки ПолучитьВид() const { ESB_INVOKE_PROPERTY_GET(esb::ВидРамки, PropId::Type); };
	ESB_PROPERTY_FIELD_RO(ВидРамки, Вид)
		// Property [[ReadOnly]] Width -> Number
		Число ПолучитьТолщина() const { ESB_INVOKE_PROPERTY_GET(esb::Число, PropId::Width); };
	ESB_PROPERTY_FIELD_RO(Число, Толщина)
#endif	// #if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
};


ESB_CLASS_OBJECT(Color)
{
	ESB_CLASS_IMPLEMENT_OBJECT(Color)
#if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
public:
	static Color Create() { ESB_INVOKE_CTOR0(Color); }
	static Color Create(ConstParam<Number> Red_, ConstParam<Number> Green_, ConstParam<Number> Blue_) { ESB_INVOKE_CTOR3(Color, Red_, Green_, Blue_); }
public:
	// Свойство [[ТолькоЧтение]] Синий Как Число
	Number GetB() const { ESB_INVOKE_PROPERTY_GET(esb::Number, PropId::B); };
	ESB_PROPERTY_FIELD_RO(Number, B)
		// Свойство [[ТолькоЧтение]] Зеленый Как Число
		Number GetG() const { ESB_INVOKE_PROPERTY_GET(esb::Number, PropId::G); };
	ESB_PROPERTY_FIELD_RO(Number, G)
		// Свойство [[ТолькоЧтение]] Красный Как Число
		Number GetR() const { ESB_INVOKE_PROPERTY_GET(esb::Number, PropId::R); };
	ESB_PROPERTY_FIELD_RO(Number, R)
		// Свойство [[ТолькоЧтение]] Вид Как ВидЦвета
		ColorType GetType() const { ESB_INVOKE_PROPERTY_GET(esb::ColorType, PropId::Type); };
	ESB_PROPERTY_FIELD_RO(ColorType, Type)
#endif	// #if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
public:
	static Цвет Создать() { ESB_INVOKE_CTOR0(Цвет); }
	static Цвет Создать(КонстПарам<Число> Красный_, КонстПарам<Число> Зеленый_, КонстПарам<Число> Синий_) { ESB_INVOKE_CTOR3(Цвет, Красный_, Зеленый_, Синий_); }
public:
	// Property [[ReadOnly]] B -> Number
	Число ПолучитьСиний() const { ESB_INVOKE_PROPERTY_GET(esb::Число, PropId::B); };
	ESB_PROPERTY_FIELD_RO(Число, Синий)
		// Property [[ReadOnly]] G -> Number
		Число ПолучитьЗеленый() const { ESB_INVOKE_PROPERTY_GET(esb::Число, PropId::G); };
	ESB_PROPERTY_FIELD_RO(Число, Зеленый)
		// Property [[ReadOnly]] R -> Number
		Число ПолучитьКрасный() const { ESB_INVOKE_PROPERTY_GET(esb::Число, PropId::R); };
	ESB_PROPERTY_FIELD_RO(Число, Красный)
		// Property [[ReadOnly]] Type -> ColorType
		ВидЦвета ПолучитьВид() const { ESB_INVOKE_PROPERTY_GET(esb::ВидЦвета, PropId::Type); };
	ESB_PROPERTY_FIELD_RO(ВидЦвета, Вид)
#endif	// #if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
};


ESB_CLASS_OBJECT(Picture)
{
	ESB_CLASS_IMPLEMENT_OBJECT(Picture)
#if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
public:
	static Picture Create() { ESB_INVOKE_CTOR0(Picture); }
	static Picture Create(ConstParam<String> FileName_, ConstParam<Boolean> Transparent_ = DefUndef<esb::Boolean>) { ESB_INVOKE_CTOR2(Picture, FileName_, Transparent_); }
	static Picture Create(ConstParam<BinaryData> BinaryData_, ConstParam<Boolean> Transparent_ = DefUndef<esb::Boolean>) { ESB_INVOKE_CTOR2(Picture, BinaryData_, Transparent_); }
public:
	// Функция Преобразовать(ФорматКартинки_ Как ФорматКартинки) Возвращает Картинка
	Picture Convert(ConstParam<PictureFormat> PictureFormat_) { ESB_INVOKE_MEMB_FUNC1(esb::Picture, MethId::Convert, PictureFormat_); };
	// Функция ФильтрИменФайлов() Возвращает Строка
	String FileNameFilter() { ESB_INVOKE_MEMB_FUNC0(esb::String, MethId::FileNameFilter); };
	// Функция Формат() Возвращает ФорматКартинки
	PictureFormat Format() const { ESB_INVOKE_MEMB_FUNC0(esb::PictureFormat, MethId::Format); };
	// Функция ПолучитьДвоичныеДанные() Возвращает ДвоичныеДанные
	BinaryData GetBinaryData();// { ESB_INVOKE_MEMB_FUNC0(esb::BinaryData, MethId::GetBinaryData); };
	// Процедура Записать(ИмяФайла_ Как Строка)
	void Write(ConstParam<String> FileName_) const { ESB_INVOKE_MEMB_PROC1(MethId::Write, FileName_); };
public:
	// Свойство [[ТолькоЧтение]] Вид Как ВидКартинки
	PictureType GetType() const { ESB_INVOKE_PROPERTY_GET(esb::PictureType, PropId::Type); };
	ESB_PROPERTY_FIELD_RO(PictureType, Type)
		// Свойство [[ТолькоЧтение]] НаборВариантов Как Булево
		Boolean GetVariantSet() const { ESB_INVOKE_PROPERTY_GET(esb::Boolean, PropId::VariantSet); };
	ESB_PROPERTY_FIELD_RO(Boolean, VariantSet)
#endif	// #if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
public:
	static Картинка Создать() { ESB_INVOKE_CTOR0(Картинка); }
	static Картинка Создать(КонстПарам<Строка> ИмяФайла_, КонстПарам<Булево> ПрозрачныйФон_ = ПоУмолчаниюНеопределеноИли<esb::Булево>) { ESB_INVOKE_CTOR2(Картинка, ИмяФайла_, ПрозрачныйФон_); }
	static Картинка Создать(КонстПарам<ДвоичныеДанные> ДвоичныеДанные_, КонстПарам<Булево> ПрозрачныйФон_ = ПоУмолчаниюНеопределеноИли<esb::Булево>) { ESB_INVOKE_CTOR2(Картинка, ДвоичныеДанные_, ПрозрачныйФон_); }
public:
	// Function Convert(PictureFormat PictureFormat_) -> Picture
	Картинка Преобразовать(КонстПарам<ФорматКартинки> ФорматКартинки_) { ESB_INVOKE_MEMB_FUNC1(esb::Картинка, MethId::Convert, ФорматКартинки_); };
	// Function FileNameFilter() -> String
	Строка ФильтрИменФайлов() { ESB_INVOKE_MEMB_FUNC0(esb::Строка, MethId::FileNameFilter); };
	// Function Format() -> PictureFormat
	ФорматКартинки Формат() const { ESB_INVOKE_MEMB_FUNC0(esb::ФорматКартинки, MethId::Format); };
	// Function GetBinaryData() -> BinaryData
	ДвоичныеДанные ПолучитьДвоичныеДанные();// { ESB_INVOKE_MEMB_FUNC0(esb::ДвоичныеДанные, MethId::GetBinaryData); };
	// Void Write(String FileName_)
	void Записать(КонстПарам<Строка> ИмяФайла_) const { ESB_INVOKE_MEMB_PROC1(MethId::Write, ИмяФайла_); };
public:
	// Property [[ReadOnly]] Type -> PictureType
	ВидКартинки ПолучитьВид() const { ESB_INVOKE_PROPERTY_GET(esb::ВидКартинки, PropId::Type); };
	ESB_PROPERTY_FIELD_RO(ВидКартинки, Вид)
		// Property [[ReadOnly]] VariantSet -> Boolean
		Булево ПолучитьНаборВариантов() const { ESB_INVOKE_PROPERTY_GET(esb::Булево, PropId::VariantSet); };
	ESB_PROPERTY_FIELD_RO(Булево, НаборВариантов)
#endif	// #if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
};


ESB_CLASS_OBJECT(Font)
{
	ESB_CLASS_IMPLEMENT_OBJECT(Font)
#if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
public:
	static Font Create() { ESB_INVOKE_CTOR0(Font); }
	static Font Create(ConstParam<String> FontName_ = DefUndef<esb::String>, ConstParam<Number> Size_ = DefUndef<esb::Number>, ConstParam<Boolean> Bold_ = DefUndef<esb::Boolean>, ConstParam<Boolean> Italics_ = DefUndef<esb::Boolean>, ConstParam<Boolean> Underscore_ = DefUndef<esb::Boolean>, ConstParam<Boolean> StrikeOut_ = DefUndef<esb::Boolean>, ConstParam<Number> Scale_ = DefUndef<esb::Number>) { ESB_INVOKE_CTOR7(Font, FontName_, Size_, Bold_, Italics_, Underscore_, StrikeOut_, Scale_); }
	static Font Create(ConstParam<Font> Font_, ConstParam<String> FontName_ = DefUndef<esb::String>, ConstParam<Number> Size_ = DefUndef<esb::Number>, ConstParam<Boolean> Bold_ = DefUndef<esb::Boolean>, ConstParam<Boolean> Italics_ = DefUndef<esb::Boolean>, ConstParam<Boolean> Underscore_ = DefUndef<esb::Boolean>, ConstParam<Boolean> StrikeOut_ = DefUndef<esb::Boolean>, ConstParam<Number> Scale_ = DefUndef<esb::Number>) { ESB_INVOKE_CTOR8(Font, Font_, FontName_, Size_, Bold_, Italics_, Underscore_, StrikeOut_, Scale_); }
public:
	// Свойство [[ТолькоЧтение]] Жирный Как ОдинИз<Булево, Неопределено>
	UndefOr<Boolean> GetBold() const { ESB_INVOKE_PROPERTY_GET(UndefOr<esb::Boolean>, PropId::Bold); };
	ESB_PROPERTY_FIELD_RO(UndefOr<Boolean>, Bold)
		// Свойство [[ТолькоЧтение]] Наклонный Как ОдинИз<Булево, Неопределено>
		UndefOr<Boolean> GetItalic() const { ESB_INVOKE_PROPERTY_GET(UndefOr<esb::Boolean>, PropId::Italic); };
	ESB_PROPERTY_FIELD_RO(UndefOr<Boolean>, Italic)
		// Свойство [[ТолькоЧтение]] Имя Как Строка
		String GetName() const { ESB_INVOKE_PROPERTY_GET(esb::String, PropId::Name); };
	ESB_PROPERTY_FIELD_RO(String, Name)
		// Свойство [[ТолькоЧтение]] Масштаб Как Число
		Number GetScale() const { ESB_INVOKE_PROPERTY_GET(esb::Number, PropId::Scale); };
	ESB_PROPERTY_FIELD_RO(Number, Scale)
		// Свойство [[ТолькоЧтение]] Размер Как Число
		Number GetSize() const { ESB_INVOKE_PROPERTY_GET(esb::Number, PropId::Size); };
	ESB_PROPERTY_FIELD_RO(Number, Size)
		// Свойство [[ТолькоЧтение]] Зачеркивание Как ОдинИз<Булево, Неопределено>
		UndefOr<Boolean> GetStrikeout() const { ESB_INVOKE_PROPERTY_GET(UndefOr<esb::Boolean>, PropId::Strikeout); };
	ESB_PROPERTY_FIELD_RO(UndefOr<Boolean>, Strikeout)
		// Свойство [[ТолькоЧтение]] Вид Как ВидШрифта
		FontType GetType() const { ESB_INVOKE_PROPERTY_GET(esb::FontType, PropId::Type); };
	ESB_PROPERTY_FIELD_RO(FontType, Type)
		// Свойство [[ТолькоЧтение]] Подчеркивание Как ОдинИз<Булево, Неопределено>
		UndefOr<Boolean> GetUnderline() const { ESB_INVOKE_PROPERTY_GET(UndefOr<esb::Boolean>, PropId::Underline); };
	ESB_PROPERTY_FIELD_RO(UndefOr<Boolean>, Underline)
#endif	// #if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
public:
	static Шрифт Создать() { ESB_INVOKE_CTOR0(Шрифт); }
	static Шрифт Создать(КонстПарам<Строка> ИмяШрифта_ = ПоУмолчаниюНеопределеноИли<esb::Строка>, КонстПарам<Число> Размер_ = ПоУмолчаниюНеопределеноИли<esb::Число>, КонстПарам<Булево> Жирный_ = ПоУмолчаниюНеопределеноИли<esb::Булево>, КонстПарам<Булево> Наклонный_ = ПоУмолчаниюНеопределеноИли<esb::Булево>, КонстПарам<Булево> Подчеркнутый_ = ПоУмолчаниюНеопределеноИли<esb::Булево>, КонстПарам<Булево> Зачеркнутый_ = ПоУмолчаниюНеопределеноИли<esb::Булево>, КонстПарам<Число> Масштаб_ = ПоУмолчаниюНеопределеноИли<esb::Число>) { ESB_INVOKE_CTOR7(Шрифт, ИмяШрифта_, Размер_, Жирный_, Наклонный_, Подчеркнутый_, Зачеркнутый_, Масштаб_); }
	static Шрифт Создать(КонстПарам<Шрифт> Шрифт_, КонстПарам<Строка> ИмяШрифта_ = ПоУмолчаниюНеопределеноИли<esb::Строка>, КонстПарам<Число> Размер_ = ПоУмолчаниюНеопределеноИли<esb::Число>, КонстПарам<Булево> Жирный_ = ПоУмолчаниюНеопределеноИли<esb::Булево>, КонстПарам<Булево> Наклонный_ = ПоУмолчаниюНеопределеноИли<esb::Булево>, КонстПарам<Булево> Подчеркнутый_ = ПоУмолчаниюНеопределеноИли<esb::Булево>, КонстПарам<Булево> Зачеркнутый_ = ПоУмолчаниюНеопределеноИли<esb::Булево>, КонстПарам<Число> Масштаб_ = ПоУмолчаниюНеопределеноИли<esb::Число>) { ESB_INVOKE_CTOR8(Шрифт, Шрифт_, ИмяШрифта_, Размер_, Жирный_, Наклонный_, Подчеркнутый_, Зачеркнутый_, Масштаб_); }
public:
	// Property [[ReadOnly]] Bold -> OneOf<Boolean, Undefined>
	НеопределеноИли<Булево> ПолучитьЖирный() const { ESB_INVOKE_PROPERTY_GET(НеопределеноИли<esb::Булево>, PropId::Bold); };
	ESB_PROPERTY_FIELD_RO(НеопределеноИли<Булево>, Жирный)
		// Property [[ReadOnly]] Italic -> OneOf<Boolean, Undefined>
		НеопределеноИли<Булево> ПолучитьНаклонный() const { ESB_INVOKE_PROPERTY_GET(НеопределеноИли<esb::Булево>, PropId::Italic); };
	ESB_PROPERTY_FIELD_RO(НеопределеноИли<Булево>, Наклонный)
		// Property [[ReadOnly]] Name -> String
		Строка ПолучитьИмя() const { ESB_INVOKE_PROPERTY_GET(esb::Строка, PropId::Name); };
	ESB_PROPERTY_FIELD_RO(Строка, Имя)
		// Property [[ReadOnly]] Scale -> Number
		Число ПолучитьМасштаб() const { ESB_INVOKE_PROPERTY_GET(esb::Число, PropId::Scale); };
	ESB_PROPERTY_FIELD_RO(Число, Масштаб)
		// Property [[ReadOnly]] Size -> Number
		Число ПолучитьРазмер() const { ESB_INVOKE_PROPERTY_GET(esb::Число, PropId::Size); };
	ESB_PROPERTY_FIELD_RO(Число, Размер)
		// Property [[ReadOnly]] Strikeout -> OneOf<Boolean, Undefined>
		НеопределеноИли<Булево> ПолучитьЗачеркивание() const { ESB_INVOKE_PROPERTY_GET(НеопределеноИли<esb::Булево>, PropId::Strikeout); };
	ESB_PROPERTY_FIELD_RO(НеопределеноИли<Булево>, Зачеркивание)
		// Property [[ReadOnly]] Type -> FontType
		ВидШрифта ПолучитьВид() const { ESB_INVOKE_PROPERTY_GET(esb::ВидШрифта, PropId::Type); };
	ESB_PROPERTY_FIELD_RO(ВидШрифта, Вид)
		// Property [[ReadOnly]] Underline -> OneOf<Boolean, Undefined>
		НеопределеноИли<Булево> ПолучитьПодчеркивание() const { ESB_INVOKE_PROPERTY_GET(НеопределеноИли<esb::Булево>, PropId::Underline); };
	ESB_PROPERTY_FIELD_RO(НеопределеноИли<Булево>, Подчеркивание)
#endif	// #if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
};


ESB_CLASS_OBJECT(FormattedString)
{
	ESB_CLASS_IMPLEMENT_OBJECT(FormattedString)
#if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
public:
	static FormattedString Create() { ESB_INVOKE_CTOR0(FormattedString); }
	static FormattedString Create(ConstParam<String> Content_, ConstParam<Font> Font_ = DefUndef<esb::Font>, ConstParam<Color> TextColor_ = DefUndef<esb::Color>, ConstParam<Color> BackColor_ = DefUndef<esb::Color>, ConstParam<String> Ref_ = DefUndef<esb::String>) { ESB_INVOKE_CTOR5(FormattedString, Content_, Font_, TextColor_, BackColor_, Ref_); }
	static FormattedString Create(ConstParam<Array> Content_, ConstParam<Font> Font_ = DefUndef<esb::Font>, ConstParam<Color> TextColor_ = DefUndef<esb::Color>, ConstParam<Color> BackColor_ = DefUndef<esb::Color>, ConstParam<String> Ref_ = DefUndef<esb::String>) { ESB_INVOKE_CTOR5(FormattedString, Content_, Font_, TextColor_, BackColor_, Ref_); }
	static FormattedString Create(ConstParam<FormattedString> Content_, ConstParam<Font> Font_ = DefUndef<esb::Font>, ConstParam<Color> TextColor_ = DefUndef<esb::Color>, ConstParam<Color> BackColor_ = DefUndef<esb::Color>, ConstParam<String> Ref_ = DefUndef<esb::String>) { ESB_INVOKE_CTOR5(FormattedString, Content_, Font_, TextColor_, BackColor_, Ref_); }
	static FormattedString Create(ConstParam<Picture> Content_, ConstParam<Font> Font_ = DefUndef<esb::Font>, ConstParam<Color> TextColor_ = DefUndef<esb::Color>, ConstParam<Color> BackColor_ = DefUndef<esb::Color>, ConstParam<String> Ref_ = DefUndef<esb::String>) { ESB_INVOKE_CTOR5(FormattedString, Content_, Font_, TextColor_, BackColor_, Ref_); }
	template<std::same_as<String>... StringTs>
	static FormattedString Create(ConstParam<StringTs>... Content1ContentN_) { ESB_INVOKE_CTOR_PACK1(FormattedString, Content1ContentN_ ...); }
	template<std::same_as<Array>... ArrayTs>
	static FormattedString Create(ConstParam<ArrayTs>... Content1ContentN_) { ESB_INVOKE_CTOR_PACK1(FormattedString, Content1ContentN_ ...); }
	template<std::same_as<FormattedString>... FormattedStringTs>
	static FormattedString Create(ConstParam<FormattedStringTs>... Content1ContentN_) { ESB_INVOKE_CTOR_PACK1(FormattedString, Content1ContentN_ ...); }
	template<std::same_as<Picture>... PictureTs>
	static FormattedString Create(ConstParam<PictureTs>... Content1ContentN_) { ESB_INVOKE_CTOR_PACK1(FormattedString, Content1ContentN_ ...); }
#endif	// #if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
public:
	static ФорматированнаяСтрока Создать() { ESB_INVOKE_CTOR0(ФорматированнаяСтрока); }
	static ФорматированнаяСтрока Создать(КонстПарам<Строка> Содержимое_, КонстПарам<Шрифт> Шрифт_ = ПоУмолчаниюНеопределеноИли<esb::Шрифт>, КонстПарам<Цвет> ЦветТекста_ = ПоУмолчаниюНеопределеноИли<esb::Цвет>, КонстПарам<Цвет> ЦветФона_ = ПоУмолчаниюНеопределеноИли<esb::Цвет>, КонстПарам<Строка> Ссылка_ = ПоУмолчаниюНеопределеноИли<esb::Строка>) { ESB_INVOKE_CTOR5(ФорматированнаяСтрока, Содержимое_, Шрифт_, ЦветТекста_, ЦветФона_, Ссылка_); }
	static ФорматированнаяСтрока Создать(КонстПарам<Массив> Содержимое_, КонстПарам<Шрифт> Шрифт_ = ПоУмолчаниюНеопределеноИли<esb::Шрифт>, КонстПарам<Цвет> ЦветТекста_ = ПоУмолчаниюНеопределеноИли<esb::Цвет>, КонстПарам<Цвет> ЦветФона_ = ПоУмолчаниюНеопределеноИли<esb::Цвет>, КонстПарам<Строка> Ссылка_ = ПоУмолчаниюНеопределеноИли<esb::Строка>) { ESB_INVOKE_CTOR5(ФорматированнаяСтрока, Содержимое_, Шрифт_, ЦветТекста_, ЦветФона_, Ссылка_); }
	static ФорматированнаяСтрока Создать(КонстПарам<ФорматированнаяСтрока> Содержимое_, КонстПарам<Шрифт> Шрифт_ = ПоУмолчаниюНеопределеноИли<esb::Шрифт>, КонстПарам<Цвет> ЦветТекста_ = ПоУмолчаниюНеопределеноИли<esb::Цвет>, КонстПарам<Цвет> ЦветФона_ = ПоУмолчаниюНеопределеноИли<esb::Цвет>, КонстПарам<Строка> Ссылка_ = ПоУмолчаниюНеопределеноИли<esb::Строка>) { ESB_INVOKE_CTOR5(ФорматированнаяСтрока, Содержимое_, Шрифт_, ЦветТекста_, ЦветФона_, Ссылка_); }
	static ФорматированнаяСтрока Создать(КонстПарам<Картинка> Содержимое_, КонстПарам<Шрифт> Шрифт_ = ПоУмолчаниюНеопределеноИли<esb::Шрифт>, КонстПарам<Цвет> ЦветТекста_ = ПоУмолчаниюНеопределеноИли<esb::Цвет>, КонстПарам<Цвет> ЦветФона_ = ПоУмолчаниюНеопределеноИли<esb::Цвет>, КонстПарам<Строка> Ссылка_ = ПоУмолчаниюНеопределеноИли<esb::Строка>) { ESB_INVOKE_CTOR5(ФорматированнаяСтрока, Содержимое_, Шрифт_, ЦветТекста_, ЦветФона_, Ссылка_); }
	template<std::same_as<Строка>... СтрокаTs>
	static ФорматированнаяСтрока Создать(КонстПарам<СтрокаTs>... Содержимое1СодержимоеN_) { ESB_INVOKE_CTOR_PACK1(ФорматированнаяСтрока, Содержимое1СодержимоеN_ ...); }
	template<std::same_as<Массив>... МассивTs>
	static ФорматированнаяСтрока Создать(КонстПарам<МассивTs>... Содержимое1СодержимоеN_) { ESB_INVOKE_CTOR_PACK1(ФорматированнаяСтрока, Содержимое1СодержимоеN_ ...); }
	template<std::same_as<ФорматированнаяСтрока>... ФорматированнаяСтрокаTs>
	static ФорматированнаяСтрока Создать(КонстПарам<ФорматированнаяСтрокаTs>... Содержимое1СодержимоеN_) { ESB_INVOKE_CTOR_PACK1(ФорматированнаяСтрока, Содержимое1СодержимоеN_ ...); }
	template<std::same_as<Картинка>... КартинкаTs>
	static ФорматированнаяСтрока Создать(КонстПарам<КартинкаTs>... Содержимое1СодержимоеN_) { ESB_INVOKE_CTOR_PACK1(ФорматированнаяСтрока, Содержимое1СодержимоеN_ ...); }
#endif	// #if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
};


}	//namespace esb 

#endif //ESB_ES1_MAIN_H_
