/*
%module(	module_guid="EEEFE873-DEAD-BEEF-8FC5-F95FFB941C70", 
			name_local="Пример", 
			module_description="Пример есб-компоненты") ortools_util_sorted_interval_list
*/
//%esb_module("EEEFE873-DEAD-BEEF-8FC5-F95FFB941C70", "Пример", "Пример есб-компоненты") ortools_util_sorted_interval_list
//


//NOTE	Этот файл не самостоятельный модуль, а должен включаться (например sat)
//		Здесь используется механизм обхода проблемы с конфликтом имен классов в extend-методах
//		с использованием конкретного пространства имен куда будут помещены врапперы
//		Нужно определить макрос ESB_MODULE_NS до включения этого файла
//
//TODO	что-то этот #error работу не прерывает, а только Warning на экран выводит.
//		возможно опцию где-то проставить надо
//
//NOTE	Если макрос делать обычный (не %), то не прерывает и там где должен бы развернуться ESB_MODULE_NS он подставляется как есть
//		но если сделать Свиг-макрос (начиная с %), то там где он должен развернуться будет ошибка синтаксиса
//		(лучше в Свиг файлах делать Свиг-макросы)
//
#ifndef %ESB_MODULE_NS
#	error	Macro ESB_MODULE_NS must be defined!
#endif





//
%include <esb_std_vector.swg>
//

//Used in meth 'operations_research::SortedDisjointIntervalList::SortedDisjointIntervalList'
//Used in meth 'operations_research::SortedDisjointIntervalList::InsertIntervals'
//
%esb_wrap_info_cref(	std::vector<int>,  				"9B8FD93E-F9D3-48A2-BFA1-44527934DE08", "CppArrayOfInteger32CRef", 	"СппМассивЦелых32КонстСсылка",	"Объект СппМассивЦелых32КонстСсылка");
%esb_ctor_creator(std::vector<int>::vector());
%esb_wrap_as_template(	std::vector<int>, "(Numeric)",	"B2B091FF-F33C-4305-8F63-AFDEC4B5B44E",	CppArrayOfInteger32,		"СппМассивЦелых32", 			"");


%include <absl/absl.swg>


// ИЗ ortools/base/base.i
/////////////////////////////////////////////////////////
// SWIG macros for explicit API declaration.
// Usage:
//
// %ignoreall
// %unignore SomeName;   // namespace / class / method
// %include "somelib.h"
// %unignoreall  // mandatory closing "bracket"
%define %ignoreall %ignore ""; %enddef
%define %unignore %rename("%s") %enddef
%define %unignoreall %rename("%s") ""; %enddef
/////////////////////////////////////////////////////////


// использется в operations_research::Domain::FlattenedIntervals
//NOTE	esb_wrap_info_cref нужно делать ДО esb_wrap_as_val_template, т.к. ОПЯТЬ все аттрибуты нужно задавать ДО самого типа (%template в данном случае) !!!
%esb_wrap_info_cref(	std::vector<int64_t>,  					"E820E8F6-ED43-4049-A742-3BB336C535A1", "CppArrayOfInteger64CRef", 	"СппМассивЦелых64КонстСсылка",	"Объект СппМассивЦелых64КонстСсылка");
//NOTE	_ref и _cval варианты напрямую в ор-тулс АПИ не требуются, но std::vector<int64_t> используется как элемент в контейнере std::vector< std::vector<int64_t> >
//		поэтому для полноценной реализации контейнера std::vector< std::vector<int64_t> > требуются ссылка и конст-ссылка на его элементы (и просто конст для порядка)
%esb_wrap_info_ref(		std::vector< int64_t >,  				"131BA997-CE05-48D2-9654-B7A4D506F09D", "CppArrayOfInteger64Ref", 	"СппМассивЦелых64Ссылка",		"Объект СппМассивЦелых64Ссылка");
%esb_wrap_info_cval(	std::vector< int64_t >,  				"62359BD0-DFC4-4A9C-BFF5-FE8EDF579B9D", "CppArrayOfInteger64Const", "СппМассивЦелых64Конст",		"Объект СппМассивЦелых64Конст");
%esb_ctor_creator(std::vector<int64_t>::vector());
//
%esb_wrap_as_template(	std::vector<int64_t>, 	"(Numeric)", 	"9B24BBA5-C83C-41F9-9BA1-B354BAB687C5",	CppArrayOfInteger64,		"СппМассивЦелых64", 			"");


