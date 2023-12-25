/*
	"ESB" is designed to create external native components for the 1C(tm) program.
	Copyright © 2023 IntUnsigned	(v8classes@gmail.com)
	"ESB" is free software under LGPLv2.1 license with essential restriction about binary part (view esb-license.txt)
	"ESB" IS DISTRIBUTED "AS-IS" WITHOUT ANY, EVEN THE IMPLIED, WARRANTY. IN NO EVENT SHALL THE AUTHORS OR THE COPYRIGHT OWNER BE LIABLE FOR ANY CLAIM, 
	DAMAGES OR OTHER LIABILITY ARISING OUT OF THE USE OF THIS SOFTWARE.
	THE AUTHOR DOES NOT GIVE ANY GUARANTEES AND DOES NOT BEAR ANY RESPONSIBILITY REGARDING THE USE OF 1C(tm) COMPONENTS PRODUCED USING THE ESB LIBRARY IN THE 1C(tm) ENVIRONMENT.
*/


#pragma once
#ifndef ESB_ES1_FILE_H_
#define ESB_ES1_FILE_H_

#include "es1_main.h"
#include "esdisp_file.h"


#if ESB_VER < ESB_VER_v8320
namespace esb {
	namespace _internal {
		extern size_t	IBinaryDataBufferBytes_GetSize(esb::IIxCollectionRW& ibuffer_);
		extern uint8_t	IBinaryDataBufferBytes_GetByte(esb::IIxCollectionRW& ibuffer_, size_t at_);
		extern void		IBinaryDataBufferBytes_SetByte(esb::IIxCollectionRW& ibuffer_, size_t at_, uint8_t value_);
	}
}
#endif


