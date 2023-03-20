#pragma once
#ifndef ESB_H
#define ESB_H

// базовое для работы с 1С
#include "esb_base.h"

//
#include <concepts>
#include <compare>


namespace esb {
//
	class UUIDValue;
	class TypeValue;
	class ErrorInfoValue;
	//...
	class Array;
	//...


	class UUIDValue : public Value {
		ESB_CLASS_IMPLEMENT_MAKE_VAL(UUIDValue)
		ESB_CLASS_IMPLEMENT_MAKE_OPT(UUIDValue)
	public:
		ESB_DECLARE_TYPEINFO()
		ESB_IMPLEMENT_ARGBYVAL_OPERATOR(UUIDValue)
	public:
		static UUIDValue  Create()								{ ESB_INVOKE_CTOR0(UUIDValue); }
		static UUIDValue  Create(ConstPara<String> Line_)		{ ESB_INVOKE_CTOR1(UUIDValue, Line_); }
	public:
		//esbhlp
		const GUID& getUUID() const;
	};


	// это TypeValue платформы и предназначена только передаче платформе где она хочет тип в виде аргумента.
	// в есб везде используется TypeDef, в том числе и как возвращаемое значение функции Type(name). Для передаче платформе использовать метод .Value() ТипДефа.
	class TypeValue : public Value {
		ESB_CLASS_IMPLEMENT_MAKE_VAL(TypeValue)
		ESB_CLASS_IMPLEMENT_MAKE_OPT(TypeValue)
	public:
		ESB_DECLARE_TYPEINFO(TypeValue)
		ESB_IMPLEMENT_ARGBYVAL_OPERATOR(TypeValue)
	public:
		const GUID& getTypeCLSID() const;
		//TOBE: можно точнее проверить, но вроде в отличие от типов тип-валюе не синглтоны и идентифицируются через CLSID. Пусть платформа равенство проверяет.
		//		(если так, то можно здесь inline is_equl_guid сделать...)
		bool operator==(const TypeValue& other_) const {
			return (Value::operator==(other_)) ? true : IValue_IsEqualValue(*get_interface(*this), *get_interface(other_));
		}
	};

	inline TypeValue TypeDef::Value() const {
		assert(m_ptype);
		return make<TypeValue>(IType_CreateTypeValueForType(*m_ptype));
	}



	class ErrorInfoValue : public Object {
		ESB_CLASS_IMPLEMENT_MAKE_OBJ(ErrorInfoValue)
		ESB_CLASS_IMPLEMENT_MAKE_OPT(ErrorInfoValue)
		struct PropId { ESB_ID(Description, 0); ESB_ID(ModuleName, 1); ESB_ID(LineNumber, 2); ESB_ID(SourceLine, 3); ESB_ID(Cause, 4); };
	public:
		ESB_DECLARE_TYPEINFO(ErrorInfoValue)
		ESB_IMPLEMENT_ARGBYVAL_OPERATOR(ErrorInfoValue)
	public:
		// Not Creatable
	public:
		String GetDescription()	const				{ ESB_INVOKE_PROPERTY_GET(String, PropId::Description); }
		ESB_PROPERTY_FIELD_RO(String, Description);
		String GetModuleName()	const				{ ESB_INVOKE_PROPERTY_GET(String, PropId::ModuleName); }
		ESB_PROPERTY_FIELD_RO(String, ModuleName);
		Numeric GetLineNumber()	const				{ ESB_INVOKE_PROPERTY_GET(Numeric, PropId::LineNumber); }
		ESB_PROPERTY_FIELD_RO(Numeric, LineNumber);
		String GetSourceLine()	const				{ ESB_INVOKE_PROPERTY_GET(String, PropId::SourceLine); }
		ESB_PROPERTY_FIELD_RO(String, SourceLine);
		UndefOr<ErrorInfoValue> GetCause() const	{ ESB_INVOKE_PROPERTY_GET(UndefOr<ErrorInfoValue>, PropId::Cause); }
	};





	//---------------------------------- модельная реализация классов базовых контейнеров----------------------------------------------------------
	// (полу-ручками для отработки кодогенератора)
	//
	class Array;
	class FixedValueArray;
	class Structure;
	class FixedStructure;
	class Map;
	class FixedMap;



	class Array : public ObjectDual<IxCollectionRW<Arbitrary>>
	{
		ESB_CLASS_IMPLEMENT_MAKE_OBJ_DUAL(Array)
		ESB_CLASS_IMPLEMENT_MAKE_OPT(Array)
		struct MethId {
			ESB_ID(Count, 0); ESB_ID(UBound, 1); ESB_ID(Insert, 2); ESB_ID(Add, 3);	ESB_ID(Delete, 4); ESB_ID(Clear, 5); ESB_ID(Get, 6); ESB_ID(Set, 7); ESB_ID(Find, 8);
		};
	public:
		ESB_DECLARE_TYPEINFO(Array)
		ESB_IMPLEMENT_ARGBYVAL_OPERATOR(Array)
		// К сожалению этот шаблон умеет разворачиваться только для явно прописанных типов аргументов
		// а для голого Numeric "не удалось вывести аргумент шаблон для ..."
		template<std::same_as<Numeric>... NumericTy>
		static Array	Create(ConstPara<NumericTy>... args_)			{ ESB_INVOKE_CTOR_PACK0(Array, args_); }
		// поэтому сделаем еще принудительно для наиболее часто используемого Create с одним параметром
		static Array 	Create(ConstPara<Numeric> count_)				{ ESB_INVOKE_CTOR1(Array, count_); }
		static Array 	Create(ConstPara<FixedValueArray> source_)		{ ESB_INVOKE_CTOR1(Array, source_); }
	public:
		// Процедура Добавить()
		void				 Add(ConstPara<Arbitrary> value_)				{ ESB_INVOKE_MEMB_PROC1(MethId::Add, value_); }
		void				 Clear()										{ ESB_INVOKE_MEMB_PROC0(MethId::Clear); }
		Numeric				 Count()	const								{ ESB_INVOKE_MEMB_FUNC0(Numeric, MethId::Count); }
		void				 Delete(ConstPara<Numeric> index_)				{ ESB_INVOKE_MEMB_PROC1(MethId::Delete, index_); }
		UndefOr<Numeric>	 Find(ConstPara<Arbitrary> value_) const		{ ESB_INVOKE_MEMB_FUNC1(ESB_MIX2(Undef, Numeric), MethId::Find, value_); }
		Arbitrary			 Get(ConstPara<Numeric> index_) const			{ ESB_INVOKE_MEMB_FUNC1(Arbitrary, MethId::Get, index_); }
		void				 Insert(ConstPara<Numeric> index_, ConstPara<Arbitrary> value_ = DefUndef<Arbitrary>)	{ ESB_INVOKE_MEMB_PROC2(MethId::Insert, index_, value_); }
		void				 Set(ConstPara<Numeric> index_, ConstPara<Arbitrary> value_)							{ ESB_INVOKE_MEMB_PROC2(MethId::Set, index_, value_); }
		Numeric				 UBound() const									{ ESB_INVOKE_MEMB_FUNC0(Numeric, MethId::UBound); }
#if ESB_USE_RUSSIAN
		void				 Добавить(ConstPara<Arbitrary> value_)		{ ESB_INVOKE_MEMB_PROC1(MethId::Add, value_); }
		void				 Очистить()									{ ESB_INVOKE_MEMB_PROC0(MethId::Clear); }
		Numeric				 Количество() const							{ ESB_INVOKE_MEMB_FUNC0(Numeric, MethId::Count); }
		void				 Удалить(ConstPara<Numeric> index_)			{ ESB_INVOKE_MEMB_PROC1(MethId::Delete, index_); }
		UndefOr<Numeric>	 Найти(ConstPara<Arbitrary> value_) const	{ ESB_INVOKE_MEMB_FUNC1(ESB_MIX2(Undef, Numeric), MethId::Find, value_); }
		Arbitrary			 Получить(ConstPara<Numeric> index_) const	{ ESB_INVOKE_MEMB_FUNC1(Arbitrary, MethId::Get, index_); }
		void				 Вставить(ConstPara<Numeric> index_, ConstPara<Arbitrary> value_ = DefUndef<Arbitrary>)		{ ESB_INVOKE_MEMB_PROC2(MethId::Insert, index_, value_); }
		void				 Установить(ConstPara<Numeric> index_, ConstPara<Arbitrary> value_)							{ ESB_INVOKE_MEMB_PROC2(MethId::Set, index_, value_); }
		Numeric				 ВГраница() const							{ ESB_INVOKE_MEMB_FUNC0(Numeric, MethId::UBound); }
#endif
	public:
		//TOBE: нужно и можно попытаться сделать EnumItems<прямо-с-нужным-типом>: template<class T = Arbitrary> EnumItems(){...}
		ESB_IMPLEMENT_ENUM_ITEMS(Array, Arbitrary)
	public:
		using ix_collection_t::GetAt;
		using ix_collection_t::SetAt;
		using ix_collection_t::At;
		using ix_collection_t::operator[];
		ix_collection_t& AsIndexedCollection()					{ return *static_cast<ix_collection_t*>(this); }
		const ix_collection_t& AsIndexedCollection() const		{ return *static_cast<const ix_collection_t*>(this); }
		using ix_collection_t::Size;
	public:
		// интерфейс с++ контейнера. не думаю, что его нужно реализовывать на уровне ix_collection_t, т.к. контейнер - это объект, а не интерфейс
		// у разных объектов имплементирующих интерфейс могут быть отличающиеся реализации. Массив - да - контейнер. А колонки запроса... вопрос..
		using value_type = ix_collection_t::value_t;
		using iterator = IxCollectionIterator<value_type>;	//alwais IIxCollectionRWPtr
		using const_iterator = IxCollectionIteratorConst<value_type, ix_collection_t::interface_t>;
		//
		iterator		begin()			{ return iterator{ ix_collection_t::get_interface(), 0 }; }
		const_iterator	begin() const	{ return const_iterator{ ix_collection_t::get_interface(), 0 }; }
		iterator		end()			{ return iterator{ ix_collection_t::get_interface(), ix_collection_t::Size() }; }
		const_iterator	end() const		{ return const_iterator{ ix_collection_t::get_interface(), ix_collection_t::Size() }; }
		//
		const_iterator	cbegin() const	{ return const_iterator{ ix_collection_t::get_interface(), 0 }; }
		const_iterator	cend()	const 	{ return const_iterator{ ix_collection_t::get_interface(), ix_collection_t::Size() }; }
		//
		friend iterator begin(Array& array_)				{ return array_.begin(); }
		friend const_iterator begin(const Array& array_)	{ return array_.begin(); }
		friend iterator end(Array& array_)					{ return array_.end(); }
		friend const_iterator end(const Array& array_)		{ return array_.end(); }
	};
#if ESB_USE_RUSSIAN
	using МассивЗначений = Array;
#endif


//TODO
//#define ESB_IMPLEMENT_IX_COLLECTION_ITERATORS(ESB_CLASS_T_)																		\
//	using value_type		= ix_collection_t::value_t;																			\
//	using iterator			= IxCollectionIterator<value_type>;																	\
//	using const_iterator	= IxCollectionIteratorConst<value_type, ix_collection_t::interface_t>;								\
//	iterator		begin()			{ return iterator{ ix_collection_t::get_interface(), 0 }; }									\
//	const_iterator	begin() const	{ return const_iterator{ ix_collection_t::get_interface(), 0 }; }							\
//	iterator		end()			{ return iterator{ ix_collection_t::get_interface(), ix_collection_t::Size() }; }			\
//	const_iterator	end() const		{ return const_iterator{ ix_collection_t::get_interface(), ix_collection_t::Size() }; }		\
//	const_iterator	cbegin() const	{ return const_iterator{ ix_collection_t::get_interface(), 0 }; }							\
//	const_iterator	cend()	const	{ return const_iterator{ ix_collection_t::get_interface(), ix_collection_t::Size() }; }		\
//	friend iterator			begin(ESB_CLASS_T_& container_)			{ return container_.begin(); }								\
//	friend const_iterator	begin(const ESB_CLASS_T_& container_)	{ return container_.begin(); }								\
//	friend iterator			end(ESB_CLASS_T_& container_)			{ return container_.end(); }								\
//	friend const_iterator	end(const ESB_CLASS_T_& container_)		{ return container_.end(); }
//

	class FixedArray : public ObjectDual<IxCollectionRO<Arbitrary>> {
		ESB_CLASS_IMPLEMENT_MAKE_OBJ_DUAL(FixedArray)
		ESB_CLASS_IMPLEMENT_MAKE_OPT(FixedArray)
		struct MethId { ESB_ID(Count, 0); ESB_ID(UBound, 1); ESB_ID(Get, 2); ESB_ID(Find, 3); };
	public:
		ESB_DECLARE_TYPEINFO()
		ESB_IMPLEMENT_ARGBYVAL_OPERATOR(FixedArray)
	public:
		static FixedArray  Create(ConstPara<Array> Array_)		{ ESB_INVOKE_CTOR1(FixedArray, Array_); }
	public:
		ESB_MEMB(Numeric)			Count()	const							{ ESB_INVOKE_MEMB_FUNC0(Numeric, MethId::Count); }
		ESB_MEMB(Numeric)			UBound() const							{ ESB_INVOKE_MEMB_FUNC0(Numeric, MethId::UBound); }
		ESB_MEMB(Arbitrary)			Get(ConstPara<Numeric> Index_) const		{ ESB_INVOKE_MEMB_FUNC1(Arbitrary, MethId::Get, Index_); }
		ESB_MEMB(UndefOr<Numeric>)	Find(ConstPara<Arbitrary> Value_) const	{ ESB_INVOKE_MEMB_FUNC1(UndefOr<Numeric>, MethId::Find, Value_); }
	public:
		ESB_IMPLEMENT_ENUM_ITEMS(FixedArray, Arbitrary)
	public:
		ESB_IMPLEMENT_INDEXED_COLLECTION_RO()
		using ix_collection_t::Size;
	public:
		using value_type = ix_collection_t::value_t;
		using const_iterator = IxCollectionIteratorConst<value_type, ix_collection_t::interface_t>;
		// Фиксированный массив отдает только const_iterator
		const_iterator	begin() const	{ return const_iterator{ ix_collection_t::get_interface(), 0 }; }
		const_iterator	end() const		{ return const_iterator{ ix_collection_t::get_interface(), ix_collection_t::Size() }; }
		//
		const_iterator	cbegin() const	{ return const_iterator{ ix_collection_t::get_interface(), 0 }; }
		const_iterator	cend()	const 	{ return const_iterator{ ix_collection_t::get_interface(), ix_collection_t::Size() }; }
		//
		friend const_iterator begin(const FixedArray& array_)	{ return array_.begin(); }
		friend const_iterator end(const FixedArray& array_)		{ return array_.end(); }
	};


	class KeyAndValue : public ValueDual<ValuePairAny> {
	protected:	// нас будет наследовать KeyAndValueForStructure поэтому дадим ему доступ к конструкторам
		ESB_CLASS_IMPLEMENT_MAKE_VAL_DUAL(KeyAndValue)
		ESB_CLASS_IMPLEMENT_MAKE_OPT(KeyAndValue)
		//TOBE	friend template partitial spec esb_class_maker_t. Как interface_checker так и maker
		//		(для AxCollectionPair::MakeKeyAndValue)
		friend KeyAndValue make_from_dual<KeyAndValue, IValuePairPtr>(IValuePairPtr&&);
	public:
		ESB_DECLARE_TYPEINFO(KeyAndValue)
		ESB_IMPLEMENT_ARGBYVAL_OPERATOR(KeyAndValue)
	public:
		// Not Creatable
	public:
		Arbitrary GetKey() const	{ return base_t::GetValue1st(); }
		ESB_PROPERTY_FIELD_RO(Arbitrary, Key);
		Arbitrary GetValue() const	{ return base_t::GetValue2nd(); }
		ESB_PROPERTY_FIELD_RO(Arbitrary, Value);
	public:
		explicit
		operator const ValuePairAny&() const { return static_cast<const ValuePairAny&>(*this); }
	};


	template<EsbClassConcept EsbClassKeyT, EsbClassConcept EsbClassValT>
	inline KeyAndValue AxCollectionPair<EsbClassKeyT, EsbClassValT>::MakeKeyAndValue() const { return make_from_dual<KeyAndValue>(interface_t{ base_t::get_interface() }); }


	class KeyAndValueForStructure : public KeyAndValue {
		using base_t = KeyAndValue;
		friend KeyAndValueForStructure make<KeyAndValueForStructure>(IValuePtr&&);
		KeyAndValueForStructure(IValuePtr&& val_, IValuePairPtr&& obj_) throw() : KeyAndValue(std::move(val_), std::move(obj_)) {}
		ESB_CLASS_IMPLEMENT_MAKE_OPT(KeyAndValueForStructure)
		// прячем Arbitrary KeyAndValue::GetKey;
		using KeyAndValue::GetKey;
	public:
		// тип-инфо и все прочее у нас от KeyAndValue т.к. по-сути нашего класса в системе 1С нет. мы "удобство есб".
	public:
		// перекрываем GetKey KeyAndValue. при необходимости остается возможность привести к KeyAndValue и получить ключ в виде Arbitrary 
		String GetKey() const { return KeyAndValue::GetKey().as_value<String>(); } //здесь будет мув помирающего Arbitrary как String
		ESB_PROPERTY_FIELD_RO(String, Key);
		// выносить свой в публичный интерфейс Value не обязательно т.к. мы и так публично унаследованы, но IDE не понимает и красным подсвечивает. (компилятор - ок)
		// поэтому еще раз покажем для IDE
		// (!!! и опять! нельзя выносить через using тот же самый GetValue что и у KeyAndValue::GetValue - крах компилятора !!!)
		//using KeyAndValue::GetValue;
		ESB_PROPERTY_FIELD_RO(Arbitrary, Value);
	};


	class Structure : public Object, protected DynamicCollectionImplRW<Structure, Arbitrary>
	{
		ESB_FRIEND_DYNAMIC_COLLECTION()
		friend Structure make<Structure>(IValuePtr&&);
		Structure(IValuePtr&& val_, IObjectPtr&& obj_) throw() : Object(std::move(val_), std::move(obj_)) {}
		struct MethId { ESB_ID(Count, 0); ESB_ID(Insert, 1); ESB_ID(Delete, 2); ESB_ID(Clear, 3); ESB_ID(Property, 4); };
	public:
		ESB_DECLARE_TYPEINFO()
		ESB_IMPLEMENT_ARGBYVAL_OPERATOR(Structure)
	public:
		template<std::same_as<Arbitrary>... ArbitraryT>
		static Structure Create(ConstPara<String> Keys_, ConstPara<ArbitraryT> ...Values_)	{ ESB_INVOKE_CTOR_PACK1(Structure, ConstPara<String>, Keys_, ConstPara<ArbitraryT>, Values_); }
		static Structure Create(ConstPara<FixedStructure> FixedStructure_)					{ ESB_INVOKE_CTOR1(Structure, FixedStructure_); }
		static Structure Create()															{ ESB_INVOKE_CTOR0(Structure); }
	public:
		Numeric	Count()	const																			{ ESB_INVOKE_MEMB_FUNC0(Numeric, MethId::Count); }
		void	Insert(ConstPara<String> Key_, ConstParaAny Value_ = DefUndef<Arbitrary>)				{ ESB_INVOKE_MEMB_PROC2(MethId::Insert, Key_, Value_); }
		void	Delete(ConstPara<String> Key_)															{ ESB_INVOKE_MEMB_PROC1(MethId::Delete, Key_); }
		void	Clear()																					{ ESB_INVOKE_MEMB_PROC0(MethId::Clear); }
		Boolean	Property(ConstPara<String> Key_, RefParaAny ValueFound_ = DefUndef<Arbitrary>) const	{ ESB_INVOKE_MEMB_FUNC2(Boolean, MethId::Property, Key_, ValueFound_); }
	public:
		ESB_IMPLEMENT_DYNAMIC_COLLECTION_RW()
	public:
		ESB_IMPLEMENT_ENUM_ITEMS(Structure, KeyAndValueForStructure)
	};


	class FixedStructure : public Object, protected DynamicCollectionImplRW<FixedStructure, Arbitrary> {
		ESB_FRIEND_DYNAMIC_COLLECTION()
		friend FixedStructure make<FixedStructure>(IValuePtr&&);
		FixedStructure(IValuePtr&& val_, IObjectPtr&& obj_) throw() : Object(std::move(val_), std::move(obj_)) {}
		struct MethId { ESB_ID(Count, 0); ESB_ID(Property, 1); };
	public:
		ESB_DECLARE_TYPEINFO()
		ESB_IMPLEMENT_ARGBYVAL_OPERATOR(FixedStructure)
	public:
		template<std::same_as<Arbitrary>... ArbitraryT>
		static FixedStructure  Create(ConstPara<String> Keys_, ConstPara<ArbitraryT> ...Values_)	{ ESB_INVOKE_CTOR_PACK1(FixedStructure, ConstPara<String>, Keys_, ConstPara<ArbitraryT>, Values_); }
		static FixedStructure  Create(ConstPara<Structure> Structure_)								{ ESB_INVOKE_CTOR1(FixedStructure, Structure_); }
		static FixedStructure  Create()																{ ESB_INVOKE_CTOR0(FixedStructure); }
	public:
		Numeric	 Count()	const																		{ ESB_INVOKE_MEMB_FUNC0(Numeric, MethId::Count); }
		Boolean	 Property(ConstPara<String> Key_, RefParaAny ValueFound_ = DefUndef<Arbitrary>) const	{ ESB_INVOKE_MEMB_FUNC2(Boolean, MethId::Property, Key_, ValueFound_); }
	public:
		ESB_IMPLEMENT_DYNAMIC_COLLECTION_RW()
	public:
		ESB_IMPLEMENT_ENUM_ITEMS(FixedStructure, KeyAndValueForStructure)
	};