//NOTE	ClosedInterval в АПИ используется только как конст-ссылка. 
//		НО также используется std::vector<ClosedInterval> в конструкторе SortedDisjointIntervalList, 
//		и чтобы сделать полноценный вектор нам нужно (можно) задекларировать все варианты.
//
%esb_wrap_info_cref(operations_research::ClosedInterval,	"39486EFE-DF9B-402C-B931-2D43FFE78251", "ClosedIntervalCRef", 		"ЗакрытыйИнтервалКонстСсылка", 	"");
%esb_wrap_info_ref(	operations_research::ClosedInterval,	"90B4AF65-68F9-4259-86EF-17523E4A4A6D", "ClosedIntervalRef", 		"ЗакрытыйИнтервалСсылка", 		"");
%esb_wrap_info_cval(operations_research::ClosedInterval,	"95CDE9EE-BDA9-4671-A15C-2FC78D03F532", "ClosedIntervalConst", 		"ЗакрытыйИнтервалКонст", 		"");
%esb_wrap_as_val(	operations_research::ClosedInterval,    "C720E072-A36B-454E-87B3-795CA8F8BC50", "ClosedInterval",     		"ЗакрытыйИнтервал",     		"");
//TODO	По какой-то причине при указании создать креатор только для пустого конструктора, 
//		креатор создается и для конструктора с параметрами
//		Какие-то причуды Свиг..
//		Ну.. хуже не будет..
%esb_ctor_creator(operations_research::ClosedInterval::ClosedInterval());
//TODO	А тут как-то рус-имя автоматом не назначилось... назначилось как для пустого - СоздатьЗакрытыйИнтервал
//		и мало того - как-то проверка на дублирование имен в интерфейсе тоже прошляпила..
//		поправляем.
%esb_name_local("СоздатьЗакрытыйИнтервалНачКон")			operations_research::ClosedInterval::ClosedInterval(int64_t s, int64_t e);

%esb_wrap_info_cref(operations_research::Domain,			"A52CA2B3-0227-44E5-9DF9-7784E6B6A033", "DomainOfInteger64CRef",	"ДоменЦелых64КонстСсылка",		"");
%esb_wrap_info_stat(operations_research::Domain,			"996E5113-B02D-4507-BE02-336032C6CF72", "DomainOfInteger64Stat",	"ДоменЦелых64Статик",			"");
%esb_wrap_as_val(	operations_research::Domain,			"42B818B3-6D45-442C-A1C0-60DDA86BCCFB", "DomainOfInteger64",     	"ДоменЦелых64",					"");	
%esb_ctor_creator(operations_research::Domain::Domain());
//TODO	Также как и с ClosedInterval - создались все креаторы, даже от одного параметра помеченный explicit (explicit Domain(int64_t value))
//		плюс еще проблем - создались с именами CreateDomain.., а не CreateDomainOfInteger64
//		..также нужно всем давать разные ру-имена вручную
//TODO	плюс еще хуже.. если для просто Domain() он для ен-имен он сам Create один раз подставляет, 
//		то для ру-имен он 'Создать' тоже подставляет, не учитывая что имя УЖЕ начинается с 'Создать'
//		поэтому зесь нужно писать БЕЗ 'Создать'
%esb_name_local("ДоменЦелых64НаЗначении")			operations_research::Domain::Domain(int64_t);
%esb_name_local("ДоменЦелых64НаИнтервале")			operations_research::Domain::Domain(int64_t, int64_t);




%esb_wrap_info_ref(	operations_research::SortedDisjointIntervalList,	"C5780A5E-3F0D-4166-8084-3D47158BE5CF", "SortedDisjointIntervalListRef",	"СортированныйСписокНепересекающихсяИнтерваловСсылка",		"");
%esb_wrap_as_val(	operations_research::SortedDisjointIntervalList,  	"B9BCC371-E685-406A-B68B-65717E9C6527", "SortedDisjointIntervalList",     	"СортированныйСписокНепересекающихсяИнтервалов",     "");	


