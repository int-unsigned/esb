/* -----------------------------------------------------------------------------
 * esb.swg
 *
 * esb typemaps and other esb-language-module related stuff
 * ----------------------------------------------------------------------------- */

// Определение esb-модуля с дополнительными аттрибутами
// Все параметры - строки в кавычках
// GUID_ должен быть IN_REGISTERY_FORMAT_: "EEEFE873-DEAD-BEEF-8FC5-F95FFB941C71"
%define %esb_module(GUID_STRING_, NAME_LOCAL_, DESCRIPTION_) 	/*module-name-here*/
	%module( module_guid=GUID_STRING_, name_local=NAME_LOCAL_, module_description=DESCRIPTION_) 
%enddef 


%define %esb_pragma(PRAGMA_, PRAGMA_VALUE_IN_QUOTER_) 
	%pragma(esb) PRAGMA_ = PRAGMA_VALUE_IN_QUOTER_
%enddef 


// Аттрибуты модуля могут быть определены (переопределны прагмами)
%define %esb_module_guid(GUID_STRING_IN_REGISTERY_FORMAT_IN_QUOTER_) 
	%esb_pragma(module_guid, GUID_STRING_IN_REGISTERY_FORMAT_IN_QUOTER_)
%enddef 
%define %esb_module_name_local(NAME_LOCAL_STRING_IN_QUOTER_) 
	%esb_pragma(module_name_local, NAME_LOCAL_STRING_IN_QUOTER_)
%enddef 
%define %esb_module_description(DESCRIPTION_STRING_IN_QUOTER_) 
	%esb_pragma(module_description, DESCRIPTION_STRING_IN_QUOTER_)
%enddef 

%define %esb_module_classes_prefix(CLASSES_PREFIX_) 
	%esb_pragma(module_classes_prefix, CLASSES_PREFIX_)
%enddef 

%define %esb_module_namespace(NAMESPACE_) 
	%esb_pragma(module_namespace, NAMESPACE_)
%enddef 

%define %esb_top_level_namespace_from_global(NAMESPACE_) 
	%esb_pragma(top_level_namespace_from_global, NAMESPACE_)
%enddef 


// Вставка произвольного кода в результирующий файл
//TODO 	Секция "esb_inline" обрабатывается на уровне LangugeModuleEsb::insertDirective
//		и привязывается к последующему врапперу класса или последующему free-методу
//		(или вставляется в конце)
//		Пока у esb-swig нет последовательности эмиттинга кода - сначала все классы, потом все фрии
//		Использование директивы внутри класса считается ошибкой
#define %esb_inline     %insert("esb_inline")

#define %esb_insert_module_begin		%insert("esb_module_begin")
#define %esb_insert_wrappers_begin		%insert("esb_wrappers_begin")
#define %esb_insert_wrappers_final		%insert("esb_wrappers_final")

#define %esb_insert_module_macro		%insert("esb_module_macro")


