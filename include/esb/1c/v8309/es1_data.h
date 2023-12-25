/*
	"ESB" is designed to create external native components for the 1C(tm) program.
	Copyright © 2023 IntUnsigned	(v8classes@gmail.com)
	"ESB" is free software under LGPLv2.1 license with essential restriction about binary part (view esb-license.txt)
	"ESB" IS DISTRIBUTED "AS-IS" WITHOUT ANY, EVEN THE IMPLIED, WARRANTY. IN NO EVENT SHALL THE AUTHORS OR THE COPYRIGHT OWNER BE LIABLE FOR ANY CLAIM, 
	DAMAGES OR OTHER LIABILITY ARISING OUT OF THE USE OF THIS SOFTWARE.
	THE AUTHOR DOES NOT GIVE ANY GUARANTEES AND DOES NOT BEAR ANY RESPONSIBILITY REGARDING THE USE OF 1C(tm) COMPONENTS PRODUCED USING THE ESB LIBRARY IN THE 1C(tm) ENVIRONMENT.
*/


#pragma once
#ifndef ESB_ES1_DATA_H_
#define ESB_ES1_DATA_H_

#include "es1_main.h"
#include "esdisp_data.h"


namespace esb 
{
class QueryRecordType;
class QueryResultIteration;
class ValueTableColumn;
class ValueTableColumnCollection;
class ValueTableRow;
class CollectionIndex;
class CollectionIndexes;
class ValueTable;
class ValueTreeColumn;
class ValueTreeColumnCollection;
class ValueTreeRow;
class ValueTreeRowCollection;
class ValueTree;
class QueryParameterDescription;
class QueryParametersDescription;
class QueryResultColumn;
class QueryResultColumnsCollection;
class QueryResultSelection;
class QueryResult;
class QueryTempTableColumn;
class QueryTempTableColumns;
class QueryTempTable;
class QueryTempTables;
class TempTablesManager;
class Query;
class ValueListItem;
class ValueList;


#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
using ТипЗаписиЗапроса = QueryRecordType;
using ОбходРезультатаЗапроса = QueryResultIteration;
using КолонкаТаблицыЗначений = ValueTableColumn;
using КоллекцияКолонокТаблицыЗначений = ValueTableColumnCollection;
using СтрокаТаблицыЗначений = ValueTableRow;
using ИндексКоллекции = CollectionIndex;
using ИндексыКоллекции = CollectionIndexes;
using ТаблицаЗначений = ValueTable;
using КолонкаДереваЗначений = ValueTreeColumn;
using КоллекцияКолонокДереваЗначений = ValueTreeColumnCollection;
using СтрокаДереваЗначений = ValueTreeRow;
using КоллекцияСтрокДереваЗначений = ValueTreeRowCollection;
using ДеревоЗначений = ValueTree;
using ОписаниеПараметраЗапроса = QueryParameterDescription;
using ОписаниеПараметровЗапроса = QueryParametersDescription;
using КолонкаРезультатаЗапроса = QueryResultColumn;
using КоллекцияКолонокРезультатаЗапроса = QueryResultColumnsCollection;
using ВыборкаИзРезультатаЗапроса = QueryResultSelection;
using РезультатЗапроса = QueryResult;
using КолонкаВременнойТаблицыЗапроса = QueryTempTableColumn;
using КолонкиВременнойТаблицыЗапроса = QueryTempTableColumns;
using ВременнаяТаблицаЗапроса = QueryTempTable;
using ВременныеТаблицыЗапроса = QueryTempTables;
using МенеджерВременныхТаблиц = TempTablesManager;
using Запрос = Query;
using ЭлементСпискаЗначений = ValueListItem;
using СписокЗначений = ValueList;
#endif	// #if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN


ESB_CLASS_ENUM(QueryRecordType)
{ ESB_CLASS_IMPLEMENT_ENUM(QueryRecordType)
public:
ESB_CLASS_ENUM_ITEM(DetailRecord, ДетальнаяЗапись)
ESB_CLASS_ENUM_ITEM(GroupTotal, ИтогПоГруппировке)
ESB_CLASS_ENUM_ITEM(TotalByHierarchy, ИтогПоИерархии)
ESB_CLASS_ENUM_ITEM(Overall, ОбщийИтог)
};


ESB_CLASS_ENUM(QueryResultIteration)
{ ESB_CLASS_IMPLEMENT_ENUM(QueryResultIteration)
public:
ESB_CLASS_ENUM_ITEM(Linear, Прямой)
ESB_CLASS_ENUM_ITEM(ByGroups, ПоГруппировкам)
ESB_CLASS_ENUM_ITEM(ByGroupsWithHierarchy, ПоГруппировкамСИерархией)
};




ESB_CLASS_OBJECT(ValueTableColumn)
{
	ESB_CLASS_IMPLEMENT_OBJECT(ValueTableColumn)
#if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
public:
	// Not Creatable
public:
	// Свойство [[Чтение,Запись]] Имя Как Строка
	String GetName() const { ESB_INVOKE_PROPERTY_GET(esb::String, PropId::Name); };
	void SetName(ConstParam<String> Value_) { ESB_INVOKE_PROPERTY_SET(PropId::Name, Value_); };
	ESB_PROPERTY_FIELD_RW(String, Name)
		// Свойство [[Чтение,Запись]] Заголовок Как Строка
		String GetTitle() const { ESB_INVOKE_PROPERTY_GET(esb::String, PropId::Title); };
	void SetTitle(ConstParam<String> Value_) { ESB_INVOKE_PROPERTY_SET(PropId::Title, Value_); };
	ESB_PROPERTY_FIELD_RW(String, Title)
		// Свойство [[ТолькоЧтение]] ТипЗначения Как ОписаниеТипов
		TypeDescription GetValueType() const { ESB_INVOKE_PROPERTY_GET(esb::TypeDescription, PropId::ValueType); };
	ESB_PROPERTY_FIELD_RO(TypeDescription, ValueType)
		// Свойство [[Чтение,Запись]] Ширина Как Число
		Number GetWidth() const { ESB_INVOKE_PROPERTY_GET(esb::Number, PropId::Width); };
	void SetWidth(ConstParam<Number> Value_) { ESB_INVOKE_PROPERTY_SET(PropId::Width, Value_); };
	ESB_PROPERTY_FIELD_RW(Number, Width)
#endif	// #if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
public:
	// Not Creatable
public:
	// Property [[Read,Write]] Name -> String
	Строка ПолучитьИмя() const { ESB_INVOKE_PROPERTY_GET(esb::Строка, PropId::Name); };
	void УстановитьИмя(КонстПарам<Строка> Значение_) { ESB_INVOKE_PROPERTY_SET(PropId::Name, Значение_); };
	ESB_PROPERTY_FIELD_RW(Строка, Имя)
		// Property [[Read,Write]] Title -> String
		Строка ПолучитьЗаголовок() const { ESB_INVOKE_PROPERTY_GET(esb::Строка, PropId::Title); };
	void УстановитьЗаголовок(КонстПарам<Строка> Значение_) { ESB_INVOKE_PROPERTY_SET(PropId::Title, Значение_); };
	ESB_PROPERTY_FIELD_RW(Строка, Заголовок)
		// Property [[ReadOnly]] ValueType -> TypeDescription
		ОписаниеТипов ПолучитьТипЗначения() const { ESB_INVOKE_PROPERTY_GET(esb::ОписаниеТипов, PropId::ValueType); };
	ESB_PROPERTY_FIELD_RO(ОписаниеТипов, ТипЗначения)
		// Property [[Read,Write]] Width -> Number
		Число ПолучитьШирина() const { ESB_INVOKE_PROPERTY_GET(esb::Число, PropId::Width); };
	void УстановитьШирина(КонстПарам<Число> Значение_) { ESB_INVOKE_PROPERTY_SET(PropId::Width, Значение_); };
	ESB_PROPERTY_FIELD_RW(Число, Ширина)
#endif	// #if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
};


ESB_CLASS_OBJECT_DUAL_WITH_DYNAMIC_PROPERTIES_RO(ValueTableColumnCollection, ValueTableColumn, IxCollectionRO<ValueTableColumn>)
{
	ESB_CLASS_IMPLEMENT_OBJECT_DUAL_WITH_DYNAMIC_PROPERTIES(ValueTableColumnCollection)
#if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
public:
	// Not Creatable
public:
	// Функция Добавить([Имя_ Как Строка], [Тип_ Как ОписаниеТипов], [Заголовок_ Как Строка], [Ширина_ Как Число]) Возвращает КолонкаТаблицыЗначений
	ValueTableColumn Add(ConstParam<String> Name_ = DefUndef<esb::String>, ConstParam<TypeDescription> type_ = DefUndef<esb::TypeDescription>, ConstParam<String> Title_ = DefUndef<esb::String>, ConstParam<Number> Width_ = DefUndef<esb::Number>) { ESB_INVOKE_MEMB_FUNC4(esb::ValueTableColumn, MethId::Add, Name_, type_, Title_, Width_); };
	// Процедура Очистить()
	void Clear() { ESB_INVOKE_MEMB_PROC0(MethId::Clear); };
	// Функция Количество() Возвращает Число
	Number Count() const { ESB_INVOKE_MEMB_FUNC0(esb::Number, MethId::Count); };
	// Процедура Удалить(Индекс_ Как ОдинИз<Число, Строка, КолонкаТаблицыЗначений>)
	void Delete(ConstParam<Number> Index_) { ESB_INVOKE_MEMB_PROC1(MethId::Delete, Index_); };
	void Delete(ConstParam<String> Index_) { ESB_INVOKE_MEMB_PROC1(MethId::Delete, Index_); };
	void Delete(ConstParam<ValueTableColumn> Index_) { ESB_INVOKE_MEMB_PROC1(MethId::Delete, Index_); };
	// Функция Найти(НаименованиеКолонки_ Как Строка) Возвращает ОдинИз<Неопределено, КолонкаТаблицыЗначений>
	UndefOr<ValueTableColumn> Find(ConstParam<String> ColumnDescription_) { ESB_INVOKE_MEMB_FUNC1(UndefOr<esb::ValueTableColumn>, MethId::Find, ColumnDescription_); };
	// Функция Получить(Индекс_ Как Число) Возвращает КолонкаТаблицыЗначений
	ValueTableColumn Get(ConstParam<Number> Index_) { ESB_INVOKE_MEMB_FUNC1(esb::ValueTableColumn, MethId::Get, Index_); };
	// Функция Индекс(Колонка_ Как КолонкаТаблицыЗначений) Возвращает Число
	Number IndexOf(ConstParam<ValueTableColumn> Column_) const { ESB_INVOKE_MEMB_FUNC1(esb::Number, MethId::IndexOf, Column_); };
	// Функция Вставить(Индекс_ Как Число, [Имя_ Как Строка], [Тип_ Как ОписаниеТипов], [Заголовок_ Как Строка], [Ширина_ Как Число]) Возвращает КолонкаТаблицыЗначений
	ValueTableColumn Insert(ConstParam<Number> Index_, ConstParam<String> Name_ = DefUndef<esb::String>, ConstParam<TypeDescription> type_ = DefUndef<esb::TypeDescription>, ConstParam<String> Title_ = DefUndef<esb::String>, ConstParam<Number> Width_ = DefUndef<esb::Number>) { ESB_INVOKE_MEMB_FUNC5(esb::ValueTableColumn, MethId::Insert, Index_, Name_, type_, Title_, Width_); };
	// Процедура Сдвинуть(Колонка_ Как ОдинИз<Число, КолонкаТаблицыЗначений>, Смещение_ Как Число)
	void Move(ConstParam<Number> Column_, ConstParam<Number> Offset_) { ESB_INVOKE_MEMB_PROC2(MethId::Move, Column_, Offset_); };
	void Move(ConstParam<ValueTableColumn> Column_, ConstParam<Number> Offset_) { ESB_INVOKE_MEMB_PROC2(MethId::Move, Column_, Offset_); };
public:
	ESB_IMPLEMENT_DYNAMIC_COLLECTION_RO_NAME_EN(ColumnName)
#endif	// #if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
public:
	// Not Creatable
public:
	// Function Add(String Name_ = <default>, TypeDescription Type_ = <default>, String Title_ = <default>, Number Width_ = <default>) -> ValueTableColumn
	КолонкаТаблицыЗначений Добавить(КонстПарам<Строка> Имя_ = ПоУмолчаниюНеопределеноИли<esb::Строка>, КонстПарам<ОписаниеТипов> Тип_ = ПоУмолчаниюНеопределеноИли<esb::ОписаниеТипов>, КонстПарам<Строка> Заголовок_ = ПоУмолчаниюНеопределеноИли<esb::Строка>, КонстПарам<Число> Ширина_ = ПоУмолчаниюНеопределеноИли<esb::Число>) { ESB_INVOKE_MEMB_FUNC4(esb::КолонкаТаблицыЗначений, MethId::Add, Имя_, Тип_, Заголовок_, Ширина_); };
	// Void Clear()
	void Очистить() { ESB_INVOKE_MEMB_PROC0(MethId::Clear); };
	// Function Count() -> Number
	Число Количество() const { ESB_INVOKE_MEMB_FUNC0(esb::Число, MethId::Count); };
	// Void Delete(OneOf<Number, String, ValueTableColumn> Index_)
	void Удалить(КонстПарам<Число> Индекс_) { ESB_INVOKE_MEMB_PROC1(MethId::Delete, Индекс_); };
	void Удалить(КонстПарам<Строка> Индекс_) { ESB_INVOKE_MEMB_PROC1(MethId::Delete, Индекс_); };
	void Удалить(КонстПарам<КолонкаТаблицыЗначений> Индекс_) { ESB_INVOKE_MEMB_PROC1(MethId::Delete, Индекс_); };
	// Function Find(String ColumnDescription_) -> OneOf<Undefined, ValueTableColumn>
	НеопределеноИли<КолонкаТаблицыЗначений> Найти(КонстПарам<Строка> НаименованиеКолонки_) { ESB_INVOKE_MEMB_FUNC1(НеопределеноИли<esb::КолонкаТаблицыЗначений>, MethId::Find, НаименованиеКолонки_); };
	// Function Get(Number Index_) -> ValueTableColumn
	КолонкаТаблицыЗначений Получить(КонстПарам<Число> Индекс_) { ESB_INVOKE_MEMB_FUNC1(esb::КолонкаТаблицыЗначений, MethId::Get, Индекс_); };
	// Function IndexOf(ValueTableColumn Column_) -> Number
	Число Индекс(КонстПарам<КолонкаТаблицыЗначений> Колонка_) const { ESB_INVOKE_MEMB_FUNC1(esb::Число, MethId::IndexOf, Колонка_); };
	// Function Insert(Number Index_, String Name_ = <default>, TypeDescription Type_ = <default>, String Title_ = <default>, Number Width_ = <default>) -> ValueTableColumn
	КолонкаТаблицыЗначений Вставить(КонстПарам<Число> Индекс_, КонстПарам<Строка> Имя_ = ПоУмолчаниюНеопределеноИли<esb::Строка>, КонстПарам<ОписаниеТипов> Тип_ = ПоУмолчаниюНеопределеноИли<esb::ОписаниеТипов>, КонстПарам<Строка> Заголовок_ = ПоУмолчаниюНеопределеноИли<esb::Строка>, КонстПарам<Число> Ширина_ = ПоУмолчаниюНеопределеноИли<esb::Число>) { ESB_INVOKE_MEMB_FUNC5(esb::КолонкаТаблицыЗначений, MethId::Insert, Индекс_, Имя_, Тип_, Заголовок_, Ширина_); };
	// Void Move(OneOf<Number, ValueTableColumn> Column_, Number Offset_)
	void Сдвинуть(КонстПарам<Число> Колонка_, КонстПарам<Число> Смещение_) { ESB_INVOKE_MEMB_PROC2(MethId::Move, Колонка_, Смещение_); };
	void Сдвинуть(КонстПарам<КолонкаТаблицыЗначений> Колонка_, КонстПарам<Число> Смещение_) { ESB_INVOKE_MEMB_PROC2(MethId::Move, Колонка_, Смещение_); };
public:
	ESB_IMPLEMENT_DYNAMIC_COLLECTION_RO_NAME_RU(ИмяКолонки)
#endif	// #if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
public:
	ESB_IMPLEMENT_ENUM_ITEMS(ValueTableColumnCollection, ValueTableColumn)
public:
	ESB_IMPLEMENT_INDEXED_COLLECTION_RO()
};


ESB_CLASS_OBJECT_DUAL_WITH_DYNAMIC_PROPERTIES_RW(ValueTableRow, Arbitrary, IxCollectionRW<Arbitrary>)
{
	ESB_CLASS_IMPLEMENT_OBJECT_DUAL_WITH_DYNAMIC_PROPERTIES(ValueTableRow)
#if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
public:
	// Not Creatable
public:
	// Функция Получить(Индекс_ Как Число) Возвращает Произвольный
	Arbitrary Get(ConstParam<Number> Index_) const { ESB_INVOKE_MEMB_FUNC1(esb::Arbitrary, MethId::Get, Index_); };
	// Функция Владелец() Возвращает ТаблицаЗначений
	ValueTable Owner() const; // defer implementation { ESB_INVOKE_MEMB_FUNC0(esb::ValueTable, MethId::Owner); };
	// Процедура Установить(Индекс_ Как Число, Значение_ Как Произвольный)
	void Set(ConstParam<Number> Index_, ConstParam<Arbitrary> Value_) { ESB_INVOKE_MEMB_PROC2(MethId::Set, Index_, Value_); };
public:
	ESB_IMPLEMENT_DYNAMIC_COLLECTION_RW_NAME_EN(ColumnName)
#endif	// #if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
public:
	// Not Creatable
public:
	// Function Get(Number Index_) -> Arbitrary
	Произвольный Получить(КонстПарам<Число> Индекс_) const { ESB_INVOKE_MEMB_FUNC1(esb::Произвольный, MethId::Get, Индекс_); };
	// Function Owner() -> ValueTable
	ТаблицаЗначений Владелец() const; // defer implementation  { ESB_INVOKE_MEMB_FUNC0(esb::ТаблицаЗначений, MethId::Owner); };
	// Void Set(Number Index_, Arbitrary Value_)
	void Установить(КонстПарам<Число> Индекс_, КонстПарам<Произвольный> Значение_) { ESB_INVOKE_MEMB_PROC2(MethId::Set, Индекс_, Значение_); };
public:
	ESB_IMPLEMENT_DYNAMIC_COLLECTION_RW_NAME_RU(ИмяКолонки)
#endif	// #if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
public:
	ESB_IMPLEMENT_ENUM_ITEMS(ValueTableRow, Arbitrary)
public:
	ESB_IMPLEMENT_INDEXED_COLLECTION_RW()
};


ESB_CLASS_VALUE_DUAL(CollectionIndex, IxCollectionRO<String>)
{
	ESB_CLASS_IMPLEMENT_VALUE_DUAL(CollectionIndex)
#if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
public:
	// Not Creatable
#endif	// #if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
public:
	// Not Creatable
#endif	// #if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
public:
	ESB_IMPLEMENT_ENUM_ITEMS(CollectionIndex, String)
public:
	ESB_IMPLEMENT_INDEXED_COLLECTION_RO()
};


ESB_CLASS_OBJECT_DUAL(CollectionIndexes, IxCollectionRO<CollectionIndex>)
{
	ESB_CLASS_IMPLEMENT_OBJECT_DUAL(CollectionIndexes)
#if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
public:
	// Not Creatable
public:
	// Функция Добавить(Колонки_ Как Строка) Возвращает ИндексКоллекции
	CollectionIndex Add(ConstParam<String> Columns_) { ESB_INVOKE_MEMB_FUNC1(esb::CollectionIndex, MethId::Add, Columns_); };
	// Процедура Очистить()
	void Clear() { ESB_INVOKE_MEMB_PROC0(MethId::Clear); };
	// Функция Количество() Возвращает Число
	Number Count() const { ESB_INVOKE_MEMB_FUNC0(esb::Number, MethId::Count); };
	// Процедура Удалить(Индекс_ Как ИндексКоллекции)
	void Delete(ConstParam<CollectionIndex> Index_) { ESB_INVOKE_MEMB_PROC1(MethId::Delete, Index_); };
	// Процедура Удалить(Индекс_ Как ИндексКоллекции)
	void Delete(ConstParam<Number> Index_) { ESB_INVOKE_MEMB_PROC1(MethId::Delete, Index_); };
#endif	// #if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
public:
	// Not Creatable
public:
	// Function Add(String Columns_) -> CollectionIndex
	ИндексКоллекции Добавить(КонстПарам<Строка> Колонки_) { ESB_INVOKE_MEMB_FUNC1(esb::ИндексКоллекции, MethId::Add, Колонки_); };
	// Void Clear()
	void Очистить() { ESB_INVOKE_MEMB_PROC0(MethId::Clear); };
	// Function Count() -> Number
	Число Количество() const { ESB_INVOKE_MEMB_FUNC0(esb::Число, MethId::Count); };
	// Void Delete(CollectionIndex Index_)
	void Удалить(КонстПарам<ИндексКоллекции> Индекс_) { ESB_INVOKE_MEMB_PROC1(MethId::Delete, Индекс_); };
	// Void Delete(CollectionIndex Index_)
	void Удалить(КонстПарам<Число> Индекс_) { ESB_INVOKE_MEMB_PROC1(MethId::Delete, Индекс_); };
#endif	// #if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
public:
	ESB_IMPLEMENT_ENUM_ITEMS(CollectionIndexes, CollectionIndex)
public:
	ESB_IMPLEMENT_INDEXED_COLLECTION_RO()
};


ESB_CLASS_OBJECT_DUAL(ValueTable, IxCollectionRO<ValueTableRow>)
{
	ESB_CLASS_IMPLEMENT_OBJECT_DUAL(ValueTable)
#if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
public:
	static ValueTable Create() { ESB_INVOKE_CTOR0(ValueTable); }
public:
	// Функция Добавить() Возвращает СтрокаТаблицыЗначений
	ValueTableRow Add() { ESB_INVOKE_MEMB_FUNC0(esb::ValueTableRow, MethId::Add); };
	// Функция ВыбратьСтроку([Заголовок_ Как Строка], [НачальнаяСтрока_ Как СтрокаТаблицыЗначений]) Возвращает ОдинИз<Неопределено, СтрокаТаблицыЗначений>
	UndefOr<ValueTableRow> ChooseRow(ConstParam<String> Title_ = DefUndef<esb::String>, ConstParam<ValueTableRow> InitialRow_ = DefUndef<esb::ValueTableRow>) const { ESB_INVOKE_MEMB_FUNC2(UndefOr<esb::ValueTableRow>, MethId::ChooseRow, Title_, InitialRow_); };
	// Процедура Очистить()
	void Clear() { ESB_INVOKE_MEMB_PROC0(MethId::Clear); };
	// Функция Скопировать([Строки_ Как Массив], [Колонки_ Как Строка]) Возвращает ТаблицаЗначений
	ValueTable Copy(ConstParam<Array> Rows_ = DefUndef<esb::Array>, ConstParam<String> Columns_ = DefUndef<esb::String>) const { ESB_INVOKE_MEMB_FUNC2(esb::ValueTable, MethId::Copy, Rows_, Columns_); };
	// Функция Скопировать([Строки_ Как Массив], [Колонки_ Как Строка]) Возвращает ТаблицаЗначений
	ValueTable Copy(ConstParam<Structure> ParametersOfFilter_ = DefUndef<esb::Structure>, ConstParam<String> Columns_ = DefUndef<esb::String>) const { ESB_INVOKE_MEMB_FUNC2(esb::ValueTable, MethId::Copy, ParametersOfFilter_, Columns_); };
	// Функция СкопироватьКолонки([Колонки_ Как Строка]) Возвращает ТаблицаЗначений
	ValueTable CopyColumns(ConstParam<String> Columns_ = DefUndef<esb::String>) const { ESB_INVOKE_MEMB_FUNC1(esb::ValueTable, MethId::CopyColumns, Columns_); };
	// Функция Количество() Возвращает Число
	Number Count() const { ESB_INVOKE_MEMB_FUNC0(esb::Number, MethId::Count); };
	// Процедура Удалить(Строка_ Как СтрокаТаблицыЗначений)
	void Delete(ConstParam<ValueTableRow> Row_) { ESB_INVOKE_MEMB_PROC1(MethId::Delete, Row_); };
	// Процедура Удалить(Строка_ Как СтрокаТаблицыЗначений)
	void Delete(ConstParam<Number> Index_) { ESB_INVOKE_MEMB_PROC1(MethId::Delete, Index_); };
	// Процедура ЗаполнитьЗначения(Значение_ Как Произвольный, [Колонки_ Как Строка])
	void FillValues(ConstParam<Arbitrary> Value_, ConstParam<String> Columns_ = DefUndef<esb::String>) { ESB_INVOKE_MEMB_PROC2(MethId::FillValues, Value_, Columns_); };
	// Функция Найти(Значение_ Как Произвольный, [Колонки_ Как Строка]) Возвращает ОдинИз<Неопределено, СтрокаТаблицыЗначений>
	UndefOr<ValueTableRow> Find(ConstParam<Arbitrary> Value_, ConstParam<String> Columns_ = DefUndef<esb::String>) const { ESB_INVOKE_MEMB_FUNC2(UndefOr<esb::ValueTableRow>, MethId::Find, Value_, Columns_); };
	// Функция НайтиСтроки(ПараметрыОтбора_ Как Структура) Возвращает МассивИз<СтрокаТаблицыЗначений>
	ArrayOf<ValueTableRow> FindRows(ConstParam<Structure> FilterParameters_) const { ESB_INVOKE_MEMB_FUNC1(esb::ArrayOf<ValueTableRow>, MethId::FindRows, FilterParameters_); };
	// Функция Получить(Индекс_ Как Число) Возвращает СтрокаТаблицыЗначений
	ValueTableRow Get(ConstParam<Number> Index_) const { ESB_INVOKE_MEMB_FUNC1(esb::ValueTableRow, MethId::Get, Index_); };
	// Процедура Свернуть(КолонкиГруппировок_ Как Строка, [КолонкиСуммирования_ Как Строка])
	void GroupBy(ConstParam<String> GroupingColumns_, ConstParam<String> TotalingColumns_ = DefUndef<esb::String>) { ESB_INVOKE_MEMB_PROC2(MethId::GroupBy, GroupingColumns_, TotalingColumns_); };
	// Функция Индекс(Строка_ Как СтрокаТаблицыЗначений) Возвращает Число
	Number IndexOf(ConstParam<ValueTableRow> Row_) const { ESB_INVOKE_MEMB_FUNC1(esb::Number, MethId::IndexOf, Row_); };
	// Функция Вставить(Индекс_ Как Число) Возвращает СтрокаТаблицыЗначений
	ValueTableRow Insert(ConstParam<Number> Index_) { ESB_INVOKE_MEMB_FUNC1(esb::ValueTableRow, MethId::Insert, Index_); };
	// Процедура ЗагрузитьКолонку(Массив_ Как Массив, Колонка_ Как ОдинИз<Число, Строка, КолонкаТаблицыЗначений>)
	void LoadColumn(ConstParam<Array> Array_, ConstParam<Number> Column_) { ESB_INVOKE_MEMB_PROC2(MethId::LoadColumn, Array_, Column_); };
	void LoadColumn(ConstParam<Array> Array_, ConstParam<String> Column_) { ESB_INVOKE_MEMB_PROC2(MethId::LoadColumn, Array_, Column_); };
	void LoadColumn(ConstParam<Array> Array_, ConstParam<ValueTableColumn> Column_) { ESB_INVOKE_MEMB_PROC2(MethId::LoadColumn, Array_, Column_); };
	// Процедура Сдвинуть(Строка_ Как ОдинИз<Число, СтрокаТаблицыЗначений>, Смещение_ Как Число)
	void Move(ConstParam<Number> Row_, ConstParam<Number> Offset_) { ESB_INVOKE_MEMB_PROC2(MethId::Move, Row_, Offset_); };
	void Move(ConstParam<ValueTableRow> Row_, ConstParam<Number> Offset_) { ESB_INVOKE_MEMB_PROC2(MethId::Move, Row_, Offset_); };
	// Процедура Сортировать(Колонки_ Как Строка, [ОбъектСравнения_ Как СравнениеЗначений])
	void Sort(ConstParam<String> Columns_, ConstParam<CompareValues> ObjectOfComparison_ = DefUndef<esb::CompareValues>) { ESB_INVOKE_MEMB_PROC2(MethId::Sort, Columns_, ObjectOfComparison_); };
	// Функция Итог(Колонка_ Как Строка) Возвращает ОдинИз<Число, Неопределено>
	UndefOr<Number> Total(ConstParam<String> Column_) const { ESB_INVOKE_MEMB_FUNC1(UndefOr<esb::Number>, MethId::Total, Column_); };
	// Функция ВыгрузитьКолонку(Колонка_ Как ОдинИз<Число, Строка, КолонкаТаблицыЗначений>) Возвращает Массив
	Array UnloadColumn(ConstParam<Number> Column_) const { ESB_INVOKE_MEMB_FUNC1(esb::Array, MethId::UnloadColumn, Column_); };
	Array UnloadColumn(ConstParam<String> Column_) const { ESB_INVOKE_MEMB_FUNC1(esb::Array, MethId::UnloadColumn, Column_); };
	Array UnloadColumn(ConstParam<ValueTableColumn> Column_) const { ESB_INVOKE_MEMB_FUNC1(esb::Array, MethId::UnloadColumn, Column_); };
public:
	// Свойство [[ТолькоЧтение]] Колонки Как КоллекцияКолонокТаблицыЗначений
	ValueTableColumnCollection GetColumns() const { ESB_INVOKE_PROPERTY_GET(esb::ValueTableColumnCollection, PropId::Columns); };
	ESB_PROPERTY_FIELD_RO(ValueTableColumnCollection, Columns)
		// Свойство [[ТолькоЧтение]] Индексы Как ИндексыКоллекции
		CollectionIndexes GetIndexes() const { ESB_INVOKE_PROPERTY_GET(esb::CollectionIndexes, PropId::Indexes); };
	ESB_PROPERTY_FIELD_RO(CollectionIndexes, Indexes)
#endif	// #if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
public:
	static ТаблицаЗначений Создать() { ESB_INVOKE_CTOR0(ТаблицаЗначений); }
public:
	// Function Add() -> ValueTableRow
	СтрокаТаблицыЗначений Добавить() { ESB_INVOKE_MEMB_FUNC0(esb::СтрокаТаблицыЗначений, MethId::Add); };
	// Function ChooseRow(String Title_ = <default>, ValueTableRow InitialRow_ = <default>) -> OneOf<Undefined, ValueTableRow>
	НеопределеноИли<СтрокаТаблицыЗначений> ВыбратьСтроку(КонстПарам<Строка> Заголовок_ = ПоУмолчаниюНеопределеноИли<esb::Строка>, КонстПарам<СтрокаТаблицыЗначений> НачальнаяСтрока_ = ПоУмолчаниюНеопределеноИли<esb::СтрокаТаблицыЗначений>) const { ESB_INVOKE_MEMB_FUNC2(НеопределеноИли<esb::СтрокаТаблицыЗначений>, MethId::ChooseRow, Заголовок_, НачальнаяСтрока_); };
	// Void Clear()
	void Очистить() { ESB_INVOKE_MEMB_PROC0(MethId::Clear); };
	// Function Copy(Array Rows_ = <default>, String Columns_ = <default>) -> ValueTable
	ТаблицаЗначений Скопировать(КонстПарам<Массив> Строки_ = ПоУмолчаниюНеопределеноИли<esb::Массив>, КонстПарам<Строка> Колонки_ = ПоУмолчаниюНеопределеноИли<esb::Строка>) const { ESB_INVOKE_MEMB_FUNC2(esb::ТаблицаЗначений, MethId::Copy, Строки_, Колонки_); };
	// Function Copy(Array Rows_ = <default>, String Columns_ = <default>) -> ValueTable
	ТаблицаЗначений Скопировать(КонстПарам<Структура> ПараметрыОтбора_ = ПоУмолчаниюНеопределеноИли<esb::Структура>, КонстПарам<Строка> Колонки_ = ПоУмолчаниюНеопределеноИли<esb::Строка>) const { ESB_INVOKE_MEMB_FUNC2(esb::ТаблицаЗначений, MethId::Copy, ПараметрыОтбора_, Колонки_); };
	// Function CopyColumns(String Columns_ = <default>) -> ValueTable
	ТаблицаЗначений СкопироватьКолонки(КонстПарам<Строка> Колонки_ = ПоУмолчаниюНеопределеноИли<esb::Строка>) const { ESB_INVOKE_MEMB_FUNC1(esb::ТаблицаЗначений, MethId::CopyColumns, Колонки_); };
	// Function Count() -> Number
	Число Количество() const { ESB_INVOKE_MEMB_FUNC0(esb::Число, MethId::Count); };
	// Void Delete(ValueTableRow Row_)
	void Удалить(КонстПарам<СтрокаТаблицыЗначений> Строка_) { ESB_INVOKE_MEMB_PROC1(MethId::Delete, Строка_); };
	// Void Delete(ValueTableRow Row_)
	void Удалить(КонстПарам<Число> Индекс_) { ESB_INVOKE_MEMB_PROC1(MethId::Delete, Индекс_); };
	// Void FillValues(Arbitrary Value_, String Columns_ = <default>)
	void ЗаполнитьЗначения(КонстПарам<Произвольный> Значение_, КонстПарам<Строка> Колонки_ = ПоУмолчаниюНеопределеноИли<esb::Строка>) { ESB_INVOKE_MEMB_PROC2(MethId::FillValues, Значение_, Колонки_); };
	// Function Find(Arbitrary Value_, String Columns_ = <default>) -> OneOf<Undefined, ValueTableRow>
	НеопределеноИли<СтрокаТаблицыЗначений> Найти(КонстПарам<Произвольный> Значение_, КонстПарам<Строка> Колонки_ = ПоУмолчаниюНеопределеноИли<esb::Строка>) const { ESB_INVOKE_MEMB_FUNC2(НеопределеноИли<esb::СтрокаТаблицыЗначений>, MethId::Find, Значение_, Колонки_); };
	// Function FindRows(Structure FilterParameters_) -> ArrayOf<ValueTableRow>
	МассивИз<СтрокаТаблицыЗначений> НайтиСтроки(КонстПарам<Структура> ПараметрыОтбора_) const { ESB_INVOKE_MEMB_FUNC1(esb::МассивИз<СтрокаТаблицыЗначений>, MethId::FindRows, ПараметрыОтбора_); };
	// Function Get(Number Index_) -> ValueTableRow
	СтрокаТаблицыЗначений Получить(КонстПарам<Число> Индекс_) const { ESB_INVOKE_MEMB_FUNC1(esb::СтрокаТаблицыЗначений, MethId::Get, Индекс_); };
	// Void GroupBy(String GroupingColumns_, String TotalingColumns_ = <default>)
	void Свернуть(КонстПарам<Строка> КолонкиГруппировок_, КонстПарам<Строка> КолонкиСуммирования_ = ПоУмолчаниюНеопределеноИли<esb::Строка>) { ESB_INVOKE_MEMB_PROC2(MethId::GroupBy, КолонкиГруппировок_, КолонкиСуммирования_); };
	// Function IndexOf(ValueTableRow Row_) -> Number
	Число Индекс(КонстПарам<СтрокаТаблицыЗначений> Строка_) const { ESB_INVOKE_MEMB_FUNC1(esb::Число, MethId::IndexOf, Строка_); };
	// Function Insert(Number Index_) -> ValueTableRow
	СтрокаТаблицыЗначений Вставить(КонстПарам<Число> Индекс_) { ESB_INVOKE_MEMB_FUNC1(esb::СтрокаТаблицыЗначений, MethId::Insert, Индекс_); };
	// Void LoadColumn(Array Array_, OneOf<Number, String, ValueTableColumn> Column_)
	void ЗагрузитьКолонку(КонстПарам<Массив> Массив_, КонстПарам<Число> Колонка_) { ESB_INVOKE_MEMB_PROC2(MethId::LoadColumn, Массив_, Колонка_); };
	void ЗагрузитьКолонку(КонстПарам<Массив> Массив_, КонстПарам<Строка> Колонка_) { ESB_INVOKE_MEMB_PROC2(MethId::LoadColumn, Массив_, Колонка_); };
	void ЗагрузитьКолонку(КонстПарам<Массив> Массив_, КонстПарам<КолонкаТаблицыЗначений> Колонка_) { ESB_INVOKE_MEMB_PROC2(MethId::LoadColumn, Массив_, Колонка_); };
	// Void Move(OneOf<Number, ValueTableRow> Row_, Number Offset_)
	void Сдвинуть(КонстПарам<Число> Строка_, КонстПарам<Число> Смещение_) { ESB_INVOKE_MEMB_PROC2(MethId::Move, Строка_, Смещение_); };
	void Сдвинуть(КонстПарам<СтрокаТаблицыЗначений> Строка_, КонстПарам<Число> Смещение_) { ESB_INVOKE_MEMB_PROC2(MethId::Move, Строка_, Смещение_); };
	// Void Sort(String Columns_, CompareValues ObjectOfComparison_ = <default>)
	void Сортировать(КонстПарам<Строка> Колонки_, КонстПарам<СравнениеЗначений> ОбъектСравнения_ = ПоУмолчаниюНеопределеноИли<esb::СравнениеЗначений>) { ESB_INVOKE_MEMB_PROC2(MethId::Sort, Колонки_, ОбъектСравнения_); };
	// Function Total(String Column_) -> OneOf<Number, Undefined>
	НеопределеноИли<Число> Итог(КонстПарам<Строка> Колонка_) const { ESB_INVOKE_MEMB_FUNC1(НеопределеноИли<esb::Число>, MethId::Total, Колонка_); };
	// Function UnloadColumn(OneOf<Number, String, ValueTableColumn> Column_) -> Array
	Массив ВыгрузитьКолонку(КонстПарам<Число> Колонка_) const { ESB_INVOKE_MEMB_FUNC1(esb::Массив, MethId::UnloadColumn, Колонка_); };
	Массив ВыгрузитьКолонку(КонстПарам<Строка> Колонка_) const { ESB_INVOKE_MEMB_FUNC1(esb::Массив, MethId::UnloadColumn, Колонка_); };
	Массив ВыгрузитьКолонку(КонстПарам<КолонкаТаблицыЗначений> Колонка_) const { ESB_INVOKE_MEMB_FUNC1(esb::Массив, MethId::UnloadColumn, Колонка_); };
public:
	// Property [[ReadOnly]] Columns -> ValueTableColumnCollection
	КоллекцияКолонокТаблицыЗначений ПолучитьКолонки() const { ESB_INVOKE_PROPERTY_GET(esb::КоллекцияКолонокТаблицыЗначений, PropId::Columns); };
	ESB_PROPERTY_FIELD_RO(КоллекцияКолонокТаблицыЗначений, Колонки)
		// Property [[ReadOnly]] Indexes -> CollectionIndexes
		ИндексыКоллекции ПолучитьИндексы() const { ESB_INVOKE_PROPERTY_GET(esb::ИндексыКоллекции, PropId::Indexes); };
	ESB_PROPERTY_FIELD_RO(ИндексыКоллекции, Индексы)
#endif	// #if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
public:
	ESB_IMPLEMENT_ENUM_ITEMS(ValueTable, ValueTableRow)
public:
	ESB_IMPLEMENT_INDEXED_COLLECTION_RO()
};


ESB_CLASS_OBJECT(ValueTreeColumn)
{
	ESB_CLASS_IMPLEMENT_OBJECT(ValueTreeColumn)
#if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
public:
	// Not Creatable
public:
	// Свойство [[Чтение,Запись]] Имя Как Строка
	String GetName() const { ESB_INVOKE_PROPERTY_GET(esb::String, PropId::Name); };
	void SetName(ConstParam<String> Value_) { ESB_INVOKE_PROPERTY_SET(PropId::Name, Value_); };
	ESB_PROPERTY_FIELD_RW(String, Name)
		// Свойство [[Чтение,Запись]] Заголовок Как Строка
		String GetTitle() const { ESB_INVOKE_PROPERTY_GET(esb::String, PropId::Title); };
	void SetTitle(ConstParam<String> Value_) { ESB_INVOKE_PROPERTY_SET(PropId::Title, Value_); };
	ESB_PROPERTY_FIELD_RW(String, Title)
		// Свойство [[ТолькоЧтение]] ТипЗначения Как ОписаниеТипов
		TypeDescription GetValueType() const { ESB_INVOKE_PROPERTY_GET(esb::TypeDescription, PropId::ValueType); };
	ESB_PROPERTY_FIELD_RO(TypeDescription, ValueType)
		// Свойство [[Чтение,Запись]] Ширина Как Число
		Number GetWidth() const { ESB_INVOKE_PROPERTY_GET(esb::Number, PropId::Width); };
	void SetWidth(ConstParam<Number> Value_) { ESB_INVOKE_PROPERTY_SET(PropId::Width, Value_); };
	ESB_PROPERTY_FIELD_RW(Number, Width)
#endif	// #if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
public:
	// Not Creatable
public:
	// Property [[Read,Write]] Name -> String
	Строка ПолучитьИмя() const { ESB_INVOKE_PROPERTY_GET(esb::Строка, PropId::Name); };
	void УстановитьИмя(КонстПарам<Строка> Значение_) { ESB_INVOKE_PROPERTY_SET(PropId::Name, Значение_); };
	ESB_PROPERTY_FIELD_RW(Строка, Имя)
		// Property [[Read,Write]] Title -> String
		Строка ПолучитьЗаголовок() const { ESB_INVOKE_PROPERTY_GET(esb::Строка, PropId::Title); };
	void УстановитьЗаголовок(КонстПарам<Строка> Значение_) { ESB_INVOKE_PROPERTY_SET(PropId::Title, Значение_); };
	ESB_PROPERTY_FIELD_RW(Строка, Заголовок)
		// Property [[ReadOnly]] ValueType -> TypeDescription
		ОписаниеТипов ПолучитьТипЗначения() const { ESB_INVOKE_PROPERTY_GET(esb::ОписаниеТипов, PropId::ValueType); };
	ESB_PROPERTY_FIELD_RO(ОписаниеТипов, ТипЗначения)
		// Property [[Read,Write]] Width -> Number
		Число ПолучитьШирина() const { ESB_INVOKE_PROPERTY_GET(esb::Число, PropId::Width); };
	void УстановитьШирина(КонстПарам<Число> Значение_) { ESB_INVOKE_PROPERTY_SET(PropId::Width, Значение_); };
	ESB_PROPERTY_FIELD_RW(Число, Ширина)
#endif	// #if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
};


ESB_CLASS_OBJECT_DUAL_WITH_DYNAMIC_PROPERTIES_RO(ValueTreeColumnCollection, ValueTreeColumn, IxCollectionRO<ValueTreeColumn>)
{
	ESB_CLASS_IMPLEMENT_OBJECT_DUAL_WITH_DYNAMIC_PROPERTIES(ValueTreeColumnCollection)
#if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
public:
	// Not Creatable
public:
	// Функция Добавить([Имя_ Как Строка], [Тип_ Как ОписаниеТипов], [Заголовок_ Как Строка], [Ширина_ Как Число]) Возвращает КолонкаДереваЗначений
	ValueTreeColumn Add(ConstParam<String> Name_ = DefUndef<esb::String>, ConstParam<TypeDescription> type_ = DefUndef<esb::TypeDescription>, ConstParam<String> Title_ = DefUndef<esb::String>, ConstParam<Number> Width_ = DefUndef<esb::Number>) { ESB_INVOKE_MEMB_FUNC4(esb::ValueTreeColumn, MethId::Add, Name_, type_, Title_, Width_); };
	// Процедура Очистить()
	void Clear() { ESB_INVOKE_MEMB_PROC0(MethId::Clear); };
	// Функция Количество() Возвращает Число
	Number Count() const { ESB_INVOKE_MEMB_FUNC0(esb::Number, MethId::Count); };
	// Процедура Удалить(Индекс_ Как ОдинИз<Число, КолонкаДереваЗначений>)
	void Delete(ConstParam<Number> Index_) { ESB_INVOKE_MEMB_PROC1(MethId::Delete, Index_); };
	void Delete(ConstParam<ValueTreeColumn> Index_) { ESB_INVOKE_MEMB_PROC1(MethId::Delete, Index_); };
	// Функция Найти(НаименованиеКолонки_ Как Строка) Возвращает ОдинИз<Неопределено, КолонкаДереваЗначений>
	UndefOr<ValueTreeColumn> Find(ConstParam<String> ColumnDescription_) const { ESB_INVOKE_MEMB_FUNC1(UndefOr<esb::ValueTreeColumn>, MethId::Find, ColumnDescription_); };
	// Функция Получить(Индекс_ Как Число) Возвращает КолонкаДереваЗначений
	ValueTreeColumn Get(ConstParam<Number> Index_) const { ESB_INVOKE_MEMB_FUNC1(esb::ValueTreeColumn, MethId::Get, Index_); };
	// Функция Индекс(Колонка_ Как КолонкаДереваЗначений) Возвращает Число
	Number IndexOf(ConstParam<ValueTreeColumn> Column_) const { ESB_INVOKE_MEMB_FUNC1(esb::Number, MethId::IndexOf, Column_); };
	// Функция Вставить(Индекс_ Как Число, [Имя_ Как Строка], [Тип_ Как ОписаниеТипов], [Заголовок_ Как Строка], [Ширина_ Как Число]) Возвращает КолонкаДереваЗначений
	ValueTreeColumn Insert(ConstParam<Number> Index_, ConstParam<String> Name_ = DefUndef<esb::String>, ConstParam<TypeDescription> type_ = DefUndef<esb::TypeDescription>, ConstParam<String> Title_ = DefUndef<esb::String>, ConstParam<Number> Width_ = DefUndef<esb::Number>) { ESB_INVOKE_MEMB_FUNC5(esb::ValueTreeColumn, MethId::Insert, Index_, Name_, type_, Title_, Width_); };
	// Процедура Сдвинуть(Колонка_ Как ОдинИз<Число, КолонкаДереваЗначений>, Смещение_ Как Число)
	void Move(ConstParam<Number> Column_, ConstParam<Number> Offset_) { ESB_INVOKE_MEMB_PROC2(MethId::Move, Column_, Offset_); };
	void Move(ConstParam<ValueTreeColumn> Column_, ConstParam<Number> Offset_) { ESB_INVOKE_MEMB_PROC2(MethId::Move, Column_, Offset_); };
public:
	ESB_IMPLEMENT_DYNAMIC_COLLECTION_RO_NAME_EN(ColumnName)
#endif	// #if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
public:
	// Not Creatable
public:
	// Function Add(String Name_ = <default>, TypeDescription Type_ = <default>, String Title_ = <default>, Number Width_ = <default>) -> ValueTreeColumn
	КолонкаДереваЗначений Добавить(КонстПарам<Строка> Имя_ = ПоУмолчаниюНеопределеноИли<esb::Строка>, КонстПарам<ОписаниеТипов> Тип_ = ПоУмолчаниюНеопределеноИли<esb::ОписаниеТипов>, КонстПарам<Строка> Заголовок_ = ПоУмолчаниюНеопределеноИли<esb::Строка>, КонстПарам<Число> Ширина_ = ПоУмолчаниюНеопределеноИли<esb::Число>) { ESB_INVOKE_MEMB_FUNC4(esb::КолонкаДереваЗначений, MethId::Add, Имя_, Тип_, Заголовок_, Ширина_); };
	// Void Clear()
	void Очистить() { ESB_INVOKE_MEMB_PROC0(MethId::Clear); };
	// Function Count() -> Number
	Число Количество() const { ESB_INVOKE_MEMB_FUNC0(esb::Число, MethId::Count); };
	// Void Delete(OneOf<Number, ValueTreeColumn> Index_)
	void Удалить(КонстПарам<Число> Индекс_) { ESB_INVOKE_MEMB_PROC1(MethId::Delete, Индекс_); };
	void Удалить(КонстПарам<КолонкаДереваЗначений> Индекс_) { ESB_INVOKE_MEMB_PROC1(MethId::Delete, Индекс_); };
	// Function Find(String ColumnDescription_) -> OneOf<Undefined, ValueTreeColumn>
	НеопределеноИли<КолонкаДереваЗначений> Найти(КонстПарам<Строка> НаименованиеКолонки_) const { ESB_INVOKE_MEMB_FUNC1(НеопределеноИли<esb::КолонкаДереваЗначений>, MethId::Find, НаименованиеКолонки_); };
	// Function Get(Number Index_) -> ValueTreeColumn
	КолонкаДереваЗначений Получить(КонстПарам<Число> Индекс_) const { ESB_INVOKE_MEMB_FUNC1(esb::КолонкаДереваЗначений, MethId::Get, Индекс_); };
	// Function IndexOf(ValueTreeColumn Column_) -> Number
	Число Индекс(КонстПарам<КолонкаДереваЗначений> Колонка_) const { ESB_INVOKE_MEMB_FUNC1(esb::Число, MethId::IndexOf, Колонка_); };
	// Function Insert(Number Index_, String Name_ = <default>, TypeDescription Type_ = <default>, String Title_ = <default>, Number Width_ = <default>) -> ValueTreeColumn
	КолонкаДереваЗначений Вставить(КонстПарам<Число> Индекс_, КонстПарам<Строка> Имя_ = ПоУмолчаниюНеопределеноИли<esb::Строка>, КонстПарам<ОписаниеТипов> Тип_ = ПоУмолчаниюНеопределеноИли<esb::ОписаниеТипов>, КонстПарам<Строка> Заголовок_ = ПоУмолчаниюНеопределеноИли<esb::Строка>, КонстПарам<Число> Ширина_ = ПоУмолчаниюНеопределеноИли<esb::Число>) { ESB_INVOKE_MEMB_FUNC5(esb::КолонкаДереваЗначений, MethId::Insert, Индекс_, Имя_, Тип_, Заголовок_, Ширина_); };
	// Void Move(OneOf<Number, ValueTreeColumn> Column_, Number Offset_)
	void Сдвинуть(КонстПарам<Число> Колонка_, КонстПарам<Число> Смещение_) { ESB_INVOKE_MEMB_PROC2(MethId::Move, Колонка_, Смещение_); };
	void Сдвинуть(КонстПарам<КолонкаДереваЗначений> Колонка_, КонстПарам<Число> Смещение_) { ESB_INVOKE_MEMB_PROC2(MethId::Move, Колонка_, Смещение_); };
public:
	ESB_IMPLEMENT_DYNAMIC_COLLECTION_RO_NAME_RU(ИмяКолонки)
#endif	// #if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
public:
	ESB_IMPLEMENT_ENUM_ITEMS(ValueTreeColumnCollection, ValueTreeColumn)
public:
	ESB_IMPLEMENT_INDEXED_COLLECTION_RO()
};


ESB_CLASS_OBJECT_DUAL_WITH_DYNAMIC_PROPERTIES_RW(ValueTreeRow, Arbitrary, IxCollectionRW<Arbitrary>)
{
	ESB_CLASS_IMPLEMENT_OBJECT_DUAL_WITH_DYNAMIC_PROPERTIES(ValueTreeRow)
#if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
public:
	// Not Creatable
public:
	// Функция Получить(Индекс_ Как Число) Возвращает Произвольный
	Arbitrary Get(ConstParam<Number> Index_) const { ESB_INVOKE_MEMB_FUNC1(esb::Arbitrary, MethId::Get, Index_); };
	// Функция Уровень() Возвращает Число
	Number Level() const { ESB_INVOKE_MEMB_FUNC0(esb::Number, MethId::Level); };
	// Функция Владелец() Возвращает ДеревоЗначений
	ValueTree Owner() const; // defer implementation  { ESB_INVOKE_MEMB_FUNC0(esb::ValueTree, MethId::Owner); };
	// Процедура Установить(Индекс_ Как Число, Значение_ Как Произвольный)
	void Set(ConstParam<Number> Index_, ConstParam<Arbitrary> Value_) { ESB_INVOKE_MEMB_PROC2(MethId::Set, Index_, Value_); };
public:
	// Свойство [[ТолькоЧтение]] Родитель Как СтрокаДереваЗначений
	ValueTreeRow GetParent() const { ESB_INVOKE_PROPERTY_GET(esb::ValueTreeRow, PropId::Parent); };
	ESB_PROPERTY_FIELD_RO(ValueTreeRow, Parent)
	// Свойство [[ТолькоЧтение]] Строки Как КоллекцияСтрокДереваЗначений
	ValueTreeRowCollection GetRows() const; // defer implementation  { ESB_INVOKE_PROPERTY_GET(esb::ValueTreeRowCollection, PropId::Rows); };
	ESB_PROPERTY_FIELD_RO(ValueTreeRowCollection, Rows)
public:
	ESB_IMPLEMENT_DYNAMIC_COLLECTION_RW_NAME_EN(ColumnName)
#endif	// #if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
public:
	// Not Creatable
public:
	// Function Get(Number Index_) -> Arbitrary
	Произвольный Получить(КонстПарам<Число> Индекс_) const { ESB_INVOKE_MEMB_FUNC1(esb::Произвольный, MethId::Get, Индекс_); };
	// Function Level() -> Number
	Число Уровень() const { ESB_INVOKE_MEMB_FUNC0(esb::Число, MethId::Level); };
	// Function Owner() -> ValueTree
	ДеревоЗначений Владелец() const; // defer implementation  { ESB_INVOKE_MEMB_FUNC0(esb::ДеревоЗначений, MethId::Owner); };
	// Void Set(Number Index_, Arbitrary Value_)
	void Установить(КонстПарам<Число> Индекс_, КонстПарам<Произвольный> Значение_) { ESB_INVOKE_MEMB_PROC2(MethId::Set, Индекс_, Значение_); };
public:
	// Property [[ReadOnly]] Parent -> ValueTreeRow
	СтрокаДереваЗначений ПолучитьРодитель() const { ESB_INVOKE_PROPERTY_GET(esb::СтрокаДереваЗначений, PropId::Parent); };
	ESB_PROPERTY_FIELD_RO(СтрокаДереваЗначений, Родитель)
	// Property [[ReadOnly]] Rows -> ValueTreeRowCollection
	КоллекцияСтрокДереваЗначений ПолучитьСтроки() const; // defer implementation  { ESB_INVOKE_PROPERTY_GET(esb::КоллекцияСтрокДереваЗначений, PropId::Rows); };
	ESB_PROPERTY_FIELD_RO(КоллекцияСтрокДереваЗначений, Строки)
public:
	ESB_IMPLEMENT_DYNAMIC_COLLECTION_RW_NAME_RU(ИмяКолонки)
#endif	// #if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
public:
	ESB_IMPLEMENT_ENUM_ITEMS(ValueTreeRow, Arbitrary)
public:
	ESB_IMPLEMENT_INDEXED_COLLECTION_RW()
};


ESB_CLASS_OBJECT_DUAL(ValueTreeRowCollection, IxCollectionRO<ValueTreeRow>)
{
	ESB_CLASS_IMPLEMENT_OBJECT_DUAL(ValueTreeRowCollection)
#if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
public:
	// Not Creatable
public:
	// Функция Добавить() Возвращает СтрокаДереваЗначений
	ValueTreeRow Add() { ESB_INVOKE_MEMB_FUNC0(esb::ValueTreeRow, MethId::Add); };
	// Процедура Очистить()
	void Clear() { ESB_INVOKE_MEMB_PROC0(MethId::Clear); };
	// Функция Количество() Возвращает Число
	Number Count() const { ESB_INVOKE_MEMB_FUNC0(esb::Number, MethId::Count); };
	// Процедура Удалить(Строка_ Как СтрокаДереваЗначений)
	void Delete(ConstParam<ValueTreeRow> Row_) { ESB_INVOKE_MEMB_PROC1(MethId::Delete, Row_); };
	// Процедура Удалить(Строка_ Как СтрокаДереваЗначений)
	void Delete(ConstParam<Number> Index_) { ESB_INVOKE_MEMB_PROC1(MethId::Delete, Index_); };
	// Функция Найти(Значение_ Как Произвольный, [Колонки_ Как Строка], [ВключатьПодчиненные_ Как Булево]) Возвращает ОдинИз<Неопределено, СтрокаДереваЗначений>
	UndefOr<ValueTreeRow> Find(ConstParam<Arbitrary> Value_, ConstParam<String> Columns_ = DefUndef<esb::String>, ConstParam<Boolean> IncludeSubordinates_ = DefUndef<esb::Boolean>) const { ESB_INVOKE_MEMB_FUNC3(UndefOr<esb::ValueTreeRow>, MethId::Find, Value_, Columns_, IncludeSubordinates_); };
	// Функция НайтиСтроки(ПараметрыОтбора_ Как Структура, [ВключатьПодчиненные_ Как Булево]) Возвращает МассивИз<СтрокаДереваЗначений>
	ArrayOf<ValueTreeRow> FindRows(ConstParam<Structure> FilterParameters_, ConstParam<Boolean> IncludeSubordinates_ = DefUndef<esb::Boolean>) const { ESB_INVOKE_MEMB_FUNC2(esb::ArrayOf<ValueTreeRow>, MethId::FindRows, FilterParameters_, IncludeSubordinates_); };
	// Функция Получить(Индекс_ Как Число) Возвращает СтрокаДереваЗначений
	ValueTreeRow Get(ConstParam<Number> Index_) const { ESB_INVOKE_MEMB_FUNC1(esb::ValueTreeRow, MethId::Get, Index_); };
	// Функция Индекс(Строка_ Как СтрокаДереваЗначений) Возвращает Число
	Number IndexOf(ConstParam<ValueTreeRow> Row_) const { ESB_INVOKE_MEMB_FUNC1(esb::Number, MethId::IndexOf, Row_); };
	// Функция Вставить(Индекс_ Как Число) Возвращает СтрокаДереваЗначений
	ValueTreeRow Insert(ConstParam<Number> Index_) { ESB_INVOKE_MEMB_FUNC1(esb::ValueTreeRow, MethId::Insert, Index_); };
	// Процедура ЗагрузитьКолонку(Массив_ Как Массив, Колонка_ Как ОдинИз<Число, Строка, КолонкаДереваЗначений>)
	void LoadColumn(ConstParam<Array> Array_, ConstParam<Number> Column_) { ESB_INVOKE_MEMB_PROC2(MethId::LoadColumn, Array_, Column_); };
	void LoadColumn(ConstParam<Array> Array_, ConstParam<String> Column_) { ESB_INVOKE_MEMB_PROC2(MethId::LoadColumn, Array_, Column_); };
	void LoadColumn(ConstParam<Array> Array_, ConstParam<ValueTreeColumn> Column_) { ESB_INVOKE_MEMB_PROC2(MethId::LoadColumn, Array_, Column_); };
	// Процедура Сдвинуть(Строка_ Как ОдинИз<Число, СтрокаДереваЗначений>, Смещение_ Как Число)
	void Move(ConstParam<Number> Row_, ConstParam<Number> Offset_) { ESB_INVOKE_MEMB_PROC2(MethId::Move, Row_, Offset_); };
	void Move(ConstParam<ValueTreeRow> Row_, ConstParam<Number> Offset_) { ESB_INVOKE_MEMB_PROC2(MethId::Move, Row_, Offset_); };
	// Процедура Сортировать(СтрокаСортировки_ Как Строка, [ВключатьПодчиненные_ Как Булево], [ОбъектСравнения_ Как СравнениеЗначений])
	void Sort(ConstParam<String> SortingLine_, ConstParam<Boolean> IncludeSubordinates_ = DefUndef<esb::Boolean>, ConstParam<CompareValues> ObjectOfComparison_ = DefUndef<esb::CompareValues>) { ESB_INVOKE_MEMB_PROC3(MethId::Sort, SortingLine_, IncludeSubordinates_, ObjectOfComparison_); };
	// Функция Итог(Колонка_ Как ОдинИз<Число, Строка, КолонкаДереваЗначений>, [ВключатьПодчиненные_ Как Булево]) Возвращает ОдинИз<Число, Неопределено>
	UndefOr<Number> Total(ConstParam<Number> Column_, ConstParam<Boolean> IncludeSubordinates_ = DefUndef<esb::Boolean>) const { ESB_INVOKE_MEMB_FUNC2(UndefOr<esb::Number>, MethId::Total, Column_, IncludeSubordinates_); };
	UndefOr<Number> Total(ConstParam<String> Column_, ConstParam<Boolean> IncludeSubordinates_ = DefUndef<esb::Boolean>) const { ESB_INVOKE_MEMB_FUNC2(UndefOr<esb::Number>, MethId::Total, Column_, IncludeSubordinates_); };
	UndefOr<Number> Total(ConstParam<ValueTreeColumn> Column_, ConstParam<Boolean> IncludeSubordinates_ = DefUndef<esb::Boolean>) const { ESB_INVOKE_MEMB_FUNC2(UndefOr<esb::Number>, MethId::Total, Column_, IncludeSubordinates_); };
	// Функция ВыгрузитьКолонку(Колонка_ Как ОдинИз<Число, Строка, КолонкаДереваЗначений>) Возвращает Массив
	Array UnloadColumn(ConstParam<Number> Column_) const { ESB_INVOKE_MEMB_FUNC1(esb::Array, MethId::UnloadColumn, Column_); };
	Array UnloadColumn(ConstParam<String> Column_) const { ESB_INVOKE_MEMB_FUNC1(esb::Array, MethId::UnloadColumn, Column_); };
	Array UnloadColumn(ConstParam<ValueTreeColumn> Column_) const { ESB_INVOKE_MEMB_FUNC1(esb::Array, MethId::UnloadColumn, Column_); };
public:
	// Свойство [[ТолькоЧтение]] Родитель Как ОдинИз<Неопределено, СтрокаДереваЗначений>
	UndefOr<ValueTreeRow> GetParent() const { ESB_INVOKE_PROPERTY_GET(UndefOr<esb::ValueTreeRow>, PropId::Parent); };
	ESB_PROPERTY_FIELD_RO(UndefOr<ValueTreeRow>, Parent)
#endif	// #if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
public:
	// Not Creatable
public:
	// Function Add() -> ValueTreeRow
	СтрокаДереваЗначений Добавить() { ESB_INVOKE_MEMB_FUNC0(esb::СтрокаДереваЗначений, MethId::Add); };
	// Void Clear()
	void Очистить() { ESB_INVOKE_MEMB_PROC0(MethId::Clear); };
	// Function Count() -> Number
	Число Количество() const { ESB_INVOKE_MEMB_FUNC0(esb::Число, MethId::Count); };
	// Void Delete(ValueTreeRow Row_)
	void Удалить(КонстПарам<СтрокаДереваЗначений> Строка_) { ESB_INVOKE_MEMB_PROC1(MethId::Delete, Строка_); };
	// Void Delete(ValueTreeRow Row_)
	void Удалить(КонстПарам<Число> Индекс_) { ESB_INVOKE_MEMB_PROC1(MethId::Delete, Индекс_); };
	// Function Find(Arbitrary Value_, String Columns_ = <default>, Boolean IncludeSubordinates_ = <default>) -> OneOf<Undefined, ValueTreeRow>
	НеопределеноИли<СтрокаДереваЗначений> Найти(КонстПарам<Произвольный> Значение_, КонстПарам<Строка> Колонки_ = ПоУмолчаниюНеопределеноИли<esb::Строка>, КонстПарам<Булево> ВключатьПодчиненные_ = ПоУмолчаниюНеопределеноИли<esb::Булево>) const { ESB_INVOKE_MEMB_FUNC3(НеопределеноИли<esb::СтрокаДереваЗначений>, MethId::Find, Значение_, Колонки_, ВключатьПодчиненные_); };
	// Function FindRows(Structure FilterParameters_, Boolean IncludeSubordinates_ = <default>) -> ArrayOf<ValueTreeRow>
	МассивИз<СтрокаДереваЗначений> НайтиСтроки(КонстПарам<Структура> ПараметрыОтбора_, КонстПарам<Булево> ВключатьПодчиненные_ = ПоУмолчаниюНеопределеноИли<esb::Булево>) const { ESB_INVOKE_MEMB_FUNC2(esb::МассивИз<СтрокаДереваЗначений>, MethId::FindRows, ПараметрыОтбора_, ВключатьПодчиненные_); };
	// Function Get(Number Index_) -> ValueTreeRow
	СтрокаДереваЗначений Получить(КонстПарам<Число> Индекс_) const { ESB_INVOKE_MEMB_FUNC1(esb::СтрокаДереваЗначений, MethId::Get, Индекс_); };
	// Function IndexOf(ValueTreeRow Row_) -> Number
	Число Индекс(КонстПарам<СтрокаДереваЗначений> Строка_) const { ESB_INVOKE_MEMB_FUNC1(esb::Число, MethId::IndexOf, Строка_); };
	// Function Insert(Number Index_) -> ValueTreeRow
	СтрокаДереваЗначений Вставить(КонстПарам<Число> Индекс_) { ESB_INVOKE_MEMB_FUNC1(esb::СтрокаДереваЗначений, MethId::Insert, Индекс_); };
	// Void LoadColumn(Array Array_, OneOf<Number, String, ValueTreeColumn> Column_)
	void ЗагрузитьКолонку(КонстПарам<Массив> Массив_, КонстПарам<Число> Колонка_) { ESB_INVOKE_MEMB_PROC2(MethId::LoadColumn, Массив_, Колонка_); };
	void ЗагрузитьКолонку(КонстПарам<Массив> Массив_, КонстПарам<Строка> Колонка_) { ESB_INVOKE_MEMB_PROC2(MethId::LoadColumn, Массив_, Колонка_); };
	void ЗагрузитьКолонку(КонстПарам<Массив> Массив_, КонстПарам<КолонкаДереваЗначений> Колонка_) { ESB_INVOKE_MEMB_PROC2(MethId::LoadColumn, Массив_, Колонка_); };
	// Void Move(OneOf<Number, ValueTreeRow> Row_, Number Offset_)
	void Сдвинуть(КонстПарам<Число> Строка_, КонстПарам<Число> Смещение_) { ESB_INVOKE_MEMB_PROC2(MethId::Move, Строка_, Смещение_); };
	void Сдвинуть(КонстПарам<СтрокаДереваЗначений> Строка_, КонстПарам<Число> Смещение_) { ESB_INVOKE_MEMB_PROC2(MethId::Move, Строка_, Смещение_); };
	// Void Sort(String SortingLine_, Boolean IncludeSubordinates_ = <default>, CompareValues ObjectOfComparison_ = <default>)
	void Сортировать(КонстПарам<Строка> СтрокаСортировки_, КонстПарам<Булево> ВключатьПодчиненные_ = ПоУмолчаниюНеопределеноИли<esb::Булево>, КонстПарам<СравнениеЗначений> ОбъектСравнения_ = ПоУмолчаниюНеопределеноИли<esb::СравнениеЗначений>) { ESB_INVOKE_MEMB_PROC3(MethId::Sort, СтрокаСортировки_, ВключатьПодчиненные_, ОбъектСравнения_); };
	// Function Total(OneOf<Number, String, ValueTreeColumn> Column_, Boolean IncludeSubordinates_ = <default>) -> OneOf<Number, Undefined>
	НеопределеноИли<Число> Итог(КонстПарам<Число> Колонка_, КонстПарам<Булево> ВключатьПодчиненные_ = ПоУмолчаниюНеопределеноИли<esb::Булево>) const { ESB_INVOKE_MEMB_FUNC2(НеопределеноИли<esb::Число>, MethId::Total, Колонка_, ВключатьПодчиненные_); };
	НеопределеноИли<Число> Итог(КонстПарам<Строка> Колонка_, КонстПарам<Булево> ВключатьПодчиненные_ = ПоУмолчаниюНеопределеноИли<esb::Булево>) const { ESB_INVOKE_MEMB_FUNC2(НеопределеноИли<esb::Число>, MethId::Total, Колонка_, ВключатьПодчиненные_); };
	НеопределеноИли<Число> Итог(КонстПарам<КолонкаДереваЗначений> Колонка_, КонстПарам<Булево> ВключатьПодчиненные_ = ПоУмолчаниюНеопределеноИли<esb::Булево>) const { ESB_INVOKE_MEMB_FUNC2(НеопределеноИли<esb::Число>, MethId::Total, Колонка_, ВключатьПодчиненные_); };
	// Function UnloadColumn(OneOf<Number, String, ValueTreeColumn> Column_) -> Array
	Массив ВыгрузитьКолонку(КонстПарам<Число> Колонка_) const { ESB_INVOKE_MEMB_FUNC1(esb::Массив, MethId::UnloadColumn, Колонка_); };
	Массив ВыгрузитьКолонку(КонстПарам<Строка> Колонка_) const { ESB_INVOKE_MEMB_FUNC1(esb::Массив, MethId::UnloadColumn, Колонка_); };
	Массив ВыгрузитьКолонку(КонстПарам<КолонкаДереваЗначений> Колонка_) const { ESB_INVOKE_MEMB_FUNC1(esb::Массив, MethId::UnloadColumn, Колонка_); };
public:
	// Property [[ReadOnly]] Parent -> OneOf<Undefined, ValueTreeRow>
	НеопределеноИли<СтрокаДереваЗначений> ПолучитьРодитель() const { ESB_INVOKE_PROPERTY_GET(НеопределеноИли<esb::СтрокаДереваЗначений>, PropId::Parent); };
	ESB_PROPERTY_FIELD_RO(НеопределеноИли<СтрокаДереваЗначений>, Родитель)
#endif	// #if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
public:
	ESB_IMPLEMENT_ENUM_ITEMS(ValueTreeRowCollection, ValueTreeRow)
public:
	ESB_IMPLEMENT_INDEXED_COLLECTION_RO()
};


ESB_CLASS_OBJECT(ValueTree)
{
	ESB_CLASS_IMPLEMENT_OBJECT(ValueTree)
#if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
public:
	static ValueTree Create() { ESB_INVOKE_CTOR0(ValueTree); }
public:
	// Функция ВыбратьСтроку([Заголовок_ Как Строка], [НачальнаяСтрока_ Как СтрокаДереваЗначений]) Возвращает ОдинИз<Неопределено, СтрокаДереваЗначений>
	UndefOr<ValueTreeRow> ChooseRow(ConstParam<String> Title_ = DefUndef<esb::String>, ConstParam<ValueTreeRow> InitialRow_ = DefUndef<esb::ValueTreeRow>) const { ESB_INVOKE_MEMB_FUNC2(UndefOr<esb::ValueTreeRow>, MethId::ChooseRow, Title_, InitialRow_); };
	// Функция Скопировать() Возвращает ДеревоЗначений
	ValueTree Copy() const { ESB_INVOKE_MEMB_FUNC0(esb::ValueTree, MethId::Copy); };
public:
	// Свойство [[ТолькоЧтение]] Колонки Как КоллекцияКолонокДереваЗначений
	ValueTreeColumnCollection GetColumns() const { ESB_INVOKE_PROPERTY_GET(esb::ValueTreeColumnCollection, PropId::Columns); };
	ESB_PROPERTY_FIELD_RO(ValueTreeColumnCollection, Columns)
	// Свойство [[ТолькоЧтение]] Строки Как КоллекцияСтрокДереваЗначений
	ValueTreeRowCollection GetRows(); // defer implementation  const { ESB_INVOKE_PROPERTY_GET(esb::ValueTreeRowCollection, PropId::Rows); };
	ESB_PROPERTY_FIELD_RO(ValueTreeRowCollection, Rows)
#endif	// #if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
public:
	static ДеревоЗначений Создать() { ESB_INVOKE_CTOR0(ДеревоЗначений); }
public:
	// Function ChooseRow(String Title_ = <default>, ValueTreeRow InitialRow_ = <default>) -> OneOf<Undefined, ValueTreeRow>
	НеопределеноИли<СтрокаДереваЗначений> ВыбратьСтроку(КонстПарам<Строка> Заголовок_ = ПоУмолчаниюНеопределеноИли<esb::Строка>, КонстПарам<СтрокаДереваЗначений> НачальнаяСтрока_ = ПоУмолчаниюНеопределеноИли<esb::СтрокаДереваЗначений>) const { ESB_INVOKE_MEMB_FUNC2(НеопределеноИли<esb::СтрокаДереваЗначений>, MethId::ChooseRow, Заголовок_, НачальнаяСтрока_); };
	// Function Copy() -> ValueTree
	ДеревоЗначений Скопировать() const { ESB_INVOKE_MEMB_FUNC0(esb::ДеревоЗначений, MethId::Copy); };
public:
	// Property [[ReadOnly]] Columns -> ValueTreeColumnCollection
	КоллекцияКолонокДереваЗначений ПолучитьКолонки() const { ESB_INVOKE_PROPERTY_GET(esb::КоллекцияКолонокДереваЗначений, PropId::Columns); };
	ESB_PROPERTY_FIELD_RO(КоллекцияКолонокДереваЗначений, Колонки)
	// Property [[ReadOnly]] Rows -> ValueTreeRowCollection
	КоллекцияСтрокДереваЗначений ПолучитьСтроки() const; // defer implementation  { ESB_INVOKE_PROPERTY_GET(esb::КоллекцияСтрокДереваЗначений, PropId::Rows); };
	ESB_PROPERTY_FIELD_RO(КоллекцияСтрокДереваЗначений, Строки)
#endif	// #if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
};


ESB_CLASS_OBJECT(QueryParameterDescription)
{
	ESB_CLASS_IMPLEMENT_OBJECT(QueryParameterDescription)
#if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
public:
	// Not Creatable
public:
	// Свойство [[ТолькоЧтение]] Имя Как Строка
	String GetName() const { ESB_INVOKE_PROPERTY_GET(esb::String, PropId::Name); };
	ESB_PROPERTY_FIELD_RO(String, Name)
		// Свойство [[ТолькоЧтение]] ТипЗначения Как ОписаниеТипов
		TypeDescription GetValueType() const { ESB_INVOKE_PROPERTY_GET(esb::TypeDescription, PropId::ValueType); };
	ESB_PROPERTY_FIELD_RO(TypeDescription, ValueType)
#endif	// #if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
public:
	// Not Creatable
public:
	// Property [[ReadOnly]] Name -> String
	Строка ПолучитьИмя() const { ESB_INVOKE_PROPERTY_GET(esb::Строка, PropId::Name); };
	ESB_PROPERTY_FIELD_RO(Строка, Имя)
		// Property [[ReadOnly]] ValueType -> TypeDescription
		ОписаниеТипов ПолучитьТипЗначения() const { ESB_INVOKE_PROPERTY_GET(esb::ОписаниеТипов, PropId::ValueType); };
	ESB_PROPERTY_FIELD_RO(ОписаниеТипов, ТипЗначения)
#endif	// #if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
};


ESB_CLASS_OBJECT_DUAL_WITH_DYNAMIC_PROPERTIES_RO(QueryParametersDescription, QueryParameterDescription, AxCollectionRO<Arbitrary, QueryParameterDescription>)
{
	ESB_CLASS_IMPLEMENT_OBJECT_DUAL_WITH_DYNAMIC_PROPERTIES(QueryParametersDescription)
#if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
public:
	// Not Creatable
public:
	// Функция Количество() Возвращает Число
	Number Count() const { ESB_INVOKE_MEMB_FUNC0(esb::Number, MethId::Count); };
	// Функция Найти(Имя_ Как Строка) Возвращает ОдинИз<ОписаниеПараметраЗапроса, Неопределено>
	UndefOr<QueryParameterDescription> Find(ConstParam<String> Name_) const { ESB_INVOKE_MEMB_FUNC1(UndefOr<esb::QueryParameterDescription>, MethId::Find, Name_); };
public:
	ESB_IMPLEMENT_DYNAMIC_COLLECTION_RO_NAME_EN(ParameterName)
#endif	// #if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
public:
	// Not Creatable
public:
	// Function Count() -> Number
	Число Количество() const { ESB_INVOKE_MEMB_FUNC0(esb::Число, MethId::Count); };
	// Function Find(String Name_) -> OneOf<QueryParameterDescription, Undefined>
	НеопределеноИли<ОписаниеПараметраЗапроса> Найти(КонстПарам<Строка> Имя_) const { ESB_INVOKE_MEMB_FUNC1(НеопределеноИли<esb::ОписаниеПараметраЗапроса>, MethId::Find, Имя_); };
public:
	ESB_IMPLEMENT_DYNAMIC_COLLECTION_RO_NAME_RU(ИмяПараметра)
#endif	// #if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
public:
	ESB_IMPLEMENT_ENUM_ITEMS(QueryParametersDescription, QueryParameterDescription)
public:
	// MANUAL!!! ESB_IMPLEMENT_ASSICIATIVE_COLLECTION()
};


ESB_CLASS_OBJECT(QueryResultColumn)
{
	ESB_CLASS_IMPLEMENT_OBJECT(QueryResultColumn)
#if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
public:
	// Not Creatable
public:
	// Свойство [[ТолькоЧтение]] Имя Как Строка
	String GetName() const { ESB_INVOKE_PROPERTY_GET(esb::String, PropId::Name); };
	ESB_PROPERTY_FIELD_RO(String, Name)
		// Свойство [[ТолькоЧтение]] ТипЗначения Как ОписаниеТипов
		TypeDescription GetValueType() const { ESB_INVOKE_PROPERTY_GET(esb::TypeDescription, PropId::ValueType); };
	ESB_PROPERTY_FIELD_RO(TypeDescription, ValueType)
		// Свойство [[ТолькоЧтение]] Ширина Как Число
		Number GetWidth() const { ESB_INVOKE_PROPERTY_GET(esb::Number, PropId::Width); };
	ESB_PROPERTY_FIELD_RO(Number, Width)
#endif	// #if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
public:
	// Not Creatable
public:
	// Property [[ReadOnly]] Name -> String
	Строка ПолучитьИмя() const { ESB_INVOKE_PROPERTY_GET(esb::Строка, PropId::Name); };
	ESB_PROPERTY_FIELD_RO(Строка, Имя)
		// Property [[ReadOnly]] ValueType -> TypeDescription
		ОписаниеТипов ПолучитьТипЗначения() const { ESB_INVOKE_PROPERTY_GET(esb::ОписаниеТипов, PropId::ValueType); };
	ESB_PROPERTY_FIELD_RO(ОписаниеТипов, ТипЗначения)
		// Property [[ReadOnly]] Width -> Number
		Число ПолучитьШирина() const { ESB_INVOKE_PROPERTY_GET(esb::Число, PropId::Width); };
	ESB_PROPERTY_FIELD_RO(Число, Ширина)
#endif	// #if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
};


ESB_CLASS_OBJECT_DUAL2_WITH_DYNAMIC_PROPERTIES_RO(QueryResultColumnsCollection, QueryResultColumn, IxCollectionRO<QueryResultColumn>, AxCollectionRO<Arbitrary, QueryResultColumn>)
{
	ESB_CLASS_IMPLEMENT_OBJECT_DUAL2_WITH_DYNAMIC_PROPERTIES(QueryResultColumnsCollection)
#if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
public:
	// Not Creatable
public:
	// Функция Количество() Возвращает Число
	Number Count() const { ESB_INVOKE_MEMB_FUNC0(esb::Number, MethId::Count); };
	// Функция Найти(НаименованиеКолонки_ Как Строка) Возвращает ОдинИз<КолонкаРезультатаЗапроса, Неопределено>
	UndefOr<QueryResultColumn> Find(ConstParam<String> ColumnDescription_) const { ESB_INVOKE_MEMB_FUNC1(UndefOr<esb::QueryResultColumn>, MethId::Find, ColumnDescription_); };
	// Функция Получить(Индекс_ Как Число) Возвращает КолонкаРезультатаЗапроса
	QueryResultColumn Get(ConstParam<Number> Index_) const { ESB_INVOKE_MEMB_FUNC1(esb::QueryResultColumn, MethId::Get, Index_); };
	// Функция Индекс(Колонка_ Как КолонкаРезультатаЗапроса) Возвращает Число
	Number IndexOf(ConstParam<QueryResultColumn> Column_) const { ESB_INVOKE_MEMB_FUNC1(esb::Number, MethId::IndexOf, Column_); };
public:
	ESB_IMPLEMENT_DYNAMIC_COLLECTION_RO_NAME_EN(ColumnName)
#endif	// #if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
public:
	// Not Creatable
public:
	// Function Count() -> Number
	Число Количество() const { ESB_INVOKE_MEMB_FUNC0(esb::Число, MethId::Count); };
	// Function Find(String ColumnDescription_) -> OneOf<QueryResultColumn, Undefined>
	НеопределеноИли<КолонкаРезультатаЗапроса> Найти(КонстПарам<Строка> НаименованиеКолонки_) const { ESB_INVOKE_MEMB_FUNC1(НеопределеноИли<esb::КолонкаРезультатаЗапроса>, MethId::Find, НаименованиеКолонки_); };
	// Function Get(Number Index_) -> QueryResultColumn
	КолонкаРезультатаЗапроса Получить(КонстПарам<Число> Индекс_) const { ESB_INVOKE_MEMB_FUNC1(esb::КолонкаРезультатаЗапроса, MethId::Get, Индекс_); };
	// Function IndexOf(QueryResultColumn Column_) -> Number
	Число Индекс(КонстПарам<КолонкаРезультатаЗапроса> Колонка_) const { ESB_INVOKE_MEMB_FUNC1(esb::Число, MethId::IndexOf, Колонка_); };
public:
	ESB_IMPLEMENT_DYNAMIC_COLLECTION_RO_NAME_RU(ИмяКолонки)
#endif	// #if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
public:
	ESB_IMPLEMENT_ENUM_ITEMS(QueryResultColumnsCollection, QueryResultColumn)
public:
	ESB_IMPLEMENT_INDEXED_COLLECTION_RO()
public:
	// MANUAL!!! ESB_IMPLEMENT_ASSICIATIVE_COLLECTION()
};


ESB_CLASS_OBJECT_DUAL2_WITH_DYNAMIC_PROPERTIES_RO(QueryResultSelection, Arbitrary, IxCollectionRO<Arbitrary>, AxCollectionRO<Arbitrary, Arbitrary>)
{
	ESB_CLASS_IMPLEMENT_OBJECT_DUAL2_WITH_DYNAMIC_PROPERTIES(QueryResultSelection)
#if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
public:
	// Not Creatable
public:
	// Функция Количество() Возвращает Число
	Number Count() const { ESB_INVOKE_MEMB_FUNC0(esb::Number, MethId::Count); };
	// Функция НайтиСледующий(СтруктураПоиска_ Как Структура) Возвращает Булево
	Boolean FindNext(ConstParam<Structure> TheStructureOfTheSearch_) { ESB_INVOKE_MEMB_FUNC2(esb::Boolean, MethId::FindNext, TheStructureOfTheSearch_, ESB_DEFARG(Undefined)); };
	// Функция НайтиСледующий(СтруктураПоиска_ Как Структура) Возвращает Булево
	Boolean FindNext(ConstParam<Arbitrary> Value_, ConstParam<String> Columns_ = DefUndef<esb::String>) { ESB_INVOKE_MEMB_FUNC2(esb::Boolean, MethId::FindNext, Value_, Columns_); };
	// Функция Получить(Индекс_ Как Число) Возвращает Произвольный
	Arbitrary Get(ConstParam<Number> Index_) const { ESB_INVOKE_MEMB_FUNC1(esb::Arbitrary, MethId::Get, Index_); };
	// Функция Группировка() Возвращает Строка
	String Group() const { ESB_INVOKE_MEMB_FUNC0(esb::String, MethId::Group); };
	// Функция Уровень() Возвращает Число
	Number Level() const { ESB_INVOKE_MEMB_FUNC0(esb::Number, MethId::Level); };
	// Функция Следующий() Возвращает Булево
	Boolean Next() { ESB_INVOKE_MEMB_FUNC0(esb::Boolean, MethId::Next); };
	// Функция СледующийПоЗначениюПоля(ИмяПоля_ Как Строка) Возвращает Булево
	Boolean NextByFieldValue(ConstParam<String> FieldName_) { ESB_INVOKE_MEMB_FUNC1(esb::Boolean, MethId::NextByFieldValue, FieldName_); };
	// Функция Владелец() Возвращает РезультатЗапроса
	QueryResult Owner() const; // defer implementation  { ESB_INVOKE_MEMB_FUNC0(esb::QueryResult, MethId::Owner); };
	// Функция ТипЗаписи() Возвращает ТипЗаписиЗапроса
	QueryRecordType RecordType() const { ESB_INVOKE_MEMB_FUNC0(esb::QueryRecordType, MethId::RecordType); };
	// Процедура Сбросить()
	//!> the method is actually a function according to disp-information.
	// Функция Выбрать([ТипОбхода_ Как ОбходРезультатаЗапроса], [Группировки_ Как Строка], [ГруппировкиДляЗначенийГруппировок_ Как Строка]) Возвращает ВыборкаИзРезультатаЗапроса
	QueryResultSelection Select(ConstParam<QueryResultIteration> TabOrderType_ = DefUndef<esb::QueryResultIteration>, ConstParam<String> Groups_ = DefUndef<esb::String>, ConstParam<String> GroupsForGroupValues_ = DefUndef<esb::String>) const { ESB_INVOKE_MEMB_FUNC3(esb::QueryResultSelection, MethId::Select, TabOrderType_, Groups_, GroupsForGroupValues_); };
public:
	ESB_IMPLEMENT_DYNAMIC_COLLECTION_RO_NAME_EN(FieldName)
#endif	// #if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
public:
	// Not Creatable
public:
	// Function Count() -> Number
	Число Количество() const { ESB_INVOKE_MEMB_FUNC0(esb::Число, MethId::Count); };
	// Function FindNext(Structure TheStructureOfTheSearch_) -> Boolean
	Булево НайтиСледующий(КонстПарам<Структура> СтруктураПоиска_) { ESB_INVOKE_MEMB_FUNC2(esb::Булево, MethId::FindNext, СтруктураПоиска_, ESB_DEFARG(Неопределено)); };
	// Function FindNext(Structure TheStructureOfTheSearch_) -> Boolean
	Булево НайтиСледующий(КонстПарам<Произвольный> Значение_, КонстПарам<Строка> Колонки_ = ПоУмолчаниюНеопределеноИли<esb::Строка>) { ESB_INVOKE_MEMB_FUNC2(esb::Булево, MethId::FindNext, Значение_, Колонки_); };
	// Function Get(Number Index_) -> Arbitrary
	Произвольный Получить(КонстПарам<Число> Индекс_) const { ESB_INVOKE_MEMB_FUNC1(esb::Произвольный, MethId::Get, Индекс_); };
	// Function Group() -> String
	Строка Группировка() const { ESB_INVOKE_MEMB_FUNC0(esb::Строка, MethId::Group); };
	// Function Level() -> Number
	Число Уровень() const { ESB_INVOKE_MEMB_FUNC0(esb::Число, MethId::Level); };
	// Function Next() -> Boolean
	Булево Следующий() { ESB_INVOKE_MEMB_FUNC0(esb::Булево, MethId::Next); };
	// Function NextByFieldValue(String FieldName_) -> Boolean
	Булево СледующийПоЗначениюПоля(КонстПарам<Строка> ИмяПоля_) { ESB_INVOKE_MEMB_FUNC1(esb::Булево, MethId::NextByFieldValue, ИмяПоля_); };
	// Function Owner() -> QueryResult
	РезультатЗапроса Владелец() const; // defer implementation  { ESB_INVOKE_MEMB_FUNC0(esb::РезультатЗапроса, MethId::Owner); };
	// Function RecordType() -> QueryRecordType
	ТипЗаписиЗапроса ТипЗаписи() const { ESB_INVOKE_MEMB_FUNC0(esb::ТипЗаписиЗапроса, MethId::RecordType); };
	// Void Reset()
	//!> the method is actually a function according to disp-information.
	// Function Select(QueryResultIteration TabOrderType_ = <default>, String Groups_ = <default>, String GroupsForGroupValues_ = <default>) -> QueryResultSelection
	ВыборкаИзРезультатаЗапроса Выбрать(КонстПарам<ОбходРезультатаЗапроса> ТипОбхода_ = ПоУмолчаниюНеопределеноИли<esb::ОбходРезультатаЗапроса>, КонстПарам<Строка> Группировки_ = ПоУмолчаниюНеопределеноИли<esb::Строка>, КонстПарам<Строка> ГруппировкиДляЗначенийГруппировок_ = ПоУмолчаниюНеопределеноИли<esb::Строка>) const { ESB_INVOKE_MEMB_FUNC3(esb::ВыборкаИзРезультатаЗапроса, MethId::Select, ТипОбхода_, Группировки_, ГруппировкиДляЗначенийГруппировок_); };
public:
	ESB_IMPLEMENT_DYNAMIC_COLLECTION_RO_NAME_RU(ИмяПоля)
#endif	// #if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
public:
	ESB_IMPLEMENT_INDEXED_COLLECTION_RO()
public:
	// MANUAL!!! ESB_IMPLEMENT_ASSICIATIVE_COLLECTION()
};


ESB_CLASS_OBJECT(QueryResult)
{
	ESB_CLASS_IMPLEMENT_OBJECT(QueryResult)
#if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
public:
	// Not Creatable
public:
	// Функция Пустой() Возвращает Булево
	Boolean IsEmpty() const { ESB_INVOKE_MEMB_FUNC0(esb::Boolean, MethId::IsEmpty); };
	// Функция Выбрать([ТипОбхода_ Как ОбходРезультатаЗапроса], [Группировки_ Как Строка], [ГруппировкиДляЗначенийГруппировок_ Как Строка]) Возвращает ВыборкаИзРезультатаЗапроса
	QueryResultSelection Select(ConstParam<QueryResultIteration> TabOrderType_ = QueryResultIteration::Linear, ConstParam<String> Groups_ = DefUndef<esb::String>, ConstParam<String> GroupsForGroupValues_ = DefUndef<esb::String>) const { ESB_INVOKE_MEMB_FUNC3(esb::QueryResultSelection, MethId::Select, TabOrderType_, Groups_, GroupsForGroupValues_); };
	// Функция Выгрузить([ТипОбхода_ Как ОбходРезультатаЗапроса]) Возвращает ОдинИз<ТаблицаЗначений, ДеревоЗначений>
	OneOf<ValueTable, ValueTree> Unload(ConstParam<QueryResultIteration> TabOrderType_ = DefUndef<esb::QueryResultIteration>) const { ESB_INVOKE_MEMB_FUNC1(ESB_ONEOF(esb::ValueTable, esb::ValueTree), MethId::Unload, TabOrderType_); };
public:
	// Свойство [[ТолькоЧтение]] Колонки Как КоллекцияКолонокРезультатаЗапроса
	QueryResultColumnsCollection GetColumns() const { ESB_INVOKE_PROPERTY_GET(esb::QueryResultColumnsCollection, PropId::Columns); };
	ESB_PROPERTY_FIELD_RO(QueryResultColumnsCollection, Columns)
#endif	// #if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
public:
	// Not Creatable
public:
	// Function IsEmpty() -> Boolean
	Булево Пустой() const { ESB_INVOKE_MEMB_FUNC0(esb::Булево, MethId::IsEmpty); };
	// Function Select(QueryResultIteration TabOrderType_ = <default>, String Groups_ = <default>, String GroupsForGroupValues_ = <default>) -> QueryResultSelection
	ВыборкаИзРезультатаЗапроса Выбрать(КонстПарам<ОбходРезультатаЗапроса> ТипОбхода_ = ПоУмолчаниюНеопределеноИли<esb::ОбходРезультатаЗапроса>, КонстПарам<Строка> Группировки_ = ПоУмолчаниюНеопределеноИли<esb::Строка>, КонстПарам<Строка> ГруппировкиДляЗначенийГруппировок_ = ПоУмолчаниюНеопределеноИли<esb::Строка>) const { ESB_INVOKE_MEMB_FUNC3(esb::ВыборкаИзРезультатаЗапроса, MethId::Select, ТипОбхода_, Группировки_, ГруппировкиДляЗначенийГруппировок_); };
	// Function Unload(QueryResultIteration TabOrderType_ = <default>) -> OneOf<ValueTable, ValueTree>
	ОдинИз<ТаблицаЗначений, ДеревоЗначений> Выгрузить(КонстПарам<ОбходРезультатаЗапроса> ТипОбхода_ = ПоУмолчаниюНеопределеноИли<esb::ОбходРезультатаЗапроса>) const { ESB_INVOKE_MEMB_FUNC1(ESB_ONEOF(esb::ТаблицаЗначений, esb::ДеревоЗначений), MethId::Unload, ТипОбхода_); };
public:
	// Property [[ReadOnly]] Columns -> QueryResultColumnsCollection
	КоллекцияКолонокРезультатаЗапроса ПолучитьКолонки() const { ESB_INVOKE_PROPERTY_GET(esb::КоллекцияКолонокРезультатаЗапроса, PropId::Columns); };
	ESB_PROPERTY_FIELD_RO(КоллекцияКолонокРезультатаЗапроса, Колонки)
#endif	// #if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
};


ESB_CLASS_OBJECT(QueryTempTableColumn)
{
	ESB_CLASS_IMPLEMENT_OBJECT(QueryTempTableColumn)
#if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
public:
	// Not Creatable
public:
	// Свойство [[ТолькоЧтение]] Имя Как Строка
	String GetName() const { ESB_INVOKE_PROPERTY_GET(esb::String, PropId::Name); };
	ESB_PROPERTY_FIELD_RO(String, Name)
		// Свойство [[ТолькоЧтение]] ТипЗначения Как ОписаниеТипов
		TypeDescription GetValueType() const { ESB_INVOKE_PROPERTY_GET(esb::TypeDescription, PropId::ValueType); };
	ESB_PROPERTY_FIELD_RO(TypeDescription, ValueType)
#endif	// #if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
public:
	// Not Creatable
public:
	// Property [[ReadOnly]] Name -> String
	Строка ПолучитьИмя() const { ESB_INVOKE_PROPERTY_GET(esb::Строка, PropId::Name); };
	ESB_PROPERTY_FIELD_RO(Строка, Имя)
		// Property [[ReadOnly]] ValueType -> TypeDescription
		ОписаниеТипов ПолучитьТипЗначения() const { ESB_INVOKE_PROPERTY_GET(esb::ОписаниеТипов, PropId::ValueType); };
	ESB_PROPERTY_FIELD_RO(ОписаниеТипов, ТипЗначения)
#endif	// #if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
};


ESB_CLASS_OBJECT_DUAL2_WITH_DYNAMIC_PROPERTIES_RO(QueryTempTableColumns, QueryTempTableColumn, IxCollectionRO<QueryTempTableColumn>, AxCollectionRO<Arbitrary, QueryTempTableColumn>)
{
	ESB_CLASS_IMPLEMENT_OBJECT_DUAL2_WITH_DYNAMIC_PROPERTIES(QueryTempTableColumns)
#if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
public:
	// Not Creatable
public:
	// Функция Количество() Возвращает Число
	Number Count() const { ESB_INVOKE_MEMB_FUNC0(esb::Number, MethId::Count); };
	// Функция Найти(НаименованиеКолонки_ Как Строка) Возвращает ОдинИз<КолонкаВременнойТаблицыЗапроса, Неопределено>
	UndefOr<QueryTempTableColumn> Find(ConstParam<String> ColumnDescription_) const { ESB_INVOKE_MEMB_FUNC1(UndefOr<esb::QueryTempTableColumn>, MethId::Find, ColumnDescription_); };
	// Функция Получить(Индекс_ Как Число) Возвращает КолонкаВременнойТаблицыЗапроса
	QueryTempTableColumn Get(ConstParam<Number> Index_) const { ESB_INVOKE_MEMB_FUNC1(esb::QueryTempTableColumn, MethId::Get, Index_); };
	// Функция Индекс(Колонка_ Как КолонкаВременнойТаблицыЗапроса) Возвращает Число
	Number IndexOf(ConstParam<QueryTempTableColumn> Column_) const { ESB_INVOKE_MEMB_FUNC1(esb::Number, MethId::IndexOf, Column_); };
public:
	ESB_IMPLEMENT_DYNAMIC_COLLECTION_RO_NAME_EN(ColumnName)
#endif	// #if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
public:
	// Not Creatable
public:
	// Function Count() -> Number
	Число Количество() const { ESB_INVOKE_MEMB_FUNC0(esb::Число, MethId::Count); };
	// Function Find(String ColumnDescription_) -> OneOf<QueryTempTableColumn, Undefined>
	НеопределеноИли<КолонкаВременнойТаблицыЗапроса> Найти(КонстПарам<Строка> НаименованиеКолонки_) const { ESB_INVOKE_MEMB_FUNC1(НеопределеноИли<esb::КолонкаВременнойТаблицыЗапроса>, MethId::Find, НаименованиеКолонки_); };
	// Function Get(Number Index_) -> QueryTempTableColumn
	КолонкаВременнойТаблицыЗапроса Получить(КонстПарам<Число> Индекс_) const { ESB_INVOKE_MEMB_FUNC1(esb::КолонкаВременнойТаблицыЗапроса, MethId::Get, Индекс_); };
	// Function IndexOf(QueryTempTableColumn Column_) -> Number
	Число Индекс(КонстПарам<КолонкаВременнойТаблицыЗапроса> Колонка_) const { ESB_INVOKE_MEMB_FUNC1(esb::Число, MethId::IndexOf, Колонка_); };
public:
	ESB_IMPLEMENT_DYNAMIC_COLLECTION_RO_NAME_RU(ИмяКолонки)
#endif	// #if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
public:
	ESB_IMPLEMENT_ENUM_ITEMS(QueryTempTableColumns, QueryTempTableColumn)
public:
	ESB_IMPLEMENT_INDEXED_COLLECTION_RO()
public:
	// MANUAL!!! ESB_IMPLEMENT_ASSICIATIVE_COLLECTION()
};


ESB_CLASS_OBJECT(QueryTempTable)
{
	ESB_CLASS_IMPLEMENT_OBJECT(QueryTempTable)
#if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
public:
	// Not Creatable
public:
	// Функция ПолучитьДанные() Возвращает РезультатЗапроса
	QueryResult GetData() const { ESB_INVOKE_MEMB_FUNC0(esb::QueryResult, MethId::GetData); };
public:
	// Свойство [[ТолькоЧтение]] Колонки Как КолонкиВременнойТаблицыЗапроса
	QueryTempTableColumns GetColumns() const { ESB_INVOKE_PROPERTY_GET(esb::QueryTempTableColumns, PropId::Columns); };
	ESB_PROPERTY_FIELD_RO(QueryTempTableColumns, Columns)
		// Свойство [[ТолькоЧтение]] ПолноеИмя Как Строка
		String GetFullName() const { ESB_INVOKE_PROPERTY_GET(esb::String, PropId::FullName); };
	ESB_PROPERTY_FIELD_RO(String, FullName)
#endif	// #if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
public:
	// Not Creatable
public:
	// Function GetData() -> QueryResult
	РезультатЗапроса ПолучитьДанные() const { ESB_INVOKE_MEMB_FUNC0(esb::РезультатЗапроса, MethId::GetData); };
public:
	// Property [[ReadOnly]] Columns -> QueryTempTableColumns
	КолонкиВременнойТаблицыЗапроса ПолучитьКолонки() const { ESB_INVOKE_PROPERTY_GET(esb::КолонкиВременнойТаблицыЗапроса, PropId::Columns); };
	ESB_PROPERTY_FIELD_RO(КолонкиВременнойТаблицыЗапроса, Колонки)
		// Property [[ReadOnly]] FullName -> String
		Строка ПолучитьПолноеИмя() const { ESB_INVOKE_PROPERTY_GET(esb::Строка, PropId::FullName); };
	ESB_PROPERTY_FIELD_RO(Строка, ПолноеИмя)
#endif	// #if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
};


ESB_CLASS_OBJECT_DUAL2(QueryTempTables, IxCollectionRO<QueryTempTable>, AxCollectionRO<Arbitrary, QueryTempTable>)
{
	ESB_CLASS_IMPLEMENT_OBJECT_DUAL2(QueryTempTables)
#if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
public:
	// Not Creatable
public:
	// Функция Количество() Возвращает Число
	Number Count() const { ESB_INVOKE_MEMB_FUNC0(esb::Number, MethId::Count); };
	// Функция Найти(НаименованиеТаблицы_ Как Строка) Возвращает ОдинИз<ВременнаяТаблицаЗапроса, Неопределено>
	UndefOr<QueryTempTable> Find(ConstParam<String> ColumnDescription_) const { ESB_INVOKE_MEMB_FUNC1(UndefOr<esb::QueryTempTable>, MethId::Find, ColumnDescription_); };
	// Функция Получить(Индекс_ Как Число) Возвращает ВременнаяТаблицаЗапроса
	QueryTempTable Get(ConstParam<Number> Index_) const { ESB_INVOKE_MEMB_FUNC1(esb::QueryTempTable, MethId::Get, Index_); };
	// Функция Индекс(ВременнаяТаблица_ Как ВременнаяТаблицаЗапроса) Возвращает Число
	Number IndexOf(ConstParam<QueryTempTable> TempTable_) const { ESB_INVOKE_MEMB_FUNC1(esb::Number, MethId::IndexOf, TempTable_); };
#endif	// #if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
public:
	// Not Creatable
public:
	// Function Count() -> Number
	Число Количество() const { ESB_INVOKE_MEMB_FUNC0(esb::Число, MethId::Count); };
	// Function Find(String ColumnDescription_) -> OneOf<QueryTempTable, Undefined>
	НеопределеноИли<ВременнаяТаблицаЗапроса> Найти(КонстПарам<Строка> НаименованиеТаблицы_) const { ESB_INVOKE_MEMB_FUNC1(НеопределеноИли<esb::ВременнаяТаблицаЗапроса>, MethId::Find, НаименованиеТаблицы_); };
	// Function Get(Number Index_) -> QueryTempTable
	ВременнаяТаблицаЗапроса Получить(КонстПарам<Число> Индекс_) const { ESB_INVOKE_MEMB_FUNC1(esb::ВременнаяТаблицаЗапроса, MethId::Get, Индекс_); };
	// Function IndexOf(QueryTempTable TempTable_) -> Number
	Число Индекс(КонстПарам<ВременнаяТаблицаЗапроса> ВременнаяТаблица_) const { ESB_INVOKE_MEMB_FUNC1(esb::Число, MethId::IndexOf, ВременнаяТаблица_); };
#endif	// #if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
public:
	ESB_IMPLEMENT_ENUM_ITEMS(QueryTempTables, QueryTempTable)
public:
	ESB_IMPLEMENT_INDEXED_COLLECTION_RO()
public:
	// MANUAL!!! ESB_IMPLEMENT_ASSICIATIVE_COLLECTION()
};


ESB_CLASS_OBJECT(TempTablesManager)
{
	ESB_CLASS_IMPLEMENT_OBJECT(TempTablesManager)
#if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
public:
	static TempTablesManager Create() { ESB_INVOKE_CTOR0(TempTablesManager); }
public:
	// Процедура Закрыть()
	void Close() { ESB_INVOKE_MEMB_PROC0(MethId::Close); };
public:
	// Свойство [[ТолькоЧтение]] Таблицы Как ВременныеТаблицыЗапроса
	QueryTempTables GetTables() const { ESB_INVOKE_PROPERTY_GET(esb::QueryTempTables, PropId::Tables); };
	ESB_PROPERTY_FIELD_RO(QueryTempTables, Tables)
#endif	// #if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
public:
	static МенеджерВременныхТаблиц Создать() { ESB_INVOKE_CTOR0(МенеджерВременныхТаблиц); }
public:
	// Void Close()
	void Закрыть() { ESB_INVOKE_MEMB_PROC0(MethId::Close); };
public:
	// Property [[ReadOnly]] Tables -> QueryTempTables
	ВременныеТаблицыЗапроса ПолучитьТаблицы() const { ESB_INVOKE_PROPERTY_GET(esb::ВременныеТаблицыЗапроса, PropId::Tables); };
	ESB_PROPERTY_FIELD_RO(ВременныеТаблицыЗапроса, Таблицы)
#endif	// #if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
};


ESB_CLASS_OBJECT(Query)
{
	ESB_CLASS_IMPLEMENT_OBJECT(Query)
#if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
public:
	static Query Create() { ESB_INVOKE_CTOR0(Query); }  // no information about this constructor, but present in disp-info.
	static Query Create(ConstParam<String> RequestText_) { ESB_INVOKE_CTOR1(Query, RequestText_); }
public:
	// Функция Выполнить() Возвращает ОдинИз<РезультатЗапроса, Неопределено>
	UndefOr<QueryResult> Execute() const { ESB_INVOKE_MEMB_FUNC0(UndefOr<esb::QueryResult>, MethId::Execute); };
	// Функция ВыполнитьПакет() Возвращает МассивИз<РезультатЗапроса>
	ArrayOf<QueryResult> ExecuteBatch() const { ESB_INVOKE_MEMB_FUNC0(esb::ArrayOf<QueryResult>, MethId::ExecuteBatch); };
	// Функция ВыполнитьПакетСПромежуточнымиДанными() Возвращает МассивИз<РезультатЗапроса>
	ArrayOf<QueryResult> ExecuteBatchWithIntermediateData() const { ESB_INVOKE_MEMB_FUNC0(esb::ArrayOf<QueryResult>, MethId::ExecuteBatchWithIntermediateData); };
	// Функция НайтиПараметры() Возвращает ОписаниеПараметровЗапроса
	QueryParametersDescription FindParameters() const { ESB_INVOKE_MEMB_FUNC0(esb::QueryParametersDescription, MethId::FindParameters); };
	// Процедура УстановитьПараметр(Имя_ Как Строка, Значение_ Как Произвольный)
	void SetParameter(ConstParam<String> Name_, ConstParam<Arbitrary> Value_) { ESB_INVOKE_MEMB_PROC2(MethId::SetParameter, Name_, Value_); };
public:
	// Свойство [[ТолькоЧтение]] Параметры Как Структура
	Structure GetParameters() const { ESB_INVOKE_PROPERTY_GET(esb::Structure, PropId::Parameters); };
	ESB_PROPERTY_FIELD_RO(Structure, Parameters)
		// Свойство [[Чтение,Запись]] МенеджерВременныхТаблиц Как ОдинИз<МенеджерВременныхТаблиц, Неопределено>
		UndefOr<TempTablesManager> GetTempTablesManager() const { ESB_INVOKE_PROPERTY_GET(UndefOr<esb::TempTablesManager>, PropId::TempTablesManager); };
	void SetTempTablesManager(ConstParam<UndefOr<TempTablesManager>> Value_) { ESB_INVOKE_PROPERTY_SET(PropId::TempTablesManager, Value_); };
	ESB_PROPERTY_FIELD_RW(UndefOr<TempTablesManager>, TempTablesManager)
		// Свойство [[Чтение,Запись]] Текст Как Строка
		String GetText() const { ESB_INVOKE_PROPERTY_GET(esb::String, PropId::Text); };
	void SetText(ConstParam<String> Value_) { ESB_INVOKE_PROPERTY_SET(PropId::Text, Value_); };
	ESB_PROPERTY_FIELD_RW(String, Text)
#endif	// #if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
public:
	static Запрос Создать() { ESB_INVOKE_CTOR0(Запрос); }  // no information about this constructor, but present in disp-info.
	static Запрос Создать(КонстПарам<Строка> ТекстЗапроса_ = ПоУмолчаниюНеопределеноИли<esb::Строка>) { ESB_INVOKE_CTOR1(Запрос, ТекстЗапроса_); }
public:
	// Function Execute() -> OneOf<QueryResult, Undefined>
	НеопределеноИли<РезультатЗапроса> Выполнить() const { ESB_INVOKE_MEMB_FUNC0(НеопределеноИли<esb::РезультатЗапроса>, MethId::Execute); };
	// Function ExecuteBatch() -> ArrayOf<QueryResult>
	МассивИз<РезультатЗапроса> ВыполнитьПакет() const { ESB_INVOKE_MEMB_FUNC0(esb::МассивИз<РезультатЗапроса>, MethId::ExecuteBatch); };
	// Function ExecuteBatchWithIntermediateData() -> ArrayOf<QueryResult>
	МассивИз<РезультатЗапроса> ВыполнитьПакетСПромежуточнымиДанными() const { ESB_INVOKE_MEMB_FUNC0(esb::МассивИз<РезультатЗапроса>, MethId::ExecuteBatchWithIntermediateData); };
	// Function FindParameters() -> QueryParametersDescription
	ОписаниеПараметровЗапроса НайтиПараметры() const { ESB_INVOKE_MEMB_FUNC0(esb::ОписаниеПараметровЗапроса, MethId::FindParameters); };
	// Void SetParameter(String Name_, Arbitrary Value_)
	void УстановитьПараметр(КонстПарам<Строка> Имя_, КонстПарам<Произвольный> Значение_) { ESB_INVOKE_MEMB_PROC2(MethId::SetParameter, Имя_, Значение_); };
public:
	// Property [[ReadOnly]] Parameters -> Structure
	Структура ПолучитьПараметры() const { ESB_INVOKE_PROPERTY_GET(esb::Структура, PropId::Parameters); };
	ESB_PROPERTY_FIELD_RO(Структура, Параметры)
		// Property [[Read,Write]] TempTablesManager -> OneOf<TempTablesManager, Undefined>
		НеопределеноИли<МенеджерВременныхТаблиц> ПолучитьМенеджерВременныхТаблиц() const { ESB_INVOKE_PROPERTY_GET(НеопределеноИли<esb::МенеджерВременныхТаблиц>, PropId::TempTablesManager); };
	void УстановитьМенеджерВременныхТаблиц(КонстПарам<НеопределеноИли<МенеджерВременныхТаблиц>> Значение_) { ESB_INVOKE_PROPERTY_SET(PropId::TempTablesManager, Значение_); };
	ESB_PROPERTY_FIELD_RW(НеопределеноИли<МенеджерВременныхТаблиц>, МенеджерВременныхТаблиц)
		// Property [[Read,Write]] Text -> String
		Строка ПолучитьТекст() const { ESB_INVOKE_PROPERTY_GET(esb::Строка, PropId::Text); };
	void УстановитьТекст(КонстПарам<Строка> Значение_) { ESB_INVOKE_PROPERTY_SET(PropId::Text, Значение_); };
	ESB_PROPERTY_FIELD_RW(Строка, Текст)
#endif	// #if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
};


ESB_CLASS_OBJECT(ValueListItem)
{
	ESB_CLASS_IMPLEMENT_OBJECT(ValueListItem)
#if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
public:
	// Not Creatable
public:
	// Функция ПолучитьИдентификатор() Возвращает Число
	Number GetID() const { ESB_INVOKE_MEMB_FUNC0(esb::Number, MethId::GetID); };
public:
	// Свойство [[Чтение,Запись]] Пометка Как Булево
	Boolean GetCheck() const { ESB_INVOKE_PROPERTY_GET(esb::Boolean, PropId::Check); };
	void SetCheck(ConstParam<Boolean> Value_) { ESB_INVOKE_PROPERTY_SET(PropId::Check, Value_); };
	ESB_PROPERTY_FIELD_RW(Boolean, Check)
		// Свойство [[Чтение,Запись]] Картинка Как Картинка
		Picture GetPicture() const { ESB_INVOKE_PROPERTY_GET(esb::Picture, PropId::Picture); };
	void SetPicture(ConstParam<Picture> Value_) { ESB_INVOKE_PROPERTY_SET(PropId::Picture, Value_); };
	ESB_PROPERTY_FIELD_RW(Picture, Picture)
		// Свойство [[Чтение,Запись]] Представление Как ОдинИз<ФорматированнаяСтрока, Строка>
		OneOf<String, FormattedString> GetPresentation() const { ESB_INVOKE_PROPERTY_GET(ESB_ONEOF(esb::String, esb::FormattedString), PropId::Presentation); };
	void SetPresentation(ConstParam<String> Value_) { ESB_INVOKE_PROPERTY_SET(PropId::Presentation, Value_); };
	void SetPresentation(ConstParam<FormattedString> Value_) { ESB_INVOKE_PROPERTY_SET(PropId::Presentation, Value_); };
	// Свойство [[Чтение,Запись]] Значение Как Произвольный
	Arbitrary GetValue() const { ESB_INVOKE_PROPERTY_GET(esb::Arbitrary, PropId::Value); };
	void SetValue(ConstParam<Arbitrary> Value_) { ESB_INVOKE_PROPERTY_SET(PropId::Value, Value_); };
	ESB_PROPERTY_FIELD_RW(Arbitrary, Value)
#endif	// #if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
public:
	// Not Creatable
public:
	// Function GetID() -> Number
	Число ПолучитьИдентификатор() const { ESB_INVOKE_MEMB_FUNC0(esb::Число, MethId::GetID); };
public:
	// Property [[Read,Write]] Check -> Boolean
	Булево ПолучитьПометка() const { ESB_INVOKE_PROPERTY_GET(esb::Булево, PropId::Check); };
	void УстановитьПометка(КонстПарам<Булево> Значение_) { ESB_INVOKE_PROPERTY_SET(PropId::Check, Значение_); };
	ESB_PROPERTY_FIELD_RW(Булево, Пометка)
		// Property [[Read,Write]] Picture -> Picture
		Картинка ПолучитьКартинка() const { ESB_INVOKE_PROPERTY_GET(esb::Картинка, PropId::Picture); };
	void УстановитьКартинка(КонстПарам<Картинка> Значение_) { ESB_INVOKE_PROPERTY_SET(PropId::Picture, Значение_); };
	ESB_PROPERTY_FIELD_RW(Картинка, Картинка)
		// Property [[Read,Write]] Presentation -> OneOf<FormattedString, String>
		ОдинИз<Строка, ФорматированнаяСтрока> ПолучитьПредставление() const { ESB_INVOKE_PROPERTY_GET(ESB_ONEOF(esb::Строка, esb::ФорматированнаяСтрока), PropId::Presentation); };
	void УстановитьПредставление(КонстПарам<Строка> Значение_) { ESB_INVOKE_PROPERTY_SET(PropId::Presentation, Значение_); };
	void УстановитьПредставление(КонстПарам<ФорматированнаяСтрока> Значение_) { ESB_INVOKE_PROPERTY_SET(PropId::Presentation, Значение_); };
	// Property [[Read,Write]] Value -> Arbitrary
	Произвольный ПолучитьЗначение() const { ESB_INVOKE_PROPERTY_GET(esb::Произвольный, PropId::Value); };
	void УстановитьЗначение(КонстПарам<Произвольный> Значение_) { ESB_INVOKE_PROPERTY_SET(PropId::Value, Значение_); };
	ESB_PROPERTY_FIELD_RW(Произвольный, Значение)
#endif	// #if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
};


ESB_CLASS_OBJECT_DUAL(ValueList, IxCollectionRO<ValueListItem>)
{
	ESB_CLASS_IMPLEMENT_OBJECT_DUAL(ValueList)
#if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
public:
	static ValueList Create() { ESB_INVOKE_CTOR0(ValueList); }
public:
	// Функция Добавить([Значение_ Как Произвольный], [Представление_ Как ОдинИз<ФорматированнаяСтрока, Строка>], [Пометка_ Как Булево], [Картинка_ Как Картинка]) Возвращает ЭлементСпискаЗначений
	ValueListItem Add(ConstParam<Arbitrary> Value_ = DefUndef<esb::Arbitrary>, ConstParam<String> Representation_ = DefUndef<esb::String>, ConstParam<Boolean> CheckMark_ = DefUndef<esb::Boolean>, ConstParam<Picture> Picture_ = DefUndef<esb::Picture>) { ESB_INVOKE_MEMB_FUNC4(esb::ValueListItem, MethId::Add, Value_, Representation_, CheckMark_, Picture_); };
	ValueListItem Add(ConstParam<Arbitrary> Value_ = DefUndef<esb::Arbitrary>, ConstParam<FormattedString> Representation_ = DefUndef<esb::FormattedString>, ConstParam<Boolean> CheckMark_ = DefUndef<esb::Boolean>, ConstParam<Picture> Picture_ = DefUndef<esb::Picture>) { ESB_INVOKE_MEMB_FUNC4(esb::ValueListItem, MethId::Add, Value_, Representation_, CheckMark_, Picture_); };
	// Функция ОтметитьЭлементы([Заголовок_ Как Строка]) Возвращает Булево
	Boolean CheckItems(ConstParam<String> Title_ = DefUndef<esb::String>) { ESB_INVOKE_MEMB_FUNC1(esb::Boolean, MethId::CheckItems, Title_); };
	// Функция ВыбратьЭлемент([Заголовок_ Как Строка], [Элемент_ Как ЭлементСпискаЗначений]) Возвращает ОдинИз<Неопределено, ЭлементСпискаЗначений>
	UndefOr<ValueListItem> ChooseItem(ConstParam<String> Title_ = DefUndef<esb::String>, ConstParam<ValueListItem> Item_ = DefUndef<esb::ValueListItem>) const { ESB_INVOKE_MEMB_FUNC2(UndefOr<esb::ValueListItem>, MethId::ChooseItem, Title_, Item_); };
	// Процедура Очистить()
	void Clear() { ESB_INVOKE_MEMB_PROC0(MethId::Clear); };
	// Функция Скопировать() Возвращает СписокЗначений
	ValueList Copy() const { ESB_INVOKE_MEMB_FUNC0(esb::ValueList, MethId::Copy); };
	// Функция Количество() Возвращает Число
	Number Count() const { ESB_INVOKE_MEMB_FUNC0(esb::Number, MethId::Count); };
	// Процедура Удалить(Элемент_ Как ОдинИз<Число, ЭлементСпискаЗначений>)
	void Delete(ConstParam<Number> Item_) { ESB_INVOKE_MEMB_PROC1(MethId::Delete, Item_); };
	void Delete(ConstParam<ValueListItem> Item_) { ESB_INVOKE_MEMB_PROC1(MethId::Delete, Item_); };
	// Процедура ЗаполнитьПометки(ЗначениеПометки_ Как Булево)
	void FillChecks(ConstParam<Boolean> CheckMarkValue_) { ESB_INVOKE_MEMB_PROC1(MethId::FillChecks, CheckMarkValue_); };
	// Функция НайтиПоИдентификатору(Идентификатор_ Как Число) Возвращает ОдинИз<Неопределено, ЭлементСпискаЗначений>
	UndefOr<ValueListItem> FindByID(ConstParam<Number> ID_) const { ESB_INVOKE_MEMB_FUNC1(UndefOr<esb::ValueListItem>, MethId::FindByID, ID_); };
	// Функция НайтиПоЗначению(ИскомоеЗначение_ Как Произвольный) Возвращает ОдинИз<Неопределено, ЭлементСпискаЗначений>
	UndefOr<ValueListItem> FindByValue(ConstParam<Arbitrary> SoughtValue_) const { ESB_INVOKE_MEMB_FUNC1(UndefOr<esb::ValueListItem>, MethId::FindByValue, SoughtValue_); };
	// Функция Получить(Индекс_ Как Число) Возвращает ЭлементСпискаЗначений
	ValueListItem Get(ConstParam<Number> Index_) { ESB_INVOKE_MEMB_FUNC1(esb::ValueListItem, MethId::Get, Index_); };
	// Функция Индекс(Элемент_ Как ЭлементСпискаЗначений) Возвращает Число
	Number IndexOf(ConstParam<ValueListItem> Item_) const { ESB_INVOKE_MEMB_FUNC1(esb::Number, MethId::IndexOf, Item_); };
	// Функция Вставить(Индекс_ Как Число, [Значение_ Как Произвольный], [Представление_ Как ОдинИз<ФорматированнаяСтрока, Строка>], [Пометка_ Как Булево], [Картинка_ Как Картинка]) Возвращает ЭлементСпискаЗначений
	ValueListItem Insert(ConstParam<Number> Index_, ConstParam<Arbitrary> Value_ = DefUndef<esb::Arbitrary>, ConstParam<String> Representation_ = DefUndef<esb::String>, ConstParam<Boolean> CheckMark_ = DefUndef<esb::Boolean>, ConstParam<Picture> Picture_ = DefUndef<esb::Picture>) { ESB_INVOKE_MEMB_FUNC5(esb::ValueListItem, MethId::Insert, Index_, Value_, Representation_, CheckMark_, Picture_); };
	ValueListItem Insert(ConstParam<Number> Index_, ConstParam<Arbitrary> Value_ = DefUndef<esb::Arbitrary>, ConstParam<FormattedString> Representation_ = DefUndef<esb::FormattedString>, ConstParam<Boolean> CheckMark_ = DefUndef<esb::Boolean>, ConstParam<Picture> Picture_ = DefUndef<esb::Picture>) { ESB_INVOKE_MEMB_FUNC5(esb::ValueListItem, MethId::Insert, Index_, Value_, Representation_, CheckMark_, Picture_); };
	// Процедура ЗагрузитьЗначения(МассивЗначений_ Как Массив)
	void LoadValues(ConstParam<Array> ArrayOfValues_) { ESB_INVOKE_MEMB_PROC1(MethId::LoadValues, ArrayOfValues_); };
	// Процедура Сдвинуть(Элемент_ Как ОдинИз<Число, ЭлементСпискаЗначений>, Смещение_ Как Число)
	void Move(ConstParam<Number> Item_, ConstParam<Number> Offset_) { ESB_INVOKE_MEMB_PROC2(MethId::Move, Item_, Offset_); };
	void Move(ConstParam<ValueListItem> Item_, ConstParam<Number> Offset_) { ESB_INVOKE_MEMB_PROC2(MethId::Move, Item_, Offset_); };
	// Процедура ПоказатьОтметкуЭлементов(ОписаниеОповещенияОЗакрытии_ Как ОписаниеОповещения, [Заголовок_ Как Строка])
	void ShowCheckItems(ConstParam<NotifyDescription> NotifyOnCloseDescription_, ConstParam<String> Title_ = DefUndef<esb::String>) { ESB_INVOKE_MEMB_PROC2(MethId::ShowCheckItems, NotifyOnCloseDescription_, Title_); };
	// Процедура ПоказатьВыборЭлемента(ОписаниеОповещенияОЗакрытии_ Как ОписаниеОповещения, [Заголовок_ Как Строка], [Элемент_ Как ЭлементСпискаЗначений])
	void ShowChooseItem(ConstParam<NotifyDescription> NotifyOnCloseDescription_, ConstParam<String> Title_ = DefUndef<esb::String>, ConstParam<ValueListItem> Item_ = DefUndef<esb::ValueListItem>) const { ESB_INVOKE_MEMB_PROC3(MethId::ShowChooseItem, NotifyOnCloseDescription_, Title_, Item_); };
	// Процедура СортироватьПоПредставлению([Направление_ Как НаправлениеСортировки])
	void SortByPresentation(ConstParam<SortDirection> Direction_ = DefUndef<esb::SortDirection>) { ESB_INVOKE_MEMB_PROC1(MethId::SortByPresentation, Direction_); };
	// Процедура СортироватьПоЗначению([Направление_ Как НаправлениеСортировки])
	void SortByValue(ConstParam<SortDirection> Direction_ = DefUndef<esb::SortDirection>) { ESB_INVOKE_MEMB_PROC1(MethId::SortByValue, Direction_); };
	// Функция ВыгрузитьЗначения() Возвращает Массив
	Array UnloadValues() const { ESB_INVOKE_MEMB_FUNC0(esb::Array, MethId::UnloadValues); };
public:
	// Свойство [[Чтение,Запись]] ДоступныеЗначения Как ОдинИз<Неопределено, СписокЗначений>
	UndefOr<ValueList> GetAvailableValues() const { ESB_INVOKE_PROPERTY_GET(UndefOr<esb::ValueList>, PropId::AvailableValues); };
	void SetAvailableValues(ConstParam<UndefOr<ValueList>> Value_) { ESB_INVOKE_PROPERTY_SET(PropId::AvailableValues, Value_); };
	ESB_PROPERTY_FIELD_RW(UndefOr<ValueList>, AvailableValues)
		// Свойство [[Чтение,Запись]] ТипЗначения Как ОписаниеТипов
		TypeDescription GetValueType() const { ESB_INVOKE_PROPERTY_GET(esb::TypeDescription, PropId::ValueType); };
	void SetValueType(ConstParam<TypeDescription> Value_) { ESB_INVOKE_PROPERTY_SET(PropId::ValueType, Value_); };
	ESB_PROPERTY_FIELD_RW(TypeDescription, ValueType)
#endif	// #if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
public:
	static СписокЗначений Создать() { ESB_INVOKE_CTOR0(СписокЗначений); }
public:
	// Function Add(Arbitrary Value_ = <default>, OneOf<FormattedString, String> Representation_ = <default>, Boolean CheckMark_ = <default>, Picture Picture_ = <default>) -> ValueListItem
	ЭлементСпискаЗначений Добавить(КонстПарам<Произвольный> Значение_ = ПоУмолчаниюНеопределеноИли<esb::Произвольный>, КонстПарам<Строка> Представление_ = ПоУмолчаниюНеопределеноИли<esb::Строка>, КонстПарам<Булево> Пометка_ = ПоУмолчаниюНеопределеноИли<esb::Булево>, КонстПарам<Картинка> Картинка_ = ПоУмолчаниюНеопределеноИли<esb::Картинка>) { ESB_INVOKE_MEMB_FUNC4(esb::ЭлементСпискаЗначений, MethId::Add, Значение_, Представление_, Пометка_, Картинка_); };
	ЭлементСпискаЗначений Добавить(КонстПарам<Произвольный> Значение_ = ПоУмолчаниюНеопределеноИли<esb::Произвольный>, КонстПарам<ФорматированнаяСтрока> Представление_ = ПоУмолчаниюНеопределеноИли<esb::ФорматированнаяСтрока>, КонстПарам<Булево> Пометка_ = ПоУмолчаниюНеопределеноИли<esb::Булево>, КонстПарам<Картинка> Картинка_ = ПоУмолчаниюНеопределеноИли<esb::Картинка>) { ESB_INVOKE_MEMB_FUNC4(esb::ЭлементСпискаЗначений, MethId::Add, Значение_, Представление_, Пометка_, Картинка_); };
	// Function CheckItems(String Title_ = <default>) -> Boolean
	Булево ОтметитьЭлементы(КонстПарам<Строка> Заголовок_ = ПоУмолчаниюНеопределеноИли<esb::Строка>) { ESB_INVOKE_MEMB_FUNC1(esb::Булево, MethId::CheckItems, Заголовок_); };
	// Function ChooseItem(String Title_ = <default>, ValueListItem Item_ = <default>) -> OneOf<Undefined, ValueListItem>
	НеопределеноИли<ЭлементСпискаЗначений> ВыбратьЭлемент(КонстПарам<Строка> Заголовок_ = ПоУмолчаниюНеопределеноИли<esb::Строка>, КонстПарам<ЭлементСпискаЗначений> Элемент_ = ПоУмолчаниюНеопределеноИли<esb::ЭлементСпискаЗначений>) const { ESB_INVOKE_MEMB_FUNC2(НеопределеноИли<esb::ЭлементСпискаЗначений>, MethId::ChooseItem, Заголовок_, Элемент_); };
	// Void Clear()
	void Очистить() { ESB_INVOKE_MEMB_PROC0(MethId::Clear); };
	// Function Copy() -> ValueList
	СписокЗначений Скопировать() const { ESB_INVOKE_MEMB_FUNC0(esb::СписокЗначений, MethId::Copy); };
	// Function Count() -> Number
	Число Количество() const { ESB_INVOKE_MEMB_FUNC0(esb::Число, MethId::Count); };
	// Void Delete(OneOf<Number, ValueListItem> Item_)
	void Удалить(КонстПарам<Число> Элемент_) { ESB_INVOKE_MEMB_PROC1(MethId::Delete, Элемент_); };
	void Удалить(КонстПарам<ЭлементСпискаЗначений> Элемент_) { ESB_INVOKE_MEMB_PROC1(MethId::Delete, Элемент_); };
	// Void FillChecks(Boolean CheckMarkValue_)
	void ЗаполнитьПометки(КонстПарам<Булево> ЗначениеПометки_) { ESB_INVOKE_MEMB_PROC1(MethId::FillChecks, ЗначениеПометки_); };
	// Function FindByID(Number ID_) -> OneOf<Undefined, ValueListItem>
	НеопределеноИли<ЭлементСпискаЗначений> НайтиПоИдентификатору(КонстПарам<Число> Идентификатор_) const { ESB_INVOKE_MEMB_FUNC1(НеопределеноИли<esb::ЭлементСпискаЗначений>, MethId::FindByID, Идентификатор_); };
	// Function FindByValue(Arbitrary SoughtValue_) -> OneOf<Undefined, ValueListItem>
	НеопределеноИли<ЭлементСпискаЗначений> НайтиПоЗначению(КонстПарам<Произвольный> ИскомоеЗначение_) const { ESB_INVOKE_MEMB_FUNC1(НеопределеноИли<esb::ЭлементСпискаЗначений>, MethId::FindByValue, ИскомоеЗначение_); };
	// Function Get(Number Index_) -> ValueListItem
	ЭлементСпискаЗначений Получить(КонстПарам<Число> Индекс_) { ESB_INVOKE_MEMB_FUNC1(esb::ЭлементСпискаЗначений, MethId::Get, Индекс_); };
	// Function IndexOf(ValueListItem Item_) -> Number
	Число Индекс(КонстПарам<ЭлементСпискаЗначений> Элемент_) const { ESB_INVOKE_MEMB_FUNC1(esb::Число, MethId::IndexOf, Элемент_); };
	// Function Insert(Number Index_, Arbitrary Value_ = <default>, OneOf<FormattedString, String> Representation_ = <default>, Boolean CheckMark_ = <default>, Picture Picture_ = <default>) -> ValueListItem
	ЭлементСпискаЗначений Вставить(КонстПарам<Число> Индекс_, КонстПарам<Произвольный> Значение_ = ПоУмолчаниюНеопределеноИли<esb::Произвольный>, КонстПарам<Строка> Представление_ = ПоУмолчаниюНеопределеноИли<esb::Строка>, КонстПарам<Булево> Пометка_ = ПоУмолчаниюНеопределеноИли<esb::Булево>, КонстПарам<Картинка> Картинка_ = ПоУмолчаниюНеопределеноИли<esb::Картинка>) { ESB_INVOKE_MEMB_FUNC5(esb::ЭлементСпискаЗначений, MethId::Insert, Индекс_, Значение_, Представление_, Пометка_, Картинка_); };
	ЭлементСпискаЗначений Вставить(КонстПарам<Число> Индекс_, КонстПарам<Произвольный> Значение_ = ПоУмолчаниюНеопределеноИли<esb::Произвольный>, КонстПарам<ФорматированнаяСтрока> Представление_ = ПоУмолчаниюНеопределеноИли<esb::ФорматированнаяСтрока>, КонстПарам<Булево> Пометка_ = ПоУмолчаниюНеопределеноИли<esb::Булево>, КонстПарам<Картинка> Картинка_ = ПоУмолчаниюНеопределеноИли<esb::Картинка>) { ESB_INVOKE_MEMB_FUNC5(esb::ЭлементСпискаЗначений, MethId::Insert, Индекс_, Значение_, Представление_, Пометка_, Картинка_); };
	// Void LoadValues(Array ArrayOfValues_)
	void ЗагрузитьЗначения(КонстПарам<Массив> МассивЗначений_) { ESB_INVOKE_MEMB_PROC1(MethId::LoadValues, МассивЗначений_); };
	// Void Move(OneOf<Number, ValueListItem> Item_, Number Offset_)
	void Сдвинуть(КонстПарам<Число> Элемент_, КонстПарам<Число> Смещение_) { ESB_INVOKE_MEMB_PROC2(MethId::Move, Элемент_, Смещение_); };
	void Сдвинуть(КонстПарам<ЭлементСпискаЗначений> Элемент_, КонстПарам<Число> Смещение_) { ESB_INVOKE_MEMB_PROC2(MethId::Move, Элемент_, Смещение_); };
	// Void ShowCheckItems(NotifyDescription NotifyOnCloseDescription_, String Title_ = <default>)
	void ПоказатьОтметкуЭлементов(КонстПарам<ОписаниеОповещения> ОписаниеОповещенияОЗакрытии_, КонстПарам<Строка> Заголовок_ = ПоУмолчаниюНеопределеноИли<esb::Строка>) { ESB_INVOKE_MEMB_PROC2(MethId::ShowCheckItems, ОписаниеОповещенияОЗакрытии_, Заголовок_); };
	// Void ShowChooseItem(NotifyDescription NotifyOnCloseDescription_, String Title_ = <default>, ValueListItem Item_ = <default>)
	void ПоказатьВыборЭлемента(КонстПарам<ОписаниеОповещения> ОписаниеОповещенияОЗакрытии_, КонстПарам<Строка> Заголовок_ = ПоУмолчаниюНеопределеноИли<esb::Строка>, КонстПарам<ЭлементСпискаЗначений> Элемент_ = ПоУмолчаниюНеопределеноИли<esb::ЭлементСпискаЗначений>) const { ESB_INVOKE_MEMB_PROC3(MethId::ShowChooseItem, ОписаниеОповещенияОЗакрытии_, Заголовок_, Элемент_); };
	// Void SortByPresentation(SortDirection Direction_ = <default>)
	void СортироватьПоПредставлению(КонстПарам<НаправлениеСортировки> Направление_ = ПоУмолчаниюНеопределеноИли<esb::НаправлениеСортировки>) { ESB_INVOKE_MEMB_PROC1(MethId::SortByPresentation, Направление_); };
	// Void SortByValue(SortDirection Direction_ = <default>)
	void СортироватьПоЗначению(КонстПарам<НаправлениеСортировки> Направление_ = ПоУмолчаниюНеопределеноИли<esb::НаправлениеСортировки>) { ESB_INVOKE_MEMB_PROC1(MethId::SortByValue, Направление_); };
	// Function UnloadValues() -> Array
	Массив ВыгрузитьЗначения() const { ESB_INVOKE_MEMB_FUNC0(esb::Массив, MethId::UnloadValues); };
public:
	// Property [[Read,Write]] AvailableValues -> OneOf<Undefined, ValueList>
	НеопределеноИли<СписокЗначений> ПолучитьДоступныеЗначения() const { ESB_INVOKE_PROPERTY_GET(НеопределеноИли<esb::СписокЗначений>, PropId::AvailableValues); };
	void УстановитьДоступныеЗначения(КонстПарам<НеопределеноИли<СписокЗначений>> Значение_) { ESB_INVOKE_PROPERTY_SET(PropId::AvailableValues, Значение_); };
	ESB_PROPERTY_FIELD_RW(НеопределеноИли<СписокЗначений>, ДоступныеЗначения)
		// Property [[Read,Write]] ValueType -> TypeDescription
		ОписаниеТипов ПолучитьТипЗначения() const { ESB_INVOKE_PROPERTY_GET(esb::ОписаниеТипов, PropId::ValueType); };
	void УстановитьТипЗначения(КонстПарам<ОписаниеТипов> Значение_) { ESB_INVOKE_PROPERTY_SET(PropId::ValueType, Значение_); };
	ESB_PROPERTY_FIELD_RW(ОписаниеТипов, ТипЗначения)
#endif	// #if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
public:
	ESB_IMPLEMENT_ENUM_ITEMS(ValueList, ValueListItem)
public:
	ESB_IMPLEMENT_INDEXED_COLLECTION_RO()
};


inline ValueTable		ValueTableRow::Owner() const			{ ESB_INVOKE_MEMB_FUNC0(esb::ValueTable, MethId::Owner); };
inline ValueTree		ValueTreeRow::Owner() const				{ ESB_INVOKE_MEMB_FUNC0(esb::ValueTree, MethId::Owner); };
inline ValueTreeRowCollection	ValueTreeRow::GetRows() const	{ ESB_INVOKE_PROPERTY_GET(esb::ValueTreeRowCollection, PropId::Rows); };
inline QueryResult				QueryResultSelection::Owner() const	{ ESB_INVOKE_MEMB_FUNC0(esb::QueryResult, MethId::Owner); };
#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
inline ТаблицаЗначений			СтрокаТаблицыЗначений::Владелец() const			{ ESB_INVOKE_MEMB_FUNC0(esb::ТаблицаЗначений, MethId::Owner); };
inline ДеревоЗначений			СтрокаДереваЗначений::Владелец() const			{ESB_INVOKE_MEMB_FUNC0(esb::ДеревоЗначений, MethId::Owner); };
inline КоллекцияСтрокДереваЗначений	СтрокаДереваЗначений::ПолучитьСтроки() const { ESB_INVOKE_PROPERTY_GET(esb::КоллекцияСтрокДереваЗначений, PropId::Rows); };
inline РезультатЗапроса				ВыборкаИзРезультатаЗапроса::Владелец() const { ESB_INVOKE_MEMB_FUNC0(esb::РезультатЗапроса, MethId::Owner); };
#endif


}	//namespace esb 



