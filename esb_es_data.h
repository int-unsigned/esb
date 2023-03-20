#pragma once
#ifndef ESBDATA_H
#define ESBDATA_H

#include "esb.h"



namespace esb{
//
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
//



class ValueTableColumn : public Object {
	friend ValueTableColumn make<ValueTableColumn>(IValuePtr&&);
	ValueTableColumn(IValuePtr&& val_, IObjectPtr&& obj_) throw() : Object(std::move(val_), std::move(obj_)) {}
	struct PropId { ESB_ID(Name, 0); ESB_ID(ValueType, 1); ESB_ID(Title, 2); ESB_ID(Width, 3); };
public:
	ESB_DECLARE_TYPEINFO()
	ESB_IMPLEMENT_ARGBYVAL_OPERATOR(ValueTableColumn)
public:
	// Not Creatable
public:
	String GetName() const { ESB_INVOKE_PROPERTY_GET(String, PropId::Name); }
	void SetName(ConstPara<String> Value_) { ESB_INVOKE_PROPERTY_SET(PropId::Name, Value_); }
	ESB_PROPERTY_FIELD_RW(String, Name);
	TypeDescription GetValueType() const { ESB_INVOKE_PROPERTY_GET(TypeDescription, PropId::ValueType); }
	ESB_PROPERTY_FIELD_RO(TypeDescription, ValueType);
	String GetTitle() const { ESB_INVOKE_PROPERTY_GET(String, PropId::Title); }
	void SetTitle(ConstPara<String> Value_) { ESB_INVOKE_PROPERTY_SET(PropId::Title, Value_); }
	ESB_PROPERTY_FIELD_RW(String, Title);
	Numeric GetWidth() const { ESB_INVOKE_PROPERTY_GET(Numeric, PropId::Width); }
	void SetWidth(ConstPara<Numeric> Value_) { ESB_INVOKE_PROPERTY_SET(PropId::Width, Value_); }
	ESB_PROPERTY_FIELD_RW(Numeric, Width);
};


class ValueTableColumnCollection : public ObjectDual<IxCollectionRO<ValueTableColumn>>, protected DynamicCollectionImplRO<ValueTableColumnCollection, ValueTableColumn> {
	ESB_FRIEND_DYNAMIC_COLLECTION()
	friend ValueTableColumnCollection make<ValueTableColumnCollection>(IValuePtr&&);
	ValueTableColumnCollection(IValuePtr&& val_, IObjectPtr&& obj_, IIxCollectionROPtr&& ixc_) throw() : ObjectDual(std::move(val_), std::move(obj_), std::move(ixc_)) {}
	struct MethId { ESB_ID(Count, 0); ESB_ID(IndexOf, 1); ESB_ID(Insert, 2); ESB_ID(Add, 3); ESB_ID(Delete, 4); ESB_ID(Move, 5); ESB_ID(Clear, 6); ESB_ID(Find, 7); ESB_ID(Get, 8); };
public:
	ESB_DECLARE_TYPEINFO()
public:
	// Not Creatable
public:
	Numeric  Count() const { ESB_INVOKE_MEMB_FUNC0(Numeric, MethId::Count); }
	Numeric  IndexOf(ConstPara<ValueTableColumn> Column_) const { ESB_INVOKE_MEMB_FUNC1(Numeric, MethId::IndexOf, Column_); }
	ValueTableColumn  Insert(ConstPara<Numeric> Index_, ConstPara<String> Name_ = DefUndef<String>, ConstPara<TypeDescription> type_ = DefUndef<TypeDescription>, ConstPara<String> Title_ = DefUndef<String>, ConstPara<Numeric> Width_ = DefUndef<Numeric>) { ESB_INVOKE_MEMB_FUNC5(ValueTableColumn, MethId::Insert, Index_, Name_, type_, Title_, Width_); }
	ValueTableColumn  Add(ConstPara<String> Name_, ConstPara<TypeDescription> type_, ConstPara<String> Title_, ConstPara<Numeric> Width_ = DefUndef<Numeric>) { ESB_INVOKE_MEMB_FUNC4(ValueTableColumn, MethId::Add, Name_, type_, Title_, Width_); }
	void  Delete(ConstPara<Numeric> Index_) { ESB_INVOKE_MEMB_PROC1(MethId::Delete, Index_); }
	void  Delete(ConstPara<ValueTableColumn> Index_) { ESB_INVOKE_MEMB_PROC1(MethId::Delete, Index_); }
	void  Delete(ConstPara<String> Index_) { ESB_INVOKE_MEMB_PROC1(MethId::Delete, Index_); }
	void  Move(ConstPara<Numeric> Column_, ConstPara<Numeric> Offset_) { ESB_INVOKE_MEMB_PROC2(MethId::Move, Column_, Offset_); }
	void  Move(ConstPara<ValueTableColumn> Column_, ConstPara<Numeric> Offset_) { ESB_INVOKE_MEMB_PROC2(MethId::Move, Column_, Offset_); }
	void  Clear() { ESB_INVOKE_MEMB_PROC0(MethId::Clear); }
	UndefOr<ValueTableColumn>  Find(ConstPara<String> ColumnDescription_) { ESB_INVOKE_MEMB_FUNC1(UndefOr<ValueTableColumn>, MethId::Find, ColumnDescription_); }
	ValueTableColumn  Get(ConstPara<Numeric> Index_) { ESB_INVOKE_MEMB_FUNC1(ValueTableColumn, MethId::Get, Index_); }
public:
	ESB_IMPLEMENT_ENUM_ITEMS(ValueTableColumnCollection, ValueTableColumn)
public:
	ESB_IMPLEMENT_INDEXED_COLLECTION_RO()
public:
	ESB_IMPLEMENT_DYNAMIC_COLLECTION_RO_NAME(ColumnName)
};


class ValueTableRow : public ObjectDual<IxCollectionRW<Arbitrary>>, protected DynamicCollectionImplRW<ValueTableRow, Arbitrary> {
	ESB_FRIEND_DYNAMIC_COLLECTION()
	friend ValueTableRow make<ValueTableRow>(IValuePtr&&);
	ValueTableRow(IValuePtr&& val_, IObjectPtr&& obj_, IIxCollectionRWPtr&& ixc_) throw() : ObjectDual(std::move(val_), std::move(obj_), std::move(ixc_)) {}
	struct MethId { ESB_ID(Get, 0); ESB_ID(Set, 1); ESB_ID(Owner, 2); };
public:
	ESB_DECLARE_TYPEINFO()
public:
	// Not Creatable
public:
	Arbitrary  Get(ConstPara<Numeric> Index_) const { ESB_INVOKE_MEMB_FUNC1(Arbitrary, MethId::Get, Index_); }
	void  Set(ConstPara<Numeric> Index_, ConstPara<Arbitrary> Value_) { ESB_INVOKE_MEMB_PROC2(MethId::Set, Index_, Value_); }
	ValueTable  Owner() const;		// { ESB_INVOKE_MEMB_FUNC0(ValueTable, MethId::Owner); }
public:
	ESB_IMPLEMENT_ENUM_ITEMS(ValueTableRow, Arbitrary)
public:
	ESB_IMPLEMENT_INDEXED_COLLECTION_RW()
public:
	ESB_IMPLEMENT_DYNAMIC_COLLECTION_RW_NAME(ColumnName)
};


class CollectionIndex : public ValueDual<IxCollectionRO<String>> {
	friend CollectionIndex make<CollectionIndex>(IValuePtr&&);
	CollectionIndex(IValuePtr&& val_, IIxCollectionROPtr&& ixc_) throw() : ValueDual(std::move(val_), std::move(ixc_)) {}
public:
	ESB_DECLARE_TYPEINFO()
public:
	// Not Creatable
public:
	ESB_IMPLEMENT_ENUM_ITEMS(CollectionIndex, String)
public:
	ESB_IMPLEMENT_INDEXED_COLLECTION_RO()
};


class CollectionIndexes : public ObjectDual<IxCollectionRO<CollectionIndex>> {
	friend CollectionIndexes make<CollectionIndexes>(IValuePtr&&);
	CollectionIndexes(IValuePtr&& val_, IObjectPtr&& obj_, IIxCollectionROPtr&& ixc_) throw() : ObjectDual(std::move(val_), std::move(obj_), std::move(ixc_)) {}
	struct MethId { ESB_ID(Count, 0); ESB_ID(Add, 1); ESB_ID(Delete, 2); ESB_ID(Clear, 3); };
public:
	ESB_DECLARE_TYPEINFO()
public:
	// Not Creatable
public:
	Numeric  Count() const { ESB_INVOKE_MEMB_FUNC0(Numeric, MethId::Count); }
	CollectionIndex  Add(ConstPara<String> Columns_) { ESB_INVOKE_MEMB_FUNC1(CollectionIndex, MethId::Add, Columns_); }
	void  Delete(ConstPara<CollectionIndex> Index_) { ESB_INVOKE_MEMB_PROC1(MethId::Delete, Index_); }
	void  Delete(ConstPara<Numeric> Index_) { ESB_INVOKE_MEMB_PROC1(MethId::Delete, Index_); }
	void  Clear() { ESB_INVOKE_MEMB_PROC0(MethId::Clear); }
public:
	ESB_IMPLEMENT_ENUM_ITEMS(CollectionIndexes, CollectionIndex)
public:
	ESB_IMPLEMENT_INDEXED_COLLECTION_RO()
};


class ValueTable : public ObjectDual<IxCollectionRO<ValueTableRow>> {
	friend ValueTable make<ValueTable>(IValuePtr&&);
	ValueTable(IValuePtr&& val_, IObjectPtr&& obj_, IIxCollectionROPtr&& ixc_) throw() : ObjectDual(std::move(val_), std::move(obj_), std::move(ixc_)) {}
	struct MethId { ESB_ID(Count, 0); ESB_ID(IndexOf, 1); ESB_ID(Add, 2); ESB_ID(Insert, 3); ESB_ID(Delete, 4); ESB_ID(Clear, 5); ESB_ID(Move, 6); ESB_ID(Find, 7); ESB_ID(FindRows, 8); ESB_ID(Total, 9); ESB_ID(Sort, 10); ESB_ID(LoadColumn, 11); ESB_ID(UnloadColumn, 12); ESB_ID(Get, 13); ESB_ID(Copy, 14); ESB_ID(CopyColumns, 15); ESB_ID(GroupBy, 16); ESB_ID(FillValues, 17); ESB_ID(ChooseRow, 18); };
	struct PropId { ESB_ID(Columns, 0); ESB_ID(Indexes, 1); };
public:
	ESB_DECLARE_TYPEINFO()
public:
	static ValueTable Create() { ESB_INVOKE_CTOR0(ValueTable); }
public:
	Numeric  Count() const { ESB_INVOKE_MEMB_FUNC0(Numeric, MethId::Count); }
	Numeric  IndexOf(ConstPara<ValueTableRow> Row_) const { ESB_INVOKE_MEMB_FUNC1(Numeric, MethId::IndexOf, Row_); }
	ValueTableRow  Add() { ESB_INVOKE_MEMB_FUNC0(ValueTableRow, MethId::Add); }
	ValueTableRow  Insert(ConstPara<Numeric> Index_) { ESB_INVOKE_MEMB_FUNC1(ValueTableRow, MethId::Insert, Index_); }
	void  Delete(ConstPara<ValueTableRow> Row_) { ESB_INVOKE_MEMB_PROC1(MethId::Delete, Row_); }
	void  Delete(ConstPara<Numeric> Index_) { ESB_INVOKE_MEMB_PROC1(MethId::Delete, Index_); }
	void  Clear() { ESB_INVOKE_MEMB_PROC0(MethId::Clear); }
	void  Move(ConstPara<Numeric> Row_, ConstPara<Numeric> Offset_) { ESB_INVOKE_MEMB_PROC2(MethId::Move, Row_, Offset_); }
	void  Move(ConstPara<ValueTableRow> Row_, ConstPara<Numeric> Offset_) { ESB_INVOKE_MEMB_PROC2(MethId::Move, Row_, Offset_); }
	UndefOr<ValueTableRow>  Find(ConstPara<Arbitrary> Value_, ConstPara<String> Columns_ = DefUndef<String>) const { ESB_INVOKE_MEMB_FUNC2(UndefOr<ValueTableRow>, MethId::Find, Value_, Columns_); }
	ArrayOfValue  FindRows(ConstPara<Structure> FilterParameters_) const { ESB_INVOKE_MEMB_FUNC1(ArrayOfValue, MethId::FindRows, FilterParameters_); }
	UndefOr<Numeric>  Total(ConstPara<String> Column_) const { ESB_INVOKE_MEMB_FUNC1(UndefOr<Numeric>, MethId::Total, Column_); }
	void  Sort(ConstPara<String> Columns_, ConstPara<CompareValues> ObjectOfComparison_ = DefUndef<CompareValues>) { ESB_INVOKE_MEMB_PROC2(MethId::Sort, Columns_, ObjectOfComparison_); }
	void  LoadColumn(ConstPara<ArrayOfValue> Array_, ConstPara<Numeric> Column_) { ESB_INVOKE_MEMB_PROC2(MethId::LoadColumn, Array_, Column_); }
	void  LoadColumn(ConstPara<ArrayOfValue> Array_, ConstPara<String> Column_) { ESB_INVOKE_MEMB_PROC2(MethId::LoadColumn, Array_, Column_); }
	void  LoadColumn(ConstPara<ArrayOfValue> Array_, ConstPara<ValueTableColumn> Column_) { ESB_INVOKE_MEMB_PROC2(MethId::LoadColumn, Array_, Column_); }
	ArrayOfValue  UnloadColumn(ConstPara<Numeric> Column_) const { ESB_INVOKE_MEMB_FUNC1(ArrayOfValue, MethId::UnloadColumn, Column_); }
	ArrayOfValue  UnloadColumn(ConstPara<String> Column_) const { ESB_INVOKE_MEMB_FUNC1(ArrayOfValue, MethId::UnloadColumn, Column_); }
	ArrayOfValue  UnloadColumn(ConstPara<ValueTableColumn> Column_) const { ESB_INVOKE_MEMB_FUNC1(ArrayOfValue, MethId::UnloadColumn, Column_); }
	ValueTableRow  Get(ConstPara<Numeric> Index_) const { ESB_INVOKE_MEMB_FUNC1(ValueTableRow, MethId::Get, Index_); }
	ValueTable  Copy(ConstPara<ArrayOfValue> Rows_, ConstPara<String> Columns_ = DefUndef<String>) const { ESB_INVOKE_MEMB_FUNC2(ValueTable, MethId::Copy, Rows_, Columns_); }
	ValueTable  Copy(ConstPara<Structure> ParametersOfFilter_, ConstPara<String> Columns_ = DefUndef<String>) const { ESB_INVOKE_MEMB_FUNC2(ValueTable, MethId::Copy, ParametersOfFilter_, Columns_); }
	ValueTable  CopyColumns(ConstPara<String> Columns_) const { ESB_INVOKE_MEMB_FUNC1(ValueTable, MethId::CopyColumns, Columns_); }
	void  GroupBy(ConstPara<String> GroupingColumns_, ConstPara<String> TotalingColumns_ = DefUndef<String>) { ESB_INVOKE_MEMB_PROC2(MethId::GroupBy, GroupingColumns_, TotalingColumns_); }
	void  FillValues(ConstPara<Arbitrary> Value_, ConstPara<String> Columns_ = DefUndef<String>) { ESB_INVOKE_MEMB_PROC2(MethId::FillValues, Value_, Columns_); }
	UndefOr<ValueTableRow>  ChooseRow(ConstPara<String> Title_, ConstPara<ValueTableRow> InitialRow_ = DefUndef<ValueTableRow>) const { ESB_INVOKE_MEMB_FUNC2(UndefOr<ValueTableRow>, MethId::ChooseRow, Title_, InitialRow_); }
public:
	ValueTableColumnCollection GetColumns() const { ESB_INVOKE_PROPERTY_GET(ValueTableColumnCollection, PropId::Columns); }
	ESB_PROPERTY_FIELD_RO(ValueTableColumnCollection, Columns);
	CollectionIndexes GetIndexes() const { ESB_INVOKE_PROPERTY_GET(CollectionIndexes, PropId::Indexes); }
	ESB_PROPERTY_FIELD_RO(CollectionIndexes, Indexes);
public:
	ESB_IMPLEMENT_ENUM_ITEMS(ValueTable, ValueTableRow)
public:
	ESB_IMPLEMENT_INDEXED_COLLECTION_RO()
};

inline
ValueTable  ValueTableRow::Owner() const { ESB_INVOKE_MEMB_FUNC0(ValueTable, MethId::Owner); }



class ValueTreeColumn : public Object {
	friend ValueTreeColumn make<ValueTreeColumn>(IValuePtr&&);
	ValueTreeColumn(IValuePtr&& val_, IObjectPtr&& obj_) throw() : Object(std::move(val_), std::move(obj_)) {}
	struct PropId { ESB_ID(Name, 0); ESB_ID(ValueType, 1); ESB_ID(Title, 2); ESB_ID(Width, 3); };
public:
	ESB_DECLARE_TYPEINFO()
public:
	// Not Creatable
public:
	String	GetName() const							{ ESB_INVOKE_PROPERTY_GET(String, PropId::Name); }
	void	SetName(ConstPara<String> Value_)		{ ESB_INVOKE_PROPERTY_SET(PropId::Name, Value_); }
	ESB_PROPERTY_FIELD_RW(String, Name);
	TypeDescription GetValueType()	const			{ ESB_INVOKE_PROPERTY_GET(TypeDescription, PropId::ValueType); }
	ESB_PROPERTY_FIELD_RO(TypeDescription, ValueType);
	String GetTitle() const							{ ESB_INVOKE_PROPERTY_GET(String, PropId::Title); }
	void SetTitle(ConstPara<String> Value_)		{ ESB_INVOKE_PROPERTY_SET(PropId::Title, Value_); }
	ESB_PROPERTY_FIELD_RW(String, Title);
	Numeric GetWidth() const							{ ESB_INVOKE_PROPERTY_GET(Numeric, PropId::Width); }
	void SetWidth(ConstPara<Numeric> Value_)		{ ESB_INVOKE_PROPERTY_SET(PropId::Width, Value_); }
	ESB_PROPERTY_FIELD_RW(Numeric, Width);
};


class ValueTreeColumnCollection : public ObjectDual<IxCollectionRO<ValueTreeColumn>>, protected DynamicCollectionImplRO<ValueTreeColumnCollection, ValueTreeColumn> {
	ESB_FRIEND_DYNAMIC_COLLECTION()
	friend ValueTreeColumnCollection make<ValueTreeColumnCollection>(IValuePtr&&);
	ValueTreeColumnCollection(IValuePtr&& val_, IObjectPtr&& obj_, IIxCollectionROPtr&& ixc_) throw() : ObjectDual(std::move(val_), std::move(obj_), std::move(ixc_)) {}
	struct MethId { ESB_ID(Count, 0); ESB_ID(IndexOf, 1); ESB_ID(Insert, 2); ESB_ID(Add, 3); ESB_ID(Delete, 4); ESB_ID(Move, 5); ESB_ID(Clear, 6); ESB_ID(Find, 7); ESB_ID(Get, 8); };
public:
	ESB_DECLARE_TYPEINFO()
public:
	// Not Creatable
public:
	Numeric  Count() const { ESB_INVOKE_MEMB_FUNC0(Numeric, MethId::Count); }
	Numeric  IndexOf(ConstPara<ValueTreeColumn> Column_) const { ESB_INVOKE_MEMB_FUNC1(Numeric, MethId::IndexOf, Column_); }
	ValueTreeColumn  Insert(ConstPara<Numeric> Index_, ConstPara<String> Name_ = DefUndef<String>, ConstPara<TypeDescription> type_ = DefUndef<TypeDescription>, ConstPara<String> Title_ = DefUndef<String>, ConstPara<Numeric> Width_ = DefUndef<Numeric>) { ESB_INVOKE_MEMB_FUNC5(ValueTreeColumn, MethId::Insert, Index_, Name_, type_, Title_, Width_); }
	ValueTreeColumn  Add(ConstPara<String> Name_, ConstPara<TypeDescription> type_, ConstPara<String> Title_, ConstPara<Numeric> Width_ = DefUndef<Numeric>) { ESB_INVOKE_MEMB_FUNC4(ValueTreeColumn, MethId::Add, Name_, type_, Title_, Width_); }
	void  Delete(ConstPara<Numeric> Index_) { ESB_INVOKE_MEMB_PROC1(MethId::Delete, Index_); }
	void  Delete(ConstPara<ValueTreeColumn> Index_) { ESB_INVOKE_MEMB_PROC1(MethId::Delete, Index_); }
	void  Move(ConstPara<Numeric> Column_, ConstPara<Numeric> Offset_) { ESB_INVOKE_MEMB_PROC2(MethId::Move, Column_, Offset_); }
	void  Move(ConstPara<ValueTreeColumn> Column_, ConstPara<Numeric> Offset_) { ESB_INVOKE_MEMB_PROC2(MethId::Move, Column_, Offset_); }
	void  Clear() { ESB_INVOKE_MEMB_PROC0(MethId::Clear); }
	UndefOr<ValueTreeColumn>  Find(ConstPara<String> ColumnDescription_) const { ESB_INVOKE_MEMB_FUNC1(UndefOr<ValueTreeColumn>, MethId::Find, ColumnDescription_); }
	ValueTreeColumn  Get(ConstPara<Numeric> Index_) const { ESB_INVOKE_MEMB_FUNC1(ValueTreeColumn, MethId::Get, Index_); }
public:
	ESB_IMPLEMENT_ENUM_ITEMS(ValueTreeColumnCollection, ValueTreeColumn)
public:
	ESB_IMPLEMENT_INDEXED_COLLECTION_RO()
public:
	ESB_IMPLEMENT_DYNAMIC_COLLECTION_RO_NAME(ColumnName)
};


class ValueTreeRow : public ObjectDual<IxCollectionRW<Arbitrary>>, protected DynamicCollectionImplRW<ValueTreeRow, Arbitrary> {
	ESB_FRIEND_DYNAMIC_COLLECTION()
	friend ValueTreeRow make<ValueTreeRow>(IValuePtr&&);
	ValueTreeRow(IValuePtr&& val_, IObjectPtr&& obj_, IIxCollectionRWPtr&& ixc_) throw() : ObjectDual(std::move(val_), std::move(obj_), std::move(ixc_)) {}
	struct MethId { ESB_ID(Get, 0); ESB_ID(Set, 1); ESB_ID(Level, 2); ESB_ID(Owner, 3); };
	struct PropId { ESB_ID(Parent, 0); ESB_ID(Rows, 1); };
public:
	ESB_DECLARE_TYPEINFO()
public:
	// Not Creatable
public:
	Arbitrary  Get(ConstPara<Numeric> Index_) const { ESB_INVOKE_MEMB_FUNC1(Arbitrary, MethId::Get, Index_); }
	void  Set(ConstPara<Numeric> Index_, ConstPara<Arbitrary> Value_) { ESB_INVOKE_MEMB_PROC2(MethId::Set, Index_, Value_); }
	Numeric  Level() const { ESB_INVOKE_MEMB_FUNC0(Numeric, MethId::Level); }
	ValueTree  Owner() const; //{ ESB_INVOKE_MEMB_FUNC0(ValueTree, MethId::Owner); }
public:
	ValueTreeRow GetParent() const { ESB_INVOKE_PROPERTY_GET(ValueTreeRow, PropId::Parent); }
	ESB_PROPERTY_FIELD_RO(ValueTreeRow, Parent);
	ValueTreeRowCollection GetRows() const; //{ ESB_INVOKE_PROPERTY_GET(ValueTreeRowCollection, PropId::Rows); }
	ESB_PROPERTY_FIELD_RO(ValueTreeRowCollection, Rows);
public:
	ESB_IMPLEMENT_ENUM_ITEMS(ValueTreeRow, Arbitrary)
public:
	ESB_IMPLEMENT_INDEXED_COLLECTION_RW()
public:
	ESB_IMPLEMENT_DYNAMIC_COLLECTION_RW_NAME(ColumnName)
};


class ValueTreeRowCollection : public ObjectDual<IxCollectionRO<ValueTreeRow>> {
	friend ValueTreeRowCollection make<ValueTreeRowCollection>(IValuePtr&&);
	ValueTreeRowCollection(IValuePtr&& val_, IObjectPtr&& obj_, IIxCollectionROPtr&& ixc_) throw() : ObjectDual(std::move(val_), std::move(obj_), std::move(ixc_)) {}
	struct MethId { ESB_ID(Count, 0); ESB_ID(IndexOf, 1); ESB_ID(Add, 2); ESB_ID(Insert, 3); ESB_ID(Delete, 4); ESB_ID(Clear, 5); ESB_ID(Move, 6); ESB_ID(Find, 7); ESB_ID(FindRows, 8); ESB_ID(Total, 9); ESB_ID(Sort, 10); ESB_ID(LoadColumn, 11); ESB_ID(UnloadColumn, 12); ESB_ID(Get, 13); };
	struct PropId { ESB_ID(Parent, 0); };
public:
	ESB_DECLARE_TYPEINFO()
public:
	// Not Creatable
public:
	Numeric  Count() const { ESB_INVOKE_MEMB_FUNC0(Numeric, MethId::Count); }
	Numeric  IndexOf(ConstPara<ValueTreeRow> Row_) const { ESB_INVOKE_MEMB_FUNC1(Numeric, MethId::IndexOf, Row_); }
	ValueTreeRow  Add() { ESB_INVOKE_MEMB_FUNC0(ValueTreeRow, MethId::Add); }
	ValueTreeRow  Insert(ConstPara<Numeric> Index_) { ESB_INVOKE_MEMB_FUNC1(ValueTreeRow, MethId::Insert, Index_); }
	void  Delete(ConstPara<ValueTreeRow> Row_) { ESB_INVOKE_MEMB_PROC1(MethId::Delete, Row_); }
	void  Delete(ConstPara<Numeric> Index_) { ESB_INVOKE_MEMB_PROC1(MethId::Delete, Index_); }
	void  Clear() { ESB_INVOKE_MEMB_PROC0(MethId::Clear); }
	void  Move(ConstPara<Numeric> Row_, ConstPara<Numeric> Offset_) { ESB_INVOKE_MEMB_PROC2(MethId::Move, Row_, Offset_); }
	void  Move(ConstPara<ValueTreeRow> Row_, ConstPara<Numeric> Offset_) { ESB_INVOKE_MEMB_PROC2(MethId::Move, Row_, Offset_); }
	UndefOr<ValueTreeRow>  Find(ConstPara<Arbitrary> Value_, ConstPara<String> Columns_ = DefUndef<String>, ConstPara<Boolean> IncludeSubordinates_ = DefUndef<Boolean>) const { ESB_INVOKE_MEMB_FUNC3(UndefOr<ValueTreeRow>, MethId::Find, Value_, Columns_, IncludeSubordinates_); }
	ArrayOfValue  FindRows(ConstPara<Structure> FilterParameters_, ConstPara<Boolean> IncludeSubordinates_ = DefUndef<Boolean>) const { ESB_INVOKE_MEMB_FUNC2(ArrayOfValue, MethId::FindRows, FilterParameters_, IncludeSubordinates_); }
	UndefOr<Numeric>  Total(ConstPara<Numeric> Column_, ConstPara<Boolean> IncludeSubordinates_ = DefUndef<Boolean>) const { ESB_INVOKE_MEMB_FUNC2(UndefOr<Numeric>, MethId::Total, Column_, IncludeSubordinates_); }
	UndefOr<Numeric>  Total(ConstPara<String> Column_, ConstPara<Boolean> IncludeSubordinates_ = DefUndef<Boolean>) const { ESB_INVOKE_MEMB_FUNC2(UndefOr<Numeric>, MethId::Total, Column_, IncludeSubordinates_); }
	UndefOr<Numeric>  Total(ConstPara<ValueTreeColumn> Column_, ConstPara<Boolean> IncludeSubordinates_ = DefUndef<Boolean>) { ESB_INVOKE_MEMB_FUNC2(UndefOr<Numeric>, MethId::Total, Column_, IncludeSubordinates_); }
	void  Sort(ConstPara<String> SortingLine_, ConstPara<Boolean> IncludeSubordinates_ = DefUndef<Boolean>, ConstPara<CompareValues> ObjectOfComparison_ = DefUndef<CompareValues>) { ESB_INVOKE_MEMB_PROC3(MethId::Sort, SortingLine_, IncludeSubordinates_, ObjectOfComparison_); }
	void  LoadColumn(ConstPara<ArrayOfValue> Array_, ConstPara<Numeric> Column_) { ESB_INVOKE_MEMB_PROC2(MethId::LoadColumn, Array_, Column_); }
	void  LoadColumn(ConstPara<ArrayOfValue> Array_, ConstPara<String> Column_) { ESB_INVOKE_MEMB_PROC2(MethId::LoadColumn, Array_, Column_); }
	void  LoadColumn(ConstPara<ArrayOfValue> Array_, ConstPara<ValueTreeColumn> Column_) { ESB_INVOKE_MEMB_PROC2(MethId::LoadColumn, Array_, Column_); }
	ArrayOfValue  UnloadColumn(ConstPara<Numeric> Column_) const { ESB_INVOKE_MEMB_FUNC1(ArrayOfValue, MethId::UnloadColumn, Column_); }
	ArrayOfValue  UnloadColumn(ConstPara<String> Column_) const { ESB_INVOKE_MEMB_FUNC1(ArrayOfValue, MethId::UnloadColumn, Column_); }
	ArrayOfValue  UnloadColumn(ConstPara<ValueTreeColumn> Column_) const { ESB_INVOKE_MEMB_FUNC1(ArrayOfValue, MethId::UnloadColumn, Column_); }
	ValueTreeRow  Get(ConstPara<Numeric> Index_) const { ESB_INVOKE_MEMB_FUNC1(ValueTreeRow, MethId::Get, Index_); }
public:
	UndefOr<ValueTreeRow> GetParent() const { ESB_INVOKE_PROPERTY_GET(UndefOr<ValueTreeRow>, PropId::Parent); }
public:
	ESB_IMPLEMENT_ENUM_ITEMS(ValueTreeRowCollection, ValueTreeRow)
public:
	ESB_IMPLEMENT_INDEXED_COLLECTION_RO()
};

inline
ValueTreeRowCollection ValueTreeRow::GetRows() const { ESB_INVOKE_PROPERTY_GET(ValueTreeRowCollection, PropId::Rows); }



class ValueTree : public Object {
	friend ValueTree make<ValueTree>(IValuePtr&&);
	ValueTree(IValuePtr&& val_, IObjectPtr&& obj_) throw() : Object(std::move(val_), std::move(obj_)) {}
	struct MethId { ESB_ID(Copy, 0); ESB_ID(ChooseRow, 1); };
	struct PropId { ESB_ID(Columns, 0); ESB_ID(Rows, 1); };
public:
	ESB_DECLARE_TYPEINFO()
public:
	static ValueTree Create() { ESB_INVOKE_CTOR0(ValueTree); }
public:
	ValueTree  Copy() const { ESB_INVOKE_MEMB_FUNC0(ValueTree, MethId::Copy); }
	UndefOr<ValueTreeRow>  ChooseRow(ConstPara<String> Title_, ConstPara<ValueTreeRow> InitialRow_ = DefUndef<ValueTreeRow>) const { ESB_INVOKE_MEMB_FUNC2(UndefOr<ValueTreeRow>, MethId::ChooseRow, Title_, InitialRow_); }
public:
	ValueTreeColumnCollection GetColumns() const { ESB_INVOKE_PROPERTY_GET(ValueTreeColumnCollection, PropId::Columns); }
	ESB_PROPERTY_FIELD_RO(ValueTreeColumnCollection, Columns);
	ValueTreeRowCollection GetRows() const { ESB_INVOKE_PROPERTY_GET(ValueTreeRowCollection, PropId::Rows); }
	ESB_PROPERTY_FIELD_RO(ValueTreeRowCollection, Rows);
};


inline
ValueTree ValueTreeRow::Owner() const { ESB_INVOKE_MEMB_FUNC0(ValueTree, MethId::Owner); }




class QueryParameterDescription : public Object {
	friend QueryParameterDescription make<QueryParameterDescription>(IValuePtr&&);
	QueryParameterDescription(IValuePtr&& val_, IObjectPtr&& obj_) throw() : Object(std::move(val_), std::move(obj_)) {}
	struct PropId { ESB_ID(Name, 0); ESB_ID(ValueType, 1); };
public:
	ESB_DECLARE_TYPEINFO()
public:
	// Not Creatable
public:
	String GetName() const { ESB_INVOKE_PROPERTY_GET(String, PropId::Name); }
	ESB_PROPERTY_FIELD_RO(String, Name);
	TypeDescription GetValueType() const { ESB_INVOKE_PROPERTY_GET(TypeDescription, PropId::ValueType); }
	ESB_PROPERTY_FIELD_RO(TypeDescription, ValueType);
};


//class QueryParametersDescription : public ObjectDual<AxCollectionRO<Arbitrary, ClassMix<void>>>, protected DynamicCollectionImplRO<QueryParametersDescription, QueryParameterDescription> {
class QueryParametersDescription : public Object, protected DynamicCollectionImplRO<QueryParametersDescription, QueryParameterDescription> {
	ESB_FRIEND_DYNAMIC_COLLECTION()
		friend QueryParametersDescription make<QueryParametersDescription>(IValuePtr&&);
	//QueryParametersDescription(IValuePtr&& val_, IObjectPtr&& obj_, IIxCollectionROPtr&& axc_) throw() : ObjectDual(std::move(val_), std::move(obj_), std::move(axc_)) {}
	QueryParametersDescription(IValuePtr&& val_, IObjectPtr&& obj_) throw() : Object(std::move(val_), std::move(obj_)) {}
	struct MethId { ESB_ID(Count, 0); ESB_ID(Find, 1); };
public:
	ESB_DECLARE_TYPEINFO()
public:
	// Not Creatable
public:
	Numeric  Count() const { ESB_INVOKE_MEMB_FUNC0(Numeric, MethId::Count); }
	UndefOr<QueryParameterDescription>  Find(ConstPara<String> Name_) const { ESB_INVOKE_MEMB_FUNC1(UndefOr<QueryParameterDescription>, MethId::Find, Name_); }
public:
	ESB_IMPLEMENT_ENUM_ITEMS(QueryParametersDescription, QueryParameterDescription)
public:
	ESB_IMPLEMENT_DYNAMIC_COLLECTION_RO_NAME(ParameterName)
public:
	//ESB_IMPLEMENT_ASSICIATIVE_COLLECTION() //manual!  
};


class QueryResultColumn : public Object {
	friend QueryResultColumn make<QueryResultColumn>(IValuePtr&&);
	QueryResultColumn(IValuePtr&& val_, IObjectPtr&& obj_) throw() : Object(std::move(val_), std::move(obj_)) {}
	struct PropId { ESB_ID(Name, 0); ESB_ID(Width, 1); ESB_ID(ValueType, 2); };
public:
	ESB_DECLARE_TYPEINFO()
public:
	// Not Creatable
public:
	String GetName() const { ESB_INVOKE_PROPERTY_GET(String, PropId::Name); }
	ESB_PROPERTY_FIELD_RO(String, Name);
	Numeric GetWidth() const { ESB_INVOKE_PROPERTY_GET(Numeric, PropId::Width); }
	ESB_PROPERTY_FIELD_RO(Numeric, Width);
	TypeDescription GetValueType() const { ESB_INVOKE_PROPERTY_GET(TypeDescription, PropId::ValueType); }
	ESB_PROPERTY_FIELD_RO(TypeDescription, ValueType);
};


class QueryResultColumnsCollection : public ObjectDual2<IxCollectionRO<QueryResultColumn>, AxCollectionRO<Arbitrary, QueryResultColumn>>, protected DynamicCollectionImplRO<QueryResultColumnsCollection, QueryResultColumn> {
	ESB_FRIEND_DYNAMIC_COLLECTION()
		friend QueryResultColumnsCollection make<QueryResultColumnsCollection>(IValuePtr&&);
	QueryResultColumnsCollection(IValuePtr&& val_, IObjectPtr&& obj_, IIxCollectionROPtr&& ixc_, IAxCollectionROPtr&& axc_) throw() : ObjectDual2(std::move(val_), std::move(obj_), std::move(ixc_), std::move(axc_)) {}
	struct MethId { ESB_ID(Count, 0); ESB_ID(IndexOf, 1); ESB_ID(Find, 2); ESB_ID(Get, 3); };
public:
	ESB_DECLARE_TYPEINFO()
public:
	// Not Creatable
public:
	Numeric  Count() const { ESB_INVOKE_MEMB_FUNC0(Numeric, MethId::Count); }
	Numeric  IndexOf(ConstPara<QueryResultColumn> Column_) const { ESB_INVOKE_MEMB_FUNC1(Numeric, MethId::IndexOf, Column_); }
	UndefOr<QueryResultColumn>  Find(ConstPara<String> ColumnDescription_) const { ESB_INVOKE_MEMB_FUNC1(UndefOr<QueryResultColumn>, MethId::Find, ColumnDescription_); }
	QueryResultColumn  Get(ConstPara<Numeric> Index_) const { ESB_INVOKE_MEMB_FUNC1(QueryResultColumn, MethId::Get, Index_); }
public:
	ESB_IMPLEMENT_ENUM_ITEMS(QueryResultColumnsCollection, QueryResultColumn)
public:
	ESB_IMPLEMENT_INDEXED_COLLECTION_RO()
public:
	ESB_IMPLEMENT_DYNAMIC_COLLECTION_RO_NAME(ColumnName)
public:
	//ESB_IMPLEMENT_ASSICIATIVE_COLLECTION() //manual!  
};


//TOBE:		entext IResultSelectionIterator:: bool next(). больше не надо - IIndexedCollection[] отлично данные отдает.
//NOTE:	мы считаем методы навигации по запросу not-const, т.к. при этом, хотя данные в запросе и не меняются, но видимые изменения через свойства (текущее) присутствуют.
class QueryResultSelection : public ObjectDual2<IxCollectionRO<Arbitrary>, AxCollectionRO<Arbitrary, Arbitrary>>, protected DynamicCollectionImplRO<QueryResultSelection, Arbitrary> {
	using base_t = ObjectDual2<IxCollectionRO<Arbitrary>, AxCollectionRO<Arbitrary, Arbitrary>>;
	ESB_FRIEND_DYNAMIC_COLLECTION()
	friend QueryResultSelection make<QueryResultSelection>(IValuePtr&&);
	QueryResultSelection(IValuePtr&& val_, IObjectPtr&& obj_, IIxCollectionROPtr&& ixc_, IAxCollectionROPtr&& axc_) throw() : ObjectDual2(std::move(val_), std::move(obj_), std::move(ixc_), std::move(axc_)) {}
	ESB_CLASS_IMPLEMENT_MAKE_OPT(QueryResultSelection)
	struct MethId { ESB_ID(Next, 0); ESB_ID(Select, 1); ESB_ID(Group, 2); ESB_ID(RecordType, 3); ESB_ID(Level, 4); ESB_ID(NextByFieldValue, 5); ESB_ID(FindNext, 6); ESB_ID(Count, 7); ESB_ID(Reset, 8); ESB_ID(Get, 9); ESB_ID(Owner, 10); };
public:
	ESB_DECLARE_TYPEINFO()
public:
	// Not Creatable
public:
	Boolean		 Next()							{ ESB_INVOKE_MEMB_FUNC0(Boolean, MethId::Next); }
	QueryResultSelection  Select(ConstPara<QueryResultIterationValue> TabOrderType_, ConstPara<String> Groups_, ConstPara<String> GroupsForGroupValues_ = DefUndef<String>) const { ESB_INVOKE_MEMB_FUNC3(QueryResultSelection, MethId::Select, TabOrderType_, Groups_, GroupsForGroupValues_); }
	String		 Group() const					{ ESB_INVOKE_MEMB_FUNC0(String, MethId::Group); }
	QueryRecordTypeValue  RecordType() const	{ ESB_INVOKE_MEMB_FUNC0(QueryRecordTypeValue, MethId::RecordType); }
	Numeric		 Level() const					{ ESB_INVOKE_MEMB_FUNC0(Numeric, MethId::Level); }
	Boolean		 NextByFieldValue(ConstPara<String> FieldName_)											{ ESB_INVOKE_MEMB_FUNC1(Boolean, MethId::NextByFieldValue, FieldName_); }
	Boolean		 FindNext(ConstPara<Structure> TheStructureOfTheSearch_)								{ ESB_INVOKE_MEMB_FUNC1(Boolean, MethId::FindNext, TheStructureOfTheSearch_); }
	Boolean		 FindNext(ConstPara<Arbitrary> Value_, ConstPara<String> Columns_ = DefUndef<String>)	{ ESB_INVOKE_MEMB_FUNC2(Boolean, MethId::FindNext, Value_, Columns_); }
	Numeric		 Count() const							{ ESB_INVOKE_MEMB_FUNC0(Numeric, MethId::Count); }
	void		 Reset()								{ ESB_INVOKE_MEMB_PROC0(MethId::Reset);}
	Arbitrary	 Get(ConstPara<Numeric> Index_)	const	{ ESB_INVOKE_MEMB_FUNC1(Arbitrary, MethId::Get, Index_); }
	QueryResult  Owner() const;			// { ESB_INVOKE_MEMB_FUNC0(QueryResult, MethId::Owner); } - после QueryResult
public:
	ESB_IMPLEMENT_INDEXED_COLLECTION_RO()
public:
	ESB_IMPLEMENT_DYNAMIC_COLLECTION_RO_NAME(FieldName)
public:
	//ESB_IMPLEMENT_ASSICIATIVE_COLLECTION() //manual!  
};


class QueryResult : public Object {
	friend QueryResult make<QueryResult>(IValuePtr&&);
	QueryResult(IValuePtr&& val_, IObjectPtr&& obj_) throw() : Object(std::move(val_), std::move(obj_)) {}
	struct MethId { ESB_ID(Select, 0); ESB_ID(Unload, 1); ESB_ID(IsEmpty, 2); };
	struct PropId { ESB_ID(Columns, 0); };
public:
	ESB_DECLARE_TYPEINFO()
public:
	// Not Creatable
public:
	//manual
	QueryResultSelection Select(ConstPara<QueryResultIterationValue> TabOrderType_ = QueryResultIterationEnum::Linear,
								ConstPara<String> Groups_ = DefUndef<String>, 
								ConstPara<String> GroupsForGroupValues_ = DefUndef<String>) const { ESB_INVOKE_MEMB_FUNC3(QueryResultSelection, MethId::Select, TabOrderType_, Groups_, GroupsForGroupValues_); }
	//
	ClassMix<ValueTable, ValueTree>  Unload(ConstPara<QueryResultIterationValue> TabOrderType_) const { ESB_INVOKE_MEMB_FUNC1(ESB_MIX2(ValueTable, ValueTree), MethId::Unload, TabOrderType_); }
	Boolean  IsEmpty() const { ESB_INVOKE_MEMB_FUNC0(Boolean, MethId::IsEmpty); }
public:
	QueryResultColumnsCollection GetColumns() const { ESB_INVOKE_PROPERTY_GET(QueryResultColumnsCollection, PropId::Columns); }
	ESB_PROPERTY_FIELD_RO(QueryResultColumnsCollection, Columns);
};


inline
QueryResult QueryResultSelection::Owner() const { ESB_INVOKE_MEMB_FUNC0(QueryResult, MethId::Owner); }


class QueryTempTableColumn : public Object {
	friend QueryTempTableColumn make<QueryTempTableColumn>(IValuePtr&&);
	QueryTempTableColumn(IValuePtr&& val_, IObjectPtr&& obj_) throw() : Object(std::move(val_), std::move(obj_)) {}
	struct PropId { ESB_ID(Name, 0); ESB_ID(ValueType, 1); };
public:
	ESB_DECLARE_TYPEINFO()
public:
	// Not Creatable
public:
	String GetName() const					{ ESB_INVOKE_PROPERTY_GET(String, PropId::Name); }
	ESB_PROPERTY_FIELD_RO(String, Name);
	TypeDescription GetValueType() const	{ ESB_INVOKE_PROPERTY_GET(TypeDescription, PropId::ValueType); }
	ESB_PROPERTY_FIELD_RO(TypeDescription, ValueType);
};


class QueryTempTableColumns : public ObjectDual2<IxCollectionRO<QueryTempTableColumn>, AxCollectionRO<Arbitrary, QueryTempTableColumn>>, protected DynamicCollectionImplRO<QueryTempTableColumns, QueryTempTableColumn> {
	ESB_FRIEND_DYNAMIC_COLLECTION()
	friend QueryTempTableColumns make<QueryTempTableColumns>(IValuePtr&&);
	QueryTempTableColumns(IValuePtr&& val_, IObjectPtr&& obj_, IIxCollectionROPtr&& ixc_, IAxCollectionROPtr&& axc_) throw() : ObjectDual2(std::move(val_), std::move(obj_), std::move(ixc_), std::move(axc_)) {}
	struct MethId { ESB_ID(Count, 0); ESB_ID(Find, 1); ESB_ID(Get, 2); ESB_ID(IndexOf, 3); };
public:
	ESB_DECLARE_TYPEINFO()
public:
	// Not Creatable
public:
	Numeric  Count() const { ESB_INVOKE_MEMB_FUNC0(Numeric, MethId::Count); }
	UndefOr<QueryTempTableColumn>  Find(ConstPara<String> ColumnDescription_) const { ESB_INVOKE_MEMB_FUNC1(UndefOr<QueryTempTableColumn>, MethId::Find, ColumnDescription_); }
	QueryTempTableColumn  Get(ConstPara<Numeric> Index_) const { ESB_INVOKE_MEMB_FUNC1(QueryTempTableColumn, MethId::Get, Index_); }
	Numeric  IndexOf(ConstPara<QueryTempTableColumn> Column_) const { ESB_INVOKE_MEMB_FUNC1(Numeric, MethId::IndexOf, Column_); }
public:
	ESB_IMPLEMENT_ENUM_ITEMS(QueryTempTableColumns, QueryTempTableColumn)
public:
	ESB_IMPLEMENT_INDEXED_COLLECTION_RO()
public:
	ESB_IMPLEMENT_DYNAMIC_COLLECTION_RO_NAME(ColumnName)
public:
	//ESB_IMPLEMENT_ASSICIATIVE_COLLECTION() //manual!  
};


class QueryTempTable : public Object {
	friend QueryTempTable make<QueryTempTable>(IValuePtr&&);
	QueryTempTable(IValuePtr&& val_, IObjectPtr&& obj_) throw() : Object(std::move(val_), std::move(obj_)) {}
	struct MethId { ESB_ID(GetData, 0); };
	struct PropId { ESB_ID(FullName, 0); ESB_ID(Columns, 1); };
public:
	ESB_DECLARE_TYPEINFO()
public:
	// Not Creatable
public:
	QueryResult  GetData() const { ESB_INVOKE_MEMB_FUNC0(QueryResult, MethId::GetData); }
public:
	String GetFullName() const { ESB_INVOKE_PROPERTY_GET(String, PropId::FullName); }
	ESB_PROPERTY_FIELD_RO(String, FullName);
	QueryTempTableColumns GetColumns() const { ESB_INVOKE_PROPERTY_GET(QueryTempTableColumns, PropId::Columns); }
	ESB_PROPERTY_FIELD_RO(QueryTempTableColumns, Columns);
};


class QueryTempTables : public ObjectDual2<IxCollectionRO<QueryTempTable>, AxCollectionRO<Arbitrary, QueryTempTable>> {
	friend QueryTempTables make<QueryTempTables>(IValuePtr&&);
	QueryTempTables(IValuePtr&& val_, IObjectPtr&& obj_, IIxCollectionROPtr&& ixc_, IAxCollectionROPtr&& axc_) throw() : ObjectDual2(std::move(val_), std::move(obj_), std::move(ixc_), std::move(axc_)) {}
	struct MethId { ESB_ID(Count, 0); ESB_ID(Find, 1); ESB_ID(Get, 2); ESB_ID(IndexOf, 3); };
public:
	ESB_DECLARE_TYPEINFO()
public:
	// Not Creatable
public:
	Numeric  Count() const { ESB_INVOKE_MEMB_FUNC0(Numeric, MethId::Count); }
	UndefOr<QueryTempTable>  Find(ConstPara<String> ColumnDescription_) const { ESB_INVOKE_MEMB_FUNC1(UndefOr<QueryTempTable>, MethId::Find, ColumnDescription_); }
	QueryTempTable  Get(ConstPara<Numeric> Index_) const { ESB_INVOKE_MEMB_FUNC1(QueryTempTable, MethId::Get, Index_); }
	Numeric  IndexOf(ConstPara<QueryTempTable> TempTable_) const { ESB_INVOKE_MEMB_FUNC1(Numeric, MethId::IndexOf, TempTable_); }
public:
	ESB_IMPLEMENT_ENUM_ITEMS(QueryTempTables, QueryTempTable)
public:
	ESB_IMPLEMENT_INDEXED_COLLECTION_RO()
public:
	//ESB_IMPLEMENT_ASSICIATIVE_COLLECTION() //manual!  
};


class TempTablesManager : public Object {
	friend TempTablesManager make<TempTablesManager>(IValuePtr&&);
	TempTablesManager(IValuePtr&& val_, IObjectPtr&& obj_) throw() : Object(std::move(val_), std::move(obj_)) {}
	struct MethId { ESB_ID(Close, 0); };
	struct PropId { ESB_ID(Tables, 0); };
public:
	ESB_DECLARE_TYPEINFO()
public:
	// Not Creatable
public:
	void  Close() { ESB_INVOKE_MEMB_PROC0(MethId::Close); }
public:
	QueryTempTables GetTables() const { ESB_INVOKE_PROPERTY_GET(QueryTempTables, PropId::Tables); }
	ESB_PROPERTY_FIELD_RO(QueryTempTables, Tables);
};


class Query : public Object {
	friend Query make<Query>(IValuePtr&&);
	Query(IValuePtr&& val_, IObjectPtr&& obj_) throw() : Object(std::move(val_), std::move(obj_)) {}
	struct MethId { ESB_ID(Execute, 0); ESB_ID(SetParameter, 1); ESB_ID(FindParameters, 2); ESB_ID(ExecuteBatch, 3); ESB_ID(ExecuteBatchWithIntermediateData, 4); };
	struct PropId { ESB_ID(Text, 0); ESB_ID(Parameters, 1); ESB_ID(TempTablesManager, 2); };
public:
	ESB_DECLARE_TYPEINFO()
public:
	static Query  Create() { ESB_INVOKE_CTOR0(Query); }
	static Query  Create(ConstPara<String> RequestText_) { ESB_INVOKE_CTOR1(Query, RequestText_); }
public:
	UndefOr<QueryResult>  Execute() const					{ ESB_INVOKE_MEMB_FUNC0(UndefOr<QueryResult>, MethId::Execute); }
	void  SetParameter(ConstPara<String> Name_, ConstPara<Arbitrary> Value_)	{ ESB_INVOKE_MEMB_PROC2(MethId::SetParameter, Name_, Value_); }
	QueryParametersDescription  FindParameters() const		{ ESB_INVOKE_MEMB_FUNC0(QueryParametersDescription, MethId::FindParameters); }
	ArrayOfValue  ExecuteBatch() const						{ ESB_INVOKE_MEMB_FUNC0(ArrayOfValue, MethId::ExecuteBatch); }
	ArrayOfValue  ExecuteBatchWithIntermediateData() const	{ ESB_INVOKE_MEMB_FUNC0(ArrayOfValue, MethId::ExecuteBatchWithIntermediateData); }
public:
	String GetText() const					{ ESB_INVOKE_PROPERTY_GET(String, PropId::Text); }
	void SetText(ConstPara<String> Value_)	{ ESB_INVOKE_PROPERTY_SET(PropId::Text, Value_); }
	ESB_PROPERTY_FIELD_RW(String, Text);
	Structure GetParameters() const			{ ESB_INVOKE_PROPERTY_GET(Structure, PropId::Parameters); }
	ESB_PROPERTY_FIELD_RO(Structure, Parameters);
	UndefOr<TempTablesManager> GetTempTablesManager() const			{ ESB_INVOKE_PROPERTY_GET(UndefOr<TempTablesManager>, PropId::TempTablesManager); }
	void SetTempTablesManager(ConstPara<TempTablesManager> Value_)	{ ESB_INVOKE_PROPERTY_SET(PropId::TempTablesManager, Value_); }
	void SetTempTablesManager(ConstPara<Undef> Value_)				{ ESB_INVOKE_PROPERTY_SET(PropId::TempTablesManager, Value_); }
};


class ValueListItem : public Object {
	friend ValueListItem make<ValueListItem>(IValuePtr&&);
	ValueListItem(IValuePtr&& val_, IObjectPtr&& obj_) throw() : Object(std::move(val_), std::move(obj_)) {}
	struct MethId { ESB_ID(GetID, 0); };
	struct PropId { ESB_ID(Value, 0); ESB_ID(Presentation, 1); ESB_ID(Check, 2); ESB_ID(Picture, 3); };
public:
	ESB_DECLARE_TYPEINFO()
public:
	// Not Creatable
public:
	Numeric  GetID() const { ESB_INVOKE_MEMB_FUNC0(Numeric, MethId::GetID); }
public:
	Arbitrary GetValue() const { ESB_INVOKE_PROPERTY_GET(Arbitrary, PropId::Value); }
	void SetValue(ConstPara<Arbitrary> Value_) { ESB_INVOKE_PROPERTY_SET(PropId::Value, Value_); }
	ESB_PROPERTY_FIELD_RW(Arbitrary, Value);
	ClassMix<String, FormattedString> GetPresentation() const { ESB_INVOKE_PROPERTY_GET(ESB_MIX2(String, FormattedString), PropId::Presentation); }
	void SetPresentation(ConstPara<String> Value_) { ESB_INVOKE_PROPERTY_SET(PropId::Presentation, Value_); }
	void SetPresentation(ConstPara<FormattedString> Value_) { ESB_INVOKE_PROPERTY_SET(PropId::Presentation, Value_); }
	Boolean GetCheck() const { ESB_INVOKE_PROPERTY_GET(Boolean, PropId::Check); }
	void SetCheck(ConstPara<Boolean> Value_) { ESB_INVOKE_PROPERTY_SET(PropId::Check, Value_); }
	ESB_PROPERTY_FIELD_RW(Boolean, Check);
	Picture GetPicture() const { ESB_INVOKE_PROPERTY_GET(esb::Picture, PropId::Picture); }
	void SetPicture(ConstPara<Picture> Value_) { ESB_INVOKE_PROPERTY_SET(PropId::Picture, Value_); }
	ESB_PROPERTY_FIELD_RW(esb::Picture, Picture);
};


class ValueList : public ObjectDual<IxCollectionRO<ValueListItem>> {
	friend ValueList make<ValueList>(IValuePtr&&);
	ValueList(IValuePtr&& val_, IObjectPtr&& obj_, IIxCollectionROPtr&& ixc_) throw() : ObjectDual(std::move(val_), std::move(obj_), std::move(ixc_)) {}
	struct MethId { ESB_ID(FindByValue, 0); ESB_ID(FindByID, 1); ESB_ID(SortByValue, 2); ESB_ID(SortByPresentation, 3); ESB_ID(ChooseItem, 4); ESB_ID(ShowChooseItem, 5); ESB_ID(CheckItems, 6); ESB_ID(ShowCheckItems, 7); ESB_ID(UnloadValues, 8); ESB_ID(LoadValues, 9); ESB_ID(Copy, 10); ESB_ID(Count, 11); ESB_ID(Add, 12); ESB_ID(Insert, 13); ESB_ID(Delete, 14); ESB_ID(Clear, 15); ESB_ID(Move, 16); ESB_ID(IndexOf, 17); ESB_ID(Get, 18); ESB_ID(FillChecks, 19); };
	struct PropId { ESB_ID(ValueType, 0); ESB_ID(AvailableValues, 1); };
public:
	ESB_DECLARE_TYPEINFO()
public:
	static ValueList Create() { ESB_INVOKE_CTOR0(ValueList); }
public:
	UndefOr<ValueListItem>  FindByValue(ConstPara<Arbitrary> SoughtValue_) const { ESB_INVOKE_MEMB_FUNC1(UndefOr<ValueListItem>, MethId::FindByValue, SoughtValue_); }
	UndefOr<ValueListItem>  FindByID(ConstPara<Numeric> ID_) const { ESB_INVOKE_MEMB_FUNC1(UndefOr<ValueListItem>, MethId::FindByID, ID_); }
	void  SortByValue(ConstPara<SortDirectionValue> Direction_) { ESB_INVOKE_MEMB_PROC1(MethId::SortByValue, Direction_); }
	void  SortByPresentation(ConstPara<SortDirectionValue> Direction_) { ESB_INVOKE_MEMB_PROC1(MethId::SortByPresentation, Direction_); }
	UndefOr<ValueListItem>  ChooseItem(ConstPara<String> Title_, ConstPara<ValueListItem> Item_ = DefUndef<ValueListItem>) const { ESB_INVOKE_MEMB_FUNC2(UndefOr<ValueListItem>, MethId::ChooseItem, Title_, Item_); }
	void  ShowChooseItem(ConstPara<NotifyDescription> NotifyOnCloseDescription_, ConstPara<String> Title_ = DefUndef<String>, ConstPara<ValueListItem> Item_ = DefUndef<ValueListItem>) const { ESB_INVOKE_MEMB_PROC3(MethId::ShowChooseItem, NotifyOnCloseDescription_, Title_, Item_); }
	Boolean  CheckItems(ConstPara<String> Title_) { ESB_INVOKE_MEMB_FUNC1(Boolean, MethId::CheckItems, Title_); }
	void  ShowCheckItems(ConstPara<NotifyDescription> NotifyOnCloseDescription_, ConstPara<String> Title_ = DefUndef<String>) { ESB_INVOKE_MEMB_PROC2(MethId::ShowCheckItems, NotifyOnCloseDescription_, Title_); }
	ArrayOfValue  UnloadValues() const { ESB_INVOKE_MEMB_FUNC0(ArrayOfValue, MethId::UnloadValues); }
	void  LoadValues(ConstPara<ArrayOfValue> ArrayOfValues_) { ESB_INVOKE_MEMB_PROC1(MethId::LoadValues, ArrayOfValues_); }
	ValueList  Copy() const { ESB_INVOKE_MEMB_FUNC0(ValueList, MethId::Copy); }
	Numeric  Count() const { ESB_INVOKE_MEMB_FUNC0(Numeric, MethId::Count); }
	ValueListItem  Add(ConstPara<Arbitrary> Value_, ConstPara<String> Representation_, ConstPara<Boolean> CheckMark_, ConstPara<Picture> Picture_ = DefUndef<Picture>) { ESB_INVOKE_MEMB_FUNC4(ValueListItem, MethId::Add, Value_, Representation_, CheckMark_, Picture_); }
	ValueListItem  Add(ConstPara<Arbitrary> Value_, ConstPara<FormattedString> Representation_, ConstPara<Boolean> CheckMark_, ConstPara<Picture> Picture_ = DefUndef<Picture>) { ESB_INVOKE_MEMB_FUNC4(ValueListItem, MethId::Add, Value_, Representation_, CheckMark_, Picture_); }
	ValueListItem  Insert(ConstPara<Numeric> Index_, ConstPara<Arbitrary> Value_ = Undef::Value_, ConstPara<String> Representation_ = DefUndef<String>, ConstPara<Boolean> CheckMark_ = DefUndef<Boolean>, ConstPara<Picture> Picture_ = DefUndef<Picture>) { ESB_INVOKE_MEMB_FUNC5(ValueListItem, MethId::Insert, Index_, Value_, Representation_, CheckMark_, Picture_); }
	ValueListItem  Insert(ConstPara<Numeric> Index_, ConstPara<Arbitrary> Value_ = Undef::Value_, ConstPara<FormattedString> Representation_ = DefUndef<FormattedString>, ConstPara<Boolean> CheckMark_ = DefUndef<Boolean>, ConstPara<Picture> Picture_ = DefUndef<Picture>) { ESB_INVOKE_MEMB_FUNC5(ValueListItem, MethId::Insert, Index_, Value_, Representation_, CheckMark_, Picture_); }
	void  Delete(ConstPara<Numeric> Item_) { ESB_INVOKE_MEMB_PROC1(MethId::Delete, Item_); }
	void  Delete(ConstPara<ValueListItem> Item_) { ESB_INVOKE_MEMB_PROC1(MethId::Delete, Item_); }
	void  Clear() { ESB_INVOKE_MEMB_PROC0(MethId::Clear); }
	void  Move(ConstPara<Numeric> Item_, ConstPara<Numeric> Offset_) { ESB_INVOKE_MEMB_PROC2(MethId::Move, Item_, Offset_); }
	void  Move(ConstPara<ValueListItem> Item_, ConstPara<Numeric> Offset_) { ESB_INVOKE_MEMB_PROC2(MethId::Move, Item_, Offset_); }
	Numeric  IndexOf(ConstPara<ValueListItem> Item_) const { ESB_INVOKE_MEMB_FUNC1(Numeric, MethId::IndexOf, Item_); }
	ValueListItem  Get(ConstPara<Numeric> Index_) const { ESB_INVOKE_MEMB_FUNC1(ValueListItem, MethId::Get, Index_); }
	void  FillChecks(ConstPara<Boolean> CheckMarkValue_) { ESB_INVOKE_MEMB_PROC1(MethId::FillChecks, CheckMarkValue_); }
public:
	TypeDescription GetValueType() const { ESB_INVOKE_PROPERTY_GET(TypeDescription, PropId::ValueType); }
	void SetValueType(ConstPara<TypeDescription> Value_) { ESB_INVOKE_PROPERTY_SET(PropId::ValueType, Value_); }
	ESB_PROPERTY_FIELD_RW(TypeDescription, ValueType);
	UndefOr<ValueList> GetAvailableValues() const { ESB_INVOKE_PROPERTY_GET(UndefOr<ValueList>, PropId::AvailableValues); }
	void SetAvailableValues(ConstPara<ValueList> Value_) { ESB_INVOKE_PROPERTY_SET(PropId::AvailableValues, Value_); }
	void SetAvailableValues(ConstPara<Undef> Value_) { ESB_INVOKE_PROPERTY_SET(PropId::AvailableValues, Value_); }
public:
	ESB_IMPLEMENT_ENUM_ITEMS(ValueList, ValueListItem)
public:
	ESB_IMPLEMENT_INDEXED_COLLECTION_RO()
};



// определение АПИ зависящего от типов esb_data
//
// Функция НайтиПоСсылкам(СписокСсылок_ Как Массив, ОбластьПоиска_ Как Массив, ВключитьОбъекты_ Как Массив, ИсключитьОбъекты_ Как Массив) Возвращает ТаблицаЗначений
inline ValueTable FindByRef(ConstPara<Array> ReferenceList_, ConstPara<Array> SearchArea_ /*= DefUndef<Array>*/, ConstPara<Array> IncludeObjects_ /*= DefUndef<Array>*/, ConstPara<Array> ExcludeObjects_ /*= DefUndef<Array>*/) { ESB_INVOKE_API_FUNC4(ValueTable, backend, 6, ReferenceList_, SearchArea_, IncludeObjects_, ExcludeObjects_); }
// Функция ПолучитьСтруктуруХраненияБазыДанных(ОбъектыМетаданных_ Как Массив, ИменаБазыДанных_ Как Булево) Возвращает ТаблицаЗначений
inline ValueTable GetDBStorageStructureInfo(ConstPara<Array> MetadataObjects_, ConstPara<Boolean> DBMSNames_ /*= DefUndef<Boolean>*/) { ESB_INVOKE_API_FUNC2(ValueTable, backend, 45, MetadataObjects_, DBMSNames_); }
// Функция ПользователиWindows() Возвращает [ТаблицаЗначений,Массив]
inline ClassMix<ValueTable, Array> WindowsUsers() { ESB_INVOKE_API_FUNC0(ESB_MIX2(ValueTable, Array), frntend, 29); }
// Функция ПользователиОС() Возвращает [ТаблицаЗначений,Массив]
inline ClassMix<ValueTable, Array> OSUsers() { ESB_INVOKE_API_FUNC0(ESB_MIX2(ValueTable, Array), frntend, 30); }
// Функция ПолучитьДанныеВыбора(ТипЗначения_ Как Тип, Параметры_ Как Структура) Возвращает СписокЗначений
inline ValueList GetChoiceData(ConstPara<TypeValue> ValueType_, ConstPara<Structure> Parameters_) { ESB_INVOKE_API_FUNC2(ValueList, mngbase, 9, ValueType_, Parameters_); }
/////////////////////////////////////////////////


} //namespace esb

#endif	//ESBDATA_H