//SortedDisjointIntervalListFromArrayOfInteger64
//SortedDisjointIntervalListFromArrayOfInteger32
//%esb_localisation_dictionry(SortedDisjointIntervalList, "СортированныйСписокНепересекающихсяИнтервалов")
%esb_localisation_dictionry(FromArray, 					"ИзМассива")
%esb_localisation_dictionry(OfInteger32, 				"Целых32")
%esb_localisation_dictionry(OfInteger64, 				"Целых64")





%esb_name_local("КоличествоИнтервалов")			operations_research::Domain::NumIntervals() const;
%esb_name_local("ИнтервалПоИндексу")			operations_research::Domain::operator [](int) const;

%esb_name_local("УмноженныйНа")					operations_research::Domain::MultiplicationBy(int64_t) const;
%esb_name_local("НепрерывноУмноженныйНаКоэфф")	operations_research::Domain::ContinuousMultiplicationBy(int64_t) const;
%esb_name_local("НепрерывноУмноженныйНаДомен")	operations_research::Domain::ContinuousMultiplicationBy(operations_research::Domain const &) const;
// Ниже будет определен метод-расширение
// operations_research::Domain::ContinuousMultiplicationBy(OneOf< Numeric,SatDomain > const &) const
// и это локализация для него. И всех возможных методов с таким именам (по-умолчанию) 
// поскольку мы не указываем сигнатуру
%esb_name_local("НепрерывноУмноженныйНа")		operations_research::Domain::ContinuousMultiplicationBy;

%esb_name_local("ДеленныйНа")					operations_research::Domain::DivisionBy(int64_t) const;
%esb_name_local("ИнверсноУмноженныйНа")			operations_research::Domain::InverseMultiplicationBy(int64_t) const;

//If NumIntervals() is too large, this return a superset of the domain.
%esb_name_local("УпрощенноеНадмножество")		operations_research::Domain::RelaxIfTooComplex() const;

%esb_name_local("ПоложительныйМодульПоНадмножеству")	operations_research::Domain::PositiveModuloBySuperset(operations_research::Domain const &) const;
%esb_name_local("ПоложительноеДелениеНаНадмножество")	operations_research::Domain::PositiveDivisionBySuperset(operations_research::Domain const &) const;
%esb_name_local("ЧастьВокругНуля")						operations_research::Domain::PartAroundZero() const;


%esb_name_local("ПостроитьДополнениеНаИнтервале")		operations_research::SortedDisjointIntervalList::BuildComplementOnInterval(int64_t,int64_t);
%esb_name_local("КоличествоИнтервалов")     			operations_research::SortedDisjointIntervalList::NumIntervals() const;


%esb_name_local("ИнтервалыСортированыИНеСмежные")     	operations_research::IntervalsAreSortedAndNonAdjacent(absl::Span< operations_research::ClosedInterval const >);
%esb_name_local("СуммаМинимальныхКЗначенийДомена")     	operations_research::SumOfKMinValueInDomain(operations_research::Domain const &,int);
%esb_name_local("СуммаМаксивальныхКЗначенийДомена")     operations_research::SumOfKMaxValueInDomain(operations_research::Domain const &,int);




//	static Domain FromValues(std::vector<int64_t> values);
//	static Domain FromVectorIntervals(const std::vector<std::vector<int64_t> >& intervals);
//TODO	Эта "фича" esb_comp_types - исключительный костыль !!!
//		Как минимум здесь. Здесь все эти ..Ref/..CRef можно вывести из параметра шаблона std::vector<int64_t>
//		И мало того - не факт, что для std::vector<int64_t> определено создание ..Ref/..CRef
%esb_wrap_info_cref(	std::vector< std::vector<int64_t> >,  	"8DD92EFC-F05A-4ED4-ACA5-2D5D8EBBCE5D", "CppArrayOfCppArrayOfInteger64CRef", 	"СппМассивМассивовЦелых64КонстСсылка",	"Объект СппМассивМассивовЦелых64КонстСсылка");
%esb_wrap_as_template(	std::vector< std::vector<int64_t> >, 	"(CppArrayOfInteger64, CppArrayOfInteger64Ref, CppArrayOfInteger64, CppArrayOfInteger64CRef)",
																"0005B3F3-C740-49A9-AD83-B5CC1EC07214",	CppArrayOfCppArrayOfInteger64,			"СппМассивМассивовЦелых64", 	"");