%define %esb_localisation_dictionry(EN_, RU_) 
	%esb_pragma(localisation_dictionry$##EN_, RU_)
%enddef 

%define %add_to_module_interface_meth(EN_, RU_) 
	%esb_pragma(add_to_module_interface_meth$##EN_, RU_)
%enddef 


%esb_localisation_dictionry(Abs, "Абсолютн")
%esb_localisation_dictionry(Add, "Добавить")	// Append тоже Добавить, но они синонимы и обычно не встречаются в одном контексте
%esb_localisation_dictionry(Addition, "Добавление")
%esb_localisation_dictionry(After, "После")
%esb_localisation_dictionry(All, "Все")
%esb_localisation_dictionry(Allowed, "Допустим")
%esb_localisation_dictionry(And, "И")
%esb_localisation_dictionry(Append, "Добавить")
%esb_localisation_dictionry(Array, "Массив")
%esb_localisation_dictionry(As, "Как")
%esb_localisation_dictionry(Assignments, "Присвоение")
%esb_localisation_dictionry(Assumption, "Предположение")
%esb_localisation_dictionry(Assumptions, "Предположения")
%esb_localisation_dictionry(Back, "Назад")
%esb_localisation_dictionry(Before, "Перед")
%esb_localisation_dictionry(Boolean, "Булево")
%esb_localisation_dictionry(Build, "Построить")
%esb_localisation_dictionry(Builder, "Построитель")
%esb_localisation_dictionry(Clear, "Очистить")
%esb_localisation_dictionry(Clone, "Клонировать")
%esb_localisation_dictionry(Closed, 			"Закрытый")
%esb_localisation_dictionry(Closest, "Ближайший")
%esb_localisation_dictionry(Coeff, "Коэффициент")
%esb_localisation_dictionry(Coefficients, "Коэффициенты")
%esb_localisation_dictionry(Complement, "Дополнить")
%esb_localisation_dictionry(Complex, "Сложный")
%esb_localisation_dictionry(Const, 		"Конст")
%esb_localisation_dictionry(Constant, 	"Константн")
%esb_localisation_dictionry(Constraint, "Ограничение")
%esb_localisation_dictionry(Contains, "Содержит")
%esb_localisation_dictionry(Continuous, "Напрерывн")
%esb_localisation_dictionry(Create, 		"Создать")
%esb_localisation_dictionry(Cumulative, "Кумулятивн")
%esb_localisation_dictionry(Debug, "Отлад")
%esb_localisation_dictionry(Decision, "Решение")
%esb_localisation_dictionry(Demand, "Требовать")
%esb_localisation_dictionry(Different, "Различные")
%esb_localisation_dictionry(Disjoint, "Непересекающ")
%esb_localisation_dictionry(Distance, "Расстояние")
%esb_localisation_dictionry(Division, "Деление")
%esb_localisation_dictionry(Domain, "Домен")
%esb_localisation_dictionry(Double, "Действительное")
%esb_localisation_dictionry(Element, "Элемент")
%esb_localisation_dictionry(Empty, "Пуст")
%esb_localisation_dictionry(End, "Конец")
%esb_localisation_dictionry(Enforce, "Принудить")
%esb_localisation_dictionry(Equal, "Равно")
%esb_localisation_dictionry(Equality, "Равенство")
%esb_localisation_dictionry(Event, "Событие")
%esb_localisation_dictionry(Exactly, "Точно")
%esb_localisation_dictionry(Export, "Экспорт")
%esb_localisation_dictionry(Expr, "Выраж")
%esb_localisation_dictionry(Expression, "Выражение")
%esb_localisation_dictionry(False, "Ложь")
%esb_localisation_dictionry(File, "Файл")
%esb_localisation_dictionry(Fix, "Фикс")
%esb_localisation_dictionry(Fixed, "Фиксированный")
%esb_localisation_dictionry(Flat, "Плоский")
%esb_localisation_dictionry(Flattened, "Уплощенн")
%esb_localisation_dictionry(Forbidden, "Запрещенный")
%esb_localisation_dictionry(From, "Из")
%esb_localisation_dictionry(Front, "Первый")
%esb_localisation_dictionry(Get, "Получить")
%esb_localisation_dictionry(Greater, "Больший")
%esb_localisation_dictionry(Has, "Имеет")
%esb_localisation_dictionry(Hint, "Подсказка")
%esb_localisation_dictionry(Hints, "Подсказки")
%esb_localisation_dictionry(If, "Если")
%esb_localisation_dictionry(Implied, "Подразумеваем")
%esb_localisation_dictionry(In, "В")
%esb_localisation_dictionry(Included, "Включенн")
%esb_localisation_dictionry(Index, "Индекс")
%esb_localisation_dictionry(Insert, "Вставить")
%esb_localisation_dictionry(Integer, "Целое")
%esb_localisation_dictionry(Integer32, "Целое32")
%esb_localisation_dictionry(Integer64, "Целое64")
%esb_localisation_dictionry(Integers64, "Целые32")
%esb_localisation_dictionry(Intersection, "Пересечение")
%esb_localisation_dictionry(Interval, "Интервал")
%esb_localisation_dictionry(Intervals, "Интервалы")
%esb_localisation_dictionry(Intervals32, "Интервалы32")
%esb_localisation_dictionry(Intervals64, "Интервалы64")
%esb_localisation_dictionry(Inverse, "Обратный")
%esb_localisation_dictionry(Invert, "Инвертировать")
%esb_localisation_dictionry(Is, "Есть")
%esb_localisation_dictionry(Last, "Последн")
%esb_localisation_dictionry(Least, "Наименее")
%esb_localisation_dictionry(Left, "Левый")
%esb_localisation_dictionry(Less, "Меньше")
%esb_localisation_dictionry(List, "Список")
%esb_localisation_dictionry(Max, "Макс")
%esb_localisation_dictionry(Maximize, "Максимизировать")
%esb_localisation_dictionry(Min, "Мин")
%esb_localisation_dictionry(Minimize, "Минимизировать")
%esb_localisation_dictionry(Model, "Модель")
%esb_localisation_dictionry(Modulo, "Модуль")
%esb_localisation_dictionry(Most, "Наиболее")
%esb_localisation_dictionry(Multiple, "Несколько")
%esb_localisation_dictionry(Multiplication, "Умножение")
%esb_localisation_dictionry(Multiply, "Умножить")
%esb_localisation_dictionry(Mutable, "Изменяемый")
%esb_localisation_dictionry(Name, "Имя")
%esb_localisation_dictionry(Negation, "Отрицание")
%esb_localisation_dictionry(New, "Нов")
%esb_localisation_dictionry(No, "Не")
%esb_localisation_dictionry(Not, "Нет")
%esb_localisation_dictionry(Numeric, "Число")
%esb_localisation_dictionry(Objective, "Цель")
%esb_localisation_dictionry(One, "Один")
%esb_localisation_dictionry(Only, "Только")
%esb_localisation_dictionry(Optional, "Опциональн")
%esb_localisation_dictionry(Or, "Или")
%esb_localisation_dictionry(Overlap, "Перекрыв")
%esb_localisation_dictionry(Overlap2, "Перекрыв2")
%esb_localisation_dictionry(Parameter, "Параметр")
%esb_localisation_dictionry(Parameters, "Параметры")
%esb_localisation_dictionry(Positive, "Положительн")
%esb_localisation_dictionry(Presence, "Присутств")
%esb_localisation_dictionry(Rectangle, "Прямоугольник")
%esb_localisation_dictionry(Reservoir, "Резервуар")
%esb_localisation_dictionry(Response, "Ответ")
%esb_localisation_dictionry(Right, "Прав")
%esb_localisation_dictionry(Sat, "Сат")
%esb_localisation_dictionry(Set, "Установить")
%esb_localisation_dictionry(Simplify, "Упростить")
%esb_localisation_dictionry(Size, "Размер")
%esb_localisation_dictionry(Smallest, "Меньший")
%esb_localisation_dictionry(Solver, "Решатель")
%esb_localisation_dictionry(Sorted, "Сортированный")
%esb_localisation_dictionry(Span, "Диапазон")
%esb_localisation_dictionry(Square, "Квадрат")
%esb_localisation_dictionry(Start, "Начинать")
%esb_localisation_dictionry(Strategy, "Стратегия")
%esb_localisation_dictionry(String, "Строка")
%esb_localisation_dictionry(Substruct, "Вычесть")
%esb_localisation_dictionry(Sum, "Сумма")
%esb_localisation_dictionry(Superset, "Надмножество")
%esb_localisation_dictionry(Swap, "Обменять")
%esb_localisation_dictionry(Table, "Таблица")
%esb_localisation_dictionry(Term, "Терм")
%esb_localisation_dictionry(Than, "Чем")
%esb_localisation_dictionry(Too, "Слишком")
%esb_localisation_dictionry(Transition, "Переход")
%esb_localisation_dictionry(True, "Истина")
%esb_localisation_dictionry(Tuple, "Кортеж")
%esb_localisation_dictionry(Two, "Два")
%esb_localisation_dictionry(Union, "Объедин")
%esb_localisation_dictionry(Using, "Использ")
%esb_localisation_dictionry(Value, "Значение")
%esb_localisation_dictionry(Values, "Значения")
%esb_localisation_dictionry(Var, "Перем")
%esb_localisation_dictionry(Variable, "Переменная")
%esb_localisation_dictionry(Variables, "Переменные")
%esb_localisation_dictionry(Vector, "Массив")
%esb_localisation_dictionry(Weighted, "Взвешенный")
%esb_localisation_dictionry(With, "С")
%esb_localisation_dictionry(Xor, "ИсключающееИли")
//
%esb_localisation_dictionry(Export, 			"Экспорт")
%esb_localisation_dictionry(File, 				"Файл")
%esb_localisation_dictionry(ExportToFile, 		"ЭкспортВФайл")
%esb_localisation_dictionry(ToString, 			"ВСтроку")
%esb_localisation_dictionry(Only, 				"Только")
%esb_localisation_dictionry(Literal, 			"Литерал")
%esb_localisation_dictionry(Literals, 			"Литералы")
%esb_localisation_dictionry(Enforce, 			"Обеспечить")
//%esb_localisation_dictionry(If, 				"Если")


// На заметку: "фичу" нужно декларировать ДО декларации класс/метода. А то Свиг не видит.
%define %esb_guid(GUID_STRING_IN_REGISTERY_FORMAT_)									/* symbol declaration here*/
	%feature("esb:guid_attr", GUID_STRING_IN_REGISTERY_FORMAT_)
%enddef 



//%feature("esb:name_local","НОД") gcd;
%define %esb_name_local(NAME_LOCAL_) 												/* symbol declaration here*/
	%feature("esb:name_local", NAME_LOCAL_)
%enddef 

//%feature("esb:name_local", "Фуу", get="ПолучитьФуу", set="УстановитьФуу") Foo;
%define %esb_name_local_prop(NAME_LOCAL_, NAME_LOCAL_GET_, NAME_LOCAL_SET_) 		/* symbol declaration here*/
	%feature("esb:name_local", NAME_LOCAL_, get=NAME_LOCAL_GET_, set=NAME_LOCAL_SET_)
%enddef 
%define %esb_name_local_prop_ro(NAME_LOCAL_, NAME_LOCAL_GET_) 						/* symbol declaration here*/
	%feature("esb:name_local", NAME_LOCAL_, get=NAME_LOCAL_GET_)
%enddef 

%define %esb_description_local(DESCRIPTION_LOCAL_) 									/* symbol declaration here*/
	%feature("esb:description_local", DESCRIPTION_LOCAL_)
%enddef 


%define %esb_param_rename(PARAM_NAME_, PARAM_NEW_NAME_STRING_IN_QUOTE_) 			/* symbol declaration here*/
	%feature("esb:param_rename", PARAM_NAME_=PARAM_NEW_NAME_STRING_IN_QUOTE_)
%enddef 


%define %esb_template_name(NAME_) 													/* symbol declaration here*/
	%feature("esb:template_name", NAME_)
%enddef 


%define %esb_declare_as_ref_type(CLASS_TYPE_)
	%feature("esb:declare_as_ref_type", 1) CLASS_TYPE_
%enddef 
%define %esb_declare_as_val_type(CLASS_TYPE_) 
	%feature("esb:declare_as_val_type", 1) CLASS_TYPE_
%enddef

%define %esb_prop(SYMBOL_, PROP_NAME_)
	%feature("esb:prop", PROP_NAME_) SYMBOL_
%enddef 
%define %esb_prop_decl(SYMBOL_, PROP_DECL_, PROP_TAG_)
	%feature("esb:prop_decl", PROP_DECL_) SYMBOL_;
	%feature("esb:prop_tag", PROP_TAG_) SYMBOL_
%enddef 


%define %esb_well_known_type(SYMBOL_)
	%feature("esb:well_known_type", 1) SYMBOL_
%enddef 

%define %esb_container(SYMBOL_)
	%feature("esb:container", 1) SYMBOL_
%enddef 
%define %esb_container_of(SYMBOL_, TYPE_)
	%feature("esb:container", TYPE_) SYMBOL_
%enddef 

%define %esb_converter_for_arg(SYMBOL_, CONVERTER_)
	%feature("esb:converter_for_arg", CONVERTER_) SYMBOL_
%enddef 


// принудительная предустановка аттрибутов использования
//TODO	_as_val не делаю почему-то.. само должно проставиться ? посмотрим..
%define %esb_type_used_as_arg_cref(SYMBOL_)		%feature("esb:type_used_as_arg_cref", 1)	SYMBOL_
%enddef 
%define %esb_type_used_as_arg_ref(SYMBOL_)		%feature("esb:type_used_as_arg_ref", 1)		SYMBOL_
%enddef 
%define %esb_type_used_as_arg_const(SYMBOL_)	%feature("esb:type_used_as_arg_const", 1)	SYMBOL_
%enddef 
%define %esb_type_used_as_ret_cref(SYMBOL_)		%feature("esb:type_used_as_ret_cref", 1)	SYMBOL_
%enddef
%define %esb_type_used_as_ret_ref(SYMBOL_)		%feature("esb:type_used_as_ret_ref", 1)		SYMBOL_
%enddef
%define %esb_type_used_as_ret_const(SYMBOL_)	%feature("esb:type_used_as_ret_const", 1)	SYMBOL_
%enddef 


%define %esb_ctor_constructor(SYMBOL_)
	%feature("esb:ctor_constructor", 1) SYMBOL_
%enddef 
%define %esb_ctor_creator(SYMBOL_)
	%feature("esb:ctor_creator", 1) SYMBOL_
%enddef 

// положить имплементацию в самом конце
%define %esb_implementation_place_at_end(SYMBOL_)
	%feature("esb:implementation_place_at_end", 1) SYMBOL_
%enddef 


//TODO	Для того чтобы получить "приемлемое" английское имя во враппере
//		можно как использовать принятый у Свиг метод %rename(NAME_)	SYMBOL_;
//		(тогда NAME_ будет в аттрибуте sym:name)
//		так и сделать свою "фичу" esb:name
//		Плюсом свиг-метода является стандартность и учет возможеых нюансов
//			(то же является и минусом - не всегда однозначно понятно что, как и зачем Свиг со своими аттрибутами делает)
//		Плюсом фича-метода является однозначность - это наш и только наш аттрибут
//			(то же является и минусом - возможны какие-то тонкие нюансы конфликтов/согласования имен в контексте, которые Свиг учитывает, а мы тупо нет)
//		НУЖНО
//		Принять какое-то окончательное решение что использовать, а пока делаем и то и то..
%define %esb_name(NAME_)															/* symbol declaration here*/
	%feature("esb:name", NAME_)
%enddef

%define %esb_wrap_names(SYMBOL_, NAME_, NAME_LOCAL_)
	%esb_name(NAME_)				SYMBOL_;
	%esb_name_local(NAME_LOCAL_)	SYMBOL_;
	%rename(NAME_)					SYMBOL_
%enddef


%define %esb_wrap_info_(PREFIX_, CLASS_TYPE_, GUID_, NAME_, NAME_LOCAL_, DESCRIPTION_LOCAL_)
	%feature("esb:"PREFIX_"guid_attr", GUID_) 						CLASS_TYPE_;
#if NAME_ != ""
	%feature("esb:"PREFIX_"name", NAME_) 							CLASS_TYPE_;
#endif
#if NAME_LOCAL_ != ""
	%feature("esb:"PREFIX_"name_local", NAME_LOCAL_) 				CLASS_TYPE_;
#endif
#if DESCRIPTION_LOCAL_ != ""
	%feature("esb:"PREFIX_"description_local", DESCRIPTION_LOCAL_) CLASS_TYPE_
#endif
%enddef

%define %esb_wrap_info_cref(CLASS_TYPE_, GUID_, NAME_, NAME_LOCAL_, DESCRIPTION_LOCAL_)
	%esb_wrap_info_("cref_", CLASS_TYPE_, GUID_, NAME_, NAME_LOCAL_, DESCRIPTION_LOCAL_)
%enddef
%define %esb_wrap_info_ref(CLASS_TYPE_, GUID_, NAME_, NAME_LOCAL_, DESCRIPTION_LOCAL_)
	%esb_wrap_info_("ref_", CLASS_TYPE_, GUID_, NAME_, NAME_LOCAL_, DESCRIPTION_LOCAL_)
%enddef
%define %esb_wrap_info_cval(CLASS_TYPE_, GUID_, NAME_, NAME_LOCAL_, DESCRIPTION_LOCAL_)
	%esb_wrap_info_("cval_", CLASS_TYPE_, GUID_, NAME_, NAME_LOCAL_, DESCRIPTION_LOCAL_)
%enddef
//
%define %esb_wrap_info_enum(CLASS_TYPE_, GUID_, NAME_, NAME_LOCAL_, DESCRIPTION_LOCAL_)
	%esb_wrap_info_("enum_", CLASS_TYPE_, GUID_, NAME_, NAME_LOCAL_, DESCRIPTION_LOCAL_)
%enddef
//
%define %esb_wrap_info_stat(CLASS_TYPE_, GUID_, NAME_, NAME_LOCAL_, DESCRIPTION_LOCAL_)
	%esb_wrap_info_("stat_", CLASS_TYPE_, GUID_, NAME_, NAME_LOCAL_, DESCRIPTION_LOCAL_)
%enddef
//
%define %esb_wrap_info(CLASS_TYPE_, GUID_, NAME_, NAME_LOCAL_, DESCRIPTION_LOCAL_)
	%esb_wrap_info_("", CLASS_TYPE_, GUID_, NAME_, NAME_LOCAL_, DESCRIPTION_LOCAL_);
//NOTE	"безпрефиксное (безсуффиксное)" esb_wrap_info предназначено для самого типа (не его модификаций) 
//		дополнительно делает %rename, чтобы наше NAME_ было и в аттрибуте ::sym::name
//TODO	Воможно это и не нужно и нужно всегда пользоваться аттрибутом ::feature::esb::name
//		(нужно посмотреть..)
#if NAME_ != ""
	%rename(NAME_)									CLASS_TYPE_;
#endif
%enddef


%define %esb_comp_types(CLASS_TYPE_, COMPAT_TYPES_LIST_)
	%feature("esb:comp_types", COMPAT_TYPES_LIST_) CLASS_TYPE_
%enddef


/*	TODO	Раньше еще было %rename(NAME_), а теперь я как-то без него...
			Надо бы разобраться как и на что это влияет..
			Вроде-бы например на переименование конструкторов повлияло..
			- много сыпет отсутствием русского имени для конструкторов.. хз..
%define %esb_wrap_info(CLASS_TYPE_, GUID_, NAME_, NAME_LOCAL_, DESCRIPTION_LOCAL_)
	%esb_guid(GUID_)								CLASS_TYPE_;
#if NAME_ != ""
	%rename(NAME_)									CLASS_TYPE_;
#endif
#if NAME_LOCAL_ != ""
	%esb_name_local(NAME_LOCAL_)       				CLASS_TYPE_;
#endif
#if DESCRIPTION_LOCAL_ != ""
	%esb_description_local(DESCRIPTION_LOCAL_)		CLASS_TYPE_
#endif
%enddef
*/


%define %esb_wrap_as_val(CLASS_TYPE_, GUID_, NAME_, NAME_LOCAL_, DESCRIPTION_LOCAL_)
	%esb_declare_as_val_type(CLASS_TYPE_);
	%esb_wrap_info(CLASS_TYPE_, GUID_, NAME_, NAME_LOCAL_, DESCRIPTION_LOCAL_)
%enddef
%define %esb_wrap_as_ref(CLASS_TYPE_, GUID_, NAME_, NAME_LOCAL_, DESCRIPTION_LOCAL_)
	%esb_declare_as_ref_type(CLASS_TYPE_);
	%esb_wrap_info(CLASS_TYPE_, GUID_, NAME_, NAME_LOCAL_, DESCRIPTION_LOCAL_)
%enddef


%define %esb_wrap_as_template(CLASS_TYPE_, COMP_TYPES_, GUID_, NAME_, NAME_LOCAL_, DESCRIPTION_LOCAL_)
	%esb_wrap_info(CLASS_TYPE_, GUID_, #NAME_, NAME_LOCAL_, DESCRIPTION_LOCAL_);
	%esb_comp_types(CLASS_TYPE_, COMP_TYPES_);
//NOTE	слово "%template" нужно говорить ПОСЛЕ всех аттрибутов связанных с этой специализацией шаблона!
//		ПОСЛЕ! 
	%template(NAME_) CLASS_TYPE_
%enddef

%define %esb_wrap_as_template_val(CLASS_TYPE_, GUID_, NAME_, NAME_LOCAL_, DESCRIPTION_LOCAL_)
	%esb_declare_as_val_type(CLASS_TYPE_);
	%esb_wrap_as_template(CLASS_TYPE_, GUID_, NAME_, NAME_LOCAL_, DESCRIPTION_LOCAL_)
%enddef
%define %esb_wrap_as_template_ref(CLASS_TYPE_, GUID_, NAME_, NAME_LOCAL_, DESCRIPTION_LOCAL_)
	%esb_declare_as_ref_type(CLASS_TYPE_);
	%esb_wrap_as_template(CLASS_TYPE_, GUID_, NAME_, NAME_LOCAL_, DESCRIPTION_LOCAL_)
%enddef


//NOTE	В принципе, в этом случае, NAME_FOR_TYPE_ нам в общем то и не нужен (видно это имя нигде не будет)
//		Мы даем алгоритмам Свиг всю необходимую информацию через typemap
//		и флагом esb_well_known_type указываем ЕСвиг не делать этому типу враппер, а только "принять к сведению"
//		И директива Свиг '%template() TYPE_' может указываться без имени типа
//		НО! в этом случае Свиг вообще не вызывает наш модуль и мы вообще ничего не знаем о том что "это" существует
//			то есть не знаем что есть такой _known_type_, что приводит ко всяким нюансам и ошибкам..
//			(мы можем только косвенно что-то понять по потрохам Свиг-а полазив..)
//		Поэтому настоятельно рекомендуется имя типу порождаемому из шаблона ДАВАТЬ, 
//		и давать "нормально" как будто бы враппер делаем, а не от балды.. Мало ли что завтра будет..
//
%define %esb_map_well_known_for_arg_template(TYPE_, IN_WRAP_TYPE_, CONVERTER_FOR_ARG_, NAME_FOR_TYPE_)
	%typemap(ctype, converter_for_arg=CONVERTER_FOR_ARG_)	TYPE_	IN_WRAP_TYPE_;
	%esb_well_known_type(TYPE_);
	%template(NAME_FOR_TYPE_) TYPE_	
%enddef


/*
	typemap для ЕСБ в блоке ctype, т.к. по нашей логике ctype - это типы, которые с/с++ типы
	интерфейса библиотеки со стороны с/с++, а ЕСБ - это с++ библиотека
	Все с++ типы "byval" и "by-const-ref" становятся "by-const-ref" - это "in-аргументы"
	Все возвращаемые значение (ret=#ESB_TYPE_) остаются есб-типами как есть
	ЗАМЕТКА: В коде модуля все вновь встреченные типы ргистрируются ТОЧНО ТАКЖЕ (вызовом typemap_register_type_cref_in_and_byval_ret)
*/
%define %esb_typemap(CPP_TYPE_, ESB_TYPE_) 
	%typemap(ctype, ret=#ESB_TYPE_) 	CPP_TYPE_, const CPP_TYPE_&    			"const ESB_TYPE_&"
%enddef 


%esb_typemap(bool, 					Boolean)
// Все char считаем числовым типом
//TODO 	сомнительное решение. возможно char нужно представлять в есб-АПИ как строку
//		но можно где надо переопределять. 
//		также нужно помнить что для typedef-ов можно делать отдельные карты нр. %esb_typemap(uint8_t, Numeric)
%esb_typemap(char, 					Numeric)
%esb_typemap(signed char, 			Numeric)
%esb_typemap(unsigned char, 		Numeric)
//
%esb_typemap(short, 				Numeric)
%esb_typemap(unsigned short, 		Numeric)
%esb_typemap(int, 					Numeric)
%esb_typemap(unsigned int, 			Numeric)
%esb_typemap(long, 					Numeric)
%esb_typemap(unsigned long, 		Numeric)
%esb_typemap(long long, 			Numeric)	// это и int64_t
%esb_typemap(unsigned long long, 	Numeric)	// это и uint64_t
//
// Карты типов для типичный псевдонимов с++. 
// Их нужно делать отдельно, т.к. Свиг "раскручивает" typedef-ы только те, о которых знает
// но поскольку типы вида int64_t, ..., size_t  и т.п. определены в подключаемых заголовках
// а Свиг НЕ идет за и НЕ обрабатывает #include во включаемых файлах, то он и не видит эти typedef
// (stdint.h)
//%esb_typemap(int64_t, 			Numeric)
//TODO	Вроде не нужны отдельные карты типов. Свиг typedef-ы понимает. Нужно добавить все из stdint.h
typedef long long          int64_t;


// У 1С нет типов double и float, поэтому мы его сделаем отдельно (esb::Double, Действительное)
// поскольку "Double" может использоваться много где, то имеет смысл сделать его отдельно на уровне самого Есб
%esb_typemap(double, 				Double)
%esb_typemap(float, 				Float)	// можно сделать отдельно или использовать алиас
//
//TODO	здесь должен быть Date
//
%esb_typemap(const wchar_t*, 							String)
// массивы не могут быть возвращаемым типом - это всегда аргументы
%typemap(ctype) const wchar_t[ANY], const wchar_t[]   	"String"	//TODO	для String нужен отдельный макрос
%esb_typemap(std::wstring, 					String)					//		в том числе "захватывающий" и std::
%esb_typemap(std::wstring_view, 			String)

//TODO	Нужно сделать %esb_typemap(const char*, String)
//		но это требует "тихого" преобразования в wstring
//TODO std::string ТОЖЕ требует преобразования!
//		все тестировать и проверять надо!
%esb_typemap(std::string, 			String)
//%esb_typemap(std::string_view, 		String)
//

// Указатели и ссылки без const - это "in-out"-типы
// Для них должен быть осоая обработка "hidden-after-return-assign"
//TODO	Здесь должен быть wchar_t* (без const!), который есть "out" тип.
//		то же касается и ссылок std::
//		НУЖНО проверять "hidden-after-return-assign" технику
// И тоже касается просто char и char16_t
%typemap(ctype) char *, char *&, char[ANY], char[]   "char *"



// ВСЕ что не указано выше (ниже, отдельно) будет обработано этими картами
// (SWIGTYPE - это "любой-тип")


// странная карта.. она "выстреливает" когда реально посылают искать "void" и в частности для деструкторов классов.
// по сути использование этой карты свидетельствует о какой-то ошибке в модуле, т.к. посылать искать тип
// для метода, который известно что "void" ненормально
//TODO	неплохо было-бы отделить карту типов для дестукторов как-то так, но Свиг такое не понимает
//%typemap(ctype) void ~SWIGTYPE		"SWIG_DTOR_RESULT"
%typemap(ctype) void				"void"


//NOTE	Это ловушка. ЕСвиг отслеживает типы вида _ESB_** и если они есть где-то, 
//		то выводит предупреждение о проблеме в системе типов
%typemap(ctype, ret="__ESB_ENUM_TYPE__") 	enum SWIGTYPE, const enum SWIGTYPE&    	"__ESB_ENUM_TYPE__"


// Все типы по-константному указаделю мы принимаем аргументами как const UndefOr<'наш-ctype'>&
// а возвращаем просто как UndefOr<const 'наш-ctype'>. 
//TODO	А как это const 'наш-ctype' ??? У нас НЕТ В ЕСБ ТАКОГО!!! наш-ЕСБ-ТИП определяется ИНТЕРФЕЙСОМ !!!
// Заметка: для получения "наш-ctype as-is" мы ссылаемся на ctype:ret аттрибут в который мы пишем тип as-is
%typemap(ctype, ret="UndefOr<const $typemap(ctype:ret, $1_basetype)>") 	const SWIGTYPE * 	"const UndefOr<$typemap(ctype:ret, $1_basetype)>&"


// Все типы по mutable указателю (НЕ конст) возвращаем как UndefOr<наш-ctype>
//%typemap(ctype, ret="UndefOr<$typemap(ctype:ret, $1_basetype)>") 		SWIGTYPE * 			"void *"


// Все типы возвращаемые как константные ссылки мы возвращаем как некоторый тип
// определяемый псевдонимом const_ref_t<..>
//TODO	В нашем ЕСБ (1С, и Com вообще) нет понятия "константная ссылка на .."
//		у нас возможны только объекты с их интерфейсами.
//		Скажем для Массив его "константная ссылка" это ДРУГОЙ объект ФиксированныйМассив
//		А здесь нам нужен ДРУГОЙ объект построенный на ссылке на массив (скажем const std::vector<int>&). НЕ КОПИЯ !
//		С интерфейсом в котором оставлены только константные методы
//TODO	Наш Свиг пока такого НЕ ДЕЛАЕТ и эти объекты нужно ДЕЛАТЬ РУЧКАМИ !!! и специализировать const_ref_t<> для них
//		Нужно научить емиттер это делать, а также нужны аттрибуты (как минимум esb_guid) для этих объектов
//		т.е. некоторая специфика как сделано для %template
//TODO	Также возникает проблема, что с++ ссылка - это не самостоятельно живущий объект
//		и собственно объект на данные которого ссылаются вполне может быть уничтожен и получим "висящую ссылку"
//		Чтобы этого избежать класс ConstRef кроме ссылки на данные должен удерживать и IUnknown объекта содержащего данные
//		И тут опять проблема - наш Враппер (..+Object) ничего не знает о том что его еще заворачивают в Com-Объект
//		и решать эту проблему можно только на уровне Invoker как и с return_this_by_ref_
//
//TODO	это сейчас реализовано в коде в методе typemap_register_type_cref_in_and_byval_ret как ConstRefTypeOf
//		нужно разобраться где это все лучше делать - здесь или в коде или и тут и там. и как...
//%typemap(ctype, ret="const_ref_t<$typemap(ctype:ret, $1_basetype)>") 	const SWIGTYPE & 	"void *"


// Это говорит что во всех случаях где будет встречен ЛЮБОЙ-ТИП-ССЫЛКА это будет преобразовано в "void *"
// НО в случае возвращаемого из метода значения мы в коде специально проверяем аттрибут "ret"
// и возвращаем его значение
%typemap(ctype, ret="SWIGTYPE&") SWIGTYPE & "void *"

%typemap(ctype, ret="SWIGTYPE_") SWIGTYPE 	"void *"


%typemap(ctype) SWIGTYPE [] 		"void *"

%typemap(ctype) SWIGTYPE && 		"void *"
/* pointer to a class member */
%typemap(ctype) SWIGTYPE (CLASS::*) "char *"


//(const ClosedInterval& other) const;
//TODO	Как-то не получается указать Свиг, что нужно (можно) переименовывать только операторы
//		которые параметром имеют только "const THIS_CLASS&" (для строгости, а иначи можно напороться...)
//		Но хотя-бы понимает, что "const" в конце должен быть..
%rename(IsEqual)      		*::operator== 	/*(const THIS_CLASS& other_here)*/	const;
//%rename(NotEqual)      	operations_research::Domain::operator!=(const Domain& other) const;
%rename(IsNotEqual)			*::operator!= 	/*(const THIS_CLASS& other_here)*/	const;
//::operator<(const ClosedInterval& other) const;
%rename(IsLess)				*::operator< 	/*(const THIS_CLASS& other_here)*/	const;
%rename(IsLessOrEqual)		*::operator<= 	/*(const THIS_CLASS& other_here)*/	const;
%rename(IsGreater)			*::operator> 	/*(const THIS_CLASS& other_here)*/	const;
%rename(IsGreaterOrEqual)	*::operator>= 	/*(const THIS_CLASS& other_here)*/	const;
//
%rename(MakePositive)      	*::operator+() const;
%rename(MakeNegation)      	*::operator-() const;
%rename(MakeInvert)			*::operator~() const;
%rename(MakeAddWith)  		*::operator+ const;
%rename(MakeSubstractWith)  *::operator- const;
%rename(MakeMultiplyBy)		*::operator* const;
%rename(MakeDivideOn)  		*::operator/ const;
%rename(MakeMod)      		*::operator% const;
%rename(MakeLeftShift)   	*::operator<< const;
%rename(MakeRightShift)   	*::operator>> const;
%rename(MakeAnd)      		*::operator& const;
%rename(MakeOr)       		*::operator| const;
%rename(MakeXor)      		*::operator^ const;
//
%rename(ElementAt)      	*::operator[] 	/*(some-what INDEX here)*/	 const;
//
%rename(Multiply)			*::operator*=;
%rename(Add)				*::operator+=;
%rename(Substruct)			*::operator-=;


//TODO	посмотреть rubyautodoc.swg
/*	О как можно ?
%typemap(out) CONST TYPE[], CONST TYPE[ANY], CONST TYPE (CLASS::*) %{
#error "typemaps for $1_type not available"
%}
...
%typemap (ctype)    SWIG_SHARED_PTR_QNAMESPACE::shared_ptr< CONST TYPE > "void *"
...
А это что значит?
%apply SWIGTYPE (CLASS::*) { SWIGTYPE (CLASS::*const) }
%apply SWIGTYPE & { SWIGTYPE (CLASS::*const&) }
...
%typemap(in, canthrow=1, fragment="<memory>") SWIGTYPE MOVE ($&1_type argp)
*/




/* csharp keywords */
//TODO	%keywordwarn(...) директива, которая видимо должна что-то говорить если используются зарезервированные имена
//		оставлю пока. С++ на C# похож по ключевым словам.
//		нужно разобраться и возможно добавить нашей esb-специфики
//%include <esb_csharpkw.swg>


// "typecheck" карта типов влияет на то, как "legacy-swig" АПИ обрабатывает перегруженные методы (как минимум)
// С одной стороны вещь нужная, но не в таком виде как это сделано у Свиг
// По сути перегруженные методы ВСЕГДА могут быть обработаны т.к. 
// это всего лишь вопрос более-менее толкового назначения имени элементам букета перегрузок
//TODO	Если НЕ подключать typecheck, то Свиг генерит ворнинг, но все емитит (это хорошо)
//		А если ПОДКЛЮЧИТЬ, то ворнинга НЕТ, хотя все емитит так же (это НЕ очень хорошо)
%include esb_typecheck.swg



// ***	Все что ниже относится к "legacy-swig"	***
// (с минимальными изменениями скопировано из csharp.swg)
/*	TODO
	По большому счету это все нужно как-то убрать или как-то пеработать
	Но! 
	"legacy-swig" АПИ существенно завязан на эти свои карты типов "in", "out", т.п.
	Изменения в них зачастую малопредсказуемым образом влияют на повередение методов класса Language
	и прочие АПИ функции обработки узлов синтаксического дерева.
	к примеру отсутствие в карте "in" какого-нибудь типа для параметра приводит к тому, 
	что этот параметр просто ислючатся из рассмотрения
	(итерирование по списку параметров идет по аттрибутам tmap:in:next)
	ПОЭТОМУ
	Пока все оставлено as-is - "работает - и ладно"
	В будущих версиях "eswig" нужно будет постепенно отказываться от "legacy-swig"
	(нр. унаследоваться не от Language, а напрямую от Dispatch)
	и одновременно приводить в порядок эти карты типов
*/




/* 	The following are the in and out typemaps. 
	These are the PInvoke code generating typemaps for converting from C# to C and visa versa. */

/* primitive types */
%typemap(in) bool
%{ $1 = $input ? true : false; %}

%typemap(in) char, 
             signed char, 
             unsigned char, 
             short, 
             unsigned short, 
             int, 
             unsigned int, 
             long, 
             unsigned long, 
             long long, 
             unsigned long long, 
             float, 
             double
%{ $1 = ($1_ltype)$input; %}


%typemap(out) bool               %{ $result = $1; %}
%typemap(out) char               %{ $result = $1; %}
%typemap(out) signed char        %{ $result = $1; %}
%typemap(out) unsigned char      %{ $result = $1; %}
%typemap(out) short              %{ $result = $1; %}
%typemap(out) unsigned short     %{ $result = $1; %}
%typemap(out) int                %{ $result = $1; %}
%typemap(out) unsigned int       %{ $result = (unsigned int)$1; %}
%typemap(out) long               %{ $result = $1; %}
%typemap(out) unsigned long      %{ $result = $1; %}
%typemap(out) long long          %{ $result = $1; %}
%typemap(out) unsigned long long %{ $result = $1; %}
%typemap(out) float              %{ $result = $1; %}
%typemap(out) double             %{ $result = $1; %}


/* char * - treat as String */
%typemap(in) char * %{ $1 = ($1_ltype)$input; %}
%typemap(out) char * %{ $result = SWIG_csharp_string_callback((const char *)$1); %}

/* char *& - treat as String */
%typemap(in) char *& ($*1_ltype temp = 0) %{ 
  temp = ($*1_ltype)$input;
  $1 = &temp;
%}
%typemap(out) char *& %{ if ($1) $result = SWIG_csharp_string_callback((const char *)*$1); %}

%typemap(out, null="") void ""


/* primitive types by const reference */
%typemap(in) const bool & ($*1_ltype temp)
%{ temp = $input ? true : false; 
   $1 = &temp; %}


%typemap(in) const char & ($*1_ltype temp), 
             const signed char & ($*1_ltype temp), 
             const unsigned char & ($*1_ltype temp), 
             const short & ($*1_ltype temp), 
             const unsigned short & ($*1_ltype temp), 
             const int & ($*1_ltype temp), 
             const unsigned int & ($*1_ltype temp), 
             const long & ($*1_ltype temp), 
             const unsigned long & ($*1_ltype temp), 
             const long long & ($*1_ltype temp), 
             const unsigned long long & ($*1_ltype temp), 
             const float & ($*1_ltype temp), 
             const double & ($*1_ltype temp)
%{ temp = ($*1_ltype)$input; 
   $1 = &temp; %}



%typemap(out) const bool &               %{ $result = *$1; %}
%typemap(out) const char &               %{ $result = *$1; %}
%typemap(out) const signed char &        %{ $result = *$1; %}
%typemap(out) const unsigned char &      %{ $result = *$1; %}
%typemap(out) const short &              %{ $result = *$1; %}
%typemap(out) const unsigned short &     %{ $result = *$1; %}
%typemap(out) const int &                %{ $result = *$1; %}
%typemap(out) const unsigned int &       %{ $result = (unsigned int)*$1; %}
%typemap(out) const long &               %{ $result = *$1; %}
%typemap(out) const unsigned long &      %{ $result = *$1; %}
%typemap(out) const long long &          %{ $result = *$1; %}
%typemap(out) const unsigned long long & %{ $result = *$1; %}
%typemap(out) const float &              %{ $result = *$1; %}
%typemap(out) const double &             %{ $result = *$1; %}



/* Default handling. Object passed by value. Convert to a pointer */
//TODO	если убрать эту строку, то куча ворнингов на перегруженные методы и ассерт в емитере
//		ситуация требует изучения. видимо Свиг вообще начинает "не видеть" параметры "by-val"
%typemap(in) SWIGTYPE "void *"


%typemap(out) SWIGTYPE 
#ifdef __cplusplus
%{ $result = new $1_ltype($1); %}
#else
{
  $&1_ltype $1ptr = ($&1_ltype) malloc(sizeof($1_ltype));
  memmove($1ptr, &$1, sizeof($1_type));
  $result = $1ptr;
}
#endif


/* Generic pointers and references */
%typemap(in) SWIGTYPE * %{ $1 = ($1_ltype)$input; %}
%typemap(in, fragment="SWIG_UnPackData") SWIGTYPE (CLASS::*) %{ 
  SWIG_UnpackData($input, (void *)&$1, sizeof($1));
%}


%typemap(out) SWIGTYPE * %{ $result = (void *)$1; %} 
%typemap(out, fragment="SWIG_PackData") SWIGTYPE (CLASS::*) %{
  char buf[128];
  char *data = SWIG_PackData(buf, (void *)&$1, sizeof($1));
  *data = '\0';
  $result = SWIG_csharp_string_callback(buf);
%}
%typemap(out) SWIGTYPE & %{ $result = (void *)$1; %} 
%typemap(out) SWIGTYPE && %{ $result = (void *)$1; %} 


/* Default array handling */
%typemap(in) SWIGTYPE [] %{ $1 = ($1_ltype)$input; %}
%typemap(out) SWIGTYPE [] %{ $result = $1; %} 

/* char arrays - treat as String */
%typemap(in) char[ANY], char[] %{ $1 = ($1_ltype)$input; %}
%typemap(out) char[ANY], char[] %{ $result = SWIG_csharp_string_callback((const char *)$1); %}


/* Pointer reference typemaps */
%typemap(ctype) SWIGTYPE *const& "void *"

%typemap(in) SWIGTYPE *const& ($*1_ltype temp = 0)
%{ temp = ($*1_ltype)$input;
   $1 = ($1_ltype)&temp; %}
%typemap(out) SWIGTYPE *const&
%{ $result = (void *)*$1; %} 



/* 64-bit architecture specific typemaps */
#if defined(SWIGWORDSIZE64)
%apply long long { long };
%apply unsigned long long { unsigned long };
%apply const long long & { const long & };
%apply const unsigned long long & { const unsigned long & };
#endif


/* size_t maps to C# 32-bit uint type */
%apply unsigned int { size_t };
%apply const unsigned int & { const size_t & };


/* Array reference typemaps */
%apply SWIGTYPE & { SWIGTYPE ((&)[ANY]) }
%apply SWIGTYPE && { SWIGTYPE ((&&)[ANY]) }


/* const pointers */
%apply SWIGTYPE * { SWIGTYPE *const }
%apply SWIGTYPE (CLASS::*) { SWIGTYPE (CLASS::*const) }
%apply SWIGTYPE & { SWIGTYPE (CLASS::*const&) }



