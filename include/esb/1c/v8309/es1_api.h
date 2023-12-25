/*
	"ESB" is designed to create external native components for the 1C(tm) program.
	Copyright © 2023 IntUnsigned	(v8classes@gmail.com)
	"ESB" is free software under LGPLv2.1 license with essential restriction about binary part (view esb-license.txt)
	"ESB" IS DISTRIBUTED "AS-IS" WITHOUT ANY, EVEN THE IMPLIED, WARRANTY. IN NO EVENT SHALL THE AUTHORS OR THE COPYRIGHT OWNER BE LIABLE FOR ANY CLAIM,
	DAMAGES OR OTHER LIABILITY ARISING OUT OF THE USE OF THIS SOFTWARE.
	THE AUTHOR DOES NOT GIVE ANY GUARANTEES AND DOES NOT BEAR ANY RESPONSIBILITY REGARDING THE USE OF 1C(tm) COMPONENTS PRODUCED USING THE ESB LIBRARY IN THE 1C(tm) ENVIRONMENT.
*/


#pragma once
#ifndef ESB_ES1_API_H_
#define ESB_ES1_API_H_


// здесь "базовые" настройки, и, в том числе подключение пользовательской конфигурации (ESB_USE_API_ALL, ESB_API_PROVIDER_INCLUDE)
#include "esb/esb_config_base.h"
#include "es1_api_providers.h"
#include "esdisp_api.h"
#include "es1_main.h"