%esb_ctor_creator(std::vector< std::vector<int64_t> >::vector());


//  static Domain FromFlatSpanOfIntervals(absl::Span<const int64_t> flat_intervals);
//
%esb_map_well_known_for_arg_template(absl::Span<const int64_t>, 
	"const CppArrayOfInteger64&", "convert_to_arg<const std::vector<int64_t>&>", AbslSpanOfConstInteger64 );
// на память..
//%esb_wrap_as_template(absl::Span<const int64_t>, "(Numeric)",	"01978045-D8E3-4C56-BFBF-7F0957E61E6E",	AbslSpanOfConstInteger64,			"АбслНаборКонстЦелых64", 			"");


// SortedDisjointIntervalList(const std::vector<ClosedInterval>& intervals);
%esb_wrap_info_cref(	std::vector<operations_research::ClosedInterval>, "46AB0A28-25D9-4437-9702-6FA76802E327", "CppArrayOfClosedIntervalCRef", "СппМассивЗакрытыхИнтерваловКонстСсылка", 	"");
%esb_ctor_creator(std::vector<operations_research::ClosedInterval>::vector());
%esb_wrap_as_template(	std::vector<operations_research::ClosedInterval>, "(ClosedInterval, ClosedIntervalRef, ClosedIntervalConst, ClosedIntervalCRef)",
																			"03D76353-6F5C-43AC-A423-BB96EBEE9AC8",	CppArrayOfClosedInterval,		"СппМассивЗакрытыхИнтервалов", 				"");
//
//  static Domain FromIntervals(absl::Span<const ClosedInterval> intervals);



//NOTE	absl::Span<::operations_research::ClosedInterval const> используется только как аргумент в методах
//		ввиду этого нецелесообразно генерировать для него враппер (тем более он reference-тип)
//		ТоЕсть. Методы реально ожидают только (срез, набор, ??) Спан объектов ClosedInterval
//				А сами-то объекты должны жить еще где-то (скажем в векторе std::vector<operations_research::ClosedInterval>)
//				Если сделать для Спан собственный тип, то перед вызовом метода из 1С нужно еще и будет создавать как отдельный объект этот AbslSpanOfConstClosedInterval
//				Это совершенно излишне, т.к. absl::Span имплисит создается из std::vector.
//				Нам нужно просто принимать "основной" объект std::vector и указать делать ему конвертацию
//		(это частая ситуация при построении !хорошего! с++ АПИ - скрыть за "Спан" характер самого контейнера)
//		Поэтому мы просто указываем ЕСвиг, что absl::Span<::operations_research::ClosedInterval const>
//			- это _well_known_ тип
//			- какого типа делать аргумент для метода, где изначально используется absl::Span
//			- и каким образом проводить конвертацию
//			- и что это шаблон, который надо инстанциировать
//			- (относительно имени шаблона см. примечание к %esb_map_well_known_for_arg_template)
//
%esb_map_well_known_for_arg_template(absl::Span<::operations_research::ClosedInterval const>, 
	"const CppArrayOfClosedInterval&", "convert_to_arg<const std::vector<operations_research::ClosedInterval>&>", AbslSpanOfConstClosedInterval );
//
// это на память..
//%esb_wrap_as_template(absl::Span<::operations_research::ClosedInterval const>, 	"(ClosedInterval, void, void, ClosedIntervalCRef)",
//																"A9DE88B6-9EBF-4113-82FF-4FC090B49093",	AbslSpanOfConstClosedInterval,		"АбслНаборКонстЗакрытыхИнтервалов", "");


//operations_research::ClosedInterval::ClosedInterval(int64_t s, int64_t e)
%esb_param_rename(s, "start_") 	operations_research::ClosedInterval::ClosedInterval(int64_t s, int64_t e);
%esb_param_rename(e, "end_") 	operations_research::ClosedInterval::ClosedInterval(int64_t s, int64_t e);