	class Map : public ObjectDual<AxCollectionMap> {
		friend Map make<Map>(IValuePtr&&);
		Map(IValuePtr&& val_, IObjectPtr&& obj_, IAxCollectionPtr&& col_) throw() : ObjectDual(std::move(val_), std::move(obj_), std::move(col_)) {}
		struct MethId { ESB_ID(Count, 0); ESB_ID(Insert, 1); ESB_ID(Delete, 2); ESB_ID(Clear, 3); ESB_ID(Get, 4); };
	public:
		ESB_DECLARE_TYPEINFO()
		ESB_IMPLEMENT_ARGBYVAL_OPERATOR(Map)
	public:
		static Map  Create(ConstPara<FixedMap> FixedMap_)				{ ESB_INVOKE_CTOR1(Map, FixedMap_); }
		static Map  Create()											{ ESB_INVOKE_CTOR0(Map); } 
	public:
		Numeric		 Count()	const																		{ ESB_INVOKE_MEMB_FUNC0(Numeric, MethId::Count); }
		void		 Insert(ConstPara<Arbitrary> Key_, ConstPara<Arbitrary> Value_ = DefUndef<Arbitrary>)	{ ESB_INVOKE_MEMB_PROC2(MethId::Insert, Key_, Value_); }
		void		 Delete(ConstPara<Arbitrary> Key_)														{ ESB_INVOKE_MEMB_PROC1(MethId::Delete, Key_); }
		void		 Clear()																				{ ESB_INVOKE_MEMB_PROC0(MethId::Clear); }
		Arbitrary	 Get(ConstPara<Arbitrary> Key_) const													{ ESB_INVOKE_MEMB_FUNC1(Arbitrary, MethId::Get, Key_); }
	public:
		ESB_IMPLEMENT_ENUM_ITEMS(Map, KeyAndValue)
	public:
		using ax_collection_t::GetValue;
		using ax_collection_t::SetValue;
		using ax_collection_t::Value;
		using ax_collection_t::operator[];
		ax_collection_t& AsAssociativeCollection()				{ return *static_cast<ax_collection_t*>(this); }
		const ax_collection_t& AsAssociativeCollection() const	{ return *static_cast<const ax_collection_t*>(this); }
	};

#define ESB_IMPLEMENT_ASSOCIATIVE_COLLECTION_RO()																		\
	ax_collection_t& AsAssociativeCollection()				{ return *static_cast<ax_collection_t*>(this); }			\
	const ax_collection_t& AsAssociativeCollection() const	{ return *static_cast<const ax_collection_t*>(this); }		\
	using ax_collection_t::GetValue;		\
	using ax_collection_t::Value;			\
	using ax_collection_t::operator[];		



	// Элементами коллекции для Map и FixedMap являются объекты KeyAndValue (как и для структуры, только там ключ строго строка) - цитата из хелпа
	// Но это имеется в виду тип данных для енумерации коллекции в цикле (ESB_IMPLEMENT_ENUM_ITEMS...)
	// Оператор же [] возвращает не элемент коллекции, а соответствующее ключу значение. Это как-бы сбивает с толку....
	// Но логично. В самом деле внутри мапы и структуры никакие KeyAndValue не хранятся. Они создаются на лету при енумерации коллекции.
	// Еще раз - KeyAndValue - это объект только для енумерации.
	// Для оператора [] нам нужно возвращать некий класс-ацессор позволяющий получить\установить значение используя интерфейс мап\фиксдмап
	class FixedMap : public ObjectDual <AxCollectionMapRO> {
		friend FixedMap make<FixedMap>(IValuePtr&&);
		FixedMap(IValuePtr&& val_, IObjectPtr&& obj_, IAxCollectionROPtr&& axc_) throw() : ObjectDual(std::move(val_), std::move(obj_), std::move(axc_)) {}
		struct MethId { ESB_ID(Count, 0); ESB_ID(Get, 1); };
	public:
		ESB_DECLARE_TYPEINFO()
		ESB_IMPLEMENT_ARGBYVAL_OPERATOR(FixedMap)
	public:
		static FixedMap  Create(ConstPara<Map> Map_)		{ ESB_INVOKE_CTOR1(FixedMap, Map_); }
	public:
		Numeric		Count() const							{ ESB_INVOKE_MEMB_FUNC0(Numeric, MethId::Count); }
		Arbitrary	Get(ConstPara<Arbitrary> Key_)	const	{ ESB_INVOKE_MEMB_FUNC1(Arbitrary, MethId::Get, Key_); }
	public:
		ESB_IMPLEMENT_ENUM_ITEMS(FixedMap, KeyAndValue)
	public:
		ESB_IMPLEMENT_ASSOCIATIVE_COLLECTION_RO();
	};
}	//namespace esb 





// Все подключаемые енумераторы платформы (кроме RoundMode - он в core)
namespace esb {
// 
//TOBE: Нужно все таки либо буст подключить, либо все это в порядок привести. Ни зависимостей не хочется, ни возиться...
//		при расширении списка нумераторов конечно придется.
//
#define ESB_ENUM_OBJ_NAME(ENUM_NAME_)	ENUM_NAME_##Enum
#define ESB_ENUM_VAL_NAME(ENUM_NAME_)	ENUM_NAME_##Value

#define ESB_DECLARE_ENUM_IT_(ENUM_NAME_, IT_, N_)						static constexpr const ESB_ENUM_VAL_NAME(ENUM_NAME_) & IT_	{ _items[N_] };	
#define ESB_DECLARE_ENUM_IT1(ENUM_NAME_,_0)								ESB_DECLARE_ENUM_IT_(ENUM_NAME_,_0, 0)
#define ESB_DECLARE_ENUM_IT2(ENUM_NAME_,_0,_1)							ESB_DECLARE_ENUM_IT1(ENUM_NAME_,_0)								ESB_DECLARE_ENUM_IT_(ENUM_NAME_,_1,1)
#define ESB_DECLARE_ENUM_IT3(ENUM_NAME_,_0,_1,_2)						ESB_DECLARE_ENUM_IT2(ENUM_NAME_,_0,_1)							ESB_DECLARE_ENUM_IT_(ENUM_NAME_,_2,2)
#define ESB_DECLARE_ENUM_IT4(ENUM_NAME_,_0,_1,_2,_3)					ESB_DECLARE_ENUM_IT3(ENUM_NAME_,_0,_1,_2)						ESB_DECLARE_ENUM_IT_(ENUM_NAME_,_3,3)
#define ESB_DECLARE_ENUM_IT5(ENUM_NAME_,_0,_1,_2,_3,_4)					ESB_DECLARE_ENUM_IT4(ENUM_NAME_,_0,_1,_2,_3)					ESB_DECLARE_ENUM_IT_(ENUM_NAME_,_4,4)
#define ESB_DECLARE_ENUM_IT6(ENUM_NAME_,_0,_1,_2,_3,_4,_5)				ESB_DECLARE_ENUM_IT5(ENUM_NAME_,_0,_1,_2,_3,_4)					ESB_DECLARE_ENUM_IT_(ENUM_NAME_,_5,5)
#define ESB_DECLARE_ENUM_IT7(ENUM_NAME_,_0,_1,_2,_3,_4,_5,_6)			ESB_DECLARE_ENUM_IT6(ENUM_NAME_,_0,_1,_2,_3,_4,_5)				ESB_DECLARE_ENUM_IT_(ENUM_NAME_,_6,6)
#define ESB_DECLARE_ENUM_IT8(ENUM_NAME_,_0,_1,_2,_3,_4,_5,_6,_7)		ESB_DECLARE_ENUM_IT7(ENUM_NAME_,_0,_1,_2,_3,_4,_5,_6)			ESB_DECLARE_ENUM_IT_(ENUM_NAME_,_7,7)
#define ESB_DECLARE_ENUM_IT9(ENUM_NAME_,_0,_1,_2,_3,_4,_5,_6,_7,_8)		ESB_DECLARE_ENUM_IT8(ENUM_NAME_,_0,_1,_2,_3,_4,_5,_6,_7)		ESB_DECLARE_ENUM_IT_(ENUM_NAME_,_8,8)
#define ESB_DECLARE_ENUM_IT10(ENUM_NAME_,_0,_1,_2,_3,_4,_5,_6,_7,_8,_9)	ESB_DECLARE_ENUM_IT9(ENUM_NAME_,_0,_1,_2,_3,_4,_5,_6,_7,_8)		ESB_DECLARE_ENUM_IT_(ENUM_NAME_,_9,9)

#define ESB_DECLARE_ENUM_OBJ(ENUM_NAME_, N_)																							\
	class ESB_ENUM_OBJ_NAME(ENUM_NAME_) ;																								\
	using ESB_ENUM_VAL_NAME(ENUM_NAME_) = EnumValueImpl< ESB_ENUM_OBJ_NAME(ENUM_NAME_) , N_>;											\
	class ESB_ENUM_OBJ_NAME(ENUM_NAME_) : public EnumObjectImpl< ESB_ENUM_OBJ_NAME(ENUM_NAME_), ESB_ENUM_VAL_NAME(ENUM_NAME_), N_ > {	\
	public:

#define ESB_DECLARE_ENUM2(ENUM_NAME_,_0,_1)								ESB_DECLARE_ENUM_OBJ(ENUM_NAME_, 2)		ESB_DECLARE_ENUM_IT2(ENUM_NAME_,_0,_1)							};
#define ESB_DECLARE_ENUM3(ENUM_NAME_,_0,_1,_2)							ESB_DECLARE_ENUM_OBJ(ENUM_NAME_, 3)		ESB_DECLARE_ENUM_IT3(ENUM_NAME_,_0,_1,_2)						};
#define ESB_DECLARE_ENUM4(ENUM_NAME_,_0,_1,_2,_3)						ESB_DECLARE_ENUM_OBJ(ENUM_NAME_, 4)		ESB_DECLARE_ENUM_IT4(ENUM_NAME_,_0,_1,_2,_3)					};
#define ESB_DECLARE_ENUM5(ENUM_NAME_,_0,_1,_2,_3,_4)					ESB_DECLARE_ENUM_OBJ(ENUM_NAME_, 5)		ESB_DECLARE_ENUM_IT5(ENUM_NAME_,_0,_1,_2,_3,_4)					};
#define ESB_DECLARE_ENUM6(ENUM_NAME_,_0,_1,_2,_3,_4,_5)					ESB_DECLARE_ENUM_OBJ(ENUM_NAME_, 6)		ESB_DECLARE_ENUM_IT6(ENUM_NAME_,_0,_1,_2,_3,_4,_5)				};
#define ESB_DECLARE_ENUM7(ENUM_NAME_,_0,_1,_2,_3,_4,_5,_6)				ESB_DECLARE_ENUM_OBJ(ENUM_NAME_, 7)		ESB_DECLARE_ENUM_IT7(ENUM_NAME_,_0,_1,_2,_3,_4,_5,_6)			};
#define ESB_DECLARE_ENUM8(ENUM_NAME_,_0,_1,_2,_3,_4,_5,_6,_7)			ESB_DECLARE_ENUM_OBJ(ENUM_NAME_, 8)		ESB_DECLARE_ENUM_IT8(ENUM_NAME_,_0,_1,_2,_3,_4,_5,_6,_7)		};
#define ESB_DECLARE_ENUM9(ENUM_NAME_,_0,_1,_2,_3,_4,_5,_6,_7,_8)		ESB_DECLARE_ENUM_OBJ(ENUM_NAME_, 9)		ESB_DECLARE_ENUM_IT9(ENUM_NAME_,_0,_1,_2,_3,_4,_5,_6,_7,_8)		};
#define ESB_DECLARE_ENUM10(ENUM_NAME_,_0,_1,_2,_3,_4,_5,_6,_7,_8,_9)	ESB_DECLARE_ENUM_OBJ(ENUM_NAME_, 10)	ESB_DECLARE_ENUM_IT10(ENUM_NAME_,_0,_1,_2,_3,_4,_5,_6,_7,_8,_9)	};

// Если раскомментировать ниже, то добавится псевдоним с именем самого нумератора для значения нумератор
// если изменить на using ENUM_NAME_ = ESB_ENUM_OBJ_NAME(ENUM_NAME_); то псевдоним будет для класса енумератора 
// (см. заметку где объявление EnumValueImpl)
#define ESBLIB_ENUM(ENUM_NAME_, N_, ...)		PP_EXPAND( PP_CAT(ESB_DECLARE_ENUM,N_) (ENUM_NAME_ , __VA_ARGS__) )		//using ENUM_NAME_ = ESB_ENUM_VAL_NAME(ENUM_NAME_);
// модель
	ESBLIB_ENUM(SortDirection, 2, Asc, Desc)
// все прочие
#include "esblib_enums.inc"
// ------------------------
}	//namespace esb enums




namespace esb {
	//TOBE: или убрать алиас, или Array переименовать
	using ArrayOfValue = Array;

	// определены здесь (esb_main). ипользуются в апи и т.п.
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

	// определены в esb_file
	class BinaryData;
	class BinaryDataBuffer;
	class BinaryDataQualifiers;
	class Stream;
	class MemoryStream;
	class FileStream;

	// определены в esb_data
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


	//TOBE:		Разобраться с Create() помеченными		// no info, but present in context
	//			это пометка означает, что платформа рапортует что конструктор с указанным кол-вом параметров у класса есть
	//			а по прочей информации его нет. Чаще и в хелпе пишется что нет такого конструктора
	//			Как суслик, короче... Вызывать, проверять, исправлять...


	class CompareValues : public Object {
		ESB_CLASS_IMPLEMENT_MAKE_OBJ(CompareValues)
		ESB_CLASS_IMPLEMENT_MAKE_OPT(CompareValues)
		struct MethId { ESB_ID(Compare, 0); };
	public:
		ESB_DECLARE_TYPEINFO(CompareValues)
		ESB_IMPLEMENT_ARGBYVAL_OPERATOR(CompareValues)
	public:
		static CompareValues  Create() { ESB_INVOKE_CTOR0(CompareValues); }  // no info, but present in context
	public:
		// Функция Сравнить(Значение1_ Как Произвольный, Значение2_ Как Произвольный) Возвращает Число
		Numeric Compare(ConstPara<Arbitrary> Value1_, ConstPara<Arbitrary> Value2_) const { ESB_INVOKE_MEMB_FUNC2(Numeric, MethId::Compare, Value1_, Value2_); }
	};


	class NumberQualifiers : public Object {
		ESB_CLASS_IMPLEMENT_MAKE_OBJ(NumberQualifiers)
		ESB_CLASS_IMPLEMENT_MAKE_OPT(NumberQualifiers)
		struct PropId { ESB_ID(Digits, 0); ESB_ID(FractionDigits, 1); ESB_ID(AllowedSign, 2); };
	public:
		ESB_DECLARE_TYPEINFO(NumberQualifiers)
		ESB_IMPLEMENT_ARGBYVAL_OPERATOR(NumberQualifiers)
	public:
		static NumberQualifiers Create() { ESB_INVOKE_CTOR0(NumberQualifiers); }  // no info, but present in context - ??? что это будет без параметров?
		// для с++ лучше не делать опт-параметры в конце. оно их в центре не понимает. лучше просто перегрузку методов, а с опт-значениями 1С само разберется
		// (тем более не DefUndef оно, а конкретные значения и только в хелпе это написано)
		//static NumberQualifiers Create(ConstPara<Numeric> NumberOfDigits_ = DefUndef<Numeric>, ConstPara<Numeric> NumberOfDigitsAfterTheDecimalPoint_ = DefUndef<Numeric>, ConstPara<AllowedSign> AllowedSign_ = DefUndef<esb::AllowedSign>) { ESB_INVOKE_CTOR3(NumberQualifiers, NumberOfDigits_, NumberOfDigitsAfterTheDecimalPoint_, AllowedSign_); }
		//static NumberQualifiers Create(ConstPara<Numeric> NumberOfDigits_ = Numeric::Value_0_, ConstPara<Numeric> NumberOfDigitsAfterTheDecimalPoint_ = Numeric::Value_0_, ConstPara<AllowedSign> AllowedSign_ = esb::AllowedSignEnum::Any) { ESB_INVOKE_CTOR3(NumberQualifiers, NumberOfDigits_, NumberOfDigitsAfterTheDecimalPoint_, AllowedSign_); }
		static NumberQualifiers Create(ConstPara<Numeric> NumberOfDigits_) { ESB_INVOKE_CTOR1(NumberQualifiers, NumberOfDigits_); }
		static NumberQualifiers Create(ConstPara<Numeric> NumberOfDigits_, ConstPara<Numeric> NumberOfDigitsAfterTheDecimalPoint_) { ESB_INVOKE_CTOR2(NumberQualifiers, NumberOfDigits_, NumberOfDigitsAfterTheDecimalPoint_); }
		static NumberQualifiers Create(ConstPara<Numeric> NumberOfDigits_, ConstPara<Numeric> NumberOfDigitsAfterTheDecimalPoint_, ConstPara<AllowedSignValue> AllowedSign_) { ESB_INVOKE_CTOR3(NumberQualifiers, NumberOfDigits_, NumberOfDigitsAfterTheDecimalPoint_, AllowedSign_); }
	public:
		// Свойство [[Только Чтение]] Разрядность Как Число
		Numeric GetDigits()	const				{ ESB_INVOKE_PROPERTY_GET(esb::Numeric, PropId::Digits); }
		ESB_PROPERTY_FIELD_RO(esb::Numeric, Digits);
		// Свойство [[Только Чтение]] РазрядностьДробнойЧасти Как Число
		Numeric GetFractionDigits()	const		{ ESB_INVOKE_PROPERTY_GET(esb::Numeric, PropId::FractionDigits); }
		ESB_PROPERTY_FIELD_RO(esb::Numeric, FractionDigits);
		// Свойство [[Только Чтение]] ДопустимыйЗнак Как ДопустимыйЗнак
		AllowedSignValue GetAllowedSign() const { ESB_INVOKE_PROPERTY_GET(esb::AllowedSignValue, PropId::AllowedSign); }
		ESB_PROPERTY_FIELD_RO(esb::AllowedSignValue, AllowedSign);
	};


	class StringQualifiers : public Object {
		ESB_CLASS_IMPLEMENT_MAKE_OBJ(StringQualifiers)
		ESB_CLASS_IMPLEMENT_MAKE_OPT(StringQualifiers)
		struct PropId { ESB_ID(Length, 0); ESB_ID(AllowedLength, 1); };
	public:
		ESB_DECLARE_TYPEINFO(StringQualifiers)
		ESB_IMPLEMENT_ARGBYVAL_OPERATOR(StringQualifiers)
	public:
		static StringQualifiers Create() { ESB_INVOKE_CTOR0(StringQualifiers); }  // no info, but present in context
		//Вариант ограничения длины строки, если задана длина строки, отличная от 0. Значение по умолчанию : Переменная.
		static StringQualifiers Create(ConstPara<Numeric> StringLength_, ConstPara<AllowedLengthValue> AllowedLength_ = esb::AllowedLengthEnum::Variable) { ESB_INVOKE_CTOR2(StringQualifiers, StringLength_, AllowedLength_); }
	public:
		// Свойство [[Только Чтение]] Длина Как Число
		Numeric GetLength()	const { ESB_INVOKE_PROPERTY_GET(esb::Numeric, PropId::Length); }
		ESB_PROPERTY_FIELD_RO(esb::Numeric, Length);
		// Свойство [[Только Чтение]] ДопустимаяДлина Как ДопустимаяДлина
		AllowedLengthValue GetAllowedLength() const { ESB_INVOKE_PROPERTY_GET(esb::AllowedLengthValue, PropId::AllowedLength); }
		ESB_PROPERTY_FIELD_RO(esb::AllowedLengthValue, AllowedLength);
	};


	class DateQualifiers : public Object {
		ESB_CLASS_IMPLEMENT_MAKE_OBJ(DateQualifiers)
		ESB_CLASS_IMPLEMENT_MAKE_OPT(DateQualifiers)
		struct PropId { ESB_ID(DateFractions, 0); };
	public:
		ESB_DECLARE_TYPEINFO(DateQualifiers)
		ESB_IMPLEMENT_ARGBYVAL_OPERATOR(DateQualifiers)
	public:
		static DateQualifiers Create() { ESB_INVOKE_CTOR0(DateQualifiers); }  // no info, but present in context
		static DateQualifiers Create(ConstPara<DateFractionsValue> DateFractions_) { ESB_INVOKE_CTOR1(DateQualifiers, DateFractions_); }
	public:
		// Свойство [[Только Чтение]] ЧастиДаты Как ЧастиДаты
		DateFractionsValue GetDateFractions() const { ESB_INVOKE_PROPERTY_GET(esb::DateFractionsValue, PropId::DateFractions); }
		ESB_PROPERTY_FIELD_RO(esb::DateFractionsValue, DateFractions);
	};


	class BinaryDataQualifiers : public Object {
		ESB_CLASS_IMPLEMENT_MAKE_OBJ(BinaryDataQualifiers)
		ESB_CLASS_IMPLEMENT_MAKE_OPT(BinaryDataQualifiers)
		struct PropId { ESB_ID(Length, 0); ESB_ID(AllowedLength, 1); };
	public:
		ESB_DECLARE_TYPEINFO(BinaryDataQualifiers)
		ESB_IMPLEMENT_ARGBYVAL_OPERATOR(BinaryDataQualifiers)
	public:
		static BinaryDataQualifiers Create() { ESB_INVOKE_CTOR0(BinaryDataQualifiers); }  // no info, but present in context
		static BinaryDataQualifiers Create(ConstPara<Numeric> Length_, ConstPara<AllowedLengthValue> AllowedLength_ = esb::AllowedLengthEnum::Variable) { ESB_INVOKE_CTOR2(BinaryDataQualifiers, Length_, AllowedLength_); }
	public:
		// Свойство [[Только Чтение]] Длина Как Число
		Numeric GetLength()	const					{ ESB_INVOKE_PROPERTY_GET(esb::Numeric, PropId::Length); }
		ESB_PROPERTY_FIELD_RO(esb::Numeric, Length);
		// Свойство [[Только Чтение]] ДопустимаяДлина Как ДопустимаяДлина
		AllowedLengthValue GetAllowedLength() const { ESB_INVOKE_PROPERTY_GET(esb::AllowedLengthValue, PropId::AllowedLength); }
		ESB_PROPERTY_FIELD_RO(esb::AllowedLengthValue, AllowedLength);
	};