namespace esb //API part
{
#if ESB_API_PROVIDER_INCLUDE(ext0xD)
#if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
	// Функция ЧислоПрописью(Число_ Как Число, [ФорматнаяСтрока_ Как Строка], [ПараметрыПредметаИсчисления_ Как Строка]) Возвращает Строка
	inline String NumberInWords(ConstParam<Number> Number_, ConstParam<String> FormatString_ = DefUndef<String>, ConstParam<String> NumerationItemOptions_ = DefUndef<String>) { ESB_INVOKE_API_FUNC3(String, ext0xD, NumberInWords, Number_, FormatString_, NumerationItemOptions_); };
	// Функция ПредставлениеПериода(ДатаНачалаПериода_ Как Дата, ДатаОкончанияПериода_ Как Дата, [ФорматнаяСтрока_ Как Строка]) Возвращает Строка
	inline String PeriodPresentation(ConstParam<DateTime> BeginningDateOfPeriod_, ConstParam<DateTime> EndingDateOfPeriod_, ConstParam<String> FormatString_ = DefUndef<String>) { ESB_INVOKE_API_FUNC3(String, ext0xD, PeriodPresentation, BeginningDateOfPeriod_, EndingDateOfPeriod_, FormatString_); };
	// Процедура КопироватьФайл(ИмяФайлаИсточника_ Как Строка, ИмяФайлаПриемника_ Как Строка)
	inline void FileCopy(ConstParam<String> SourceFileName_, ConstParam<String> ReceiverFileName_) { ESB_INVOKE_API_PROC2(ext0xD, FileCopy, SourceFileName_, ReceiverFileName_); };
	// Процедура ПереместитьФайл(ИмяФайлаИсточника_ Как Строка, ИмяФайлаПриемника_ Как Строка)
	inline void MoveFile(ConstParam<String> SourceFileName_, ConstParam<String> ReceiverFileName_) { ESB_INVOKE_API_PROC2(ext0xD, MoveFile, SourceFileName_, ReceiverFileName_); };
	// Процедура УдалитьФайлы(Путь_ Как Строка, [Маска_ Как Строка])
	inline void DeleteFiles(ConstParam<String> Path_, ConstParam<String> Mask_ = DefUndef<String>) { ESB_INVOKE_API_PROC2(ext0xD, DeleteFiles, Path_, Mask_); };
	// Процедура СоздатьКаталог(ИмяКаталога_ Как Строка)
	inline void CreateDirectory(ConstParam<String> DirectoryName_) { ESB_INVOKE_API_PROC1(ext0xD, CreateDirectory, DirectoryName_); };
	// Процедура ОбъединитьФайлы(ШаблонПоиска_ Как Строка, ИмяРезультирующегоФайла_ Как Строка)
	inline void MergeFiles(ConstParam<String> SearchTemplate_, ConstParam<String> ResultingFileName_) { ESB_INVOKE_API_PROC2(ext0xD, MergeFiles, SearchTemplate_, ResultingFileName_); };
	// Процедура ОбъединитьФайлы(ШаблонПоиска_ Как Строка, ИмяРезультирующегоФайла_ Как Строка)
	inline void MergeFiles(ConstParam<Array> SegmentNames_, ConstParam<String> ResultingFileName_) { ESB_INVOKE_API_PROC2(ext0xD, MergeFiles, SegmentNames_, ResultingFileName_); };
	// Процедура ЗапуститьПриложение(СтрокаКоманды_ Как Строка, [ТекущийКаталог_ Как Строка], [ДождатьсяЗавершения_ Как Булево], [КодВозврата_ Как ОдинИз<Число, Неопределено>])
	inline void RunApp(ConstParam<String> CommandString_, ConstParam<String> CurrentDirectory_ = DefUndef<String>, ConstParam<Boolean> WaitForCompletion_ = DefUndef<Boolean>, ConstParam<Number> ReturnCode_ = DefUndef<Number>) { ESB_INVOKE_API_PROC4(ext0xD, RunApp, CommandString_, CurrentDirectory_, WaitForCompletion_, ReturnCode_); };
	// Функция ТекущаяУниверсальнаяДатаВМиллисекундах() Возвращает Число
	inline Number CurrentUniversalDateInMilliseconds() { ESB_INVOKE_API_FUNC0(Number, ext0xD, CurrentUniversalDateInMilliseconds); };
	// Процедура НачатьКопированиеФайла(ОписаниеОповещения_ Как ОписаниеОповещения, ИмяФайлаИсточника_ Как Строка, ИмяФайлаПриемника_ Как Строка)
	inline void BeginCopyingFile(ConstParam<NotifyDescription> NotifyDescription_, ConstParam<String> SourceFileName_, ConstParam<String> ReceiverFileName_) { ESB_INVOKE_API_PROC3(ext0xD, BeginCopyingFile, NotifyDescription_, SourceFileName_, ReceiverFileName_); };
	// Процедура НачатьПеремещениеФайла(ОписаниеОповещения_ Как ОписаниеОповещения, ИмяФайлаИсточника_ Как Строка, ИмяФайлаПриемника_ Как Строка)
	inline void BeginMovingFile(ConstParam<NotifyDescription> NotifyDescription_, ConstParam<String> SourceFileName_, ConstParam<String> ReceiverFileName_) { ESB_INVOKE_API_PROC3(ext0xD, BeginMovingFile, NotifyDescription_, SourceFileName_, ReceiverFileName_); };
	// Процедура НачатьУдалениеФайлов(ОписаниеОповещения_ Как ОписаниеОповещения, Путь_ Как Строка, [Маска_ Как Строка])
	inline void BeginDeletingFiles(ConstParam<NotifyDescription> NotifyDescription_, ConstParam<String> Path_, ConstParam<String> Mask_ = DefUndef<String>) { ESB_INVOKE_API_PROC3(ext0xD, BeginDeletingFiles, NotifyDescription_, Path_, Mask_); };
	// Процедура НачатьПоискФайлов(ОписаниеОповещения_ Как ОписаниеОповещения, Путь_ Как Строка, [Маска_ Как Строка], [ИскатьВПодкаталогах_ Как Булево])
	inline void BeginFindingFiles(ConstParam<NotifyDescription> NotifyDescription_, ConstParam<String> Path_, ConstParam<String> Mask_ = DefUndef<String>, ConstParam<Boolean> SearchInSubdirectories_ = DefUndef<Boolean>) { ESB_INVOKE_API_PROC4(ext0xD, BeginFindingFiles, NotifyDescription_, Path_, Mask_, SearchInSubdirectories_); };
	// Процедура НачатьСозданиеКаталога(ОписаниеОповещения_ Как ОписаниеОповещения, ИмяКаталога_ Как Строка)
	inline void BeginCreatingDirectory(ConstParam<NotifyDescription> NotifyDescription_, ConstParam<String> DirectoryName_) { ESB_INVOKE_API_PROC2(ext0xD, BeginCreatingDirectory, NotifyDescription_, DirectoryName_); };
	// Процедура НачатьЗапускПриложения([ОписаниеОповещения_ Как ОписаниеОповещения], СтрокаКоманды_ Как Строка, [ТекущийКаталог_ Как Строка], [ДождатьсяЗавершения_ Как Булево])
	inline void BeginRunningApplication(ConstParam<UndefOr<NotifyDescription>> NotifyDescription_, ConstParam<String> CommandLine_, ConstParam<String> CurrentDirectory_ = DefUndef<String>, ConstParam<Boolean> WaitForCompletion_ = DefUndef<Boolean>) { ESB_INVOKE_API_PROC4(ext0xD, BeginRunningApplication, NotifyDescription_, CommandLine_, CurrentDirectory_, WaitForCompletion_); };
#endif	// #if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
	// Function NumberInWords(Number Number_, String FormatString_ = <default>, String NumerationItemOptions_ = <default>) -> String
	inline Строка ЧислоПрописью(КонстПарам<Число> Число_, КонстПарам<Строка> ФорматнаяСтрока_ = ПоУмолчаниюНеопределеноИли<Строка>, КонстПарам<Строка> ПараметрыПредметаИсчисления_ = ПоУмолчаниюНеопределеноИли<Строка>) { ESB_INVOKE_API_FUNC3(Строка, ext0xD, NumberInWords, Число_, ФорматнаяСтрока_, ПараметрыПредметаИсчисления_); };
	// Function PeriodPresentation(Date BeginningDateOfPeriod_, Date EndingDateOfPeriod_, String FormatString_ = <default>) -> String
	inline Строка ПредставлениеПериода(КонстПарам<ДатаВремя> ДатаНачалаПериода_, КонстПарам<ДатаВремя> ДатаОкончанияПериода_, КонстПарам<Строка> ФорматнаяСтрока_ = ПоУмолчаниюНеопределеноИли<Строка>) { ESB_INVOKE_API_FUNC3(Строка, ext0xD, PeriodPresentation, ДатаНачалаПериода_, ДатаОкончанияПериода_, ФорматнаяСтрока_); };
	// Void FileCopy(String SourceFileName_, String ReceiverFileName_)
	inline void КопироватьФайл(КонстПарам<Строка> ИмяФайлаИсточника_, КонстПарам<Строка> ИмяФайлаПриемника_) { ESB_INVOKE_API_PROC2(ext0xD, FileCopy, ИмяФайлаИсточника_, ИмяФайлаПриемника_); };
	// Void MoveFile(String SourceFileName_, String ReceiverFileName_)
	inline void ПереместитьФайл(КонстПарам<Строка> ИмяФайлаИсточника_, КонстПарам<Строка> ИмяФайлаПриемника_) { ESB_INVOKE_API_PROC2(ext0xD, MoveFile, ИмяФайлаИсточника_, ИмяФайлаПриемника_); };
	// Void DeleteFiles(String Path_, String Mask_ = <default>)
	inline void УдалитьФайлы(КонстПарам<Строка> Путь_, КонстПарам<Строка> Маска_ = ПоУмолчаниюНеопределеноИли<Строка>) { ESB_INVOKE_API_PROC2(ext0xD, DeleteFiles, Путь_, Маска_); };
	// Void CreateDirectory(String DirectoryName_)
	inline void СоздатьКаталог(КонстПарам<Строка> ИмяКаталога_) { ESB_INVOKE_API_PROC1(ext0xD, CreateDirectory, ИмяКаталога_); };
	// Void MergeFiles(String SearchTemplate_, String ResultingFileName_)
	inline void ОбъединитьФайлы(КонстПарам<Строка> ШаблонПоиска_, КонстПарам<Строка> ИмяРезультирующегоФайла_) { ESB_INVOKE_API_PROC2(ext0xD, MergeFiles, ШаблонПоиска_, ИмяРезультирующегоФайла_); };
	// Void MergeFiles(String SearchTemplate_, String ResultingFileName_)
	inline void ОбъединитьФайлы(КонстПарам<Массив> ИменаЧастей_, КонстПарам<Строка> ИмяРезультирующегоФайла_) { ESB_INVOKE_API_PROC2(ext0xD, MergeFiles, ИменаЧастей_, ИмяРезультирующегоФайла_); };
	// Void RunApp(String CommandString_, String CurrentDirectory_ = <default>, Boolean WaitForCompletion_ = <default>, OneOf<Number, Undefined> ReturnCode_ = <default>)
	inline void ЗапуститьПриложение(КонстПарам<Строка> СтрокаКоманды_, КонстПарам<Строка> ТекущийКаталог_ = ПоУмолчаниюНеопределеноИли<Строка>, КонстПарам<Булево> ДождатьсяЗавершения_ = ПоУмолчаниюНеопределеноИли<Булево>, КонстПарам<Число> КодВозврата_ = ПоУмолчаниюНеопределеноИли<Число>) { ESB_INVOKE_API_PROC4(ext0xD, RunApp, СтрокаКоманды_, ТекущийКаталог_, ДождатьсяЗавершения_, КодВозврата_); };
	// Function CurrentUniversalDateInMilliseconds() -> Number
	inline Число ТекущаяУниверсальнаяДатаВМиллисекундах() { ESB_INVOKE_API_FUNC0(Число, ext0xD, CurrentUniversalDateInMilliseconds); };
	// Void BeginCopyingFile(NotifyDescription NotifyDescription_, String SourceFileName_, String ReceiverFileName_)
	inline void НачатьКопированиеФайла(КонстПарам<ОписаниеОповещения> ОписаниеОповещения_, КонстПарам<Строка> ИмяФайлаИсточника_, КонстПарам<Строка> ИмяФайлаПриемника_) { ESB_INVOKE_API_PROC3(ext0xD, BeginCopyingFile, ОписаниеОповещения_, ИмяФайлаИсточника_, ИмяФайлаПриемника_); };
	// Void BeginMovingFile(NotifyDescription NotifyDescription_, String SourceFileName_, String ReceiverFileName_)
	inline void НачатьПеремещениеФайла(КонстПарам<ОписаниеОповещения> ОписаниеОповещения_, КонстПарам<Строка> ИмяФайлаИсточника_, КонстПарам<Строка> ИмяФайлаПриемника_) { ESB_INVOKE_API_PROC3(ext0xD, BeginMovingFile, ОписаниеОповещения_, ИмяФайлаИсточника_, ИмяФайлаПриемника_); };
	// Void BeginDeletingFiles(NotifyDescription NotifyDescription_, String Path_, String Mask_ = <default>)
	inline void НачатьУдалениеФайлов(КонстПарам<ОписаниеОповещения> ОписаниеОповещения_, КонстПарам<Строка> Путь_, КонстПарам<Строка> Маска_ = ПоУмолчаниюНеопределеноИли<Строка>) { ESB_INVOKE_API_PROC3(ext0xD, BeginDeletingFiles, ОписаниеОповещения_, Путь_, Маска_); };
	// Void BeginFindingFiles(NotifyDescription NotifyDescription_, String Path_, String Mask_ = <default>, Boolean SearchInSubdirectories_ = <default>)
	inline void НачатьПоискФайлов(КонстПарам<ОписаниеОповещения> ОписаниеОповещения_, КонстПарам<Строка> Путь_, КонстПарам<Строка> Маска_ = ПоУмолчаниюНеопределеноИли<Строка>, КонстПарам<Булево> ИскатьВПодкаталогах_ = ПоУмолчаниюНеопределеноИли<Булево>) { ESB_INVOKE_API_PROC4(ext0xD, BeginFindingFiles, ОписаниеОповещения_, Путь_, Маска_, ИскатьВПодкаталогах_); };
	// Void BeginCreatingDirectory(NotifyDescription NotifyDescription_, String DirectoryName_)
	inline void НачатьСозданиеКаталога(КонстПарам<ОписаниеОповещения> ОписаниеОповещения_, КонстПарам<Строка> ИмяКаталога_) { ESB_INVOKE_API_PROC2(ext0xD, BeginCreatingDirectory, ОписаниеОповещения_, ИмяКаталога_); };
	// Void BeginRunningApplication(NotifyDescription NotifyDescription_ = <default>, String CommandLine_, String CurrentDirectory_ = <default>, Boolean WaitForCompletion_ = <default>)
	inline void НачатьЗапускПриложения(КонстПарам<НеопределеноИли<ОписаниеОповещения>> ОписаниеОповещения_, КонстПарам<Строка> СтрокаКоманды_, КонстПарам<Строка> ТекущийКаталог_ = ПоУмолчаниюНеопределеноИли<Строка>, КонстПарам<Булево> ДождатьсяЗавершения_ = ПоУмолчаниюНеопределеноИли<Булево>) { ESB_INVOKE_API_PROC4(ext0xD, BeginRunningApplication, ОписаниеОповещения_, СтрокаКоманды_, ТекущийКаталог_, ДождатьсяЗавершения_); };
#endif	// #if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
#endif	// #if ESB_API_PROVIDER_INCLUDE(ext0xD)
#if ESB_API_PROVIDER_INCLUDE(mngbase)
#if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
	// Функция ПолучитьФункциональнуюОпцию(Имя_ Как Строка, [Параметры_ Как Структура]) Возвращает Произвольный
	inline Arbitrary GetFunctionalOption(ConstParam<String> Name_, ConstParam<Structure> Parameters_ = DefUndef<Structure>) { ESB_INVOKE_API_FUNC2(Arbitrary, mngbase, GetFunctionalOption, Name_, Parameters_); };
	// Функция ПоместитьВоВременноеХранилище(Данные_ Как Произвольный, [Адрес_ Как ОдинИз<Строка, УникальныйИдентификатор>]) Возвращает Строка
	inline String PutToTempStorage(ConstParam<Arbitrary> Data_, ConstParam<String> Address_ = DefUndef<String>) { ESB_INVOKE_API_FUNC2(String, mngbase, PutToTempStorage, Data_, Address_); };
	inline String PutToTempStorage(ConstParam<Arbitrary> Data_, ConstParam<UUIDValue> Address_ = DefUndef<UUIDValue>) { ESB_INVOKE_API_FUNC2(String, mngbase, PutToTempStorage, Data_, Address_); };
	// Функция ПолучитьИзВременногоХранилища(Адрес_ Как Строка) Возвращает Произвольный
	inline Arbitrary GetFromTempStorage(ConstParam<String> Address_) { ESB_INVOKE_API_FUNC1(Arbitrary, mngbase, GetFromTempStorage, Address_); };
	// Процедура УдалитьИзВременногоХранилища(Адрес_ Как Строка)
	inline void DeleteFromTempStorage(ConstParam<String> Address_) { ESB_INVOKE_API_PROC1(mngbase, DeleteFromTempStorage, Address_); };
	// Функция ПолучитьПолноеИмяПредопределенногоЗначения(ПредопределенноеЗначение_ Как Произвольный) Возвращает Строка
	inline String GetPredefinedValueFullName(ConstParam<Arbitrary> PredefinedValue_) { ESB_INVOKE_API_FUNC1(String, mngbase, GetPredefinedValueFullName, PredefinedValue_); };
	// Функция ПолучитьМаскуВсеФайлыКлиента() Возвращает Строка
	inline String GetClientAllFilesMask() { ESB_INVOKE_API_FUNC0(String, mngbase, GetClientAllFilesMask); };
	// Функция ПолучитьРазделительПутиКлиента() Возвращает Строка
	inline String GetClientPathSeparator() { ESB_INVOKE_API_FUNC0(String, mngbase, GetClientPathSeparator); };
	// Процедура УстановитьСоединениеСВнешнимИсточникомДанных([ИмяВнешнегоИсточникаДанных_ Как Строка])
	inline void ConnectExternalDataSource(ConstParam<String> ExternalDataSourceName_ = DefUndef<String>) { ESB_INVOKE_API_PROC1(mngbase, ConnectExternalDataSource, ExternalDataSourceName_); };
	// Процедура РазорватьСоединениеСВнешнимИсточникомДанных([ИмяВнешнегоИсточникаДанных_ Как Строка])
	inline void DisconnectExternalDataSource(ConstParam<String> ExternalDataSourceName_ = DefUndef<String>) { ESB_INVOKE_API_PROC1(mngbase, DisconnectExternalDataSource, ExternalDataSourceName_); };
#endif	// #if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
	// Function GetFunctionalOption(String Name_, Structure Parameters_ = <default>) -> Arbitrary
	inline Произвольный ПолучитьФункциональнуюОпцию(КонстПарам<Строка> Имя_, КонстПарам<Структура> Параметры_ = ПоУмолчаниюНеопределеноИли<Структура>) { ESB_INVOKE_API_FUNC2(Произвольный, mngbase, GetFunctionalOption, Имя_, Параметры_); };
	// Function PutToTempStorage(Arbitrary Data_, OneOf<String, UUID> Address_ = <default>) -> String
	inline Строка ПоместитьВоВременноеХранилище(КонстПарам<Произвольный> Данные_, КонстПарам<Строка> Адрес_ = ПоУмолчаниюНеопределеноИли<Строка>) { ESB_INVOKE_API_FUNC2(Строка, mngbase, PutToTempStorage, Данные_, Адрес_); };
	inline Строка ПоместитьВоВременноеХранилище(КонстПарам<Произвольный> Данные_, КонстПарам<УникальныйИдентификатор> Адрес_ = ПоУмолчаниюНеопределеноИли<УникальныйИдентификатор>) { ESB_INVOKE_API_FUNC2(Строка, mngbase, PutToTempStorage, Данные_, Адрес_); };
	// Function GetFromTempStorage(String Address_) -> Arbitrary
	inline Произвольный ПолучитьИзВременногоХранилища(КонстПарам<Строка> Адрес_) { ESB_INVOKE_API_FUNC1(Произвольный, mngbase, GetFromTempStorage, Адрес_); };
	// Void DeleteFromTempStorage(String Address_)
	inline void УдалитьИзВременногоХранилища(КонстПарам<Строка> Адрес_) { ESB_INVOKE_API_PROC1(mngbase, DeleteFromTempStorage, Адрес_); };
	// Function GetPredefinedValueFullName(Arbitrary PredefinedValue_) -> String
	inline Строка ПолучитьПолноеИмяПредопределенногоЗначения(КонстПарам<Произвольный> ПредопределенноеЗначение_) { ESB_INVOKE_API_FUNC1(Строка, mngbase, GetPredefinedValueFullName, ПредопределенноеЗначение_); };
	// Function GetClientAllFilesMask() -> String
	inline Строка ПолучитьМаскуВсеФайлыКлиента() { ESB_INVOKE_API_FUNC0(Строка, mngbase, GetClientAllFilesMask); };
	// Function GetClientPathSeparator() -> String
	inline Строка ПолучитьРазделительПутиКлиента() { ESB_INVOKE_API_FUNC0(Строка, mngbase, GetClientPathSeparator); };
	// Void ConnectExternalDataSource(String ExternalDataSourceName_ = <default>)
	inline void УстановитьСоединениеСВнешнимИсточникомДанных(КонстПарам<Строка> ИмяВнешнегоИсточникаДанных_ = ПоУмолчаниюНеопределеноИли<Строка>) { ESB_INVOKE_API_PROC1(mngbase, ConnectExternalDataSource, ИмяВнешнегоИсточникаДанных_); };
	// Void DisconnectExternalDataSource(String ExternalDataSourceName_ = <default>)
	inline void РазорватьСоединениеСВнешнимИсточникомДанных(КонстПарам<Строка> ИмяВнешнегоИсточникаДанных_ = ПоУмолчаниюНеопределеноИли<Строка>) { ESB_INVOKE_API_PROC1(mngbase, DisconnectExternalDataSource, ИмяВнешнегоИсточникаДанных_); };
#endif	// #if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
#endif	// #if ESB_API_PROVIDER_INCLUDE(mngbase)
#if ESB_API_PROVIDER_INCLUDE(ext0xA)
#if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
	// Процедура НачатьСозданиеДвоичныхДанныхИзФайла(ОписаниеОповещения_ Как ОписаниеОповещения, ИмяФайла_ Как Строка)
	inline void BeginCreateBinaryDataFromFile(ConstParam<NotifyDescription> NotifyDescription_, ConstParam<String> FileName_) { ESB_INVOKE_API_PROC2(ext0xA, BeginCreateBinaryDataFromFile, NotifyDescription_, FileName_); };
#endif	// #if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
	// Void BeginCreateBinaryDataFromFile(NotifyDescription NotifyDescription_, String FileName_)
	inline void НачатьСозданиеДвоичныхДанныхИзФайла(КонстПарам<ОписаниеОповещения> ОписаниеОповещения_, КонстПарам<Строка> ИмяФайла_) { ESB_INVOKE_API_PROC2(ext0xA, BeginCreateBinaryDataFromFile, ОписаниеОповещения_, ИмяФайла_); };
#endif	// #if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
#endif	// #if ESB_API_PROVIDER_INCLUDE(ext0xA)
#if ESB_API_PROVIDER_INCLUDE(frntend)
#if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
	// Свойство [[Чтение,Запись]] РабочаяДата Как Дата
	//-PROP: WorkingDate	 ! not writable in context, but writable in info !
	// Свойство [[ТолькоЧтение]] ПараметрЗапуска Как Строка
	inline String GetLaunchParameter() { ESB_INVOKE_API_PROP_GET(String, frntend, LaunchParameter); };
	// Процедура Предупреждение(ТекстПредупреждения_ Как ОдинИз<ФорматированнаяСтрока, Строка>, [Таймаут_ Как Число], [Заголовок_ Как Строка])
	inline void DoMessageBox(ConstParam<String> MessageText_, ConstParam<Number> Timeout_ = DefUndef<Number>, ConstParam<String> Title_ = DefUndef<String>) { ESB_INVOKE_API_PROC3(frntend, DoMessageBox, MessageText_, Timeout_, Title_); };
	inline void DoMessageBox(ConstParam<FormattedString> MessageText_, ConstParam<Number> Timeout_ = DefUndef<Number>, ConstParam<String> Title_ = DefUndef<String>) { ESB_INVOKE_API_PROC3(frntend, DoMessageBox, MessageText_, Timeout_, Title_); };
	// Функция ВвестиСтроку(Строка_ Как Строка, [Подсказка_ Как Строка], [Длина_ Как Число], [Многострочность_ Как Булево]) Возвращает Булево
	inline Boolean InputString(ConstParam<String> String_, ConstParam<String> Tooltip_ = DefUndef<String>, ConstParam<Number> Length_ = DefUndef<Number>, ConstParam<Boolean> Multiline_ = DefUndef<Boolean>) { ESB_INVOKE_API_FUNC4(Boolean, frntend, InputString, String_, Tooltip_, Length_, Multiline_); };
	// Функция ВвестиЧисло(Число_ Как Число, [Подсказка_ Как Строка], [Длина_ Как Число], [Точность_ Как Число]) Возвращает Булево
	inline Boolean InputNumber(ConstParam<Number> Number_, ConstParam<String> Tooltip_ = DefUndef<String>, ConstParam<Number> Length_ = DefUndef<Number>, ConstParam<Number> Precision_ = DefUndef<Number>) { ESB_INVOKE_API_FUNC4(Boolean, frntend, InputNumber, Number_, Tooltip_, Length_, Precision_); };
	// Функция ВвестиДату(Дата_ Как Дата, [Подсказка_ Как Строка], [ЧастьДаты_ Как ЧастиДаты]) Возвращает Булево
	inline Boolean InputDate(ConstParam<DateTime> Date_, ConstParam<String> Tooltip_ = DefUndef<String>, ConstParam<DateFractions> DatePart_ = DefUndef<DateFractions>) { ESB_INVOKE_API_FUNC3(Boolean, frntend, InputDate, Date_, Tooltip_, DatePart_); };
	// Функция ВвестиЗначение(Значение_ Как Произвольный, [Подсказка_ Как Строка], [Тип_ Как ОдинИз<Тип, ОписаниеТипов>]) Возвращает Булево
	inline Boolean InputValue(ConstParam<Arbitrary> Value_, ConstParam<String> Tooltip_ = DefUndef<String>, ConstParam<TypeValue> type_ = DefUndef<TypeValue>) { ESB_INVOKE_API_FUNC3(Boolean, frntend, InputValue, Value_, Tooltip_, type_); };
	inline Boolean InputValue(ConstParam<Arbitrary> Value_, ConstParam<String> Tooltip_ = DefUndef<String>, ConstParam<TypeDescription> type_ = DefUndef<TypeDescription>) { ESB_INVOKE_API_FUNC3(Boolean, frntend, InputValue, Value_, Tooltip_, type_); };
	// Процедура ОткрытьЗначение(Значение_ Как Произвольный)
	inline void OpenValue(ConstParam<Arbitrary> Value_) { ESB_INVOKE_API_PROC1(frntend, OpenValue, Value_); };
	// Процедура Сигнал()
	inline void Beep() { ESB_INVOKE_API_PROC0(frntend, Beep); };
	// Функция ПолучитьЗаголовокСистемы() Возвращает Строка
	inline String GetCaption() { ESB_INVOKE_API_FUNC0(String, frntend, GetCaption); };
	// Процедура УстановитьЗаголовокСистемы(Заголовок_ Как Строка)
	inline void SetCaption(ConstParam<String> Title_) { ESB_INVOKE_API_PROC1(frntend, SetCaption, Title_); };
	// Процедура СохранитьЗначение(Имя_ Как Строка, Значение_ Как Произвольный)
	inline void SaveValue(ConstParam<String> Name_, ConstParam<Arbitrary> Value_) { ESB_INVOKE_API_PROC2(frntend, SaveValue, Name_, Value_); };
	// Функция ВосстановитьЗначение(Имя_ Как Строка) Возвращает Произвольный
	inline Arbitrary RestoreValue(ConstParam<String> Name_) { ESB_INVOKE_API_FUNC1(Arbitrary, frntend, RestoreValue, Name_); };
	// Процедура СохранитьНастройкиПользователя()
	inline void SaveUserSettings() { ESB_INVOKE_API_PROC0(frntend, SaveUserSettings); };
	// Процедура ЗапуститьСистему([ДополнительныеПараметрыКоманднойСтроки_ Как Строка], [ДождатьсяЗавершения_ Как Булево], [КодВозврата_ Как ОдинИз<Число, Неопределено>])
	inline void RunSystem(ConstParam<String> AdditionalParametersOfCommandLine_ = DefUndef<String>, ConstParam<Boolean> WaitForCompletion_ = DefUndef<Boolean>, ConstParam<Number> ReturnCode_ = DefUndef<Number>) { ESB_INVOKE_API_PROC3(frntend, RunSystem, AdditionalParametersOfCommandLine_, WaitForCompletion_, ReturnCode_); };
	// Процедура ЗавершитьРаботуСистемы([ЗапрашиватьВозможность_ Как Булево], [ПерезапуститьПриЗавершении_ Как Булево], [ДополнительныеПараметрыКоманднойСтроки_ Как Строка])
	inline void Exit(ConstParam<Boolean> OptionRequest_ = DefUndef<Boolean>, ConstParam<Boolean> RestartOnCompletion_ = DefUndef<Boolean>, ConstParam<String> AdditionalParametersOfCommandLine_ = DefUndef<String>) { ESB_INVOKE_API_PROC3(frntend, Exit, OptionRequest_, RestartOnCompletion_, AdditionalParametersOfCommandLine_); };
	// Процедура ПрекратитьРаботуСистемы([ПерезапуститьПриЗавершении_ Как Булево], [ДополнительныеПараметрыКоманднойСтроки_ Как Строка])
	inline void Terminate(ConstParam<Boolean> RestartOnCompletion_ = DefUndef<Boolean>, ConstParam<String> AdditionalParametersOfCommandLine_ = DefUndef<String>) { ESB_INVOKE_API_PROC2(frntend, Terminate, RestartOnCompletion_, AdditionalParametersOfCommandLine_); };
	// Процедура ПодключитьОбработчикОжидания(ИмяПроцедуры_ Как Строка, Интервал_ Как Число, [Однократно_ Как Булево])
	inline void AttachIdleHandler(ConstParam<String> ProcedureName_, ConstParam<Number> Interval_, ConstParam<Boolean> Single_ = DefUndef<Boolean>) { ESB_INVOKE_API_PROC3(frntend, AttachIdleHandler, ProcedureName_, Interval_, Single_); };
	// Процедура ОтключитьОбработчикОжидания(ИмяПроцедуры_ Как Строка)
	inline void DetachIdleHandler(ConstParam<String> ProcedureName_) { ESB_INVOKE_API_PROC1(frntend, DetachIdleHandler, ProcedureName_); };
	// Процедура ЗаблокироватьРаботуПользователя()
	inline void LockApplication() { ESB_INVOKE_API_PROC0(frntend, LockApplication); };
	// Процедура ОчиститьСообщения()
	inline void ClearMessages() { ESB_INVOKE_API_PROC0(frntend, ClearMessages); };
	// Процедура Состояние([ТекстСообщения_ Как Строка], [Прогресс_ Как Число], [Пояснение_ Как Строка], [Картинка_ Как Картинка])
	inline void Status(ConstParam<String> MessageText_ = DefUndef<String>, ConstParam<Number> Progress_ = DefUndef<Number>, ConstParam<String> Explanation_ = DefUndef<String>, ConstParam<Picture> Picture_ = DefUndef<Picture>) { ESB_INVOKE_API_PROC4(frntend, Status, MessageText_, Progress_, Explanation_, Picture_); };
	// Процедура Оповестить([ИмяСобытия_ Как Строка], [Параметр_ Как Произвольный], [Источник_ Как Произвольный])
	inline void Notify(ConstParam<String> EventName_ = DefUndef<String>, ConstParam<Arbitrary> Parameter_ = DefUndef<Arbitrary>, ConstParam<Arbitrary> Source_ = DefUndef<Arbitrary>) { ESB_INVOKE_API_PROC3(frntend, Notify, EventName_, Parameter_, Source_); };
	// Процедура ОбработкаПрерыванияПользователя()
	inline void UserInterruptProcessing() { ESB_INVOKE_API_PROC0(frntend, UserInterruptProcessing); };
	// Функция ОткрытьСодержаниеСправки() Возвращает Булево
	inline Boolean OpenHelpContent() { ESB_INVOKE_API_FUNC0(Boolean, frntend, OpenHelpContent); };
	// Функция ОткрытьИндексСправки([ТекстДляПоиска_ Как Строка]) Возвращает Булево
	inline Boolean OpenHelpIndex(ConstParam<String> SearchText_ = DefUndef<String>) { ESB_INVOKE_API_FUNC1(Boolean, frntend, OpenHelpIndex, SearchText_); };
	// Процедура ЗакрытьСправку()
	inline void CloseHelp() { ESB_INVOKE_API_PROC0(frntend, CloseHelp); };
	// Процедура ПодключитьОбработчикОповещения(ИмяПроцедуры_ Как Строка)
	inline void AttachNotificationHandler(ConstParam<String> ProcedureName_) { ESB_INVOKE_API_PROC1(frntend, AttachNotificationHandler, ProcedureName_); };
	// Процедура ОтключитьОбработчикОповещения(ИмяПроцедуры_ Как Строка)
	inline void DetachNotificationHandler(ConstParam<String> ProcedureName_) { ESB_INVOKE_API_PROC1(frntend, DetachNotificationHandler, ProcedureName_); };
	// Процедура ПоказатьИнформациюОбОшибке(ИнформацияОбОшибке_ Как ИнформацияОбОшибке)
	inline void ShowErrorInfo(ConstParam<ErrorInfoValue> ErrorInfo_) { ESB_INVOKE_API_PROC1(frntend, ShowErrorInfo, ErrorInfo_); };
	// Процедура ВыполнитьОбработкуЗаданий([ВремяОбработки_ Как Число])
	inline void ProcessJobs(ConstParam<Number> ProcessingTime_ = DefUndef<Number>) { ESB_INVOKE_API_PROC1(frntend, ProcessJobs, ProcessingTime_); };
	// Функция РабочийКаталогДанныхПользователя() Возвращает Строка
	inline String UserDataWorkDir() { ESB_INVOKE_API_FUNC0(String, frntend, UserDataWorkDir); };
	// Функция КаталогДокументов() Возвращает Строка
	inline String DocumentsDir() { ESB_INVOKE_API_FUNC0(String, frntend, DocumentsDir); };
	// Процедура НачатьПолучениеКаталогаДокументов(ОписаниеОповещения_ Как ОписаниеОповещения)
	inline void BeginGettingDocumentsDir(ConstParam<NotifyDescription> NotifyDescription_) { ESB_INVOKE_API_PROC1(frntend, BeginGettingDocumentsDir, NotifyDescription_); };
	// Процедура НачатьПолучениеРабочегоКаталогаДанныхПользователя(ОписаниеОповещения_ Как ОписаниеОповещения)
	inline void BeginGettingUserDataWorkDir(ConstParam<NotifyDescription> NotifyDescription_) { ESB_INVOKE_API_PROC1(frntend, BeginGettingUserDataWorkDir, NotifyDescription_); };
	// Процедура ПоказатьПредупреждение([ОписаниеОповещенияОЗавершении_ Как ОписаниеОповещения], ТекстПредупреждения_ Как ОдинИз<ФорматированнаяСтрока, Строка>, [Таймаут_ Как Число], [Заголовок_ Как Строка])
	inline void ShowMessageBox(ConstParam<UndefOr<NotifyDescription>> NotifyDescriptionOnCompletion_, ConstParam<String> MessageText_, ConstParam<Number> Timeout_ = DefUndef<Number>, ConstParam<String> Title_ = DefUndef<String>) { ESB_INVOKE_API_PROC4(frntend, ShowMessageBox, NotifyDescriptionOnCompletion_, MessageText_, Timeout_, Title_); };
	inline void ShowMessageBox(ConstParam<UndefOr<NotifyDescription>> NotifyDescriptionOnCompletion_, ConstParam<FormattedString> MessageText_, ConstParam<Number> Timeout_ = DefUndef<Number>, ConstParam<String> Title_ = DefUndef<String>) { ESB_INVOKE_API_PROC4(frntend, ShowMessageBox, NotifyDescriptionOnCompletion_, MessageText_, Timeout_, Title_); };
	// Процедура ПоказатьЗначение([ОписаниеОповещенияОЗавершении_ Как ОписаниеОповещения], Значение_ Как Произвольный)
	inline void ShowValue(ConstParam<UndefOr<NotifyDescription>> NotifyDescriptionOnCompletion_, ConstParam<Arbitrary> Value_) { ESB_INVOKE_API_PROC2(frntend, ShowValue, NotifyDescriptionOnCompletion_, Value_); };
	// Процедура ПоказатьВводДаты(ОписаниеОповещенияОЗавершении_ Как ОписаниеОповещения, [Дата_ Как Дата], [Подсказка_ Как Строка], [ЧастьДаты_ Как ЧастиДаты])
	inline void ShowInputDate(ConstParam<NotifyDescription> NotifyDescriptionOnCompletion_, ConstParam<DateTime> Date_ = DefUndef<DateTime>, ConstParam<String> Tooltip_ = DefUndef<String>, ConstParam<DateFractions> DatePart_ = DefUndef<DateFractions>) { ESB_INVOKE_API_PROC4(frntend, ShowInputDate, NotifyDescriptionOnCompletion_, Date_, Tooltip_, DatePart_); };
	// Процедура ПоказатьВводЗначения(ОписаниеОповещенияОЗавершении_ Как ОписаниеОповещения, Значение_ Как Произвольный, [Подсказка_ Как Строка], [Тип_ Как ОдинИз<Тип, ОписаниеТипов>])
	inline void ShowInputValue(ConstParam<NotifyDescription> NotifyDescriptionOnCompletion_, ConstParam<Arbitrary> Value_, ConstParam<String> Tooltip_ = DefUndef<String>, ConstParam<TypeValue> type_ = DefUndef<TypeValue>) { ESB_INVOKE_API_PROC4(frntend, ShowInputValue, NotifyDescriptionOnCompletion_, Value_, Tooltip_, type_); };
	inline void ShowInputValue(ConstParam<NotifyDescription> NotifyDescriptionOnCompletion_, ConstParam<Arbitrary> Value_, ConstParam<String> Tooltip_ = DefUndef<String>, ConstParam<TypeDescription> type_ = DefUndef<TypeDescription>) { ESB_INVOKE_API_PROC4(frntend, ShowInputValue, NotifyDescriptionOnCompletion_, Value_, Tooltip_, type_); };
	// Процедура ПоказатьВводСтроки(ОписаниеОповещенияОЗавершении_ Как ОписаниеОповещения, [Строка_ Как Строка], [Подсказка_ Как Строка], [Длина_ Как Число], [Многострочность_ Как Булево])
	inline void ShowInputString(ConstParam<NotifyDescription> NotifyDescriptionOnCompletion_, ConstParam<String> String_ = DefUndef<String>, ConstParam<String> Tooltip_ = DefUndef<String>, ConstParam<Number> Length_ = DefUndef<Number>, ConstParam<Boolean> Multiline_ = DefUndef<Boolean>) { ESB_INVOKE_API_PROC5(frntend, ShowInputString, NotifyDescriptionOnCompletion_, String_, Tooltip_, Length_, Multiline_); };
	// Процедура ПоказатьВводЧисла(ОписаниеОповещенияОЗавершении_ Как ОписаниеОповещения, Число_ Как Число, [Подсказка_ Как Строка], [Длина_ Как Число], [Точность_ Как Число])
	inline void ShowInputNumber(ConstParam<NotifyDescription> NotifyDescriptionOnCompletion_, ConstParam<Number> Number_, ConstParam<String> Tooltip_ = DefUndef<String>, ConstParam<Number> Length_ = DefUndef<Number>, ConstParam<Number> Precision_ = DefUndef<Number>) { ESB_INVOKE_API_PROC5(frntend, ShowInputNumber, NotifyDescriptionOnCompletion_, Number_, Tooltip_, Length_, Precision_); };
	// Процедура ПодключитьОбработчикЗапросаНастроекКлиентаЛицензирования(ИмяПроцедуры_ Как Строка)
	inline void AttachLicensingClientParametersRequestHandler(ConstParam<String> ProcedureName_) { ESB_INVOKE_API_PROC1(frntend, AttachLicensingClientParametersRequestHandler, ProcedureName_); };
	// Процедура ОтключитьОбработчикЗапросаНастроекКлиентаЛицензирования()
	inline void DetachLicensingClientParametersRequestHandler() { ESB_INVOKE_API_PROC0(frntend, DetachLicensingClientParametersRequestHandler); };
#endif	// #if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
	// Property [[Read,Write]] WorkingDate -> Date
	//-PROP: WorkingDate	 ! not writable in context, but writable in info !
	// Property [[ReadOnly]] LaunchParameter -> String
	inline Строка ПолучитьПараметрЗапуска() { ESB_INVOKE_API_PROP_GET(Строка, frntend, LaunchParameter); };
	// Void DoMessageBox(OneOf<FormattedString, String> MessageText_, Number Timeout_ = <default>, String Title_ = <default>)
	inline void Предупреждение(КонстПарам<Строка> ТекстПредупреждения_, КонстПарам<Число> Таймаут_ = ПоУмолчаниюНеопределеноИли<Число>, КонстПарам<Строка> Заголовок_ = ПоУмолчаниюНеопределеноИли<Строка>) { ESB_INVOKE_API_PROC3(frntend, DoMessageBox, ТекстПредупреждения_, Таймаут_, Заголовок_); };
	inline void Предупреждение(КонстПарам<ФорматированнаяСтрока> ТекстПредупреждения_, КонстПарам<Число> Таймаут_ = ПоУмолчаниюНеопределеноИли<Число>, КонстПарам<Строка> Заголовок_ = ПоУмолчаниюНеопределеноИли<Строка>) { ESB_INVOKE_API_PROC3(frntend, DoMessageBox, ТекстПредупреждения_, Таймаут_, Заголовок_); };
	// Function InputString(String String_, String Tooltip_ = <default>, Number Length_ = <default>, Boolean Multiline_ = <default>) -> Boolean
	inline Булево ВвестиСтроку(КонстПарам<Строка> Строка_, КонстПарам<Строка> Подсказка_ = ПоУмолчаниюНеопределеноИли<Строка>, КонстПарам<Число> Длина_ = ПоУмолчаниюНеопределеноИли<Число>, КонстПарам<Булево> Многострочность_ = ПоУмолчаниюНеопределеноИли<Булево>) { ESB_INVOKE_API_FUNC4(Булево, frntend, InputString, Строка_, Подсказка_, Длина_, Многострочность_); };
	// Function InputNumber(Number Number_, String Tooltip_ = <default>, Number Length_ = <default>, Number Precision_ = <default>) -> Boolean
	inline Булево ВвестиЧисло(КонстПарам<Число> Число_, КонстПарам<Строка> Подсказка_ = ПоУмолчаниюНеопределеноИли<Строка>, КонстПарам<Число> Длина_ = ПоУмолчаниюНеопределеноИли<Число>, КонстПарам<Число> Точность_ = ПоУмолчаниюНеопределеноИли<Число>) { ESB_INVOKE_API_FUNC4(Булево, frntend, InputNumber, Число_, Подсказка_, Длина_, Точность_); };
	// Function InputDate(Date Date_, String Tooltip_ = <default>, DateFractions DatePart_ = <default>) -> Boolean
	inline Булево ВвестиДату(КонстПарам<ДатаВремя> Дата_, КонстПарам<Строка> Подсказка_ = ПоУмолчаниюНеопределеноИли<Строка>, КонстПарам<ЧастиДаты> ЧастьДаты_ = ПоУмолчаниюНеопределеноИли<ЧастиДаты>) { ESB_INVOKE_API_FUNC3(Булево, frntend, InputDate, Дата_, Подсказка_, ЧастьДаты_); };
	// Function InputValue(Arbitrary Value_, String Tooltip_ = <default>, OneOf<Type, TypeDescription> Type_ = <default>) -> Boolean
	inline Булево ВвестиЗначение(КонстПарам<Произвольный> Значение_, КонстПарам<Строка> Подсказка_ = ПоУмолчаниюНеопределеноИли<Строка>, КонстПарам<Тип> Тип_ = ПоУмолчаниюНеопределеноИли<Тип>) { ESB_INVOKE_API_FUNC3(Булево, frntend, InputValue, Значение_, Подсказка_, Тип_); };
	inline Булево ВвестиЗначение(КонстПарам<Произвольный> Значение_, КонстПарам<Строка> Подсказка_ = ПоУмолчаниюНеопределеноИли<Строка>, КонстПарам<ОписаниеТипов> Тип_ = ПоУмолчаниюНеопределеноИли<ОписаниеТипов>) { ESB_INVOKE_API_FUNC3(Булево, frntend, InputValue, Значение_, Подсказка_, Тип_); };
	// Void OpenValue(Arbitrary Value_)
	inline void ОткрытьЗначение(КонстПарам<Произвольный> Значение_) { ESB_INVOKE_API_PROC1(frntend, OpenValue, Значение_); };
	// Void Beep()
	inline void Сигнал() { ESB_INVOKE_API_PROC0(frntend, Beep); };
	// Function GetCaption() -> String
	inline Строка ПолучитьЗаголовокСистемы() { ESB_INVOKE_API_FUNC0(Строка, frntend, GetCaption); };
	// Void SetCaption(String Title_)
	inline void УстановитьЗаголовокСистемы(КонстПарам<Строка> Заголовок_) { ESB_INVOKE_API_PROC1(frntend, SetCaption, Заголовок_); };
	// Void SaveValue(String Name_, Arbitrary Value_)
	inline void СохранитьЗначение(КонстПарам<Строка> Имя_, КонстПарам<Произвольный> Значение_) { ESB_INVOKE_API_PROC2(frntend, SaveValue, Имя_, Значение_); };
	// Function RestoreValue(String Name_) -> Arbitrary
	inline Произвольный ВосстановитьЗначение(КонстПарам<Строка> Имя_) { ESB_INVOKE_API_FUNC1(Произвольный, frntend, RestoreValue, Имя_); };
	// Void SaveUserSettings()
	inline void СохранитьНастройкиПользователя() { ESB_INVOKE_API_PROC0(frntend, SaveUserSettings); };
	// Void RunSystem(String AdditionalParametersOfCommandLine_ = <default>, Boolean WaitForCompletion_ = <default>, OneOf<Number, Undefined> ReturnCode_ = <default>)
	inline void ЗапуститьСистему(КонстПарам<Строка> ДополнительныеПараметрыКоманднойСтроки_ = ПоУмолчаниюНеопределеноИли<Строка>, КонстПарам<Булево> ДождатьсяЗавершения_ = ПоУмолчаниюНеопределеноИли<Булево>, КонстПарам<Число> КодВозврата_ = ПоУмолчаниюНеопределеноИли<Число>) { ESB_INVOKE_API_PROC3(frntend, RunSystem, ДополнительныеПараметрыКоманднойСтроки_, ДождатьсяЗавершения_, КодВозврата_); };
	// Void Exit(Boolean OptionRequest_ = <default>, Boolean RestartOnCompletion_ = <default>, String AdditionalParametersOfCommandLine_ = <default>)
	inline void ЗавершитьРаботуСистемы(КонстПарам<Булево> ЗапрашиватьВозможность_ = ПоУмолчаниюНеопределеноИли<Булево>, КонстПарам<Булево> ПерезапуститьПриЗавершении_ = ПоУмолчаниюНеопределеноИли<Булево>, КонстПарам<Строка> ДополнительныеПараметрыКоманднойСтроки_ = ПоУмолчаниюНеопределеноИли<Строка>) { ESB_INVOKE_API_PROC3(frntend, Exit, ЗапрашиватьВозможность_, ПерезапуститьПриЗавершении_, ДополнительныеПараметрыКоманднойСтроки_); };
	// Void Terminate(Boolean RestartOnCompletion_ = <default>, String AdditionalParametersOfCommandLine_ = <default>)
	inline void ПрекратитьРаботуСистемы(КонстПарам<Булево> ПерезапуститьПриЗавершении_ = ПоУмолчаниюНеопределеноИли<Булево>, КонстПарам<Строка> ДополнительныеПараметрыКоманднойСтроки_ = ПоУмолчаниюНеопределеноИли<Строка>) { ESB_INVOKE_API_PROC2(frntend, Terminate, ПерезапуститьПриЗавершении_, ДополнительныеПараметрыКоманднойСтроки_); };
	// Void AttachIdleHandler(String ProcedureName_, Number Interval_, Boolean Single_ = <default>)
	inline void ПодключитьОбработчикОжидания(КонстПарам<Строка> ИмяПроцедуры_, КонстПарам<Число> Интервал_, КонстПарам<Булево> Однократно_ = ПоУмолчаниюНеопределеноИли<Булево>) { ESB_INVOKE_API_PROC3(frntend, AttachIdleHandler, ИмяПроцедуры_, Интервал_, Однократно_); };
	// Void DetachIdleHandler(String ProcedureName_)
	inline void ОтключитьОбработчикОжидания(КонстПарам<Строка> ИмяПроцедуры_) { ESB_INVOKE_API_PROC1(frntend, DetachIdleHandler, ИмяПроцедуры_); };
	// Void LockApplication()
	inline void ЗаблокироватьРаботуПользователя() { ESB_INVOKE_API_PROC0(frntend, LockApplication); };
	// Void ClearMessages()
	inline void ОчиститьСообщения() { ESB_INVOKE_API_PROC0(frntend, ClearMessages); };
	// Void Status(String MessageText_ = <default>, Number Progress_ = <default>, String Explanation_ = <default>, Picture Picture_ = <default>)
	inline void Состояние(КонстПарам<Строка> ТекстСообщения_ = ПоУмолчаниюНеопределеноИли<Строка>, КонстПарам<Число> Прогресс_ = ПоУмолчаниюНеопределеноИли<Число>, КонстПарам<Строка> Пояснение_ = ПоУмолчаниюНеопределеноИли<Строка>, КонстПарам<Картинка> Картинка_ = ПоУмолчаниюНеопределеноИли<Картинка>) { ESB_INVOKE_API_PROC4(frntend, Status, ТекстСообщения_, Прогресс_, Пояснение_, Картинка_); };
	// Void Notify(String EventName_ = <default>, Arbitrary Parameter_ = <default>, Arbitrary Source_ = <default>)
	inline void Оповестить(КонстПарам<Строка> ИмяСобытия_ = ПоУмолчаниюНеопределеноИли<Строка>, КонстПарам<Произвольный> Параметр_ = ПоУмолчаниюНеопределеноИли<Произвольный>, КонстПарам<Произвольный> Источник_ = ПоУмолчаниюНеопределеноИли<Произвольный>) { ESB_INVOKE_API_PROC3(frntend, Notify, ИмяСобытия_, Параметр_, Источник_); };
	// Void UserInterruptProcessing()
	inline void ОбработкаПрерыванияПользователя() { ESB_INVOKE_API_PROC0(frntend, UserInterruptProcessing); };
	// Function OpenHelpContent() -> Boolean
	inline Булево ОткрытьСодержаниеСправки() { ESB_INVOKE_API_FUNC0(Булево, frntend, OpenHelpContent); };
	// Function OpenHelpIndex(String SearchText_ = <default>) -> Boolean
	inline Булево ОткрытьИндексСправки(КонстПарам<Строка> ТекстДляПоиска_ = ПоУмолчаниюНеопределеноИли<Строка>) { ESB_INVOKE_API_FUNC1(Булево, frntend, OpenHelpIndex, ТекстДляПоиска_); };
	// Void CloseHelp()
	inline void ЗакрытьСправку() { ESB_INVOKE_API_PROC0(frntend, CloseHelp); };
	// Void AttachNotificationHandler(String ProcedureName_)
	inline void ПодключитьОбработчикОповещения(КонстПарам<Строка> ИмяПроцедуры_) { ESB_INVOKE_API_PROC1(frntend, AttachNotificationHandler, ИмяПроцедуры_); };
	// Void DetachNotificationHandler(String ProcedureName_)
	inline void ОтключитьОбработчикОповещения(КонстПарам<Строка> ИмяПроцедуры_) { ESB_INVOKE_API_PROC1(frntend, DetachNotificationHandler, ИмяПроцедуры_); };
	// Void ShowErrorInfo(ErrorInfo ErrorInfo_)
	inline void ПоказатьИнформациюОбОшибке(КонстПарам<ИнформацияОбОшибке> ИнформацияОбОшибке_) { ESB_INVOKE_API_PROC1(frntend, ShowErrorInfo, ИнформацияОбОшибке_); };
	// Void ProcessJobs(Number ProcessingTime_ = <default>)
	inline void ВыполнитьОбработкуЗаданий(КонстПарам<Число> ВремяОбработки_ = ПоУмолчаниюНеопределеноИли<Число>) { ESB_INVOKE_API_PROC1(frntend, ProcessJobs, ВремяОбработки_); };
	// Function UserDataWorkDir() -> String
	inline Строка РабочийКаталогДанныхПользователя() { ESB_INVOKE_API_FUNC0(Строка, frntend, UserDataWorkDir); };
	// Function DocumentsDir() -> String
	inline Строка КаталогДокументов() { ESB_INVOKE_API_FUNC0(Строка, frntend, DocumentsDir); };
	// Void BeginGettingDocumentsDir(NotifyDescription NotifyDescription_)
	inline void НачатьПолучениеКаталогаДокументов(КонстПарам<ОписаниеОповещения> ОписаниеОповещения_) { ESB_INVOKE_API_PROC1(frntend, BeginGettingDocumentsDir, ОписаниеОповещения_); };
	// Void BeginGettingUserDataWorkDir(NotifyDescription NotifyDescription_)
	inline void НачатьПолучениеРабочегоКаталогаДанныхПользователя(КонстПарам<ОписаниеОповещения> ОписаниеОповещения_) { ESB_INVOKE_API_PROC1(frntend, BeginGettingUserDataWorkDir, ОписаниеОповещения_); };
	// Void ShowMessageBox(NotifyDescription NotifyDescriptionOnCompletion_ = <default>, OneOf<FormattedString, String> MessageText_, Number Timeout_ = <default>, String Title_ = <default>)
	inline void ПоказатьПредупреждение(КонстПарам<НеопределеноИли<ОписаниеОповещения>> ОписаниеОповещенияОЗавершении_, КонстПарам<Строка> ТекстПредупреждения_, КонстПарам<Число> Таймаут_ = ПоУмолчаниюНеопределеноИли<Число>, КонстПарам<Строка> Заголовок_ = ПоУмолчаниюНеопределеноИли<Строка>) { ESB_INVOKE_API_PROC4(frntend, ShowMessageBox, ОписаниеОповещенияОЗавершении_, ТекстПредупреждения_, Таймаут_, Заголовок_); };
	inline void ПоказатьПредупреждение(КонстПарам<НеопределеноИли<ОписаниеОповещения>> ОписаниеОповещенияОЗавершении_, КонстПарам<ФорматированнаяСтрока> ТекстПредупреждения_, КонстПарам<Число> Таймаут_ = ПоУмолчаниюНеопределеноИли<Число>, КонстПарам<Строка> Заголовок_ = ПоУмолчаниюНеопределеноИли<Строка>) { ESB_INVOKE_API_PROC4(frntend, ShowMessageBox, ОписаниеОповещенияОЗавершении_, ТекстПредупреждения_, Таймаут_, Заголовок_); };
	// Void ShowValue(NotifyDescription NotifyDescriptionOnCompletion_ = <default>, Arbitrary Value_)
	inline void ПоказатьЗначение(КонстПарам<НеопределеноИли<ОписаниеОповещения>> ОписаниеОповещенияОЗавершении_, КонстПарам<Произвольный> Значение_) { ESB_INVOKE_API_PROC2(frntend, ShowValue, ОписаниеОповещенияОЗавершении_, Значение_); };
	// Void ShowInputDate(NotifyDescription NotifyDescriptionOnCompletion_, Date Date_ = <default>, String Tooltip_ = <default>, DateFractions DatePart_ = <default>)
	inline void ПоказатьВводДаты(КонстПарам<ОписаниеОповещения> ОписаниеОповещенияОЗавершении_, КонстПарам<ДатаВремя> Дата_ = ПоУмолчаниюНеопределеноИли<ДатаВремя>, КонстПарам<Строка> Подсказка_ = ПоУмолчаниюНеопределеноИли<Строка>, КонстПарам<ЧастиДаты> ЧастьДаты_ = ПоУмолчаниюНеопределеноИли<ЧастиДаты>) { ESB_INVOKE_API_PROC4(frntend, ShowInputDate, ОписаниеОповещенияОЗавершении_, Дата_, Подсказка_, ЧастьДаты_); };
	// Void ShowInputValue(NotifyDescription NotifyDescriptionOnCompletion_, Arbitrary Value_, String Tooltip_ = <default>, OneOf<Type, TypeDescription> Type_ = <default>)
	inline void ПоказатьВводЗначения(КонстПарам<ОписаниеОповещения> ОписаниеОповещенияОЗавершении_, КонстПарам<Произвольный> Значение_, КонстПарам<Строка> Подсказка_ = ПоУмолчаниюНеопределеноИли<Строка>, КонстПарам<Тип> Тип_ = ПоУмолчаниюНеопределеноИли<Тип>) { ESB_INVOKE_API_PROC4(frntend, ShowInputValue, ОписаниеОповещенияОЗавершении_, Значение_, Подсказка_, Тип_); };
	inline void ПоказатьВводЗначения(КонстПарам<ОписаниеОповещения> ОписаниеОповещенияОЗавершении_, КонстПарам<Произвольный> Значение_, КонстПарам<Строка> Подсказка_ = ПоУмолчаниюНеопределеноИли<Строка>, КонстПарам<ОписаниеТипов> Тип_ = ПоУмолчаниюНеопределеноИли<ОписаниеТипов>) { ESB_INVOKE_API_PROC4(frntend, ShowInputValue, ОписаниеОповещенияОЗавершении_, Значение_, Подсказка_, Тип_); };
	// Void ShowInputString(NotifyDescription NotifyDescriptionOnCompletion_, String String_ = <default>, String Tooltip_ = <default>, Number Length_ = <default>, Boolean Multiline_ = <default>)
	inline void ПоказатьВводСтроки(КонстПарам<ОписаниеОповещения> ОписаниеОповещенияОЗавершении_, КонстПарам<Строка> Строка_ = ПоУмолчаниюНеопределеноИли<Строка>, КонстПарам<Строка> Подсказка_ = ПоУмолчаниюНеопределеноИли<Строка>, КонстПарам<Число> Длина_ = ПоУмолчаниюНеопределеноИли<Число>, КонстПарам<Булево> Многострочность_ = ПоУмолчаниюНеопределеноИли<Булево>) { ESB_INVOKE_API_PROC5(frntend, ShowInputString, ОписаниеОповещенияОЗавершении_, Строка_, Подсказка_, Длина_, Многострочность_); };
	// Void ShowInputNumber(NotifyDescription NotifyDescriptionOnCompletion_, Number Number_, String Tooltip_ = <default>, Number Length_ = <default>, Number Precision_ = <default>)
	inline void ПоказатьВводЧисла(КонстПарам<ОписаниеОповещения> ОписаниеОповещенияОЗавершении_, КонстПарам<Число> Число_, КонстПарам<Строка> Подсказка_ = ПоУмолчаниюНеопределеноИли<Строка>, КонстПарам<Число> Длина_ = ПоУмолчаниюНеопределеноИли<Число>, КонстПарам<Число> Точность_ = ПоУмолчаниюНеопределеноИли<Число>) { ESB_INVOKE_API_PROC5(frntend, ShowInputNumber, ОписаниеОповещенияОЗавершении_, Число_, Подсказка_, Длина_, Точность_); };
	// Void AttachLicensingClientParametersRequestHandler(String ProcedureName_)
	inline void ПодключитьОбработчикЗапросаНастроекКлиентаЛицензирования(КонстПарам<Строка> ИмяПроцедуры_) { ESB_INVOKE_API_PROC1(frntend, AttachLicensingClientParametersRequestHandler, ИмяПроцедуры_); };
	// Void DetachLicensingClientParametersRequestHandler()
	inline void ОтключитьОбработчикЗапросаНастроекКлиентаЛицензирования() { ESB_INVOKE_API_PROC0(frntend, DetachLicensingClientParametersRequestHandler); };
#endif	// #if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
#endif	// #if ESB_API_PROVIDER_INCLUDE(frntend)
#if ESB_API_PROVIDER_INCLUDE(extui)
#if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
	// Процедура УстановитьРасширениеРаботыСКриптографией()
	inline void InstallCryptoExtension() { ESB_INVOKE_API_PROC0(extui, InstallCryptoExtension); };
	// Процедура НачатьУстановкуРасширенияРаботыСКриптографией([ОписаниеОповещенияОЗавершении_ Как ОписаниеОповещения])
	inline void BeginInstallCryptoExtension(ConstParam<NotifyDescription> NotifyDescriptionOnCompletion_ = DefUndef<NotifyDescription>) { ESB_INVOKE_API_PROC1(extui, BeginInstallCryptoExtension, NotifyDescriptionOnCompletion_); };
	// Функция ПодключитьРасширениеРаботыСКриптографией() Возвращает Булево
	inline Boolean AttachCryptoExtension() { ESB_INVOKE_API_FUNC0(Boolean, extui, AttachCryptoExtension); };
	// Процедура НачатьПодключениеРасширенияРаботыСКриптографией(ОписаниеОповещения_ Как ОписаниеОповещения)
	inline void BeginAttachingCryptoExtension(ConstParam<NotifyDescription> NotifyDescription_) { ESB_INVOKE_API_PROC1(extui, BeginAttachingCryptoExtension, NotifyDescription_); };
#endif	// #if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
	// Void InstallCryptoExtension()
	inline void УстановитьРасширениеРаботыСКриптографией() { ESB_INVOKE_API_PROC0(extui, InstallCryptoExtension); };
	// Void BeginInstallCryptoExtension(NotifyDescription NotifyDescriptionOnCompletion_ = <default>)
	inline void НачатьУстановкуРасширенияРаботыСКриптографией(КонстПарам<ОписаниеОповещения> ОписаниеОповещенияОЗавершении_ = ПоУмолчаниюНеопределеноИли<ОписаниеОповещения>) { ESB_INVOKE_API_PROC1(extui, BeginInstallCryptoExtension, ОписаниеОповещенияОЗавершении_); };
	// Function AttachCryptoExtension() -> Boolean
	inline Булево ПодключитьРасширениеРаботыСКриптографией() { ESB_INVOKE_API_FUNC0(Булево, extui, AttachCryptoExtension); };
	// Void BeginAttachingCryptoExtension(NotifyDescription NotifyDescription_)
	inline void НачатьПодключениеРасширенияРаботыСКриптографией(КонстПарам<ОписаниеОповещения> ОписаниеОповещения_) { ESB_INVOKE_API_PROC1(extui, BeginAttachingCryptoExtension, ОписаниеОповещения_); };
#endif	// #if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
#endif	// #if ESB_API_PROVIDER_INCLUDE(extui)
#if ESB_API_PROVIDER_INCLUDE(xml2)
#if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
	// Функция НайтиНедопустимыеСимволыXML(СтрокаСимволов_ Как Строка, [ПозицияНачала_ Как Число], [Версия_ Как Строка]) Возвращает Число
	inline Number FindDisallowedXMLCharacters(ConstParam<String> CharacterString_, ConstParam<Number> StartPosition_ = DefUndef<Number>, ConstParam<String> Version_ = DefUndef<String>) { ESB_INVOKE_API_FUNC3(Number, xml2, FindDisallowedXMLCharacters, CharacterString_, StartPosition_, Version_); };
#endif	// #if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
	// Function FindDisallowedXMLCharacters(String CharacterString_, Number StartPosition_ = <default>, String Version_ = <default>) -> Number
	inline Число НайтиНедопустимыеСимволыXML(КонстПарам<Строка> СтрокаСимволов_, КонстПарам<Число> ПозицияНачала_ = ПоУмолчаниюНеопределеноИли<Число>, КонстПарам<Строка> Версия_ = ПоУмолчаниюНеопределеноИли<Строка>) { ESB_INVOKE_API_FUNC3(Число, xml2, FindDisallowedXMLCharacters, СтрокаСимволов_, ПозицияНачала_, Версия_); };
#endif	// #if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
#endif	// #if ESB_API_PROVIDER_INCLUDE(xml2)
#if ESB_API_PROVIDER_INCLUDE(mngcore)
#if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
	// Функция ЭтоАдресВременногоХранилища(Адрес_ Как Строка) Возвращает Булево
	inline Boolean IsTempStorageURL(ConstParam<String> Address_) { ESB_INVOKE_API_FUNC1(Boolean, mngcore, IsTempStorageURL, Address_); };
	// Функция ПолучитьНавигационнуюСсылкуИнформационнойБазы() Возвращает Строка
	inline String GetInfoBaseURL() { ESB_INVOKE_API_FUNC0(String, mngcore, GetInfoBaseURL); };
	// Функция ПолучитьМаскуВсеФайлы() Возвращает Строка
	inline String GetAllFilesMask() { ESB_INVOKE_API_FUNC0(String, mngcore, GetAllFilesMask); };
	// Функция ПолучитьРазделительПути() Возвращает Строка
	inline String GetPathSeparator() { ESB_INVOKE_API_FUNC0(String, mngcore, GetPathSeparator); };
#endif	// #if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
	// Function IsTempStorageURL(String Address_) -> Boolean
	inline Булево ЭтоАдресВременногоХранилища(КонстПарам<Строка> Адрес_) { ESB_INVOKE_API_FUNC1(Булево, mngcore, IsTempStorageURL, Адрес_); };
	// Function GetInfoBaseURL() -> String
	inline Строка ПолучитьНавигационнуюСсылкуИнформационнойБазы() { ESB_INVOKE_API_FUNC0(Строка, mngcore, GetInfoBaseURL); };
	// Function GetAllFilesMask() -> String
	inline Строка ПолучитьМаскуВсеФайлы() { ESB_INVOKE_API_FUNC0(Строка, mngcore, GetAllFilesMask); };
	// Function GetPathSeparator() -> String
	inline Строка ПолучитьРазделительПути() { ESB_INVOKE_API_FUNC0(Строка, mngcore, GetPathSeparator); };
#endif	// #if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
#endif	// #if ESB_API_PROVIDER_INCLUDE(mngcore)
#if ESB_API_PROVIDER_INCLUDE(odata)
#if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
	// Процедура УстановитьСоставСтандартногоИнтерфейсаOData(ОбъектыМетаданных_ Как Массив)
	inline void SetStandardODataInterfaceContent(ConstParam<Array> MetadataObjects_) { ESB_INVOKE_API_PROC1(odata, SetStandardODataInterfaceContent, MetadataObjects_); };
#endif	// #if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
	// Void SetStandardODataInterfaceContent(Array MetadataObjects_)
	inline void УстановитьСоставСтандартногоИнтерфейсаOData(КонстПарам<Массив> ОбъектыМетаданных_) { ESB_INVOKE_API_PROC1(odata, SetStandardODataInterfaceContent, ОбъектыМетаданных_); };
#endif	// #if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
#endif	// #if ESB_API_PROVIDER_INCLUDE(odata)
#if ESB_API_PROVIDER_INCLUDE(core83)
#if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
	// Процедура ЗаполнитьЗначенияСвойств(Приемник_ Как Произвольный, Источник_ Как Произвольный, [СписокСвойств_ Как Строка], [ИсключаяСвойства_ Как Строка])
	inline void FillPropertyValues(ConstParam<Arbitrary> Receiver_, ConstParam<Arbitrary> Source_, ConstParam<String> ListOfProperties_ = DefUndef<String>, ConstParam<String> ExcludeProperties_ = DefUndef<String>) { ESB_INVOKE_API_PROC4(core83, FillPropertyValues, Receiver_, Source_, ListOfProperties_, ExcludeProperties_); };
	// Функция ЗначениеЗаполнено(Значение_ Как Произвольный) Возвращает Булево
	inline Boolean ValueIsFilled(ConstParam<Arbitrary> Value_) { ESB_INVOKE_API_FUNC1(Boolean, core83, ValueIsFilled, Value_); };
	// Функция СтрНайти(Строка_ Как Строка, ПодстрокаПоиска_ Как Строка, [НаправлениеПоиска_ Как НаправлениеПоиска], [НачальнаяПозиция_ Как Число], [НомерВхождения_ Как Число]) Возвращает Число
	inline Number StrFind(ConstParam<String> Line_, ConstParam<String> SearchSubstring_, ConstParam<SearchDirection> SearchDirection_ = SearchDirection::FromBegin, ConstParam<Number> StartIndex_ = DefUndef<Number>, ConstParam<Number> EntryNumber_ = Number::Value_1_) { ESB_INVOKE_API_FUNC5(Number, core83, StrFind, Line_, SearchSubstring_, SearchDirection_, StartIndex_, EntryNumber_); };
	// Функция СтрШаблон(Шаблон_ Как Строка, [Значение1-Значение10_ Как Произвольный] ...) Возвращает Строка
	// !> The number of parameters differs from the disp-information:
	// !> Finction StrTemplate(_in_out_ _0, _in_out_ _1 = <Undef>, _in_out_ _2 = <Undef>, _in_out_ _3 = <Undef>, _in_out_ _4 = <Undef>, _in_out_ _5 = <Undef>, _in_out_ _6 = <Undef>, _in_out_ _7 = <Undef>, _in_out_ _8 = <Undef>, _in_out_ _9 = <Undef>, _in_out_ _10 = <Undef>); #5
	// Функция СтрСравнить(Строка1_ Как Строка, Строка2_ Как Строка) Возвращает Число
	inline Number StrCompare(ConstParam<String> String1_, ConstParam<String> String2_) { ESB_INVOKE_API_FUNC2(Number, core83, StrCompare, String1_, String2_); };
	// Функция СтрНачинаетсяС(Строка_ Как Строка, СтрокаПоиска_ Как Строка) Возвращает Булево
	inline Boolean StrStartWith(ConstParam<String> String_, ConstParam<String> SearchString_) { ESB_INVOKE_API_FUNC2(Boolean, core83, StrStartWith, String_, SearchString_); };
	// Функция СтрЗаканчиваетсяНа(Строка_ Как Строка, СтрокаПоиска_ Как Строка) Возвращает Булево
	inline Boolean StrEndsWith(ConstParam<String> String_, ConstParam<String> SearchString_) { ESB_INVOKE_API_FUNC2(Boolean, core83, StrEndsWith, String_, SearchString_); };
	// Функция СтрРазделить(Строка_ Как Строка, Разделитель_ Как Строка, [ВключатьПустые_ Как Булево]) Возвращает МассивИз<Строка>
	inline ArrayOf<String> StrSplit(ConstParam<String> String_, ConstParam<String> Separator_, ConstParam<Boolean> IncludeBlank_ = DefUndef<Boolean>) { ESB_INVOKE_API_FUNC3(ArrayOf<String>, core83, StrSplit, String_, Separator_, IncludeBlank_); };
	// Функция СтрСоединить(Строки_ Как ОдинИз<Массив, ФиксированныйМассив>, [Разделитель_ Как Строка]) Возвращает Строка
	inline String StrConcat(ConstParam<Array> Strings_, ConstParam<String> Separator_ = DefUndef<String>) { ESB_INVOKE_API_FUNC2(String, core83, StrConcat, Strings_, Separator_); };
	inline String StrConcat(ConstParam<FixedArray> Strings_, ConstParam<String> Separator_ = DefUndef<String>) { ESB_INVOKE_API_FUNC2(String, core83, StrConcat, Strings_, Separator_); };
#endif	// #if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
	// Void FillPropertyValues(Arbitrary Receiver_, Arbitrary Source_, String ListOfProperties_ = <default>, String ExcludeProperties_ = <default>)
	inline void ЗаполнитьЗначенияСвойств(КонстПарам<Произвольный> Приемник_, КонстПарам<Произвольный> Источник_, КонстПарам<Строка> СписокСвойств_ = ПоУмолчаниюНеопределеноИли<Строка>, КонстПарам<Строка> ИсключаяСвойства_ = ПоУмолчаниюНеопределеноИли<Строка>) { ESB_INVOKE_API_PROC4(core83, FillPropertyValues, Приемник_, Источник_, СписокСвойств_, ИсключаяСвойства_); };
	// Function ValueIsFilled(Arbitrary Value_) -> Boolean
	inline Булево ЗначениеЗаполнено(КонстПарам<Произвольный> Значение_) { ESB_INVOKE_API_FUNC1(Булево, core83, ValueIsFilled, Значение_); };
	// Function StrFind(String Line_, String SearchSubstring_, SearchDirection SearchDirection_ = <default>, Number StartIndex_ = <default>, Number EntryNumber_ = <default>) -> Number
	inline Число СтрНайти(КонстПарам<Строка> Строка_, КонстПарам<Строка> ПодстрокаПоиска_, КонстПарам<НаправлениеПоиска> НаправлениеПоиска_ = ПоУмолчаниюНеопределеноИли<НаправлениеПоиска>, КонстПарам<Число> НачальнаяПозиция_ = ПоУмолчаниюНеопределеноИли<Число>, КонстПарам<Число> НомерВхождения_ = ПоУмолчаниюНеопределеноИли<Число>) { ESB_INVOKE_API_FUNC5(Число, core83, StrFind, Строка_, ПодстрокаПоиска_, НаправлениеПоиска_, НачальнаяПозиция_, НомерВхождения_); };
	// Function StrTemplate(String Template_, Arbitrary Value1-Value10_ = <default> ...) -> String
	// !> The number of parameters differs from the disp-information:
	// !> Finction StrTemplate(_in_out_ _0, _in_out_ _1 = <Undef>, _in_out_ _2 = <Undef>, _in_out_ _3 = <Undef>, _in_out_ _4 = <Undef>, _in_out_ _5 = <Undef>, _in_out_ _6 = <Undef>, _in_out_ _7 = <Undef>, _in_out_ _8 = <Undef>, _in_out_ _9 = <Undef>, _in_out_ _10 = <Undef>); #5
	// Function StrCompare(String String1_, String String2_) -> Number
	inline Число СтрСравнить(КонстПарам<Строка> Строка1_, КонстПарам<Строка> Строка2_) { ESB_INVOKE_API_FUNC2(Число, core83, StrCompare, Строка1_, Строка2_); };
	// Function StrStartWith(String String_, String SearchString_) -> Boolean
	inline Булево СтрНачинаетсяС(КонстПарам<Строка> Строка_, КонстПарам<Строка> СтрокаПоиска_) { ESB_INVOKE_API_FUNC2(Булево, core83, StrStartWith, Строка_, СтрокаПоиска_); };
	// Function StrEndsWith(String String_, String SearchString_) -> Boolean
	inline Булево СтрЗаканчиваетсяНа(КонстПарам<Строка> Строка_, КонстПарам<Строка> СтрокаПоиска_) { ESB_INVOKE_API_FUNC2(Булево, core83, StrEndsWith, Строка_, СтрокаПоиска_); };
	// Function StrSplit(String String_, String Separator_, Boolean IncludeBlank_ = <default>) -> ArrayOf<String>
	inline МассивИз<Строка> СтрРазделить(КонстПарам<Строка> Строка_, КонстПарам<Строка> Разделитель_, КонстПарам<Булево> ВключатьПустые_ = ПоУмолчаниюНеопределеноИли<Булево>) { ESB_INVOKE_API_FUNC3(МассивИз<Строка>, core83, StrSplit, Строка_, Разделитель_, ВключатьПустые_); };
	// Function StrConcat(OneOf<Array, FixedArray> Strings_, String Separator_ = <default>) -> String
	inline Строка СтрСоединить(КонстПарам<Массив> Строки_, КонстПарам<Строка> Разделитель_ = ПоУмолчаниюНеопределеноИли<Строка>) { ESB_INVOKE_API_FUNC2(Строка, core83, StrConcat, Строки_, Разделитель_); };
	inline Строка СтрСоединить(КонстПарам<ФиксированныйМассив> Строки_, КонстПарам<Строка> Разделитель_ = ПоУмолчаниюНеопределеноИли<Строка>) { ESB_INVOKE_API_FUNC2(Строка, core83, StrConcat, Строки_, Разделитель_); };
#endif	// #if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
#endif	// #if ESB_API_PROVIDER_INCLUDE(core83)
#if ESB_API_PROVIDER_INCLUDE(backend0x7)
#if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
	// Процедура ЗафиксироватьТранзакцию()
	inline void CommitTransaction() { ESB_INVOKE_API_PROC0(backend0x7, CommitTransaction); };
	// Процедура ОтменитьТранзакцию()
	inline void RollbackTransaction() { ESB_INVOKE_API_PROC0(backend0x7, RollbackTransaction); };
	// Функция ТранзакцияАктивна() Возвращает Булево
	inline Boolean TransactionActive() { ESB_INVOKE_API_FUNC0(Boolean, backend0x7, TransactionActive); };
	// Процедура Сообщить(ТекстСообщения_ Как Строка, [Статус_ Как СтатусСообщения])
	inline void Message(ConstParam<String> MessageText_, ConstParam<MessageStatus> Status_ = DefUndef<MessageStatus>) { ESB_INVOKE_API_PROC2(backend0x7, Message, MessageText_, Status_); };
	// Процедура УстановитьМонопольныйРежим(МонопольныйРежим_ Как Булево)
	inline void SetExclusiveMode(ConstParam<Boolean> ExclusiveMode_) { ESB_INVOKE_API_PROC1(backend0x7, SetExclusiveMode, ExclusiveMode_); };
	// Функция МонопольныйРежим() Возвращает Булево
	inline Boolean ExclusiveMode() { ESB_INVOKE_API_FUNC0(Boolean, backend0x7, ExclusiveMode); };
	// Функция НСтр(ИсходнаяСтрока_ Как Строка, [КодЯзыка_ Как Строка]) Возвращает Строка
	inline String NStr(ConstParam<String> InitialString_, ConstParam<String> LanguageCode_ = DefUndef<String>) { ESB_INVOKE_API_FUNC2(String, backend0x7, NStr, InitialString_, LanguageCode_); };
	// Функция ПолучитьОперативнуюОтметкуВремени() Возвращает Дата
	inline DateTime GetRealTimeTimestamp() { ESB_INVOKE_API_FUNC0(DateTime, backend0x7, GetRealTimeTimestamp); };
	// Функция ЗначениеВСтрокуВнутр(Значение_ Как Произвольный) Возвращает Строка
	inline String ValueToStringInternal(ConstParam<Arbitrary> Value_) { ESB_INVOKE_API_FUNC1(String, backend0x7, ValueToStringInternal, Value_); };
	// Функция ЗначениеИзСтрокиВнутр(Строка_ Как Строка) Возвращает Произвольный
	inline Arbitrary ValueFromStringInternal(ConstParam<String> Line_) { ESB_INVOKE_API_FUNC1(Arbitrary, backend0x7, ValueFromStringInternal, Line_); };
	// Функция ЗначениеВФайл(ИмяФайла_ Как Строка, Значение_ Как Произвольный) Возвращает Булево
	inline Boolean ValueToFile(ConstParam<String> FileName_, ConstParam<Arbitrary> Value_) { ESB_INVOKE_API_FUNC2(Boolean, backend0x7, ValueToFile, FileName_, Value_); };
	// Функция ЗначениеИзФайла(ИмяФайла_ Как Строка) Возвращает Произвольный
	inline Arbitrary ValueFromFile(ConstParam<String> FileName_) { ESB_INVOKE_API_FUNC1(Arbitrary, backend0x7, ValueFromFile, FileName_); };
	// Функция ИмяКомпьютера() Возвращает Строка
	inline String ComputerName() { ESB_INVOKE_API_FUNC0(String, backend0x7, ComputerName); };
	// Функция ИмяПользователя() Возвращает Строка
	inline String UserName() { ESB_INVOKE_API_FUNC0(String, backend0x7, UserName); };
	// Функция ПолноеИмяПользователя() Возвращает Строка
	inline String UserFullName() { ESB_INVOKE_API_FUNC0(String, backend0x7, UserFullName); };
	// Функция КаталогПрограммы() Возвращает Строка
	inline String BinDir() { ESB_INVOKE_API_FUNC0(String, backend0x7, BinDir); };
	// Функция КаталогВременныхФайлов() Возвращает Строка
	inline String TempFilesDir() { ESB_INVOKE_API_FUNC0(String, backend0x7, TempFilesDir); };
	// Процедура НачатьПолучениеКаталогаВременныхФайлов(ОписаниеОповещения_ Как ОписаниеОповещения)
	inline void BeginGettingTempFilesDir(ConstParam<NotifyDescription> NotifyDescription_) { ESB_INVOKE_API_PROC1(backend0x7, BeginGettingTempFilesDir, NotifyDescription_); };
	// Функция ТекущийКодЛокализации() Возвращает Строка
	inline String CurrentLocaleCode() { ESB_INVOKE_API_FUNC0(String, backend0x7, CurrentLocaleCode); };
	// Функция КодЛокализацииИнформационнойБазы() Возвращает Строка
	inline String InfoBaseLocaleCode() { ESB_INVOKE_API_FUNC0(String, backend0x7, InfoBaseLocaleCode); };
	// Функция СтрокаСоединенияИнформационнойБазы() Возвращает Строка
	inline String InfoBaseConnectionString() { ESB_INVOKE_API_FUNC0(String, backend0x7, InfoBaseConnectionString); };
	// Процедура СкопироватьЖурналРегистрации([ИмяВходногоФайла_ Как Строка], [ИмяВыходногоФайла_ Как Строка], Фильтр_ Как Структура)
	inline void CopyEventLog(ConstParam<UndefOr<String>> InputFileName_, ConstParam<UndefOr<String>> OutputFileName_, ConstParam<Structure> Filter_) { ESB_INVOKE_API_PROC3(backend0x7, CopyEventLog, InputFileName_, OutputFileName_, Filter_); };
	// Процедура ОчиститьЖурналРегистрации(Фильтр_ Как Структура)
	inline void ClearEventLog(ConstParam<Structure> Filter_) { ESB_INVOKE_API_PROC1(backend0x7, ClearEventLog, Filter_); };
	// Функция ПредставлениеПриложения(ИмяПриложения_ Как Строка) Возвращает Строка
	inline String ApplicationPresentation(ConstParam<String> ApplicationIdentifier_) { ESB_INVOKE_API_FUNC1(String, backend0x7, ApplicationPresentation, ApplicationIdentifier_); };
	// Функция НомерСоединенияИнформационнойБазы() Возвращает Число
	inline Number InfoBaseConnectionNumber() { ESB_INVOKE_API_FUNC0(Number, backend0x7, InfoBaseConnectionNumber); };
	// Функция НомерСеансаИнформационнойБазы() Возвращает Число
	inline Number InfoBaseSessionNumber() { ESB_INVOKE_API_FUNC0(Number, backend0x7, InfoBaseSessionNumber); };
	// Функция ПредставлениеСобытияЖурналаРегистрации(ИдентификаторСобытия_ Как Строка) Возвращает Строка
	inline String EventLogEventPresentation(ConstParam<String> EventIdentifier_) { ESB_INVOKE_API_FUNC1(String, backend0x7, EventLogEventPresentation, EventIdentifier_); };
	// Процедура УстановитьИспользованиеЖурналаРегистрации(Уровни_ Как Массив)
	inline void SetEventLogUsing(ConstParam<Array> Levels_) { ESB_INVOKE_API_PROC1(backend0x7, SetEventLogUsing, Levels_); };
	// Функция КонфигурацияИзменена() Возвращает Булево
	inline Boolean ConfigurationChanged() { ESB_INVOKE_API_FUNC0(Boolean, backend0x7, ConfigurationChanged); };
	// Функция КонфигурацияБазыДанныхИзмененаДинамически() Возвращает Булево
	inline Boolean DataBaseConfigurationChangedDynamically() { ESB_INVOKE_API_FUNC0(Boolean, backend0x7, DataBaseConfigurationChangedDynamically); };
	// Процедура УстановитьВремяОжиданияБлокировкиДанных([Таймаут_ Как Число])
	inline void SetLockWaitTime(ConstParam<Number> Timeout_ = DefUndef<Number>) { ESB_INVOKE_API_PROC1(backend0x7, SetLockWaitTime, Timeout_); };
	// Функция ПолучитьВремяОжиданияБлокировкиДанных() Возвращает Число
	inline Number GetLockWaitTime() { ESB_INVOKE_API_FUNC0(Number, backend0x7, GetLockWaitTime); };
	// Процедура УстановитьМинимальнуюДлинуПаролейПользователей([Длина_ Как Число])
	inline void SetUserPasswordMinLength(ConstParam<Number> Length_ = DefUndef<Number>) { ESB_INVOKE_API_PROC1(backend0x7, SetUserPasswordMinLength, Length_); };
	// Функция ПолучитьМинимальнуюДлинуПаролейПользователей() Возвращает Число
	inline Number GetUserPasswordMinLength() { ESB_INVOKE_API_FUNC0(Number, backend0x7, GetUserPasswordMinLength); };
	// Процедура УстановитьПроверкуСложностиПаролейПользователей(СложныйПароль_ Как Булево)
	inline void SetUserPasswordStrengthCheck(ConstParam<Boolean> ComplexPassword_) { ESB_INVOKE_API_PROC1(backend0x7, SetUserPasswordStrengthCheck, ComplexPassword_); };
	// Функция ПолучитьПроверкуСложностиПаролейПользователей() Возвращает Булево
	inline Boolean GetUserPasswordStrengthCheck() { ESB_INVOKE_API_FUNC0(Boolean, backend0x7, GetUserPasswordStrengthCheck); };
	// Функция ТекущийЯзыкСистемы() Возвращает Строка
	inline String CurrentSystemLanguage() { ESB_INVOKE_API_FUNC0(String, backend0x7, CurrentSystemLanguage); };
	// Функция КраткоеПредставлениеОшибки(ИнформацияОбОшибке_ Как ИнформацияОбОшибке) Возвращает Строка
	inline String BriefErrorDescription(ConstParam<ErrorInfoValue> ErrorInfo_) { ESB_INVOKE_API_FUNC1(String, backend0x7, BriefErrorDescription, ErrorInfo_); };
	// Функция ПодробноеПредставлениеОшибки(ИнформацияОбОшибке_ Как ИнформацияОбОшибке) Возвращает Строка
	inline String DetailErrorDescription(ConstParam<ErrorInfoValue> ErrorInfo_) { ESB_INVOKE_API_FUNC1(String, backend0x7, DetailErrorDescription, ErrorInfo_); };
	// Процедура УстановитьПривилегированныйРежим(Включить_ Как Булево)
	inline void SetPrivilegedMode(ConstParam<Boolean> PrivilegedModeOn_) { ESB_INVOKE_API_PROC1(backend0x7, SetPrivilegedMode, PrivilegedModeOn_); };
	// Функция ПривилегированныйРежим() Возвращает Булево
	inline Boolean PrivilegedMode() { ESB_INVOKE_API_FUNC0(Boolean, backend0x7, PrivilegedMode); };
	// Процедура ЗаблокироватьДанныеДляРедактирования(Ключ_ Как Произвольный, [ВерсияДанных_ Как Строка], [ИдентификаторФормы_ Как УникальныйИдентификатор])
	inline void LockDataForEdit(ConstParam<Arbitrary> Key_, ConstParam<String> DataVersion_ = DefUndef<String>, ConstParam<UUIDValue> FormIdentifier_ = DefUndef<UUIDValue>) { ESB_INVOKE_API_PROC3(backend0x7, LockDataForEdit, Key_, DataVersion_, FormIdentifier_); };
	// Процедура ОбновитьПовторноИспользуемыеЗначения()
	inline void RefreshReusableValues() { ESB_INVOKE_API_PROC0(backend0x7, RefreshReusableValues); };
	// Функция ПолучитьЗначенияОтбораЖурналаРегистрации([Колонки_ Как Строка], [ИмяВходногоФайла_ Как Строка]) Возвращает Структура_GETEVENTLOGFILTERVALUES
	//TODO	Structure_GETEVENTLOGFILTERVALUES
	inline Structure GetEventLogFilterValues(ConstParam<String> Columns_ = DefUndef<String>, ConstParam<String> InputFileName_ = DefUndef<String>) { ESB_INVOKE_API_FUNC2(Structure, backend0x7, GetEventLogFilterValues, Columns_, InputFileName_); };
	// Процедура УстановитьБезопасныйРежим(БезопасныйРежим_ Как ОдинИз<Булево, Строка>)
	inline void SetSafeMode(ConstParam<Boolean> SafeMode_) { ESB_INVOKE_API_PROC1(backend0x7, SetSafeMode, SafeMode_); };
	inline void SetSafeMode(ConstParam<String> SafeMode_) { ESB_INVOKE_API_PROC1(backend0x7, SetSafeMode, SafeMode_); };
	// Функция БезопасныйРежим() Возвращает ОдинИз<Булево, Строка>
	inline OneOf<Boolean, String> SafeMode() { ESB_INVOKE_API_FUNC0(ESB_ONEOF(Boolean, String), backend0x7, SafeMode); };
	// Процедура УстановитьЧасовойПоясИнформационнойБазы([ЧасовойПояс_ Как Строка])
	inline void SetInfoBaseTimeZone(ConstParam<String> TimeZone_ = DefUndef<String>) { ESB_INVOKE_API_PROC1(backend0x7, SetInfoBaseTimeZone, TimeZone_); };
	// Функция ПолучитьЧасовойПоясИнформационнойБазы() Возвращает ОдинИз<Строка, Неопределено>
	inline UndefOr<String> GetInfoBaseTimeZone() { ESB_INVOKE_API_FUNC0(UndefOr<String>, backend0x7, GetInfoBaseTimeZone); };
	// Процедура УстановитьЧасовойПоясСеанса([ЧасовойПояс_ Как Строка])
	inline void SetSessionTimeZone(ConstParam<String> TimeZone_ = DefUndef<String>) { ESB_INVOKE_API_PROC1(backend0x7, SetSessionTimeZone, TimeZone_); };
	// Функция ЧасовойПоясСеанса() Возвращает Строка
	inline String SessionTimeZone() { ESB_INVOKE_API_FUNC0(String, backend0x7, SessionTimeZone); };
	// Функция ТекущаяДатаСеанса() Возвращает Дата
	inline DateTime CurrentSessionDate() { ESB_INVOKE_API_FUNC0(DateTime, backend0x7, CurrentSessionDate); };
	// Функция СмещениеЛетнегоВремени([ЧасовойПояс_ Как ОдинИз<Строка, Неопределено>], [УниверсальноеВремя_ Как ОдинИз<Дата, Неопределено>]) Возвращает Число
	inline Number DaylightTimeOffset(ConstParam<String> TimeZone_ = DefUndef<String>, ConstParam<DateTime> UniversalTime_ = DefUndef<DateTime>) { ESB_INVOKE_API_FUNC2(Number, backend0x7, DaylightTimeOffset, TimeZone_, UniversalTime_); };
	// Функция ТекущаяУниверсальнаяДата() Возвращает Дата
	inline DateTime CurrentUniversalDate() { ESB_INVOKE_API_FUNC0(DateTime, backend0x7, CurrentUniversalDate); };
	// Функция МестноеВремя(УниверсальноеВремя_ Как Дата, [ЧасовойПояс_ Как ОдинИз<Строка, Неопределено>]) Возвращает Дата
	inline DateTime ToLocalTime(ConstParam<DateTime> UniversalTime_, ConstParam<String> TimeZone_ = DefUndef<String>) { ESB_INVOKE_API_FUNC2(DateTime, backend0x7, ToLocalTime, UniversalTime_, TimeZone_); };
	// Функция УниверсальноеВремя(МестноеВремя_ Как Дата, [ЧасовойПояс_ Как ОдинИз<Строка, Неопределено>]) Возвращает Дата
	inline DateTime ToUniversalTime(ConstParam<DateTime> LocalTime_, ConstParam<String> TimeZone_ = DefUndef<String>) { ESB_INVOKE_API_FUNC2(DateTime, backend0x7, ToUniversalTime, LocalTime_, TimeZone_); };
	// Функция ЧасовойПояс() Возвращает Строка
	inline String TimeZone() { ESB_INVOKE_API_FUNC0(String, backend0x7, TimeZone); };
	// Функция ПолучитьДопустимыеЧасовыеПояса() Возвращает МассивИз<Строка>
	inline ArrayOf<String> GetAvailableTimeZones() { ESB_INVOKE_API_FUNC0(ArrayOf<String>, backend0x7, GetAvailableTimeZones); };
	// Функция ПредставлениеЧасовогоПояса(ЧасовойПояс_ Как Строка) Возвращает Строка
	inline String TimeZonePresentation(ConstParam<String> TimeZone_) { ESB_INVOKE_API_FUNC1(String, backend0x7, TimeZonePresentation, TimeZone_); };
	// Функция ПолучитьДопустимыеКодыЛокализации() Возвращает МассивИз<Строка>
	inline ArrayOf<String> GetAvailableLocaleCodes() { ESB_INVOKE_API_FUNC0(ArrayOf<String>, backend0x7, GetAvailableLocaleCodes); };
	// Функция ПредставлениеКодаЛокализации(КодЛокализации_ Как Строка) Возвращает Строка
	inline String LocaleCodePresentation(ConstParam<String> LocalizationCode_) { ESB_INVOKE_API_FUNC1(String, backend0x7, LocaleCodePresentation, LocalizationCode_); };
	// Функция СмещениеСтандартногоВремени([ЧасовойПояс_ Как ОдинИз<Строка, Неопределено>], [УниверсальноеВремя_ Как ОдинИз<Дата, Неопределено>]) Возвращает Дата
	inline DateTime StandardTimeOffset(ConstParam<String> TimeZone_ = DefUndef<String>, ConstParam<DateTime> UniversalTime_ = DefUndef<DateTime>) { ESB_INVOKE_API_FUNC2(DateTime, backend0x7, StandardTimeOffset, TimeZone_, UniversalTime_); };
	// Функция ПолучитьИмяВременногоФайла([Расширение_ Как Строка]) Возвращает Строка
	inline String GetTempFileName(ConstParam<String> Extension_ = DefUndef<String>) { ESB_INVOKE_API_FUNC1(String, backend0x7, GetTempFileName, Extension_); };
	// Функция ПредставлениеПрава([ИмяПрава_ Как Строка]) Возвращает Строка
	inline String RightPresentation(ConstParam<String> NameOfRight_ = DefUndef<String>) { ESB_INVOKE_API_FUNC1(String, backend0x7, RightPresentation, NameOfRight_); };
	// Процедура УдалитьДанныеИнформационнойБазы()
	inline void EraseInfoBaseData() { ESB_INVOKE_API_PROC0(backend0x7, EraseInfoBaseData); };
	// Процедура УстановитьБезопасныйРежимРазделенияДанных(ОбщийРеквизит_ Как Строка, Режим_ Как Булево)
	inline void SetDataSeparationSafeMode(ConstParam<String> CommonAttribute_, ConstParam<Boolean> Mode_) { ESB_INVOKE_API_PROC2(backend0x7, SetDataSeparationSafeMode, CommonAttribute_, Mode_); };
	// Функция БезопасныйРежимРазделенияДанных(ОбщийРеквизит_ Как Строка) Возвращает Булево
	inline Boolean DataSeparationSafeMode(ConstParam<String> CommonAttribute_) { ESB_INVOKE_API_FUNC1(Boolean, backend0x7, DataSeparationSafeMode, CommonAttribute_); };
	// Процедура ИнициализироватьПредопределенныеДанные()
	inline void InitializePredefinedData() { ESB_INVOKE_API_PROC0(backend0x7, InitializePredefinedData); };
	// Процедура УстановитьВремяЗасыпанияПассивногоСеанса([Таймаут_ Как Число])
	inline void SetPassiveSessionHibernateTime(ConstParam<Number> Timeout_ = DefUndef<Number>) { ESB_INVOKE_API_PROC1(backend0x7, SetPassiveSessionHibernateTime, Timeout_); };
	// Функция ПолучитьВремяЗасыпанияПассивногоСеанса() Возвращает Число
	inline Number GetPassiveSessionHibernateTime() { ESB_INVOKE_API_FUNC0(Number, backend0x7, GetPassiveSessionHibernateTime); };
	// Процедура УстановитьВремяЗавершенияСпящегоСеанса([Таймаут_ Как Число])
	inline void SetHibernateSessionTerminateTime(ConstParam<Number> Timeout_ = DefUndef<Number>) { ESB_INVOKE_API_PROC1(backend0x7, SetHibernateSessionTerminateTime, Timeout_); };
	// Функция ПолучитьВремяЗавершенияСпящегоСеанса() Возвращает Число
	inline Number GetHibernateSessionTerminateTime() { ESB_INVOKE_API_FUNC0(Number, backend0x7, GetHibernateSessionTerminateTime); };
	// Процедура УстановитьНастройкиКлиентаЛицензирования([Имя_ Как Строка], [Пароль_ Как Строка], [ДополнительныйПараметр_ Как Строка])
	inline void SetLicensingClientParameters(ConstParam<String> Name_ = DefUndef<String>, ConstParam<String> Password_ = DefUndef<String>, ConstParam<String> AdditionalOption_ = DefUndef<String>) { ESB_INVOKE_API_PROC3(backend0x7, SetLicensingClientParameters, Name_, Password_, AdditionalOption_); };
	// Процедура УстановитьОтключениеБезопасногоРежима([Отключить_ Как Булево])
	inline void SetSafeModeDisabled(ConstParam<Boolean> Disable_ = DefUndef<Boolean>) { ESB_INVOKE_API_PROC1(backend0x7, SetSafeModeDisabled, Disable_); };
	// Функция ПолучитьОтключениеБезопасногоРежима() Возвращает Булево
	inline Boolean GetSafeModeDisabled() { ESB_INVOKE_API_FUNC0(Boolean, backend0x7, GetSafeModeDisabled); };
#endif	// #if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
	// Void CommitTransaction()
	inline void ЗафиксироватьТранзакцию() { ESB_INVOKE_API_PROC0(backend0x7, CommitTransaction); };
	// Void RollbackTransaction()
	inline void ОтменитьТранзакцию() { ESB_INVOKE_API_PROC0(backend0x7, RollbackTransaction); };
	// Function TransactionActive() -> Boolean
	inline Булево ТранзакцияАктивна() { ESB_INVOKE_API_FUNC0(Булево, backend0x7, TransactionActive); };
	// Void Message(String MessageText_, MessageStatus Status_ = <default>)
	inline void Сообщить(КонстПарам<Строка> ТекстСообщения_, КонстПарам<СтатусСообщения> Статус_ = ПоУмолчаниюНеопределеноИли<СтатусСообщения>) { ESB_INVOKE_API_PROC2(backend0x7, Message, ТекстСообщения_, Статус_); };
	// Void SetExclusiveMode(Boolean ExclusiveMode_)
	inline void УстановитьМонопольныйРежим(КонстПарам<Булево> МонопольныйРежим_) { ESB_INVOKE_API_PROC1(backend0x7, SetExclusiveMode, МонопольныйРежим_); };
	// Function ExclusiveMode() -> Boolean
	inline Булево МонопольныйРежим() { ESB_INVOKE_API_FUNC0(Булево, backend0x7, ExclusiveMode); };
	// Function NStr(String InitialString_, String LanguageCode_ = <default>) -> String
	inline Строка НСтр(КонстПарам<Строка> ИсходнаяСтрока_, КонстПарам<Строка> КодЯзыка_ = ПоУмолчаниюНеопределеноИли<Строка>) { ESB_INVOKE_API_FUNC2(Строка, backend0x7, NStr, ИсходнаяСтрока_, КодЯзыка_); };
	// Function GetRealTimeTimestamp() -> Date
	inline ДатаВремя ПолучитьОперативнуюОтметкуВремени() { ESB_INVOKE_API_FUNC0(ДатаВремя, backend0x7, GetRealTimeTimestamp); };
	// Function ValueToStringInternal(Arbitrary Value_) -> String
	inline Строка ЗначениеВСтрокуВнутр(КонстПарам<Произвольный> Значение_) { ESB_INVOKE_API_FUNC1(Строка, backend0x7, ValueToStringInternal, Значение_); };
	// Function ValueFromStringInternal(String Line_) -> Arbitrary
	inline Произвольный ЗначениеИзСтрокиВнутр(КонстПарам<Строка> Строка_) { ESB_INVOKE_API_FUNC1(Произвольный, backend0x7, ValueFromStringInternal, Строка_); };
	// Function ValueToFile(String FileName_, Arbitrary Value_) -> Boolean
	inline Булево ЗначениеВФайл(КонстПарам<Строка> ИмяФайла_, КонстПарам<Произвольный> Значение_) { ESB_INVOKE_API_FUNC2(Булево, backend0x7, ValueToFile, ИмяФайла_, Значение_); };
	// Function ValueFromFile(String FileName_) -> Arbitrary
	inline Произвольный ЗначениеИзФайла(КонстПарам<Строка> ИмяФайла_) { ESB_INVOKE_API_FUNC1(Произвольный, backend0x7, ValueFromFile, ИмяФайла_); };
	// Function ComputerName() -> String
	inline Строка ИмяКомпьютера() { ESB_INVOKE_API_FUNC0(Строка, backend0x7, ComputerName); };
	// Function UserName() -> String
	inline Строка ИмяПользователя() { ESB_INVOKE_API_FUNC0(Строка, backend0x7, UserName); };
	// Function UserFullName() -> String
	inline Строка ПолноеИмяПользователя() { ESB_INVOKE_API_FUNC0(Строка, backend0x7, UserFullName); };
	// Function BinDir() -> String
	inline Строка КаталогПрограммы() { ESB_INVOKE_API_FUNC0(Строка, backend0x7, BinDir); };
	// Function TempFilesDir() -> String
	inline Строка КаталогВременныхФайлов() { ESB_INVOKE_API_FUNC0(Строка, backend0x7, TempFilesDir); };
	// Void BeginGettingTempFilesDir(NotifyDescription NotifyDescription_)
	inline void НачатьПолучениеКаталогаВременныхФайлов(КонстПарам<ОписаниеОповещения> ОписаниеОповещения_) { ESB_INVOKE_API_PROC1(backend0x7, BeginGettingTempFilesDir, ОписаниеОповещения_); };
	// Function CurrentLocaleCode() -> String
	inline Строка ТекущийКодЛокализации() { ESB_INVOKE_API_FUNC0(Строка, backend0x7, CurrentLocaleCode); };
	// Function InfoBaseLocaleCode() -> String
	inline Строка КодЛокализацииИнформационнойБазы() { ESB_INVOKE_API_FUNC0(Строка, backend0x7, InfoBaseLocaleCode); };
	// Function InfoBaseConnectionString() -> String
	inline Строка СтрокаСоединенияИнформационнойБазы() { ESB_INVOKE_API_FUNC0(Строка, backend0x7, InfoBaseConnectionString); };
	// Void CopyEventLog(String InputFileName_ = <default>, String OutputFileName_ = <default>, Structure Filter_)
	inline void СкопироватьЖурналРегистрации(КонстПарам<НеопределеноИли<Строка>> ИмяВходногоФайла_, КонстПарам<НеопределеноИли<Строка>> ИмяВыходногоФайла_, КонстПарам<Структура> Фильтр_) { ESB_INVOKE_API_PROC3(backend0x7, CopyEventLog, ИмяВходногоФайла_, ИмяВыходногоФайла_, Фильтр_); };
	// Void ClearEventLog(Structure Filter_)
	inline void ОчиститьЖурналРегистрации(КонстПарам<Структура> Фильтр_) { ESB_INVOKE_API_PROC1(backend0x7, ClearEventLog, Фильтр_); };
	// Function ApplicationPresentation(String ApplicationIdentifier_) -> String
	inline Строка ПредставлениеПриложения(КонстПарам<Строка> ИмяПриложения_) { ESB_INVOKE_API_FUNC1(Строка, backend0x7, ApplicationPresentation, ИмяПриложения_); };
	// Function InfoBaseConnectionNumber() -> Number
	inline Число НомерСоединенияИнформационнойБазы() { ESB_INVOKE_API_FUNC0(Число, backend0x7, InfoBaseConnectionNumber); };
	// Function InfoBaseSessionNumber() -> Number
	inline Число НомерСеансаИнформационнойБазы() { ESB_INVOKE_API_FUNC0(Число, backend0x7, InfoBaseSessionNumber); };
	// Function EventLogEventPresentation(String EventIdentifier_) -> String
	inline Строка ПредставлениеСобытияЖурналаРегистрации(КонстПарам<Строка> ИдентификаторСобытия_) { ESB_INVOKE_API_FUNC1(Строка, backend0x7, EventLogEventPresentation, ИдентификаторСобытия_); };
	// Void SetEventLogUsing(Array Levels_)
	inline void УстановитьИспользованиеЖурналаРегистрации(КонстПарам<Массив> Уровни_) { ESB_INVOKE_API_PROC1(backend0x7, SetEventLogUsing, Уровни_); };
	// Function ConfigurationChanged() -> Boolean
	inline Булево КонфигурацияИзменена() { ESB_INVOKE_API_FUNC0(Булево, backend0x7, ConfigurationChanged); };
	// Function DataBaseConfigurationChangedDynamically() -> Boolean
	inline Булево КонфигурацияБазыДанныхИзмененаДинамически() { ESB_INVOKE_API_FUNC0(Булево, backend0x7, DataBaseConfigurationChangedDynamically); };
	// Void SetLockWaitTime(Number Timeout_ = <default>)
	inline void УстановитьВремяОжиданияБлокировкиДанных(КонстПарам<Число> Таймаут_ = ПоУмолчаниюНеопределеноИли<Число>) { ESB_INVOKE_API_PROC1(backend0x7, SetLockWaitTime, Таймаут_); };
	// Function GetLockWaitTime() -> Number
	inline Число ПолучитьВремяОжиданияБлокировкиДанных() { ESB_INVOKE_API_FUNC0(Число, backend0x7, GetLockWaitTime); };
	// Void SetUserPasswordMinLength(Number Length_ = <default>)
	inline void УстановитьМинимальнуюДлинуПаролейПользователей(КонстПарам<Число> Длина_ = ПоУмолчаниюНеопределеноИли<Число>) { ESB_INVOKE_API_PROC1(backend0x7, SetUserPasswordMinLength, Длина_); };
	// Function GetUserPasswordMinLength() -> Number
	inline Число ПолучитьМинимальнуюДлинуПаролейПользователей() { ESB_INVOKE_API_FUNC0(Число, backend0x7, GetUserPasswordMinLength); };
	// Void SetUserPasswordStrengthCheck(Boolean ComplexPassword_)
	inline void УстановитьПроверкуСложностиПаролейПользователей(КонстПарам<Булево> СложныйПароль_) { ESB_INVOKE_API_PROC1(backend0x7, SetUserPasswordStrengthCheck, СложныйПароль_); };
	// Function GetUserPasswordStrengthCheck() -> Boolean
	inline Булево ПолучитьПроверкуСложностиПаролейПользователей() { ESB_INVOKE_API_FUNC0(Булево, backend0x7, GetUserPasswordStrengthCheck); };
	// Function CurrentSystemLanguage() -> String
	inline Строка ТекущийЯзыкСистемы() { ESB_INVOKE_API_FUNC0(Строка, backend0x7, CurrentSystemLanguage); };
	// Function BriefErrorDescription(ErrorInfo ErrorInfo_) -> String
	inline Строка КраткоеПредставлениеОшибки(КонстПарам<ИнформацияОбОшибке> ИнформацияОбОшибке_) { ESB_INVOKE_API_FUNC1(Строка, backend0x7, BriefErrorDescription, ИнформацияОбОшибке_); };
	// Function DetailErrorDescription(ErrorInfo ErrorInfo_) -> String
	inline Строка ПодробноеПредставлениеОшибки(КонстПарам<ИнформацияОбОшибке> ИнформацияОбОшибке_) { ESB_INVOKE_API_FUNC1(Строка, backend0x7, DetailErrorDescription, ИнформацияОбОшибке_); };
	// Void SetPrivilegedMode(Boolean PrivilegedModeOn_)
	inline void УстановитьПривилегированныйРежим(КонстПарам<Булево> Включить_) { ESB_INVOKE_API_PROC1(backend0x7, SetPrivilegedMode, Включить_); };
	// Function PrivilegedMode() -> Boolean
	inline Булево ПривилегированныйРежим() { ESB_INVOKE_API_FUNC0(Булево, backend0x7, PrivilegedMode); };
	// Void LockDataForEdit(Arbitrary Key_, String DataVersion_ = <default>, UUID FormIdentifier_ = <default>)
	inline void ЗаблокироватьДанныеДляРедактирования(КонстПарам<Произвольный> Ключ_, КонстПарам<Строка> ВерсияДанных_ = ПоУмолчаниюНеопределеноИли<Строка>, КонстПарам<УникальныйИдентификатор> ИдентификаторФормы_ = ПоУмолчаниюНеопределеноИли<УникальныйИдентификатор>) { ESB_INVOKE_API_PROC3(backend0x7, LockDataForEdit, Ключ_, ВерсияДанных_, ИдентификаторФормы_); };
	// Void RefreshReusableValues()
	inline void ОбновитьПовторноИспользуемыеЗначения() { ESB_INVOKE_API_PROC0(backend0x7, RefreshReusableValues); };
	// Function GetEventLogFilterValues(String Columns_ = <default>, String InputFileName_ = <default>) -> Structure_GETEVENTLOGFILTERVALUES
	//TODO	Структура_GETEVENTLOGFILTERVALUES
	inline Структура ПолучитьЗначенияОтбораЖурналаРегистрации(КонстПарам<Строка> Колонки_ = ПоУмолчаниюНеопределеноИли<Строка>, КонстПарам<Строка> ИмяВходногоФайла_ = ПоУмолчаниюНеопределеноИли<Строка>) { ESB_INVOKE_API_FUNC2(Структура, backend0x7, GetEventLogFilterValues, Колонки_, ИмяВходногоФайла_); };
	// Void SetSafeMode(OneOf<Boolean, String> SafeMode_)
	inline void УстановитьБезопасныйРежим(КонстПарам<Булево> БезопасныйРежим_) { ESB_INVOKE_API_PROC1(backend0x7, SetSafeMode, БезопасныйРежим_); };
	inline void УстановитьБезопасныйРежим(КонстПарам<Строка> БезопасныйРежим_) { ESB_INVOKE_API_PROC1(backend0x7, SetSafeMode, БезопасныйРежим_); };
	// Function SafeMode() -> OneOf<Boolean, String>
	inline ОдинИз<Булево, Строка> БезопасныйРежим() { ESB_INVOKE_API_FUNC0(ESB_ONEOF(Булево, Строка), backend0x7, SafeMode); };
	// Void SetInfoBaseTimeZone(String TimeZone_ = <default>)
	inline void УстановитьЧасовойПоясИнформационнойБазы(КонстПарам<Строка> ЧасовойПояс_ = ПоУмолчаниюНеопределеноИли<Строка>) { ESB_INVOKE_API_PROC1(backend0x7, SetInfoBaseTimeZone, ЧасовойПояс_); };
	// Function GetInfoBaseTimeZone() -> OneOf<String, Undefined>
	inline НеопределеноИли<Строка> ПолучитьЧасовойПоясИнформационнойБазы() { ESB_INVOKE_API_FUNC0(НеопределеноИли<Строка>, backend0x7, GetInfoBaseTimeZone); };
	// Void SetSessionTimeZone(String TimeZone_ = <default>)
	inline void УстановитьЧасовойПоясСеанса(КонстПарам<Строка> ЧасовойПояс_ = ПоУмолчаниюНеопределеноИли<Строка>) { ESB_INVOKE_API_PROC1(backend0x7, SetSessionTimeZone, ЧасовойПояс_); };
	// Function SessionTimeZone() -> String
	inline Строка ЧасовойПоясСеанса() { ESB_INVOKE_API_FUNC0(Строка, backend0x7, SessionTimeZone); };
	// Function CurrentSessionDate() -> Date
	inline ДатаВремя ТекущаяДатаСеанса() { ESB_INVOKE_API_FUNC0(ДатаВремя, backend0x7, CurrentSessionDate); };
	// Function DaylightTimeOffset(OneOf<String, Undefined> TimeZone_ = <default>, OneOf<Date, Undefined> UniversalTime_ = <default>) -> Number
	inline Число СмещениеЛетнегоВремени(КонстПарам<Строка> ЧасовойПояс_ = ПоУмолчаниюНеопределеноИли<Строка>, КонстПарам<ДатаВремя> УниверсальноеВремя_ = ПоУмолчаниюНеопределеноИли<ДатаВремя>) { ESB_INVOKE_API_FUNC2(Число, backend0x7, DaylightTimeOffset, ЧасовойПояс_, УниверсальноеВремя_); };
	// Function CurrentUniversalDate() -> Date
	inline ДатаВремя ТекущаяУниверсальнаяДата() { ESB_INVOKE_API_FUNC0(ДатаВремя, backend0x7, CurrentUniversalDate); };
	// Function ToLocalTime(Date UniversalTime_, OneOf<String, Undefined> TimeZone_ = <default>) -> Date
	inline ДатаВремя МестноеВремя(КонстПарам<ДатаВремя> УниверсальноеВремя_, КонстПарам<Строка> ЧасовойПояс_ = ПоУмолчаниюНеопределеноИли<Строка>) { ESB_INVOKE_API_FUNC2(ДатаВремя, backend0x7, ToLocalTime, УниверсальноеВремя_, ЧасовойПояс_); };
	// Function ToUniversalTime(Date LocalTime_, OneOf<String, Undefined> TimeZone_ = <default>) -> Date
	inline ДатаВремя УниверсальноеВремя(КонстПарам<ДатаВремя> МестноеВремя_, КонстПарам<Строка> ЧасовойПояс_ = ПоУмолчаниюНеопределеноИли<Строка>) { ESB_INVOKE_API_FUNC2(ДатаВремя, backend0x7, ToUniversalTime, МестноеВремя_, ЧасовойПояс_); };
	// Function TimeZone() -> String
	inline Строка ЧасовойПояс() { ESB_INVOKE_API_FUNC0(Строка, backend0x7, TimeZone); };
	// Function GetAvailableTimeZones() -> ArrayOf<String>
	inline МассивИз<Строка> ПолучитьДопустимыеЧасовыеПояса() { ESB_INVOKE_API_FUNC0(МассивИз<Строка>, backend0x7, GetAvailableTimeZones); };
	// Function TimeZonePresentation(String TimeZone_) -> String
	inline Строка ПредставлениеЧасовогоПояса(КонстПарам<Строка> ЧасовойПояс_) { ESB_INVOKE_API_FUNC1(Строка, backend0x7, TimeZonePresentation, ЧасовойПояс_); };
	// Function GetAvailableLocaleCodes() -> ArrayOf<String>
	inline МассивИз<Строка> ПолучитьДопустимыеКодыЛокализации() { ESB_INVOKE_API_FUNC0(МассивИз<Строка>, backend0x7, GetAvailableLocaleCodes); };
	// Function LocaleCodePresentation(String LocalizationCode_) -> String
	inline Строка ПредставлениеКодаЛокализации(КонстПарам<Строка> КодЛокализации_) { ESB_INVOKE_API_FUNC1(Строка, backend0x7, LocaleCodePresentation, КодЛокализации_); };
	// Function StandardTimeOffset(OneOf<String, Undefined> TimeZone_ = <default>, OneOf<Date, Undefined> UniversalTime_ = <default>) -> Date
	inline ДатаВремя СмещениеСтандартногоВремени(КонстПарам<Строка> ЧасовойПояс_ = ПоУмолчаниюНеопределеноИли<Строка>, КонстПарам<ДатаВремя> УниверсальноеВремя_ = ПоУмолчаниюНеопределеноИли<ДатаВремя>) { ESB_INVOKE_API_FUNC2(ДатаВремя, backend0x7, StandardTimeOffset, ЧасовойПояс_, УниверсальноеВремя_); };
	// Function GetTempFileName(String Extension_ = <default>) -> String
	inline Строка ПолучитьИмяВременногоФайла(КонстПарам<Строка> Расширение_ = ПоУмолчаниюНеопределеноИли<Строка>) { ESB_INVOKE_API_FUNC1(Строка, backend0x7, GetTempFileName, Расширение_); };
	// Function RightPresentation(String NameOfRight_ = <default>) -> String
	inline Строка ПредставлениеПрава(КонстПарам<Строка> ИмяПрава_ = ПоУмолчаниюНеопределеноИли<Строка>) { ESB_INVOKE_API_FUNC1(Строка, backend0x7, RightPresentation, ИмяПрава_); };
	// Void EraseInfoBaseData()
	inline void УдалитьДанныеИнформационнойБазы() { ESB_INVOKE_API_PROC0(backend0x7, EraseInfoBaseData); };
	// Void SetDataSeparationSafeMode(String CommonAttribute_, Boolean Mode_)
	inline void УстановитьБезопасныйРежимРазделенияДанных(КонстПарам<Строка> ОбщийРеквизит_, КонстПарам<Булево> Режим_) { ESB_INVOKE_API_PROC2(backend0x7, SetDataSeparationSafeMode, ОбщийРеквизит_, Режим_); };
	// Function DataSeparationSafeMode(String CommonAttribute_) -> Boolean
	inline Булево БезопасныйРежимРазделенияДанных(КонстПарам<Строка> ОбщийРеквизит_) { ESB_INVOKE_API_FUNC1(Булево, backend0x7, DataSeparationSafeMode, ОбщийРеквизит_); };
	// Void InitializePredefinedData()
	inline void ИнициализироватьПредопределенныеДанные() { ESB_INVOKE_API_PROC0(backend0x7, InitializePredefinedData); };
	// Void SetPassiveSessionHibernateTime(Number Timeout_ = <default>)
	inline void УстановитьВремяЗасыпанияПассивногоСеанса(КонстПарам<Число> Таймаут_ = ПоУмолчаниюНеопределеноИли<Число>) { ESB_INVOKE_API_PROC1(backend0x7, SetPassiveSessionHibernateTime, Таймаут_); };
	// Function GetPassiveSessionHibernateTime() -> Number
	inline Число ПолучитьВремяЗасыпанияПассивногоСеанса() { ESB_INVOKE_API_FUNC0(Число, backend0x7, GetPassiveSessionHibernateTime); };
	// Void SetHibernateSessionTerminateTime(Number Timeout_ = <default>)
	inline void УстановитьВремяЗавершенияСпящегоСеанса(КонстПарам<Число> Таймаут_ = ПоУмолчаниюНеопределеноИли<Число>) { ESB_INVOKE_API_PROC1(backend0x7, SetHibernateSessionTerminateTime, Таймаут_); };
	// Function GetHibernateSessionTerminateTime() -> Number
	inline Число ПолучитьВремяЗавершенияСпящегоСеанса() { ESB_INVOKE_API_FUNC0(Число, backend0x7, GetHibernateSessionTerminateTime); };
	// Void SetLicensingClientParameters(String Name_ = <default>, String Password_ = <default>, String AdditionalOption_ = <default>)
	inline void УстановитьНастройкиКлиентаЛицензирования(КонстПарам<Строка> Имя_ = ПоУмолчаниюНеопределеноИли<Строка>, КонстПарам<Строка> Пароль_ = ПоУмолчаниюНеопределеноИли<Строка>, КонстПарам<Строка> ДополнительныйПараметр_ = ПоУмолчаниюНеопределеноИли<Строка>) { ESB_INVOKE_API_PROC3(backend0x7, SetLicensingClientParameters, Имя_, Пароль_, ДополнительныйПараметр_); };
	// Void SetSafeModeDisabled(Boolean Disable_ = <default>)
	inline void УстановитьОтключениеБезопасногоРежима(КонстПарам<Булево> Отключить_ = ПоУмолчаниюНеопределеноИли<Булево>) { ESB_INVOKE_API_PROC1(backend0x7, SetSafeModeDisabled, Отключить_); };
	// Function GetSafeModeDisabled() -> Boolean
	inline Булево ПолучитьОтключениеБезопасногоРежима() { ESB_INVOKE_API_FUNC0(Булево, backend0x7, GetSafeModeDisabled); };
#endif	// #if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
#endif	// #if ESB_API_PROVIDER_INCLUDE(backend0x7)
#if ESB_API_PROVIDER_INCLUDE(addin)
#if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
	// Функция ПодключитьВнешнююКомпоненту(Местоположение_ Как Строка, Имя_ Как Строка, [Тип_ Как ТипВнешнейКомпоненты]) Возвращает Булево
	inline Boolean AttachAddIn(ConstParam<String> Location_, ConstParam<String> Name_, ConstParam<AddInType> type_ = DefUndef<AddInType>) { ESB_INVOKE_API_FUNC3(Boolean, addin, AttachAddIn, Location_, Name_, type_); };
	// Функция ПодключитьВнешнююКомпоненту(Местоположение_ Как Строка, Имя_ Как Строка, [Тип_ Как ТипВнешнейКомпоненты]) Возвращает Булево
	inline Boolean AttachAddIn(ConstParam<String> ObjectIdentificator_) { ESB_INVOKE_API_FUNC3(Boolean, addin, AttachAddIn, ObjectIdentificator_, ESB_DEFARG(Undefined), ESB_DEFARG(Undefined)); };
	// Процедура ЗагрузитьВнешнююКомпоненту(ИмяФайла_ Как Строка)
	inline void LoadAddIn(ConstParam<String> FileName_) { ESB_INVOKE_API_PROC1(addin, LoadAddIn, FileName_); };
	// Процедура НачатьПодключениеВнешнейКомпоненты([ОписаниеОповещения_ Как ОписаниеОповещения], [ИдентификаторОбъекта_ Как Строка])
	inline void BeginAttachingAddIn(ConstParam<NotifyDescription> NotifyDescription_ = DefUndef<NotifyDescription>, ConstParam<String> ObjectId_ = DefUndef<String>) { ESB_INVOKE_API_PROC4(addin, BeginAttachingAddIn, NotifyDescription_, ObjectId_, ESB_DEFARG(Undefined), ESB_DEFARG(Undefined)); };
	// Процедура НачатьПодключениеВнешнейКомпоненты([ОписаниеОповещения_ Как ОписаниеОповещения], [ИдентификаторОбъекта_ Как Строка])
	inline void BeginAttachingAddIn(ConstParam<NotifyDescription> NotifyDescription_, ConstParam<String> Location_, ConstParam<String> Name_, ConstParam<AddInType> type_ = DefUndef<AddInType>) { ESB_INVOKE_API_PROC4(addin, BeginAttachingAddIn, NotifyDescription_, Location_, Name_, type_); };
#endif	// #if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
	// Function AttachAddIn(String Location_, String Name_, AddInType Type_ = <default>) -> Boolean
	inline Булево ПодключитьВнешнююКомпоненту(КонстПарам<Строка> Местоположение_, КонстПарам<Строка> Имя_, КонстПарам<ТипВнешнейКомпоненты> Тип_ = ПоУмолчаниюНеопределеноИли<ТипВнешнейКомпоненты>) { ESB_INVOKE_API_FUNC3(Булево, addin, AttachAddIn, Местоположение_, Имя_, Тип_); };
	// Function AttachAddIn(String Location_, String Name_, AddInType Type_ = <default>) -> Boolean
	inline Булево ПодключитьВнешнююКомпоненту(КонстПарам<Строка> ИдентификаторОбъекта_) { ESB_INVOKE_API_FUNC3(Булево, addin, AttachAddIn, ИдентификаторОбъекта_, ESB_DEFARG(Неопределено), ESB_DEFARG(Неопределено)); };
	// Void LoadAddIn(String FileName_)
	inline void ЗагрузитьВнешнююКомпоненту(КонстПарам<Строка> ИмяФайла_) { ESB_INVOKE_API_PROC1(addin, LoadAddIn, ИмяФайла_); };
	// Void BeginAttachingAddIn(NotifyDescription NotifyDescription_ = <default>, String ObjectId_ = <default>)
	inline void НачатьПодключениеВнешнейКомпоненты(КонстПарам<ОписаниеОповещения> ОписаниеОповещения_ = ПоУмолчаниюНеопределеноИли<ОписаниеОповещения>, КонстПарам<Строка> ИдентификаторОбъекта_ = ПоУмолчаниюНеопределеноИли<Строка>) { ESB_INVOKE_API_PROC4(addin, BeginAttachingAddIn, ОписаниеОповещения_, ИдентификаторОбъекта_, ESB_DEFARG(Неопределено), ESB_DEFARG(Неопределено)); };
	// Void BeginAttachingAddIn(NotifyDescription NotifyDescription_ = <default>, String ObjectId_ = <default>)
	inline void НачатьПодключениеВнешнейКомпоненты(КонстПарам<ОписаниеОповещения> ОписаниеОповещения_, КонстПарам<Строка> Местоположение_, КонстПарам<Строка> Имя_, КонстПарам<ТипВнешнейКомпоненты> Тип_ = ПоУмолчаниюНеопределеноИли<ТипВнешнейКомпоненты>) { ESB_INVOKE_API_PROC4(addin, BeginAttachingAddIn, ОписаниеОповещения_, Местоположение_, Имя_, Тип_); };
#endif	// #if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
#endif	// #if ESB_API_PROVIDER_INCLUDE(addin)
#if ESB_API_PROVIDER_INCLUDE(entext0x1)
#if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
	// Функция КодироватьСтроку(Строка_ Как Строка, СпособКодированияСтроки_ Как СпособКодированияСтроки, [КодировкаСтроки_ Как Строка]) Возвращает Строка
	inline String EncodeString(ConstParam<String> String_, ConstParam<StringEncodingMethod> StringEncodingMethod_, ConstParam<String> StringEncoding_ = DefUndef<String>) { ESB_INVOKE_API_FUNC3(String, entext0x1, EncodeString, String_, StringEncodingMethod_, StringEncoding_); };
	// Функция РаскодироватьСтроку(Строка_ Как Строка, СпособКодированияСтроки_ Как СпособКодированияСтроки, [КодировкаСтроки_ Как Строка]) Возвращает Строка
	inline String DecodeString(ConstParam<String> String_, ConstParam<StringEncodingMethod> StringEncodingMethod_, ConstParam<String> StringEncoding_ = DefUndef<String>) { ESB_INVOKE_API_FUNC3(String, entext0x1, DecodeString, String_, StringEncodingMethod_, StringEncoding_); };
	// Функция ПолучитьМаскуВсеФайлыСервера() Возвращает Строка
	inline String GetServerAllFilesMask() { ESB_INVOKE_API_FUNC0(String, entext0x1, GetServerAllFilesMask); };
	// Функция ПолучитьРазделительПутиСервера() Возвращает Строка
	inline String GetServerPathSeparator() { ESB_INVOKE_API_FUNC0(String, entext0x1, GetServerPathSeparator); };
#endif	// #if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
	// Function EncodeString(String String_, StringEncodingMethod StringEncodingMethod_, String StringEncoding_ = <default>) -> String
	inline Строка КодироватьСтроку(КонстПарам<Строка> Строка_, КонстПарам<СпособКодированияСтроки> СпособКодированияСтроки_, КонстПарам<Строка> КодировкаСтроки_ = ПоУмолчаниюНеопределеноИли<Строка>) { ESB_INVOKE_API_FUNC3(Строка, entext0x1, EncodeString, Строка_, СпособКодированияСтроки_, КодировкаСтроки_); };
	// Function DecodeString(String String_, StringEncodingMethod StringEncodingMethod_, String StringEncoding_ = <default>) -> String
	inline Строка РаскодироватьСтроку(КонстПарам<Строка> Строка_, КонстПарам<СпособКодированияСтроки> СпособКодированияСтроки_, КонстПарам<Строка> КодировкаСтроки_ = ПоУмолчаниюНеопределеноИли<Строка>) { ESB_INVOKE_API_FUNC3(Строка, entext0x1, DecodeString, Строка_, СпособКодированияСтроки_, КодировкаСтроки_); };
	// Function GetServerAllFilesMask() -> String
	inline Строка ПолучитьМаскуВсеФайлыСервера() { ESB_INVOKE_API_FUNC0(Строка, entext0x1, GetServerAllFilesMask); };
	// Function GetServerPathSeparator() -> String
	inline Строка ПолучитьРазделительПутиСервера() { ESB_INVOKE_API_FUNC0(Строка, entext0x1, GetServerPathSeparator); };
#endif	// #if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
#endif	// #if ESB_API_PROVIDER_INCLUDE(entext0x1)
#if ESB_API_PROVIDER_INCLUDE(mngui0x7)
#if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
	// Процедура ПоказатьОповещениеПользователя([Текст_ Как Строка], [НавигационнаяСсылка_ Как Строка], [Пояснение_ Как Строка], [Картинка_ Как Картинка])
	inline void ShowUserNotification(ConstParam<String> Text_ = DefUndef<String>, ConstParam<String> NavigationLink_ = DefUndef<String>, ConstParam<String> Explanation_ = DefUndef<String>, ConstParam<Picture> Picture_ = DefUndef<Picture>) { ESB_INVOKE_API_PROC4(mngui0x7, ShowUserNotification, Text_, NavigationLink_, Explanation_, Picture_); };
	// Функция ПолучитьФайл(Адрес_ Как Строка, [ИмяФайла_ Как Строка], [Интерактивно_ Как Булево]) Возвращает ОдинИз<Булево, Неопределено>
	inline UndefOr<Boolean> GetFile(ConstParam<String> Address_, ConstParam<String> FileName_ = DefUndef<String>, ConstParam<Boolean> Interactively_ = DefUndef<Boolean>) { ESB_INVOKE_API_FUNC3(UndefOr<Boolean>, mngui0x7, GetFile, Address_, FileName_, Interactively_); };
	// Функция ПоместитьФайл([Адрес_ Как Строка], [НачальноеИмяФайла_ Как Строка], [ВыбранноеИмяФайла_ Как Строка], [Интерактивно_ Как Булево], [УникальныйИдентификаторФормы_ Как УникальныйИдентификатор]) Возвращает Булево
	inline Boolean PutFile(ConstParam<String> Address_ = DefUndef<String>, ConstParam<String> FileName_ = DefUndef<String>, ConstParam<String> SelectedFileName_ = DefUndef<String>, ConstParam<Boolean> Interactively_ = DefUndef<Boolean>, ConstParam<UUIDValue> FormUniqueID_ = DefUndef<UUIDValue>) { ESB_INVOKE_API_FUNC5(Boolean, mngui0x7, PutFile, Address_, FileName_, SelectedFileName_, Interactively_, FormUniqueID_); };
	// Функция НачатьПомещениеФайла([ОписаниеОповещенияОЗавершении_ Как ОписаниеОповещения], [Адрес_ Как Строка], [НачальноеИмяФайла_ Как Строка], [Интерактивно_ Как Булево], [УникальныйИдентификаторФормы_ Как УникальныйИдентификатор]) Возвращает Булево
	//!> the method is actually a procedure according to disp-information.
	// Функция ПолучитьФункциональнуюОпциюИнтерфейса(Имя_ Как Строка) Возвращает Произвольный
	inline Arbitrary GetInterfaceFunctionalOption(ConstParam<String> Name_) { ESB_INVOKE_API_FUNC1(Arbitrary, mngui0x7, GetInterfaceFunctionalOption, Name_); };
	// Процедура УстановитьПараметрыФункциональныхОпцийИнтерфейса(УстанавливаемыеПараметры_ Как Структура, [СбрасываемыеПараметры_ Как ОдинИз<Массив, Строка>])
	inline void SetInterfaceFunctionalOptionParameters(ConstParam<Structure> ParametersToBeSpecified_, ConstParam<String> ParametersToBeReset_ = DefUndef<String>) { ESB_INVOKE_API_PROC2(mngui0x7, SetInterfaceFunctionalOptionParameters, ParametersToBeSpecified_, ParametersToBeReset_); };
	inline void SetInterfaceFunctionalOptionParameters(ConstParam<Structure> ParametersToBeSpecified_, ConstParam<Array> ParametersToBeReset_ = DefUndef<Array>) { ESB_INVOKE_API_PROC2(mngui0x7, SetInterfaceFunctionalOptionParameters, ParametersToBeSpecified_, ParametersToBeReset_); };
	// Функция ПолучитьПараметрыФункциональныхОпцийИнтерфейса() Возвращает Структура
	inline Structure GetInterfaceFunctionalOptionParameters() { ESB_INVOKE_API_FUNC0(Structure, mngui0x7, GetInterfaceFunctionalOptionParameters); };
	// Процедура ОбновитьИнтерфейс()
	inline void RefreshInterface() { ESB_INVOKE_API_PROC0(mngui0x7, RefreshInterface); };
	// Процедура УстановитьКраткийЗаголовокПриложения(КраткийЗаголовок_ Как Строка)
	inline void SetShortApplicationCaption(ConstParam<String> ShortCaption_) { ESB_INVOKE_API_PROC1(mngui0x7, SetShortApplicationCaption, ShortCaption_); };
	// Функция ПолучитьКраткийЗаголовокПриложения() Возвращает Строка
	inline String GetShortApplicationCaption() { ESB_INVOKE_API_FUNC0(String, mngui0x7, GetShortApplicationCaption); };
	// Процедура ПерейтиПоНавигационнойСсылке(НавигационнаяСсылка_ Как Строка)
	inline void GotoURL(ConstParam<String> NavigationLink_) { ESB_INVOKE_API_PROC1(mngui0x7, GotoURL, NavigationLink_); };
	// Процедура ВыполнитьОбработкуОповещения(ВыполняемоеОповещение_ Как ОписаниеОповещения, [Результат_ Как Произвольный])
	inline void ExecuteNotifyProcessing(ConstParam<NotifyDescription> NotificationPerformed_, ConstParam<Arbitrary> Result_ = DefUndef<Arbitrary>) { ESB_INVOKE_API_PROC2(mngui0x7, ExecuteNotifyProcessing, NotificationPerformed_, Result_); };
	// Процедура КомандаСистемы(СтрокаКоманды_ Как Строка, [ТекущийКаталог_ Как Строка])
	inline void System(ConstParam<String> CommandString_, ConstParam<String> CurrentDirectory_ = DefUndef<String>) { ESB_INVOKE_API_PROC2(mngui0x7, System, CommandString_, CurrentDirectory_); };
	// Функция ПодключитьРасширениеРаботыСФайлами() Возвращает Булево
	inline Boolean AttachFileSystemExtension() { ESB_INVOKE_API_FUNC0(Boolean, mngui0x7, AttachFileSystemExtension); };
	// Процедура НачатьПодключениеРасширенияРаботыСФайлами(ОписаниеОповещения_ Как ОписаниеОповещения)
	inline void BeginAttachingFileSystemExtension(ConstParam<NotifyDescription> NotifyDescription_) { ESB_INVOKE_API_PROC1(mngui0x7, BeginAttachingFileSystemExtension, NotifyDescription_); };
	// Процедура УстановитьРасширениеРаботыСФайлами()
	inline void InstallFileSystemExtension() { ESB_INVOKE_API_PROC0(mngui0x7, InstallFileSystemExtension); };
	// Процедура НачатьУстановкуРасширенияРаботыСФайлами([ОписаниеОповещенияОЗавершении_ Как ОписаниеОповещения])
	inline void BeginInstallFileSystemExtension(ConstParam<NotifyDescription> NotifyDescriptionOnCompletion_ = DefUndef<NotifyDescription>) { ESB_INVOKE_API_PROC1(mngui0x7, BeginInstallFileSystemExtension, NotifyDescriptionOnCompletion_); };
	// Процедура УстановитьВнешнююКомпоненту(Местоположение_ Как Строка)
	inline void InstallAddIn(ConstParam<String> Location_) { ESB_INVOKE_API_PROC1(mngui0x7, InstallAddIn, Location_); };
	// Процедура НачатьУстановкуВнешнейКомпоненты([ОписаниеОповещенияОЗавершении_ Как ОписаниеОповещения], Местоположение_ Как Строка)
	inline void BeginInstallAddIn(ConstParam<UndefOr<NotifyDescription>> NotifyDescriptionOnCompletion_, ConstParam<String> Location_) { ESB_INVOKE_API_PROC2(mngui0x7, BeginInstallAddIn, NotifyDescriptionOnCompletion_, Location_); };
	// Функция ЗапроситьРазрешениеПользователя(Вызовы_ Как Массив) Возвращает Булево
	inline Boolean RequestUserPermission(ConstParam<Array> Calls_) { ESB_INVOKE_API_FUNC1(Boolean, mngui0x7, RequestUserPermission, Calls_); };
	// Процедура НачатьЗапросРазрешенияПользователя(ОписаниеОповещения_ Как ОписаниеОповещения, Вызовы_ Как Массив)
	inline void BeginRequestingUserPermission(ConstParam<NotifyDescription> NotifyDescription_, ConstParam<Array> Calls_) { ESB_INVOKE_API_PROC2(mngui0x7, BeginRequestingUserPermission, NotifyDescription_, Calls_); };
	// Функция ПолучитьЗаголовокКлиентскогоПриложения() Возвращает Строка
	inline String GetClientApplicationCaption() { ESB_INVOKE_API_FUNC0(String, mngui0x7, GetClientApplicationCaption); };
	// Процедура УстановитьЗаголовокКлиентскогоПриложения(ПредставлениеКлиентскогоПриложения_ Как Строка)
	inline void SetClientApplicationCaption(ConstParam<String> ClientApplicationPresentation_) { ESB_INVOKE_API_PROC1(mngui0x7, SetClientApplicationCaption, ClientApplicationPresentation_); };
#endif	// #if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
	// Void ShowUserNotification(String Text_ = <default>, String NavigationLink_ = <default>, String Explanation_ = <default>, Picture Picture_ = <default>)
	inline void ПоказатьОповещениеПользователя(КонстПарам<Строка> Текст_ = ПоУмолчаниюНеопределеноИли<Строка>, КонстПарам<Строка> НавигационнаяСсылка_ = ПоУмолчаниюНеопределеноИли<Строка>, КонстПарам<Строка> Пояснение_ = ПоУмолчаниюНеопределеноИли<Строка>, КонстПарам<Картинка> Картинка_ = ПоУмолчаниюНеопределеноИли<Картинка>) { ESB_INVOKE_API_PROC4(mngui0x7, ShowUserNotification, Текст_, НавигационнаяСсылка_, Пояснение_, Картинка_); };
	// Function GetFile(String Address_, String FileName_ = <default>, Boolean Interactively_ = <default>) -> OneOf<Boolean, Undefined>
	inline НеопределеноИли<Булево> ПолучитьФайл(КонстПарам<Строка> Адрес_, КонстПарам<Строка> ИмяФайла_ = ПоУмолчаниюНеопределеноИли<Строка>, КонстПарам<Булево> Интерактивно_ = ПоУмолчаниюНеопределеноИли<Булево>) { ESB_INVOKE_API_FUNC3(НеопределеноИли<Булево>, mngui0x7, GetFile, Адрес_, ИмяФайла_, Интерактивно_); };
	// Function PutFile(String Address_ = <default>, String FileName_ = <default>, String SelectedFileName_ = <default>, Boolean Interactively_ = <default>, UUID FormUniqueID_ = <default>) -> Boolean
	inline Булево ПоместитьФайл(КонстПарам<Строка> Адрес_ = ПоУмолчаниюНеопределеноИли<Строка>, КонстПарам<Строка> НачальноеИмяФайла_ = ПоУмолчаниюНеопределеноИли<Строка>, КонстПарам<Строка> ВыбранноеИмяФайла_ = ПоУмолчаниюНеопределеноИли<Строка>, КонстПарам<Булево> Интерактивно_ = ПоУмолчаниюНеопределеноИли<Булево>, КонстПарам<УникальныйИдентификатор> УникальныйИдентификаторФормы_ = ПоУмолчаниюНеопределеноИли<УникальныйИдентификатор>) { ESB_INVOKE_API_FUNC5(Булево, mngui0x7, PutFile, Адрес_, НачальноеИмяФайла_, ВыбранноеИмяФайла_, Интерактивно_, УникальныйИдентификаторФормы_); };
	// Function BeginPutFile(NotifyDescription NotifyDescriptionOnCompletion_ = <default>, String Address_ = <default>, String FileName_ = <default>, Boolean Interactively_ = <default>, UUID FormUniqueID_ = <default>) -> Boolean
	//!> the method is actually a procedure according to disp-information.
	// Function GetInterfaceFunctionalOption(String Name_) -> Arbitrary
	inline Произвольный ПолучитьФункциональнуюОпциюИнтерфейса(КонстПарам<Строка> Имя_) { ESB_INVOKE_API_FUNC1(Произвольный, mngui0x7, GetInterfaceFunctionalOption, Имя_); };
	// Void SetInterfaceFunctionalOptionParameters(Structure ParametersToBeSpecified_, OneOf<Array, String> ParametersToBeReset_ = <default>)
	inline void УстановитьПараметрыФункциональныхОпцийИнтерфейса(КонстПарам<Структура> УстанавливаемыеПараметры_, КонстПарам<Строка> СбрасываемыеПараметры_ = ПоУмолчаниюНеопределеноИли<Строка>) { ESB_INVOKE_API_PROC2(mngui0x7, SetInterfaceFunctionalOptionParameters, УстанавливаемыеПараметры_, СбрасываемыеПараметры_); };
	inline void УстановитьПараметрыФункциональныхОпцийИнтерфейса(КонстПарам<Структура> УстанавливаемыеПараметры_, КонстПарам<Массив> СбрасываемыеПараметры_ = ПоУмолчаниюНеопределеноИли<Массив>) { ESB_INVOKE_API_PROC2(mngui0x7, SetInterfaceFunctionalOptionParameters, УстанавливаемыеПараметры_, СбрасываемыеПараметры_); };
	// Function GetInterfaceFunctionalOptionParameters() -> Structure
	inline Структура ПолучитьПараметрыФункциональныхОпцийИнтерфейса() { ESB_INVOKE_API_FUNC0(Структура, mngui0x7, GetInterfaceFunctionalOptionParameters); };
	// Void RefreshInterface()
	inline void ОбновитьИнтерфейс() { ESB_INVOKE_API_PROC0(mngui0x7, RefreshInterface); };
	// Void SetShortApplicationCaption(String ShortCaption_)
	inline void УстановитьКраткийЗаголовокПриложения(КонстПарам<Строка> КраткийЗаголовок_) { ESB_INVOKE_API_PROC1(mngui0x7, SetShortApplicationCaption, КраткийЗаголовок_); };
	// Function GetShortApplicationCaption() -> String
	inline Строка ПолучитьКраткийЗаголовокПриложения() { ESB_INVOKE_API_FUNC0(Строка, mngui0x7, GetShortApplicationCaption); };
	// Void GotoURL(String NavigationLink_)
	inline void ПерейтиПоНавигационнойСсылке(КонстПарам<Строка> НавигационнаяСсылка_) { ESB_INVOKE_API_PROC1(mngui0x7, GotoURL, НавигационнаяСсылка_); };
	// Void ExecuteNotifyProcessing(NotifyDescription NotificationPerformed_, Arbitrary Result_ = <default>)
	inline void ВыполнитьОбработкуОповещения(КонстПарам<ОписаниеОповещения> ВыполняемоеОповещение_, КонстПарам<Произвольный> Результат_ = ПоУмолчаниюНеопределеноИли<Произвольный>) { ESB_INVOKE_API_PROC2(mngui0x7, ExecuteNotifyProcessing, ВыполняемоеОповещение_, Результат_); };
	// Void System(String CommandString_, String CurrentDirectory_ = <default>)
	inline void КомандаСистемы(КонстПарам<Строка> СтрокаКоманды_, КонстПарам<Строка> ТекущийКаталог_ = ПоУмолчаниюНеопределеноИли<Строка>) { ESB_INVOKE_API_PROC2(mngui0x7, System, СтрокаКоманды_, ТекущийКаталог_); };
	// Function AttachFileSystemExtension() -> Boolean
	inline Булево ПодключитьРасширениеРаботыСФайлами() { ESB_INVOKE_API_FUNC0(Булево, mngui0x7, AttachFileSystemExtension); };
	// Void BeginAttachingFileSystemExtension(NotifyDescription NotifyDescription_)
	inline void НачатьПодключениеРасширенияРаботыСФайлами(КонстПарам<ОписаниеОповещения> ОписаниеОповещения_) { ESB_INVOKE_API_PROC1(mngui0x7, BeginAttachingFileSystemExtension, ОписаниеОповещения_); };
	// Void InstallFileSystemExtension()
	inline void УстановитьРасширениеРаботыСФайлами() { ESB_INVOKE_API_PROC0(mngui0x7, InstallFileSystemExtension); };
	// Void BeginInstallFileSystemExtension(NotifyDescription NotifyDescriptionOnCompletion_ = <default>)
	inline void НачатьУстановкуРасширенияРаботыСФайлами(КонстПарам<ОписаниеОповещения> ОписаниеОповещенияОЗавершении_ = ПоУмолчаниюНеопределеноИли<ОписаниеОповещения>) { ESB_INVOKE_API_PROC1(mngui0x7, BeginInstallFileSystemExtension, ОписаниеОповещенияОЗавершении_); };
	// Void InstallAddIn(String Location_)
	inline void УстановитьВнешнююКомпоненту(КонстПарам<Строка> Местоположение_) { ESB_INVOKE_API_PROC1(mngui0x7, InstallAddIn, Местоположение_); };
	// Void BeginInstallAddIn(NotifyDescription NotifyDescriptionOnCompletion_ = <default>, String Location_)
	inline void НачатьУстановкуВнешнейКомпоненты(КонстПарам<НеопределеноИли<ОписаниеОповещения>> ОписаниеОповещенияОЗавершении_, КонстПарам<Строка> Местоположение_) { ESB_INVOKE_API_PROC2(mngui0x7, BeginInstallAddIn, ОписаниеОповещенияОЗавершении_, Местоположение_); };
	// Function RequestUserPermission(Array Calls_) -> Boolean
	inline Булево ЗапроситьРазрешениеПользователя(КонстПарам<Массив> Вызовы_) { ESB_INVOKE_API_FUNC1(Булево, mngui0x7, RequestUserPermission, Вызовы_); };
	// Void BeginRequestingUserPermission(NotifyDescription NotifyDescription_, Array Calls_)
	inline void НачатьЗапросРазрешенияПользователя(КонстПарам<ОписаниеОповещения> ОписаниеОповещения_, КонстПарам<Массив> Вызовы_) { ESB_INVOKE_API_PROC2(mngui0x7, BeginRequestingUserPermission, ОписаниеОповещения_, Вызовы_); };
	// Function GetClientApplicationCaption() -> String
	inline Строка ПолучитьЗаголовокКлиентскогоПриложения() { ESB_INVOKE_API_FUNC0(Строка, mngui0x7, GetClientApplicationCaption); };
	// Void SetClientApplicationCaption(String ClientApplicationPresentation_)
	inline void УстановитьЗаголовокКлиентскогоПриложения(КонстПарам<Строка> ПредставлениеКлиентскогоПриложения_) { ESB_INVOKE_API_PROC1(mngui0x7, SetClientApplicationCaption, ПредставлениеКлиентскогоПриложения_); };
#endif	// #if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
#endif	// #if ESB_API_PROVIDER_INCLUDE(mngui0x7)

	
}	// namespace esb (api part)


#endif	// ESB_ES1_API_H_