//TODO 	Это что-то малопонятное. Нужно разбираться.. пока игнорируем
//operations_research::Domain
//	absl::InlinedVector<ClosedInterval, 1>::const_iterator begin() const
%ignore operations_research::Domain::begin() const;
//	absl::InlinedVector<ClosedInterval, 1>::const_iterator end() const
%ignore operations_research::Domain::end() const;


//operations_research::SortedDisjointIntervalList
//	Iterator InsertInterval(int64_t start, int64_t end);
%ignore operations_research::SortedDisjointIntervalList::InsertInterval(int64_t start, int64_t end);
//	Iterator GrowRightByOne(int64_t value, int64_t* newly_covered);
%ignore operations_research::SortedDisjointIntervalList::GrowRightByOne;
//  Iterator FirstIntervalGreaterOrEqual(int64_t value) const;
%ignore operations_research::SortedDisjointIntervalList::FirstIntervalGreaterOrEqual(int64_t) const;
//  Iterator LastIntervalLessOrEqual(int64_t value) const;
%ignore operations_research::SortedDisjointIntervalList::LastIntervalLessOrEqual(int64_t) const;
//	ConstIterator begin() const { return intervals_.begin(); }
%ignore operations_research::SortedDisjointIntervalList::begin() const;
//	ConstIterator end() const { return intervals_.end(); }
%ignore operations_research::SortedDisjointIntervalList::end() const;


//	const ClosedInterval& last() const { return *intervals_.rbegin(); }
//TODO	а емитится:		"SatClosedInterval Last() const"
//		т.е. оригинальная функция возвращает const-ссылку на свои потроха
//		а мы емитим полноценный объект, а не ссылку...
//		ОНО вроде и правильно, т.к. никакие ссылки я емитить не могу, но... неожиданно получилось... думать...
%typemap(ctype) const operations_research::ClosedInterval&     "SatClosedInterval"


//	Domain ContinuousMultiplicationBy(int64_t 		coeff) const;
//	Domain ContinuousMultiplicationBy(const Domain& domain) const;
//TODO	Это перегруженые методы. Пока мы не умеем работать с перегрузкой (делать метод-диспетчер)
//		поэтому просто переименовываем
//		Переименовываем ОБА метода используя суффиксом имя (тип) параметра
//		В дальнейшем нужен будет сделать еще метод-диспетчер ContinuousMultiplicationBy(OneOf<Numeric,SatDomain>)
//		который будет разбираться что вызывать
//		ПС. Все равно это лучше делать полу-вручную, т.к. толковые имена методам-перегрузкам
//			автоматически всегда подбирать вряд-ли удастся
%rename(ContinuousMultiplicationByCoeff)		operations_research::Domain::ContinuousMultiplicationBy(int64_t) const;
%rename(ContinuousMultiplicationByDomain)		operations_research::Domain::ContinuousMultiplicationBy(const Domain&) const;
//NOTE	Блок кода имплементации метода расширения вставляется в код враппера полностью "as-is" 
//		- со всеми табами и переносами строк. Поэтому здесь он НЕ выделен табами внутри блока extend
//		(иначи эти табы в коде враппера лишними будут)
//TODO	Методы расширения пишутся "as-is" и к ним не применяются никакие typemap
//		НО! В этом случае мы не разбираемся что SatDomain это тот же самый класс и нужно делать defer-implementation!!!
//		ХОТЯ... метод вызывает только методы с defer-implementation. хз.. может и скомпилируется..
//TODO	вроде для примитивных типов лучше использувать .is<>() .as<>() ??? она просто кастит ??? смотреть надо..
%extend operations_research::Domain {
DomainOfInteger64 ContinuousMultiplicationBy(const OneOf<Numeric, DomainOfInteger64>& multiplicator_) const 
{
	if(auto multiplicator_may_be_Numeric = multiplicator_.make_if<Numeric>())
		return ContinuousMultiplicationByCoeff(*multiplicator_may_be_Numeric);
	return ContinuousMultiplicationByDomain(multiplicator_.make<DomainOfInteger64>());
}
}


