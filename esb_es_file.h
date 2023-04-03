/*
	"ESB" is designed to create external native components for the 1C(tm) program.
	Copyright © 2023 IntUnsigned	(v8classes@gmail.com)
	"ESB" is free software under LGPLv2.1 license with essential restriction about binary part (view esb-license.txt)
	"ESB" IS DISTRIBUTED "AS-IS" WITHOUT ANY, EVEN THE IMPLIED, WARRANTY. IN NO EVENT SHALL THE AUTHORS OR THE COPYRIGHT OWNER BE LIABLE FOR ANY CLAIM, 
	DAMAGES OR OTHER LIABILITY ARISING OUT OF THE USE OF THIS SOFTWARE.
	THE AUTHOR DOES NOT GIVE ANY GUARANTEES AND DOES NOT BEAR ANY RESPONSIBILITY REGARDING THE USE OF 1C(tm) COMPONENTS PRODUCED USING THE ESB LIBRARY IN THE 1C(tm) ENVIRONMENT.
*/


#pragma once
#ifndef ESBFILE_H
#define ESBFILE_H

#include "esb.h"



namespace esb {
//
	class Stream;
	class BinaryData;
	class BinaryDataBuffer;
	class FileStream;
	class MemoryStream;
//

	//TOBE:	Относительно Stream и его вариантов - FileStream/MemoryStream нужно подумать над "интерфейсом" - объекты разные, а интерфейс-то один
	//		В 1С такого понятия нет - там интерфейс "утиный - у кого метод Flush, тот и поток..", а у нас-то все строго!
	//		также при этом нужно учесть что каждый поток может быть GetCanRead/GetCanWrite/GetCanSeek и мы это тоже можем сделать интерфейсами
	//		как у IIndexedCollection - StreamRO/StreamWO etc..
	//		Или не надо.. в конце концов они здесь не для работы, а получить/записать и забыть.
	class Stream : public Object {
		friend Stream make<Stream>(IValuePtr&&);
		Stream(IValuePtr&& val_, IObjectPtr&& obj_) throw() : Object(std::move(val_), std::move(obj_)) {}
		struct MethId { ESB_ID(Close, 0); ESB_ID(Size, 1); ESB_ID(GetReadOnlyStream, 2); ESB_ID(CurrentPosition, 3); ESB_ID(CopyTo, 4); ESB_ID(Flush, 5); ESB_ID(Seek, 6); ESB_ID(SetSize, 7); ESB_ID(Read, 8); ESB_ID(Write, 9); ESB_ID(BeginGetSize, 10); ESB_ID(BeginCopyTo, 11); ESB_ID(BeginSeek, 12); ESB_ID(BeginSetSize, 13); ESB_ID(BeginRead, 14); ESB_ID(BeginWrite, 15); ESB_ID(BeginFlush, 16); ESB_ID(BeginClose, 17); };
		struct PropId { ESB_ID(CanRead, 0); ESB_ID(CanWrite, 1); ESB_ID(CanSeek, 2); };
	public:
		ESB_DECLARE_TYPEINFO()
	public:
		// Not Creatable
	public:
		// Процедура Закрыть()
		void  Close()				{ ESB_INVOKE_MEMB_PROC0(MethId::Close); }
		// Функция Размер() Возвращает Число
		Numeric Size() const		{ ESB_INVOKE_MEMB_FUNC0(Numeric, MethId::Size); }
		Stream  GetReadOnlyStream() const	{ ESB_INVOKE_MEMB_FUNC0(Stream, MethId::GetReadOnlyStream); }
		Numeric CurrentPosition() const		{ ESB_INVOKE_MEMB_FUNC0(Numeric, MethId::CurrentPosition); }
		void  CopyTo(ConstPara<Stream> TargetStream_, ConstPara<Numeric> Count_ = DefUndef<Numeric>) { ESB_INVOKE_MEMB_PROC2(MethId::CopyTo, TargetStream_, Count_); }
		void  Flush() const { ESB_INVOKE_MEMB_PROC0(MethId::Flush); }
		Numeric  Seek(ConstPara<Numeric> Offset_, ConstPara<PositionInStreamValue> InitialPosition_ = DefUndef<PositionInStreamValue>) { ESB_INVOKE_MEMB_FUNC2(Numeric, MethId::Seek, Offset_, InitialPosition_); }
		void  SetSize(ConstPara<Numeric> Size_) { ESB_INVOKE_MEMB_PROC1(MethId::SetSize, Size_); }
		Numeric  Read(ConstPara<BinaryDataBuffer> Buffer_, ConstPara<Numeric> PositionInBuffer_, ConstPara<Numeric> Kolichestvo_) { ESB_INVOKE_MEMB_FUNC3(Numeric, MethId::Read, Buffer_, PositionInBuffer_, Kolichestvo_); }
		void  Write(ConstPara<BinaryDataBuffer> Buffer_, ConstPara<Numeric> PositionInBuffer_, ConstPara<Numeric> Number_) { ESB_INVOKE_MEMB_PROC3(MethId::Write, Buffer_, PositionInBuffer_, Number_); }
		void  BeginGetSize(ConstPara<NotifyDescription> NotifyDescription_) { ESB_INVOKE_MEMB_PROC1(MethId::BeginGetSize, NotifyDescription_); }
		void  BeginCopyTo(ConstPara<NotifyDescription> NotifyDescription_, ConstPara<Stream> TargetStream_, ConstPara<Numeric> Count_ = DefUndef<Numeric>) { ESB_INVOKE_MEMB_PROC3(MethId::BeginCopyTo, NotifyDescription_, TargetStream_, Count_); }
		void  BeginSeek(ConstPara<NotifyDescription> NotifyDescription_, ConstPara<Numeric> Offset_, ConstPara<PositionInStreamValue> InitialPosition_ = DefUndef<PositionInStreamValue>) { ESB_INVOKE_MEMB_PROC3(MethId::BeginSeek, NotifyDescription_, Offset_, InitialPosition_); }
		void  BeginSetSize(ConstPara<NotifyDescription> NotifyDescription_, ConstPara<Numeric> Size_) { ESB_INVOKE_MEMB_PROC2(MethId::BeginSetSize, NotifyDescription_, Size_); }
		void  BeginRead(ConstPara<NotifyDescription> NotifyDescription_, ConstPara<BinaryDataBuffer> Buffer_, ConstPara<Numeric> PositionInBuffer_, ConstPara<Numeric> Number_) { ESB_INVOKE_MEMB_PROC4(MethId::BeginRead, NotifyDescription_, Buffer_, PositionInBuffer_, Number_); }
		void  BeginWrite(ConstPara<NotifyDescription> NotifyDescription_, ConstPara<BinaryDataBuffer> Buffer_, ConstPara<Numeric> PositionInBuffer_, ConstPara<Numeric> Count_) { ESB_INVOKE_MEMB_PROC4(MethId::BeginWrite, NotifyDescription_, Buffer_, PositionInBuffer_, Count_); }
		void  BeginFlush(ConstPara<NotifyDescription> NotifyDescription_) { ESB_INVOKE_MEMB_PROC1(MethId::BeginFlush, NotifyDescription_); }
		void  BeginClose(ConstPara<NotifyDescription> NotifyDescription_) { ESB_INVOKE_MEMB_PROC1(MethId::BeginClose, NotifyDescription_); }
	public:
		Boolean GetCanRead() const		{ ESB_INVOKE_PROPERTY_GET(Boolean, PropId::CanRead); }
		ESB_PROPERTY_FIELD_RO(Boolean, CanRead);
		Boolean GetCanWrite() const		{ ESB_INVOKE_PROPERTY_GET(Boolean, PropId::CanWrite); }
		ESB_PROPERTY_FIELD_RO(Boolean, CanWrite);
		Boolean GetCanSeek() const		{ ESB_INVOKE_PROPERTY_GET(Boolean, PropId::CanSeek); }
		ESB_PROPERTY_FIELD_RO(Boolean, CanSeek);
	};