	class TypeDescription : public Object {
		ESB_CLASS_IMPLEMENT_MAKE_OBJ(TypeDescription)
		ESB_CLASS_IMPLEMENT_MAKE_OPT(TypeDescription)
		struct MethId { ESB_ID(Types, 0); ESB_ID(ContainsType, 1); ESB_ID(AdjustValue, 2); };
		struct PropId { ESB_ID(NumberQualifiers, 0); ESB_ID(StringQualifiers, 1); ESB_ID(DateQualifiers, 2); ESB_ID(BinaryDataQualifiers, 3); };
	public:
		ESB_DECLARE_TYPEINFO(TypeDescription)
		ESB_IMPLEMENT_ARGBYVAL_OPERATOR(TypeDescription)
	public:
		// Это генератр
		// static TypeDescription Create() { ESB_INVOKE_CTOR0(TypeDescription); }  // no info, but present in context
		static TypeDescription Create(ConstPara<TypeDescription> SourceDescriptionOfTypes_, ConstPara<Array> AddedTypes_, ConstPara<Array> RemovedTypes_, ConstPara<NumberQualifiers> NumberQualifiers_, ConstPara<StringQualifiers> StringQualifiers_, ConstPara<DateQualifiers> DateQualifiers_, ConstPara<BinaryDataQualifiers> BinaryDataQualifiers_ = DefUndef<esb::BinaryDataQualifiers>) { ESB_INVOKE_CTOR7(TypeDescription, SourceDescriptionOfTypes_, AddedTypes_, RemovedTypes_, NumberQualifiers_, StringQualifiers_, DateQualifiers_, BinaryDataQualifiers_); }
		static TypeDescription Create(ConstPara<TypeDescription> SourceDescriptionOfTypes_, ConstPara<Array> AddedTypes_, ConstPara<String> RemovedTypes_, ConstPara<NumberQualifiers> NumberQualifiers_, ConstPara<StringQualifiers> StringQualifiers_, ConstPara<DateQualifiers> DateQualifiers_, ConstPara<BinaryDataQualifiers> BinaryDataQualifiers_ = DefUndef<esb::BinaryDataQualifiers>) { ESB_INVOKE_CTOR7(TypeDescription, SourceDescriptionOfTypes_, AddedTypes_, RemovedTypes_, NumberQualifiers_, StringQualifiers_, DateQualifiers_, BinaryDataQualifiers_); }
		static TypeDescription Create(ConstPara<TypeDescription> SourceDescriptionOfTypes_, ConstPara<String> AddedTypes_, ConstPara<Array> RemovedTypes_, ConstPara<NumberQualifiers> NumberQualifiers_, ConstPara<StringQualifiers> StringQualifiers_, ConstPara<DateQualifiers> DateQualifiers_, ConstPara<BinaryDataQualifiers> BinaryDataQualifiers_ = DefUndef<esb::BinaryDataQualifiers>) { ESB_INVOKE_CTOR7(TypeDescription, SourceDescriptionOfTypes_, AddedTypes_, RemovedTypes_, NumberQualifiers_, StringQualifiers_, DateQualifiers_, BinaryDataQualifiers_); }
		static TypeDescription Create(ConstPara<TypeDescription> SourceDescriptionOfTypes_, ConstPara<String> AddedTypes_, ConstPara<String> RemovedTypes_, ConstPara<NumberQualifiers> NumberQualifiers_, ConstPara<StringQualifiers> StringQualifiers_, ConstPara<DateQualifiers> DateQualifiers_, ConstPara<BinaryDataQualifiers> BinaryDataQualifiers_ = DefUndef<esb::BinaryDataQualifiers>) { ESB_INVOKE_CTOR7(TypeDescription, SourceDescriptionOfTypes_, AddedTypes_, RemovedTypes_, NumberQualifiers_, StringQualifiers_, DateQualifiers_, BinaryDataQualifiers_); }
		static TypeDescription Create(ConstPara<Array> Types_, ConstPara<NumberQualifiers> NumberQualifiers_ = DefUndef<esb::NumberQualifiers>, ConstPara<StringQualifiers> StringQualifiers_ = DefUndef<esb::StringQualifiers>, ConstPara<DateQualifiers> DateQualifiers_ = DefUndef<esb::DateQualifiers>, ConstPara<BinaryDataQualifiers> BinaryDataQualifiers_ = DefUndef<esb::BinaryDataQualifiers>) { ESB_INVOKE_CTOR5(TypeDescription, Types_, NumberQualifiers_, StringQualifiers_, DateQualifiers_, BinaryDataQualifiers_); }
		static TypeDescription Create(ConstPara<String> Types_, ConstPara<NumberQualifiers> NumberQualifiers_ = DefUndef<esb::NumberQualifiers>, ConstPara<StringQualifiers> StringQualifiers_ = DefUndef<esb::StringQualifiers>, ConstPara<DateQualifiers> DateQualifiers_ = DefUndef<esb::DateQualifiers>, ConstPara<BinaryDataQualifiers> BinaryDataQualifiers_ = DefUndef<esb::BinaryDataQualifiers>) { ESB_INVOKE_CTOR5(TypeDescription, Types_, NumberQualifiers_, StringQualifiers_, DateQualifiers_, BinaryDataQualifiers_); }
	public:
		// Функция Типы() Возвращает Массив
		Array Types() const { ESB_INVOKE_MEMB_FUNC0(Array, MethId::Types); }
		// Функция СодержитТип(Тип_ Как Тип) Возвращает Булево
		Boolean ContainsType(ConstPara<TypeValue> type_) const { ESB_INVOKE_MEMB_FUNC1(Boolean, MethId::ContainsType, type_); }
		// Функция ПривестиЗначение(Значение_ Как Произвольный) Возвращает Произвольный
		Arbitrary AdjustValue(ConstPara<Arbitrary> Value_) const { ESB_INVOKE_MEMB_FUNC1(Arbitrary, MethId::AdjustValue, Value_); }
	public:
		// Свойство [[Только Чтение]] КвалификаторыЧисла Как КвалификаторыЧисла
		NumberQualifiers GetNumberQualifiers() const { ESB_INVOKE_PROPERTY_GET(esb::NumberQualifiers, PropId::NumberQualifiers); }
		ESB_PROPERTY_FIELD_RO(esb::NumberQualifiers, NumberQualifiers);
		// Свойство [[Только Чтение]] КвалификаторыСтроки Как КвалификаторыСтроки
		StringQualifiers GetStringQualifiers() const { ESB_INVOKE_PROPERTY_GET(esb::StringQualifiers, PropId::StringQualifiers); }
		ESB_PROPERTY_FIELD_RO(esb::StringQualifiers, StringQualifiers);
		// Свойство [[Только Чтение]] КвалификаторыДаты Как КвалификаторыДаты
		DateQualifiers GetDateQualifiers() const { ESB_INVOKE_PROPERTY_GET(esb::DateQualifiers, PropId::DateQualifiers); }
		ESB_PROPERTY_FIELD_RO(esb::DateQualifiers, DateQualifiers);
		// Свойство [[Только Чтение]] КвалификаторыДвоичныхДанных Как КвалификаторыДвоичныхДанных
		BinaryDataQualifiers GetBinaryDataQualifiers() const { ESB_INVOKE_PROPERTY_GET(esb::BinaryDataQualifiers, PropId::BinaryDataQualifiers); }
		ESB_PROPERTY_FIELD_RO(esb::BinaryDataQualifiers, BinaryDataQualifiers);
	};


	class CommandInterfaceCommand : public Object {
		ESB_CLASS_IMPLEMENT_MAKE_OBJ(CommandInterfaceCommand)
		ESB_CLASS_IMPLEMENT_MAKE_OPT(CommandInterfaceCommand)
		struct PropId { ESB_ID(ThisObject, 0); };
	public:
		ESB_DECLARE_TYPEINFO(CommandInterfaceCommand)
		ESB_IMPLEMENT_ARGBYVAL_OPERATOR(CommandInterfaceCommand)
	public:
		static CommandInterfaceCommand Create() { ESB_INVOKE_CTOR0(CommandInterfaceCommand); }  // no info, but present in context
	public:
		// Свойство [[Только Чтение]] ЭтотОбъект Как КомандаКомандногоИнтерфейса
		CommandInterfaceCommand GetThisObject() const { ESB_INVOKE_PROPERTY_GET(esb::CommandInterfaceCommand, PropId::ThisObject); }
		ESB_PROPERTY_FIELD_RO(esb::CommandInterfaceCommand, ThisObject);
	};


	class CommonModule : public Object {
		ESB_CLASS_IMPLEMENT_MAKE_OBJ(CommonModule)
		ESB_CLASS_IMPLEMENT_MAKE_OPT(CommonModule)
	public:
		ESB_DECLARE_TYPEINFO(CommonModule)
		ESB_IMPLEMENT_ARGBYVAL_OPERATOR(CommonModule)
	public:
		// Not Creatable
		//-METH Extra!:NameOfAProcedureOrAFunction
		//-PROP Extra!:ThisObject
	};


	class NotifyDescription : public Object {
		ESB_CLASS_IMPLEMENT_MAKE_OBJ(NotifyDescription)
		ESB_CLASS_IMPLEMENT_MAKE_OPT(NotifyDescription)
		struct PropId { ESB_ID(ProcedureName, 0); ESB_ID(Module, 1); ESB_ID(AdditionalParameters, 2); ESB_ID(ErrorHandlerProcedureName, 3); ESB_ID(ErrorHandlerModule, 4); };
	public:
		ESB_DECLARE_TYPEINFO(NotifyDescription)
		ESB_IMPLEMENT_ARGBYVAL_OPERATOR(NotifyDescription)
	public:
		//NOTE ручками, генератор дурит
		static NotifyDescription Create(ConstPara<String> ProcedureName_, ConstPara<CommandInterfaceCommand> Module_, ConstPara<Arbitrary> AdditionalParameters_ = DefUndef<Arbitrary>, ConstPara<String> ErrorHandlerProcedureName_ = DefUndef<String>, ConstPara<CommandInterfaceCommand> ErrorHandlerModule_ = DefUndef<CommandInterfaceCommand>) { ESB_INVOKE_CTOR5(NotifyDescription, ProcedureName_, Module_, AdditionalParameters_, ErrorHandlerProcedureName_, ErrorHandlerModule_); }
		static NotifyDescription Create(ConstPara<String> ProcedureName_, ConstPara<CommandInterfaceCommand> Module_, ConstPara<Arbitrary> AdditionalParameters_ = DefUndef<Arbitrary>, ConstPara<String> ErrorHandlerProcedureName_ = DefUndef<String>, ConstPara<CommonModule> ErrorHandlerModule_ = DefUndef<CommonModule>) { ESB_INVOKE_CTOR5(NotifyDescription, ProcedureName_, Module_, AdditionalParameters_, ErrorHandlerProcedureName_, ErrorHandlerModule_); }
		static NotifyDescription Create(ConstPara<String> ProcedureName_, ConstPara<CommonModule> Module_, ConstPara<Arbitrary> AdditionalParameters_ = DefUndef<Arbitrary>, ConstPara<String> ErrorHandlerProcedureName_ = DefUndef<String>, ConstPara<CommandInterfaceCommand> ErrorHandlerModule_ = DefUndef<CommandInterfaceCommand>) { ESB_INVOKE_CTOR5(NotifyDescription, ProcedureName_, Module_, AdditionalParameters_, ErrorHandlerProcedureName_, ErrorHandlerModule_); }
		static NotifyDescription Create(ConstPara<String> ProcedureName_, ConstPara<CommonModule> Module_, ConstPara<Arbitrary> AdditionalParameters_ = DefUndef<Arbitrary>, ConstPara<String> ErrorHandlerProcedureName_ = DefUndef<String>, ConstPara<CommonModule> ErrorHandlerModule_ = DefUndef<CommonModule>) { ESB_INVOKE_CTOR5(NotifyDescription, ProcedureName_, Module_, AdditionalParameters_, ErrorHandlerProcedureName_, ErrorHandlerModule_); }
		static NotifyDescription Create(ConstPara<String> ProcedureName_, ConstPara<String> Module_, ConstPara<Arbitrary> AdditionalParameters_ = DefUndef<Arbitrary>, ConstPara<String> ErrorHandlerProcedureName_ = DefUndef<String>, ConstPara<CommandInterfaceCommand> ErrorHandlerModule_ = DefUndef<CommandInterfaceCommand>) { ESB_INVOKE_CTOR5(NotifyDescription, ProcedureName_, Module_, AdditionalParameters_, ErrorHandlerProcedureName_, ErrorHandlerModule_); }
		static NotifyDescription Create(ConstPara<String> ProcedureName_, ConstPara<String> Module_, ConstPara<Arbitrary> AdditionalParameters_ = DefUndef<Arbitrary>, ConstPara<String> ErrorHandlerProcedureName_ = DefUndef<String>, ConstPara<CommonModule> ErrorHandlerModule_ = DefUndef<CommonModule>) { ESB_INVOKE_CTOR5(NotifyDescription, ProcedureName_, Module_, AdditionalParameters_, ErrorHandlerProcedureName_, ErrorHandlerModule_); }
	public:
		// Свойство [[Только Чтение]] ИмяПроцедуры Как Строка
		String GetProcedureName() const { ESB_INVOKE_PROPERTY_GET(esb::String, PropId::ProcedureName); }
		ESB_PROPERTY_FIELD_RO(esb::String, ProcedureName);
		// Свойство [[Только Чтение]] Модуль Как [КомандаКомандногоИнтерфейса,ОбщийМодуль]
		ClassMix<CommandInterfaceCommand, CommonModule> GetModule() const { ESB_INVOKE_PROPERTY_GET(ESB_MIX2(CommandInterfaceCommand, CommonModule), PropId::Module); }
		// Свойство [[Только Чтение]] ДополнительныеПараметры Как Произвольный
		Arbitrary GetAdditionalParameters() const { ESB_INVOKE_PROPERTY_GET(esb::Arbitrary, PropId::AdditionalParameters); }
		ESB_PROPERTY_FIELD_RO(esb::Arbitrary, AdditionalParameters);
		// Свойство [[Только Чтение]] ИмяПроцедурыОбработкиОшибки Как [Строка,Неопределено]
		UndefOr<String> GetErrorHandlerProcedureName() const { ESB_INVOKE_PROPERTY_GET(UndefOr<String>, PropId::ErrorHandlerProcedureName); }
		// Свойство [[Только Чтение]] МодульОбработкиОшибки Как [КомандаКомандногоИнтерфейса,ОбщийМодуль]
		ClassMix<CommandInterfaceCommand, CommonModule> GetErrorHandlerModule() const { ESB_INVOKE_PROPERTY_GET(ESB_MIX2(CommandInterfaceCommand, CommonModule), PropId::ErrorHandlerModule); }
	};


	class Border : public Object {
		ESB_CLASS_IMPLEMENT_MAKE_OBJ(Border)
		ESB_CLASS_IMPLEMENT_MAKE_OPT(Border)
		struct PropId { ESB_ID(BorderType, 0); ESB_ID(Width, 1); ESB_ID(Type, 2); };
	public:
		ESB_DECLARE_TYPEINFO(Border)
		ESB_IMPLEMENT_ARGBYVAL_OPERATOR(Border)
	public:
		static Border Create(ConstPara<ControlBorderTypeValue> BorderType_, ConstPara<Numeric> Thickness_ = DefUndef<esb::Numeric>) { ESB_INVOKE_CTOR2(Border, BorderType_, Thickness_); }
	public:
		// Свойство [[Только Чтение]] ТипРамки Как ТипРамкиЭлементаУправления
		ControlBorderTypeValue GetBorderType() const { ESB_INVOKE_PROPERTY_GET(esb::ControlBorderTypeValue, PropId::BorderType); }
		ESB_PROPERTY_FIELD_RO(esb::ControlBorderTypeValue, BorderType);
		// Свойство [[Только Чтение]] Толщина Как Число
		Numeric GetWidth() const { ESB_INVOKE_PROPERTY_GET(esb::Numeric, PropId::Width); }
		ESB_PROPERTY_FIELD_RO(esb::Numeric, Width);
		// Свойство [[Только Чтение]] Вид Как ВидРамки
		esb::BorderTypeValue GetType() const { ESB_INVOKE_PROPERTY_GET(esb::BorderTypeValue, PropId::Type); }
		ESB_PROPERTY_FIELD_RO(esb::BorderTypeValue, Type);
	};


	class Color : public Object {
		ESB_CLASS_IMPLEMENT_MAKE_OBJ(Color)
		ESB_CLASS_IMPLEMENT_MAKE_OPT(Color)
		struct PropId { ESB_ID(R, 0); ESB_ID(G, 1); ESB_ID(B, 2); ESB_ID(Type, 3); };
	public:
		ESB_DECLARE_TYPEINFO(Color)
		ESB_IMPLEMENT_ARGBYVAL_OPERATOR(Color)
	public:
		static Color Create() { ESB_INVOKE_CTOR0(Color); }  // no info, but present in context
		static Color Create(ConstPara<Numeric> Red_, ConstPara<Numeric> Green_, ConstPara<Numeric> Blue_) { ESB_INVOKE_CTOR3(Color, Red_, Green_, Blue_); }
	public:
		// Свойство [[Только Чтение]] Красный Как Число
		Numeric GetR() const { ESB_INVOKE_PROPERTY_GET(esb::Numeric, PropId::R); }
		ESB_PROPERTY_FIELD_RO(esb::Numeric, R);
		// Свойство [[Только Чтение]] Зеленый Как Число
		Numeric GetG() const { ESB_INVOKE_PROPERTY_GET(esb::Numeric, PropId::G); }
		ESB_PROPERTY_FIELD_RO(esb::Numeric, G);
		// Свойство [[Только Чтение]] Синий Как Число
		Numeric GetB() const { ESB_INVOKE_PROPERTY_GET(esb::Numeric, PropId::B); }
		ESB_PROPERTY_FIELD_RO(esb::Numeric, B);
		// Свойство [[Только Чтение]] Вид Как ВидЦвета
		ColorTypeValue GetType() const { ESB_INVOKE_PROPERTY_GET(esb::ColorTypeValue, PropId::Type); }
		ESB_PROPERTY_FIELD_RO(esb::ColorTypeValue, Type);
	};


	class Picture : public Object {
		ESB_CLASS_IMPLEMENT_MAKE_OBJ(Picture)
		ESB_CLASS_IMPLEMENT_MAKE_OPT(Picture)
		struct MethId { ESB_ID(Write, 0); ESB_ID(FileNameFilter, 1); ESB_ID(Convert, 2); ESB_ID(Format, 3); ESB_ID(GetBinaryData, 4); };
		struct PropId { ESB_ID(Type, 0); ESB_ID(VariantSet, 1); };
	public:
		ESB_DECLARE_TYPEINFO(Picture)
			ESB_IMPLEMENT_ARGBYVAL_OPERATOR(Picture)
	public:
		static Picture Create() { ESB_INVOKE_CTOR0(Picture); }  // no info, but present in context
		static Picture Create(ConstPara<String> FileName_, ConstPara<Boolean> Transparent_ = DefUndef<esb::Boolean>) { ESB_INVOKE_CTOR2(Picture, FileName_, Transparent_); }
		static Picture Create(ConstPara<BinaryData> BinaryData_, ConstPara<Boolean> Transparent_ = DefUndef<esb::Boolean>) { ESB_INVOKE_CTOR2(Picture, BinaryData_, Transparent_); }
	public:
		// Процедура Записать(ИмяФайла_ Как Строка)
		void Write(ConstPara<String> FileName_) const		{ ESB_INVOKE_MEMB_PROC1(MethId::Write, FileName_); }
		// Процедура Записать(Поток_ Как [Поток,ПотокВПамяти,ФайловыйПоток])
		void Write(ConstPara<Stream> Stream_) const			{ ESB_INVOKE_MEMB_PROC1(MethId::Write, Stream_); }
		void Write(ConstPara<MemoryStream> Stream_) const	{ ESB_INVOKE_MEMB_PROC1(MethId::Write, Stream_); }
		void Write(ConstPara<FileStream> Stream_) const		{ ESB_INVOKE_MEMB_PROC1(MethId::Write, Stream_); }
		// Функция ФильтрИменФайлов(ВсеВарианты_ Как Булево, ПодобратьВариант_ Как [Соответствие,Неопределено]) Возвращает Строка
		//-METH: -para-size-mismatch: String FileNameFilter(Boolean AllVariants_, [Map,Undefined] SelectVariant_),  ctx_meth_.para_.size == 0, impl: MethId::FileNameFilter
		// Функция Преобразовать(ФорматКартинки_ Как ФорматКартинки, ВсеВарианты_ Как Булево, ПодобратьВариант_ Как [Соответствие,Неопределено]) Возвращает [Картинка,Неопределено]
		//-METH: -para-size-mismatch: [Picture,Undefined] Convert(PictureFormat PictureFormat_, Boolean AllVariants_, [Map,Undefined] SelectVariant_),  ctx_meth_.para_.size == 1, impl: MethId::Convert
		// Функция Формат() Возвращает ФорматКартинки
		PictureFormatValue Format() const					{ ESB_INVOKE_MEMB_FUNC0(PictureFormatValue, MethId::Format); }
		// Функция ПолучитьДвоичныеДанные(ВсеВарианты_ Как Булево, ПодобратьВариант_ Как [Соответствие,Неопределено]) Возвращает [ДвоичныеДанные,Неопределено]
		//-METH: -para-size-mismatch: [BinaryData,Undefined] GetBinaryData(Boolean AllVariants_, [Map,Undefined] SelectVariant_),  ctx_meth_.para_.size == 0, impl: MethId::GetBinaryData
	public:
		// Свойство [[Только Чтение]] Вид Как ВидКартинки
		PictureTypeValue GetType() const { ESB_INVOKE_PROPERTY_GET(esb::PictureTypeValue, PropId::Type); }
		ESB_PROPERTY_FIELD_RO(esb::PictureTypeValue, Type);
		// Свойство [[Только Чтение]] НаборВариантов Как Булево
		Boolean GetVariantSet() const { ESB_INVOKE_PROPERTY_GET(esb::Boolean, PropId::VariantSet); }
		ESB_PROPERTY_FIELD_RO(esb::Boolean, VariantSet);
	};