namespace esb 
{

class PositionInStream;
class ByteOrder;
class FileOpenMode;
class FileAccess;
class Stream;
class BinaryData;
class BinaryDataBuffer;
class FileStream;
class MemoryStream;


#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
using ПозицияВПотоке = PositionInStream;
using ПорядокБайтов = ByteOrder;
using РежимОткрытияФайла = FileOpenMode;
using ДоступКФайлу = FileAccess;
using Поток = Stream;
using ДвоичныеДанные = BinaryData;
using БуферДвоичныхДанных = BinaryDataBuffer;
using ФайловыйПоток = FileStream;
using ПотокВПамяти = MemoryStream;
#endif	// #if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN


ESB_CLASS_ENUM(PositionInStream)
{	ESB_CLASS_IMPLEMENT_ENUM(PositionInStream)
public:
	ESB_CLASS_ENUM_ITEM(Begin, Начало)
	ESB_CLASS_ENUM_ITEM(Current, Текущая)
	ESB_CLASS_ENUM_ITEM(End, Конец)
};


ESB_CLASS_ENUM(ByteOrder)
{	ESB_CLASS_IMPLEMENT_ENUM(ByteOrder)
public:
	ESB_CLASS_ENUM_ITEM(LittleEndian)
	ESB_CLASS_ENUM_ITEM(BigEndian)
};


ESB_CLASS_ENUM(FileOpenMode)
{	ESB_CLASS_IMPLEMENT_ENUM(FileOpenMode)
public:
	ESB_CLASS_ENUM_ITEM(Create, Создать)
	ESB_CLASS_ENUM_ITEM(CreateNew, СоздатьНовый)
	ESB_CLASS_ENUM_ITEM(Open, Открыть)
	ESB_CLASS_ENUM_ITEM(OpenOrCreate, ОткрытьИлиСоздать)
	ESB_CLASS_ENUM_ITEM(Truncate, Обрезать)
	ESB_CLASS_ENUM_ITEM(Append, Дописать)
};


ESB_CLASS_ENUM(FileAccess)
{	ESB_CLASS_IMPLEMENT_ENUM(FileAccess)
public:
	ESB_CLASS_ENUM_ITEM(Read, Чтение)
	ESB_CLASS_ENUM_ITEM(Write, Запись)
	ESB_CLASS_ENUM_ITEM(ReadAndWrite, ЧтениеИЗапись)
};




ESB_CLASS_OBJECT(Stream)
{	ESB_CLASS_IMPLEMENT_OBJECT(Stream)
#if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
public:
// Not Creatable
public:
// Процедура НачатьЗакрытие(ОписаниеОповещения_ Как ОписаниеОповещения)
void BeginClose(ConstParam<CallbackDescription> CallbackDescription_) {ESB_INVOKE_MEMB_PROC1(MethId::BeginClose, CallbackDescription_);};
// Процедура НачатьКопированиеВ(ОписаниеОповещения_ Как ОписаниеОповещения, ЦелевойПоток_ Как Поток, [Количество_ Как Число])
void BeginCopyTo(ConstParam<CallbackDescription> CallbackDescription_, ConstParam<Stream> TargetStream_, ConstParam<Number> Count_ = DefUndef<esb::Number>) const {ESB_INVOKE_MEMB_PROC3(MethId::BeginCopyTo, CallbackDescription_, TargetStream_, Count_);};
// Процедура НачатьСбросБуферов(ОписаниеОповещения_ Как ОписаниеОповещения)
void BeginFlush(ConstParam<CallbackDescription> CallbackDescription_) const {ESB_INVOKE_MEMB_PROC1(MethId::BeginFlush, CallbackDescription_);};
// Процедура НачатьПолучениеРазмера([ОписаниеОповещения_ Как ОписаниеОповещения])
void BeginGetSize(ConstParam<CallbackDescription> CallbackDescription_ = DefUndef<esb::CallbackDescription>) const {ESB_INVOKE_MEMB_PROC1(MethId::BeginGetSize, CallbackDescription_);};
// Процедура НачатьЧтение(ОписаниеОповещения_ Как ОписаниеОповещения, Буфер_ Как БуферДвоичныхДанных, ПозицияВБуфере_ Как Число, Количество_ Как Число)
void BeginRead(ConstParam<CallbackDescription> CallbackDescription_, ConstParam<BinaryDataBuffer> Buffer_, ConstParam<Number> PositionInBuffer_, ConstParam<Number> Number_) const {ESB_INVOKE_MEMB_PROC4(MethId::BeginRead, CallbackDescription_, Buffer_, PositionInBuffer_, Number_);};
// Процедура НачатьПереход(ОписаниеОповещения_ Как ОписаниеОповещения, Смещение_ Как Число, [НачальнаяПозиция_ Как ПозицияВПотоке])
void BeginSeek(ConstParam<CallbackDescription> CallbackDescription_, ConstParam<Number> Offset_, ConstParam<PositionInStream> InitialPosition_ = DefUndef<esb::PositionInStream>) const {ESB_INVOKE_MEMB_PROC3(MethId::BeginSeek, CallbackDescription_, Offset_, InitialPosition_);};
// Процедура НачатьУстановкуРазмера(ОписаниеОповещения_ Как ОписаниеОповещения, Размер_ Как Число)
void BeginSetSize(ConstParam<CallbackDescription> CallbackDescription_, ConstParam<Number> Size_) {ESB_INVOKE_MEMB_PROC2(MethId::BeginSetSize, CallbackDescription_, Size_);};
// Процедура НачатьЗапись(ОписаниеОповещения_ Как ОписаниеОповещения, Буфер_ Как БуферДвоичныхДанных, ПозицияВБуфере_ Как Число, Количество_ Как Число)
void BeginWrite(ConstParam<CallbackDescription> CallbackDescription_, ConstParam<BinaryDataBuffer> Buffer_, ConstParam<Number> PositionInBuffer_, ConstParam<Number> Count_) {ESB_INVOKE_MEMB_PROC4(MethId::BeginWrite, CallbackDescription_, Buffer_, PositionInBuffer_, Count_);};
// Процедура Закрыть()
void Close() {ESB_INVOKE_MEMB_PROC0(MethId::Close);};
// Функция ЗакрытьАсинх() Возвращает Обещание<>
Promise<> CloseAsync() {ESB_INVOKE_MEMB_FUNC0(esb::Promise<>, MethId::CloseAsync);};
// Функция КопироватьВ(ЦелевойПоток_ Как Поток, [Количество_ Как Число]) Возвращает Число
Number CopyTo(ConstParam<Stream> TargetStream_, ConstParam<Number> Count_ = DefUndef<esb::Number>) const {ESB_INVOKE_MEMB_FUNC2(esb::Number, MethId::CopyTo, TargetStream_, Count_);};
// Функция КопироватьВАсинх(ЦелевойПоток_ Как Поток, [Количество_ Как Число]) Возвращает Обещание<Число>
Promise<Number> CopyToAsync(ConstParam<Stream> TargetStream_, ConstParam<Number> Count_ = DefUndef<esb::Number>) {ESB_INVOKE_MEMB_FUNC2(esb::Promise<Number>, MethId::CopyToAsync, TargetStream_, Count_);};
// Функция ТекущаяПозиция() Возвращает Число
Number CurrentPosition() const {ESB_INVOKE_MEMB_FUNC0(esb::Number, MethId::CurrentPosition);};
// Процедура СброситьБуферы()
void Flush() const {ESB_INVOKE_MEMB_PROC0(MethId::Flush);};
// Функция СброситьБуферыАсинх() Возвращает Обещание<>
Promise<> FlushAsync() {ESB_INVOKE_MEMB_FUNC0(esb::Promise<>, MethId::FlushAsync);};
// Функция ПолучитьПотокТолькоДляЧтения() Возвращает Поток
Stream GetReadonlyStream() const {ESB_INVOKE_MEMB_FUNC0(esb::Stream, MethId::GetReadOnlyStream);};
// Функция Прочитать(Буфер_ Как БуферДвоичныхДанных, ПозицияВБуфере_ Как Число, Количество_ Как Число) Возвращает Число
Number Read(ConstParam<BinaryDataBuffer> Buffer_, ConstParam<Number> PositionInBuffer_, ConstParam<Number> Kolichestvo_) const {ESB_INVOKE_MEMB_FUNC3(esb::Number, MethId::Read, Buffer_, PositionInBuffer_, Kolichestvo_);};
// Функция ПрочитатьАсинх(Буфер_ Как БуферДвоичныхДанных, ПозицияВБуфере_ Как Число, Количество_ Как Число) Возвращает Обещание<Число>
Promise<Number> ReadAsync(ConstParam<BinaryDataBuffer> Buffer_, ConstParam<Number> PositionInBuffer_, ConstParam<Number> Number_) {ESB_INVOKE_MEMB_FUNC3(esb::Promise<Number>, MethId::ReadAsync, Buffer_, PositionInBuffer_, Number_);};
// Функция Перейти(Смещение_ Как Число, [НачальнаяПозиция_ Как ПозицияВПотоке]) Возвращает Число
Number Seek(ConstParam<Number> Offset_, ConstParam<PositionInStream> InitialPosition_ = DefUndef<esb::PositionInStream>) const {ESB_INVOKE_MEMB_FUNC2(esb::Number, MethId::Seek, Offset_, InitialPosition_);};
// Функция ПерейтиАсинх(Смещение_ Как Число, [НачальнаяПозиция_ Как ПозицияВПотоке]) Возвращает Обещание<Число>
Promise<Number> SeekAsync(ConstParam<Number> Offset_, ConstParam<PositionInStream> InitialPosition_ = DefUndef<esb::PositionInStream>) {ESB_INVOKE_MEMB_FUNC2(esb::Promise<Number>, MethId::SeekAsync, Offset_, InitialPosition_);};
// Процедура УстановитьРазмер(Размер_ Как Число)
void SetSize(ConstParam<Number> Size_) {ESB_INVOKE_MEMB_PROC1(MethId::SetSize, Size_);};
// Функция УстановитьРазмерАсинх(Размер_ Как Число) Возвращает Обещание<>
Promise<> SetSizeAsync(ConstParam<Number> Size_) {ESB_INVOKE_MEMB_FUNC1(esb::Promise<>, MethId::SetSizeAsync, Size_);};
// Функция Размер() Возвращает Число
Number Size() const {ESB_INVOKE_MEMB_FUNC0(esb::Number, MethId::Size);};
// Функция РазмерАсинх() Возвращает Обещание<Число>
Promise<Number> SizeAsync() {ESB_INVOKE_MEMB_FUNC0(esb::Promise<Number>, MethId::SizeAsync);};
// Процедура Записать(Буфер_ Как БуферДвоичныхДанных, ПозицияВБуфере_ Как Число, Количество_ Как Число)
void Write(ConstParam<BinaryDataBuffer> Buffer_, ConstParam<Number> PositionInBuffer_, ConstParam<Number> Number_) {ESB_INVOKE_MEMB_PROC3(MethId::Write, Buffer_, PositionInBuffer_, Number_);};
// Функция ЗаписатьАсинх(Буфер_ Как БуферДвоичныхДанных, ПозицияВБуфере_ Как Число, Количество_ Как Число) Возвращает Обещание<>
Promise<> WriteAsync(ConstParam<BinaryDataBuffer> Buffer_, ConstParam<Number> PositionInBuffer_, ConstParam<Number> Count_) {ESB_INVOKE_MEMB_FUNC3(esb::Promise<>, MethId::WriteAsync, Buffer_, PositionInBuffer_, Count_);};
public:
// Свойство [[ТолькоЧтение]] ДоступноЧтение Как Булево
Boolean GetCanRead() const  { ESB_INVOKE_PROPERTY_GET(esb::Boolean, PropId::CanRead); };
ESB_PROPERTY_FIELD_RO(Boolean, CanRead)
// Свойство [[ТолькоЧтение]] ДоступноИзменениеПозиции Как Булево
Boolean GetCanSeek() const  { ESB_INVOKE_PROPERTY_GET(esb::Boolean, PropId::CanSeek); };
ESB_PROPERTY_FIELD_RO(Boolean, CanSeek)
// Свойство [[ТолькоЧтение]] ДоступнаЗапись Как Булево
Boolean GetCanWrite() const  { ESB_INVOKE_PROPERTY_GET(esb::Boolean, PropId::CanWrite); };
ESB_PROPERTY_FIELD_RO(Boolean, CanWrite)
#endif	// #if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
public:
// Not Creatable
public:
// Void BeginClose(CallbackDescription CallbackDescription_)
void НачатьЗакрытие(КонстПарам<ОписаниеОповещения> ОписаниеОповещения_) {ESB_INVOKE_MEMB_PROC1(MethId::BeginClose, ОписаниеОповещения_);};
// Void BeginCopyTo(CallbackDescription CallbackDescription_, Stream TargetStream_, Number Count_ = <default>)
void НачатьКопированиеВ(КонстПарам<ОписаниеОповещения> ОписаниеОповещения_, КонстПарам<Поток> ЦелевойПоток_, КонстПарам<Число> Количество_ = ПоУмолчаниюНеопределеноИли<esb::Число>) const {ESB_INVOKE_MEMB_PROC3(MethId::BeginCopyTo, ОписаниеОповещения_, ЦелевойПоток_, Количество_);};
// Void BeginFlush(CallbackDescription CallbackDescription_)
void НачатьСбросБуферов(КонстПарам<ОписаниеОповещения> ОписаниеОповещения_) const {ESB_INVOKE_MEMB_PROC1(MethId::BeginFlush, ОписаниеОповещения_);};
// Void BeginGetSize(CallbackDescription CallbackDescription_ = <default>)
void НачатьПолучениеРазмера(КонстПарам<ОписаниеОповещения> ОписаниеОповещения_ = ПоУмолчаниюНеопределеноИли<esb::ОписаниеОповещения>) const {ESB_INVOKE_MEMB_PROC1(MethId::BeginGetSize, ОписаниеОповещения_);};
// Void BeginRead(CallbackDescription CallbackDescription_, BinaryDataBuffer Buffer_, Number PositionInBuffer_, Number Number_)
void НачатьЧтение(КонстПарам<ОписаниеОповещения> ОписаниеОповещения_, КонстПарам<БуферДвоичныхДанных> Буфер_, КонстПарам<Число> ПозицияВБуфере_, КонстПарам<Число> Количество_) const {ESB_INVOKE_MEMB_PROC4(MethId::BeginRead, ОписаниеОповещения_, Буфер_, ПозицияВБуфере_, Количество_);};
// Void BeginSeek(CallbackDescription CallbackDescription_, Number Offset_, PositionInStream InitialPosition_ = <default>)
void НачатьПереход(КонстПарам<ОписаниеОповещения> ОписаниеОповещения_, КонстПарам<Число> Смещение_, КонстПарам<ПозицияВПотоке> НачальнаяПозиция_ = ПоУмолчаниюНеопределеноИли<esb::ПозицияВПотоке>) const {ESB_INVOKE_MEMB_PROC3(MethId::BeginSeek, ОписаниеОповещения_, Смещение_, НачальнаяПозиция_);};
// Void BeginSetSize(CallbackDescription CallbackDescription_, Number Size_)
void НачатьУстановкуРазмера(КонстПарам<ОписаниеОповещения> ОписаниеОповещения_, КонстПарам<Число> Размер_) {ESB_INVOKE_MEMB_PROC2(MethId::BeginSetSize, ОписаниеОповещения_, Размер_);};
// Void BeginWrite(CallbackDescription CallbackDescription_, BinaryDataBuffer Buffer_, Number PositionInBuffer_, Number Count_)
void НачатьЗапись(КонстПарам<ОписаниеОповещения> ОписаниеОповещения_, КонстПарам<БуферДвоичныхДанных> Буфер_, КонстПарам<Число> ПозицияВБуфере_, КонстПарам<Число> Количество_) {ESB_INVOKE_MEMB_PROC4(MethId::BeginWrite, ОписаниеОповещения_, Буфер_, ПозицияВБуфере_, Количество_);};
// Void Close()
void Закрыть() {ESB_INVOKE_MEMB_PROC0(MethId::Close);};
// Function CloseAsync() -> Promise<>
Обещание<> ЗакрытьАсинх() {ESB_INVOKE_MEMB_FUNC0(esb::Обещание<>, MethId::CloseAsync);};
// Function CopyTo(Stream TargetStream_, Number Count_ = <default>) -> Number
Число КопироватьВ(КонстПарам<Поток> ЦелевойПоток_, КонстПарам<Число> Количество_ = ПоУмолчаниюНеопределеноИли<esb::Число>) const {ESB_INVOKE_MEMB_FUNC2(esb::Число, MethId::CopyTo, ЦелевойПоток_, Количество_);};
// Function CopyToAsync(Stream TargetStream_, Number Count_ = <default>) -> Promise<Number>
Обещание<Число> КопироватьВАсинх(КонстПарам<Поток> ЦелевойПоток_, КонстПарам<Число> Количество_ = ПоУмолчаниюНеопределеноИли<esb::Число>) {ESB_INVOKE_MEMB_FUNC2(esb::Обещание<Число>, MethId::CopyToAsync, ЦелевойПоток_, Количество_);};
// Function CurrentPosition() -> Number
Число ТекущаяПозиция() const {ESB_INVOKE_MEMB_FUNC0(esb::Число, MethId::CurrentPosition);};
// Void Flush()
void СброситьБуферы() const {ESB_INVOKE_MEMB_PROC0(MethId::Flush);};
// Function FlushAsync() -> Promise<>
Обещание<> СброситьБуферыАсинх() {ESB_INVOKE_MEMB_FUNC0(esb::Обещание<>, MethId::FlushAsync);};
// Function GetReadonlyStream() -> Stream
Поток ПолучитьПотокТолькоДляЧтения() const {ESB_INVOKE_MEMB_FUNC0(esb::Поток, MethId::GetReadOnlyStream);};
// Function Read(BinaryDataBuffer Buffer_, Number PositionInBuffer_, Number Kolichestvo_) -> Number
Число Прочитать(КонстПарам<БуферДвоичныхДанных> Буфер_, КонстПарам<Число> ПозицияВБуфере_, КонстПарам<Число> Количество_) const {ESB_INVOKE_MEMB_FUNC3(esb::Число, MethId::Read, Буфер_, ПозицияВБуфере_, Количество_);};
// Function ReadAsync(BinaryDataBuffer Buffer_, Number PositionInBuffer_, Number Number_) -> Promise<Number>
Обещание<Число> ПрочитатьАсинх(КонстПарам<БуферДвоичныхДанных> Буфер_, КонстПарам<Число> ПозицияВБуфере_, КонстПарам<Число> Количество_) {ESB_INVOKE_MEMB_FUNC3(esb::Обещание<Число>, MethId::ReadAsync, Буфер_, ПозицияВБуфере_, Количество_);};
// Function Seek(Number Offset_, PositionInStream InitialPosition_ = <default>) -> Number
Число Перейти(КонстПарам<Число> Смещение_, КонстПарам<ПозицияВПотоке> НачальнаяПозиция_ = ПоУмолчаниюНеопределеноИли<esb::ПозицияВПотоке>) const {ESB_INVOKE_MEMB_FUNC2(esb::Число, MethId::Seek, Смещение_, НачальнаяПозиция_);};
// Function SeekAsync(Number Offset_, PositionInStream InitialPosition_ = <default>) -> Promise<Number>
Обещание<Число> ПерейтиАсинх(КонстПарам<Число> Смещение_, КонстПарам<ПозицияВПотоке> НачальнаяПозиция_ = ПоУмолчаниюНеопределеноИли<esb::ПозицияВПотоке>) {ESB_INVOKE_MEMB_FUNC2(esb::Обещание<Число>, MethId::SeekAsync, Смещение_, НачальнаяПозиция_);};
// Void SetSize(Number Size_)
void УстановитьРазмер(КонстПарам<Число> Размер_) {ESB_INVOKE_MEMB_PROC1(MethId::SetSize, Размер_);};
// Function SetSizeAsync(Number Size_) -> Promise<>
Обещание<> УстановитьРазмерАсинх(КонстПарам<Число> Размер_) {ESB_INVOKE_MEMB_FUNC1(esb::Обещание<>, MethId::SetSizeAsync, Размер_);};
// Function Size() -> Number
Число Размер() const {ESB_INVOKE_MEMB_FUNC0(esb::Число, MethId::Size);};
// Function SizeAsync() -> Promise<Number>
Обещание<Число> РазмерАсинх() {ESB_INVOKE_MEMB_FUNC0(esb::Обещание<Число>, MethId::SizeAsync);};
// Void Write(BinaryDataBuffer Buffer_, Number PositionInBuffer_, Number Number_)
void Записать(КонстПарам<БуферДвоичныхДанных> Буфер_, КонстПарам<Число> ПозицияВБуфере_, КонстПарам<Число> Количество_) {ESB_INVOKE_MEMB_PROC3(MethId::Write, Буфер_, ПозицияВБуфере_, Количество_);};
// Function WriteAsync(BinaryDataBuffer Buffer_, Number PositionInBuffer_, Number Count_) -> Promise<>
Обещание<> ЗаписатьАсинх(КонстПарам<БуферДвоичныхДанных> Буфер_, КонстПарам<Число> ПозицияВБуфере_, КонстПарам<Число> Количество_) {ESB_INVOKE_MEMB_FUNC3(esb::Обещание<>, MethId::WriteAsync, Буфер_, ПозицияВБуфере_, Количество_);};
public:
// Property [[ReadOnly]] CanRead -> Boolean
Булево ПолучитьДоступноЧтение() const  { ESB_INVOKE_PROPERTY_GET(esb::Булево, PropId::CanRead); };
ESB_PROPERTY_FIELD_RO(Булево, ДоступноЧтение)
// Property [[ReadOnly]] CanSeek -> Boolean
Булево ПолучитьДоступноИзменениеПозиции() const  { ESB_INVOKE_PROPERTY_GET(esb::Булево, PropId::CanSeek); };
ESB_PROPERTY_FIELD_RO(Булево, ДоступноИзменениеПозиции)
// Property [[ReadOnly]] CanWrite -> Boolean
Булево ПолучитьДоступнаЗапись() const  { ESB_INVOKE_PROPERTY_GET(esb::Булево, PropId::CanWrite); };
ESB_PROPERTY_FIELD_RO(Булево, ДоступнаЗапись)
#endif	// #if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
};


ESB_CLASS_OBJECT(BinaryData)
{	ESB_CLASS_IMPLEMENT_OBJECT(BinaryData)
#if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
public:
static BinaryData Create(ConstParam<String> FileName_) {ESB_INVOKE_CTOR1(BinaryData, FileName_);}
public:
// Процедура НачатьЗапись(ОписаниеОповещения_ Как ОписаниеОповещения, Поток_ Как Поток)
void BeginWrite(ConstParam<CallbackDescription> CallbackDescription_, ConstParam<Stream> Stream_) const {ESB_INVOKE_MEMB_PROC2(MethId::BeginWrite, CallbackDescription_, Stream_);};
// Процедура НачатьЗапись(ОписаниеОповещения_ Как ОписаниеОповещения, Поток_ Как Поток)
void BeginWrite(ConstParam<CallbackDescription> CallbackDescription_, ConstParam<String> FileName_) const {ESB_INVOKE_MEMB_PROC2(MethId::BeginWrite, CallbackDescription_, FileName_);};
// Функция ОткрытьПотокДляЧтения() Возвращает Поток
Stream OpenStreamForRead() const {ESB_INVOKE_MEMB_FUNC0(esb::Stream, MethId::OpenStreamForRead);};
// Функция Размер() Возвращает Число
Number Size() const {ESB_INVOKE_MEMB_FUNC0(esb::Number, MethId::Size);};
// Процедура Записать(Поток_ Как Поток)
void Write(ConstParam<Stream> Stream_) const {ESB_INVOKE_MEMB_PROC1(MethId::Write, Stream_);};
// Процедура Записать(Поток_ Как Поток)
void Write(ConstParam<String> FileName_) const {ESB_INVOKE_MEMB_PROC1(MethId::Write, FileName_);};
// Функция ЗаписатьАсинх(ИмяФайла_ Как Строка) Возвращает Обещание<>
Promise<> WriteAsync(ConstParam<String> FileName_) {ESB_INVOKE_MEMB_FUNC1(esb::Promise<>, MethId::WriteAsync, FileName_);};
// Функция ЗаписатьАсинх(ИмяФайла_ Как Строка) Возвращает Обещание<>
Promise<> WriteAsync(ConstParam<Stream> Stream_) {ESB_INVOKE_MEMB_FUNC1(esb::Promise<>, MethId::WriteAsync, Stream_);};
#endif	// #if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
public:
static ДвоичныеДанные Создать(КонстПарам<Строка> ИмяФайла_) {ESB_INVOKE_CTOR1(ДвоичныеДанные, ИмяФайла_);}
public:
// Void BeginWrite(CallbackDescription CallbackDescription_, Stream Stream_)
void НачатьЗапись(КонстПарам<ОписаниеОповещения> ОписаниеОповещения_, КонстПарам<Поток> Поток_) const {ESB_INVOKE_MEMB_PROC2(MethId::BeginWrite, ОписаниеОповещения_, Поток_);};
// Void BeginWrite(CallbackDescription CallbackDescription_, Stream Stream_)
void НачатьЗапись(КонстПарам<ОписаниеОповещения> ОписаниеОповещения_, КонстПарам<Строка> ИмяФайла_) const {ESB_INVOKE_MEMB_PROC2(MethId::BeginWrite, ОписаниеОповещения_, ИмяФайла_);};
// Function OpenStreamForRead() -> Stream
Поток ОткрытьПотокДляЧтения() const {ESB_INVOKE_MEMB_FUNC0(esb::Поток, MethId::OpenStreamForRead);};
// Function Size() -> Number
Число Размер() const {ESB_INVOKE_MEMB_FUNC0(esb::Число, MethId::Size);};
// Void Write(Stream Stream_)
void Записать(КонстПарам<Поток> Поток_) const {ESB_INVOKE_MEMB_PROC1(MethId::Write, Поток_);};
// Void Write(Stream Stream_)
void Записать(КонстПарам<Строка> ИмяФайла_) const {ESB_INVOKE_MEMB_PROC1(MethId::Write, ИмяФайла_);};
// Function WriteAsync(String FileName_) -> Promise<>
Обещание<> ЗаписатьАсинх(КонстПарам<Строка> ИмяФайла_) {ESB_INVOKE_MEMB_FUNC1(esb::Обещание<>, MethId::WriteAsync, ИмяФайла_);};
// Function WriteAsync(String FileName_) -> Promise<>
Обещание<> ЗаписатьАсинх(КонстПарам<Поток> Поток_) {ESB_INVOKE_MEMB_FUNC1(esb::Обещание<>, MethId::WriteAsync, Поток_);};
#endif	// #if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
};


ESB_CLASS_OBJECT_DUAL(BinaryDataBuffer, IxCollectionRW<Number>)
{	ESB_CLASS_IMPLEMENT_OBJECT_DUAL(BinaryDataBuffer)
#if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
public:
static BinaryDataBuffer Create(ConstParam<Number> Size_, ConstParam<ByteOrder> ByteOrder_ = DefUndef<esb::ByteOrder>) {ESB_INVOKE_CTOR2(BinaryDataBuffer, Size_, ByteOrder_);}
public:
// Функция Соединить(Буфер_ Как БуферДвоичныхДанных) Возвращает БуферДвоичныхДанных
BinaryDataBuffer Concat(ConstParam<BinaryDataBuffer> Buffer_) const {ESB_INVOKE_MEMB_FUNC1(esb::BinaryDataBuffer, MethId::Concat, Buffer_);};
// Функция Скопировать() Возвращает БуферДвоичныхДанных
BinaryDataBuffer Copy() const {ESB_INVOKE_MEMB_FUNC0(esb::BinaryDataBuffer, MethId::Copy);};
// Функция Получить(Позиция_ Как Число) Возвращает Число
Number Get(ConstParam<Number> Position_) const {ESB_INVOKE_MEMB_FUNC1(esb::Number, MethId::Get, Position_);};
// Функция ПолучитьСрез(Позиция_ Как Число, [Количество_ Как Число]) Возвращает БуферДвоичныхДанных
BinaryDataBuffer GetSlice(ConstParam<Number> Position_, ConstParam<Number> Number_ = DefUndef<esb::Number>) const {ESB_INVOKE_MEMB_FUNC2(esb::BinaryDataBuffer, MethId::GetSlice, Position_, Number_);};
// Процедура Инвертировать([Позиция_ Как Число], [Количество_ Как Число])
void Invert(ConstParam<Number> Position_ = DefUndef<esb::Number>, ConstParam<Number> Number_ = DefUndef<esb::Number>) {ESB_INVOKE_MEMB_PROC2(MethId::Invert, Position_, Number_);};
// Функция Прочитать(Позиция_ Как Число, Количество_ Как Число) Возвращает БуферДвоичныхДанных
BinaryDataBuffer Read(ConstParam<Number> Position_, ConstParam<Number> Number_) const {ESB_INVOKE_MEMB_FUNC2(esb::BinaryDataBuffer, MethId::Read, Position_, Number_);};
// Функция ПрочитатьЦелое16(Позиция_ Как Число, [ПорядокБайтов_ Как ПорядокБайтов]) Возвращает Число
Number ReadInt16(ConstParam<Number> Position_, ConstParam<ByteOrder> ByteOrder_ = DefUndef<esb::ByteOrder>) const {ESB_INVOKE_MEMB_FUNC2(esb::Number, MethId::ReadInt16, Position_, ByteOrder_);};
// Функция ПрочитатьЦелое32(Позиция_ Как Число, [ПорядокБайтов_ Как ПорядокБайтов]) Возвращает Число
Number ReadInt32(ConstParam<Number> Position_, ConstParam<ByteOrder> ByteOrder_ = DefUndef<esb::ByteOrder>) const {ESB_INVOKE_MEMB_FUNC2(esb::Number, MethId::ReadInt32, Position_, ByteOrder_);};
// Функция ПрочитатьЦелое64(Позиция_ Как Число, [ПорядокБайтов_ Как ПорядокБайтов]) Возвращает Число
Number ReadInt64(ConstParam<Number> Position_, ConstParam<ByteOrder> ByteOrder_ = DefUndef<esb::ByteOrder>) const {ESB_INVOKE_MEMB_FUNC2(esb::Number, MethId::ReadInt64, Position_, ByteOrder_);};
// Функция Перевернуть() Возвращает БуферДвоичныхДанных
BinaryDataBuffer Reverse() const {ESB_INVOKE_MEMB_FUNC0(esb::BinaryDataBuffer, MethId::Reverse);};
// Процедура Установить(Позиция_ Как Число, Значение_ Как Число)
void Set(ConstParam<Number> Position_, ConstParam<Number> Value_) {ESB_INVOKE_MEMB_PROC2(MethId::Set, Position_, Value_);};
// Процедура УстановитьТолькоЧтение()
void SetReadOnly() {ESB_INVOKE_MEMB_PROC0(MethId::SetReadOnly);};
// Функция Разделить(Разделитель_ Как БуферДвоичныхДанных) Возвращает МассивИз<БуферДвоичныхДанных>
ArrayOf<BinaryDataBuffer> Split(ConstParam<BinaryDataBuffer> Separator_) const {ESB_INVOKE_MEMB_FUNC1(esb::ArrayOf<BinaryDataBuffer>, MethId::Split, Separator_);};
// Функция Разделить(Разделитель_ Как БуферДвоичныхДанных) Возвращает МассивИз<БуферДвоичныхДанных>
ArrayOf<BinaryDataBuffer> Split(ConstParam<Array> Separators_) const {ESB_INVOKE_MEMB_FUNC1(esb::ArrayOf<BinaryDataBuffer>, MethId::Split, Separators_);};
// Процедура Записать(Позиция_ Как Число, Байты_ Как БуферДвоичныхДанных, [Количество_ Как Число])
void Write(ConstParam<Number> Position_, ConstParam<BinaryDataBuffer> Bytes_, ConstParam<Number> Number_ = DefUndef<esb::Number>) {ESB_INVOKE_MEMB_PROC3(MethId::Write, Position_, Bytes_, Number_);};
// Процедура ЗаписатьПобитовоеИ(Позиция_ Как Число, Буфер_ Как БуферДвоичныхДанных, [Количество_ Как Число])
void WriteBitwiseAnd(ConstParam<Number> Position_, ConstParam<BinaryDataBuffer> Buffer_, ConstParam<Number> Number_ = DefUndef<esb::Number>) {ESB_INVOKE_MEMB_PROC3(MethId::WriteBitwiseAnd, Position_, Buffer_, Number_);};
// Процедура ЗаписатьПобитовоеИНе(Позиция_ Как Число, Буфер_ Как БуферДвоичныхДанных, [Количество_ Как Число])
void WriteBitwiseAndNot(ConstParam<Number> Position_, ConstParam<BinaryDataBuffer> Buffer_, ConstParam<Number> Number_ = DefUndef<esb::Number>) {ESB_INVOKE_MEMB_PROC3(MethId::WriteBitwiseAndNot, Position_, Buffer_, Number_);};
// Процедура ЗаписатьПобитовоеИли(Позиция_ Как Число, Буфер_ Как БуферДвоичныхДанных, [Количество_ Как Число])
void WriteBitwiseOr(ConstParam<Number> Position_, ConstParam<BinaryDataBuffer> Buffer_, ConstParam<Number> Number_ = DefUndef<esb::Number>) {ESB_INVOKE_MEMB_PROC3(MethId::WriteBitwiseOr, Position_, Buffer_, Number_);};
// Процедура ЗаписатьПобитовоеИсключительноеИли(Позиция_ Как Число, Буфер_ Как БуферДвоичныхДанных, [Количество_ Как Число])
void WriteBitwiseXor(ConstParam<Number> Position_, ConstParam<BinaryDataBuffer> Buffer_, ConstParam<Number> Number_ = DefUndef<esb::Number>) {ESB_INVOKE_MEMB_PROC3(MethId::WriteBitwiseXor, Position_, Buffer_, Number_);};
// Процедура ЗаписатьЦелое16(Позиция_ Как Число, Значение_ Как Число, [ПорядокБайтов_ Как ПорядокБайтов])
void WriteInt16(ConstParam<Number> Position_, ConstParam<Number> Value_, ConstParam<ByteOrder> ByteOrder_ = DefUndef<esb::ByteOrder>) {ESB_INVOKE_MEMB_PROC3(MethId::WriteInt16, Position_, Value_, ByteOrder_);};
// Процедура ЗаписатьЦелое32(Позиция_ Как Число, Значение_ Как Число, [ПорядокБайтов_ Как ПорядокБайтов])
void WriteInt32(ConstParam<Number> Position_, ConstParam<Number> Value_, ConstParam<ByteOrder> ByteOrder_ = DefUndef<esb::ByteOrder>) {ESB_INVOKE_MEMB_PROC3(MethId::WriteInt32, Position_, Value_, ByteOrder_);};
// Процедура ЗаписатьЦелое64(Позиция_ Как Число, Значение_ Как Число, [ПорядокБайтов_ Как ПорядокБайтов])
void WriteInt64(ConstParam<Number> Position_, ConstParam<Number> Value_, ConstParam<ByteOrder> ByteOrder_ = DefUndef<esb::ByteOrder>) {ESB_INVOKE_MEMB_PROC3(MethId::WriteInt64, Position_, Value_, ByteOrder_);};
public:
// Свойство [[Чтение,Запись]] ПорядокБайтов Как ПорядокБайтов
ByteOrder GetByteOrder() const  { ESB_INVOKE_PROPERTY_GET(esb::ByteOrder, PropId::ByteOrder); };
void SetByteOrder(ConstParam<ByteOrder> Value_)  { ESB_INVOKE_PROPERTY_SET(PropId::ByteOrder, Value_); };
ESB_PROPERTY_FIELD_RW(ByteOrder, ByteOrder)
// Свойство [[ТолькоЧтение]] ТолькоЧтение Как Булево
Boolean GetReadOnly() const  { ESB_INVOKE_PROPERTY_GET(esb::Boolean, PropId::ReadOnly); };
ESB_PROPERTY_FIELD_RO(Boolean, ReadOnly)
// Свойство [[ТолькоЧтение]] Размер Как Число
Number GetSize() const  { ESB_INVOKE_PROPERTY_GET(esb::Number, PropId::Size); };
ESB_PROPERTY_FIELD_RO(Number, Size)
#endif	// #if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
public:
static БуферДвоичныхДанных Создать(КонстПарам<Число> Размер_, КонстПарам<ПорядокБайтов> ПорядокБайтов_ = ПоУмолчаниюНеопределеноИли<esb::ПорядокБайтов>) {ESB_INVOKE_CTOR2(БуферДвоичныхДанных, Размер_, ПорядокБайтов_);}
public:
// Function Concat(BinaryDataBuffer Buffer_) -> BinaryDataBuffer
БуферДвоичныхДанных Соединить(КонстПарам<БуферДвоичныхДанных> Буфер_) const {ESB_INVOKE_MEMB_FUNC1(esb::БуферДвоичныхДанных, MethId::Concat, Буфер_);};
// Function Copy() -> BinaryDataBuffer
БуферДвоичныхДанных Скопировать() const {ESB_INVOKE_MEMB_FUNC0(esb::БуферДвоичныхДанных, MethId::Copy);};
// Function Get(Number Position_) -> Number
Число Получить(КонстПарам<Число> Позиция_) const {ESB_INVOKE_MEMB_FUNC1(esb::Число, MethId::Get, Позиция_);};
// Function GetSlice(Number Position_, Number Number_ = <default>) -> BinaryDataBuffer
БуферДвоичныхДанных ПолучитьСрез(КонстПарам<Число> Позиция_, КонстПарам<Число> Количество_ = ПоУмолчаниюНеопределеноИли<esb::Число>) const {ESB_INVOKE_MEMB_FUNC2(esb::БуферДвоичныхДанных, MethId::GetSlice, Позиция_, Количество_);};
// Void Invert(Number Position_ = <default>, Number Number_ = <default>)
void Инвертировать(КонстПарам<Число> Позиция_ = ПоУмолчаниюНеопределеноИли<esb::Число>, КонстПарам<Число> Количество_ = ПоУмолчаниюНеопределеноИли<esb::Число>) {ESB_INVOKE_MEMB_PROC2(MethId::Invert, Позиция_, Количество_);};
// Function Read(Number Position_, Number Number_) -> BinaryDataBuffer
БуферДвоичныхДанных Прочитать(КонстПарам<Число> Позиция_, КонстПарам<Число> Количество_) const {ESB_INVOKE_MEMB_FUNC2(esb::БуферДвоичныхДанных, MethId::Read, Позиция_, Количество_);};
// Function ReadInt16(Number Position_, ByteOrder ByteOrder_ = <default>) -> Number
Число ПрочитатьЦелое16(КонстПарам<Число> Позиция_, КонстПарам<ПорядокБайтов> ПорядокБайтов_ = ПоУмолчаниюНеопределеноИли<esb::ПорядокБайтов>) const {ESB_INVOKE_MEMB_FUNC2(esb::Число, MethId::ReadInt16, Позиция_, ПорядокБайтов_);};
// Function ReadInt32(Number Position_, ByteOrder ByteOrder_ = <default>) -> Number
Число ПрочитатьЦелое32(КонстПарам<Число> Позиция_, КонстПарам<ПорядокБайтов> ПорядокБайтов_ = ПоУмолчаниюНеопределеноИли<esb::ПорядокБайтов>) const {ESB_INVOKE_MEMB_FUNC2(esb::Число, MethId::ReadInt32, Позиция_, ПорядокБайтов_);};
// Function ReadInt64(Number Position_, ByteOrder ByteOrder_ = <default>) -> Number
Число ПрочитатьЦелое64(КонстПарам<Число> Позиция_, КонстПарам<ПорядокБайтов> ПорядокБайтов_ = ПоУмолчаниюНеопределеноИли<esb::ПорядокБайтов>) const {ESB_INVOKE_MEMB_FUNC2(esb::Число, MethId::ReadInt64, Позиция_, ПорядокБайтов_);};
// Function Reverse() -> BinaryDataBuffer
БуферДвоичныхДанных Перевернуть() const {ESB_INVOKE_MEMB_FUNC0(esb::БуферДвоичныхДанных, MethId::Reverse);};
// Void Set(Number Position_, Number Value_)
void Установить(КонстПарам<Число> Позиция_, КонстПарам<Число> Значение_) {ESB_INVOKE_MEMB_PROC2(MethId::Set, Позиция_, Значение_);};
// Void SetReadOnly()
void УстановитьТолькоЧтение() {ESB_INVOKE_MEMB_PROC0(MethId::SetReadOnly);};
// Function Split(BinaryDataBuffer Separator_) -> ArrayOf<BinaryDataBuffer>
МассивИз<БуферДвоичныхДанных> Разделить(КонстПарам<БуферДвоичныхДанных> Разделитель_) const {ESB_INVOKE_MEMB_FUNC1(esb::МассивИз<БуферДвоичныхДанных>, MethId::Split, Разделитель_);};
// Function Split(BinaryDataBuffer Separator_) -> ArrayOf<BinaryDataBuffer>
МассивИз<БуферДвоичныхДанных> Разделить(КонстПарам<Массив> Разделители_) const {ESB_INVOKE_MEMB_FUNC1(esb::МассивИз<БуферДвоичныхДанных>, MethId::Split, Разделители_);};
// Void Write(Number Position_, BinaryDataBuffer Bytes_, Number Number_ = <default>)
void Записать(КонстПарам<Число> Позиция_, КонстПарам<БуферДвоичныхДанных> Байты_, КонстПарам<Число> Количество_ = ПоУмолчаниюНеопределеноИли<esb::Число>) {ESB_INVOKE_MEMB_PROC3(MethId::Write, Позиция_, Байты_, Количество_);};
// Void WriteBitwiseAnd(Number Position_, BinaryDataBuffer Buffer_, Number Number_ = <default>)
void ЗаписатьПобитовоеИ(КонстПарам<Число> Позиция_, КонстПарам<БуферДвоичныхДанных> Буфер_, КонстПарам<Число> Количество_ = ПоУмолчаниюНеопределеноИли<esb::Число>) {ESB_INVOKE_MEMB_PROC3(MethId::WriteBitwiseAnd, Позиция_, Буфер_, Количество_);};
// Void WriteBitwiseAndNot(Number Position_, BinaryDataBuffer Buffer_, Number Number_ = <default>)
void ЗаписатьПобитовоеИНе(КонстПарам<Число> Позиция_, КонстПарам<БуферДвоичныхДанных> Буфер_, КонстПарам<Число> Количество_ = ПоУмолчаниюНеопределеноИли<esb::Число>) {ESB_INVOKE_MEMB_PROC3(MethId::WriteBitwiseAndNot, Позиция_, Буфер_, Количество_);};
// Void WriteBitwiseOr(Number Position_, BinaryDataBuffer Buffer_, Number Number_ = <default>)
void ЗаписатьПобитовоеИли(КонстПарам<Число> Позиция_, КонстПарам<БуферДвоичныхДанных> Буфер_, КонстПарам<Число> Количество_ = ПоУмолчаниюНеопределеноИли<esb::Число>) {ESB_INVOKE_MEMB_PROC3(MethId::WriteBitwiseOr, Позиция_, Буфер_, Количество_);};
// Void WriteBitwiseXor(Number Position_, BinaryDataBuffer Buffer_, Number Number_ = <default>)
void ЗаписатьПобитовоеИсключительноеИли(КонстПарам<Число> Позиция_, КонстПарам<БуферДвоичныхДанных> Буфер_, КонстПарам<Число> Количество_ = ПоУмолчаниюНеопределеноИли<esb::Число>) {ESB_INVOKE_MEMB_PROC3(MethId::WriteBitwiseXor, Позиция_, Буфер_, Количество_);};
// Void WriteInt16(Number Position_, Number Value_, ByteOrder ByteOrder_ = <default>)
void ЗаписатьЦелое16(КонстПарам<Число> Позиция_, КонстПарам<Число> Значение_, КонстПарам<ПорядокБайтов> ПорядокБайтов_ = ПоУмолчаниюНеопределеноИли<esb::ПорядокБайтов>) {ESB_INVOKE_MEMB_PROC3(MethId::WriteInt16, Позиция_, Значение_, ПорядокБайтов_);};
// Void WriteInt32(Number Position_, Number Value_, ByteOrder ByteOrder_ = <default>)
void ЗаписатьЦелое32(КонстПарам<Число> Позиция_, КонстПарам<Число> Значение_, КонстПарам<ПорядокБайтов> ПорядокБайтов_ = ПоУмолчаниюНеопределеноИли<esb::ПорядокБайтов>) {ESB_INVOKE_MEMB_PROC3(MethId::WriteInt32, Позиция_, Значение_, ПорядокБайтов_);};
// Void WriteInt64(Number Position_, Number Value_, ByteOrder ByteOrder_ = <default>)
void ЗаписатьЦелое64(КонстПарам<Число> Позиция_, КонстПарам<Число> Значение_, КонстПарам<ПорядокБайтов> ПорядокБайтов_ = ПоУмолчаниюНеопределеноИли<esb::ПорядокБайтов>) {ESB_INVOKE_MEMB_PROC3(MethId::WriteInt64, Позиция_, Значение_, ПорядокБайтов_);};
public:
// Property [[Read,Write]] ByteOrder -> ByteOrder
ПорядокБайтов ПолучитьПорядокБайтов() const  { ESB_INVOKE_PROPERTY_GET(esb::ПорядокБайтов, PropId::ByteOrder); };
void УстановитьПорядокБайтов(КонстПарам<ПорядокБайтов> Значение_)  { ESB_INVOKE_PROPERTY_SET(PropId::ByteOrder, Значение_); };
ESB_PROPERTY_FIELD_RW(ПорядокБайтов, ПорядокБайтов)
// Property [[ReadOnly]] ReadOnly -> Boolean
Булево ПолучитьТолькоЧтение() const  { ESB_INVOKE_PROPERTY_GET(esb::Булево, PropId::ReadOnly); };
ESB_PROPERTY_FIELD_RO(Булево, ТолькоЧтение)
// Property [[ReadOnly]] Size -> Number
Число ПолучитьРазмер() const  { ESB_INVOKE_PROPERTY_GET(esb::Число, PropId::Size); };
ESB_PROPERTY_FIELD_RO(Число, Размер)
#endif	// #if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
public:
ESB_IMPLEMENT_ENUM_ITEMS(BinaryDataBuffer, Number)
public:
ESB_IMPLEMENT_INDEXED_COLLECTION_RW()
	//--------------------------------------------- ^^^^	end codegen		^^^^
	// 
	// -------------------------------------------- vvvv	manual			vvvv
		// Попытка получить доступ к байт-буферу напрямую в стиле с++ минуя уровни абстракции платформы
		// реализовано только в ESB_VER_v8309, для поздних версий не работает ввиду изменений в платформе
		// исследования причины и исправления не проводились ввиду сложности доступа напрямую к байт-буферу, и,сответственно,
		// рисков опять получить несовместимость. При необходимости может быть реализовано.
	public:
#if ESB_USE_BYTEBUFFER_DIRECT_HACK
			size_t	GetByteBufferSize() const						{ return _internal::IBinaryDataBufferBytes_GetSize(ix_collection_t::get_interface()); }
			uint8_t GetByteBufferByte(size_t at_) const				{ return _internal::IBinaryDataBufferBytes_GetByte(ix_collection_t::get_interface(), at_); }
			void	SetByteBufferByte(size_t at_, uint8_t value_)	{ return _internal::IBinaryDataBufferBytes_SetByte(ix_collection_t::get_interface(), at_, value_); }
	#else
			// Просто эмулируем через диспатч-интерфейс
			size_t	GetByteBufferSize() const						{ return static_cast<size_t>(static_cast<ptrdiff_t>(GetSize())); }
			uint8_t GetByteBufferByte(size_t at_) const				{ return static_cast<uint8_t>(static_cast<int>(Get(Numeric(at_)))); }
			void	SetByteBufferByte(size_t at_, uint8_t value_)	{ return Set(Numeric(at_), Numeric(value_)); }
	#endif
};


ESB_CLASS_OBJECT(FileStream)
{	ESB_CLASS_IMPLEMENT_OBJECT(FileStream)
#if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
public:
static FileStream Create(ConstParam<String> FileName_, ConstParam<FileOpenMode> OpeningMode_, ConstParam<Number> BufferSize_ = DefUndef<esb::Number>) {ESB_INVOKE_CTOR3(FileStream, FileName_, OpeningMode_, BufferSize_);}
static FileStream Create(ConstParam<String> FileName_, ConstParam<FileOpenMode> OpeningMode_, ConstParam<FileAccess> AccessLevel_, ConstParam<Number> BufferSize_ = DefUndef<esb::Number>) {ESB_INVOKE_CTOR4(FileStream, FileName_, OpeningMode_, AccessLevel_, BufferSize_);}
public:
// Процедура НачатьЗакрытие(ОписаниеОповещения_ Как ОписаниеОповещения)
void BeginClose(ConstParam<CallbackDescription> CallbackDescription_) {ESB_INVOKE_MEMB_PROC1(MethId::BeginClose, CallbackDescription_);};
// Процедура НачатьКопированиеВ(ОписаниеОповещения_ Как ОписаниеОповещения, ЦелевойПоток_ Как Поток, [Количество_ Как Число])
void BeginCopyTo(ConstParam<CallbackDescription> CallbackDescription_, ConstParam<Stream> TargetStream_, ConstParam<Number> Count_ = DefUndef<esb::Number>) const {ESB_INVOKE_MEMB_PROC3(MethId::BeginCopyTo, CallbackDescription_, TargetStream_, Count_);};
// Процедура НачатьСбросБуферов(ОписаниеОповещения_ Как ОписаниеОповещения)
void BeginFlush(ConstParam<CallbackDescription> CallbackDescription_) const {ESB_INVOKE_MEMB_PROC1(MethId::BeginFlush, CallbackDescription_);};
// Процедура НачатьПолучениеРазмера([ОписаниеОповещения_ Как ОписаниеОповещения])
void BeginGetSize(ConstParam<CallbackDescription> CallbackDescription_ = DefUndef<esb::CallbackDescription>) const {ESB_INVOKE_MEMB_PROC1(MethId::BeginGetSize, CallbackDescription_);};
// Процедура НачатьЧтение(ОписаниеОповещения_ Как ОписаниеОповещения, Буфер_ Как БуферДвоичныхДанных, ПозицияВБуфере_ Как Число, Количество_ Как Число)
void BeginRead(ConstParam<CallbackDescription> CallbackDescription_, ConstParam<BinaryDataBuffer> Buffer_, ConstParam<Number> PositionInBuffer_, ConstParam<Number> Number_) const {ESB_INVOKE_MEMB_PROC4(MethId::BeginRead, CallbackDescription_, Buffer_, PositionInBuffer_, Number_);};
// Процедура НачатьПереход(ОписаниеОповещения_ Как ОписаниеОповещения, Смещение_ Как Число, [НачальнаяПозиция_ Как ПозицияВПотоке])
void BeginSeek(ConstParam<CallbackDescription> CallbackDescription_, ConstParam<Number> Offset_, ConstParam<PositionInStream> InitialPosition_ = DefUndef<esb::PositionInStream>) const {ESB_INVOKE_MEMB_PROC3(MethId::BeginSeek, CallbackDescription_, Offset_, InitialPosition_);};
// Процедура НачатьУстановкуРазмера(ОписаниеОповещения_ Как ОписаниеОповещения, Размер_ Как Число)
void BeginSetSize(ConstParam<CallbackDescription> CallbackDescription_, ConstParam<Number> Size_) {ESB_INVOKE_MEMB_PROC2(MethId::BeginSetSize, CallbackDescription_, Size_);};
// Процедура НачатьЗапись(ОписаниеОповещения_ Как ОписаниеОповещения, Буфер_ Как БуферДвоичныхДанных, ПозицияВБуфере_ Как Число, Количество_ Как Число)
void BeginWrite(ConstParam<CallbackDescription> CallbackDescription_, ConstParam<BinaryDataBuffer> Buffer_, ConstParam<Number> PositionInBuffer_, ConstParam<Number> Count_) {ESB_INVOKE_MEMB_PROC4(MethId::BeginWrite, CallbackDescription_, Buffer_, PositionInBuffer_, Count_);};
// Процедура Закрыть()
void Close() {ESB_INVOKE_MEMB_PROC0(MethId::Close);};
// Функция ЗакрытьАсинх() Возвращает Обещание<>
Promise<> CloseAsync() {ESB_INVOKE_MEMB_FUNC0(esb::Promise<>, MethId::CloseAsync);};
// Функция КопироватьВ(ЦелевойПоток_ Как Поток, [Количество_ Как Число]) Возвращает Число
Number CopyTo(ConstParam<Stream> TargetStream_, ConstParam<Number> Count_ = DefUndef<esb::Number>) const {ESB_INVOKE_MEMB_FUNC2(esb::Number, MethId::CopyTo, TargetStream_, Count_);};
// Функция КопироватьВАсинх(ЦелевойПоток_ Как Поток, [Количество_ Как Число]) Возвращает Обещание<Число>
Promise<Number> CopyToAsync(ConstParam<Stream> TargetStream_, ConstParam<Number> Count_ = DefUndef<esb::Number>) {ESB_INVOKE_MEMB_FUNC2(esb::Promise<Number>, MethId::CopyToAsync, TargetStream_, Count_);};
// Функция ТекущаяПозиция() Возвращает Число
Number CurrentPosition() const {ESB_INVOKE_MEMB_FUNC0(esb::Number, MethId::CurrentPosition);};
// Процедура СброситьБуферы()
void Flush() const {ESB_INVOKE_MEMB_PROC0(MethId::Flush);};
// Функция СброситьБуферыАсинх() Возвращает Обещание<>
Promise<> FlushAsync() {ESB_INVOKE_MEMB_FUNC0(esb::Promise<>, MethId::FlushAsync);};
// Функция ПолучитьПотокТолькоДляЧтения() Возвращает Поток
Stream GetReadonlyStream() const {ESB_INVOKE_MEMB_FUNC0(esb::Stream, MethId::GetReadOnlyStream);};
// Функция Прочитать(Буфер_ Как БуферДвоичныхДанных, ПозицияВБуфере_ Как Число, Количество_ Как Число) Возвращает Число
Number Read(ConstParam<BinaryDataBuffer> Buffer_, ConstParam<Number> PositionInBuffer_, ConstParam<Number> Kolichestvo_) const {ESB_INVOKE_MEMB_FUNC3(esb::Number, MethId::Read, Buffer_, PositionInBuffer_, Kolichestvo_);};
// Функция ПрочитатьАсинх(Буфер_ Как БуферДвоичныхДанных, ПозицияВБуфере_ Как Число, Количество_ Как Число) Возвращает Обещание<Число>
Promise<Number> ReadAsync(ConstParam<BinaryDataBuffer> Buffer_, ConstParam<Number> PositionInBuffer_, ConstParam<Number> Number_) {ESB_INVOKE_MEMB_FUNC3(esb::Promise<Number>, MethId::ReadAsync, Buffer_, PositionInBuffer_, Number_);};
// Функция Перейти(Смещение_ Как Число, [НачальнаяПозиция_ Как ПозицияВПотоке]) Возвращает Число
Number Seek(ConstParam<Number> Offset_, ConstParam<PositionInStream> InitialPosition_ = DefUndef<esb::PositionInStream>) const {ESB_INVOKE_MEMB_FUNC2(esb::Number, MethId::Seek, Offset_, InitialPosition_);};
// Функция ПерейтиАсинх(Смещение_ Как Число, [НачальнаяПозиция_ Как ПозицияВПотоке]) Возвращает Обещание<Число>
Promise<Number> SeekAsync(ConstParam<Number> Offset_, ConstParam<PositionInStream> InitialPosition_ = DefUndef<esb::PositionInStream>) {ESB_INVOKE_MEMB_FUNC2(esb::Promise<Number>, MethId::SeekAsync, Offset_, InitialPosition_);};
// Процедура УстановитьРазмер(Размер_ Как Число)
void SetSize(ConstParam<Number> Size_) {ESB_INVOKE_MEMB_PROC1(MethId::SetSize, Size_);};
// Функция УстановитьРазмерАсинх(Размер_ Как Число) Возвращает Обещание<>
Promise<> SetSizeAsync(ConstParam<Number> Size_) {ESB_INVOKE_MEMB_FUNC1(esb::Promise<>, MethId::SetSizeAsync, Size_);};
// Функция Размер() Возвращает Число
Number Size() const {ESB_INVOKE_MEMB_FUNC0(esb::Number, MethId::Size);};
// Функция РазмерАсинх() Возвращает Обещание<Число>
Promise<Number> SizeAsync() {ESB_INVOKE_MEMB_FUNC0(esb::Promise<Number>, MethId::SizeAsync);};
// Процедура Записать(Буфер_ Как БуферДвоичныхДанных, ПозицияВБуфере_ Как Число, Количество_ Как Число)
void Write(ConstParam<BinaryDataBuffer> Buffer_, ConstParam<Number> PositionInBuffer_, ConstParam<Number> Number_) {ESB_INVOKE_MEMB_PROC3(MethId::Write, Buffer_, PositionInBuffer_, Number_);};
// Функция ЗаписатьАсинх(Буфер_ Как БуферДвоичныхДанных, ПозицияВБуфере_ Как Число, Количество_ Как Число) Возвращает Обещание<>
Promise<> WriteAsync(ConstParam<BinaryDataBuffer> Buffer_, ConstParam<Number> PositionInBuffer_, ConstParam<Number> Count_) {ESB_INVOKE_MEMB_FUNC3(esb::Promise<>, MethId::WriteAsync, Buffer_, PositionInBuffer_, Count_);};
public:
// Свойство [[ТолькоЧтение]] ДоступноЧтение Как Булево
Boolean GetCanRead() const  { ESB_INVOKE_PROPERTY_GET(esb::Boolean, PropId::CanRead); };
ESB_PROPERTY_FIELD_RO(Boolean, CanRead)
// Свойство [[ТолькоЧтение]] ДоступноИзменениеПозиции Как Булево
Boolean GetCanSeek() const  { ESB_INVOKE_PROPERTY_GET(esb::Boolean, PropId::CanSeek); };
ESB_PROPERTY_FIELD_RO(Boolean, CanSeek)
// Свойство [[ТолькоЧтение]] ДоступнаЗапись Как Булево
Boolean GetCanWrite() const  { ESB_INVOKE_PROPERTY_GET(esb::Boolean, PropId::CanWrite); };
ESB_PROPERTY_FIELD_RO(Boolean, CanWrite)
// Свойство [[ТолькоЧтение]] ИмяФайла Как Строка
String GetFileName() const  { ESB_INVOKE_PROPERTY_GET(esb::String, PropId::FileName); };
ESB_PROPERTY_FIELD_RO(String, FileName)
#endif	// #if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
public:
static ФайловыйПоток Создать(КонстПарам<Строка> ИмяФайла_, КонстПарам<РежимОткрытияФайла> РежимОткрытия_, КонстПарам<Число> РазмерБуфера_ = ПоУмолчаниюНеопределеноИли<esb::Число>) {ESB_INVOKE_CTOR3(ФайловыйПоток, ИмяФайла_, РежимОткрытия_, РазмерБуфера_);}
static ФайловыйПоток Создать(КонстПарам<Строка> ИмяФайла_, КонстПарам<РежимОткрытияФайла> РежимОткрытия_, КонстПарам<ДоступКФайлу> УровеньДоступа_, КонстПарам<Число> РазмерБуфера_ = ПоУмолчаниюНеопределеноИли<esb::Число>) {ESB_INVOKE_CTOR4(ФайловыйПоток, ИмяФайла_, РежимОткрытия_, УровеньДоступа_, РазмерБуфера_);}
public:
// Void BeginClose(CallbackDescription CallbackDescription_)
void НачатьЗакрытие(КонстПарам<ОписаниеОповещения> ОписаниеОповещения_) {ESB_INVOKE_MEMB_PROC1(MethId::BeginClose, ОписаниеОповещения_);};
// Void BeginCopyTo(CallbackDescription CallbackDescription_, Stream TargetStream_, Number Count_ = <default>)
void НачатьКопированиеВ(КонстПарам<ОписаниеОповещения> ОписаниеОповещения_, КонстПарам<Поток> ЦелевойПоток_, КонстПарам<Число> Количество_ = ПоУмолчаниюНеопределеноИли<esb::Число>) const {ESB_INVOKE_MEMB_PROC3(MethId::BeginCopyTo, ОписаниеОповещения_, ЦелевойПоток_, Количество_);};
// Void BeginFlush(CallbackDescription CallbackDescription_)
void НачатьСбросБуферов(КонстПарам<ОписаниеОповещения> ОписаниеОповещения_) const {ESB_INVOKE_MEMB_PROC1(MethId::BeginFlush, ОписаниеОповещения_);};
// Void BeginGetSize(CallbackDescription CallbackDescription_ = <default>)
void НачатьПолучениеРазмера(КонстПарам<ОписаниеОповещения> ОписаниеОповещения_ = ПоУмолчаниюНеопределеноИли<esb::ОписаниеОповещения>) const {ESB_INVOKE_MEMB_PROC1(MethId::BeginGetSize, ОписаниеОповещения_);};
// Void BeginRead(CallbackDescription CallbackDescription_, BinaryDataBuffer Buffer_, Number PositionInBuffer_, Number Number_)
void НачатьЧтение(КонстПарам<ОписаниеОповещения> ОписаниеОповещения_, КонстПарам<БуферДвоичныхДанных> Буфер_, КонстПарам<Число> ПозицияВБуфере_, КонстПарам<Число> Количество_) const {ESB_INVOKE_MEMB_PROC4(MethId::BeginRead, ОписаниеОповещения_, Буфер_, ПозицияВБуфере_, Количество_);};
// Void BeginSeek(CallbackDescription CallbackDescription_, Number Offset_, PositionInStream InitialPosition_ = <default>)
void НачатьПереход(КонстПарам<ОписаниеОповещения> ОписаниеОповещения_, КонстПарам<Число> Смещение_, КонстПарам<ПозицияВПотоке> НачальнаяПозиция_ = ПоУмолчаниюНеопределеноИли<esb::ПозицияВПотоке>) const {ESB_INVOKE_MEMB_PROC3(MethId::BeginSeek, ОписаниеОповещения_, Смещение_, НачальнаяПозиция_);};
// Void BeginSetSize(CallbackDescription CallbackDescription_, Number Size_)
void НачатьУстановкуРазмера(КонстПарам<ОписаниеОповещения> ОписаниеОповещения_, КонстПарам<Число> Размер_) {ESB_INVOKE_MEMB_PROC2(MethId::BeginSetSize, ОписаниеОповещения_, Размер_);};
// Void BeginWrite(CallbackDescription CallbackDescription_, BinaryDataBuffer Buffer_, Number PositionInBuffer_, Number Count_)
void НачатьЗапись(КонстПарам<ОписаниеОповещения> ОписаниеОповещения_, КонстПарам<БуферДвоичныхДанных> Буфер_, КонстПарам<Число> ПозицияВБуфере_, КонстПарам<Число> Количество_) {ESB_INVOKE_MEMB_PROC4(MethId::BeginWrite, ОписаниеОповещения_, Буфер_, ПозицияВБуфере_, Количество_);};
// Void Close()
void Закрыть() {ESB_INVOKE_MEMB_PROC0(MethId::Close);};
// Function CloseAsync() -> Promise<>
Обещание<> ЗакрытьАсинх() {ESB_INVOKE_MEMB_FUNC0(esb::Обещание<>, MethId::CloseAsync);};
// Function CopyTo(Stream TargetStream_, Number Count_ = <default>) -> Number
Число КопироватьВ(КонстПарам<Поток> ЦелевойПоток_, КонстПарам<Число> Количество_ = ПоУмолчаниюНеопределеноИли<esb::Число>) const {ESB_INVOKE_MEMB_FUNC2(esb::Число, MethId::CopyTo, ЦелевойПоток_, Количество_);};
// Function CopyToAsync(Stream TargetStream_, Number Count_ = <default>) -> Promise<Number>
Обещание<Число> КопироватьВАсинх(КонстПарам<Поток> ЦелевойПоток_, КонстПарам<Число> Количество_ = ПоУмолчаниюНеопределеноИли<esb::Число>) {ESB_INVOKE_MEMB_FUNC2(esb::Обещание<Число>, MethId::CopyToAsync, ЦелевойПоток_, Количество_);};
// Function CurrentPosition() -> Number
Число ТекущаяПозиция() const {ESB_INVOKE_MEMB_FUNC0(esb::Число, MethId::CurrentPosition);};
// Void Flush()
void СброситьБуферы() const {ESB_INVOKE_MEMB_PROC0(MethId::Flush);};
// Function FlushAsync() -> Promise<>
Обещание<> СброситьБуферыАсинх() {ESB_INVOKE_MEMB_FUNC0(esb::Обещание<>, MethId::FlushAsync);};
// Function GetReadonlyStream() -> Stream
Поток ПолучитьПотокТолькоДляЧтения() const {ESB_INVOKE_MEMB_FUNC0(esb::Поток, MethId::GetReadOnlyStream);};
// Function Read(BinaryDataBuffer Buffer_, Number PositionInBuffer_, Number Kolichestvo_) -> Number
Число Прочитать(КонстПарам<БуферДвоичныхДанных> Буфер_, КонстПарам<Число> ПозицияВБуфере_, КонстПарам<Число> Количество_) const {ESB_INVOKE_MEMB_FUNC3(esb::Число, MethId::Read, Буфер_, ПозицияВБуфере_, Количество_);};
// Function ReadAsync(BinaryDataBuffer Buffer_, Number PositionInBuffer_, Number Number_) -> Promise<Number>
Обещание<Число> ПрочитатьАсинх(КонстПарам<БуферДвоичныхДанных> Буфер_, КонстПарам<Число> ПозицияВБуфере_, КонстПарам<Число> Количество_) {ESB_INVOKE_MEMB_FUNC3(esb::Обещание<Число>, MethId::ReadAsync, Буфер_, ПозицияВБуфере_, Количество_);};
// Function Seek(Number Offset_, PositionInStream InitialPosition_ = <default>) -> Number
Число Перейти(КонстПарам<Число> Смещение_, КонстПарам<ПозицияВПотоке> НачальнаяПозиция_ = ПоУмолчаниюНеопределеноИли<esb::ПозицияВПотоке>) const {ESB_INVOKE_MEMB_FUNC2(esb::Число, MethId::Seek, Смещение_, НачальнаяПозиция_);};
// Function SeekAsync(Number Offset_, PositionInStream InitialPosition_ = <default>) -> Promise<Number>
Обещание<Число> ПерейтиАсинх(КонстПарам<Число> Смещение_, КонстПарам<ПозицияВПотоке> НачальнаяПозиция_ = ПоУмолчаниюНеопределеноИли<esb::ПозицияВПотоке>) {ESB_INVOKE_MEMB_FUNC2(esb::Обещание<Число>, MethId::SeekAsync, Смещение_, НачальнаяПозиция_);};
// Void SetSize(Number Size_)
void УстановитьРазмер(КонстПарам<Число> Размер_) {ESB_INVOKE_MEMB_PROC1(MethId::SetSize, Размер_);};
// Function SetSizeAsync(Number Size_) -> Promise<>
Обещание<> УстановитьРазмерАсинх(КонстПарам<Число> Размер_) {ESB_INVOKE_MEMB_FUNC1(esb::Обещание<>, MethId::SetSizeAsync, Размер_);};
// Function Size() -> Number
Число Размер() const {ESB_INVOKE_MEMB_FUNC0(esb::Число, MethId::Size);};
// Function SizeAsync() -> Promise<Number>
Обещание<Число> РазмерАсинх() {ESB_INVOKE_MEMB_FUNC0(esb::Обещание<Число>, MethId::SizeAsync);};
// Void Write(BinaryDataBuffer Buffer_, Number PositionInBuffer_, Number Number_)
void Записать(КонстПарам<БуферДвоичныхДанных> Буфер_, КонстПарам<Число> ПозицияВБуфере_, КонстПарам<Число> Количество_) {ESB_INVOKE_MEMB_PROC3(MethId::Write, Буфер_, ПозицияВБуфере_, Количество_);};
// Function WriteAsync(BinaryDataBuffer Buffer_, Number PositionInBuffer_, Number Count_) -> Promise<>
Обещание<> ЗаписатьАсинх(КонстПарам<БуферДвоичныхДанных> Буфер_, КонстПарам<Число> ПозицияВБуфере_, КонстПарам<Число> Количество_) {ESB_INVOKE_MEMB_FUNC3(esb::Обещание<>, MethId::WriteAsync, Буфер_, ПозицияВБуфере_, Количество_);};
public:
// Property [[ReadOnly]] CanRead -> Boolean
Булево ПолучитьДоступноЧтение() const  { ESB_INVOKE_PROPERTY_GET(esb::Булево, PropId::CanRead); };
ESB_PROPERTY_FIELD_RO(Булево, ДоступноЧтение)
// Property [[ReadOnly]] CanSeek -> Boolean
Булево ПолучитьДоступноИзменениеПозиции() const  { ESB_INVOKE_PROPERTY_GET(esb::Булево, PropId::CanSeek); };
ESB_PROPERTY_FIELD_RO(Булево, ДоступноИзменениеПозиции)
// Property [[ReadOnly]] CanWrite -> Boolean
Булево ПолучитьДоступнаЗапись() const  { ESB_INVOKE_PROPERTY_GET(esb::Булево, PropId::CanWrite); };
ESB_PROPERTY_FIELD_RO(Булево, ДоступнаЗапись)
// Property [[ReadOnly]] FileName -> String
Строка ПолучитьИмяФайла() const  { ESB_INVOKE_PROPERTY_GET(esb::Строка, PropId::FileName); };
ESB_PROPERTY_FIELD_RO(Строка, ИмяФайла)
#endif	// #if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
};


ESB_CLASS_OBJECT(MemoryStream)
{	ESB_CLASS_IMPLEMENT_OBJECT(MemoryStream)
#if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
public:
static MemoryStream Create() { ESB_INVOKE_CTOR0(MemoryStream); }
static MemoryStream Create(ConstParam<BinaryDataBuffer> Buffer_) {ESB_INVOKE_CTOR1(MemoryStream, Buffer_);}
static MemoryStream Create(ConstParam<Number> InitialCapacity_) {ESB_INVOKE_CTOR1(MemoryStream, InitialCapacity_);}
public:
// Процедура НачатьЗакрытие(ОписаниеОповещения_ Как ОписаниеОповещения)
void BeginClose(ConstParam<CallbackDescription> CallbackDescription_) {ESB_INVOKE_MEMB_PROC1(MethId::BeginClose, CallbackDescription_);};
// Процедура НачатьКопированиеВ(ОписаниеОповещения_ Как ОписаниеОповещения, ЦелевойПоток_ Как Поток, [Количество_ Как Число])
void BeginCopyTo(ConstParam<CallbackDescription> CallbackDescription_, ConstParam<Stream> TargetStream_, ConstParam<Number> Count_ = DefUndef<esb::Number>) const {ESB_INVOKE_MEMB_PROC3(MethId::BeginCopyTo, CallbackDescription_, TargetStream_, Count_);};
// Процедура НачатьСбросБуферов(ОписаниеОповещения_ Как ОписаниеОповещения)
void BeginFlush(ConstParam<CallbackDescription> CallbackDescription_) const {ESB_INVOKE_MEMB_PROC1(MethId::BeginFlush, CallbackDescription_);};
// Процедура НачатьПолучениеРазмера([ОписаниеОповещения_ Как ОписаниеОповещения])
void BeginGetSize(ConstParam<CallbackDescription> CallbackDescription_ = DefUndef<esb::CallbackDescription>) const {ESB_INVOKE_MEMB_PROC1(MethId::BeginGetSize, CallbackDescription_);};
// Процедура НачатьЧтение(ОписаниеОповещения_ Как ОписаниеОповещения, Буфер_ Как БуферДвоичныхДанных, ПозицияВБуфере_ Как Число, Количество_ Как Число)
void BeginRead(ConstParam<CallbackDescription> CallbackDescription_, ConstParam<BinaryDataBuffer> Buffer_, ConstParam<Number> PositionInBuffer_, ConstParam<Number> Number_) const {ESB_INVOKE_MEMB_PROC4(MethId::BeginRead, CallbackDescription_, Buffer_, PositionInBuffer_, Number_);};
// Процедура НачатьПереход(ОписаниеОповещения_ Как ОписаниеОповещения, Смещение_ Как Число, [НачальнаяПозиция_ Как ПозицияВПотоке])
void BeginSeek(ConstParam<CallbackDescription> CallbackDescription_, ConstParam<Number> Offset_, ConstParam<PositionInStream> InitialPosition_ = DefUndef<esb::PositionInStream>) const {ESB_INVOKE_MEMB_PROC3(MethId::BeginSeek, CallbackDescription_, Offset_, InitialPosition_);};
// Процедура НачатьУстановкуРазмера(ОписаниеОповещения_ Как ОписаниеОповещения, Размер_ Как Число)
void BeginSetSize(ConstParam<CallbackDescription> CallbackDescription_, ConstParam<Number> Size_) {ESB_INVOKE_MEMB_PROC2(MethId::BeginSetSize, CallbackDescription_, Size_);};
// Процедура НачатьЗапись(ОписаниеОповещения_ Как ОписаниеОповещения, Буфер_ Как БуферДвоичныхДанных, ПозицияВБуфере_ Как Число, Количество_ Как Число)
void BeginWrite(ConstParam<CallbackDescription> CallbackDescription_, ConstParam<BinaryDataBuffer> Buffer_, ConstParam<Number> PositionInBuffer_, ConstParam<Number> Count_) {ESB_INVOKE_MEMB_PROC4(MethId::BeginWrite, CallbackDescription_, Buffer_, PositionInBuffer_, Count_);};
// Процедура Закрыть()
void Close() {ESB_INVOKE_MEMB_PROC0(MethId::Close);};
// Функция ЗакрытьАсинх() Возвращает Обещание<>
Promise<> CloseAsync() {ESB_INVOKE_MEMB_FUNC0(esb::Promise<>, MethId::CloseAsync);};
// Функция КопироватьВ(ЦелевойПоток_ Как Поток, [Количество_ Как Число]) Возвращает Число
Number CopyTo(ConstParam<Stream> TargetStream_, ConstParam<Number> Count_ = DefUndef<esb::Number>) const {ESB_INVOKE_MEMB_FUNC2(esb::Number, MethId::CopyTo, TargetStream_, Count_);};
// Функция КопироватьВАсинх(ЦелевойПоток_ Как Поток, [Количество_ Как Число]) Возвращает Обещание<Число>
Promise<Number> CopyToAsync(ConstParam<Stream> TargetStream_, ConstParam<Number> Count_ = DefUndef<esb::Number>) {ESB_INVOKE_MEMB_FUNC2(esb::Promise<Number>, MethId::CopyToAsync, TargetStream_, Count_);};
// Функция ТекущаяПозиция() Возвращает Число
Number CurrentPosition() const {ESB_INVOKE_MEMB_FUNC0(esb::Number, MethId::CurrentPosition);};
// Процедура СброситьБуферы()
void Flush() const {ESB_INVOKE_MEMB_PROC0(MethId::Flush);};
// Функция СброситьБуферыАсинх() Возвращает Обещание<>
Promise<> FlushAsync() {ESB_INVOKE_MEMB_FUNC0(esb::Promise<>, MethId::FlushAsync);};
// Функция ПолучитьПотокТолькоДляЧтения() Возвращает Поток
Stream GetReadonlyStream() const {ESB_INVOKE_MEMB_FUNC0(esb::Stream, MethId::GetReadOnlyStream);};
// Функция Прочитать(Буфер_ Как БуферДвоичныхДанных, ПозицияВБуфере_ Как Число, Количество_ Как Число) Возвращает Число
Number Read(ConstParam<BinaryDataBuffer> Buffer_, ConstParam<Number> PositionInBuffer_, ConstParam<Number> Kolichestvo_) const {ESB_INVOKE_MEMB_FUNC3(esb::Number, MethId::Read, Buffer_, PositionInBuffer_, Kolichestvo_);};
// Функция ПрочитатьАсинх(Буфер_ Как БуферДвоичныхДанных, ПозицияВБуфере_ Как Число, Количество_ Как Число) Возвращает Обещание<Число>
Promise<Number> ReadAsync(ConstParam<BinaryDataBuffer> Buffer_, ConstParam<Number> PositionInBuffer_, ConstParam<Number> Number_) {ESB_INVOKE_MEMB_FUNC3(esb::Promise<Number>, MethId::ReadAsync, Buffer_, PositionInBuffer_, Number_);};
// Функция Перейти(Смещение_ Как Число, [НачальнаяПозиция_ Как ПозицияВПотоке]) Возвращает Число
Number Seek(ConstParam<Number> Offset_, ConstParam<PositionInStream> InitialPosition_ = DefUndef<esb::PositionInStream>) const {ESB_INVOKE_MEMB_FUNC2(esb::Number, MethId::Seek, Offset_, InitialPosition_);};
// Функция ПерейтиАсинх(Смещение_ Как Число, [НачальнаяПозиция_ Как ПозицияВПотоке]) Возвращает Обещание<Число>
Promise<Number> SeekAsync(ConstParam<Number> Offset_, ConstParam<PositionInStream> InitialPosition_ = DefUndef<esb::PositionInStream>) {ESB_INVOKE_MEMB_FUNC2(esb::Promise<Number>, MethId::SeekAsync, Offset_, InitialPosition_);};
// Процедура УстановитьРазмер(Размер_ Как Число)
void SetSize(ConstParam<Number> Size_) {ESB_INVOKE_MEMB_PROC1(MethId::SetSize, Size_);};
// Функция УстановитьРазмерАсинх(Размер_ Как Число) Возвращает Обещание<>
Promise<> SetSizeAsync(ConstParam<Number> Size_) {ESB_INVOKE_MEMB_FUNC1(esb::Promise<>, MethId::SetSizeAsync, Size_);};
// Функция Размер() Возвращает Число
Number Size() const {ESB_INVOKE_MEMB_FUNC0(esb::Number, MethId::Size);};
// Функция РазмерАсинх() Возвращает Обещание<Число>
Promise<Number> SizeAsync() {ESB_INVOKE_MEMB_FUNC0(esb::Promise<Number>, MethId::SizeAsync);};
// Процедура Записать(Буфер_ Как БуферДвоичныхДанных, ПозицияВБуфере_ Как Число, Количество_ Как Число)
void Write(ConstParam<BinaryDataBuffer> Buffer_, ConstParam<Number> PositionInBuffer_, ConstParam<Number> Number_) {ESB_INVOKE_MEMB_PROC3(MethId::Write, Buffer_, PositionInBuffer_, Number_);};
// Функция ЗаписатьАсинх(Буфер_ Как БуферДвоичныхДанных, ПозицияВБуфере_ Как Число, Количество_ Как Число) Возвращает Обещание<>
Promise<> WriteAsync(ConstParam<BinaryDataBuffer> Buffer_, ConstParam<Number> PositionInBuffer_, ConstParam<Number> Count_) {ESB_INVOKE_MEMB_FUNC3(esb::Promise<>, MethId::WriteAsync, Buffer_, PositionInBuffer_, Count_);};
// Функция ЗакрытьИПолучитьДвоичныеДанные() Возвращает ДвоичныеДанные
BinaryData CloseAndGetBinaryData() {ESB_INVOKE_MEMB_FUNC0(esb::BinaryData, MethId::CloseAndGetBinaryData);};
public:
// Свойство [[ТолькоЧтение]] ДоступноЧтение Как Булево
Boolean GetCanRead() const  { ESB_INVOKE_PROPERTY_GET(esb::Boolean, PropId::CanRead); };
ESB_PROPERTY_FIELD_RO(Boolean, CanRead)
// Свойство [[ТолькоЧтение]] ДоступноИзменениеПозиции Как Булево
Boolean GetCanSeek() const  { ESB_INVOKE_PROPERTY_GET(esb::Boolean, PropId::CanSeek); };
ESB_PROPERTY_FIELD_RO(Boolean, CanSeek)
// Свойство [[ТолькоЧтение]] ДоступнаЗапись Как Булево
Boolean GetCanWrite() const  { ESB_INVOKE_PROPERTY_GET(esb::Boolean, PropId::CanWrite); };
ESB_PROPERTY_FIELD_RO(Boolean, CanWrite)
#endif	// #if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
public:
static ПотокВПамяти Создать() { ESB_INVOKE_CTOR0(ПотокВПамяти); }
static ПотокВПамяти Создать(КонстПарам<БуферДвоичныхДанных> Буфер_) {ESB_INVOKE_CTOR1(ПотокВПамяти, Буфер_);}
static ПотокВПамяти Создать(КонстПарам<Число> НачальнаяЕмкость_) {ESB_INVOKE_CTOR1(ПотокВПамяти, НачальнаяЕмкость_);}
public:
// Void BeginClose(CallbackDescription CallbackDescription_)
void НачатьЗакрытие(КонстПарам<ОписаниеОповещения> ОписаниеОповещения_) {ESB_INVOKE_MEMB_PROC1(MethId::BeginClose, ОписаниеОповещения_);};
// Void BeginCopyTo(CallbackDescription CallbackDescription_, Stream TargetStream_, Number Count_ = <default>)
void НачатьКопированиеВ(КонстПарам<ОписаниеОповещения> ОписаниеОповещения_, КонстПарам<Поток> ЦелевойПоток_, КонстПарам<Число> Количество_ = ПоУмолчаниюНеопределеноИли<esb::Число>) const {ESB_INVOKE_MEMB_PROC3(MethId::BeginCopyTo, ОписаниеОповещения_, ЦелевойПоток_, Количество_);};
// Void BeginFlush(CallbackDescription CallbackDescription_)
void НачатьСбросБуферов(КонстПарам<ОписаниеОповещения> ОписаниеОповещения_) const {ESB_INVOKE_MEMB_PROC1(MethId::BeginFlush, ОписаниеОповещения_);};
// Void BeginGetSize(CallbackDescription CallbackDescription_ = <default>)
void НачатьПолучениеРазмера(КонстПарам<ОписаниеОповещения> ОписаниеОповещения_ = ПоУмолчаниюНеопределеноИли<esb::ОписаниеОповещения>) const {ESB_INVOKE_MEMB_PROC1(MethId::BeginGetSize, ОписаниеОповещения_);};
// Void BeginRead(CallbackDescription CallbackDescription_, BinaryDataBuffer Buffer_, Number PositionInBuffer_, Number Number_)
void НачатьЧтение(КонстПарам<ОписаниеОповещения> ОписаниеОповещения_, КонстПарам<БуферДвоичныхДанных> Буфер_, КонстПарам<Число> ПозицияВБуфере_, КонстПарам<Число> Количество_) const {ESB_INVOKE_MEMB_PROC4(MethId::BeginRead, ОписаниеОповещения_, Буфер_, ПозицияВБуфере_, Количество_);};
// Void BeginSeek(CallbackDescription CallbackDescription_, Number Offset_, PositionInStream InitialPosition_ = <default>)
void НачатьПереход(КонстПарам<ОписаниеОповещения> ОписаниеОповещения_, КонстПарам<Число> Смещение_, КонстПарам<ПозицияВПотоке> НачальнаяПозиция_ = ПоУмолчаниюНеопределеноИли<esb::ПозицияВПотоке>) const {ESB_INVOKE_MEMB_PROC3(MethId::BeginSeek, ОписаниеОповещения_, Смещение_, НачальнаяПозиция_);};
// Void BeginSetSize(CallbackDescription CallbackDescription_, Number Size_)
void НачатьУстановкуРазмера(КонстПарам<ОписаниеОповещения> ОписаниеОповещения_, КонстПарам<Число> Размер_) {ESB_INVOKE_MEMB_PROC2(MethId::BeginSetSize, ОписаниеОповещения_, Размер_);};
// Void BeginWrite(CallbackDescription CallbackDescription_, BinaryDataBuffer Buffer_, Number PositionInBuffer_, Number Count_)
void НачатьЗапись(КонстПарам<ОписаниеОповещения> ОписаниеОповещения_, КонстПарам<БуферДвоичныхДанных> Буфер_, КонстПарам<Число> ПозицияВБуфере_, КонстПарам<Число> Количество_) {ESB_INVOKE_MEMB_PROC4(MethId::BeginWrite, ОписаниеОповещения_, Буфер_, ПозицияВБуфере_, Количество_);};
// Void Close()
void Закрыть() {ESB_INVOKE_MEMB_PROC0(MethId::Close);};
// Function CloseAsync() -> Promise<>
Обещание<> ЗакрытьАсинх() {ESB_INVOKE_MEMB_FUNC0(esb::Обещание<>, MethId::CloseAsync);};
// Function CopyTo(Stream TargetStream_, Number Count_ = <default>) -> Number
Число КопироватьВ(КонстПарам<Поток> ЦелевойПоток_, КонстПарам<Число> Количество_ = ПоУмолчаниюНеопределеноИли<esb::Число>) const {ESB_INVOKE_MEMB_FUNC2(esb::Число, MethId::CopyTo, ЦелевойПоток_, Количество_);};
// Function CopyToAsync(Stream TargetStream_, Number Count_ = <default>) -> Promise<Number>
Обещание<Число> КопироватьВАсинх(КонстПарам<Поток> ЦелевойПоток_, КонстПарам<Число> Количество_ = ПоУмолчаниюНеопределеноИли<esb::Число>) {ESB_INVOKE_MEMB_FUNC2(esb::Обещание<Число>, MethId::CopyToAsync, ЦелевойПоток_, Количество_);};
// Function CurrentPosition() -> Number
Число ТекущаяПозиция() const {ESB_INVOKE_MEMB_FUNC0(esb::Число, MethId::CurrentPosition);};
// Void Flush()
void СброситьБуферы() const {ESB_INVOKE_MEMB_PROC0(MethId::Flush);};
// Function FlushAsync() -> Promise<>
Обещание<> СброситьБуферыАсинх() {ESB_INVOKE_MEMB_FUNC0(esb::Обещание<>, MethId::FlushAsync);};
// Function GetReadonlyStream() -> Stream
Поток ПолучитьПотокТолькоДляЧтения() const {ESB_INVOKE_MEMB_FUNC0(esb::Поток, MethId::GetReadOnlyStream);};
// Function Read(BinaryDataBuffer Buffer_, Number PositionInBuffer_, Number Kolichestvo_) -> Number
Число Прочитать(КонстПарам<БуферДвоичныхДанных> Буфер_, КонстПарам<Число> ПозицияВБуфере_, КонстПарам<Число> Количество_) const {ESB_INVOKE_MEMB_FUNC3(esb::Число, MethId::Read, Буфер_, ПозицияВБуфере_, Количество_);};
// Function ReadAsync(BinaryDataBuffer Buffer_, Number PositionInBuffer_, Number Number_) -> Promise<Number>
Обещание<Число> ПрочитатьАсинх(КонстПарам<БуферДвоичныхДанных> Буфер_, КонстПарам<Число> ПозицияВБуфере_, КонстПарам<Число> Количество_) {ESB_INVOKE_MEMB_FUNC3(esb::Обещание<Число>, MethId::ReadAsync, Буфер_, ПозицияВБуфере_, Количество_);};
// Function Seek(Number Offset_, PositionInStream InitialPosition_ = <default>) -> Number
Число Перейти(КонстПарам<Число> Смещение_, КонстПарам<ПозицияВПотоке> НачальнаяПозиция_ = ПоУмолчаниюНеопределеноИли<esb::ПозицияВПотоке>) const {ESB_INVOKE_MEMB_FUNC2(esb::Число, MethId::Seek, Смещение_, НачальнаяПозиция_);};
// Function SeekAsync(Number Offset_, PositionInStream InitialPosition_ = <default>) -> Promise<Number>
Обещание<Число> ПерейтиАсинх(КонстПарам<Число> Смещение_, КонстПарам<ПозицияВПотоке> НачальнаяПозиция_ = ПоУмолчаниюНеопределеноИли<esb::ПозицияВПотоке>) {ESB_INVOKE_MEMB_FUNC2(esb::Обещание<Число>, MethId::SeekAsync, Смещение_, НачальнаяПозиция_);};
// Void SetSize(Number Size_)
void УстановитьРазмер(КонстПарам<Число> Размер_) {ESB_INVOKE_MEMB_PROC1(MethId::SetSize, Размер_);};
// Function SetSizeAsync(Number Size_) -> Promise<>
Обещание<> УстановитьРазмерАсинх(КонстПарам<Число> Размер_) {ESB_INVOKE_MEMB_FUNC1(esb::Обещание<>, MethId::SetSizeAsync, Размер_);};
// Function Size() -> Number
Число Размер() const {ESB_INVOKE_MEMB_FUNC0(esb::Число, MethId::Size);};
// Function SizeAsync() -> Promise<Number>
Обещание<Число> РазмерАсинх() {ESB_INVOKE_MEMB_FUNC0(esb::Обещание<Число>, MethId::SizeAsync);};
// Void Write(BinaryDataBuffer Buffer_, Number PositionInBuffer_, Number Number_)
void Записать(КонстПарам<БуферДвоичныхДанных> Буфер_, КонстПарам<Число> ПозицияВБуфере_, КонстПарам<Число> Количество_) {ESB_INVOKE_MEMB_PROC3(MethId::Write, Буфер_, ПозицияВБуфере_, Количество_);};
// Function WriteAsync(BinaryDataBuffer Buffer_, Number PositionInBuffer_, Number Count_) -> Promise<>
Обещание<> ЗаписатьАсинх(КонстПарам<БуферДвоичныхДанных> Буфер_, КонстПарам<Число> ПозицияВБуфере_, КонстПарам<Число> Количество_) {ESB_INVOKE_MEMB_FUNC3(esb::Обещание<>, MethId::WriteAsync, Буфер_, ПозицияВБуфере_, Количество_);};
// Function CloseAndGetBinaryData() -> BinaryData
ДвоичныеДанные ЗакрытьИПолучитьДвоичныеДанные() {ESB_INVOKE_MEMB_FUNC0(esb::ДвоичныеДанные, MethId::CloseAndGetBinaryData);};
public:
// Property [[ReadOnly]] CanRead -> Boolean
Булево ПолучитьДоступноЧтение() const  { ESB_INVOKE_PROPERTY_GET(esb::Булево, PropId::CanRead); };
ESB_PROPERTY_FIELD_RO(Булево, ДоступноЧтение)
// Property [[ReadOnly]] CanSeek -> Boolean
Булево ПолучитьДоступноИзменениеПозиции() const  { ESB_INVOKE_PROPERTY_GET(esb::Булево, PropId::CanSeek); };
ESB_PROPERTY_FIELD_RO(Булево, ДоступноИзменениеПозиции)
// Property [[ReadOnly]] CanWrite -> Boolean
Булево ПолучитьДоступнаЗапись() const  { ESB_INVOKE_PROPERTY_GET(esb::Булево, PropId::CanWrite); };
ESB_PROPERTY_FIELD_RO(Булево, ДоступнаЗапись)
#endif	// #if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
};


}	// end namespace esb 