	class BinaryData : public Object {
		friend BinaryData make<BinaryData>(IValuePtr&&);
		BinaryData(IValuePtr&& val_, IObjectPtr&& obj_) throw() : Object(std::move(val_), std::move(obj_)) {}
		struct MethId { ESB_ID(Write, 0); ESB_ID(Size, 1); ESB_ID(OpenStreamForRead, 2); ESB_ID(BeginWrite, 3); };
	public:
		ESB_DECLARE_TYPEINFO()
	public:
		static BinaryData  Create(ConstPara<String> FileName_) { ESB_INVOKE_CTOR1(BinaryData, FileName_); }
	public:
		//-METH: -para-size-mismatch: Write
		void  Write(ConstPara<Stream> Stream_) const	{ ESB_INVOKE_MEMB_PROC1(MethId::Write, Stream_); }
		void  Write(ConstPara<String> FileName_) const	{ ESB_INVOKE_MEMB_PROC1(MethId::Write, FileName_); }
		Numeric  Size() const							{ ESB_INVOKE_MEMB_FUNC0(Numeric, MethId::Size); }
		Stream  OpenStreamForRead() const				{ ESB_INVOKE_MEMB_FUNC0(Stream, MethId::OpenStreamForRead); }
		//-METH: -para-size-mismatch: BeginWrite
		void  BeginWrite(ConstPara<NotifyDescription> NotifyDescription_, ConstPara<Stream> Stream_) const		{ ESB_INVOKE_MEMB_PROC2(MethId::BeginWrite, NotifyDescription_, Stream_); }
		void  BeginWrite(ConstPara<NotifyDescription> NotifyDescription_, ConstPara<String> FileName_) const	{ ESB_INVOKE_MEMB_PROC2(MethId::BeginWrite, NotifyDescription_, FileName_); }
	};