	class Font : public Object {
		ESB_CLASS_IMPLEMENT_MAKE_OBJ(Font)
		ESB_CLASS_IMPLEMENT_MAKE_OPT(Font)
		struct PropId { ESB_ID(Name, 0); ESB_ID(Size, 1); ESB_ID(Bold, 2); ESB_ID(Italic, 3); ESB_ID(Underline, 4); ESB_ID(Strikeout, 5); ESB_ID(Type, 6); ESB_ID(Scale, 7); };
	public:
		ESB_DECLARE_TYPEINFO(Font)
		ESB_IMPLEMENT_ARGBYVAL_OPERATOR(Font)
	public:
		static Font Create() { ESB_INVOKE_CTOR0(Font); }  // no info, but present in context
		// ручками, генератор дурит
		static Font Create(ConstPara<String> FontName_ = DefUndef<String>, ConstPara<Numeric> Size_ = DefUndef<Numeric>, ConstPara<Boolean> Bold_ = DefUndef<Boolean>, ConstPara<Boolean> Italics_ = DefUndef<Boolean>, ConstPara<Boolean> Underscore_ = DefUndef<Boolean>, ConstPara<Boolean> StrikeOut_ = DefUndef<Boolean>, ConstPara<Numeric> Scale_ = DefUndef<Numeric>) { ESB_INVOKE_CTOR7(Font, FontName_, Size_, Bold_, Italics_, Underscore_, StrikeOut_, Scale_); }
		static Font Create(ConstPara<Font> Font_, ConstPara<String> FontName_ = DefUndef<String>, ConstPara<Numeric> Size_ = DefUndef<Numeric>, ConstPara<Boolean> Bold_ = DefUndef<Boolean>, ConstPara<Boolean> Italics_ = DefUndef<Boolean>, ConstPara<Boolean> Underscore_ = DefUndef<Boolean>, ConstPara<Boolean> StrikeOut_ = DefUndef<Boolean>, ConstPara<Numeric> Scale_ = DefUndef<Numeric>) { ESB_INVOKE_CTOR8(Font, Font_, FontName_, Size_, Bold_, Italics_, Underscore_, StrikeOut_, Scale_); }
	public:
		// Свойство [[Только Чтение]] Имя Как Строка
		String GetName() const { ESB_INVOKE_PROPERTY_GET(esb::String, PropId::Name); }
		ESB_PROPERTY_FIELD_RO(esb::String, Name);
		// Свойство [[Только Чтение]] Размер Как Число
		Numeric GetSize() const { ESB_INVOKE_PROPERTY_GET(esb::Numeric, PropId::Size); }
		ESB_PROPERTY_FIELD_RO(esb::Numeric, Size);
		// Свойство [[Только Чтение]] Жирный Как [Булево,Неопределено]
		UndefOr<Boolean> GetBold() const { ESB_INVOKE_PROPERTY_GET(UndefOr<Boolean>, PropId::Bold); }
		// Свойство [[Только Чтение]] Наклонный Как [Булево,Неопределено]
		UndefOr<Boolean> GetItalic() const { ESB_INVOKE_PROPERTY_GET(UndefOr<Boolean>, PropId::Italic); }
		// Свойство [[Только Чтение]] Подчеркивание Как [Булево,Неопределено]
		UndefOr<Boolean> GetUnderline() const { ESB_INVOKE_PROPERTY_GET(UndefOr<Boolean>, PropId::Underline); }
		// Свойство [[Только Чтение]] Зачеркивание Как [Булево,Неопределено]
		UndefOr<Boolean> GetStrikeout() const { ESB_INVOKE_PROPERTY_GET(UndefOr<Boolean>, PropId::Strikeout); }
		// Свойство [[Только Чтение]] Вид Как ВидШрифта
		FontTypeValue GetType() const { ESB_INVOKE_PROPERTY_GET(esb::FontTypeValue, PropId::Type); }
		ESB_PROPERTY_FIELD_RO(esb::FontTypeValue, Type);
		// Свойство [[Только Чтение]] Масштаб Как Число
		Numeric GetScale() const { ESB_INVOKE_PROPERTY_GET(esb::Numeric, PropId::Scale); }
		ESB_PROPERTY_FIELD_RO(esb::Numeric, Scale);
	};


	class FormattedString : public Object {
		ESB_CLASS_IMPLEMENT_MAKE_OBJ(FormattedString)
		ESB_CLASS_IMPLEMENT_MAKE_OPT(FormattedString)
	public:
		ESB_DECLARE_TYPEINFO(FormattedString)
		ESB_IMPLEMENT_ARGBYVAL_OPERATOR(FormattedString)
	public:
		static FormattedString Create() { ESB_INVOKE_CTOR0(FormattedString); }
		static FormattedString Create(ConstPara<String> Content_,			ConstPara<Font> Font_ = DefUndef<esb::Font>, ConstPara<Color> TextColor_ = DefUndef<esb::Color>, ConstPara<Color> BackColor_ = DefUndef<esb::Color>, ConstPara<String> Ref_ = DefUndef<esb::String>) { ESB_INVOKE_CTOR5(FormattedString, Content_, Font_, TextColor_, BackColor_, Ref_); }
		static FormattedString Create(ConstPara<FormattedString> Content_,	ConstPara<Font> Font_ = DefUndef<esb::Font>, ConstPara<Color> TextColor_ = DefUndef<esb::Color>, ConstPara<Color> BackColor_ = DefUndef<esb::Color>, ConstPara<String> Ref_ = DefUndef<esb::String>) { ESB_INVOKE_CTOR5(FormattedString, Content_, Font_, TextColor_, BackColor_, Ref_); }
		static FormattedString Create(ConstPara<Picture> Content_,			ConstPara<Font> Font_ = DefUndef<esb::Font>, ConstPara<Color> TextColor_ = DefUndef<esb::Color>, ConstPara<Color> BackColor_ = DefUndef<esb::Color>, ConstPara<String> Ref_ = DefUndef<esb::String>) { ESB_INVOKE_CTOR5(FormattedString, Content_, Font_, TextColor_, BackColor_, Ref_); }
		static FormattedString Create(ConstPara<Array> Content_,				ConstPara<Font> Font_ = DefUndef<esb::Font>, ConstPara<Color> TextColor_ = DefUndef<esb::Color>, ConstPara<Color> BackColor_ = DefUndef<esb::Color>, ConstPara<String> Ref_ = DefUndef<esb::String>) { ESB_INVOKE_CTOR5(FormattedString, Content_, Font_, TextColor_, BackColor_, Ref_); }
		template<std::same_as<String>... StringT>
		static FormattedString Create(ConstPara<StringT> ...Content1ContentN_) { ESB_INVOKE_CTOR_PACK0(FormattedString, Content1ContentN_); } //  !!! not present in context !!!
		template<std::same_as<FormattedString>... FormattedStringT>
		static FormattedString Create(ConstPara<FormattedStringT> ...Content1ContentN_) { ESB_INVOKE_CTOR_PACK0(FormattedString, Content1ContentN_); } //  !!! not present in context !!!
		template<std::same_as<Picture>... PictureT>
		static FormattedString Create(ConstPara<PictureT> ...Content1ContentN_) { ESB_INVOKE_CTOR_PACK0(FormattedString, Content1ContentN_); } //  !!! not present in context !!!
		template<std::same_as<Array>... ArrayT>
		static FormattedString Create(ConstPara<ArrayT> ...Content1ContentN_) { ESB_INVOKE_CTOR_PACK0(FormattedString, Content1ContentN_); } //  !!! not present in context !!!
	};

}//namespace esb 







//TOBE: Возможно все таки нужно сделать некоторое примитивное конфигурирование:
//#define ESB_API_USE_ALL 0
// 
//#define ESB_API_USE_core 0
//#define ESB_API_USE_addin 0
//#define ESB_API_USE_mngui 0
//#define ESB_API_USE_backend 0
//#define ESB_API_USE_entext2 0
//#define ESB_API_USE_frntend 0
//#define ESB_API_USE_mngbase 0
//#define ESB_API_USE_odata 0
//#define ESB_API_USE_xml2 0
//#define ESB_API_USE_ext 0

//NOTE: По многим и многим функциям аргументы по-умолчанию задекларированы как DefUndef<...> и для многих и многих функций это правильно.
//		НО!! Для некоторых функций аргументы по-умолчанию нужно указывать конкретно. Пример:
//		Функция СтрНайти(Строка_ Как Строка, ПодстрокаПоиска_ Как Строка, НаправлениеПоиска_ Как НаправлениеПоиска, НачальнаяПозиция_ Как Число, НомерВхождения_ Как Число) Возвращает Число
//		inline Numeric StrFind(ConstPara<String> Line_, ConstPara<String> SearchSubstring_, ConstPara<SearchDirection> SearchDirection_ = SearchDirectionEnum::FromBegin, ConstPara<Numeric> StartIndex_ = DefUndef<Numeric>, ConstPara<Numeric> EntryNumber_ = Numeric::Value_1_) { ESB_INVOKE_API_FUNC5(Numeric, core, 4, Line_, SearchSubstring_, SearchDirection_, StartIndex_, EntryNumber_); }
//		т.е. именно SearchDirection_ = SearchDirectionEnum::FromBegin, а не SearchDirection_ = DefUndef<SearchDirection> и т.п.
//		!!ВЫЯСНИТЬ это никаким образом нельзя кроме как пробовать, получать ошибки, лазить в хелп и ручками править. Электронного источника НЕТ!!
//Увы..
//
namespace esb //API part
{	

#define ESB_INVOKE_API_FUNC0(T_, P_, M_)						return check_and_make<T_>(esb_api::Provider<esb_api::ProviderId::##P_##>::invoke_func(M_))
#define ESB_INVOKE_API_FUNC1(T_, P_, M_, _1)					return check_and_make<T_>(esb_api::Provider<esb_api::ProviderId::##P_##>::invoke_func(M_, {&_1} ))
#define ESB_INVOKE_API_FUNC2(T_, P_, M_, _1,_2)					return check_and_make<T_>(esb_api::Provider<esb_api::ProviderId::##P_##>::invoke_func(M_, {&_1,&_2} ))
#define ESB_INVOKE_API_FUNC3(T_, P_, M_, _1,_2,_3)				return check_and_make<T_>(esb_api::Provider<esb_api::ProviderId::##P_##>::invoke_func(M_, {&_1,&_2,&_3} ))
#define ESB_INVOKE_API_FUNC4(T_, P_, M_, _1,_2,_3,_4)			return check_and_make<T_>(esb_api::Provider<esb_api::ProviderId::##P_##>::invoke_func(M_, {&_1,&_2,&_3,&_4} ))
#define ESB_INVOKE_API_FUNC5(T_, P_, M_, _1,_2,_3,_4,_5)		return check_and_make<T_>(esb_api::Provider<esb_api::ProviderId::##P_##>::invoke_func(M_, {&_1,&_2,&_3,&_4,&_5} ))
#define ESB_INVOKE_API_FUNC6(T_, P_, M_, _1,_2,_3,_4,_5,_6)		return check_and_make<T_>(esb_api::Provider<esb_api::ProviderId::##P_##>::invoke_func(M_, {&_1,&_2,&_3,&_4,&_5,&_6} ))
#define ESB_INVOKE_API_FUNCX(T_, P_, M_, _X)					return check_and_make<T_>(esb_api::Provider<esb_api::ProviderId::##P_##>::invoke_func(M_, {&_X...} ))

#define ESB_INVOKE_API_PROC0(P_, M_)							return esb_api::Provider<esb_api::ProviderId::##P_##>::invoke_proc(M_)
#define ESB_INVOKE_API_PROC1(P_, M_, _1)						return esb_api::Provider<esb_api::ProviderId::##P_##>::invoke_proc(M_, {&_1} )
#define ESB_INVOKE_API_PROC2(P_, M_, _1,_2)						return esb_api::Provider<esb_api::ProviderId::##P_##>::invoke_proc(M_, {&_1,&_2} )
#define ESB_INVOKE_API_PROC3(P_, M_, _1,_2,_3)					return esb_api::Provider<esb_api::ProviderId::##P_##>::invoke_proc(M_, {&_1,&_2,&_3} )
#define ESB_INVOKE_API_PROC4(P_, M_, _1,_2,_3,_4)				return esb_api::Provider<esb_api::ProviderId::##P_##>::invoke_proc(M_, {&_1,&_2,&_3,&_4} )
#define ESB_INVOKE_API_PROC5(P_, M_, _1,_2,_3,_4,_5)			return esb_api::Provider<esb_api::ProviderId::##P_##>::invoke_proc(M_, {&_1,&_2,&_3,&_4,&_5} )
#define ESB_INVOKE_API_PROC6(P_, M_, _1,_2,_3,_4,_5,_6)			return esb_api::Provider<esb_api::ProviderId::##P_##>::invoke_proc(M_, {&_1,&_2,&_3,&_4,&_5,&_6} )
#define ESB_INVOKE_API_PROC7(P_, M_, _1,_2,_3,_4,_5,_6,_7)		return esb_api::Provider<esb_api::ProviderId::##P_##>::invoke_proc(M_, {&_1,&_2,&_3,&_4,&_5,&_6,&_7} )

#define ESB_INVOKE_API_PROP_GET(T_, P_, M_)						return check_and_make<T_>(esb_api::Provider<esb_api::ProviderId::##P_##>::invoke_prop_get(M_))

// api from providers

	// Процедура ЗаполнитьЗначенияСвойств(Приемник_ Как Произвольный, Источник_ Как Произвольный, СписокСвойств_ Как Строка, ИсключаяСвойства_ Как Строка)
	inline void FillPropertyValues(ConstPara<Arbitrary> Receiver_, ConstPara<Arbitrary> Source_, ConstPara<String> ListOfProperties_ = DefUndef<String>, ConstPara<String> ExcludeProperties_ = DefUndef<String>) { ESB_INVOKE_API_PROC4(core, 0, Receiver_, Source_, ListOfProperties_, ExcludeProperties_); }
	// Функция Base64Строка(Значение_ Как ДвоичныеДанные) Возвращает Строка
	inline String Base64String(ConstPara<BinaryData> Value_) { ESB_INVOKE_API_FUNC1(String, core, 2, Value_); }
	// Функция ЗначениеЗаполнено(Значение_ Как Произвольный) Возвращает Булево
	inline Boolean ValueIsFilled(ConstPara<Arbitrary> Value_) { ESB_INVOKE_API_FUNC1(Boolean, core, 3, Value_); }
	// Функция СтрНайти(Строка_ Как Строка, ПодстрокаПоиска_ Как Строка, НаправлениеПоиска_ Как НаправлениеПоиска, НачальнаяПозиция_ Как Число, НомерВхождения_ Как Число) Возвращает Число
	inline Numeric StrFind(ConstPara<String> Line_, ConstPara<String> SearchSubstring_, ConstPara<SearchDirectionValue> SearchDirection_ = SearchDirectionEnum::FromBegin, ConstPara<Numeric> StartIndex_ = DefUndef<Numeric>, ConstPara<Numeric> EntryNumber_ = Numeric::Value_1_) { ESB_INVOKE_API_FUNC5(Numeric, core, 4, Line_, SearchSubstring_, SearchDirection_, StartIndex_, EntryNumber_); }
 	// Функция СтрСравнить(Строка1_ Как Строка, Строка2_ Как Строка) Возвращает Число
	inline Numeric StrCompare(ConstPara<String> String1_, ConstPara<String> String2_)			{ ESB_INVOKE_API_FUNC2(Numeric, core, 6, String1_, String2_); }
	// Функция СтрЗаканчиваетсяНа(Строка_ Как Строка, СтрокаПоиска_ Как Строка) Возвращает Булево
	inline Boolean StrEndsWith(ConstPara<String> String_, ConstPara<String> SearchString_)	{ ESB_INVOKE_API_FUNC2(Boolean, core, 8, String_, SearchString_); }
	// Функция СтрРазделить(Строка_ Как Строка, Разделитель_ Как Строка, ВключатьПустые_ Как Булево) Возвращает Массив
	inline Array StrSplit(ConstPara<String> String_, ConstPara<String> Separator_, ConstPara<Boolean> IncludeBlank_ = DefUndef<Boolean>) { ESB_INVOKE_API_FUNC3(Array, core, 9, String_, Separator_, IncludeBlank_); }
	// Функция СтрСоединить(Строки_ Как [ФиксированныйМассив,Массив], Разделитель_ Как Строка) Возвращает Строка
	inline String StrConcat(ConstPara<FixedArray> Strings_, ConstPara<String> Separator_ = DefUndef<String>) { ESB_INVOKE_API_FUNC2(String, core, 10, Strings_, Separator_); }
	inline String StrConcat(ConstPara<Array> Strings_, ConstPara<String> Separator_ = DefUndef<String>) { ESB_INVOKE_API_FUNC2(String, core, 10, Strings_, Separator_); }
	// Функция ПодключитьВнешнююКомпоненту(Местоположение_ Как Строка, Имя_ Как Строка, Тип_ Как ТипВнешнейКомпоненты) Возвращает Булево
	inline Boolean AttachAddIn(ConstPara<String> Location_, ConstPara<String> Name_, ConstPara<AddInTypeValue> Type_ = DefUndef<AddInTypeValue>) { ESB_INVOKE_API_FUNC3(Boolean, addin, 0, Location_, Name_, Type_); }
	// Процедура ЗагрузитьВнешнююКомпоненту(ИмяФайла_ Как Строка)
	inline void LoadAddIn(ConstPara<String> FileName_) { ESB_INVOKE_API_PROC1(addin, 1, FileName_); }
	// Процедура НачатьПодключениеВнешнейКомпоненты(ОписаниеОповещения_ Как ОписаниеОповещения, Местоположение_ Как Строка, Имя_ Как Строка, Тип_ Как ТипВнешнейКомпоненты)
	inline void BeginAttachingAddIn(ConstPara<NotifyDescription> NotifyDescription_, ConstPara<String> Location_, ConstPara<String> Name_, ConstPara<AddInTypeValue> Type_ = DefUndef<AddInTypeValue>) { ESB_INVOKE_API_PROC4(addin, 2, NotifyDescription_, Location_, Name_, Type_); }


	// Функция ПолучитьФайл(Адрес_ Как Строка, ИмяФайла_ Как Строка, Интерактивно_ Как Булево) Возвращает [Булево,Неопределено]
	inline UndefOr<Boolean> GetFile(ConstPara<String> Address_, ConstPara<String> FileName_ = DefUndef<String>, ConstPara<Boolean> Interactively_ = DefUndef<Boolean>) { ESB_INVOKE_API_FUNC3(UndefOr<Boolean>, mngui, 3, Address_, FileName_, Interactively_); }
	// Функция ПоместитьФайл(Адрес_ Как Строка, НачальноеИмяФайла_ Как Строка, ВыбранноеИмяФайла_ Как Строка, Интерактивно_ Как Булево, УникальныйИдентификаторФормы_ Как УникальныйИдентификатор) Возвращает Булево
	inline Boolean PutFile(ConstPara<String> Address_, ConstPara<String> FileName_, ConstPara<String> SelectedFileName_, ConstPara<Boolean> Interactively_, ConstPara<UUIDValue> FormUniqueID_ = DefUndef<UUIDValue>) { ESB_INVOKE_API_FUNC5(Boolean, mngui, 4, Address_, FileName_, SelectedFileName_, Interactively_, FormUniqueID_); }
	// Функция ПолучитьФункциональнуюОпциюИнтерфейса(Имя_ Как Строка) Возвращает Произвольный
	inline Arbitrary GetInterfaceFunctionalOption(ConstPara<String> Name_) { ESB_INVOKE_API_FUNC1(Arbitrary, mngui, 6, Name_); }
	// Процедура УстановитьПараметрыФункциональныхОпцийИнтерфейса(УстанавливаемыеПараметры_ Как Структура, СбрасываемыеПараметры_ Как [Массив,Строка])
	inline void SetInterfaceFunctionalOptionParameters(ConstPara<Structure> ParametersToBeSpecified_, ConstPara<Array> ParametersToBeReset_ = DefUndef<Array>) { ESB_INVOKE_API_PROC2(mngui, 7, ParametersToBeSpecified_, ParametersToBeReset_); }
	inline void SetInterfaceFunctionalOptionParameters(ConstPara<Structure> ParametersToBeSpecified_, ConstPara<String> ParametersToBeReset_ = DefUndef<String>) { ESB_INVOKE_API_PROC2(mngui, 7, ParametersToBeSpecified_, ParametersToBeReset_); }
	// Функция ПолучитьПараметрыФункциональныхОпцийИнтерфейса() Возвращает Структура
	inline Structure GetInterfaceFunctionalOptionParameters() { ESB_INVOKE_API_FUNC0(Structure, mngui, 8); }
	// Процедура ОбновитьИнтерфейс()
	inline void RefreshInterface() { ESB_INVOKE_API_PROC0(mngui, 9); }
	// Процедура ПерейтиПоНавигационнойСсылке(НавигационнаяСсылка_ Как Строка)
	inline void GotoURL(ConstPara<String> NavigationLink_) { ESB_INVOKE_API_PROC1(mngui, 14, NavigationLink_); }
	// Процедура ВыполнитьОбработкуОповещения(ВыполняемоеОповещение_ Как ОписаниеОповещения, Результат_ Как void)
	inline void ExecuteNotifyProcessing(ConstPara<NotifyDescription> NotificationPerformed_, ConstPara<Arbitrary> Result_ = DefUndef<Arbitrary>) { ESB_INVOKE_API_PROC2(mngui, 16, NotificationPerformed_, Result_); }
	// Процедура КомандаСистемы(СтрокаКоманды_ Как Строка, ТекущийКаталог_ Как Строка)
	inline void System(ConstPara<String> CommandString_, ConstPara<String> CurrentDirectory_ = DefUndef<String>) { ESB_INVOKE_API_PROC2(mngui, 17, CommandString_, CurrentDirectory_); }
	// Процедура ОповеститьОбИзменении(ИзмененныйТип_ Как Тип)
	inline void NotifyChanged(ConstPara<TypeValue> ChangedType_) { ESB_INVOKE_API_PROC1(mngui, 18, ChangedType_); }
	// Функция ПодключитьРасширениеРаботыСФайлами() Возвращает Булево
	inline Boolean AttachFileSystemExtension() { ESB_INVOKE_API_FUNC0(Boolean, mngui, 19); }
	// Процедура НачатьПодключениеРасширенияРаботыСФайлами(ОписаниеОповещения_ Как ОписаниеОповещения)
	inline void BeginAttachingFileSystemExtension(ConstPara<NotifyDescription> NotifyDescription_) { ESB_INVOKE_API_PROC1(mngui, 20, NotifyDescription_); }
	// Процедура УстановитьРасширениеРаботыСФайлами()
	inline void InstallFileSystemExtension() { ESB_INVOKE_API_PROC0(mngui, 21); }
	// Процедура НачатьУстановкуРасширенияРаботыСФайлами(ОписаниеОповещенияОЗавершении_ Как ОписаниеОповещения)
	inline void BeginInstallFileSystemExtension(ConstPara<NotifyDescription> NotifyDescriptionOnCompletion_) { ESB_INVOKE_API_PROC1(mngui, 22, NotifyDescriptionOnCompletion_); }
	// Процедура УстановитьВнешнююКомпоненту(Местоположение_ Как Строка)
	inline void InstallAddIn(ConstPara<String> Location_) { ESB_INVOKE_API_PROC1(mngui, 27, Location_); }
	// Процедура НачатьУстановкуВнешнейКомпоненты(ОписаниеОповещенияОЗавершении_ Как ОписаниеОповещения, Местоположение_ Как Строка)
	inline void BeginInstallAddIn(ConstPara<NotifyDescription> NotifyDescriptionOnCompletion_, ConstPara<String> Location_) { ESB_INVOKE_API_PROC2(mngui, 28, NotifyDescriptionOnCompletion_, Location_); }
	// Функция ЗапроситьРазрешениеПользователя(Вызовы_ Как Массив) Возвращает Булево
	inline Boolean RequestUserPermission(ConstPara<Array> Calls_) { ESB_INVOKE_API_FUNC1(Boolean, mngui, 29, Calls_); }
	// Процедура НачатьЗапросРазрешенияПользователя(ОписаниеОповещения_ Как ОписаниеОповещения, Вызовы_ Как Массив)
	inline void BeginRequestingUserPermission(ConstPara<NotifyDescription> NotifyDescription_, ConstPara<Array> Calls_) { ESB_INVOKE_API_PROC2(mngui, 30, NotifyDescription_, Calls_); }