namespace esb 
{
		// Picture объявлено в es1_main.h
#if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
		//inline BinaryData Picture::GetBinaryData() { ESB_INVOKE_MEMB_FUNC0(esb::BinaryData, MethId::GetBinaryData); };
		inline UndefOr<BinaryData> Picture::GetBinaryData(ConstParam<Boolean> AllVariants_ /*= DefUndef<esb::Boolean>*/, ConstParam<Map> SelectVariant_ /*= DefUndef<esb::Map>*/) { ESB_INVOKE_MEMB_FUNC2(UndefOr<esb::BinaryData>, MethId::GetBinaryData, AllVariants_, SelectVariant_); };
#endif
#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
		//inline ДвоичныеДанные Картинка::ПолучитьДвоичныеДанные() { ESB_INVOKE_MEMB_FUNC0(esb::ДвоичныеДанные, MethId::GetBinaryData); };
		inline НеопределеноИли<ДвоичныеДанные> Picture::ПолучитьДвоичныеДанные(КонстПарам<Булево> ВсеВарианты_ /*= ПоУмолчаниюНеопределеноИли<esb::Булево>*/, КонстПарам<Соответствие> ПодобратьВариант_ /*= ПоУмолчаниюНеопределеноИли<esb::Соответствие>*/) { ESB_INVOKE_MEMB_FUNC2(НеопределеноИли<esb::ДвоичныеДанные>, MethId::GetBinaryData, ВсеВарианты_, ПодобратьВариант_); };
#endif
}