	class BinaryDataBuffer : public ObjectDual<IxCollectionRW<Numeric>> {
		friend BinaryDataBuffer make<BinaryDataBuffer>(IValuePtr&&);
		BinaryDataBuffer(IValuePtr&& val_, IObjectPtr&& obj_, IIxCollectionRWPtr&& ixc_) throw() : ObjectDual(std::move(val_), std::move(obj_), std::move(ixc_)) {}
		struct MethId { ESB_ID(SetReadOnly, 0); ESB_ID(GetSlice, 1); ESB_ID(Get, 2); ESB_ID(Set, 3); ESB_ID(Read, 4); ESB_ID(ReadInt16, 5); ESB_ID(ReadInt32, 6); ESB_ID(ReadInt64, 7); ESB_ID(WriteInt16, 8); ESB_ID(WriteInt32, 9); ESB_ID(WriteInt64, 10); ESB_ID(Copy, 11); ESB_ID(Write, 12); ESB_ID(Reverse, 13); ESB_ID(Concat, 14); ESB_ID(Split, 15); };
		struct PropId { ESB_ID(Size, 0); ESB_ID(ReadOnly, 1); ESB_ID(ByteOrder, 2); };
	public:
		ESB_DECLARE_TYPEINFO()
		ESB_IMPLEMENT_ARGBYVAL_OPERATOR(BinaryDataBuffer)
	public:
		static BinaryDataBuffer  Create(ConstPara<Numeric> Size_, ConstPara<ByteOrderValue> ByteOrder_ = DefUndef<esb::ByteOrderValue>) { ESB_INVOKE_CTOR2(BinaryDataBuffer, Size_, ByteOrder_); }
	public:
		void  SetReadOnly()			{ ESB_INVOKE_MEMB_PROC0(MethId::SetReadOnly); }		// Так и в хелпе - свойство ReadOnly только на чтение, а SetReadOnly - это метод
		BinaryDataBuffer  GetSlice(ConstPara<Numeric> Position_, ConstPara<Numeric> Number_ = DefUndef<Numeric>) const { ESB_INVOKE_MEMB_FUNC2(BinaryDataBuffer, MethId::GetSlice, Position_, Number_); }
		Numeric  Get(ConstPara<Numeric> Position_) const { ESB_INVOKE_MEMB_FUNC1(Numeric, MethId::Get, Position_); }
		void  Set(ConstPara<Numeric> Position_, ConstPara<Numeric> Value_)	{ ESB_INVOKE_MEMB_PROC2(MethId::Set, Position_, Value_); }
		BinaryDataBuffer  Read(ConstPara<Numeric> Position_, ConstPara<Numeric> Number_) const	{ ESB_INVOKE_MEMB_FUNC2(BinaryDataBuffer, MethId::Read, Position_, Number_); }
		Numeric  ReadInt16(ConstPara<Numeric> Position_, ConstPara<ByteOrderValue> ByteOrder_ = DefUndef<esb::ByteOrderValue>) const { ESB_INVOKE_MEMB_FUNC2(Numeric, MethId::ReadInt16, Position_, ByteOrder_); }
		Numeric  ReadInt32(ConstPara<Numeric> Position_, ConstPara<ByteOrderValue> ByteOrder_ = DefUndef<esb::ByteOrderValue>) const { ESB_INVOKE_MEMB_FUNC2(Numeric, MethId::ReadInt32, Position_, ByteOrder_); }
		Numeric  ReadInt64(ConstPara<Numeric> Position_, ConstPara<ByteOrderValue> ByteOrder_ = DefUndef<esb::ByteOrderValue>) const { ESB_INVOKE_MEMB_FUNC2(Numeric, MethId::ReadInt64, Position_, ByteOrder_); }
		void  WriteInt16(ConstPara<Numeric> Position_, ConstPara<Numeric> Value_, ConstPara<ByteOrderValue> ByteOrder_ = DefUndef<esb::ByteOrderValue>) { ESB_INVOKE_MEMB_PROC3(MethId::WriteInt16, Position_, Value_, ByteOrder_); }
		void  WriteInt32(ConstPara<Numeric> Position_, ConstPara<Numeric> Value_, ConstPara<ByteOrderValue> ByteOrder_ = DefUndef<esb::ByteOrderValue>) { ESB_INVOKE_MEMB_PROC3(MethId::WriteInt32, Position_, Value_, ByteOrder_); }
		void  WriteInt64(ConstPara<Numeric> Position_, ConstPara<Numeric> Value_, ConstPara<ByteOrderValue> ByteOrder_ = DefUndef<esb::ByteOrderValue>) { ESB_INVOKE_MEMB_PROC3(MethId::WriteInt64, Position_, Value_, ByteOrder_); }
		BinaryDataBuffer  Copy() const { ESB_INVOKE_MEMB_FUNC0(BinaryDataBuffer, MethId::Copy); }
		void  Write(ConstPara<Numeric> Position_, ConstPara<BinaryDataBuffer> Bytes_, ConstPara<Numeric> Number_ = DefUndef<Numeric>) { ESB_INVOKE_MEMB_PROC3(MethId::Write, Position_, Bytes_, Number_); }
		BinaryDataBuffer  Reverse() const { ESB_INVOKE_MEMB_FUNC0(BinaryDataBuffer, MethId::Reverse); }
		BinaryDataBuffer  Concat(ConstPara<BinaryDataBuffer> Buffer_) const { ESB_INVOKE_MEMB_FUNC1(BinaryDataBuffer, MethId::Concat, Buffer_); }
		ArrayOfValue  Split(ConstPara<BinaryDataBuffer> Separator_) const { ESB_INVOKE_MEMB_FUNC1(ArrayOfValue, MethId::Split, Separator_); }
		//-METH: -para-size-mismatch: Split
		ArrayOfValue  Split(ConstPara<ArrayOfValue> Separators_) const { ESB_INVOKE_MEMB_FUNC1(ArrayOfValue, MethId::Split, Separators_); }
		//-METH Extra!:WriteBitwiseAnd
		//-METH Extra!:WriteBitwiseOr
		//-METH Extra!:WriteBitwiseXor
		//-METH Extra!:WriteBitwiseAndNot
		//-METH Extra!:Invert
	public:
		Numeric GetSize() const { ESB_INVOKE_PROPERTY_GET(Numeric, PropId::Size); }
		ESB_PROPERTY_FIELD_RO(Numeric, Size);
		Boolean GetReadOnly() const { ESB_INVOKE_PROPERTY_GET(Boolean, PropId::ReadOnly); }	// Так и в хелпе - свойство ReadOnly только на чтение, а SetReadOnly - это метод
		ESB_PROPERTY_FIELD_RO(Boolean, ReadOnly);
		ByteOrderValue GetByteOrder() const { ESB_INVOKE_PROPERTY_GET(esb::ByteOrderValue, PropId::ByteOrder); }
		void SetByteOrder(ConstPara<ByteOrderValue> Value_) { ESB_INVOKE_PROPERTY_SET(PropId::ByteOrder, Value_); }
		ESB_PROPERTY_FIELD_RW(esb::ByteOrderValue, ByteOrder);
	public:
		ESB_IMPLEMENT_ENUM_ITEMS(BinaryDataBuffer, Numeric)	//Byte
	public:
		ESB_IMPLEMENT_INDEXED_COLLECTION_RW()
	public:
		// esbhlp, чуть прямее (кривее), но в виде байтов, а не Numeric
		size_t	GetByteBufferSize() const;
		uint8_t GetByteBufferByte(size_t at_) const;
		void	SetByteBufferByte(size_t at_, uint8_t value_);
	};



//TOBE: И FileStream и MemoryStream унаследованы от Stream. Стоит подумать как сделать наследование