//void swap(SortedDisjointIntervalList& other)
//TODO	а емитится:		"void Swap(const SatSortedDisjointIntervalList& other_)"
//		т.е. c const-ом ЧТО НЕВЕРНО и НЕ СКОМПИЛИРУЕТСЯ! РАЗБИРАТЬСЯ!
//%typemap(ctype) operations_research::SortedDisjointIntervalList&     "SatSortedDisjointIntervalList"




//TODO	Как-бы это разумно сделать? Может метод Serialize ?. Или Output ? ...
//	std::ostream& operator<<(std::ostream& out, const ClosedInterval& interval);
%ignore operations_research::operator<<;


// Чисто для "красивости".
// Наш esb.swg настроен для переименования оператора [] в ElementAt
// НО здесь по-контексту лучше подходит IntervalAt, т.к. Domain это набор интервалов
//TODO	Мы (Свиг) не умеем переименовывать имена параметров, а надо-бы
//		как минимум здесь намного лучше было-бы "index_"
%rename(IntervalAt)      	operations_research::Domain::operator[](int i) const;


//Domain MultiplicationBy(int64_t coeff, bool* exact = nullptr) const;
//	Метод имеет "out" bool опциональный аргумент со значением по умолчанию. 
//TODO	мы пока не умеем ни опциональных аргументов, ни "out" аргументов
//		поэтому вариант с bool* exact игнорируем
//		(без указания =nullptr игнорируется только вариант с bool* параметром)
%ignore operations_research::Domain::MultiplicationBy(int64_t coeff, bool* exact) const;
//%typemap(ctype)		bool*	"ByRefValueTODO<Boolean>"


//	SortedDisjointIntervalList(const std::vector<int64_t>& 	starts, 	const std::vector<int64_t>& ends);
//	SortedDisjointIntervalList(const std::vector<int>& 		starts, 	const std::vector<int>& 	ends);
//TODO	Тут генерится предупреждение
// 		"Warning 467: Overloaded method SortedDisjointIntervalList(const std::vector<int>...) not supported..."
//		т.к. 
//		а) мы действительно не дали Свиг %typecheck(...) информации для сревнения 
//			const std::vector<int64_t>& и const std::vector<int>&
//		б) наш механизм "разруливания" перегрузок через суффиксы имен параметров здесь НЕ РАБОТАЕТ
//			т.к. и имена параметров одинаковые ()
//TODO	Научиться переименовывать параметры!
// 	Можно или можно игнорировать одну из перегрузок
//%ignore operations_research::SortedDisjointIntervalList::SortedDisjointIntervalList(const std::vector<int>& starts, const std::vector<int>& ends);
//	или подавить предупреждение (НО НЕ СКОМПИЛИРУЕТСЯ все равно!)
//%warnfilter(SWIGWARN_TYPEMAP_TYPECHECK) operations_research::SortedDisjointIntervalList::SortedDisjointIntervalList;
//TODO	В данном случае ^^^^^^^^^^^^^^^^^^^^^^^^^^ нужно как-то переименовывать НЕ конструкторы, а
//		create-методы в виде CreateSortedDisjointIntervalList64, CreateSortedDisjointIntervalList32
//		или CreateSortedDisjointIntervalListStartsEnds64, ...
//		или CreateSortedDisjointIntervalListArrayOfInteger64StartsEnds, ...
//		или CreateSortedDisjointIntervalListArrayOfInteger64, ...
//		или CreateSortedDisjointIntervalListFromArraysOfInteger64, ...
//		... чтобы не писать польностью ... CreateSortedDisjointIntervalListArrayOfInteger64StartsArrayOfInteger64Ends ...
//		(евристика.. красивости..)


//	struct IntervalComparator
//	Вложенный класс-функтор для сравнения интервалов. Не полезен.
%ignore operations_research::SortedDisjointIntervalList::IntervalComparator;