// dependent API section
#if ESB_API_PROVIDER_INCLUDE(mngbase) || ESB_API_PROVIDER_INCLUDE(backend0x7)
#	include "esdisp_api.h"
#endif
//
namespace esb 
{
#if ESB_API_PROVIDER_INCLUDE(mngbase)
#if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
	// Функция ПолучитьДанныеВыбора(ТипЗначения_ Как Тип, Параметры_ Как Структура) Возвращает СписокЗначений
	inline ValueList GetChoiceData(ConstParam<TypeValue> ValueType_, ConstParam<Structure> Parameters_) { ESB_INVOKE_API_FUNC2(ValueList, mngbase, GetChoiceData, ValueType_, Parameters_); };
#endif	// #if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
	// Function GetChoiceData(Type ValueType_, Structure Parameters_) -> ValueList
	inline СписокЗначений ПолучитьДанныеВыбора(КонстПарам<Тип> ТипЗначения_, КонстПарам<Структура> Параметры_) { ESB_INVOKE_API_FUNC2(СписокЗначений, mngbase, GetChoiceData, ТипЗначения_, Параметры_); };
#endif	// #if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
#endif	// #if ESB_API_PROVIDER_INCLUDE(mngbase)
#if ESB_API_PROVIDER_INCLUDE(backend0x7)
#if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
	// Функция НайтиПоСсылкам(СписокСсылок_ Как Массив, [ОбластьПоиска_ Как Массив], [ВключитьОбъекты_ Как Массив], [ИсключитьОбъекты_ Как Массив]) Возвращает ТаблицаЗначений
	inline ValueTable FindByRef(ConstParam<Array> ReferenceList_, ConstParam<Array> SearchArea_ = DefUndef<Array>, ConstParam<Array> IncludeObjects_ = DefUndef<Array>, ConstParam<Array> ExcludeObjects_ = DefUndef<Array>) { ESB_INVOKE_API_FUNC4(ValueTable, backend0x7, FindByRef, ReferenceList_, SearchArea_, IncludeObjects_, ExcludeObjects_); };
	// Процедура УдалитьОбъекты(СписокСсылок_ Как Массив, [Проверять_ Как Булево], [НайденныеДанные_ Как ТаблицаЗначений], [ОбластьПоиска_ Как Массив], [ВключитьОбъекты_ Как Массив], [ИсключитьОбъекты_ Как Массив])
	//!> the method is actually a function according to disp-information.
	// Функция ПолучитьСтруктуруХраненияБазыДанных([ОбъектыМетаданных_ Как Массив], [ИменаБазыДанных_ Как Булево]) Возвращает ТаблицаЗначений
	inline ValueTable GetDBStorageStructureInfo(ConstParam<Array> MetadataObjects_ = DefUndef<Array>, ConstParam<Boolean> DBMSNames_ = DefUndef<Boolean>) { ESB_INVOKE_API_FUNC2(ValueTable, backend0x7, GetDBStorageStructureInfo, MetadataObjects_, DBMSNames_); };
#endif	// #if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
	// Function FindByRef(Array ReferenceList_, Array SearchArea_ = <default>, Array IncludeObjects_ = <default>, Array ExcludeObjects_ = <default>) -> ValueTable
	inline ТаблицаЗначений НайтиПоСсылкам(КонстПарам<Массив> СписокСсылок_, КонстПарам<Массив> ОбластьПоиска_ = ПоУмолчаниюНеопределеноИли<Массив>, КонстПарам<Массив> ВключитьОбъекты_ = ПоУмолчаниюНеопределеноИли<Массив>, КонстПарам<Массив> ИсключитьОбъекты_ = ПоУмолчаниюНеопределеноИли<Массив>) { ESB_INVOKE_API_FUNC4(ТаблицаЗначений, backend0x7, FindByRef, СписокСсылок_, ОбластьПоиска_, ВключитьОбъекты_, ИсключитьОбъекты_); };
	// Void DeleteObjects(Array ReferenceList_, Boolean Check_ = <default>, ValueTable DataFound_ = <default>, Array SearchArea_ = <default>, Array IncludeObjects_ = <default>, Array ExcludeObjects_ = <default>)
	//!> the method is actually a function according to disp-information.
	// Function GetDBStorageStructureInfo(Array MetadataObjects_ = <default>, Boolean DBMSNames_ = <default>) -> ValueTable
	inline ТаблицаЗначений ПолучитьСтруктуруХраненияБазыДанных(КонстПарам<Массив> ОбъектыМетаданных_ = ПоУмолчаниюНеопределеноИли<Массив>, КонстПарам<Булево> ИменаБазыДанных_ = ПоУмолчаниюНеопределеноИли<Булево>) { ESB_INVOKE_API_FUNC2(ТаблицаЗначений, backend0x7, GetDBStorageStructureInfo, ОбъектыМетаданных_, ИменаБазыДанных_); };
#endif	// #if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
#endif	// #if ESB_API_PROVIDER_INCLUDE(backend0x7)

}

#endif //ESB_ES1_DATA_H_