	class FileStream : public Object {
		friend FileStream make<FileStream>(IValuePtr&&);
		FileStream(IValuePtr&& val_, IObjectPtr&& obj_) throw() : Object(std::move(val_), std::move(obj_)) {}
		struct MethId { ESB_ID(Close, 0); ESB_ID(Size, 1); ESB_ID(GetReadOnlyStream, 2); ESB_ID(CurrentPosition, 3); ESB_ID(CopyTo, 4); ESB_ID(Flush, 5); ESB_ID(Seek, 6); ESB_ID(SetSize, 7); ESB_ID(Read, 8); ESB_ID(Write, 9); ESB_ID(BeginGetSize, 10); ESB_ID(BeginCopyTo, 11); ESB_ID(BeginSeek, 12); ESB_ID(BeginSetSize, 13); ESB_ID(BeginRead, 14); ESB_ID(BeginWrite, 15); ESB_ID(BeginFlush, 16); ESB_ID(BeginClose, 17); };
		struct PropId { ESB_ID(CanRead, 0); ESB_ID(CanWrite, 1); ESB_ID(CanSeek, 2); ESB_ID(FileName, 3); };
	public:
		ESB_DECLARE_TYPEINFO()
	public:
		static FileStream  Create(ConstPara<String> FileName_, ConstPara<FileOpenModeValue> OpeningMode_, ConstPara<Numeric> BufferSize_ = DefUndef<Numeric>) { ESB_INVOKE_CTOR3(FileStream, FileName_, OpeningMode_, BufferSize_); }
		static FileStream  Create(ConstPara<String> FileName_, ConstPara<FileOpenModeValue> OpeningMode_, ConstPara<FileAccessValue> AccessLevel_, ConstPara<Numeric> BufferSize_ = DefUndef<Numeric>) { ESB_INVOKE_CTOR4(FileStream, FileName_, OpeningMode_, AccessLevel_, BufferSize_); }
	public:
		void  Close() { ESB_INVOKE_MEMB_PROC0(MethId::Close); }
		Numeric  Size() const { ESB_INVOKE_MEMB_FUNC0(Numeric, MethId::Size); }
		Stream  GetReadOnlyStream() const { ESB_INVOKE_MEMB_FUNC0(Stream, MethId::GetReadOnlyStream); }
		Numeric  CurrentPosition() const { ESB_INVOKE_MEMB_FUNC0(Numeric, MethId::CurrentPosition); }
		void  CopyTo(ConstPara<Stream> TargetStream_, ConstPara<Numeric> Count_ = DefUndef<Numeric>) const { ESB_INVOKE_MEMB_PROC2(MethId::CopyTo, TargetStream_, Count_); }
		void  Flush() const { ESB_INVOKE_MEMB_PROC0(MethId::Flush); }
		Numeric  Seek(ConstPara<Numeric> Offset_, ConstPara<PositionInStreamValue> InitialPosition_ = DefUndef<PositionInStreamValue>) { ESB_INVOKE_MEMB_FUNC2(Numeric, MethId::Seek, Offset_, InitialPosition_); }
		void  SetSize(ConstPara<Numeric> Size_) { ESB_INVOKE_MEMB_PROC1(MethId::SetSize, Size_); }
		Numeric  Read(ConstPara<BinaryDataBuffer> Buffer_, ConstPara<Numeric> PositionInBuffer_, ConstPara<Numeric> Kolichestvo_) { ESB_INVOKE_MEMB_FUNC3(Numeric, MethId::Read, Buffer_, PositionInBuffer_, Kolichestvo_); }
		void  Write(ConstPara<BinaryDataBuffer> Buffer_, ConstPara<Numeric> PositionInBuffer_, ConstPara<Numeric> Number_) { ESB_INVOKE_MEMB_PROC3(MethId::Write, Buffer_, PositionInBuffer_, Number_); }
		void  BeginGetSize(ConstPara<NotifyDescription> NotifyDescription_) const { ESB_INVOKE_MEMB_PROC1(MethId::BeginGetSize, NotifyDescription_); }
		void  BeginCopyTo(ConstPara<NotifyDescription> NotifyDescription_, ConstPara<Stream> TargetStream_, ConstPara<Numeric> Count_ = DefUndef<Numeric>) const { ESB_INVOKE_MEMB_PROC3(MethId::BeginCopyTo, NotifyDescription_, TargetStream_, Count_); }
		void  BeginSeek(ConstPara<NotifyDescription> NotifyDescription_, ConstPara<Numeric> Offset_, ConstPara<PositionInStreamValue> InitialPosition_ = DefUndef<PositionInStreamValue>) { ESB_INVOKE_MEMB_PROC3(MethId::BeginSeek, NotifyDescription_, Offset_, InitialPosition_); }
		void  BeginSetSize(ConstPara<NotifyDescription> NotifyDescription_, ConstPara<Numeric> Size_) { ESB_INVOKE_MEMB_PROC2(MethId::BeginSetSize, NotifyDescription_, Size_); }
		void  BeginRead(ConstPara<NotifyDescription> NotifyDescription_, ConstPara<BinaryDataBuffer> Buffer_, ConstPara<Numeric> PositionInBuffer_, ConstPara<Numeric> Number_) { ESB_INVOKE_MEMB_PROC4(MethId::BeginRead, NotifyDescription_, Buffer_, PositionInBuffer_, Number_); }
		void  BeginWrite(ConstPara<NotifyDescription> NotifyDescription_, ConstPara<BinaryDataBuffer> Buffer_, ConstPara<Numeric> PositionInBuffer_, ConstPara<Numeric> Count_) { ESB_INVOKE_MEMB_PROC4(MethId::BeginWrite, NotifyDescription_, Buffer_, PositionInBuffer_, Count_); }
		void  BeginFlush(ConstPara<NotifyDescription> NotifyDescription_) const { ESB_INVOKE_MEMB_PROC1(MethId::BeginFlush, NotifyDescription_); }
		void  BeginClose(ConstPara<NotifyDescription> NotifyDescription_) { ESB_INVOKE_MEMB_PROC1(MethId::BeginClose, NotifyDescription_); }
	public:
		Boolean GetCanRead() const { ESB_INVOKE_PROPERTY_GET(Boolean, PropId::CanRead); }
		ESB_PROPERTY_FIELD_RO(Boolean, CanRead);
		Boolean GetCanWrite() const { ESB_INVOKE_PROPERTY_GET(Boolean, PropId::CanWrite); }
		ESB_PROPERTY_FIELD_RO(Boolean, CanWrite);
		Boolean GetCanSeek() const { ESB_INVOKE_PROPERTY_GET(Boolean, PropId::CanSeek); }
		ESB_PROPERTY_FIELD_RO(Boolean, CanSeek);
		String GetFileName() const { ESB_INVOKE_PROPERTY_GET(String, PropId::FileName); }
		ESB_PROPERTY_FIELD_RO(String, FileName);
	};