	// Процедура ЗафиксироватьТранзакцию()
	inline void CommitTransaction() { ESB_INVOKE_API_PROC0(backend, 1); }
	// Процедура ОтменитьТранзакцию()
	inline void RollbackTransaction() { ESB_INVOKE_API_PROC0(backend, 2); }
	// Функция ТранзакцияАктивна() Возвращает Булево
	inline Boolean TransactionActive() { ESB_INVOKE_API_FUNC0(Boolean, backend, 3); }
	// Процедура Сообщить(ТекстСообщения_ Как Строка, Статус_ Как СтатусСообщения)
	inline void Message(ConstPara<String> MessageText_, ConstPara<MessageStatusValue> Status_ = DefUndef<MessageStatusValue>) { ESB_INVOKE_API_PROC2(backend, 4, MessageText_, Status_); }
	// Функция НайтиПомеченныеНаУдаление(ОбластьПоиска_ Как Массив, ВключитьОбъекты_ Как Массив, ИсключитьОбъекты_ Как Массив) Возвращает Массив
	inline Array FindMarkedForDeletion(ConstPara<Array> SearchArea_, ConstPara<Array> IncludeObjects_, ConstPara<Array> ExcludeObjects_ = DefUndef<Array>) { ESB_INVOKE_API_FUNC3(Array, backend, 5, SearchArea_, IncludeObjects_, ExcludeObjects_); }
	// Процедура УстановитьМонопольныйРежим(МонопольныйРежим_ Как Булево)
	inline void SetExclusiveMode(ConstPara<Boolean> ExclusiveMode_) { ESB_INVOKE_API_PROC1(backend, 10, ExclusiveMode_); }
	// Функция МонопольныйРежим() Возвращает Булево
	inline Boolean ExclusiveMode() { ESB_INVOKE_API_FUNC0(Boolean, backend, 11); }
	// Функция НСтр(ИсходнаяСтрока_ Как Строка, КодЯзыка_ Как Строка) Возвращает Строка
	inline String NStr(ConstPara<String> InitialString_, ConstPara<String> LanguageCode_ = DefUndef<String>) { ESB_INVOKE_API_FUNC2(String, backend, 12, InitialString_, LanguageCode_); }
	// Функция ПолучитьОперативнуюОтметкуВремени() Возвращает Дата
	inline DateTime GetRealTimeTimestamp() { ESB_INVOKE_API_FUNC0(DateTime, backend, 13); }
	// Функция ЗначениеВСтрокуВнутр(Значение_ Как Произвольный) Возвращает Строка
	inline String ValueToStringInternal(ConstPara<Arbitrary> Value_) { ESB_INVOKE_API_FUNC1(String, backend, 16, Value_); }
	// Функция ЗначениеИзСтрокиВнутр(Строка_ Как Строка) Возвращает Произвольный
	inline Arbitrary ValueFromStringInternal(ConstPara<String> Line_) { ESB_INVOKE_API_FUNC1(Arbitrary, backend, 17, Line_); }
	// Функция ЗначениеВФайл(ИмяФайла_ Как Строка, Значение_ Как Произвольный) Возвращает Булево
	inline Boolean ValueToFile(ConstPara<String> FileName_, ConstPara<Arbitrary> Value_) { ESB_INVOKE_API_FUNC2(Boolean, backend, 18, FileName_, Value_); }
	// Функция ЗначениеИзФайла(ИмяФайла_ Как Строка) Возвращает Произвольный
	inline Arbitrary ValueFromFile(ConstPara<String> FileName_) { ESB_INVOKE_API_FUNC1(Arbitrary, backend, 19, FileName_); }
	// Функция ИмяКомпьютера() Возвращает Строка
	inline String ComputerName() { ESB_INVOKE_API_FUNC0(String, backend, 20); }
	// Функция ИмяПользователя() Возвращает Строка
	inline String UserName() { ESB_INVOKE_API_FUNC0(String, backend, 21); }
	// Функция ПолноеИмяПользователя() Возвращает Строка
	inline String UserFullName() { ESB_INVOKE_API_FUNC0(String, backend, 22); }
	// Функция КаталогПрограммы() Возвращает Строка
	inline String BinDir() { ESB_INVOKE_API_FUNC0(String, backend, 23); }
	// Функция КаталогВременныхФайлов() Возвращает Строка
	inline String TempFilesDir() { ESB_INVOKE_API_FUNC0(String, backend, 24); }
	// Процедура НачатьПолучениеКаталогаВременныхФайлов(ОписаниеОповещения_ Как ОписаниеОповещения)
	inline void BeginGettingTempFilesDir(ConstPara<NotifyDescription> NotifyDescription_) { ESB_INVOKE_API_PROC1(backend, 25, NotifyDescription_); }
	// Функция ТекущийКодЛокализации() Возвращает Строка
	inline String CurrentLocaleCode() { ESB_INVOKE_API_FUNC0(String, backend, 26); }
	// Функция КодЛокализацииИнформационнойБазы() Возвращает Строка
	inline String InfoBaseLocaleCode() { ESB_INVOKE_API_FUNC0(String, backend, 27); }
	// Функция СтрокаСоединенияИнформационнойБазы() Возвращает Строка
	inline String InfoBaseConnectionString() { ESB_INVOKE_API_FUNC0(String, backend, 29); }
	// Процедура ВыгрузитьЖурналРегистрации(Приемник_ Как [Строка,ТаблицаЗначений], Отбор_ Как Структура, Колонки_ Как Строка, ИмяВходногоФайла_ Как Строка, МаксимальноеКоличество_ Как Число)
	inline void UnloadEventLog(ConstPara<String> Destination_, ConstPara<Structure> Filter_ = DefUndef<Structure>, ConstPara<String> Columns_ = DefUndef<String>, ConstPara<String> InputFileName_ = DefUndef<String>, ConstPara<Numeric> MaximumCount_ = DefUndef<Numeric>) { ESB_INVOKE_API_PROC5(backend, 31, Destination_, Filter_, Columns_, InputFileName_, MaximumCount_); }
	inline void UnloadEventLog(ConstPara<ValueTable> Destination_, ConstPara<Structure> Filter_ = DefUndef<Structure>, ConstPara<String> Columns_ = DefUndef<String>, ConstPara<String> InputFileName_ = DefUndef<String>, ConstPara<Numeric> MaximumCount_ = DefUndef<Numeric>) { ESB_INVOKE_API_PROC5(backend, 31, Destination_, Filter_, Columns_, InputFileName_, MaximumCount_); }
	// Процедура СкопироватьЖурналРегистрации(ИмяВходногоФайла_ Как Строка, ИмяВыходногоФайла_ Как Строка, Отбор_ Как Структура)
	inline void CopyEventLog(ConstPara<String> InputFileName_, ConstPara<String> OutputFileName_, ConstPara<Structure> Filter_) { ESB_INVOKE_API_PROC3(backend, 32, InputFileName_, OutputFileName_, Filter_); }
	// Процедура ОчиститьЖурналРегистрации(Отбор_ Как Структура)
	inline void ClearEventLog(ConstPara<Structure> Filter_) { ESB_INVOKE_API_PROC1(backend, 33, Filter_); }
	// Функция ПредставлениеПриложения(ИмяПриложения_ Как Строка) Возвращает Строка
	inline String ApplicationPresentation(ConstPara<String> ApplicationName_) { ESB_INVOKE_API_FUNC1(String, backend, 34, ApplicationName_); }
	// Функция ПолучитьСоединенияИнформационнойБазы() Возвращает Массив
	inline Array GetInfoBaseConnections() { ESB_INVOKE_API_FUNC0(Array, backend, 35); }
	// Функция ПолучитьСеансыИнформационнойБазы() Возвращает Массив
	inline Array GetInfoBaseSessions() { ESB_INVOKE_API_FUNC0(Array, backend, 36); }
	// Функция НомерСоединенияИнформационнойБазы() Возвращает Число
	inline Numeric InfoBaseConnectionNumber() { ESB_INVOKE_API_FUNC0(Numeric, backend, 38); }
	// Функция НомерСеансаИнформационнойБазы() Возвращает Число
	inline Numeric InfoBaseSessionNumber() { ESB_INVOKE_API_FUNC0(Numeric, backend, 39); }
	// Функция ПредставлениеСобытияЖурналаРегистрации(ИдентификаторСобытия_ Как Строка) Возвращает Строка
	inline String EventLogEventPresentation(ConstPara<String> EventIdentifier_) { ESB_INVOKE_API_FUNC1(String, backend, 40, EventIdentifier_); }
	// Функция ПолучитьИспользованиеЖурналаРегистрации() Возвращает Массив
	inline Array GetEventLogUsing() { ESB_INVOKE_API_FUNC0(Array, backend, 41); }
	// Процедура УстановитьИспользованиеЖурналаРегистрации(Уровни_ Как Массив)
	inline void SetEventLogUsing(ConstPara<Array> Levels_) { ESB_INVOKE_API_PROC1(backend, 42, Levels_); }
	// Функция КонфигурацияИзменена() Возвращает Булево
	inline Boolean ConfigurationChanged() { ESB_INVOKE_API_FUNC0(Boolean, backend, 43); }
	// Функция КонфигурацияБазыДанныхИзмененаДинамически() Возвращает Булево
	inline Boolean DataBaseConfigurationChangedDynamically() { ESB_INVOKE_API_FUNC0(Boolean, backend, 44); }
	// Процедура УстановитьВремяОжиданияБлокировкиДанных(Таймаут_ Как Число)
	inline void SetLockWaitTime(ConstPara<Numeric> Timeout_) { ESB_INVOKE_API_PROC1(backend, 46, Timeout_); }
	// Функция ПолучитьВремяОжиданияБлокировкиДанных() Возвращает Число
	inline Numeric GetLockWaitTime() { ESB_INVOKE_API_FUNC0(Numeric, backend, 47); }
	// Процедура УстановитьМинимальнуюДлинуПаролейПользователей(Длина_ Как Число)
	inline void SetUserPasswordMinLength(ConstPara<Numeric> Length_) { ESB_INVOKE_API_PROC1(backend, 48, Length_); }
	// Функция ПолучитьМинимальнуюДлинуПаролейПользователей() Возвращает Число
	inline Numeric GetUserPasswordMinLength() { ESB_INVOKE_API_FUNC0(Numeric, backend, 49); }
	// Процедура УстановитьПроверкуСложностиПаролейПользователей(СложныйПароль_ Как Булево)
	inline void SetUserPasswordStrengthCheck(ConstPara<Boolean> ComplexPassword_) { ESB_INVOKE_API_PROC1(backend, 50, ComplexPassword_); }
	// Функция ПолучитьПроверкуСложностиПаролейПользователей() Возвращает Булево
	inline Boolean GetUserPasswordStrengthCheck() { ESB_INVOKE_API_FUNC0(Boolean, backend, 51); }
	// Функция ТекущийЯзыкСистемы() Возвращает Строка
	inline String CurrentSystemLanguage() { ESB_INVOKE_API_FUNC0(String, backend, 52); }
	// Функция КраткоеПредставлениеОшибки(ИнформацияОбОшибке_ Как ИнформацияОбОшибке) Возвращает Строка
	inline String BriefErrorDescription(ConstPara<ErrorInfoValue> ErrorInfo_) { ESB_INVOKE_API_FUNC1(String, backend, 57, ErrorInfo_); }
	// Функция ПодробноеПредставлениеОшибки(ИнформацияОбОшибке_ Как ИнформацияОбОшибке) Возвращает Строка
	inline String DetailErrorDescription(ConstPara<ErrorInfoValue> ErrorInfo_) { ESB_INVOKE_API_FUNC1(String, backend, 58, ErrorInfo_); }
	// Процедура УстановитьПривилегированныйРежим(Включить_ Как Булево)
	inline void SetPrivilegedMode(ConstPara<Boolean> PrivilegedModeOn_) { ESB_INVOKE_API_PROC1(backend, 62, PrivilegedModeOn_); }
	// Функция ПривилегированныйРежим() Возвращает Булево
	inline Boolean PrivilegedMode() { ESB_INVOKE_API_FUNC0(Boolean, backend, 63); }
	// Процедура ЗаблокироватьДанныеДляРедактирования(Ключ_ Как Произвольный, ВерсияДанных_ Как Строка, ИдентификаторФормы_ Как УникальныйИдентификатор)
	inline void LockDataForEdit(ConstPara<Arbitrary> Key_, ConstPara<String> DataVersion_ = DefUndef<String>, ConstPara<UUIDValue> FormIdentifier_ = DefUndef<UUIDValue>) { ESB_INVOKE_API_PROC3(backend, 64, Key_, DataVersion_, FormIdentifier_); }
	// Процедура ОбновитьПовторноИспользуемыеЗначения()
	inline void RefreshReusableValues() { ESB_INVOKE_API_PROC0(backend, 66); }
	// Функция ПолучитьЗначенияОтбораЖурналаРегистрации(Колонки_ Как Строка, ИмяВходногоФайла_ Как Строка) Возвращает Структура
	inline Structure GetEventLogFilterValues(ConstPara<String> Columns_, ConstPara<String> InputFileName_ = DefUndef<String>) { ESB_INVOKE_API_FUNC2(Structure, backend, 67, Columns_, InputFileName_); }
	// Процедура УстановитьБезопасныйРежим(БезопасныйРежим_ Как [Булево,Строка])
	inline void SetSafeMode(ConstPara<Boolean> SafeMode_) { ESB_INVOKE_API_PROC1(backend, 68, SafeMode_); }
	inline void SetSafeMode(ConstPara<String> SafeMode_) { ESB_INVOKE_API_PROC1(backend, 68, SafeMode_); }
	// Функция БезопасныйРежим() Возвращает [Булево,Строка]
	inline ClassMix<Boolean, String> SafeMode() { ESB_INVOKE_API_FUNC0(ESB_MIX2(Boolean, String), backend, 69); }
	// Процедура УстановитьЧасовойПоясИнформационнойБазы(ЧасовойПояс_ Как Строка)
	inline void SetInfoBaseTimeZone(ConstPara<String> TimeZone_) { ESB_INVOKE_API_PROC1(backend, 70, TimeZone_); }
	// Функция ПолучитьЧасовойПоясИнформационнойБазы() Возвращает [Строка,Неопределено]
	inline UndefOr<String> GetInfoBaseTimeZone() { ESB_INVOKE_API_FUNC0(UndefOr<String>, backend, 71); }
	// Процедура УстановитьЧасовойПоясСеанса(ЧасовойПояс_ Как Строка)
	inline void SetSessionTimeZone(ConstPara<String> TimeZone_) { ESB_INVOKE_API_PROC1(backend, 72, TimeZone_); }
	// Функция ЧасовойПоясСеанса() Возвращает Строка
	inline String SessionTimeZone() { ESB_INVOKE_API_FUNC0(String, backend, 73); }
	// Функция ТекущаяДатаСеанса() Возвращает Дата
	inline DateTime CurrentSessionDate() { ESB_INVOKE_API_FUNC0(DateTime, backend, 74); }
	// Функция ПолучитьСообщенияПользователю(УдалятьПолученные_ Как Булево) Возвращает ФиксированныйМассив
	inline FixedArray GetUserMessages(ConstPara<Boolean> ToDeleteGetting_) { ESB_INVOKE_API_FUNC1(FixedArray, backend, 75, ToDeleteGetting_); }
	// Функция ТекущаяУниверсальнаяДата() Возвращает Дата
	inline DateTime CurrentUniversalDate() { ESB_INVOKE_API_FUNC0(DateTime, backend, 77); }
	// Функция МестноеВремя(УниверсальноеВремя_ Как Дата, ЧасовойПояс_ Как [Строка,Неопределено]) Возвращает Дата
	inline DateTime ToLocalTime(ConstPara<DateTime> UniversalTime_, ConstPara<String> TimeZone_ = DefUndef<String>) { ESB_INVOKE_API_FUNC2(DateTime, backend, 78, UniversalTime_, TimeZone_); }
	// Функция УниверсальноеВремя(МестноеВремя_ Как Дата, ЧасовойПояс_ Как [Строка,Неопределено]) Возвращает Дата
	inline DateTime ToUniversalTime(ConstPara<DateTime> LocalTime_, ConstPara<String> TimeZone_ = DefUndef<String>) { ESB_INVOKE_API_FUNC2(DateTime, backend, 79, LocalTime_, TimeZone_); }
	// Функция ЧасовойПояс() Возвращает Строка
	inline String TimeZone() { ESB_INVOKE_API_FUNC0(String, backend, 80); }
	// Функция ПолучитьДопустимыеЧасовыеПояса() Возвращает Массив
	inline Array GetAvailableTimeZones() { ESB_INVOKE_API_FUNC0(Array, backend, 81); }
	// Функция ПредставлениеЧасовогоПояса(ЧасовойПояс_ Как Строка) Возвращает Строка
	inline String TimeZonePresentation(ConstPara<String> TimeZone_) { ESB_INVOKE_API_FUNC1(String, backend, 82, TimeZone_); }
	// Функция ПолучитьДопустимыеКодыЛокализации() Возвращает Массив
	inline Array GetAvailableLocaleCodes() { ESB_INVOKE_API_FUNC0(Array, backend, 83); }
	// Функция ПредставлениеКодаЛокализации(КодЛокализации_ Как Строка) Возвращает Строка
	inline String LocaleCodePresentation(ConstPara<String> LocalizationCode_) { ESB_INVOKE_API_FUNC1(String, backend, 84, LocalizationCode_); }
	// Функция ПредставлениеПрава(ИмяПрава_ Как Строка) Возвращает Строка
	inline String RightPresentation(ConstPara<String> NameOfRight_) { ESB_INVOKE_API_FUNC1(String, backend, 90, NameOfRight_); }
	// Процедура УдалитьДанныеИнформационнойБазы()
	inline void EraseInfoBaseData() { ESB_INVOKE_API_PROC0(backend, 93); }
	// Процедура УстановитьБезопасныйРежимРазделенияДанных(ОбщийРеквизит_ Как Строка, Режим_ Как Булево)
	inline void SetDataSeparationSafeMode(ConstPara<String> CommonAttribute_, ConstPara<Boolean> Mode_) { ESB_INVOKE_API_PROC2(backend, 94, CommonAttribute_, Mode_); }
	// Функция БезопасныйРежимРазделенияДанных(ОбщийРеквизит_ Как Строка) Возвращает Булево
	inline Boolean DataSeparationSafeMode(ConstPara<String> CommonAttribute_) { ESB_INVOKE_API_FUNC1(Boolean, backend, 95, CommonAttribute_); }
	// Процедура ИнициализироватьПредопределенныеДанные()
	inline void InitializePredefinedData() { ESB_INVOKE_API_PROC0(backend, 96); }
	// Процедура УстановитьВремяЗасыпанияПассивногоСеанса(Таймаут_ Как Число)
	inline void SetPassiveSessionHibernateTime(ConstPara<Numeric> Timeout_) { ESB_INVOKE_API_PROC1(backend, 101, Timeout_); }
	// Функция ПолучитьВремяЗасыпанияПассивногоСеанса() Возвращает Число
	inline Numeric GetPassiveSessionHibernateTime() { ESB_INVOKE_API_FUNC0(Numeric, backend, 102); }
	// Процедура УстановитьВремяЗавершенияСпящегоСеанса(Таймаут_ Как Число)
	inline void SetHibernateSessionTerminateTime(ConstPara<Numeric> Timeout_) { ESB_INVOKE_API_PROC1(backend, 103, Timeout_); }
	// Функция ПолучитьВремяЗавершенияСпящегоСеанса() Возвращает Число
	inline Numeric GetHibernateSessionTerminateTime() { ESB_INVOKE_API_FUNC0(Numeric, backend, 104); }
	// Функция ПолучитьИмяКлиентаЛицензирования() Возвращает Строка
	inline String GetLicensingClientName() { ESB_INVOKE_API_FUNC0(String, backend, 106); }
	// Процедура УстановитьНастройкиКлиентаЛицензирования(Имя_ Как Строка, Пароль_ Как Строка, ДополнительныйПараметр_ Как Строка)
	inline void SetLicensingClientParameters(ConstPara<String> Name_, ConstPara<String> Password_, ConstPara<String> AdditionalOption_ = DefUndef<String>) { ESB_INVOKE_API_PROC3(backend, 107, Name_, Password_, AdditionalOption_); }
	// Функция ПолучитьДополнительныйПараметрКлиентаЛицензирования() Возвращает Строка
	inline String GetLicensingClientAdditionalParameter() { ESB_INVOKE_API_FUNC0(String, backend, 108); }