// определение АПИ зависящего от типов esb_file
//
#if ESB_API_PROVIDER_INCLUDE(ext0xA) || ESB_API_PROVIDER_INCLUDE(core83)
#	include "esdisp_api.h"
#endif
//
namespace esb
{
	/////////////////////////////////////////////////
#if ESB_API_PROVIDER_INCLUDE(core83)
#if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
// Функция Base64Значение(Строка_ Как Строка) Возвращает ДвоичныеДанные
inline BinaryData Base64Value(ConstParam<String> Line_) {ESB_INVOKE_API_FUNC1(BinaryData, core83, Base64Value, Line_);};
// Функция Base64Строка(Значение_ Как ДвоичныеДанные) Возвращает Строка
inline String Base64String(ConstParam<BinaryData> Value_) {ESB_INVOKE_API_FUNC1(String, core83, Base64String, Value_);};
#endif	// #if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
// Function Base64Value(String Line_) -> BinaryData
inline ДвоичныеДанные Base64Значение(КонстПарам<Строка> Строка_) {ESB_INVOKE_API_FUNC1(ДвоичныеДанные, core83, Base64Value, Строка_);};
// Function Base64String(BinaryData Value_) -> String
inline Строка Base64Строка(КонстПарам<ДвоичныеДанные> Значение_) {ESB_INVOKE_API_FUNC1(Строка, core83, Base64String, Значение_);};
#endif	// #if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
#endif	// #if ESB_API_PROVIDER_INCLUDE(core83)
#if ESB_API_PROVIDER_INCLUDE(pdf)
#if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
// Функция ДокументЗащищенПаролем(Поток_ Как ОдинИз<ФайловыйПоток, ПотокВПамяти, Поток>) Возвращает Булево
inline Boolean DocumentIsPasswordProtected(ConstParam<FileStream> Stream_) {ESB_INVOKE_API_FUNC1(Boolean, pdf, DocumentIsPasswordProtected, Stream_);};
inline Boolean DocumentIsPasswordProtected(ConstParam<MemoryStream> Stream_) {ESB_INVOKE_API_FUNC1(Boolean, pdf, DocumentIsPasswordProtected, Stream_);};
inline Boolean DocumentIsPasswordProtected(ConstParam<Stream> Stream_) {ESB_INVOKE_API_FUNC1(Boolean, pdf, DocumentIsPasswordProtected, Stream_);};
// Функция ДокументЗащищенПаролем(Поток_ Как ОдинИз<ФайловыйПоток, ПотокВПамяти, Поток>) Возвращает Булево
inline Boolean DocumentIsPasswordProtected(ConstParam<String> FileName_) {ESB_INVOKE_API_FUNC1(Boolean, pdf, DocumentIsPasswordProtected, FileName_);};
// Функция ДокументЗащищенПаролемАсинх(Поток_ Как ОдинИз<ФайловыйПоток, ПотокВПамяти, Поток>) Возвращает Обещание<>
inline Promise<> DocumentIsPasswordProtectedAsync(ConstParam<FileStream> Stream_) {ESB_INVOKE_API_FUNC1(Promise<>, pdf, DocumentIsPasswordProtectedAsync, Stream_);};
inline Promise<> DocumentIsPasswordProtectedAsync(ConstParam<MemoryStream> Stream_) {ESB_INVOKE_API_FUNC1(Promise<>, pdf, DocumentIsPasswordProtectedAsync, Stream_);};
inline Promise<> DocumentIsPasswordProtectedAsync(ConstParam<Stream> Stream_) {ESB_INVOKE_API_FUNC1(Promise<>, pdf, DocumentIsPasswordProtectedAsync, Stream_);};
// Функция ДокументЗащищенПаролемАсинх(Поток_ Как ОдинИз<ФайловыйПоток, ПотокВПамяти, Поток>) Возвращает Обещание<>
inline Promise<> DocumentIsPasswordProtectedAsync(ConstParam<String> FileName_) {ESB_INVOKE_API_FUNC1(Promise<>, pdf, DocumentIsPasswordProtectedAsync, FileName_);};
#endif	// #if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
// Function DocumentIsPasswordProtected(OneOf<FileStream, MemoryStream, Stream> Stream_) -> Boolean
inline Булево ДокументЗащищенПаролем(КонстПарам<ФайловыйПоток> Поток_) {ESB_INVOKE_API_FUNC1(Булево, pdf, DocumentIsPasswordProtected, Поток_);};
inline Булево ДокументЗащищенПаролем(КонстПарам<ПотокВПамяти> Поток_) {ESB_INVOKE_API_FUNC1(Булево, pdf, DocumentIsPasswordProtected, Поток_);};
inline Булево ДокументЗащищенПаролем(КонстПарам<Поток> Поток_) {ESB_INVOKE_API_FUNC1(Булево, pdf, DocumentIsPasswordProtected, Поток_);};
// Function DocumentIsPasswordProtected(OneOf<FileStream, MemoryStream, Stream> Stream_) -> Boolean
inline Булево ДокументЗащищенПаролем(КонстПарам<Строка> ИмяФайла_) {ESB_INVOKE_API_FUNC1(Булево, pdf, DocumentIsPasswordProtected, ИмяФайла_);};
// Function DocumentIsPasswordProtectedAsync(OneOf<FileStream, MemoryStream, Stream> Stream_) -> Promise<>
inline Обещание<> ДокументЗащищенПаролемАсинх(КонстПарам<ФайловыйПоток> Поток_) {ESB_INVOKE_API_FUNC1(Обещание<>, pdf, DocumentIsPasswordProtectedAsync, Поток_);};
inline Обещание<> ДокументЗащищенПаролемАсинх(КонстПарам<ПотокВПамяти> Поток_) {ESB_INVOKE_API_FUNC1(Обещание<>, pdf, DocumentIsPasswordProtectedAsync, Поток_);};
inline Обещание<> ДокументЗащищенПаролемАсинх(КонстПарам<Поток> Поток_) {ESB_INVOKE_API_FUNC1(Обещание<>, pdf, DocumentIsPasswordProtectedAsync, Поток_);};
// Function DocumentIsPasswordProtectedAsync(OneOf<FileStream, MemoryStream, Stream> Stream_) -> Promise<>
inline Обещание<> ДокументЗащищенПаролемАсинх(КонстПарам<Строка> ИмяФайла_) {ESB_INVOKE_API_FUNC1(Обещание<>, pdf, DocumentIsPasswordProtectedAsync, ИмяФайла_);};
#endif	// #if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
#endif	// #if ESB_API_PROVIDER_INCLUDE(pdf)
#if ESB_API_PROVIDER_INCLUDE(ext0xA)
#if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
// Функция СоздатьДвоичныеДанныеИзФайлаАсинх(ИмяФайла_ Как Строка) Возвращает Обещание<ДвоичныеДанные>
inline Promise<BinaryData> CreateBinaryDataFromFileAsync(ConstParam<String> FileName_) {ESB_INVOKE_API_FUNC1(Promise<BinaryData>, ext0xA, CreateBinaryDataFromFileAsync, FileName_);};
// Функция СоединитьБуферыДвоичныхДанных(Части_ Как Массив, [Разделитель_ Как БуферДвоичныхДанных], [ПорядокБайтов_ Как ПорядокБайтов]) Возвращает БуферДвоичныхДанных
inline BinaryDataBuffer ConcatBinaryDataBuffers(ConstParam<Array> Parts_, ConstParam<BinaryDataBuffer> Separator_ = DefUndef<BinaryDataBuffer>, ConstParam<ByteOrder> ByteOrder_ = DefUndef<ByteOrder>) {ESB_INVOKE_API_FUNC3(BinaryDataBuffer, ext0xA, ConcatBinaryDataBuffers, Parts_, Separator_, ByteOrder_);};
// Функция ПолучитьДвоичныеДанныеИзBase64Строки(Строка_ Как Строка) Возвращает ДвоичныеДанные
inline BinaryData GetBinaryDataFromBase64String(ConstParam<String> String_) {ESB_INVOKE_API_FUNC1(BinaryData, ext0xA, GetBinaryDataFromBase64String, String_);};
// Функция ПолучитьБуферДвоичныхДанныхИзBase64Строки(Строка_ Как Строка) Возвращает БуферДвоичныхДанных
inline BinaryDataBuffer GetBinaryDataBufferFromBase64String(ConstParam<String> String_) {ESB_INVOKE_API_FUNC1(BinaryDataBuffer, ext0xA, GetBinaryDataBufferFromBase64String, String_);};
// Функция ПолучитьBase64СтрокуИзДвоичныхДанных(ДвоичныеДанные_ Как ДвоичныеДанные) Возвращает Строка
inline String GetBase64StringFromBinaryData(ConstParam<BinaryData> BinaryData_) {ESB_INVOKE_API_FUNC1(String, ext0xA, GetBase64StringFromBinaryData, BinaryData_);};
// Функция ПолучитьBase64СтрокуИзБуфераДвоичныхДанных(Буфер_ Как БуферДвоичныхДанных) Возвращает Строка
inline String GetBase64StringFromBinaryDataBuffer(ConstParam<BinaryDataBuffer> Buffer_) {ESB_INVOKE_API_FUNC1(String, ext0xA, GetBase64StringFromBinaryDataBuffer, Buffer_);};
// Функция ПолучитьДвоичныеДанныеИзBase64ДвоичныхДанных(ДвоичныеДанные_ Как ДвоичныеДанные) Возвращает ДвоичныеДанные
inline BinaryData GetBinaryDataFromBase64BinaryData(ConstParam<BinaryData> BinaryData_) {ESB_INVOKE_API_FUNC1(BinaryData, ext0xA, GetBinaryDataFromBase64BinaryData, BinaryData_);};
// Функция ПолучитьБуферДвоичныхДанныхИзBase64БуфераДвоичныхДанных(Буфер_ Как БуферДвоичныхДанных) Возвращает БуферДвоичныхДанных
inline BinaryDataBuffer GetBinaryDataBufferFromBase64BinaryDataBuffer(ConstParam<BinaryDataBuffer> Buffer_) {ESB_INVOKE_API_FUNC1(BinaryDataBuffer, ext0xA, GetBinaryDataBufferFromBase64BinaryDataBuffer, Buffer_);};
// Функция ПолучитьBase64ДвоичныеДанныеИзДвоичныхДанных(ДвоичныеДанные_ Как ДвоичныеДанные) Возвращает ДвоичныеДанные
inline BinaryData GetBase64BinaryDataFromBinaryData(ConstParam<BinaryData> BinaryData_) {ESB_INVOKE_API_FUNC1(BinaryData, ext0xA, GetBase64BinaryDataFromBinaryData, BinaryData_);};
// Функция ПолучитьBase64БуферДвоичныхДанныхИзБуфераДвоичныхДанных(Буфер_ Как БуферДвоичныхДанных) Возвращает БуферДвоичныхДанных
inline BinaryDataBuffer GetBase64BinaryDataBufferFromBinaryDataBuffer(ConstParam<BinaryDataBuffer> Buffer_) {ESB_INVOKE_API_FUNC1(BinaryDataBuffer, ext0xA, GetBase64BinaryDataBufferFromBinaryDataBuffer, Buffer_);};
// Функция ПолучитьДвоичныеДанныеИзHexСтроки(Строка_ Как Строка) Возвращает ДвоичныеДанные
inline BinaryData GetBinaryDataFromHexString(ConstParam<String> String_) {ESB_INVOKE_API_FUNC1(BinaryData, ext0xA, GetBinaryDataFromHexString, String_);};
// Функция ПолучитьБуферДвоичныхДанныхИзHexСтроки(Строка_ Как Строка) Возвращает БуферДвоичныхДанных
inline BinaryDataBuffer GetBinaryDataBufferFromHexString(ConstParam<String> Line_) {ESB_INVOKE_API_FUNC1(BinaryDataBuffer, ext0xA, GetBinaryDataBufferFromHexString, Line_);};
// Функция ПолучитьHexСтрокуИзДвоичныхДанных(ДвоичныеДанные_ Как ДвоичныеДанные) Возвращает Строка
inline String GetHexStringFromBinaryData(ConstParam<BinaryData> BinaryData_) {ESB_INVOKE_API_FUNC1(String, ext0xA, GetHexStringFromBinaryData, BinaryData_);};
// Функция ПолучитьHexСтрокуИзБуфераДвоичныхДанных(Буфер_ Как БуферДвоичныхДанных) Возвращает Строка
inline String GetHexStringFromBinaryDataBuffer(ConstParam<BinaryDataBuffer> Buffer_) {ESB_INVOKE_API_FUNC1(String, ext0xA, GetHexStringFromBinaryDataBuffer, Buffer_);};
// Функция ПолучитьДвоичныеДанныеИзHexДвоичныхДанных(ДвоичныеДанные_ Как ДвоичныеДанные) Возвращает ДвоичныеДанные
inline BinaryData GetBinaryDataFromHexBinaryData(ConstParam<BinaryData> BinaryData_) {ESB_INVOKE_API_FUNC1(BinaryData, ext0xA, GetBinaryDataFromHexBinaryData, BinaryData_);};
// Функция ПолучитьБуферДвоичныхДанныхИзHexБуфераДвоичныхДанных(Буфер_ Как БуферДвоичныхДанных) Возвращает БуферДвоичныхДанных
inline BinaryDataBuffer GetBinaryDataBufferFromHexBinaryDataBuffer(ConstParam<BinaryDataBuffer> Buffer_) {ESB_INVOKE_API_FUNC1(BinaryDataBuffer, ext0xA, GetBinaryDataBufferFromHexBinaryDataBuffer, Buffer_);};
// Функция ПолучитьHexДвоичныеДанныеИзДвоичныхДанных(ДвоичныеДанные_ Как ДвоичныеДанные) Возвращает ДвоичныеДанные
inline BinaryData GetHexBinaryDataFromBinaryData(ConstParam<BinaryData> BinaryData_) {ESB_INVOKE_API_FUNC1(BinaryData, ext0xA, GetHexBinaryDataFromBinaryData, BinaryData_);};
// Функция ПолучитьHexБуферДвоичныхДанныхИзБуфераДвоичныхДанных(Буфер_ Как БуферДвоичныхДанных) Возвращает БуферДвоичныхДанных
inline BinaryDataBuffer GetHexBinaryDataBufferFromBinaryDataBuffer(ConstParam<BinaryDataBuffer> Buffer_) {ESB_INVOKE_API_FUNC1(BinaryDataBuffer, ext0xA, GetHexBinaryDataBufferFromBinaryDataBuffer, Buffer_);};
// Функция ПолучитьБуферДвоичныхДанныхИзДвоичныхДанных(ДвоичныеДанные_ Как ДвоичныеДанные) Возвращает БуферДвоичныхДанных
inline BinaryDataBuffer GetBinaryDataBufferFromBinaryData(ConstParam<BinaryData> BinaryData_) {ESB_INVOKE_API_FUNC1(BinaryDataBuffer, ext0xA, GetBinaryDataBufferFromBinaryData, BinaryData_);};
// Функция ПолучитьДвоичныеДанныеИзБуфераДвоичныхДанных(Буфер_ Как БуферДвоичныхДанных) Возвращает ДвоичныеДанные
inline BinaryData GetBinaryDataFromBinaryDataBuffer(ConstParam<BinaryDataBuffer> Buffer_) {ESB_INVOKE_API_FUNC1(BinaryData, ext0xA, GetBinaryDataFromBinaryDataBuffer, Buffer_);};
// Функция СоединитьДвоичныеДанные(МассивДвоичныхДанных_ Как Массив) Возвращает ДвоичныеДанные
inline BinaryData ConcatBinaryData(ConstParam<Array> BinaryDataArray_) {ESB_INVOKE_API_FUNC1(BinaryData, ext0xA, ConcatBinaryData, BinaryDataArray_);};
// Функция РазделитьДвоичныеДанные(ДвоичныеДанные_ Как ДвоичныеДанные, РазмерЧасти_ Как Число) Возвращает МассивИз<ДвоичныеДанные>
inline ArrayOf<BinaryData> SplitBinaryData(ConstParam<BinaryData> BinaryData_, ConstParam<Number> PartSize_) {ESB_INVOKE_API_FUNC2(ArrayOf<BinaryData>, ext0xA, SplitBinaryData, BinaryData_, PartSize_);};
#endif	// #if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
// Function CreateBinaryDataFromFileAsync(String FileName_) -> Promise<BinaryData>
inline Обещание<ДвоичныеДанные> СоздатьДвоичныеДанныеИзФайлаАсинх(КонстПарам<Строка> ИмяФайла_) {ESB_INVOKE_API_FUNC1(Обещание<ДвоичныеДанные>, ext0xA, CreateBinaryDataFromFileAsync, ИмяФайла_);};
// Function ConcatBinaryDataBuffers(Array Parts_, BinaryDataBuffer Separator_ = <default>, ByteOrder ByteOrder_ = <default>) -> BinaryDataBuffer
inline БуферДвоичныхДанных СоединитьБуферыДвоичныхДанных(КонстПарам<Массив> Части_, КонстПарам<БуферДвоичныхДанных> Разделитель_ = ПоУмолчаниюНеопределеноИли<БуферДвоичныхДанных>, КонстПарам<ПорядокБайтов> ПорядокБайтов_ = ПоУмолчаниюНеопределеноИли<ПорядокБайтов>) {ESB_INVOKE_API_FUNC3(БуферДвоичныхДанных, ext0xA, ConcatBinaryDataBuffers, Части_, Разделитель_, ПорядокБайтов_);};
// Function GetBinaryDataFromBase64String(String String_) -> BinaryData
inline ДвоичныеДанные ПолучитьДвоичныеДанныеИзBase64Строки(КонстПарам<Строка> Строка_) {ESB_INVOKE_API_FUNC1(ДвоичныеДанные, ext0xA, GetBinaryDataFromBase64String, Строка_);};
// Function GetBinaryDataBufferFromBase64String(String String_) -> BinaryDataBuffer
inline БуферДвоичныхДанных ПолучитьБуферДвоичныхДанныхИзBase64Строки(КонстПарам<Строка> Строка_) {ESB_INVOKE_API_FUNC1(БуферДвоичныхДанных, ext0xA, GetBinaryDataBufferFromBase64String, Строка_);};
// Function GetBase64StringFromBinaryData(BinaryData BinaryData_) -> String
inline Строка ПолучитьBase64СтрокуИзДвоичныхДанных(КонстПарам<ДвоичныеДанные> ДвоичныеДанные_) {ESB_INVOKE_API_FUNC1(Строка, ext0xA, GetBase64StringFromBinaryData, ДвоичныеДанные_);};
// Function GetBase64StringFromBinaryDataBuffer(BinaryDataBuffer Buffer_) -> String
inline Строка ПолучитьBase64СтрокуИзБуфераДвоичныхДанных(КонстПарам<БуферДвоичныхДанных> Буфер_) {ESB_INVOKE_API_FUNC1(Строка, ext0xA, GetBase64StringFromBinaryDataBuffer, Буфер_);};
// Function GetBinaryDataFromBase64BinaryData(BinaryData BinaryData_) -> BinaryData
inline ДвоичныеДанные ПолучитьДвоичныеДанныеИзBase64ДвоичныхДанных(КонстПарам<ДвоичныеДанные> ДвоичныеДанные_) {ESB_INVOKE_API_FUNC1(ДвоичныеДанные, ext0xA, GetBinaryDataFromBase64BinaryData, ДвоичныеДанные_);};
// Function GetBinaryDataBufferFromBase64BinaryDataBuffer(BinaryDataBuffer Buffer_) -> BinaryDataBuffer
inline БуферДвоичныхДанных ПолучитьБуферДвоичныхДанныхИзBase64БуфераДвоичныхДанных(КонстПарам<БуферДвоичныхДанных> Буфер_) {ESB_INVOKE_API_FUNC1(БуферДвоичныхДанных, ext0xA, GetBinaryDataBufferFromBase64BinaryDataBuffer, Буфер_);};
// Function GetBase64BinaryDataFromBinaryData(BinaryData BinaryData_) -> BinaryData
inline ДвоичныеДанные ПолучитьBase64ДвоичныеДанныеИзДвоичныхДанных(КонстПарам<ДвоичныеДанные> ДвоичныеДанные_) {ESB_INVOKE_API_FUNC1(ДвоичныеДанные, ext0xA, GetBase64BinaryDataFromBinaryData, ДвоичныеДанные_);};
// Function GetBase64BinaryDataBufferFromBinaryDataBuffer(BinaryDataBuffer Buffer_) -> BinaryDataBuffer
inline БуферДвоичныхДанных ПолучитьBase64БуферДвоичныхДанныхИзБуфераДвоичныхДанных(КонстПарам<БуферДвоичныхДанных> Буфер_) {ESB_INVOKE_API_FUNC1(БуферДвоичныхДанных, ext0xA, GetBase64BinaryDataBufferFromBinaryDataBuffer, Буфер_);};
// Function GetBinaryDataFromHexString(String String_) -> BinaryData
inline ДвоичныеДанные ПолучитьДвоичныеДанныеИзHexСтроки(КонстПарам<Строка> Строка_) {ESB_INVOKE_API_FUNC1(ДвоичныеДанные, ext0xA, GetBinaryDataFromHexString, Строка_);};
// Function GetBinaryDataBufferFromHexString(String Line_) -> BinaryDataBuffer
inline БуферДвоичныхДанных ПолучитьБуферДвоичныхДанныхИзHexСтроки(КонстПарам<Строка> Строка_) {ESB_INVOKE_API_FUNC1(БуферДвоичныхДанных, ext0xA, GetBinaryDataBufferFromHexString, Строка_);};
// Function GetHexStringFromBinaryData(BinaryData BinaryData_) -> String
inline Строка ПолучитьHexСтрокуИзДвоичныхДанных(КонстПарам<ДвоичныеДанные> ДвоичныеДанные_) {ESB_INVOKE_API_FUNC1(Строка, ext0xA, GetHexStringFromBinaryData, ДвоичныеДанные_);};
// Function GetHexStringFromBinaryDataBuffer(BinaryDataBuffer Buffer_) -> String
inline Строка ПолучитьHexСтрокуИзБуфераДвоичныхДанных(КонстПарам<БуферДвоичныхДанных> Буфер_) {ESB_INVOKE_API_FUNC1(Строка, ext0xA, GetHexStringFromBinaryDataBuffer, Буфер_);};
// Function GetBinaryDataFromHexBinaryData(BinaryData BinaryData_) -> BinaryData
inline ДвоичныеДанные ПолучитьДвоичныеДанныеИзHexДвоичныхДанных(КонстПарам<ДвоичныеДанные> ДвоичныеДанные_) {ESB_INVOKE_API_FUNC1(ДвоичныеДанные, ext0xA, GetBinaryDataFromHexBinaryData, ДвоичныеДанные_);};
// Function GetBinaryDataBufferFromHexBinaryDataBuffer(BinaryDataBuffer Buffer_) -> BinaryDataBuffer
inline БуферДвоичныхДанных ПолучитьБуферДвоичныхДанныхИзHexБуфераДвоичныхДанных(КонстПарам<БуферДвоичныхДанных> Буфер_) {ESB_INVOKE_API_FUNC1(БуферДвоичныхДанных, ext0xA, GetBinaryDataBufferFromHexBinaryDataBuffer, Буфер_);};
// Function GetHexBinaryDataFromBinaryData(BinaryData BinaryData_) -> BinaryData
inline ДвоичныеДанные ПолучитьHexДвоичныеДанныеИзДвоичныхДанных(КонстПарам<ДвоичныеДанные> ДвоичныеДанные_) {ESB_INVOKE_API_FUNC1(ДвоичныеДанные, ext0xA, GetHexBinaryDataFromBinaryData, ДвоичныеДанные_);};
// Function GetHexBinaryDataBufferFromBinaryDataBuffer(BinaryDataBuffer Buffer_) -> BinaryDataBuffer
inline БуферДвоичныхДанных ПолучитьHexБуферДвоичныхДанныхИзБуфераДвоичныхДанных(КонстПарам<БуферДвоичныхДанных> Буфер_) {ESB_INVOKE_API_FUNC1(БуферДвоичныхДанных, ext0xA, GetHexBinaryDataBufferFromBinaryDataBuffer, Буфер_);};
// Function GetBinaryDataBufferFromBinaryData(BinaryData BinaryData_) -> BinaryDataBuffer
inline БуферДвоичныхДанных ПолучитьБуферДвоичныхДанныхИзДвоичныхДанных(КонстПарам<ДвоичныеДанные> ДвоичныеДанные_) {ESB_INVOKE_API_FUNC1(БуферДвоичныхДанных, ext0xA, GetBinaryDataBufferFromBinaryData, ДвоичныеДанные_);};
// Function GetBinaryDataFromBinaryDataBuffer(BinaryDataBuffer Buffer_) -> BinaryData
inline ДвоичныеДанные ПолучитьДвоичныеДанныеИзБуфераДвоичныхДанных(КонстПарам<БуферДвоичныхДанных> Буфер_) {ESB_INVOKE_API_FUNC1(ДвоичныеДанные, ext0xA, GetBinaryDataFromBinaryDataBuffer, Буфер_);};
// Function ConcatBinaryData(Array BinaryDataArray_) -> BinaryData
inline ДвоичныеДанные СоединитьДвоичныеДанные(КонстПарам<Массив> МассивДвоичныхДанных_) {ESB_INVOKE_API_FUNC1(ДвоичныеДанные, ext0xA, ConcatBinaryData, МассивДвоичныхДанных_);};
// Function SplitBinaryData(BinaryData BinaryData_, Number PartSize_) -> ArrayOf<BinaryData>
inline МассивИз<ДвоичныеДанные> РазделитьДвоичныеДанные(КонстПарам<ДвоичныеДанные> ДвоичныеДанные_, КонстПарам<Число> РазмерЧасти_) {ESB_INVOKE_API_FUNC2(МассивИз<ДвоичныеДанные>, ext0xA, SplitBinaryData, ДвоичныеДанные_, РазмерЧасти_);};
#endif	// #if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
#endif	// #if ESB_API_PROVIDER_INCLUDE(ext0xA)
}


#endif //ESB_ES1_FILE_H_
