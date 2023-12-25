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
	//
	class StreamPosition;
	class ByteOrder;
	class FileOpenMode;
	class FileAccess;
	class Stream;
	class BinaryData;
	class BinaryDataBuffer;
	class FileStream;
	class MemoryStream;

#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
	using ПозицияВПотоке = StreamPosition;
	using ПорядокБайтов = ByteOrder;
	using РежимОткрытияФайла = FileOpenMode;
	using ДоступКФайлу = FileAccess;
	using Поток = Stream;
	using ДвоичныеДанные = BinaryData;
	using БуферДвоичныхДанных = BinaryDataBuffer;
	using ФайловыйПоток = FileStream;
	using ПотокВПамяти = MemoryStream;
#endif	// #if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
	


	ESB_CLASS_ENUM(StreamPosition)
	{	ESB_CLASS_IMPLEMENT_ENUM(StreamPosition)
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



	//TOBE:	Относительно Stream и его вариантов - FileStream/MemoryStream нужно подумать над "интерфейсом" - объекты разные, а интерфейс-то один
	//		В 1С такого понятия нет - там интерфейс "утиный - у кого метод Flush, тот и поток..", а у нас-то все строго!
	//		также при этом нужно учесть что каждый поток может быть GetCanRead/GetCanWrite/GetCanSeek и мы это тоже можем сделать интерфейсами
	//		как у IIndexedCollection - StreamRO/StreamWO etc..
	//		Или не надо.. в конце концов они здесь не для работы, а получить/записать и забыть.
	//
	//NOTE: Для Stream и его наследников методы меняющие текущую позицию в потоке считаем константными.
	//		Данные не меняются - значит константа. А текущая позиция mutable.


	ESB_CLASS_OBJECT(Stream)
	{
		ESB_CLASS_IMPLEMENT_OBJECT(Stream)
#if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
	public:
		// Not Creatable
	public:
		// Процедура НачатьЗакрытие(ОписаниеОповещения_ Как ОписаниеОповещения)
		void BeginClose(ConstParam<NotifyDescription> NotifyDescription_) { ESB_INVOKE_MEMB_PROC1(MethId::BeginClose, NotifyDescription_); };
		// Процедура НачатьКопированиеВ(ОписаниеОповещения_ Как ОписаниеОповещения, ЦелевойПоток_ Как Поток, [РазмерБуфера_ Как Число])
		void BeginCopyTo(ConstParam<NotifyDescription> NotifyDescription_, ConstParam<Stream> TargetStream_, ConstParam<Number> BufferSize_ = DefUndef<esb::Number>) const { ESB_INVOKE_MEMB_PROC3(MethId::BeginCopyTo, NotifyDescription_, TargetStream_, BufferSize_); };
		// Процедура НачатьСбросБуферов(ОписаниеОповещения_ Как ОписаниеОповещения)
		void BeginFlush(ConstParam<NotifyDescription> NotifyDescription_) const { ESB_INVOKE_MEMB_PROC1(MethId::BeginFlush, NotifyDescription_); };
		// Процедура НачатьПолучениеРазмера([ОписаниеОповещения_ Как ОписаниеОповещения])
		void BeginGetSize(ConstParam<NotifyDescription> NotifyDescription_ = DefUndef<esb::NotifyDescription>) const { ESB_INVOKE_MEMB_PROC1(MethId::BeginGetSize, NotifyDescription_); };
		// Процедура НачатьЧтение(ОписаниеОповещения_ Как ОписаниеОповещения, Буфер_ Как БуферДвоичныхДанных, ПозицияВБуфере_ Как Число, Количество_ Как Число)
		void BeginRead(ConstParam<NotifyDescription> NotifyDescription_, ConstParam<BinaryDataBuffer> Buffer_, ConstParam<Number> PositionInBuffer_, ConstParam<Number> Number_) const { ESB_INVOKE_MEMB_PROC4(MethId::BeginRead, NotifyDescription_, Buffer_, PositionInBuffer_, Number_); };
		// Процедура НачатьПереход(ОписаниеОповещения_ Как ОписаниеОповещения, Смещение_ Как Число, [НачальнаяПозиция_ Как ПозицияВПотоке])
		void BeginSeek(ConstParam<NotifyDescription> NotifyDescription_, ConstParam<Number> Offset_, ConstParam<StreamPosition> InitialPosition_ = DefUndef<esb::StreamPosition>) const { ESB_INVOKE_MEMB_PROC3(MethId::BeginSeek, NotifyDescription_, Offset_, InitialPosition_); };
		// Процедура НачатьУстановкуРазмера(ОписаниеОповещения_ Как ОписаниеОповещения, Размер_ Как Число)
		void BeginSetSize(ConstParam<NotifyDescription> NotifyDescription_, ConstParam<Number> Size_) { ESB_INVOKE_MEMB_PROC2(MethId::BeginSetSize, NotifyDescription_, Size_); };
		// Процедура НачатьЗапись(ОписаниеОповещения_ Как ОписаниеОповещения, Буфер_ Как БуферДвоичныхДанных, ПозицияВБуфере_ Как Число, Количество_ Как Число)
		void BeginWrite(ConstParam<NotifyDescription> NotifyDescription_, ConstParam<BinaryDataBuffer> Buffer_, ConstParam<Number> PositionInBuffer_, ConstParam<Number> Count_) { ESB_INVOKE_MEMB_PROC4(MethId::BeginWrite, NotifyDescription_, Buffer_, PositionInBuffer_, Count_); };
		// Процедура Закрыть()
		void Close() { ESB_INVOKE_MEMB_PROC0(MethId::Close); };
		// Процедура КопироватьВ(ЦелевойПоток_ Как Поток, [РазмерБуфера_ Как Число])
		void CopyTo(ConstParam<Stream> TargetStream_, ConstParam<Number> BufferSize_ = DefUndef<esb::Number>) const { ESB_INVOKE_MEMB_PROC2(MethId::CopyTo, TargetStream_, BufferSize_); };
		// Функция ТекущаяПозиция() Возвращает Число
		Number CurrentPosition() const { ESB_INVOKE_MEMB_FUNC0(esb::Number, MethId::CurrentPosition); };
		// Процедура СброситьБуферы()
		void Flush() const { ESB_INVOKE_MEMB_PROC0(MethId::Flush); };
		// Функция ПолучитьПотокТолькоДляЧтения() Возвращает Поток
		Stream GetReadonlyStream() const { ESB_INVOKE_MEMB_FUNC0(esb::Stream, MethId::GetReadOnlyStream); };
		// Функция Прочитать(Буфер_ Как БуферДвоичныхДанных, ПозицияВБуфере_ Как Число, Количество_ Как Число) Возвращает Число
		Number Read(ConstParam<BinaryDataBuffer> Buffer_, ConstParam<Number> PositionInBuffer_, ConstParam<Number> Number_) const { ESB_INVOKE_MEMB_FUNC3(esb::Number, MethId::Read, Buffer_, PositionInBuffer_, Number_); };
		// Функция Перейти(Смещение_ Как Число, [НачальнаяПозиция_ Как ПозицияВПотоке]) Возвращает Число
		Number Seek(ConstParam<Number> Offset_, ConstParam<StreamPosition> InitialPosition_ = DefUndef<esb::StreamPosition>) const { ESB_INVOKE_MEMB_FUNC2(esb::Number, MethId::Seek, Offset_, InitialPosition_); };
		// Процедура УстановитьРазмер(Размер_ Как Число)
		void SetSize(ConstParam<Number> Size_) { ESB_INVOKE_MEMB_PROC1(MethId::SetSize, Size_); };
		// Функция Размер() Возвращает Число
		Number Size() const { ESB_INVOKE_MEMB_FUNC0(esb::Number, MethId::Size); };
		// Процедура Записать(Буфер_ Как БуферДвоичныхДанных, ПозицияВБуфере_ Как Число, Количество_ Как Число)
		void Write(ConstParam<BinaryDataBuffer> Buffer_, ConstParam<Number> PositionInBuffer_, ConstParam<Number> Number_) { ESB_INVOKE_MEMB_PROC3(MethId::Write, Buffer_, PositionInBuffer_, Number_); };
	public:
		// Свойство [[ТолькоЧтение]] ДоступноЧтение Как Булево
		Boolean GetCanRead() const { ESB_INVOKE_PROPERTY_GET(esb::Boolean, PropId::CanRead); };
		ESB_PROPERTY_FIELD_RO(Boolean, CanRead)
			// Свойство [[ТолькоЧтение]] ДоступноИзменениеПозиции Как Булево
			Boolean GetCanSeek() const { ESB_INVOKE_PROPERTY_GET(esb::Boolean, PropId::CanSeek); };
		ESB_PROPERTY_FIELD_RO(Boolean, CanSeek)
			// Свойство [[ТолькоЧтение]] ДоступнаЗапись Как Булево
			Boolean GetCanWrite() const { ESB_INVOKE_PROPERTY_GET(esb::Boolean, PropId::CanWrite); };
		ESB_PROPERTY_FIELD_RO(Boolean, CanWrite)
#endif	// #if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
	public:
		// Not Creatable
	public:
		// Void BeginClose(NotifyDescription NotifyDescription_)
		void НачатьЗакрытие(КонстПарам<ОписаниеОповещения> ОписаниеОповещения_) { ESB_INVOKE_MEMB_PROC1(MethId::BeginClose, ОписаниеОповещения_); };
		// Void BeginCopyTo(NotifyDescription NotifyDescription_, Stream TargetStream_, Number BufferSize_ = <default>)
		void НачатьКопированиеВ(КонстПарам<ОписаниеОповещения> ОписаниеОповещения_, КонстПарам<Поток> ЦелевойПоток_, КонстПарам<Число> РазмерБуфера_ = ПоУмолчаниюНеопределеноИли<esb::Число>) const { ESB_INVOKE_MEMB_PROC3(MethId::BeginCopyTo, ОписаниеОповещения_, ЦелевойПоток_, РазмерБуфера_); };
		// Void BeginFlush(NotifyDescription NotifyDescription_)
		void НачатьСбросБуферов(КонстПарам<ОписаниеОповещения> ОписаниеОповещения_) const { ESB_INVOKE_MEMB_PROC1(MethId::BeginFlush, ОписаниеОповещения_); };
		// Void BeginGetSize(NotifyDescription NotifyDescription_ = <default>)
		void НачатьПолучениеРазмера(КонстПарам<ОписаниеОповещения> ОписаниеОповещения_ = ПоУмолчаниюНеопределеноИли<esb::ОписаниеОповещения>) const { ESB_INVOKE_MEMB_PROC1(MethId::BeginGetSize, ОписаниеОповещения_); };
		// Void BeginRead(NotifyDescription NotifyDescription_, BinaryDataBuffer Buffer_, Number PositionInBuffer_, Number Number_)
		void НачатьЧтение(КонстПарам<ОписаниеОповещения> ОписаниеОповещения_, КонстПарам<БуферДвоичныхДанных> Буфер_, КонстПарам<Число> ПозицияВБуфере_, КонстПарам<Число> Количество_) const { ESB_INVOKE_MEMB_PROC4(MethId::BeginRead, ОписаниеОповещения_, Буфер_, ПозицияВБуфере_, Количество_); };
		// Void BeginSeek(NotifyDescription NotifyDescription_, Number Offset_, StreamPosition InitialPosition_ = <default>)
		void НачатьПереход(КонстПарам<ОписаниеОповещения> ОписаниеОповещения_, КонстПарам<Число> Смещение_, КонстПарам<ПозицияВПотоке> НачальнаяПозиция_ = ПоУмолчаниюНеопределеноИли<esb::ПозицияВПотоке>) const { ESB_INVOKE_MEMB_PROC3(MethId::BeginSeek, ОписаниеОповещения_, Смещение_, НачальнаяПозиция_); };
		// Void BeginSetSize(NotifyDescription NotifyDescription_, Number Size_)
		void НачатьУстановкуРазмера(КонстПарам<ОписаниеОповещения> ОписаниеОповещения_, КонстПарам<Число> Размер_) { ESB_INVOKE_MEMB_PROC2(MethId::BeginSetSize, ОписаниеОповещения_, Размер_); };
		// Void BeginWrite(NotifyDescription NotifyDescription_, BinaryDataBuffer Buffer_, Number PositionInBuffer_, Number Count_)
		void НачатьЗапись(КонстПарам<ОписаниеОповещения> ОписаниеОповещения_, КонстПарам<БуферДвоичныхДанных> Буфер_, КонстПарам<Число> ПозицияВБуфере_, КонстПарам<Число> Количество_) { ESB_INVOKE_MEMB_PROC4(MethId::BeginWrite, ОписаниеОповещения_, Буфер_, ПозицияВБуфере_, Количество_); };
		// Void Close()
		void Закрыть() { ESB_INVOKE_MEMB_PROC0(MethId::Close); };
		// Void CopyTo(Stream TargetStream_, Number BufferSize_ = <default>)
		void КопироватьВ(КонстПарам<Поток> ЦелевойПоток_, КонстПарам<Число> РазмерБуфера_ = ПоУмолчаниюНеопределеноИли<esb::Число>) const { ESB_INVOKE_MEMB_PROC2(MethId::CopyTo, ЦелевойПоток_, РазмерБуфера_); };
		// Function CurrentPosition() -> Number
		Число ТекущаяПозиция() const { ESB_INVOKE_MEMB_FUNC0(esb::Число, MethId::CurrentPosition); };
		// Void Flush()
		void СброситьБуферы() const { ESB_INVOKE_MEMB_PROC0(MethId::Flush); };
		// Function GetReadonlyStream() -> Stream
		Поток ПолучитьПотокТолькоДляЧтения() const { ESB_INVOKE_MEMB_FUNC0(esb::Поток, MethId::GetReadOnlyStream); };
		// Function Read(BinaryDataBuffer Buffer_, Number PositionInBuffer_, Number Number_) -> Number
		Число Прочитать(КонстПарам<БуферДвоичныхДанных> Буфер_, КонстПарам<Число> ПозицияВБуфере_, КонстПарам<Число> Количество_) const { ESB_INVOKE_MEMB_FUNC3(esb::Число, MethId::Read, Буфер_, ПозицияВБуфере_, Количество_); };
		// Function Seek(Number Offset_, StreamPosition InitialPosition_ = <default>) -> Number
		Число Перейти(КонстПарам<Число> Смещение_, КонстПарам<ПозицияВПотоке> НачальнаяПозиция_ = ПоУмолчаниюНеопределеноИли<esb::ПозицияВПотоке>) const { ESB_INVOKE_MEMB_FUNC2(esb::Число, MethId::Seek, Смещение_, НачальнаяПозиция_); };
		// Void SetSize(Number Size_)
		void УстановитьРазмер(КонстПарам<Число> Размер_) { ESB_INVOKE_MEMB_PROC1(MethId::SetSize, Размер_); };
		// Function Size() -> Number
		Число Размер() const { ESB_INVOKE_MEMB_FUNC0(esb::Число, MethId::Size); };
		// Void Write(BinaryDataBuffer Buffer_, Number PositionInBuffer_, Number Number_)
		void Записать(КонстПарам<БуферДвоичныхДанных> Буфер_, КонстПарам<Число> ПозицияВБуфере_, КонстПарам<Число> Количество_) { ESB_INVOKE_MEMB_PROC3(MethId::Write, Буфер_, ПозицияВБуфере_, Количество_); };
	public:
		// Property [[ReadOnly]] CanRead -> Boolean
		Булево ПолучитьДоступноЧтение() const { ESB_INVOKE_PROPERTY_GET(esb::Булево, PropId::CanRead); };
		ESB_PROPERTY_FIELD_RO(Булево, ДоступноЧтение)
			// Property [[ReadOnly]] CanSeek -> Boolean
			Булево ПолучитьДоступноИзменениеПозиции() const { ESB_INVOKE_PROPERTY_GET(esb::Булево, PropId::CanSeek); };
		ESB_PROPERTY_FIELD_RO(Булево, ДоступноИзменениеПозиции)
			// Property [[ReadOnly]] CanWrite -> Boolean
			Булево ПолучитьДоступнаЗапись() const { ESB_INVOKE_PROPERTY_GET(esb::Булево, PropId::CanWrite); };
		ESB_PROPERTY_FIELD_RO(Булево, ДоступнаЗапись)
#endif	// #if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
	};


	ESB_CLASS_OBJECT(BinaryData)
	{
		ESB_CLASS_IMPLEMENT_OBJECT(BinaryData)
#if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
	public:
		static BinaryData Create(ConstParam<String> FileName_) { ESB_INVOKE_CTOR1(BinaryData, FileName_); }
	public:
		// Процедура НачатьЗапись(ОписаниеОповещения_ Как ОписаниеОповещения, ИмяФайла_ Как Строка)
		void BeginWrite(ConstParam<NotifyDescription> NotifyDescription_, ConstParam<String> FileName_) const { ESB_INVOKE_MEMB_PROC2(MethId::BeginWrite, NotifyDescription_, FileName_); };
		// Процедура НачатьЗапись(ОписаниеОповещения_ Как ОписаниеОповещения, ИмяФайла_ Как Строка)
		void BeginWrite(ConstParam<NotifyDescription> NotifyDescription_, ConstParam<Stream> Stream_) const { ESB_INVOKE_MEMB_PROC2(MethId::BeginWrite, NotifyDescription_, Stream_); };
		// Функция ОткрытьПотокДляЧтения() Возвращает Поток
		Stream OpenStreamForRead() const { ESB_INVOKE_MEMB_FUNC0(esb::Stream, MethId::OpenStreamForRead); };
		// Функция Размер() Возвращает Число
		Number Size() const { ESB_INVOKE_MEMB_FUNC0(esb::Number, MethId::Size); };
		// Процедура Записать(ИмяФайла_ Как Строка)
		void Write(ConstParam<String> FileName_) const { ESB_INVOKE_MEMB_PROC1(MethId::Write, FileName_); };
		// Процедура Записать(ИмяФайла_ Как Строка)
		void Write(ConstParam<Stream> Stream_) const { ESB_INVOKE_MEMB_PROC1(MethId::Write, Stream_); };
#endif	// #if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
	public:
		static ДвоичныеДанные Создать(КонстПарам<Строка> ИмяФайла_) { ESB_INVOKE_CTOR1(ДвоичныеДанные, ИмяФайла_); }
	public:
		// Void BeginWrite(NotifyDescription NotifyDescription_, String FileName_)
		void НачатьЗапись(КонстПарам<ОписаниеОповещения> ОписаниеОповещения_, КонстПарам<Строка> ИмяФайла_) const { ESB_INVOKE_MEMB_PROC2(MethId::BeginWrite, ОписаниеОповещения_, ИмяФайла_); };
		// Void BeginWrite(NotifyDescription NotifyDescription_, String FileName_)
		void НачатьЗапись(КонстПарам<ОписаниеОповещения> ОписаниеОповещения_, КонстПарам<Поток> Поток_) const { ESB_INVOKE_MEMB_PROC2(MethId::BeginWrite, ОписаниеОповещения_, Поток_); };
		// Function OpenStreamForRead() -> Stream
		Поток ОткрытьПотокДляЧтения() const { ESB_INVOKE_MEMB_FUNC0(esb::Поток, MethId::OpenStreamForRead); };
		// Function Size() -> Number
		Число Размер() const { ESB_INVOKE_MEMB_FUNC0(esb::Число, MethId::Size); };
		// Void Write(String FileName_)
		void Записать(КонстПарам<Строка> ИмяФайла_) const { ESB_INVOKE_MEMB_PROC1(MethId::Write, ИмяФайла_); };
		// Void Write(String FileName_)
		void Записать(КонстПарам<Поток> Поток_) const { ESB_INVOKE_MEMB_PROC1(MethId::Write, Поток_); };
#endif	// #if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
	};


	ESB_CLASS_OBJECT_DUAL(BinaryDataBuffer, IxCollectionRW<Number>)
	{	ESB_CLASS_IMPLEMENT_OBJECT_DUAL(BinaryDataBuffer)
#if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
	public:
		static BinaryDataBuffer Create(ConstParam<Number> Size_, ConstParam<ByteOrder> ByteOrder_ = DefUndef<esb::ByteOrder>) { ESB_INVOKE_CTOR2(BinaryDataBuffer, Size_, ByteOrder_); }
	public:
		// Функция Соединить(Буфер_ Как БуферДвоичныхДанных) Возвращает БуферДвоичныхДанных
		BinaryDataBuffer Concat(ConstParam<BinaryDataBuffer> Buffer_) const { ESB_INVOKE_MEMB_FUNC1(esb::BinaryDataBuffer, MethId::Concat, Buffer_); };
		// Функция Скопировать() Возвращает БуферДвоичныхДанных
		BinaryDataBuffer Copy() const { ESB_INVOKE_MEMB_FUNC0(esb::BinaryDataBuffer, MethId::Copy); };
		// Функция Получить(Позиция_ Как Число) Возвращает Число
		Number Get(ConstParam<Number> Position_) const { ESB_INVOKE_MEMB_FUNC1(esb::Number, MethId::Get, Position_); };
		// Функция ПолучитьСрез(Позиция_ Как Число, [Количество_ Как Число]) Возвращает БуферДвоичныхДанных
		BinaryDataBuffer GetSlice(ConstParam<Number> Position_, ConstParam<Number> Number_ = DefUndef<esb::Number>) const { ESB_INVOKE_MEMB_FUNC2(esb::BinaryDataBuffer, MethId::GetSlice, Position_, Number_); };
		// Функция Прочитать(Позиция_ Как Число, Количество_ Как Число) Возвращает БуферДвоичныхДанных
		BinaryDataBuffer Read(ConstParam<Number> Position_, ConstParam<Number> Number_) const { ESB_INVOKE_MEMB_FUNC2(esb::BinaryDataBuffer, MethId::Read, Position_, Number_); };
		// Функция ПрочитатьЦелое16(Позиция_ Как Число, [ПорядокБайтов_ Как ПорядокБайтов]) Возвращает Число
		Number ReadInt16(ConstParam<Number> Position_, ConstParam<ByteOrder> ByteOrder_ = DefUndef<esb::ByteOrder>) const { ESB_INVOKE_MEMB_FUNC2(esb::Number, MethId::ReadInt16, Position_, ByteOrder_); };
		// Функция ПрочитатьЦелое32(Позиция_ Как Число, [ПорядокБайтов_ Как ПорядокБайтов]) Возвращает Число
		Number ReadInt32(ConstParam<Number> Position_, ConstParam<ByteOrder> ByteOrder_ = DefUndef<esb::ByteOrder>) const { ESB_INVOKE_MEMB_FUNC2(esb::Number, MethId::ReadInt32, Position_, ByteOrder_); };
		// Функция ПрочитатьЦелое64([Позиция_ Как Число], [ПорядокБайтов_ Как ПорядокБайтов]) Возвращает Число
		Number ReadInt64(ConstParam<Number> Position_ = DefUndef<esb::Number>, ConstParam<ByteOrder> ByteOrder_ = DefUndef<esb::ByteOrder>) const { ESB_INVOKE_MEMB_FUNC2(esb::Number, MethId::ReadInt64, Position_, ByteOrder_); };
		// Функция Перевернуть() Возвращает БуферДвоичныхДанных
		BinaryDataBuffer Reverse() const { ESB_INVOKE_MEMB_FUNC0(esb::BinaryDataBuffer, MethId::Reverse); };
		// Процедура Установить(Позиция_ Как Число, Значение_ Как Число)
		void Set(ConstParam<Number> Position_, ConstParam<Number> Value_) { ESB_INVOKE_MEMB_PROC2(MethId::Set, Position_, Value_); };
		// Процедура УстановитьТолькоЧтение()
		void SetReadOnly() { ESB_INVOKE_MEMB_PROC0(MethId::SetReadOnly); };
		// Функция Разделить(Разделитель_ Как БуферДвоичныхДанных) Возвращает МассивИз<БуферДвоичныхДанных>
		ArrayOf<BinaryDataBuffer> Split(ConstParam<BinaryDataBuffer> Separator_) const { ESB_INVOKE_MEMB_FUNC1(esb::ArrayOf<BinaryDataBuffer>, MethId::Split, Separator_); };
		// Функция Разделить(Разделитель_ Как БуферДвоичныхДанных) Возвращает МассивИз<БуферДвоичныхДанных>
		ArrayOf<BinaryDataBuffer> Split(ConstParam<Array> Separators_) const { ESB_INVOKE_MEMB_FUNC1(esb::ArrayOf<BinaryDataBuffer>, MethId::Split, Separators_); };
		// Процедура Записать(Позиция_ Как Число, Байты_ Как БуферДвоичныхДанных, [Количество_ Как Число])
		void Write(ConstParam<Number> Position_, ConstParam<BinaryDataBuffer> Bytes_, ConstParam<Number> Number_ = DefUndef<esb::Number>) { ESB_INVOKE_MEMB_PROC3(MethId::Write, Position_, Bytes_, Number_); };
		// Процедура ЗаписатьЦелое16(Позиция_ Как Число, Значение_ Как Число, [ПорядокБайтов_ Как ПорядокБайтов])
		void WriteInt16(ConstParam<Number> Position_, ConstParam<Number> Value_, ConstParam<ByteOrder> ByteOrder_ = DefUndef<esb::ByteOrder>) { ESB_INVOKE_MEMB_PROC3(MethId::WriteInt16, Position_, Value_, ByteOrder_); };
		// Процедура ЗаписатьЦелое32(Позиция_ Как Число, Значение_ Как Число, [ПорядокБайтов_ Как ПорядокБайтов])
		void WriteInt32(ConstParam<Number> Position_, ConstParam<Number> Value_, ConstParam<ByteOrder> ByteOrder_ = DefUndef<esb::ByteOrder>) { ESB_INVOKE_MEMB_PROC3(MethId::WriteInt32, Position_, Value_, ByteOrder_); };
		// Процедура ЗаписатьЦелое64(Позиция_ Как Число, Значение_ Как Число, [ПорядокБайтов_ Как ПорядокБайтов])
		void WriteInt64(ConstParam<Number> Position_, ConstParam<Number> Value_, ConstParam<ByteOrder> ByteOrder_ = DefUndef<esb::ByteOrder>) { ESB_INVOKE_MEMB_PROC3(MethId::WriteInt64, Position_, Value_, ByteOrder_); };
	public:
		// Свойство [[Чтение,Запись]] ПорядокБайтов Как ПорядокБайтов
		ByteOrder GetByteOrder() const { ESB_INVOKE_PROPERTY_GET(esb::ByteOrder, PropId::ByteOrder); };
		void SetByteOrder(ConstParam<ByteOrder> Value_) { ESB_INVOKE_PROPERTY_SET(PropId::ByteOrder, Value_); };
		ESB_PROPERTY_FIELD_RW(esb::ByteOrder, ByteOrder)
		// Свойство [[ТолькоЧтение]] ТолькоЧтение Как Булево
		Boolean GetReadOnly() const { ESB_INVOKE_PROPERTY_GET(esb::Boolean, PropId::ReadOnly); };
		ESB_PROPERTY_FIELD_RO(Boolean, ReadOnly)
		// Свойство [[ТолькоЧтение]] Размер Как Число
		Number GetSize() const { ESB_INVOKE_PROPERTY_GET(esb::Number, PropId::Size); };
		ESB_PROPERTY_FIELD_RO(Number, Size)
#endif	// #if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
	public:
		static БуферДвоичныхДанных Создать(КонстПарам<Число> Размер_, КонстПарам<ПорядокБайтов> ПорядокБайтов_ = ПоУмолчаниюНеопределеноИли<esb::ПорядокБайтов>) { ESB_INVOKE_CTOR2(БуферДвоичныхДанных, Размер_, ПорядокБайтов_); }
	public:
		// Function Concat(BinaryDataBuffer Buffer_) -> BinaryDataBuffer
		БуферДвоичныхДанных Соединить(КонстПарам<БуферДвоичныхДанных> Буфер_) const { ESB_INVOKE_MEMB_FUNC1(esb::БуферДвоичныхДанных, MethId::Concat, Буфер_); };
		// Function Copy() -> BinaryDataBuffer
		БуферДвоичныхДанных Скопировать() const { ESB_INVOKE_MEMB_FUNC0(esb::БуферДвоичныхДанных, MethId::Copy); };
		// Function Get(Number Position_) -> Number
		Число Получить(КонстПарам<Число> Позиция_) const { ESB_INVOKE_MEMB_FUNC1(esb::Число, MethId::Get, Позиция_); };
		// Function GetSlice(Number Position_, Number Number_ = <default>) -> BinaryDataBuffer
		БуферДвоичныхДанных ПолучитьСрез(КонстПарам<Число> Позиция_, КонстПарам<Число> Количество_ = ПоУмолчаниюНеопределеноИли<esb::Число>) const { ESB_INVOKE_MEMB_FUNC2(esb::БуферДвоичныхДанных, MethId::GetSlice, Позиция_, Количество_); };
		// Function Read(Number Position_, Number Number_) -> BinaryDataBuffer
		БуферДвоичныхДанных Прочитать(КонстПарам<Число> Позиция_, КонстПарам<Число> Количество_) const { ESB_INVOKE_MEMB_FUNC2(esb::БуферДвоичныхДанных, MethId::Read, Позиция_, Количество_); };
		// Function ReadInt16(Number Position_, ByteOrder ByteOrder_ = <default>) -> Number
		Число ПрочитатьЦелое16(КонстПарам<Число> Позиция_, КонстПарам<ПорядокБайтов> ПорядокБайтов_ = ПоУмолчаниюНеопределеноИли<esb::ПорядокБайтов>) const { ESB_INVOKE_MEMB_FUNC2(esb::Число, MethId::ReadInt16, Позиция_, ПорядокБайтов_); };
		// Function ReadInt32(Number Position_, ByteOrder ByteOrder_ = <default>) -> Number
		Число ПрочитатьЦелое32(КонстПарам<Число> Позиция_, КонстПарам<ПорядокБайтов> ПорядокБайтов_ = ПоУмолчаниюНеопределеноИли<esb::ПорядокБайтов>) const { ESB_INVOKE_MEMB_FUNC2(esb::Число, MethId::ReadInt32, Позиция_, ПорядокБайтов_); };
		// Function ReadInt64(Number Position_ = <default>, ByteOrder ByteOrder_ = <default>) -> Number
		Число ПрочитатьЦелое64(КонстПарам<Число> Позиция_ = ПоУмолчаниюНеопределеноИли<esb::Число>, КонстПарам<ПорядокБайтов> ПорядокБайтов_ = ПоУмолчаниюНеопределеноИли<esb::ПорядокБайтов>) const { ESB_INVOKE_MEMB_FUNC2(esb::Число, MethId::ReadInt64, Позиция_, ПорядокБайтов_); };
		// Function Reverse() -> BinaryDataBuffer
		БуферДвоичныхДанных Перевернуть() const { ESB_INVOKE_MEMB_FUNC0(esb::БуферДвоичныхДанных, MethId::Reverse); };
		// Void Set(Number Position_, Number Value_)
		void Установить(КонстПарам<Число> Позиция_, КонстПарам<Число> Значение_) { ESB_INVOKE_MEMB_PROC2(MethId::Set, Позиция_, Значение_); };
		// Void SetReadOnly()
		void УстановитьТолькоЧтение() { ESB_INVOKE_MEMB_PROC0(MethId::SetReadOnly); };
		// Function Split(BinaryDataBuffer Separator_) -> ArrayOf<BinaryDataBuffer>
		МассивИз<БуферДвоичныхДанных> Разделить(КонстПарам<БуферДвоичныхДанных> Разделитель_) const { ESB_INVOKE_MEMB_FUNC1(esb::МассивИз<БуферДвоичныхДанных>, MethId::Split, Разделитель_); };
		// Function Split(BinaryDataBuffer Separator_) -> ArrayOf<BinaryDataBuffer>
		МассивИз<БуферДвоичныхДанных> Разделить(КонстПарам<Массив> Разделители_) const { ESB_INVOKE_MEMB_FUNC1(esb::МассивИз<БуферДвоичныхДанных>, MethId::Split, Разделители_); };
		// Void Write(Number Position_, BinaryDataBuffer Bytes_, Number Number_ = <default>)
		void Записать(КонстПарам<Число> Позиция_, КонстПарам<БуферДвоичныхДанных> Байты_, КонстПарам<Число> Количество_ = ПоУмолчаниюНеопределеноИли<esb::Число>) { ESB_INVOKE_MEMB_PROC3(MethId::Write, Позиция_, Байты_, Количество_); };
		// Void WriteInt16(Number Position_, Number Value_, ByteOrder ByteOrder_ = <default>)
		void ЗаписатьЦелое16(КонстПарам<Число> Позиция_, КонстПарам<Число> Значение_, КонстПарам<ПорядокБайтов> ПорядокБайтов_ = ПоУмолчаниюНеопределеноИли<esb::ПорядокБайтов>) { ESB_INVOKE_MEMB_PROC3(MethId::WriteInt16, Позиция_, Значение_, ПорядокБайтов_); };
		// Void WriteInt32(Number Position_, Number Value_, ByteOrder ByteOrder_ = <default>)
		void ЗаписатьЦелое32(КонстПарам<Число> Позиция_, КонстПарам<Число> Значение_, КонстПарам<ПорядокБайтов> ПорядокБайтов_ = ПоУмолчаниюНеопределеноИли<esb::ПорядокБайтов>) { ESB_INVOKE_MEMB_PROC3(MethId::WriteInt32, Позиция_, Значение_, ПорядокБайтов_); };
		// Void WriteInt64(Number Position_, Number Value_, ByteOrder ByteOrder_ = <default>)
		void ЗаписатьЦелое64(КонстПарам<Число> Позиция_, КонстПарам<Число> Значение_, КонстПарам<ПорядокБайтов> ПорядокБайтов_ = ПоУмолчаниюНеопределеноИли<esb::ПорядокБайтов>) { ESB_INVOKE_MEMB_PROC3(MethId::WriteInt64, Позиция_, Значение_, ПорядокБайтов_); };
	public:
		// Property [[Read,Write]] ByteOrder -> ByteOrder
		ПорядокБайтов ПолучитьПорядокБайтов() const { ESB_INVOKE_PROPERTY_GET(esb::ПорядокБайтов, PropId::ByteOrder); };
		void УстановитьПорядокБайтов(КонстПарам<ПорядокБайтов> Значение_) { ESB_INVOKE_PROPERTY_SET(PropId::ByteOrder, Значение_); };
		ESB_PROPERTY_FIELD_RW(esb::ПорядокБайтов, ПорядокБайтов)
		// Property [[ReadOnly]] ReadOnly -> Boolean
		Булево ПолучитьТолькоЧтение() const { ESB_INVOKE_PROPERTY_GET(esb::Булево, PropId::ReadOnly); };
		ESB_PROPERTY_FIELD_RO(Булево, ТолькоЧтение)
		// Property [[ReadOnly]] Size -> Number
		Число ПолучитьРазмер() const { ESB_INVOKE_PROPERTY_GET(esb::Число, PropId::Size); };
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
	{
		ESB_CLASS_IMPLEMENT_OBJECT(FileStream)
#if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
	public:
		static FileStream Create(ConstParam<String> FileName_, ConstParam<FileOpenMode> OpeningMode_, ConstParam<Number> BufferSize_ = DefUndef<esb::Number>) { ESB_INVOKE_CTOR3(FileStream, FileName_, OpeningMode_, BufferSize_); }
		static FileStream Create(ConstParam<String> FileName_, ConstParam<FileOpenMode> OpeningMode_, ConstParam<FileAccess> AccessLevel_, ConstParam<Number> BufferSize_ = DefUndef<esb::Number>) { ESB_INVOKE_CTOR4(FileStream, FileName_, OpeningMode_, AccessLevel_, BufferSize_); }
	public:
		// Процедура НачатьЗакрытие(ОписаниеОповещения_ Как ОписаниеОповещения)
		void BeginClose(ConstParam<NotifyDescription> NotifyDescription_) { ESB_INVOKE_MEMB_PROC1(MethId::BeginClose, NotifyDescription_); };
		// Процедура НачатьКопированиеВ(ОписаниеОповещения_ Как ОписаниеОповещения, ЦелевойПоток_ Как Поток, [РазмерБуфера_ Как Число])
		void BeginCopyTo(ConstParam<NotifyDescription> NotifyDescription_, ConstParam<Stream> TargetStream_, ConstParam<Number> BufferSize_ = DefUndef<esb::Number>) const { ESB_INVOKE_MEMB_PROC3(MethId::BeginCopyTo, NotifyDescription_, TargetStream_, BufferSize_); };
		// Процедура НачатьСбросБуферов(ОписаниеОповещения_ Как ОписаниеОповещения)
		void BeginFlush(ConstParam<NotifyDescription> NotifyDescription_) const { ESB_INVOKE_MEMB_PROC1(MethId::BeginFlush, NotifyDescription_); };
		// Процедура НачатьПолучениеРазмера([ОписаниеОповещения_ Как ОписаниеОповещения])
		void BeginGetSize(ConstParam<NotifyDescription> NotifyDescription_ = DefUndef<esb::NotifyDescription>) const { ESB_INVOKE_MEMB_PROC1(MethId::BeginGetSize, NotifyDescription_); };
		// Процедура НачатьЧтение(ОписаниеОповещения_ Как ОписаниеОповещения, Буфер_ Как БуферДвоичныхДанных, ПозицияВБуфере_ Как Число, Количество_ Как Число)
		void BeginRead(ConstParam<NotifyDescription> NotifyDescription_, ConstParam<BinaryDataBuffer> Buffer_, ConstParam<Number> PositionInBuffer_, ConstParam<Number> Number_) const { ESB_INVOKE_MEMB_PROC4(MethId::BeginRead, NotifyDescription_, Buffer_, PositionInBuffer_, Number_); };
		// Процедура НачатьПереход(ОписаниеОповещения_ Как ОписаниеОповещения, Смещение_ Как Число, [НачальнаяПозиция_ Как ПозицияВПотоке])
		void BeginSeek(ConstParam<NotifyDescription> NotifyDescription_, ConstParam<Number> Offset_, ConstParam<StreamPosition> InitialPosition_ = DefUndef<esb::StreamPosition>) const { ESB_INVOKE_MEMB_PROC3(MethId::BeginSeek, NotifyDescription_, Offset_, InitialPosition_); };
		// Процедура НачатьУстановкуРазмера(ОписаниеОповещения_ Как ОписаниеОповещения, Размер_ Как Число)
		void BeginSetSize(ConstParam<NotifyDescription> NotifyDescription_, ConstParam<Number> Size_) { ESB_INVOKE_MEMB_PROC2(MethId::BeginSetSize, NotifyDescription_, Size_); };
		// Процедура НачатьЗапись(ОписаниеОповещения_ Как ОписаниеОповещения, Буфер_ Как БуферДвоичныхДанных, ПозицияВБуфере_ Как Число, Количество_ Как Число)
		void BeginWrite(ConstParam<NotifyDescription> NotifyDescription_, ConstParam<BinaryDataBuffer> Buffer_, ConstParam<Number> PositionInBuffer_, ConstParam<Number> Count_) { ESB_INVOKE_MEMB_PROC4(MethId::BeginWrite, NotifyDescription_, Buffer_, PositionInBuffer_, Count_); };
		// Процедура Закрыть()
		void Close() { ESB_INVOKE_MEMB_PROC0(MethId::Close); };
		// Процедура КопироватьВ(ЦелевойПоток_ Как Поток, [РазмерБуфера_ Как Число])
		void CopyTo(ConstParam<Stream> TargetStream_, ConstParam<Number> BufferSize_ = DefUndef<esb::Number>) const { ESB_INVOKE_MEMB_PROC2(MethId::CopyTo, TargetStream_, BufferSize_); };
		// Функция ТекущаяПозиция() Возвращает Число
		Number CurrentPosition() const { ESB_INVOKE_MEMB_FUNC0(esb::Number, MethId::CurrentPosition); };
		// Процедура СброситьБуферы()
		void Flush() const { ESB_INVOKE_MEMB_PROC0(MethId::Flush); };
		// Функция ПолучитьПотокТолькоДляЧтения() Возвращает Поток
		Stream GetReadonlyStream() const { ESB_INVOKE_MEMB_FUNC0(esb::Stream, MethId::GetReadOnlyStream); };
		// Функция Прочитать(Буфер_ Как БуферДвоичныхДанных, ПозицияВБуфере_ Как Число, Количество_ Как Число) Возвращает Число
		Number Read(ConstParam<BinaryDataBuffer> Buffer_, ConstParam<Number> PositionInBuffer_, ConstParam<Number> Number_) const { ESB_INVOKE_MEMB_FUNC3(esb::Number, MethId::Read, Buffer_, PositionInBuffer_, Number_); };
		// Функция Перейти(Смещение_ Как Число, [НачальнаяПозиция_ Как ПозицияВПотоке]) Возвращает Число
		Number Seek(ConstParam<Number> Offset_, ConstParam<StreamPosition> InitialPosition_ = DefUndef<esb::StreamPosition>) const { ESB_INVOKE_MEMB_FUNC2(esb::Number, MethId::Seek, Offset_, InitialPosition_); };
		// Процедура УстановитьРазмер(Размер_ Как Число)
		void SetSize(ConstParam<Number> Size_) { ESB_INVOKE_MEMB_PROC1(MethId::SetSize, Size_); };
		// Функция Размер() Возвращает Число
		Number Size() const { ESB_INVOKE_MEMB_FUNC0(esb::Number, MethId::Size); };
		// Процедура Записать(Буфер_ Как БуферДвоичныхДанных, ПозицияВБуфере_ Как Число, Количество_ Как Число)
		void Write(ConstParam<BinaryDataBuffer> Buffer_, ConstParam<Number> PositionInBuffer_, ConstParam<Number> Number_) { ESB_INVOKE_MEMB_PROC3(MethId::Write, Buffer_, PositionInBuffer_, Number_); };
	public:
		// Свойство [[ТолькоЧтение]] ДоступноЧтение Как Булево
		Boolean GetCanRead() const { ESB_INVOKE_PROPERTY_GET(esb::Boolean, PropId::CanRead); };
		ESB_PROPERTY_FIELD_RO(Boolean, CanRead)
			// Свойство [[ТолькоЧтение]] ДоступноИзменениеПозиции Как Булево
			Boolean GetCanSeek() const { ESB_INVOKE_PROPERTY_GET(esb::Boolean, PropId::CanSeek); };
		ESB_PROPERTY_FIELD_RO(Boolean, CanSeek)
			// Свойство [[ТолькоЧтение]] ДоступнаЗапись Как Булево
			Boolean GetCanWrite() const { ESB_INVOKE_PROPERTY_GET(esb::Boolean, PropId::CanWrite); };
		ESB_PROPERTY_FIELD_RO(Boolean, CanWrite)
			// Свойство [[ТолькоЧтение]] ИмяФайла Как Строка
			String GetFileName() const { ESB_INVOKE_PROPERTY_GET(esb::String, PropId::FileName); };
		ESB_PROPERTY_FIELD_RO(String, FileName)
#endif	// #if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
	public:
		static ФайловыйПоток Создать(КонстПарам<Строка> ИмяФайла_, КонстПарам<РежимОткрытияФайла> РежимОткрытия_, КонстПарам<Число> РазмерБуфера_ = ПоУмолчаниюНеопределеноИли<esb::Число>) { ESB_INVOKE_CTOR3(ФайловыйПоток, ИмяФайла_, РежимОткрытия_, РазмерБуфера_); }
		static ФайловыйПоток Создать(КонстПарам<Строка> ИмяФайла_, КонстПарам<РежимОткрытияФайла> РежимОткрытия_, КонстПарам<ДоступКФайлу> УровеньДоступа_, КонстПарам<Число> РазмерБуфера_ = ПоУмолчаниюНеопределеноИли<esb::Число>) { ESB_INVOKE_CTOR4(ФайловыйПоток, ИмяФайла_, РежимОткрытия_, УровеньДоступа_, РазмерБуфера_); }
	public:
		// Void BeginClose(NotifyDescription NotifyDescription_)
		void НачатьЗакрытие(КонстПарам<ОписаниеОповещения> ОписаниеОповещения_) { ESB_INVOKE_MEMB_PROC1(MethId::BeginClose, ОписаниеОповещения_); };
		// Void BeginCopyTo(NotifyDescription NotifyDescription_, Stream TargetStream_, Number BufferSize_ = <default>)
		void НачатьКопированиеВ(КонстПарам<ОписаниеОповещения> ОписаниеОповещения_, КонстПарам<Поток> ЦелевойПоток_, КонстПарам<Число> РазмерБуфера_ = ПоУмолчаниюНеопределеноИли<esb::Число>) const { ESB_INVOKE_MEMB_PROC3(MethId::BeginCopyTo, ОписаниеОповещения_, ЦелевойПоток_, РазмерБуфера_); };
		// Void BeginFlush(NotifyDescription NotifyDescription_)
		void НачатьСбросБуферов(КонстПарам<ОписаниеОповещения> ОписаниеОповещения_) const { ESB_INVOKE_MEMB_PROC1(MethId::BeginFlush, ОписаниеОповещения_); };
		// Void BeginGetSize(NotifyDescription NotifyDescription_ = <default>)
		void НачатьПолучениеРазмера(КонстПарам<ОписаниеОповещения> ОписаниеОповещения_ = ПоУмолчаниюНеопределеноИли<esb::ОписаниеОповещения>) const { ESB_INVOKE_MEMB_PROC1(MethId::BeginGetSize, ОписаниеОповещения_); };
		// Void BeginRead(NotifyDescription NotifyDescription_, BinaryDataBuffer Buffer_, Number PositionInBuffer_, Number Number_)
		void НачатьЧтение(КонстПарам<ОписаниеОповещения> ОписаниеОповещения_, КонстПарам<БуферДвоичныхДанных> Буфер_, КонстПарам<Число> ПозицияВБуфере_, КонстПарам<Число> Количество_) const { ESB_INVOKE_MEMB_PROC4(MethId::BeginRead, ОписаниеОповещения_, Буфер_, ПозицияВБуфере_, Количество_); };
		// Void BeginSeek(NotifyDescription NotifyDescription_, Number Offset_, StreamPosition InitialPosition_ = <default>)
		void НачатьПереход(КонстПарам<ОписаниеОповещения> ОписаниеОповещения_, КонстПарам<Число> Смещение_, КонстПарам<ПозицияВПотоке> НачальнаяПозиция_ = ПоУмолчаниюНеопределеноИли<esb::ПозицияВПотоке>) const { ESB_INVOKE_MEMB_PROC3(MethId::BeginSeek, ОписаниеОповещения_, Смещение_, НачальнаяПозиция_); };
		// Void BeginSetSize(NotifyDescription NotifyDescription_, Number Size_)
		void НачатьУстановкуРазмера(КонстПарам<ОписаниеОповещения> ОписаниеОповещения_, КонстПарам<Число> Размер_) { ESB_INVOKE_MEMB_PROC2(MethId::BeginSetSize, ОписаниеОповещения_, Размер_); };
		// Void BeginWrite(NotifyDescription NotifyDescription_, BinaryDataBuffer Buffer_, Number PositionInBuffer_, Number Count_)
		void НачатьЗапись(КонстПарам<ОписаниеОповещения> ОписаниеОповещения_, КонстПарам<БуферДвоичныхДанных> Буфер_, КонстПарам<Число> ПозицияВБуфере_, КонстПарам<Число> Количество_) { ESB_INVOKE_MEMB_PROC4(MethId::BeginWrite, ОписаниеОповещения_, Буфер_, ПозицияВБуфере_, Количество_); };
		// Void Close()
		void Закрыть() { ESB_INVOKE_MEMB_PROC0(MethId::Close); };
		// Void CopyTo(Stream TargetStream_, Number BufferSize_ = <default>)
		void КопироватьВ(КонстПарам<Поток> ЦелевойПоток_, КонстПарам<Число> РазмерБуфера_ = ПоУмолчаниюНеопределеноИли<esb::Число>) const { ESB_INVOKE_MEMB_PROC2(MethId::CopyTo, ЦелевойПоток_, РазмерБуфера_); };
		// Function CurrentPosition() -> Number
		Число ТекущаяПозиция() const { ESB_INVOKE_MEMB_FUNC0(esb::Число, MethId::CurrentPosition); };
		// Void Flush()
		void СброситьБуферы() const { ESB_INVOKE_MEMB_PROC0(MethId::Flush); };
		// Function GetReadonlyStream() -> Stream
		Поток ПолучитьПотокТолькоДляЧтения() const { ESB_INVOKE_MEMB_FUNC0(esb::Поток, MethId::GetReadOnlyStream); };
		// Function Read(BinaryDataBuffer Buffer_, Number PositionInBuffer_, Number Number_) -> Number
		Число Прочитать(КонстПарам<БуферДвоичныхДанных> Буфер_, КонстПарам<Число> ПозицияВБуфере_, КонстПарам<Число> Количество_) const { ESB_INVOKE_MEMB_FUNC3(esb::Число, MethId::Read, Буфер_, ПозицияВБуфере_, Количество_); };
		// Function Seek(Number Offset_, StreamPosition InitialPosition_ = <default>) -> Number
		Число Перейти(КонстПарам<Число> Смещение_, КонстПарам<ПозицияВПотоке> НачальнаяПозиция_ = ПоУмолчаниюНеопределеноИли<esb::ПозицияВПотоке>) const { ESB_INVOKE_MEMB_FUNC2(esb::Число, MethId::Seek, Смещение_, НачальнаяПозиция_); };
		// Void SetSize(Number Size_)
		void УстановитьРазмер(КонстПарам<Число> Размер_) { ESB_INVOKE_MEMB_PROC1(MethId::SetSize, Размер_); };
		// Function Size() -> Number
		Число Размер() const { ESB_INVOKE_MEMB_FUNC0(esb::Число, MethId::Size); };
		// Void Write(BinaryDataBuffer Buffer_, Number PositionInBuffer_, Number Number_)
		void Записать(КонстПарам<БуферДвоичныхДанных> Буфер_, КонстПарам<Число> ПозицияВБуфере_, КонстПарам<Число> Количество_) { ESB_INVOKE_MEMB_PROC3(MethId::Write, Буфер_, ПозицияВБуфере_, Количество_); };
	public:
		// Property [[ReadOnly]] CanRead -> Boolean
		Булево ПолучитьДоступноЧтение() const { ESB_INVOKE_PROPERTY_GET(esb::Булево, PropId::CanRead); };
		ESB_PROPERTY_FIELD_RO(Булево, ДоступноЧтение)
			// Property [[ReadOnly]] CanSeek -> Boolean
			Булево ПолучитьДоступноИзменениеПозиции() const { ESB_INVOKE_PROPERTY_GET(esb::Булево, PropId::CanSeek); };
		ESB_PROPERTY_FIELD_RO(Булево, ДоступноИзменениеПозиции)
			// Property [[ReadOnly]] CanWrite -> Boolean
			Булево ПолучитьДоступнаЗапись() const { ESB_INVOKE_PROPERTY_GET(esb::Булево, PropId::CanWrite); };
		ESB_PROPERTY_FIELD_RO(Булево, ДоступнаЗапись)
			// Property [[ReadOnly]] FileName -> String
			Строка ПолучитьИмяФайла() const { ESB_INVOKE_PROPERTY_GET(esb::Строка, PropId::FileName); };
		ESB_PROPERTY_FIELD_RO(Строка, ИмяФайла)
#endif	// #if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
	};


	ESB_CLASS_OBJECT(MemoryStream)
	{
		ESB_CLASS_IMPLEMENT_OBJECT(MemoryStream)
#if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
	public:
		static MemoryStream Create() { ESB_INVOKE_CTOR0(MemoryStream); }
		static MemoryStream Create(ConstParam<Number> InitialCapacity_) { ESB_INVOKE_CTOR1(MemoryStream, InitialCapacity_); }
		static MemoryStream Create(ConstParam<BinaryDataBuffer> Buffer_) { ESB_INVOKE_CTOR1(MemoryStream, Buffer_); }
	public:
		// Процедура НачатьЗакрытие(ОписаниеОповещения_ Как ОписаниеОповещения)
		void BeginClose(ConstParam<NotifyDescription> NotifyDescription_) { ESB_INVOKE_MEMB_PROC1(MethId::BeginClose, NotifyDescription_); };
		// Процедура НачатьКопированиеВ(ОписаниеОповещения_ Как ОписаниеОповещения, ЦелевойПоток_ Как Поток, [РазмерБуфера_ Как Число])
		void BeginCopyTo(ConstParam<NotifyDescription> NotifyDescription_, ConstParam<Stream> TargetStream_, ConstParam<Number> BufferSize_ = DefUndef<esb::Number>) const { ESB_INVOKE_MEMB_PROC3(MethId::BeginCopyTo, NotifyDescription_, TargetStream_, BufferSize_); };
		// Процедура НачатьСбросБуферов(ОписаниеОповещения_ Как ОписаниеОповещения)
		void BeginFlush(ConstParam<NotifyDescription> NotifyDescription_) const { ESB_INVOKE_MEMB_PROC1(MethId::BeginFlush, NotifyDescription_); };
		// Процедура НачатьПолучениеРазмера([ОписаниеОповещения_ Как ОписаниеОповещения])
		void BeginGetSize(ConstParam<NotifyDescription> NotifyDescription_ = DefUndef<esb::NotifyDescription>) const { ESB_INVOKE_MEMB_PROC1(MethId::BeginGetSize, NotifyDescription_); };
		// Процедура НачатьЧтение(ОписаниеОповещения_ Как ОписаниеОповещения, Буфер_ Как БуферДвоичныхДанных, ПозицияВБуфере_ Как Число, Количество_ Как Число)
		void BeginRead(ConstParam<NotifyDescription> NotifyDescription_, ConstParam<BinaryDataBuffer> Buffer_, ConstParam<Number> PositionInBuffer_, ConstParam<Number> Number_) const { ESB_INVOKE_MEMB_PROC4(MethId::BeginRead, NotifyDescription_, Buffer_, PositionInBuffer_, Number_); };
		// Процедура НачатьПереход(ОписаниеОповещения_ Как ОписаниеОповещения, Смещение_ Как Число, [НачальнаяПозиция_ Как ПозицияВПотоке])
		void BeginSeek(ConstParam<NotifyDescription> NotifyDescription_, ConstParam<Number> Offset_, ConstParam<StreamPosition> InitialPosition_ = DefUndef<esb::StreamPosition>) const { ESB_INVOKE_MEMB_PROC3(MethId::BeginSeek, NotifyDescription_, Offset_, InitialPosition_); };
		// Процедура НачатьУстановкуРазмера(ОписаниеОповещения_ Как ОписаниеОповещения, Размер_ Как Число)
		void BeginSetSize(ConstParam<NotifyDescription> NotifyDescription_, ConstParam<Number> Size_) { ESB_INVOKE_MEMB_PROC2(MethId::BeginSetSize, NotifyDescription_, Size_); };
		// Процедура НачатьЗапись(ОписаниеОповещения_ Как ОписаниеОповещения, Буфер_ Как БуферДвоичныхДанных, ПозицияВБуфере_ Как Число, Количество_ Как Число)
		void BeginWrite(ConstParam<NotifyDescription> NotifyDescription_, ConstParam<BinaryDataBuffer> Buffer_, ConstParam<Number> PositionInBuffer_, ConstParam<Number> Count_) { ESB_INVOKE_MEMB_PROC4(MethId::BeginWrite, NotifyDescription_, Buffer_, PositionInBuffer_, Count_); };
		// Процедура Закрыть()
		void Close() { ESB_INVOKE_MEMB_PROC0(MethId::Close); };
		// Процедура КопироватьВ(ЦелевойПоток_ Как Поток, [РазмерБуфера_ Как Число])
		void CopyTo(ConstParam<Stream> TargetStream_, ConstParam<Number> BufferSize_ = DefUndef<esb::Number>) const { ESB_INVOKE_MEMB_PROC2(MethId::CopyTo, TargetStream_, BufferSize_); };
		// Функция ТекущаяПозиция() Возвращает Число
		Number CurrentPosition() const { ESB_INVOKE_MEMB_FUNC0(esb::Number, MethId::CurrentPosition); };
		// Процедура СброситьБуферы()
		void Flush() const { ESB_INVOKE_MEMB_PROC0(MethId::Flush); };
		// Функция ПолучитьПотокТолькоДляЧтения() Возвращает Поток
		Stream GetReadonlyStream() const { ESB_INVOKE_MEMB_FUNC0(esb::Stream, MethId::GetReadOnlyStream); };
		// Функция Прочитать(Буфер_ Как БуферДвоичныхДанных, ПозицияВБуфере_ Как Число, Количество_ Как Число) Возвращает Число
		Number Read(ConstParam<BinaryDataBuffer> Buffer_, ConstParam<Number> PositionInBuffer_, ConstParam<Number> Number_) const { ESB_INVOKE_MEMB_FUNC3(esb::Number, MethId::Read, Buffer_, PositionInBuffer_, Number_); };
		// Функция Перейти(Смещение_ Как Число, [НачальнаяПозиция_ Как ПозицияВПотоке]) Возвращает Число
		Number Seek(ConstParam<Number> Offset_, ConstParam<StreamPosition> InitialPosition_ = DefUndef<esb::StreamPosition>) const { ESB_INVOKE_MEMB_FUNC2(esb::Number, MethId::Seek, Offset_, InitialPosition_); };
		// Процедура УстановитьРазмер(Размер_ Как Число)
		void SetSize(ConstParam<Number> Size_) { ESB_INVOKE_MEMB_PROC1(MethId::SetSize, Size_); };
		// Функция Размер() Возвращает Число
		Number Size() const { ESB_INVOKE_MEMB_FUNC0(esb::Number, MethId::Size); };
		// Процедура Записать(Буфер_ Как БуферДвоичныхДанных, ПозицияВБуфере_ Как Число, Количество_ Как Число)
		void Write(ConstParam<BinaryDataBuffer> Buffer_, ConstParam<Number> PositionInBuffer_, ConstParam<Number> Number_) { ESB_INVOKE_MEMB_PROC3(MethId::Write, Buffer_, PositionInBuffer_, Number_); };
		// Функция ЗакрытьИПолучитьДвоичныеДанные() Возвращает ДвоичныеДанные
		BinaryData CloseAndGetBinaryData() { ESB_INVOKE_MEMB_FUNC0(esb::BinaryData, MethId::CloseAndGetBinaryData); };
	public:
		// Свойство [[ТолькоЧтение]] ДоступноЧтение Как Булево
		Boolean GetCanRead() const { ESB_INVOKE_PROPERTY_GET(esb::Boolean, PropId::CanRead); };
		ESB_PROPERTY_FIELD_RO(Boolean, CanRead)
			// Свойство [[ТолькоЧтение]] ДоступноИзменениеПозиции Как Булево
			Boolean GetCanSeek() const { ESB_INVOKE_PROPERTY_GET(esb::Boolean, PropId::CanSeek); };
		ESB_PROPERTY_FIELD_RO(Boolean, CanSeek)
			// Свойство [[ТолькоЧтение]] ДоступнаЗапись Как Булево
			Boolean GetCanWrite() const { ESB_INVOKE_PROPERTY_GET(esb::Boolean, PropId::CanWrite); };
		ESB_PROPERTY_FIELD_RO(Boolean, CanWrite)
#endif	// #if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
	public:
		static ПотокВПамяти Создать() { ESB_INVOKE_CTOR0(ПотокВПамяти); }
		static ПотокВПамяти Создать(КонстПарам<Число> НачальнаяЕмкость_) { ESB_INVOKE_CTOR1(ПотокВПамяти, НачальнаяЕмкость_); }
		static ПотокВПамяти Создать(КонстПарам<БуферДвоичныхДанных> Буфер_) { ESB_INVOKE_CTOR1(ПотокВПамяти, Буфер_); }
	public:
		// Void BeginClose(NotifyDescription NotifyDescription_)
		void НачатьЗакрытие(КонстПарам<ОписаниеОповещения> ОписаниеОповещения_) { ESB_INVOKE_MEMB_PROC1(MethId::BeginClose, ОписаниеОповещения_); };
		// Void BeginCopyTo(NotifyDescription NotifyDescription_, Stream TargetStream_, Number BufferSize_ = <default>)
		void НачатьКопированиеВ(КонстПарам<ОписаниеОповещения> ОписаниеОповещения_, КонстПарам<Поток> ЦелевойПоток_, КонстПарам<Число> РазмерБуфера_ = ПоУмолчаниюНеопределеноИли<esb::Число>) const { ESB_INVOKE_MEMB_PROC3(MethId::BeginCopyTo, ОписаниеОповещения_, ЦелевойПоток_, РазмерБуфера_); };
		// Void BeginFlush(NotifyDescription NotifyDescription_)
		void НачатьСбросБуферов(КонстПарам<ОписаниеОповещения> ОписаниеОповещения_) const { ESB_INVOKE_MEMB_PROC1(MethId::BeginFlush, ОписаниеОповещения_); };
		// Void BeginGetSize(NotifyDescription NotifyDescription_ = <default>)
		void НачатьПолучениеРазмера(КонстПарам<ОписаниеОповещения> ОписаниеОповещения_ = ПоУмолчаниюНеопределеноИли<esb::ОписаниеОповещения>) const { ESB_INVOKE_MEMB_PROC1(MethId::BeginGetSize, ОписаниеОповещения_); };
		// Void BeginRead(NotifyDescription NotifyDescription_, BinaryDataBuffer Buffer_, Number PositionInBuffer_, Number Number_)
		void НачатьЧтение(КонстПарам<ОписаниеОповещения> ОписаниеОповещения_, КонстПарам<БуферДвоичныхДанных> Буфер_, КонстПарам<Число> ПозицияВБуфере_, КонстПарам<Число> Количество_) const { ESB_INVOKE_MEMB_PROC4(MethId::BeginRead, ОписаниеОповещения_, Буфер_, ПозицияВБуфере_, Количество_); };
		// Void BeginSeek(NotifyDescription NotifyDescription_, Number Offset_, StreamPosition InitialPosition_ = <default>)
		void НачатьПереход(КонстПарам<ОписаниеОповещения> ОписаниеОповещения_, КонстПарам<Число> Смещение_, КонстПарам<ПозицияВПотоке> НачальнаяПозиция_ = ПоУмолчаниюНеопределеноИли<esb::ПозицияВПотоке>) const { ESB_INVOKE_MEMB_PROC3(MethId::BeginSeek, ОписаниеОповещения_, Смещение_, НачальнаяПозиция_); };
		// Void BeginSetSize(NotifyDescription NotifyDescription_, Number Size_)
		void НачатьУстановкуРазмера(КонстПарам<ОписаниеОповещения> ОписаниеОповещения_, КонстПарам<Число> Размер_) { ESB_INVOKE_MEMB_PROC2(MethId::BeginSetSize, ОписаниеОповещения_, Размер_); };
		// Void BeginWrite(NotifyDescription NotifyDescription_, BinaryDataBuffer Buffer_, Number PositionInBuffer_, Number Count_)
		void НачатьЗапись(КонстПарам<ОписаниеОповещения> ОписаниеОповещения_, КонстПарам<БуферДвоичныхДанных> Буфер_, КонстПарам<Число> ПозицияВБуфере_, КонстПарам<Число> Количество_) { ESB_INVOKE_MEMB_PROC4(MethId::BeginWrite, ОписаниеОповещения_, Буфер_, ПозицияВБуфере_, Количество_); };
		// Void Close()
		void Закрыть() { ESB_INVOKE_MEMB_PROC0(MethId::Close); };
		// Void CopyTo(Stream TargetStream_, Number BufferSize_ = <default>)
		void КопироватьВ(КонстПарам<Поток> ЦелевойПоток_, КонстПарам<Число> РазмерБуфера_ = ПоУмолчаниюНеопределеноИли<esb::Число>) const { ESB_INVOKE_MEMB_PROC2(MethId::CopyTo, ЦелевойПоток_, РазмерБуфера_); };
		// Function CurrentPosition() -> Number
		Число ТекущаяПозиция() const { ESB_INVOKE_MEMB_FUNC0(esb::Число, MethId::CurrentPosition); };
		// Void Flush()
		void СброситьБуферы() const { ESB_INVOKE_MEMB_PROC0(MethId::Flush); };
		// Function GetReadonlyStream() -> Stream
		Поток ПолучитьПотокТолькоДляЧтения() const { ESB_INVOKE_MEMB_FUNC0(esb::Поток, MethId::GetReadOnlyStream); };
		// Function Read(BinaryDataBuffer Buffer_, Number PositionInBuffer_, Number Number_) -> Number
		Число Прочитать(КонстПарам<БуферДвоичныхДанных> Буфер_, КонстПарам<Число> ПозицияВБуфере_, КонстПарам<Число> Количество_) const { ESB_INVOKE_MEMB_FUNC3(esb::Число, MethId::Read, Буфер_, ПозицияВБуфере_, Количество_); };
		// Function Seek(Number Offset_, StreamPosition InitialPosition_ = <default>) -> Number
		Число Перейти(КонстПарам<Число> Смещение_, КонстПарам<ПозицияВПотоке> НачальнаяПозиция_ = ПоУмолчаниюНеопределеноИли<esb::ПозицияВПотоке>) const { ESB_INVOKE_MEMB_FUNC2(esb::Число, MethId::Seek, Смещение_, НачальнаяПозиция_); };
		// Void SetSize(Number Size_)
		void УстановитьРазмер(КонстПарам<Число> Размер_) { ESB_INVOKE_MEMB_PROC1(MethId::SetSize, Размер_); };
		// Function Size() -> Number
		Число Размер() const { ESB_INVOKE_MEMB_FUNC0(esb::Число, MethId::Size); };
		// Void Write(BinaryDataBuffer Buffer_, Number PositionInBuffer_, Number Number_)
		void Записать(КонстПарам<БуферДвоичныхДанных> Буфер_, КонстПарам<Число> ПозицияВБуфере_, КонстПарам<Число> Количество_) { ESB_INVOKE_MEMB_PROC3(MethId::Write, Буфер_, ПозицияВБуфере_, Количество_); };
		// Function CloseAndGetBinaryData() -> BinaryData
		ДвоичныеДанные ЗакрытьИПолучитьДвоичныеДанные() { ESB_INVOKE_MEMB_FUNC0(esb::ДвоичныеДанные, MethId::CloseAndGetBinaryData); };
	public:
		// Property [[ReadOnly]] CanRead -> Boolean
		Булево ПолучитьДоступноЧтение() const { ESB_INVOKE_PROPERTY_GET(esb::Булево, PropId::CanRead); };
		ESB_PROPERTY_FIELD_RO(Булево, ДоступноЧтение)
			// Property [[ReadOnly]] CanSeek -> Boolean
			Булево ПолучитьДоступноИзменениеПозиции() const { ESB_INVOKE_PROPERTY_GET(esb::Булево, PropId::CanSeek); };
		ESB_PROPERTY_FIELD_RO(Булево, ДоступноИзменениеПозиции)
			// Property [[ReadOnly]] CanWrite -> Boolean
			Булево ПолучитьДоступнаЗапись() const { ESB_INVOKE_PROPERTY_GET(esb::Булево, PropId::CanWrite); };
		ESB_PROPERTY_FIELD_RO(Булево, ДоступнаЗапись)
#endif	// #if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
	};

}	// end namespace esb 



namespace esb 
{
		// Picture объявлено в es1_main.h
#if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
		inline BinaryData Picture::GetBinaryData() { ESB_INVOKE_MEMB_FUNC0(esb::BinaryData, MethId::GetBinaryData); };
#endif
#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
		inline ДвоичныеДанные Картинка::ПолучитьДвоичныеДанные() { ESB_INVOKE_MEMB_FUNC0(esb::ДвоичныеДанные, MethId::GetBinaryData); };
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
#if ESB_API_PROVIDER_INCLUDE(ext0xA)
#if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
// Функция СоединитьБуферыДвоичныхДанных(Части_ Как Массив, [Разделитель_ Как БуферДвоичныхДанных], [ПорядокБайтов_ Как ПорядокБайтов]) Возвращает БуферДвоичныхДанных
	inline BinaryDataBuffer ConcatBinaryDataBuffers(ConstParam<Array> Parts_, ConstParam<BinaryDataBuffer> Separator_ = DefUndef<esb::BinaryDataBuffer>, ConstParam<ByteOrder> ByteOrder_ = DefUndef<esb::ByteOrder>) { ESB_INVOKE_API_FUNC3(BinaryDataBuffer, ext0xA, ConcatBinaryDataBuffers, Parts_, Separator_, ByteOrder_); };
#endif	// #if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
	// Function ConcatBinaryDataBuffers(Array Parts_, BinaryDataBuffer Separator_ = <default>, ByteOrder ByteOrder_ = <default>) -> BinaryDataBuffer
	inline БуферДвоичныхДанных СоединитьБуферыДвоичныхДанных(КонстПарам<Массив> Части_, КонстПарам<БуферДвоичныхДанных> Разделитель_ = ПоУмолчаниюНеопределеноИли<esb::БуферДвоичныхДанных>, КонстПарам<ПорядокБайтов> ПорядокБайтов_ = ПоУмолчаниюНеопределеноИли<esb::ПорядокБайтов>) { ESB_INVOKE_API_FUNC3(БуферДвоичныхДанных, ext0xA, ConcatBinaryDataBuffers, Части_, Разделитель_, ПорядокБайтов_); };
#endif	// #if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
#endif	// #if ESB_API_INCLUDE_ext0xA


#if ESB_API_PROVIDER_INCLUDE(core83)
#if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
	// Функция Base64Значение(Строка_ Как Строка) Возвращает ДвоичныеДанные
	inline BinaryData Base64Value(ConstParam<String> Line_) { ESB_INVOKE_API_FUNC1(BinaryData, core83, Base64Value, Line_); };
	// Функция Base64Строка(Значение_ Как ДвоичныеДанные) Возвращает Строка
	inline String Base64String(ConstParam<BinaryData> Value_) { ESB_INVOKE_API_FUNC1(String, core83, Base64String, Value_); };
#endif	// #if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
	// Function Base64Value(String Line_) -> BinaryData
	inline ДвоичныеДанные Base64Значение(КонстПарам<Строка> Строка_) { ESB_INVOKE_API_FUNC1(ДвоичныеДанные, core83, Base64Value, Строка_); };
	// Function Base64String(BinaryData Value_) -> String
	inline Строка Base64Строка(КонстПарам<ДвоичныеДанные> Значение_) { ESB_INVOKE_API_FUNC1(Строка, core83, Base64String, Значение_); };
#endif	// #if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
#endif	// #if ESB_API_INCLUDE_core83
}


#endif //ESB_ES1_FILE_H_