	// Функция КодироватьСтроку(Строка_ Как Строка, СпособКодированияСтроки_ Как СпособКодированияСтроки, КодировкаСтроки_ Как Строка) Возвращает Строка
	inline String EncodeString(ConstPara<String> String_, ConstPara<StringEncodingMethodValue> StringEncodingMethod_, ConstPara<String> StringEncoding_ = DefUndef<String>) { ESB_INVOKE_API_FUNC3(String, entext2, 0, String_, StringEncodingMethod_, StringEncoding_); }
	// Функция РаскодироватьСтроку(Строка_ Как Строка, СпособКодированияСтроки_ Как СпособКодированияСтроки, КодировкаСтроки_ Как Строка) Возвращает Строка
	inline String DecodeString(ConstPara<String> String_, ConstPara<StringEncodingMethodValue> StringEncodingMethod_, ConstPara<String> StringEncoding_ = DefUndef<String>) { ESB_INVOKE_API_FUNC3(String, entext2, 1, String_, StringEncodingMethod_, StringEncoding_); }
	// Функция ПолучитьМаскуВсеФайлыСервера() Возвращает Строка
	inline String GetServerAllFilesMask() { ESB_INVOKE_API_FUNC0(String, entext2, 2); }
	// Функция ПолучитьРазделительПутиСервера() Возвращает Строка
	inline String GetServerPathSeparator() { ESB_INVOKE_API_FUNC0(String, entext2, 3); }
	// Процедура УстановитьРасширениеРаботыСКриптографией()
	inline void InstallCryptoExtension() { ESB_INVOKE_API_PROC0(extui, 0); }
	// Процедура НачатьУстановкуРасширенияРаботыСКриптографией(ОписаниеОповещенияОЗавершении_ Как ОписаниеОповещения)
	inline void BeginInstallCryptoExtension(ConstPara<NotifyDescription> NotifyDescriptionOnCompletion_) { ESB_INVOKE_API_PROC1(extui, 1, NotifyDescriptionOnCompletion_); }
	// Функция ПодключитьРасширениеРаботыСКриптографией() Возвращает Булево
	inline Boolean AttachCryptoExtension() { ESB_INVOKE_API_FUNC0(Boolean, extui, 2); }
	// Процедура НачатьПодключениеРасширенияРаботыСКриптографией(ОписаниеОповещения_ Как ОписаниеОповещения)
	inline void BeginAttachingCryptoExtension(ConstPara<NotifyDescription> NotifyDescription_) { ESB_INVOKE_API_PROC1(extui, 3, NotifyDescription_); }
	// Процедура Предупреждение(ТекстПредупреждения_ Как [Строка,ФорматированнаяСтрока], Таймаут_ Как Число, Заголовок_ Как Строка)
	inline void DoMessageBox(ConstPara<String> MessageText_, ConstPara<Numeric> Timeout_ = DefUndef<Numeric>, ConstPara<String> Title_ = DefUndef<String>) { ESB_INVOKE_API_PROC3(frntend, 0, MessageText_, Timeout_, Title_); }
	inline void DoMessageBox(ConstPara<FormattedString> MessageText_, ConstPara<Numeric> Timeout_ = DefUndef<Numeric>, ConstPara<String> Title_ = DefUndef<String>) { ESB_INVOKE_API_PROC3(frntend, 0, MessageText_, Timeout_, Title_); }
	// Функция Вопрос(ТекстВопроса_ Как [Строка,ФорматированнаяСтрока], Кнопки_ Как [РежимДиалогаВопрос,СписокЗначений], Таймаут_ Как Число, КнопкаПоУмолчанию_ Как Произвольный, Заголовок_ Как Строка, КнопкаТаймаута_ Как Произвольный) Возвращает Произвольный
	inline Arbitrary DoQueryBox(ConstPara<String> QueryText_, ConstPara<QuestionDialogModeValue> Buttons_, ConstPara<Numeric> Timeout_ = DefUndef<Numeric>, ConstPara<Arbitrary> DefaultButton_ = DefUndef<Arbitrary>, ConstPara<String> Title_ = DefUndef<String>, ConstPara<Arbitrary> TimeoutButton_ = DefUndef<Arbitrary>) { ESB_INVOKE_API_FUNC6(Arbitrary, frntend, 1, QueryText_, Buttons_, Timeout_, DefaultButton_, Title_, TimeoutButton_); }
	inline Arbitrary DoQueryBox(ConstPara<String> QueryText_, ConstPara<ValueList> Buttons_, ConstPara<Numeric> Timeout_ = DefUndef<Numeric>, ConstPara<Arbitrary> DefaultButton_ = DefUndef<Arbitrary>, ConstPara<String> Title_ = DefUndef<String>, ConstPara<Arbitrary> TimeoutButton_ = DefUndef<Arbitrary>) { ESB_INVOKE_API_FUNC6(Arbitrary, frntend, 1, QueryText_, Buttons_, Timeout_, DefaultButton_, Title_, TimeoutButton_); }
	inline Arbitrary DoQueryBox(ConstPara<FormattedString> QueryText_, ConstPara<QuestionDialogModeValue> Buttons_, ConstPara<Numeric> Timeout_ = DefUndef<Numeric>, ConstPara<Arbitrary> DefaultButton_ = DefUndef<Arbitrary>, ConstPara<String> Title_ = DefUndef<String>, ConstPara<Arbitrary> TimeoutButton_ = DefUndef<Arbitrary>) { ESB_INVOKE_API_FUNC6(Arbitrary, frntend, 1, QueryText_, Buttons_, Timeout_, DefaultButton_, Title_, TimeoutButton_); }
	inline Arbitrary DoQueryBox(ConstPara<FormattedString> QueryText_, ConstPara<ValueList> Buttons_, ConstPara<Numeric> Timeout_ = DefUndef<Numeric>, ConstPara<Arbitrary> DefaultButton_ = DefUndef<Arbitrary>, ConstPara<String> Title_ = DefUndef<String>, ConstPara<Arbitrary> TimeoutButton_ = DefUndef<Arbitrary>) { ESB_INVOKE_API_FUNC6(Arbitrary, frntend, 1, QueryText_, Buttons_, Timeout_, DefaultButton_, Title_, TimeoutButton_); }
	// Функция ВвестиСтроку(Строка_ Как Строка, Подсказка_ Как Строка, Длина_ Как Число, Многострочность_ Как Булево) Возвращает Булево
	inline Boolean InputString(ConstPara<String> String_, ConstPara<String> Tooltip_ = DefUndef<String>, ConstPara<Numeric> Length_ = DefUndef<Numeric>, ConstPara<Boolean> Multiline_ = DefUndef<Boolean>) { ESB_INVOKE_API_FUNC4(Boolean, frntend, 2, String_, Tooltip_, Length_, Multiline_); }
	// Функция ВвестиЧисло(Число_ Как Число, Подсказка_ Как Строка, Длина_ Как Число, Точность_ Как Число) Возвращает Булево
	inline Boolean InputNumber(ConstPara<Numeric> Number_, ConstPara<String> Tooltip_ = DefUndef<String>, ConstPara<Numeric> Length_ = DefUndef<Numeric>, ConstPara<Numeric> Precision_ = DefUndef<Numeric>) { ESB_INVOKE_API_FUNC4(Boolean, frntend, 3, Number_, Tooltip_, Length_, Precision_); }
	// Функция ВвестиДату(Дата_ Как Дата, Подсказка_ Как Строка, ЧастьДаты_ Как ЧастиДаты) Возвращает Булево
	inline Boolean InputDate(ConstPara<DateTime> Date_, ConstPara<String> Tooltip_ = DefUndef<String>, ConstPara<DateFractionsValue> DatePart_ = DefUndef<DateFractionsValue>) { ESB_INVOKE_API_FUNC3(Boolean, frntend, 4, Date_, Tooltip_, DatePart_); }
	// Функция ВвестиЗначение(Значение_ Как Произвольный, Подсказка_ Как Строка, Тип_ Как [Тип,ОписаниеТипов]) Возвращает Булево
	inline Boolean InputValue(ConstPara<Arbitrary> Value_, ConstPara<String> Tooltip_ = DefUndef<String>, ConstPara<TypeValue> Type_ = DefUndef<TypeValue>) { ESB_INVOKE_API_FUNC3(Boolean, frntend, 5, Value_, Tooltip_, Type_); }
	inline Boolean InputValue(ConstPara<Arbitrary> Value_, ConstPara<String> Tooltip_ = DefUndef<String>, ConstPara<TypeDescription> Type_ = DefUndef<TypeDescription>) { ESB_INVOKE_API_FUNC3(Boolean, frntend, 5, Value_, Tooltip_, Type_); }
	// Процедура ОткрытьЗначение(Значение_ Как Произвольный)
	inline void OpenValue(ConstPara<Arbitrary> Value_) { ESB_INVOKE_API_PROC1(frntend, 6, Value_); }
	// Процедура Сигнал()
	inline void Beep() { ESB_INVOKE_API_PROC0(frntend, 7); }
	// Процедура СохранитьЗначение(Имя_ Как Строка, Значение_ Как Произвольный)
	inline void SaveValue(ConstPara<String> Name_, ConstPara<Arbitrary> Value_) { ESB_INVOKE_API_PROC2(frntend, 10, Name_, Value_); }
	// Функция ВосстановитьЗначение(Имя_ Как Строка) Возвращает Произвольный
	inline Arbitrary RestoreValue(ConstPara<String> Name_) { ESB_INVOKE_API_FUNC1(Arbitrary, frntend, 11, Name_); }
	// Процедура СохранитьНастройкиПользователя()
	inline void SaveUserSettings() { ESB_INVOKE_API_PROC0(frntend, 12); }
	// Процедура ОчиститьНастройкиПользователя(ИмяПользователяИБ_ Как Строка)
	inline void ClearUserSettings(ConstPara<String> IBUserName_) { ESB_INVOKE_API_PROC1(frntend, 13, IBUserName_); }
	// Процедура ЗапуститьСистему(ДополнительныеПараметрыКоманднойСтроки_ Как Строка, ДождатьсяЗавершения_ Как Булево, КодВозврата_ Как [Число,Неопределено])
	inline void RunSystem(ConstPara<String> AdditionalParametersOfCommandLine_, ConstPara<Boolean> WaitForCompletion_, ConstPara<Numeric> ReturnCode_ = DefUndef<Numeric>) { ESB_INVOKE_API_PROC3(frntend, 14, AdditionalParametersOfCommandLine_, WaitForCompletion_, ReturnCode_); }
	// Процедура ЗавершитьРаботуСистемы(ЗапрашиватьВозможность_ Как Булево, ПерезапуститьПриЗавершении_ Как Булево, ДополнительныеПараметрыКоманднойСтроки_ Как Строка)
	inline void Exit(ConstPara<Boolean> OptionRequest_, ConstPara<Boolean> RestartOnCompletion_, ConstPara<String> AdditionalParametersOfCommandLine_ = DefUndef<String>) { ESB_INVOKE_API_PROC3(frntend, 15, OptionRequest_, RestartOnCompletion_, AdditionalParametersOfCommandLine_); }
	// Процедура ПрекратитьРаботуСистемы(ПерезапуститьПриЗавершении_ Как Булево, ДополнительныеПараметрыКоманднойСтроки_ Как Строка)
	inline void Terminate(ConstPara<Boolean> RestartOnCompletion_, ConstPara<String> AdditionalParametersOfCommandLine_ = DefUndef<String>) { ESB_INVOKE_API_PROC2(frntend, 16, RestartOnCompletion_, AdditionalParametersOfCommandLine_); }
	// Процедура ПодключитьОбработчикОжидания(ИмяПроцедуры_ Как Строка, Интервал_ Как Число, Однократно_ Как Булево)
	inline void AttachIdleHandler(ConstPara<String> ProcedureName_, ConstPara<Numeric> Interval_, ConstPara<Boolean> Single_ = DefUndef<Boolean>) { ESB_INVOKE_API_PROC3(frntend, 17, ProcedureName_, Interval_, Single_); }
	// Процедура ОтключитьОбработчикОжидания(ИмяПроцедуры_ Как Строка)
	inline void DetachIdleHandler(ConstPara<String> ProcedureName_) { ESB_INVOKE_API_PROC1(frntend, 18, ProcedureName_); }
	// Процедура ЗаблокироватьРаботуПользователя()
	inline void LockApplication() { ESB_INVOKE_API_PROC0(frntend, 19); }
	// Процедура ОчиститьСообщения()
	inline void ClearMessages() { ESB_INVOKE_API_PROC0(frntend, 20); }
	// Процедура Состояние(ТекстСообщения_ Как Строка, Прогресс_ Как Число, Пояснение_ Как Строка, Картинка_ Как Картинка)
	inline void Status(ConstPara<String> MessageText_, ConstPara<Numeric> Progress_, ConstPara<String> Explanation_, ConstPara<Picture> Picture_ = DefUndef<Picture>) { ESB_INVOKE_API_PROC4(frntend, 21, MessageText_, Progress_, Explanation_, Picture_); }
	// Процедура Оповестить(ИмяСобытия_ Как Строка, Параметр_ Как Произвольный, Источник_ Как Произвольный)
	inline void Notify(ConstPara<String> EventName_, ConstPara<Arbitrary> Parameter_, ConstPara<Arbitrary> Source_ = DefUndef<Arbitrary>) { ESB_INVOKE_API_PROC3(frntend, 22, EventName_, Parameter_, Source_); }
	// Процедура ОбработкаПрерыванияПользователя()
	inline void UserInterruptProcessing() { ESB_INVOKE_API_PROC0(frntend, 23); }
	// Функция ОткрытьСодержаниеСправки() Возвращает Булево
	inline Boolean OpenHelpContent() { ESB_INVOKE_API_FUNC0(Boolean, frntend, 24); }
	// Функция ОткрытьИндексСправки(ТекстДляПоиска_ Как Строка) Возвращает Булево
	inline Boolean OpenHelpIndex(ConstPara<String> SearchText_) { ESB_INVOKE_API_FUNC1(Boolean, frntend, 25, SearchText_); }
	// Процедура ЗакрытьСправку()
	inline void CloseHelp() { ESB_INVOKE_API_PROC0(frntend, 27); }
	// Процедура ПодключитьОбработчикОповещения(ИмяПроцедуры_ Как Строка)
	inline void AttachNotificationHandler(ConstPara<String> ProcedureName_) { ESB_INVOKE_API_PROC1(frntend, 31, ProcedureName_); }
	// Процедура ОтключитьОбработчикОповещения(ИмяПроцедуры_ Как Строка)
	inline void DetachNotificationHandler(ConstPara<String> ProcedureName_) { ESB_INVOKE_API_PROC1(frntend, 32, ProcedureName_); }
	// Процедура ПоказатьИнформациюОбОшибке(ИнформацияОбОшибке_ Как ИнформацияОбОшибке)
	inline void ShowErrorInfo(ConstPara<ErrorInfoValue> ErrorInfo_) { ESB_INVOKE_API_PROC1(frntend, 33, ErrorInfo_); }
	// Процедура ВыполнитьОбработкуЗаданий(ВремяОбработки_ Как Число)
	inline void ProcessJobs(ConstPara<Numeric> ProcessingTime_) { ESB_INVOKE_API_PROC1(frntend, 34, ProcessingTime_); }
	// Функция ОткрытьФормуМодально(ИмяФормы_ Как Строка, Параметры_ Как Структура, Владелец_ Как void, НавигационнаяСсылка_ Как [Строка,Неопределено]) Возвращает Произвольный
	inline Arbitrary OpenFormModal(ConstPara<String> FormName_, ConstPara<Structure> Parameters_ = DefUndef<Structure>, ConstPara<Arbitrary> Owner_ = DefUndef<Arbitrary>, ConstPara<String> NavigationLink_ = DefUndef<String>) { ESB_INVOKE_API_FUNC4(Arbitrary, frntend, 37, FormName_, Parameters_, Owner_, NavigationLink_); }
	// Функция РабочийКаталогДанныхПользователя() Возвращает Строка
	inline String UserDataWorkDir() { ESB_INVOKE_API_FUNC0(String, frntend, 38); }
	// Функция КаталогДокументов() Возвращает Строка
	inline String DocumentsDir() { ESB_INVOKE_API_FUNC0(String, frntend, 39); }
	// Процедура НачатьПолучениеКаталогаДокументов(ОписаниеОповещения_ Как ОписаниеОповещения)
	inline void BeginGettingDocumentsDir(ConstPara<NotifyDescription> NotifyDescription_) { ESB_INVOKE_API_PROC1(frntend, 40, NotifyDescription_); }
	// Процедура НачатьПолучениеРабочегоКаталогаДанныхПользователя(ОписаниеОповещения_ Как ОписаниеОповещения)
	inline void BeginGettingUserDataWorkDir(ConstPara<NotifyDescription> NotifyDescription_) { ESB_INVOKE_API_PROC1(frntend, 41, NotifyDescription_); }
	// Процедура ПоказатьПредупреждение(ОписаниеОповещенияОЗавершении_ Как ОписаниеОповещения, ТекстПредупреждения_ Как [Строка,ФорматированнаяСтрока], Таймаут_ Как Число, Заголовок_ Как Строка)
	inline void ShowMessageBox(ConstPara<NotifyDescription> NotifyDescriptionOnCompletion_, ConstPara<String> MessageText_, ConstPara<Numeric> Timeout_ = DefUndef<Numeric>, ConstPara<String> Title_ = DefUndef<String>) { ESB_INVOKE_API_PROC4(frntend, 42, NotifyDescriptionOnCompletion_, MessageText_, Timeout_, Title_); }
	inline void ShowMessageBox(ConstPara<NotifyDescription> NotifyDescriptionOnCompletion_, ConstPara<FormattedString> MessageText_, ConstPara<Numeric> Timeout_ = DefUndef<Numeric>, ConstPara<String> Title_ = DefUndef<String>) { ESB_INVOKE_API_PROC4(frntend, 42, NotifyDescriptionOnCompletion_, MessageText_, Timeout_, Title_); }
	// Процедура ПоказатьВопрос(ОписаниеОповещенияОЗавершении_ Как ОписаниеОповещения, ТекстВопроса_ Как [Строка,ФорматированнаяСтрока], Кнопки_ Как [РежимДиалогаВопрос,СписокЗначений], Таймаут_ Как Число, КнопкаПоУмолчанию_ Как Произвольный, Заголовок_ Как Строка, КнопкаТаймаута_ Как Произвольный)
	inline void ShowQueryBox(ConstPara<NotifyDescription> NotifyDescriptionOnCompletion_, ConstPara<String> QueryText_, ConstPara<QuestionDialogModeValue> Buttons_, ConstPara<Numeric> Timeout_ = DefUndef<Numeric>, ConstPara<Arbitrary> DefaultButton_ = DefUndef<Arbitrary>, ConstPara<String> Title_ = DefUndef<String>, ConstPara<Arbitrary> TimeoutButton_ = DefUndef<Arbitrary>) { ESB_INVOKE_API_PROC7(frntend, 43, NotifyDescriptionOnCompletion_, QueryText_, Buttons_, Timeout_, DefaultButton_, Title_, TimeoutButton_); }
	inline void ShowQueryBox(ConstPara<NotifyDescription> NotifyDescriptionOnCompletion_, ConstPara<String> QueryText_, ConstPara<ValueList> Buttons_, ConstPara<Numeric> Timeout_ = DefUndef<Numeric>, ConstPara<Arbitrary> DefaultButton_ = DefUndef<Arbitrary>, ConstPara<String> Title_ = DefUndef<String>, ConstPara<Arbitrary> TimeoutButton_ = DefUndef<Arbitrary>) { ESB_INVOKE_API_PROC7(frntend, 43, NotifyDescriptionOnCompletion_, QueryText_, Buttons_, Timeout_, DefaultButton_, Title_, TimeoutButton_); }
	inline void ShowQueryBox(ConstPara<NotifyDescription> NotifyDescriptionOnCompletion_, ConstPara<FormattedString> QueryText_, ConstPara<QuestionDialogModeValue> Buttons_, ConstPara<Numeric> Timeout_ = DefUndef<Numeric>, ConstPara<Arbitrary> DefaultButton_ = DefUndef<Arbitrary>, ConstPara<String> Title_ = DefUndef<String>, ConstPara<Arbitrary> TimeoutButton_ = DefUndef<Arbitrary>) { ESB_INVOKE_API_PROC7(frntend, 43, NotifyDescriptionOnCompletion_, QueryText_, Buttons_, Timeout_, DefaultButton_, Title_, TimeoutButton_); }
	inline void ShowQueryBox(ConstPara<NotifyDescription> NotifyDescriptionOnCompletion_, ConstPara<FormattedString> QueryText_, ConstPara<ValueList> Buttons_, ConstPara<Numeric> Timeout_ = DefUndef<Numeric>, ConstPara<Arbitrary> DefaultButton_ = DefUndef<Arbitrary>, ConstPara<String> Title_ = DefUndef<String>, ConstPara<Arbitrary> TimeoutButton_ = DefUndef<Arbitrary>) { ESB_INVOKE_API_PROC7(frntend, 43, NotifyDescriptionOnCompletion_, QueryText_, Buttons_, Timeout_, DefaultButton_, Title_, TimeoutButton_); }
	// Процедура ПоказатьЗначение(ОписаниеОповещенияОЗавершении_ Как ОписаниеОповещения, Значение_ Как Произвольный)
	inline void ShowValue(ConstPara<NotifyDescription> NotifyDescriptionOnCompletion_, ConstPara<Arbitrary> Value_) { ESB_INVOKE_API_PROC2(frntend, 44, NotifyDescriptionOnCompletion_, Value_); }
	// Процедура ПоказатьВводДаты(ОписаниеОповещенияОЗавершении_ Как ОписаниеОповещения, Дата_ Как Дата, Подсказка_ Как Строка, ЧастьДаты_ Как ЧастиДаты)
	inline void ShowInputDate(ConstPara<NotifyDescription> NotifyDescriptionOnCompletion_, ConstPara<DateTime> Date_ = DefUndef<DateTime>, ConstPara<String> Tooltip_ = DefUndef<String>, ConstPara<DateFractionsValue> DatePart_ = DefUndef<DateFractionsValue>) { ESB_INVOKE_API_PROC4(frntend, 45, NotifyDescriptionOnCompletion_, Date_, Tooltip_, DatePart_); }
	// Процедура ПоказатьВводЗначения(ОписаниеОповещенияОЗавершении_ Как ОписаниеОповещения, Значение_ Как Произвольный, Подсказка_ Как Строка, Тип_ Как [Тип,ОписаниеТипов])
	inline void ShowInputValue(ConstPara<NotifyDescription> NotifyDescriptionOnCompletion_, ConstPara<Arbitrary> Value_, ConstPara<String> Tooltip_ = DefUndef<String>, ConstPara<TypeValue> Type_ = DefUndef<TypeValue>) { ESB_INVOKE_API_PROC4(frntend, 46, NotifyDescriptionOnCompletion_, Value_, Tooltip_, Type_); }
	inline void ShowInputValue(ConstPara<NotifyDescription> NotifyDescriptionOnCompletion_, ConstPara<Arbitrary> Value_, ConstPara<String> Tooltip_ = DefUndef<String>, ConstPara<TypeDescription> Type_ = DefUndef<TypeDescription>) { ESB_INVOKE_API_PROC4(frntend, 46, NotifyDescriptionOnCompletion_, Value_, Tooltip_, Type_); }
	// Процедура ПодключитьОбработчикЗапросаНастроекКлиентаЛицензирования(ИмяПроцедуры_ Как Строка)
	inline void AttachLicensingClientParametersRequestHandler(ConstPara<String> ProcedureName_) { ESB_INVOKE_API_PROC1(frntend, 49, ProcedureName_); }
	// Процедура ОтключитьОбработчикЗапросаНастроекКлиентаЛицензирования()
	inline void DetachLicensingClientParametersRequestHandler() { ESB_INVOKE_API_PROC0(frntend, 50); }
	// Функция ПолучитьФункциональнуюОпцию(Имя_ Как Строка, Параметры_ Как Структура) Возвращает Произвольный
	inline Arbitrary GetFunctionalOption(ConstPara<String> Name_, ConstPara<Structure> Parameters_ = DefUndef<Structure>) { ESB_INVOKE_API_FUNC2(Arbitrary, mngbase, 2, Name_, Parameters_); }
	// Функция ПоместитьВоВременноеХранилище(Данные_ Как Произвольный, Адрес_ Как [УникальныйИдентификатор,Строка]) Возвращает Строка
	inline String PutToTempStorage(ConstPara<Arbitrary> Data_, ConstPara<UUIDValue> Address_ = DefUndef<UUIDValue>) { ESB_INVOKE_API_FUNC2(String, mngbase, 3, Data_, Address_); }
	inline String PutToTempStorage(ConstPara<Arbitrary> Data_, ConstPara<String> Address_ = DefUndef<String>) { ESB_INVOKE_API_FUNC2(String, mngbase, 3, Data_, Address_); }
	// Функция ПолучитьИзВременногоХранилища(Адрес_ Как Строка) Возвращает Произвольный
	inline Arbitrary GetFromTempStorage(ConstPara<String> Address_) { ESB_INVOKE_API_FUNC1(Arbitrary, mngbase, 4, Address_); }
	// Процедура УдалитьИзВременногоХранилища(Адрес_ Как Строка)
	inline void DeleteFromTempStorage(ConstPara<String> Address_) { ESB_INVOKE_API_PROC1(mngbase, 5, Address_); }
	// Функция ПолучитьПолноеИмяПредопределенногоЗначения(ПредопределенноеЗначение_ Как Произвольный) Возвращает Строка
	inline String GetPredefinedValueFullName(ConstPara<Arbitrary> PredefinedValue_) { ESB_INVOKE_API_FUNC1(String, mngbase, 7, PredefinedValue_); }
	// Функция ПолучитьМаскуВсеФайлыКлиента() Возвращает Строка
	inline String GetClientAllFilesMask() { ESB_INVOKE_API_FUNC0(String, mngbase, 11); }
	// Функция ПолучитьРазделительПутиКлиента() Возвращает Строка
	inline String GetClientPathSeparator() { ESB_INVOKE_API_FUNC0(String, mngbase, 12); }
	// Функция ПолучитьПредставленияНавигационныхСсылок(МассивСсылок_ Как Массив) Возвращает Массив
	inline Array GetURLsPresentations(ConstPara<Array> ReferencesArrray_) { ESB_INVOKE_API_FUNC1(Array, mngbase, 17, ReferencesArrray_); }
	// Процедура УстановитьСоединениеСВнешнимИсточникомДанных(ИмяВнешнегоИсточникаДанных_ Как Строка)
	inline void ConnectExternalDataSource(ConstPara<String> ExternalDataSourceName_) { ESB_INVOKE_API_PROC1(mngbase, 18, ExternalDataSourceName_); }
	// Процедура РазорватьСоединениеСВнешнимИсточникомДанных(ИмяВнешнегоИсточникаДанных_ Как Строка)
	inline void DisconnectExternalDataSource(ConstPara<String> ExternalDataSourceName_) { ESB_INVOKE_API_PROC1(mngbase, 19, ExternalDataSourceName_); }
	// Функция ЭтоАдресВременногоХранилища(Адрес_ Как Строка) Возвращает Булево
	inline Boolean IsTempStorageURL(ConstPara<String> Address_) { ESB_INVOKE_API_FUNC1(Boolean, mngcore, 1, Address_); }
	// Функция ПолучитьНавигационнуюСсылкуИнформационнойБазы() Возвращает Строка
	inline String GetInfoBaseURL() { ESB_INVOKE_API_FUNC0(String, mngcore, 2); }
	// Функция ПолучитьИнформациюЭкрановКлиента() Возвращает ФиксированныйМассив
	inline FixedArray GetClientDisplaysInformation() { ESB_INVOKE_API_FUNC0(FixedArray, mngcore, 3); }
	// Функция ПолучитьМаскуВсеФайлы() Возвращает Строка
	inline String GetAllFilesMask() { ESB_INVOKE_API_FUNC0(String, mngcore, 4); }
	// Функция ПолучитьРазделительПути() Возвращает Строка
	inline String GetPathSeparator() { ESB_INVOKE_API_FUNC0(String, mngcore, 5); }
	// Процедура УстановитьСоставСтандартногоИнтерфейсаOData(ОбъектыМетаданных_ Как Массив)
	inline void SetStandardODataInterfaceContent(ConstPara<Array> MetadataObjects_) { ESB_INVOKE_API_PROC1(odata, 0, MetadataObjects_); }
	// Функция ПолучитьСоставСтандартногоИнтерфейсаOData() Возвращает Массив
	inline Array GetStandardODataInterfaceContent() { ESB_INVOKE_API_FUNC0(Array, odata, 1); }
	// Функция ИзXMLТипа(ИмяТипа_ Как Строка, URIПространстваИмен_ Как Строка) Возвращает [Тип,Неопределено]
	inline UndefOr<TypeValue> FromXMLType(ConstPara<String> TypeName_, ConstPara<String> NamespaceURI_) { ESB_INVOKE_API_FUNC2(UndefOr<TypeValue>, xml2, 4, TypeName_, NamespaceURI_); }
	// Функция НайтиНедопустимыеСимволыXML(СтрокаСимволов_ Как Строка, ПозицияНачала_ Как Число, Версия_ Как Строка) Возвращает Число
	inline Numeric FindDisallowedXMLCharacters(ConstPara<String> CharacterString_, ConstPara<Numeric> StartPosition_ = DefUndef<Numeric>, ConstPara<String> Version_ = DefUndef<String>) { ESB_INVOKE_API_FUNC3(Numeric, xml2, 9, CharacterString_, StartPosition_, Version_); }
	// Функция ЧислоПрописью(Число_ Как Число, ФорматнаяСтрока_ Как Строка, ПараметрыПредметаИсчисления_ Как Строка) Возвращает Строка
	inline String NumberInWords(ConstPara<Numeric> Number_, ConstPara<String> FormatString_ = DefUndef<String>, ConstPara<String> NumerationItemOptions_ = DefUndef<String>) { ESB_INVOKE_API_FUNC3(String, ext, 0, Number_, FormatString_, NumerationItemOptions_); }
	// Функция ПредставлениеПериода(ДатаНачалаПериода_ Как Дата, ДатаОкончанияПериода_ Как Дата, ФорматнаяСтрока_ Как Строка) Возвращает Строка
	inline String PeriodPresentation(ConstPara<DateTime> BeginningDateOfPeriod_, ConstPara<DateTime> EndingDateOfPeriod_, ConstPara<String> FormatString_ = DefUndef<String>) { ESB_INVOKE_API_FUNC3(String, ext, 1, BeginningDateOfPeriod_, EndingDateOfPeriod_, FormatString_); }
	// Процедура КопироватьФайл(ИмяФайлаИсточника_ Как Строка, ИмяФайлаПриемника_ Как Строка)
	inline void FileCopy(ConstPara<String> SourceFileName_, ConstPara<String> ReceiverFileName_) { ESB_INVOKE_API_PROC2(ext, 2, SourceFileName_, ReceiverFileName_); }
	// Процедура ПереместитьФайл(ИмяФайлаИсточника_ Как Строка, ИмяФайлаПриемника_ Как Строка)
	inline void MoveFile(ConstPara<String> SourceFileName_, ConstPara<String> ReceiverFileName_) { ESB_INVOKE_API_PROC2(ext, 3, SourceFileName_, ReceiverFileName_); }
	// Процедура УдалитьФайлы(Путь_ Как Строка, Маска_ Как Строка)
	inline void DeleteFiles(ConstPara<String> Path_, ConstPara<String> Mask_ = DefUndef<String>) { ESB_INVOKE_API_PROC2(ext, 4, Path_, Mask_); }
	// Функция НайтиФайлы(Путь_ Как Строка, Маска_ Как Строка, ИскатьВПодкаталогах_ Как Булево) Возвращает Массив
	inline Array FindFiles(ConstPara<String> Path_, ConstPara<String> Mask_ = DefUndef<String>, ConstPara<Boolean> SearchInSubdirectories_ = DefUndef<Boolean>) { ESB_INVOKE_API_FUNC3(Array, ext, 5, Path_, Mask_, SearchInSubdirectories_); }
	// Процедура СоздатьКаталог(ИмяКаталога_ Как Строка)
	inline void CreateDirectory(ConstPara<String> DirectoryName_) { ESB_INVOKE_API_PROC1(ext, 6, DirectoryName_); }
	// Функция РазделитьФайл(ИмяФайла_ Как Строка, РазмерЧасти_ Как Число, Путь_ Как Строка) Возвращает Массив
	inline Array SplitFile(ConstPara<String> FileName_, ConstPara<Numeric> SegmentSize_, ConstPara<String> Path_ = DefUndef<String>) { ESB_INVOKE_API_FUNC3(Array, ext, 8, FileName_, SegmentSize_, Path_); }
	// Процедура ОбъединитьФайлы(ШаблонПоиска_ Как Строка, ИмяРезультирующегоФайла_ Как Строка)
	inline void MergeFiles(ConstPara<String> SearchTemplate_, ConstPara<String> ResultingFileName_) { ESB_INVOKE_API_PROC2(ext, 9, SearchTemplate_, ResultingFileName_); }
	// Процедура ОбъединитьФайлы(ИменаЧастей_ Как Массив, ИмяРезультирующегоФайла_ Как Строка)
	inline void MergeFiles(ConstPara<Array> SegmentNames_, ConstPara<String> ResultingFileName_) { ESB_INVOKE_API_PROC2(ext, 9, SegmentNames_, ResultingFileName_); }
	// Процедура ЗапуститьПриложение(СтрокаКоманды_ Как Строка, ТекущийКаталог_ Как Строка, ДождатьсяЗавершения_ Как Булево, КодВозврата_ Как [Число,Неопределено])
	inline void RunApp(ConstPara<String> CommandString_, ConstPara<String> CurrentDirectory_ = DefUndef<String>, ConstPara<Boolean> WaitForCompletion_ = DefUndef<Boolean>, ConstPara<Numeric> ReturnCode_ = DefUndef<Numeric>) { ESB_INVOKE_API_PROC4(ext, 10, CommandString_, CurrentDirectory_, WaitForCompletion_, ReturnCode_); }
	// Функция ТекущаяУниверсальнаяДатаВМиллисекундах() Возвращает Число
	inline Numeric CurrentUniversalDateInMilliseconds() { ESB_INVOKE_API_FUNC0(Numeric, ext, 11); }
	// Процедура НачатьКопированиеФайла(ОписаниеОповещения_ Как ОписаниеОповещения, ИмяФайлаИсточника_ Как Строка, ИмяФайлаПриемника_ Как Строка)
	inline void BeginCopyingFile(ConstPara<NotifyDescription> NotifyDescription_, ConstPara<String> SourceFileName_, ConstPara<String> ReceiverFileName_) { ESB_INVOKE_API_PROC3(ext, 12, NotifyDescription_, SourceFileName_, ReceiverFileName_); }
	// Процедура НачатьПеремещениеФайла(ОписаниеОповещения_ Как ОписаниеОповещения, ИмяФайлаИсточника_ Как Строка, ИмяФайлаПриемника_ Как Строка)
	inline void BeginMovingFile(ConstPara<NotifyDescription> NotifyDescription_, ConstPara<String> SourceFileName_, ConstPara<String> ReceiverFileName_) { ESB_INVOKE_API_PROC3(ext, 13, NotifyDescription_, SourceFileName_, ReceiverFileName_); }
	// Процедура НачатьУдалениеФайлов(ОписаниеОповещения_ Как ОписаниеОповещения, Путь_ Как Строка, Маска_ Как void)
	inline void BeginDeletingFiles(ConstPara<NotifyDescription> NotifyDescription_, ConstPara<String> Path_, ConstPara<Arbitrary> Mask_ = DefUndef<Arbitrary>) { ESB_INVOKE_API_PROC3(ext, 14, NotifyDescription_, Path_, Mask_); }
	// Процедура НачатьПоискФайлов(ОписаниеОповещения_ Как ОписаниеОповещения, Путь_ Как Строка, Маска_ Как Строка, ИскатьВПодкаталогах_ Как Булево)
	inline void BeginFindingFiles(ConstPara<NotifyDescription> NotifyDescription_, ConstPara<String> Path_, ConstPara<String> Mask_ = DefUndef<String>, ConstPara<Boolean> SearchInSubdirectories_ = DefUndef<Boolean>) { ESB_INVOKE_API_PROC4(ext, 15, NotifyDescription_, Path_, Mask_, SearchInSubdirectories_); }
	// Процедура НачатьСозданиеКаталога(ОписаниеОповещения_ Как ОписаниеОповещения, ИмяКаталога_ Как Строка)
	inline void BeginCreatingDirectory(ConstPara<NotifyDescription> NotifyDescription_, ConstPara<String> DirectoryName_) { ESB_INVOKE_API_PROC2(ext, 16, NotifyDescription_, DirectoryName_); }
	// Процедура НачатьЗапускПриложения(ОписаниеОповещения_ Как ОписаниеОповещения, СтрокаКоманды_ Как Строка, ТекущийКаталог_ Как Строка, ДождатьсяЗавершения_ Как Булево)
	inline void BeginRunningApplication(ConstPara<NotifyDescription> NotifyDescription_, ConstPara<String> CommandLine_, ConstPara<String> CurrentDirectory_ = DefUndef<String>, ConstPara<Boolean> WaitForCompletion_ = DefUndef<Boolean>) { ESB_INVOKE_API_PROC4(ext, 17, NotifyDescription_, CommandLine_, CurrentDirectory_, WaitForCompletion_); }