	class MemoryStream : public Object {
		friend MemoryStream make<MemoryStream>(IValuePtr&&);
		MemoryStream(IValuePtr&& val_, IObjectPtr&& obj_) throw() : Object(std::move(val_), std::move(obj_)) {}
		struct MethId { ESB_ID(Close, 0); ESB_ID(Size, 1); ESB_ID(GetReadOnlyStream, 2); ESB_ID(CurrentPosition, 3); ESB_ID(CopyTo, 4); ESB_ID(Flush, 5); ESB_ID(Seek, 6); ESB_ID(SetSize, 7); ESB_ID(Read, 8); ESB_ID(Write, 9); ESB_ID(BeginGetSize, 10); ESB_ID(BeginCopyTo, 11); ESB_ID(BeginSeek, 12); ESB_ID(BeginSetSize, 13); ESB_ID(BeginRead, 14); ESB_ID(BeginWrite, 15); ESB_ID(BeginFlush, 16); ESB_ID(BeginClose, 17); ESB_ID(CloseAndGetBinaryData, 18); };
		struct PropId { ESB_ID(CanRead, 0); ESB_ID(CanWrite, 1); ESB_ID(CanSeek, 2); };
	public:
		ESB_DECLARE_TYPEINFO()
	public:
		static MemoryStream  Create() { ESB_INVOKE_CTOR0(MemoryStream); }
		static MemoryStream  Create(ConstPara<Numeric> InitialCapacity_) { ESB_INVOKE_CTOR1(MemoryStream, InitialCapacity_); }
		static MemoryStream  Create(ConstPara<BinaryDataBuffer> Buffer_) { ESB_INVOKE_CTOR1(MemoryStream, Buffer_); }
	public:
		void  Close() { ESB_INVOKE_MEMB_PROC0(MethId::Close); }
		Numeric  Size() const { ESB_INVOKE_MEMB_FUNC0(Numeric, MethId::Size); }
		Stream  GetReadOnlyStream() const { ESB_INVOKE_MEMB_FUNC0(Stream, MethId::GetReadOnlyStream); }
		Numeric  CurrentPosition() const { ESB_INVOKE_MEMB_FUNC0(Numeric, MethId::CurrentPosition); }
		void  CopyTo(ConstPara<Stream> TargetStream_, ConstPara<Numeric> Count_ = DefUndef<Numeric>) const { ESB_INVOKE_MEMB_PROC2(MethId::CopyTo, TargetStream_, Count_); }
		void  Flush() const { ESB_INVOKE_MEMB_PROC0(MethId::Flush); }
		Numeric  Seek(ConstPara<Numeric> Offset_, ConstPara<PositionInStreamValue> InitialPosition_ = DefUndef<PositionInStreamValue>) { ESB_INVOKE_MEMB_FUNC2(Numeric, MethId::Seek, Offset_, InitialPosition_); }
		void  SetSize(ConstPara<Numeric> Size_) { ESB_INVOKE_MEMB_PROC1(MethId::SetSize, Size_); }
		Numeric  Read(ConstPara<BinaryDataBuffer> Buffer_, ConstPara<Numeric> PositionInBuffer_, ConstPara<Numeric> Kolichestvo_) { ESB_INVOKE_MEMB_FUNC3(Numeric, MethId::Read, Buffer_, PositionInBuffer_, Kolichestvo_); }
		void  Write(ConstPara<BinaryDataBuffer> Buffer_, ConstPara<Numeric> PositionInBuffer_, ConstPara<Numeric> Number_) { ESB_INVOKE_MEMB_PROC3(MethId::Write, Buffer_, PositionInBuffer_, Number_); }
		void  BeginGetSize(ConstPara<NotifyDescription> NotifyDescription_) { ESB_INVOKE_MEMB_PROC1(MethId::BeginGetSize, NotifyDescription_); }
		void  BeginCopyTo(ConstPara<NotifyDescription> NotifyDescription_, ConstPara<Stream> TargetStream_, ConstPara<Numeric> Count_ = DefUndef<Numeric>) { ESB_INVOKE_MEMB_PROC3(MethId::BeginCopyTo, NotifyDescription_, TargetStream_, Count_); }
		void  BeginSeek(ConstPara<NotifyDescription> NotifyDescription_, ConstPara<Numeric> Offset_, ConstPara<PositionInStreamValue> InitialPosition_ = DefUndef<PositionInStreamValue>) { ESB_INVOKE_MEMB_PROC3(MethId::BeginSeek, NotifyDescription_, Offset_, InitialPosition_); }
		void  BeginSetSize(ConstPara<NotifyDescription> NotifyDescription_, ConstPara<Numeric> Size_) { ESB_INVOKE_MEMB_PROC2(MethId::BeginSetSize, NotifyDescription_, Size_); }
		void  BeginRead(ConstPara<NotifyDescription> NotifyDescription_, ConstPara<BinaryDataBuffer> Buffer_, ConstPara<Numeric> PositionInBuffer_, ConstPara<Numeric> Number_) { ESB_INVOKE_MEMB_PROC4(MethId::BeginRead, NotifyDescription_, Buffer_, PositionInBuffer_, Number_); }
		void  BeginWrite(ConstPara<NotifyDescription> NotifyDescription_, ConstPara<BinaryDataBuffer> Buffer_, ConstPara<Numeric> PositionInBuffer_, ConstPara<Numeric> Count_) { ESB_INVOKE_MEMB_PROC4(MethId::BeginWrite, NotifyDescription_, Buffer_, PositionInBuffer_, Count_); }
		void  BeginFlush(ConstPara<NotifyDescription> NotifyDescription_) { ESB_INVOKE_MEMB_PROC1(MethId::BeginFlush, NotifyDescription_); }
		void  BeginClose(ConstPara<NotifyDescription> NotifyDescription_) { ESB_INVOKE_MEMB_PROC1(MethId::BeginClose, NotifyDescription_); }
		BinaryData  CloseAndGetBinaryData() { ESB_INVOKE_MEMB_FUNC0(BinaryData, MethId::CloseAndGetBinaryData); }
	public:
		Boolean GetCanRead() const { ESB_INVOKE_PROPERTY_GET(Boolean, PropId::CanRead); }
		ESB_PROPERTY_FIELD_RO(Boolean, CanRead);
		Boolean GetCanWrite() const { ESB_INVOKE_PROPERTY_GET(Boolean, PropId::CanWrite); }
		ESB_PROPERTY_FIELD_RO(Boolean, CanWrite);
		Boolean GetCanSeek() const { ESB_INVOKE_PROPERTY_GET(Boolean, PropId::CanSeek); }
		ESB_PROPERTY_FIELD_RO(Boolean, CanSeek);
	};


// определение АПИ зависящего от типов esb_file
//
	// Функция Base64Значение(Строка_ Как Строка) Возвращает ДвоичныеДанные
	inline BinaryData Base64Value(ConstPara<String> Line_) { ESB_INVOKE_API_FUNC1(BinaryData, core, 1, Line_); }
/////////////////////////////////////////////////

}	//namespace esb 
#endif //ESBFILE_H