//	void SortedDisjointIntervalList::InsertIntervals(const std::vector<int64_t>& 	starts,  	const std::vector<int64_t>& ends);
//	void SortedDisjointIntervalList::InsertIntervals(const std::vector<int>& 		starts, 	const std::vector<int>& 	ends);
%rename(InsertIntervals64)		operations_research::SortedDisjointIntervalList::InsertIntervals(const std::vector<int64_t>& starts, const std::vector<int64_t>& ends);
%rename(InsertIntervals32)		operations_research::SortedDisjointIntervalList::InsertIntervals(const std::vector<int>& starts, const std::vector<int>& ends);
//
//TODO	метод operations_research::SortedDisjointIntervalList::swap декларирован от ссылки (SortedDisjointIntervalList&)
//		и наш ESwig по простому и делает от SortedDisjointIntervalListRef, а в общем имеется ввиду, что в метод нужно по-ссылке подавать сам SortedDisjointIntervalList
//		ну или на худой конец OneOf<SortedDisjointIntervalList, SortedDisjointIntervalListRef>
//		на голом месте делать целый объект SortedDisjointIntervalListRef если нужно обменять два SortedDisjointIntervalList как-то странно
//		нужно по-внимательней разобраться с нюансами имплементации врапперов, а не тупо - что-вижу-то-пишу
//
//NOTE	Пока оригинальный swap игнорим и добавляем extended Swap(OneOf<SortedDisjointIntervalList, SortedDisjointIntervalListRef>)
//
%ignore operations_research::SortedDisjointIntervalList::swap;
//
//NOTE	В методе ::swap было единственное использование SortedDisjointIntervalList как ссылки,
//		а нам нужен SortedDisjointIntervalListRef для extend метода
//		поэтому помечаем -используемый-как-ссылка- принудительно
//		(мы внутрь extend-методов не лазим и что они хотят/делаеют не понимаем. в общем это в принципе невозможно)
//
%esb_type_used_as_arg_ref(operations_research::SortedDisjointIntervalList);
//
//NOTE	Для Swap. Если делать OneOf<SortedDisjointIntervalList, ..>, то Свиг опять услужливо находит SortedDisjointIntervalList в operations_research::
//		и делает OneOf<operations_research::SortedDisjointIntervalList, ..>
//		Поэтому делаем OneOf<esb::SortedDisjointIntervalList, ..> чтобы он не думал лишнего.
//TODO	Это поведение Свиг какое-то странное. Причем что очень глубоко зарытое - уже в cDeclaration хэндлер все приходит уже в таком виде.
//		Это не логично для extended-метода, т.к. очевидно, что он вставляется не в оригинальный класс, а в уже некий другой класс враппера
//		Возможно это может где-то настраиваться, хз.. Пока просто выкручиваемся..
//
%extend operations_research::SortedDisjointIntervalList {
void InsertIntervals(const OneOf<CppArrayOfInteger32, CppArrayOfInteger64>& starts_, const OneOf<CppArrayOfInteger32, CppArrayOfInteger64>& ends_)
{
	// Мы здесь привязываемся только к starts_. если starts_ ArrayOfInteger64(32), то и ends_ обязан быть того же типа.
	if(auto starts_may_be_ArrayOfInteger64 = starts_.make_if<CppArrayOfInteger64>() )
		return InsertIntervals64(*starts_may_be_ArrayOfInteger64, ends_.make<CppArrayOfInteger64>());
	return InsertIntervals32(starts_.make<CppArrayOfInteger32>(), ends_.make<CppArrayOfInteger32>());
}
void Swap(OneOf<%ESB_MODULE_NS::SortedDisjointIntervalList, SortedDisjointIntervalListRef> other_)
{
	if(other_.is<SortedDisjointIntervalList>())
		return _.swap(other_.as<SortedDisjointIntervalList>().GetContained().Data());
	else
		return _.swap(other_.as<SortedDisjointIntervalListRef>().GetContained().Data());
}
}

//SortedDisjointIntervalList(const std::vector<int64_t>& 	starts, const std::vector<int64_t>& ends);
//SortedDisjointIntervalList(const std::vector<int>& 		starts, const std::vector<int>& 	ends);
%rename(SortedDisjointIntervalListFromArrayOfInteger64)		operations_research::SortedDisjointIntervalList::SortedDisjointIntervalList(const std::vector<int64_t>&, const std::vector<int64_t>&);
%rename(SortedDisjointIntervalListFromArrayOfInteger32)		operations_research::SortedDisjointIntervalList::SortedDisjointIntervalList(const std::vector<int>&, const std::vector<int>&);
//TODO SortedDisjointIntervalListFromArray[Of]

//
%include "ortools/util/sorted_interval_list.h"
//