	// Функция ПолучитьИмяВременногоФайла(Расширение_ Как Строка) Возвращает Строка
	// (конфликтует с WinApi GetTempFileName поэтому имя изменено)
	inline String GetTempFileNameFrom1C(ConstPara<String> Extension_) { ESB_INVOKE_API_FUNC1(String, backend, 87, Extension_); }

	// Функция СмещениеСтандартногоВремени(ЧасовойПояс_ Как [Строка,Неопределено], УниверсальноеВремя_ Как [Дата,Неопределено]) Возвращает Дата
	inline DateTime StandardTimeOffset(ConstPara<String> TimeZone_ = DefUndef<String>, ConstPara<DateTime> UniversalTime_ = DefUndef<DateTime>) { ESB_INVOKE_API_FUNC2(DateTime, backend, 85, TimeZone_, UniversalTime_); }

	// Свойство [[Чтение,Запись]] РабочаяДата Как Дата
	// *Доступно для записи в случае, если свойство ИспользованиеРабочейДаты имеет значение Назначать
	inline DateTime GetWorkingDate() { ESB_INVOKE_API_PROP_GET(DateTime, frntend, 1); }	
	// Свойство [[Только Чтение]] ПараметрЗапуска Как Строка
	inline String GetLaunchParameter() { ESB_INVOKE_API_PROP_GET(String, frntend, 4); }


// эти функции зависят от типов esb_data. здесь только объявления, а определения там
// 
	// Функция НайтиПоСсылкам(СписокСсылок_ Как Массив, ОбластьПоиска_ Как Массив, ВключитьОбъекты_ Как Массив, ИсключитьОбъекты_ Как Массив) Возвращает ТаблицаЗначений
	inline ValueTable FindByRef(ConstPara<Array> ReferenceList_, ConstPara<Array> SearchArea_ = DefUndef<Array>, ConstPara<Array> IncludeObjects_ = DefUndef<Array>, ConstPara<Array> ExcludeObjects_ = DefUndef<Array>);
	// Функция ПолучитьСтруктуруХраненияБазыДанных(ОбъектыМетаданных_ Как Массив, ИменаБазыДанных_ Как Булево) Возвращает ТаблицаЗначений
	inline ValueTable GetDBStorageStructureInfo(ConstPara<Array> MetadataObjects_, ConstPara<Boolean> DBMSNames_ = DefUndef<Boolean>);
	// Функция ПользователиWindows() Возвращает [ТаблицаЗначений,Массив]
	inline ClassMix<ValueTable, Array> WindowsUsers();
	// Функция ПользователиОС() Возвращает [ТаблицаЗначений,Массив]
	inline ClassMix<ValueTable, Array> OSUsers();
	// Функция ПолучитьДанныеВыбора(ТипЗначения_ Как Тип, Параметры_ Как Структура) Возвращает СписокЗначений
	inline ValueList GetChoiceData(ConstPara<TypeValue> ValueType_, ConstPara<Structure> Parameters_);
//

// эти функции зависят от типов esb_file. здесь только объявления, а определения там
//
	// Функция Base64Значение(Строка_ Как Строка) Возвращает ДвоичныеДанные
	inline BinaryData Base64Value(ConstPara<String> Line_);
//...
// end from providers -----------------------------------------------------



// from providers manual ----------------------------------------------------->>> 
	// Функция СтрШаблон(Шаблон_ Как Строка, Значение1 - Значение10_ Как Произвольный) Возвращает Строка
	//  - функция с переменным кол-вом аргументов
	//  - Строка, содержащая маркеры подстановки вида : "%1..%N".Нумерация маркеров начинается с 1. N не может быть более 10.
	//  - Если требуется сразу после номера подстановки написать цифру, то для номер подставновки должен быть взят в скобки.Тогда строка может иметь следующий вид : "%(1)1cv8с.exe"
	template<EsbClassConcept ... EsbClassArgsT>
	requires (sizeof...(EsbClassArgsT) <= 10)
	inline String StrTemplate(ConstPara<String> Template_, ConstPara< EsbClassArgsT>... args_) { ESB_INVOKE_API_FUNCX(String, core, 5, args_); }
// <<< end from providers manual -----------------------------------------------------





// api native (not present in contexts)
// 
// КОНСТРУКТОРЫ ЗНАЧЕНИЙ (которые Boolean, Numeric и т.п. - у нас здесь это имена классов - поэтому к функциям добавил To...)
// NOTE	Возможно классам сделать такие шаблонные конструкторы и будет в точность как в 1С, но... конструкторы иногда вызываются имплисит 
//		или вооще неожиданно или конфликтовать начинают.. Не считаю целесообразным это делать.
template<class EsbClassT>
inline Boolean ToBoolean(const EsbClassT& Value_) {
	if constexpr (std::is_same_v<std::remove_cvref_t<EsbClassT>, Numeric>)
		return (static_cast<int>(Value_))? Boolean::True_ : Boolean::False_;
	else if constexpr (has_meth_ToBoolean<EsbClassT>)
		return Value_.ESB_CONVERTER_METH_ToBoolean();
	else {
		IValuePtr pv = IValue_ToBoolean(*get_interface(Value_));
		if (pv)
			return make<Boolean>(std::move(pv));
		else
			ESL_THROW_NOTBOOL();
	}
}

template<class EsbClassT>
inline Numeric ToNumeric(const EsbClassT& Value_) {
	if constexpr (std::is_same_v<std::remove_cvref_t<EsbClassT>, Boolean>)
		return (static_cast<bool>(Value_))? Numeric::Value_1_ : Numeric::Value_0_;
	else if constexpr (has_meth_ToNumeric<EsbClassT>)
		return Value_.ESB_CONVERTER_METH_ToNumeric();
	else {
		//Значение типа Строка преобразуется в число в том случае, если параметр является строковым представлением литерала численного типа.
		//При этом строковое представление анализируется как с учетом региональных установок информационной базы, так и без их учета.
		//В противном случае будет выдано сообщение об ошибке выполнения функции.
		IValuePtr pv = IValue_ToNumeric(*get_interface(Value_));
		if (pv)
			return make<Numeric>(std::move(pv));
		else
			ESL_THROW_NOTNUMBER();
	}
}
template<class EsbClassT>
inline Numeric ToNumber(const EsbClassT& Value_) {
	return ToNumeric(Value_);
}
// специальный случая для Boolean т.к. может быть constexpr и возвращать лвал (это используется в Min/Max)
inline constexpr const Numeric& ToNumeric(const Boolean& Value_) {
	return (Value_) ? Numeric::Value_1_ : Numeric::Value_0_;
}
inline constexpr const Numeric& ToNumber(const Boolean& Value_) {
	return ToNumeric(Value_);// (Value_) ? Numeric::Value_1_ : Numeric::Value_0_;
}

template<class EsbClassT>
inline String ToString(const EsbClassT& Value_) {
	if constexpr (has_meth_ToString<EsbClassT>)
		return Value_.ESB_CONVERTER_METH_ToString();
	else {
		// Гхм... --Преобразование к строке значений остальных типов производится **по мере возможности** и обычно содержит **читабельное** представление значения.---> ??
		// будем бросать ошибку по аналогии с другими методами.
		IValuePtr pv = IValue_ToString(*get_interface(static_cast<const Value&>(Value_)));
		if (pv)
			return make<String>(std::move(pv));
		else
			ESL_THROW_NOTSTRING();
	}
}

template<class EsbClassT>
inline DateTime ToDate(const EsbClassT& Value_) {
	if constexpr (has_meth_ToDate<EsbClassT>)
		return Value_.ESB_CONVERTER_METH_ToDate();
	else {
		IValuePtr pv = IValue_ToDate(*get_interface(Value_));
		if (pv)
			return make<DateTime>(std::move(pv));
		else
			ESL_THROW_NOTDATE();
	}
}
//DateTime Date(ConstPara<Numeric> Year_, ConstPara<Numeric> Month_, ConstPara<Numeric> Day_, ArgByValOpt<Numeric> Hour_ = Undef::Value_, ArgByValOpt<Numeric> Minute_ = Undef::Value_, ArgByValOpt<Numeric> Second_ = Undef::Value_)
//Numeric NumberFromBinaryString(ConstPara<StringValue> BinaryString_)
//---------------------------------





// template<std::same_as<Numeric>... NumericT> 
// ClassMix<Numeric, StringValue, DateTime, BoolValue> 
// Min(ConstPara<Numeric> Value1_, ArgErrUnk<Arbitrary> _, ConstPara<NumericT> ...ValueN_)
// 
// Тот или иной семантический вариант функции определяется по типу данных первого параметра. 
// В случае несовпадения типа второго и последующих параметров с требуемым, их значения преобразуются к требуемому типу в соответствии с правилами преобразования типов. 
// Если тип первого параметра не соответствует ни одному из допустимых типов, 
// то в зависимости от ситуации может производиться преобразование типов или возбуждаться состояние ошибки исполнения.
// Гхм...
//	- если первый Дата, то остальные  должны быть Дата и ничего никуда не конвертируется, или ошибка. Сравнение просто <
//	- если первый Строка, то остальные должны быть строка и ничего никуда не конвертируется. сравнивается через коллатор
//	- если первый Число, то остальные могут быть либо Число, либо Буул (Буул приводится к 0/1). Возвращается минимальное: т.е. Мин(10, Истина) дает Буул::Истина (а не число!)
//	- если первый Буул, то тоже что для числа. Также Мин(Истина,0) возвращает Число::0
//		то есть Буул в целях сравнения всегда тихо конвертируется в число, и возвращается собственно говоря (минимальное)
//		каждое очередное сравнение может перещелкивать тип результата (ну неболее двух раз)
//	допускается вызов с одним аргументом

template<class EsbClassT>
struct minmax_comparator_t {
	const EsbClassT& data_;
	const minmax_comparator_t<EsbClassT>& operator<(const minmax_comparator_t<EsbClassT>& other_) const	{ return (data_ < other_.data_)? *this : other_; }
	const minmax_comparator_t<EsbClassT>& operator>(const minmax_comparator_t<EsbClassT>& other_) const { return (data_ > other_.data_)? *this : other_; }
};

template<std::same_as<DateTime>... DataTypeT>
DateTime Min(const DateTime& a1_, const DataTypeT&... ax_) {
	//1) ( pack op ... ):	Unary right fold (E op ...) becomes (E1 op (... op (EN-1 op EN)))
	//2) ( ... op pack ):	Unary left fold(... op E) becomes(((E1 op E2) op ...) op EN)
	//3) ( pack op ... op init ):	Binary right fold (E op ... op I) becomes (E1 op (... op (EN−1 op (EN op I))))
	//4) ( init op ... op pack ):	Binary left fold (I op ... op E) becomes ((((I op E1) op E2) op ...) op EN)
	//for Min<>(a1_, __ax_1, __ax_2)
	//return (cmp_t{ a1_ }).operator<(cmp_t{ __ax_1 }).operator<(cmp_t{ __ax_2 }).it_;
	//eval from left to right -> a1 compares with __ax_1, then result compares with __ax_2 and return result of second comparision. cycle not used
	return (minmax_comparator_t{ a1_ } < ... < minmax_comparator_t{ ax_ }).data_;
}
template<std::same_as<String>... DataTypeT>
String Min(const String& a1_, const DataTypeT&... ax_) {
	return (minmax_comparator_t{ a1_ } < ... < minmax_comparator_t{ ax_ }).data_;
}
template<std::same_as<Numeric>... DataTypeT>
Numeric Min(const Numeric& a1_, const DataTypeT&... ax_) {
	return (minmax_comparator_t{ a1_ } < ... < minmax_comparator_t{ ax_ }).data_;
}
template<std::same_as<Boolean>... DataTypeT>
Boolean Min(const Boolean& a1_, const DataTypeT&... ax_) {
	return (minmax_comparator_t{ a1_ } < ... < minmax_comparator_t{ ax_ }).data_;
}

// мал клоп, да вонюч...
template<class EsbClassT>
concept NumericOrBooleanConcept = (std::same_as<Boolean, EsbClassT> || std::same_as<Numeric, EsbClassT>);

using NumericOrBooleanType = ClassMix<Numeric, Boolean>;

struct minmax_comparator_nb {
	const Value&	value_;
	const Numeric*	pcomp_;
	constexpr minmax_comparator_nb(const Numeric& val_) noexcept : value_(val_), pcomp_(&val_)				{}
	constexpr minmax_comparator_nb(const Boolean& val_) noexcept : value_(val_), pcomp_(&ToNumber(val_))	{}
	minmax_comparator_nb(const minmax_comparator_nb&) = delete;
	minmax_comparator_nb& operator=(const minmax_comparator_nb&) = delete;
	minmax_comparator_nb& operator=(minmax_comparator_nb&&) = delete;
	const minmax_comparator_nb& operator<(const minmax_comparator_nb& other_) const		{ return (*pcomp_ < *other_.pcomp_)? *this : other_; }
	const minmax_comparator_nb& operator>(const minmax_comparator_nb& other_) const		{ return (*pcomp_ > *other_.pcomp_)? *this : other_; }
	NumericOrBooleanType make_object() const											{ return make_copy<NumericOrBooleanType>(get_interface(value_));	}
};

template<NumericOrBooleanConcept Arg1T, NumericOrBooleanConcept... ArgsT>
NumericOrBooleanType Min(const Arg1T& a1_, const ArgsT&... ax_) {
	return (minmax_comparator_nb{ a1_ } < ... < minmax_comparator_nb{ ax_ }).make_object();
}


template<std::same_as<DateTime>... DataTypeT>
DateTime Max(const DateTime& a1_, const DataTypeT&... ax_) {
	return (minmax_comparator_t{ a1_ } > ... > minmax_comparator_t{ ax_ }).data_;
}
template<std::same_as<String>... DataTypeT>
String Max(const String& a1_, const DataTypeT&... ax_) {
	return (minmax_comparator_t{ a1_ } > ... > minmax_comparator_t{ ax_ }).data_;
}
template<std::same_as<Numeric>... DataTypeT>
Numeric Max(const Numeric& a1_, const DataTypeT&... ax_) {
	return (minmax_comparator_t{ a1_ } > ... > minmax_comparator_t{ ax_ }).data_;
}
template<std::same_as<Boolean>... DataTypeT>
Boolean Max(const Boolean& a1_, const DataTypeT&... ax_) {
	return (minmax_comparator_t{ a1_ } > ... > minmax_comparator_t{ ax_ }).data_;
}
template<NumericOrBooleanConcept Arg1T, NumericOrBooleanConcept... ArgsT>
NumericOrBooleanType Max(const Arg1T& a1_, const ArgsT&... ax_) {
	return (minmax_comparator_nb{ a1_ } > ... > minmax_comparator_nb{ ax_ }).make_object();
}



// навязываем использование ErrorInfo только в блоке catch
inline ErrorInfoValue ErrorInfo(const core::Exception& e_) {
	return make<ErrorInfoValue>(create_error_value(e_));
}

//TOBE: ErrorDescription как-то формируется из полей ErrorInfoValue. надо разобраться...
//Применять данную функцию имеет смысл только внутри операторных скобок Исключение - КонецПопытки, так как в ином случае она вернет пустую строку.
//StringValue ErrorDescription()



// Never
//Arbitrary Eval(ConstPara<StringValue> Expression_)
//Arbitrary ProceedWithCall(ArgByValOpt<Arbitrary> Value1ValueN_)
//ValueTable CheckScriptCircularRefs(ArgByValOpt<Arbitrary> Value_, ArgByValOpt<StringValue> ValueDescription_)
//StringValue GetExternalResourcesMode()
//void SetSafeModeOff(ArgByValOpt<BoolValue> Disable_)
//BoolValue GetSafeModeDisabled()
//BoolValue MapRepresentationSupported()
//


// ДАТА
inline DateTime EndOfHour(const DateTime& Date_)		{ return Date_.EndOfHour(); }
inline DateTime BegOfMinute(const DateTime& Date_)		{ return Date_.BegOfMinute(); }
inline DateTime EndOfMinute(const DateTime& Date_)		{ return Date_.EndOfMinute(); }
inline DateTime CurrentDate()							{ return DateTime::GetCurrentDate(); }
inline DateTime BegOfQuarter(const DateTime& Date_)		{ return Date_.BegOfQuart(); }
inline DateTime BegOfMonth(const DateTime& Date_)		{ return Date_.BegOfMonth(); }
inline DateTime EndOfMonth(const DateTime& Date_)		{ return Date_.EndOfMonth(); }
inline DateTime EndOfYear(const DateTime& Date_)		{ return Date_.EndOfYear(); }
inline DateTime BegOfWeek(const DateTime& Date_)		{ return Date_.BegOfWeek(); }
inline DateTime EndOfWeek(const DateTime& Date_)		{ return Date_.EndOfWeek(); }
inline DateTime BegOfHour(const DateTime& Date_)		{ return Date_.BegOfHour(); }
inline DateTime EndOfQuarter(const DateTime& Date_)		{ return Date_.EndOfQuart(); }
inline DateTime BegOfDay(const DateTime& Date_)			{ return Date_.BegOfDay(); }
inline DateTime EndOfDay(const DateTime& Date_)			{ return Date_.EndOfDay(); }
inline DateTime BegOfYear(const DateTime& Date_)								{ return Date_.BegOfYear(); }
inline DateTime AddMonth(const DateTime& Date_, int CountOfMonths_)				{ return Date_.AddMonth(CountOfMonths_); }
inline DateTime AddMonth(const DateTime& Date_, const Numeric& CountOfMonths_)	{ return Date_.AddMonth(CountOfMonths_); }
inline Numeric Year(const DateTime& Date_)										{ return Numeric{ Date_.GetYear() }; }
inline Numeric Month(const DateTime& Date_)				{ return Numeric{ Date_.GetMonth() }; }
inline Numeric DayOfYear(const DateTime& Date_)			{ return Numeric{ Date_.GetYearDay() }; }
inline Numeric WeekDay(const DateTime& Date_)			{ return Numeric{ Date_.GetWeekDay() }; }
inline Numeric Day(const DateTime& Date_)				{ return Numeric{ Date_.GetMonthDay() }; }
inline Numeric Hour(const DateTime& Date_)				{ return Numeric{ Date_.GetHour() }; }
inline Numeric Minute(const DateTime& Date_)			{ return Numeric{ Date_.GetMinute() }; }
inline Numeric Second(const DateTime& Date_)			{ return Numeric{ Date_.GetSecond() }; }
inline Numeric WeekOfYear(const DateTime& Date_)		{ return Numeric{ Date_.GetYearWeek() }; }
// 


// ЧИСЛО
//Numeric NumberFromHexString(ConstPara<StringValue> HexString_)
//
inline Numeric Int(const Numeric& Number_) {
	return Numeric(static_cast<int>(Number_));
}
// 


//TOBE:
// МАТЕМАТИКА
//Numeric ACos(ConstPara<Numeric> Number_)
//Numeric ASin(ConstPara<Numeric> Number_)
//Numeric ATan(ConstPara<Numeric> Number_)
//Numeric Cos(ConstPara<Numeric> Angle_)
//Numeric Exp(ConstPara<Numeric> Number_)
//Numeric Pow(ConstPara<Numeric> Base_, ConstPara<Numeric> Factor_)
//Numeric Sin(ConstPara<Numeric> Angle_)
//Numeric Sqrt(ConstPara<Numeric> Number_)
//Numeric Tan(ConstPara<Numeric> Angle_)
//Numeric Log(ConstPara<Numeric> Number_)
//Numeric Log10(ConstPara<Numeric> Number_)
//


//String Format(ConstPara<Numeric> Value_, ArgByValOpt<StringValue> FormatString_ = Undef::Value_)
//String Format(ConstPara<DateTime> Value_, ArgByValOpt<StringValue> FormatString_ = Undef::Value_)
//String Format(ConstPara<BoolValue> Value_, ArgByValOpt<StringValue> FormatString_ = Undef::Value_)
inline String Format(ConstPara<Numeric> Value_, const String& FormatString_)	{ return FormatVar(Value_, FormatString_);}
inline String Format(ConstPara<Numeric> Value_)									{ return FormatVar(Value_, String::Empty_);}
inline String Format(ConstPara<Numeric> Value_, const Undef&)					{ return FormatVar(Value_, String::Empty_); }
inline String Format(ConstPara<DateTime> Value_, const String& FormatString_)	{ return FormatVar(Value_, FormatString_); }
inline String Format(ConstPara<DateTime> Value_)								{ return FormatVar(Value_, String::Empty_); }
inline String Format(ConstPara<DateTime> Value_, const Undef&)					{ return FormatVar(Value_, String::Empty_); }
inline String Format(ConstPara<Boolean> Value_, const String& FormatString_)	{ return FormatVar(Value_, FormatString_); }
inline String Format(ConstPara<Boolean> Value_)									{ return FormatVar(Value_, String::Empty_); }
inline String Format(ConstPara<Boolean> Value_, const Undef&)					{ return FormatVar(Value_, String::Empty_); }



// ТИП
// 
template<EsbClassConcept EsbClassT>
constexpr const TypeDef& TypeDefOf = EsbClassT::Type_;;

template<EsbClassConcept EsbClassT>
constexpr const TypeDef& TypeOf() {
	return EsbClassT::Type_;
}
template<EsbClassConcept EsbClassT>
constexpr const TypeDef& TypeOf(const EsbClassT&) {
	return EsbClassT::Type_;
}
inline const TypeDef TypeOf(const Arbitrary& val_) {
	//non template<> due to different return type.
	IType* pt = IValue_GetType(*get_interface(val_));
	assert(pt);
	return TypeDef{ *pt };
}
inline const TypeDef Type(const strview_t& name_) {
	IType* pt = IType_FromName(name_);
	if (pt == nullptr)
		ESL_THROW_TYPE_BYNAME_NOT_FOUND();
	return TypeDef{ *pt };
}



// ---- СТРОКА
inline Numeric StrLen(const String& Line_) { 
	return Numeric{ Line_.length() }; 
}
inline String Left(const String& Line_, const Numeric& CountOfCharacters_) {
	int n_char = static_cast<int>(CountOfCharacters_);
	size_t len = Line_.length();
	if (len == 0 || n_char <= 0) return String::Empty_;
	return String{ std::wstring_view{Line_.c_str(), std::min((unsigned)n_char, len) } };
}
inline String Right(const String& Line_, const Numeric& CountOfCharacters_) {
	int in_char = static_cast<int>(CountOfCharacters_);
	size_t len = Line_.length();
	if (len == 0 || in_char <= 0) return String::Empty_;
	size_t un_char = static_cast<unsigned>(in_char);
	if (un_char >= len) return Line_;
	size_t off = len - un_char;
	return String{ std::wstring_view{Line_.c_str() + off, un_char} };
}
inline void trim_l(const wchar_t*& ptr_, size_t& len_) {
	while (len_ > 0 && is_space(*ptr_)) {
		++ptr_;
		--len_;
	}
}
inline void trim_r(const wchar_t*& ptr_, size_t& len_) {
	while (len_ > 0 && is_space(*ptr_)) {
		--ptr_;
		--len_;
	}
}
inline String TrimL(const String& Line_) {
	const wchar_t* p_head = Line_.c_str();
	size_t len = Line_.length();
	if (len == 0) return String::Empty_;
	trim_l(p_head, len);
	return (len == 0) ? String::Empty_ : String{ std::wstring_view{p_head, len} };
}
inline String TrimR(const String& Line_) {
	const wchar_t* p_head = Line_.c_str();
	size_t len = Line_.length();
	if (len == 0) return String::Empty_;
	assert(p_head);
	const wchar_t* p_last = p_head + len - 1;
	trim_r(p_last, len);
	return (len == 0) ? String::Empty_ : String{ std::wstring_view{p_head, len} };
}
inline String TrimAll(const String& Line_) {
	const wchar_t* p_head = Line_.c_str();
	size_t len = Line_.length();
	if (len == 0) return String::Empty_;
	assert(p_head);
	const wchar_t* p_last = p_head + len - 1;
	trim_l(p_head, len);
	trim_r(p_last, len);
	return (len == 0) ? String::Empty_ : String{ std::wstring_view{p_head, len} };
}

inline String Mid(const String& Line_, const Numeric& InitialNumber_, int CountOfCharacters_ /*= Undef::Value_*/) {
	//Начальный номер символа, с которого начинается выборка.Нумерация символов в строке начинается с 1. 
	//Если указано значение, меньшее или равное нулю, то параметр принимает значение 1.
	int pos = static_cast<int>(InitialNumber_) - 1;		//1C pos 1-based, c++ pos == (pos_1c - 1)
	if (pos < 0) pos = 0;								
	size_t len = Line_.length();
	if (len == 0 || unsigned(pos) >= len || CountOfCharacters_ == 0) 
		return String::Empty_;
	else {
		size_t count_rest = len - pos;						// 0<=(pos_1c-1)<len
		size_t count_for_mid = (CountOfCharacters_ < 0 || (unsigned)CountOfCharacters_ > count_rest)? count_rest /*assume all rest*/ : (unsigned)CountOfCharacters_;
		const wchar_t* p_1st = Line_.c_str();
		assert( p_1st && (p_1st + pos + count_for_mid) <= (p_1st + len) );		//!! new_end <= org_end
		return String{ std::wstring_view{p_1st + pos, count_for_mid} };
	}
}
inline String Mid(const String& Line_, const Numeric& InitialNumber_, const Numeric& CountOfCharacters_) {
	return Mid(Line_, InitialNumber_, static_cast<int>(CountOfCharacters_));
}
inline String Mid(const String& Line_, const Numeric& InitialNumber_) {
	return Mid(Line_, InitialNumber_, -1);
}
inline String Mid(const String& Line_, const Numeric& InitialNumber_, const Undef&) {
	return Mid(Line_, InitialNumber_, -1);
}

String Upper(const String& Line_);	//esbhlp
String Lower(const String& Line_);	//esbhlp
inline String Char(const Numeric& CharacterCode_) {
	int ich = static_cast<int>(CharacterCode_);
	wchar_t wch = static_cast<wchar_t>(ich);
	return String{ std::wstring_view{&wch,1} };
}

inline Numeric CharCode(const String& Line_, int CharacterNumber1Based_ /* =Undef::Value_ */) {
	// при задании недопустимой позиции возвращается -1
	size_t len = Line_.length();
	if (CharacterNumber1Based_ < 1 || (unsigned)CharacterNumber1Based_ > len)
		return Numeric::Value__1_;
	else {
		const wchar_t* pstr = Line_.c_str();
		assert(pstr);
		wchar_t wch = pstr[CharacterNumber1Based_ - 1];
		return Numeric{ static_cast<int>(wch) };
	}
}
inline Numeric CharCode(const String& Line_, const Numeric& CharacterNumber_)	{	return CharCode(Line_, static_cast<int>(CharacterNumber_)); }
inline Numeric CharCode(const String& Line_)									{	return CharCode(Line_, 1); }
inline Numeric CharCode(const String& Line_, const Undef&)						{	return CharCode(Line_); }

inline Numeric StrOccurrenceCount(const String& Line_, const String& SearchSubstring_) {
	const std::wstring_view source = Line_.view();
	const std::wstring_view pattern = SearchSubstring_.view();
	if (source.empty() || pattern.empty())
		return Numeric::Value_0_;
	else {
		size_t cnt = 0;
		size_t pos = source.find(pattern);
		while (pos != std::wstring_view::npos) {
			++cnt;
			pos = source.find(pattern, pos + pattern.length());
		}
		return Numeric{ cnt };
	}
}

//К незначащим символам относятся символы "пробел", "неразрывный пробел" (НПП), "табуляция" (горизонтальная Таб и вертикальная ВТаб), 
//"возврат каретки" (ВК), "перевод строки" (ПС), "перевод формы (страницы)" (ПФ). 
inline const Boolean& IsBlankString(const String& Line_) {
	size_t len = Line_.length();
	if (len == 0) return Boolean::True_;
	const wchar_t* ptr = Line_.c_str();
	assert(ptr);
	const wchar_t* ptr_end = ptr + len;
	while (ptr < ptr_end) {
		if (!is_space(*ptr)) return Boolean::False_;
		++ptr;
	}
	return Boolean::True_;
}

String StrReplace(const String& Line_, const String& SearchSubstring_, const String& ReplaceSubstring_);	//esbhlp
String Title(const String& Line_);	//esbhlp


inline Numeric StrLineCount(const String& Line_) {
	// Позволяет посчитать число строк в многострочной строке. Строки в многострочной строке разделены символами перевода строк (Символы.ПС). (LF, 0x0A (10 decimal), \n)
	// Гхм.. судя по коду разделителями продстрок также являются CR и при этом он экранирует LF. т.е. голый CR тоже разделитель
	// причем что new-line в конце строки, то он не начинает новую строку (т.е. в строке из одного \n только одна строка, а не две пустых) 
	const wchar_t* pstr = Line_.c_str();
	size_t n_lines = 1;		// в любой строке есть как минимум одна строка
	if (pstr) {
		const wchar_t* pstr_end = pstr + Line_.length();
		if (pstr != pstr_end) {
			while (true) {
				wchar_t ch = *pstr++;
				if (pstr == pstr_end)
					break;	// последний char не анализируется в любом случае - если он обычный это не важно, если разделитель - игнорируется
				else if (ch == L'\n')
					++n_lines;
				else if (ch == L'\r') {
					++n_lines;
					if (*pstr == L'\n') {
						++pstr;
						if (pstr == pstr_end) 
							break;
					} 
				}
			}
		}
	}
	return Numeric(n_lines);
}

inline String StrGetLine(const String& Line_, const Numeric& LineNumber_) {
	auto _find_line_next = [](const wchar_t*& pstr_, const wchar_t* pstr_end_) -> bool {
		while (pstr_ != pstr_end_) {
			wchar_t ch = *pstr_++;
			if (ch == L'\n') {
				return true;
			}
			else if (ch == L'\r') {
				if (pstr_ != pstr_end_ && *pstr_ == L'\n') ++pstr_;
				return true;
			}
		}
		return false;
	};
	auto _find_line = [&_find_line_next](const wchar_t*& pstr_, const wchar_t* pstr_end_, int n_line_need_) -> bool {
		int n_line = 1;
		while (n_line != n_line_need_) {
			if (_find_line_next(pstr_, pstr_end_)) 
				++n_line;
			else
				return false;
		}
		return true;
	};
	auto _find_line_end = [](const wchar_t* pstr_, const wchar_t* pstr_end_) -> const wchar_t* {
		for ( ; pstr_ != pstr_end_; ++pstr_) {
			if (*pstr_ == L'\n' || *pstr_ == L'\r')
				break;
		}
		return pstr_;
	};
	//---------------------
	const wchar_t* pstr = Line_.c_str();
	if (pstr) {
		const wchar_t* pstr_end = pstr + Line_.length();
		if (_find_line(pstr, pstr_end, static_cast<int>(LineNumber_)))
			return String(std::wstring_view{ pstr, _find_line_end(pstr, pstr_end) });
	}
	
	return String::Empty_;
}
	// end native -----------------------------------------------------
}	// namespace esb (api part)

#endif	//ESB_H
