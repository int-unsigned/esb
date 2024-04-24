using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
#if DEBUG || DEBUG_IN_RELEASE
	using System.Diagnostics;
#endif



namespace EsbStudio.Esl
{
	using static EsbStudio.Esl.EslContextApi;
	using static EsbStudio.Esl.EslContextApi2;


	internal class EslContextGlobal 
	{
		private class EslTypesGlobal
		{
			public EslType Void_ = EslTypes.Void_;
			// Это и не Войд, и не Неопределено и не Любое (которые нормальные типы) - парсер не вывел... Можно считать или Любое или Ошибка в зависимости от предпочтений
			public EslType Unknown_		= EslTypes.Unknown_;
			public EslType Undefined_	= EslTypes.Undefined_;
			public EslType Any_			= EslTypes.Any_;
			public EslType Boolean_		= EslTypes.Boolean_;
			public EslType Number_		= EslTypes.Number_;
			public EslType DateTime_	= EslTypes.DateTime_;
			public EslType String_		= EslTypes.String_;
			public EslType TypeType_	= EslTypes.TypeType_;
			//
			public EslType NumberOrUndef_ = EslTypes.NumberOrUndef_;
			//
			public EslTypeEnumType		MessageStatus_;
			public EslTypeEnumerator	Enumerator_MessageStatus_;
			public EslTypeEnumType		PlatformType_;			
			public EslTypeEnumerator	Enumerator_PlatformType_;
			public EslTypeEnumType		QuestionDialogMode_;
			public EslTypeEnumerator	Enumerator_QuestionDialogMode_;
			public EslTypeEnumType		DialogReturnCode_;
			public EslTypeEnumerator	Enumerator_DialogReturnCode_;

			public EslTypeClass			Chars_;
			public EslTypeClass			FixedArray_;
			public EslTypeClass			Array_;
		}

		private class Type_FixedArray
		{
			internal static string NameEn_ = "FixedArray";
			internal static string CodeEn_ = "FIXEDARRAY";
			internal static string NameRu_ = "ФиксированныйМассив";
			internal static string CodeRu_ = "ФИКСИРОВАННЫЙМАССИВ";
			//
			internal static EslName Name_ = new EslName(NameRu_, CodeRu_);			
			//
			public static EslDecl[] GetInterface(EslTypesGlobal TYPES_) 
			{
				return new EslDecl[]
				{
				FUNC("ВГраница", TYPES_.Number_),
				FUNC("Количество", TYPES_.Number_),
				FUNC("Найти", TYPES_.Any_, PARA("<Значение>", TYPES_.NumberOrUndef_)),
				FUNC("Получить", TYPES_.Any_, PARA("<Индекс>", TYPES_.Number_)),
				//TODO	В интерфейсе массива есть еще [] и вообще могут быть всякие операторы. В с++ они реально методы вида operator[] и в списке автодополнения так и показываются
				//		(и так их и вызывать можно), но для языков попроще это не принято. Заманчиво их как-то особо показать в списке, а при выборе подправить строку вызова - убрать точку.
				//		Попрубую пока тупо. Постепенно доработается.
				FUNC(EslMeth.OperatorIndexableName_, TYPES_.Any_, PARA("<Индекс>", TYPES_.Number_)),
				};
			}
			public static EslMeth[] GetConstructors(EslType my_type_, EslTypesGlobal TYPES_) 
			{
				return CTOR(my_type_,	PARA("<Массив>", TYPES_.Array_)	);

			}
		}

		private class Type_Array
		{
			internal static string NameEn_ = "Array";
			internal static string CodeEn_ = "ARRAY";
			internal static string NameRu_ = "Массив";
			internal static string CodeRu_ = "МАССИВ";
			//TODO	EslName (и/или EslTerm) нужны
			internal static EslName Name_ = new EslName(NameRu_, CodeRu_);

			public static EslDecl[] GetInterface(EslTypesGlobal TYPES_) {
				return new EslDecl[]
				{
				FUNC("ВГраница", EslTypes.Number_),
				// <Значение> - НЕ-Обязательный тип. Если Опущено - Неопределено. Поэтому у декларации ДВА метода - отдин БЕЗ <Значение>, второй С НИМ
				PROC("Вставить", PARASET(PARA("<Индекс>", EslTypes.Number_)), PARASET(PARA("<Индекс>", EslTypes.Number_), PARA("<Значение>", EslTypes.Any_))),
				// Тоже самое, что и Вставить
				PROC("Добавить", PARASET(PARA("<Индекс>", EslTypes.Number_)), PARASET(PARA("<Индекс>", EslTypes.Number_), PARA("<Значение>", EslTypes.Any_))),
				FUNC("Количество", EslTypes.Number_),
				FUNC("Найти", EslTypes.Any_, PARA("<Значение>", EslTypes.NumberOrUndef_)),
				PROC("Очистить"),
				FUNC("Получить", EslTypes.Any_, PARA("<Индекс>", EslTypes.Number_)),	//Аналогично []
				PROC("Удалить", PARA("<Индекс>", EslTypes.Number_)),					//Если указанный в параметре индекс больше, чем ВГраница, то никаких действий не производится и не выдается сообщение "Индекс находится за границами массива".
				PROC("Установить", PARA("<Индекс>", EslTypes.Number_), PARA("<Значение>", EslTypes.Any_)),
				FUNC(EslMeth.OperatorIndexableName_, TYPES_.Any_, PARA("<Индекс>", TYPES_.Number_)),
				};
			}
			public static EslMeth[] GetConstructors(EslType my_type_, EslTypesGlobal TYPES_) 
			{
				//TODO	Массив(<КоличествоЭлементов1>,...,<КоличествоЭлементовN>) - нам нужно ..., или точнее ...бесконечность и ...колво, т.к. есть функции типа "до 10 параметров"
				return CTOR(my_type_, 
							PARASET(PARA("<ФиксированныйМассив>", TYPES_.FixedArray_)),
							PARASET(PARA("<КоличествоЭлементов1>", TYPES_.Number_)),
							PARASET(PARA("<КоличествоЭлементов1>", TYPES_.Number_), PARA("<КоличествоЭлементов2>", EslTypes.Number_))
							);
			}
		} // class Array


		public static class Enums
		{
			//static var xxx = foo_e.a.

			//1. Сам енумератор есть некоторый объект со своим типом, то тогда этот тип енумератора отличается по имени от имени типа underline_type<>
			//		Тогда у underline_type нет никакого контекста - ни свойств ни методов (ХОТЯ МОГУТ БЫТЬ! типа .ToString() или .ToInt т.п. - методы от Object)
			//		Тогда у типа енумератора должен быть набор деклараций (свойств, констант) возвращающих элементы енумератор
			//		Тогда у глобального контекста должен быть набор свойств с именами типов-енумов (НЕ именами типов самих енумераторов) возвращающие тип ЕНУМЕРАТОРА
			//
			//2. Сам енумератор не является объектом имеющим тип. Просто некоторая декларация типа модуля
			//		Тогда его (декларацию) также надо добавить в список деклараций глобального модуля и у нас получится, что среди деклараций модуля появятся
			//		декларации вложенных модулей, НО имя у этого модуля должно совпадать с именем типа-енума то есть фактически и у типа енума и у енумератора одно имя
			//		НО! в декларации типа-енума никакого контекста быть не должно, а в декларации енумератора должны быть прописаны элементы... где-то

			//TODO	вот такие большие строки делаю, да еще и с _ только чтобы это на экране пристойно смотрелось..
			//		хз.. то ли им DisplayText придумать, то-ли имена типов такие и должны быть..
			internal static string EnumPrefixNameEn_ = "Enumerator_";
			internal static string EnumPrefixCodeEn_ = "ENUMERATOR_";
			internal static string EnumPrefixNameRu_ = "Перечисление_";
			internal static string EnumPrefixCodeRu_ = "ПЕРЕЧИСЛЕНИЕ_";
			//
			internal static string EnumPrefixName_ = EnumPrefixNameRu_;
			internal static string EnumPrefixCode_ = EnumPrefixCodeRu_;
			//
			static EslName make_enumerator_name(EslName enum_name_) {
				return new EslName($"{EnumPrefixName_}{enum_name_.GetNameText()}", $"{EnumPrefixCode_}{enum_name_.GetNameCode()}");
			}
			//
			//
			public static class MessageStatus
			{
				internal static string EnumTypeNameTextEn_	= "MessageStatus";
				internal static string EnumTypeCodeTextEn_	= "MESSAGESTATUS";
				internal static string EnumTypeNameTextRu_	= "СтатусСообщения";
				internal static string EnumTypeCodeTextRu_	= "СТАТУССООБЩЕНИЯ";
				//
				internal static string EnumTypeNameText_	= EnumTypeNameTextRu_;
				internal static string EnumTypeCodeText_	= EnumTypeCodeTextRu_;
				//
				internal static EslName EnumTypeName_		= new EslName(EnumTypeNameText_, EnumTypeCodeText_);
				internal static EslName EnumeratorName_		= make_enumerator_name(EnumTypeName_);
				//
				internal static EslDecl[] GetDeclarations(EslTypeEnumType my_enum_type_) {
					return new EslDecl[]
					{
						ENUM_ELEM("БезСтатуса",  my_enum_type_),
						ENUM_ELEM("Важное",      my_enum_type_),
						ENUM_ELEM("Внимание",    my_enum_type_),
						ENUM_ELEM("Информация",  my_enum_type_),
						ENUM_ELEM("Обычное",     my_enum_type_),
						ENUM_ELEM("ОченьВажное", my_enum_type_),
					};
				}
			}
			public class PlatformType
			{
				internal static string EnumTypeNameTextEn_	= "PlatformType";
				internal static string EnumTypeCodeTextEn_	= "PLATFORMTYPE";
				internal static string EnumTypeNameTextRu_	= "ТипПлатформы";
				internal static string EnumTypeCodeTextRu_	= "ТИППЛАТФОРМЫ";
				//
				internal static string EnumTypeNameText_	= EnumTypeNameTextRu_;
				internal static string EnumTypeCodeText_	= EnumTypeCodeTextRu_;
				//
				internal static EslName EnumTypeName_		= new EslName(EnumTypeNameText_, EnumTypeCodeText_);
				internal static EslName EnumeratorName_		= make_enumerator_name(EnumTypeName_);
				//
				internal static EslDecl[] GetDeclarations(EslTypeEnumType my_enum_type_) {
					return new EslDecl[]
					{
						ENUM_ELEM("Linux_x86",       my_enum_type_),
						ENUM_ELEM("Linux_x86_64",    my_enum_type_),
						ENUM_ELEM("MacOS_x86",       my_enum_type_),
						ENUM_ELEM("MacOS_x86_64",    my_enum_type_),
						ENUM_ELEM("Windows_x86",     my_enum_type_),
						ENUM_ELEM("Windows_x86_64",  my_enum_type_),
					};
				}
			}
			public class QuestionDialogMode
			{
				internal static string EnumTypeNameTextEn_ = "QuestionDialogMode";
				internal static string EnumTypeCodeTextEn_ = "QUESTIONDIALOGMODE";
				internal static string EnumTypeNameTextRu_ = "РежимДиалогаВопрос";
				internal static string EnumTypeCodeTextRu_ = "РЕЖИМДИАЛОГАВОПРОС";
				//
				internal static string EnumTypeNameText_ = EnumTypeNameTextRu_;
				internal static string EnumTypeCodeText_ = EnumTypeCodeTextRu_;
				//
				internal static EslName EnumTypeName_ = new EslName(EnumTypeNameText_, EnumTypeCodeText_);
				internal static EslName EnumeratorName_ = make_enumerator_name(EnumTypeName_);
				//
				internal static EslDecl[] GetDeclarations(EslTypeEnumType my_enum_type_) {
					return new EslDecl[]
					{
						ENUM_ELEM("ДаНет",							my_enum_type_),	//ДаНет (YesNo)
						ENUM_ELEM("ДаНетОтмена",					my_enum_type_),	//ДаНетОтмена (YesNoCancel)
						ENUM_ELEM("ОК",								my_enum_type_),	//ОК (OK)
						ENUM_ELEM("ОКОтмена",						my_enum_type_),	//ОКОтмена (OKCancel)
						ENUM_ELEM("ПовторитьОтмена",				my_enum_type_),	//ПовторитьОтмена (RetryCancel)
						ENUM_ELEM("ПрерватьПовторитьПропустить",	my_enum_type_),	//ПрерватьПовторитьПропустить (AbortRetryIgnore)
					};
				}
			}
			public class DialogReturnCode
			{
				internal static string EnumTypeNameTextEn_ = "DialogReturnCode";
				internal static string EnumTypeCodeTextEn_ = "DIALOGRETURNCODE";
				internal static string EnumTypeNameTextRu_ = "КодВозвратаДиалога";
				internal static string EnumTypeCodeTextRu_ = "КОДВОЗВРАТАДИАЛОГА";
				//
				internal static string EnumTypeNameText_ = EnumTypeNameTextRu_;
				internal static string EnumTypeCodeText_ = EnumTypeCodeTextRu_;
				//
				internal static EslName EnumTypeName_ = new EslName(EnumTypeNameText_, EnumTypeCodeText_);
				internal static EslName EnumeratorName_ = make_enumerator_name(EnumTypeName_);
				//
				internal static EslDecl[] GetDeclarations(EslTypeEnumType my_enum_type_) {
					return new EslDecl[]
					{	// хм.. интересно что в хелпе элементы сразу перечислены по алфавиту (по возрастанию)
						ENUM_ELEM("Да",             my_enum_type_),	//Да (Yes)
						ENUM_ELEM("Нет",            my_enum_type_),	//Нет (No)
						ENUM_ELEM("ОК",             my_enum_type_),	//ОК (OK)
						ENUM_ELEM("Отмена",         my_enum_type_),	//Отмена (Cancel)
						ENUM_ELEM("Повторить",      my_enum_type_),	//Повторить (Retry)
						ENUM_ELEM("Прервать",		my_enum_type_),	//Прервать (Abort)
						ENUM_ELEM("Пропустить",		my_enum_type_),	//Пропустить (Ignore)
						ENUM_ELEM("Таймаут",		my_enum_type_),	//Таймаут (Timeout)
					};
				}
			}


		} // class Enums

		static class Classes 
		{
			// Специфическая штука. И не перечисление и не класс, а точнее класс у которого одни статические поля-константы - неймспейс? модуль? - нет свой тип имеет
			public class ClassChars
			{
				public static string NameTextEn_ = "Chars";
				public static string NameCodeEn_ = "CHARS";
				public static string NameTextRu_ = "Символы";
				public static string NameCodeRu_ = "СИМВОЛЫ";
				//
				public static EslName NameEn_ = new EslName(NameTextEn_, NameCodeEn_);
				public static EslName NameRu_ = new EslName(NameTextRu_, NameCodeRu_);
				public static EslName Name_		= NameRu_;
				//
				public static EslDecl[] GetDeclarations() {
					return new EslDecl[]
					{	// хм.. интересно что в хелпе элементы сразу перечислены по алфавиту (по возрастанию)
						CONST("ВК",         EslTypes.String_),	//ВК (CR)
						CONST("ВТаб",       EslTypes.String_),	//ВТаб (VTab)
						CONST("НПП",        EslTypes.String_),	//НПП (NBSp)
						CONST("ПС",         EslTypes.String_),	//ПС (LF)
						CONST("ПФ",			EslTypes.String_),	//ПФ (FF)
						CONST("Таб",		EslTypes.String_),	//Таб (Tab)
					};
				}
			}
		}

		private static	EslDecl[]			m_declarations;
		private static	HashSet<EslDecl>	m_declarations_index;
		
		private static EslDecl[]			m_classes;
		private static	HashSet<EslDecl>	m_classes_index;


		public static void Initialize() 
		{
			//TODO	Копипаста жжжуть.. как только так сразу нужно по человечески сделать..

			EslTypesGlobal MY_TYPES = new EslTypesGlobal();
			// енумератора, они ни от чего не зависят кроме себя
			MY_TYPES.MessageStatus_	= EslTypes.RegisterEnumType(Enums.MessageStatus.EnumTypeName_);
			MY_TYPES.Enumerator_MessageStatus_ = EslTypes.RegisterEnumerator(Enums.MessageStatus.EnumeratorName_, Enums.MessageStatus.GetDeclarations(MY_TYPES.MessageStatus_));
			MY_TYPES.PlatformType_	= EslTypes.RegisterEnumType(Enums.PlatformType.EnumTypeName_);
			MY_TYPES.Enumerator_PlatformType_ = EslTypes.RegisterEnumerator(Enums.PlatformType.EnumeratorName_, Enums.PlatformType.GetDeclarations(MY_TYPES.PlatformType_));
			MY_TYPES.QuestionDialogMode_ = EslTypes.RegisterEnumType(Enums.QuestionDialogMode.EnumTypeName_);
			MY_TYPES.Enumerator_QuestionDialogMode_ = EslTypes.RegisterEnumerator(Enums.QuestionDialogMode.EnumeratorName_, Enums.QuestionDialogMode.GetDeclarations(MY_TYPES.QuestionDialogMode_));
			MY_TYPES.DialogReturnCode_ = EslTypes.RegisterEnumType(Enums.DialogReturnCode.EnumTypeName_);
			MY_TYPES.Enumerator_DialogReturnCode_ = EslTypes.RegisterEnumerator(Enums.DialogReturnCode.EnumeratorName_, Enums.DialogReturnCode.GetDeclarations(MY_TYPES.DialogReturnCode_));

			// классы не зависящие от других. сразу и целиком
			MY_TYPES.Chars_			= EslTypes.RegisterTypeClass(Classes.ClassChars.Name_, Classes.ClassChars.GetDeclarations(), EslType.ConstructorsAbsent_);
			// классы, предварительная декларация
			MY_TYPES.FixedArray_	= EslTypes.RegisterTypeClassName(Type_FixedArray.Name_);
			MY_TYPES.Array_			= EslTypes.RegisterTypeClassName(Type_Array.Name_);
			// система типов готова
			//
			// После того, как все типы предварительно объявлены, мы можем их определить окончательно
			MY_TYPES.FixedArray_	.SetDefinition(Type_FixedArray.GetInterface(MY_TYPES), Type_FixedArray.GetConstructors(MY_TYPES.FixedArray_, MY_TYPES));
			MY_TYPES.Array_			.SetDefinition(Type_Array.GetInterface(MY_TYPES), Type_Array.GetConstructors(MY_TYPES.Array_, MY_TYPES));
			//

			//Все типы реди, Определяем декларации методов и свойств для енумераторов
			m_declarations = new EslDecl[]
			{	
				// *** НЕ ЗАБЫВАЕМ, ЧТО НУЖНО ВСТАВЛЯТЬ ПО ВОЗРАСТАНИЮ ИМЕНИ! КОНСТЕКСПР СОРТИРОВКИ ТУТ НЕТ! ***

				// 1C позволяет опускать параметры в середине списка. эту уникальную возможность недоступную в большинстве других языков мы поддерживать небудем
				// (реализуем через перегрузку методов)
				//TODO Хотя с точки зрения "просто показать на экран" можно и сделать. Ну а в Есб (с++) свои в общем-то правила. тоже выкрутиться можно..

				//Вопрос(<ТекстВопроса>, <Кнопки>, <Таймаут>, <КнопкаПоУмолчанию>, <Заголовок>, <КнопкаТаймаута>) 
				//TODO	Очень сложный для нас пока метод. Много вариантов типов параметров. И у него еще и в зависимости от типа параметров разное возвращаемое значение.
				//		нужно заняться как модельным. пока по-простому
				FUNC("Вопрос",  MY_TYPES.DialogReturnCode_,		
						PARASET(PARA("<ТекстВопроса>", EslTypes.String_), PARA("<Кнопки>", MY_TYPES.QuestionDialogMode_)),
						PARASET(PARA("<ТекстВопроса>", EslTypes.String_), PARA("<Кнопки>", MY_TYPES.QuestionDialogMode_), PARA("<Таймаут>", MY_TYPES.Number_)),
						PARASET(PARA("<ТекстВопроса>", EslTypes.String_), PARA("<Кнопки>", MY_TYPES.QuestionDialogMode_), PARA("<Таймаут>", MY_TYPES.Number_), PARA("<КнопкаПоУмолчанию>", MY_TYPES.DialogReturnCode_)),
						PARASET(PARA("<ТекстВопроса>", EslTypes.String_), PARA("<Кнопки>", MY_TYPES.QuestionDialogMode_), PARA("<Таймаут>", MY_TYPES.Number_), PARA("<КнопкаПоУмолчанию>", MY_TYPES.DialogReturnCode_), PARA("<Заголовок>", EslTypes.String_)),
						PARASET(PARA("<ТекстВопроса>", EslTypes.String_), PARA("<Кнопки>", MY_TYPES.QuestionDialogMode_), PARA("<Таймаут>", MY_TYPES.Number_), PARA("<КнопкаПоУмолчанию>", MY_TYPES.DialogReturnCode_), PARA("<Заголовок>", EslTypes.String_), PARA("<КнопкаТаймаута>", EslTypes.Any_))
						),
				PROC("ЗавершитьРаботуСистемы",			//ЗавершитьРаботуСистемы(<ЗапрашиватьВозможность>, <ПерезапуститьПриЗавершении>, <ДополнительныеПараметрыКоманднойСтроки>) 
						PARASET(), 
						PARASET(PARA("<ЗапрашиватьВозможность>", EslTypes.Boolean_)), 
						PARASET(PARA("<ЗапрашиватьВозможность>", EslTypes.Boolean_), PARA("<ПерезапуститьПриЗавершении>", MY_TYPES.Boolean_)),
						PARASET(PARA("<ЗапрашиватьВозможность>", EslTypes.Boolean_), PARA("<ПерезапуститьПриЗавершении>", MY_TYPES.Boolean_), PARA("<ДополнительныеПараметрыКоманднойСтроки>", MY_TYPES.String_))
						),
				ENUM_PROP(MY_TYPES.DialogReturnCode_, MY_TYPES.Enumerator_DialogReturnCode_),		//КодВозвратаДиалога
				FUNC("Новый", EslTypes.Any_,														//Новый(<Тип>[, <ПараметрыКонструктора>])
					PARASET(PARA("<Тип>", EslTypes.TypeType_)),		//TODO А возвращать-то она должна не Любое, а тот тип что заказали - в шаблоны могешь? не функция оно.. спец.функция
					PARASET(PARA("<Тип>", EslTypes.TypeType_), PARA("<ПараметрыКонструктора>", MY_TYPES.Array_))	//TODO А фик-массив? проверить.
					),
				PROP("Символы", MY_TYPES.Chars_),													//Символы (Chars)
				ENUM_PROP(MY_TYPES.QuestionDialogMode_, MY_TYPES.Enumerator_QuestionDialogMode_),	//РежимДиалогаВопрос				
				PROC("Сообщить",																	//Сообщить(<ТекстСообщения>, <Статус>) 
						PARASET(PARA("<ТекстСообщения>", EslTypes.String_)), 
						PARASET(PARA("<ТекстСообщения>", EslTypes.String_), PARA("<Статус>", MY_TYPES.MessageStatus_))),
				ENUM_PROP(MY_TYPES.MessageStatus_, MY_TYPES.Enumerator_MessageStatus_),				//СтатусСообщения				
				FUNC("СтрДлина", EslTypes.Number_, PARA("<Строка>", EslTypes.String_)),				//СтрДлина(<Строка> Как Строка)
				ENUM_PROP(MY_TYPES.PlatformType_, MY_TYPES.Enumerator_PlatformType_),				//ТипПлатформы				
			};			
			m_declarations_index = m_declarations.ToHashSet(EslDeclComparierEq.Instance_);
			//
			m_classes = new EslDecl[] { 
				MY_TYPES.FixedArray_, 
				MY_TYPES.Array_ 
			};
			m_classes_index = m_classes.ToHashSet(EslDeclComparierEq.Instance_);
			//готово
		} //Initialize


		//////////
		internal static EslDecl FindDeclaration(EslName declaration_name_) {
			// мы декларации ищем просто по имени. не наше дело разбираться что там нужно - свойства, методы, т.п.
			return EslContextApi.FindDeclaration(m_declarations_index, declaration_name_);
		}

		// мы как модуль можем экспортировать разные декларации - методов, типов, енумов, других модулей (неймспейс)
		// это задача вызывающего попросить у нас то что ему в контексте надобно
		//TODO	Конкретно эта функция вызывается из AddMatchDeclarationsGlobal для заполнения списка "похожих"
		//		(юзверь сам не знает чего (или знает но сказать не может, лучший друг наш..))
		//		Сейчас там алгоритм примитивнейшего перебора, и когда у нас бедет много деклараций нужно будет думать над оптимизацие поиска "похожих"
		//		по задумке мы не должны знать алгоритм "похожести" - он должен настраиваться где-то у них ы LanguageService
		//		и сюда нам должны передать этот класс-алгоритм, а мы должны будем применить его к нашим внутренним структурам наиоптимальнейшим образом...
		//			void AddMatchDeclarationsGlobal(HashSet<EslDecl> match_declarations_, string declaration_code_part_) ->
		//			void AddMatchDeclarationsGlobal(<DeclHashSet> match_declarations_, <match-algorithm-> match_)
		//пока тупо возвращаем все - пусть там разбираются
		public static EslDecl[] GetModuleDeclarationsExport() {
			return m_declarations;
		}
		public static EslDecl[] GetModuleClassesExport() {
			return m_classes;
		}
	} // EslContextGlobal
}


namespace EsbStudio.Esl
{
	using static EsbStudio.Esl.EslContextApi;
	using static EsbStudio.Esl.EslContextApi2;

	namespace EsbProjectModules 
	{
		internal class AppModule    //OrdinaryApplicationModule.bsl, считаем глобальным, хотя он некоторый спец.модуль
		{
			internal static string	NameTextEn_ = "OrdinaryApplicationModule";
			internal static string	NameCodeEn_ = "ORDINARYAPPLICATIONMODULE";
			internal static EslName NameEn_		= new EslName(NameTextEn_, NameCodeEn_);
			internal static string	NameTextRu_ = "МодульОбычногоПриложения";
			internal static string	NameCodeRu_ = "МОДУЛЬОБЫЧНОГОПРИЛОЖЕНИЯ";
			internal static EslName NameRu_		= new EslName(NameTextRu_, NameCodeRu_);
			//
			//Процедура ПриНачалеРаботыСистемы()
			//Процедура ПриЗавершенииРаботыСистемы ()

			internal static EslDeclModule CreateInstance() 
			{
				EslDecl[] my_declarations_all = new EslDecl[]
				{
					VARI("__EsbComponent__", EslTypes.EsbComponent_),
					VARI("__EsbComponentObject__", EslTypes.EsbObject_),
					PROC("ПриЗавершенииРаботыСистемы"),
					PROC("ПриНачалеРаботыСистемы"),
				};
				EslDecl[] my_declarations_exp = new EslDecl[]
				{
					my_declarations_all[0],
					my_declarations_all[1],
				};

				return new EslDeclModule(NameTextEn_, NameCodeEn_, true, my_declarations_all, my_declarations_exp);  // true - глобальный
			}
		} // class EsbManager

		internal class EsbManager
		{
			internal static string	NameTextEn_	= "EsbManager";
			internal static string	NameCodeEn_	= "ESBMANAGER";
			internal static EslName NameEn_		= new EslName(NameTextEn_, NameCodeEn_);
			internal static string	NameTextRu_	= "ЕсбМенеджер";
			internal static string	NameCodeRu_	= "ЕСБМЕНЕДЖЕР";
			internal static EslName NameRu_		= new EslName(NameTextRu_, NameCodeRu_);		
			//
			internal static EslDeclModule CreateInstance() 
			{
				EslDecl[] my_declarations_all_exp = new EslDecl[]
				{
					//TODO	вот здесь параметры out (именно out в смысле c#, могут быть любым на входе, а на выходе обязательно им присвоить значение (как return-value))
					FUNC("ЗагрузитьЕсбКомпоненту", EslTypes.Boolean_, PARA("<out_EsbComponent__>", EslTypes.String_), PARA("<out_EsbComponentObject__>", EslTypes.EsbObject_) ),
					//TODO	вот здесь параметры in_out или ref 
					//TODO	ообще с параметрами разбираться надо! const, in, out, optional, default-value, paramarray в том числе и ограниченный по кол-ву paramarray(10) н.р...
					FUNC("ВыгрузитьЕсбКомпоненту", EslTypes.Boolean_, PARA("<in_out_EsbComponent_>", EslTypes.String_), PARA("<in_out_EsbComponentObject_>", EslTypes.EsbObject_) ),
				};

				return new EslDeclModule(NameTextEn_, NameCodeEn_, false, my_declarations_all_exp, my_declarations_all_exp);  // true - глобальный
			}
		} // class EsbManager

		
		//TODO	Ну вот херня какая-то с именами.. 1С-модуль EsbComponent инкапсулирует EsbObject_
		//		а у нас еще собственно EsbComponent есть который собственно компонент..
		//		может EsbObject_ это все таки EsbAddin ну или EsbAddinObject
		//		Ну а с третьей стороны имя то ему юзер в с++ задает и оно всяким может быть. для нас важно чтобы оно единственно было и мы знали где его взять
		internal class EsbComponent
		{
			internal static string ModuleNameEn_ = "EsbComponent";
			internal static string ModuleCodeEn_ = "ESBCOMPONENT";
			//
			private static EslDecl[] my_declarations_all_exp = new EslDecl[]
			{
					PROC("Выгрузить"),
					FUNC("Загрузить", EslTypes.Boolean_, PARA("<путь_к_файлу_компоненты_>", EslTypes.String_) ),
					FUNC("ИмяФайлаКомпоненты", EslTypes.String_),
					FUNC("СтартТест", EslTypes.Boolean_),
					FUNC("ЭтотКомпонент", EslTypes.EsbObject_),
			};
			//
			internal static EslDeclModule Instance_ = new EslDeclModule(ModuleNameEn_, ModuleCodeEn_, false, my_declarations_all_exp, my_declarations_all_exp);
		}// class EsbComponent


		internal class EsbCommon
		{
			internal static string ModuleNameEn_ = "EsbCommon";
			internal static string ModuleCodeEn_ = "ESBCOMMON";
			//
			internal static EslDeclModule CreateInstance() 
			{
				//TODO	Видимо у глобального контекста структура TYPES должна быть публичной. на то он и глобальный.
				//		иначи мы должны искать свои импортируемые типы по тексту имени.
				EslTypeEnumType MY_IMPORT_PlatformType_ = EslTypes.GetTypeByName(EslContextGlobal.Enums.PlatformType.EnumTypeName_) as EslTypeEnumType;
				Debug.Assert(MY_IMPORT_PlatformType_ != null);
				if (MY_IMPORT_PlatformType_ == null)
					return null;

				EslDecl[] my_declarations_all_exp = new EslDecl[]
				{
					FUNC("ВерсияБольше", EslTypes.Boolean_, PARA("<строка_версия_1_>", EslTypes.String_), PARA("<строка_версия_2_>", EslTypes.String_) ),
					FUNC("ВерсияМеньше", EslTypes.Boolean_, PARA("<строка_версия_1_>", EslTypes.String_), PARA("<строка_версия_2_>", EslTypes.String_) ),

					FUNC("ЕсбПутьДляОтладки", EslTypes.String_, PARA("<использовать_дебаг_длл_>", EslTypes.Boolean_)),
					FUNC("ЕсбПутьДляОтладкиПостроить", EslTypes.String_,
							PARA("<путь_к_проекту_>", EslTypes.String_), PARA("<версия_платформы_>", EslTypes.String_), PARA("<тип_платформы_>", MY_IMPORT_PlatformType_),
							PARA("<использовать_дебаг_длл_>", EslTypes.Boolean_) ),
					FUNC("ЕсбСовместимаяПлатформа", EslTypes.Boolean_ ),

					FUNC("ПодключитьВнешнююКомпонентуЕсб", EslTypes.Boolean_, PARA("<файл_компоненты_>", EslTypes.String_) ),
					FUNC("ПодключитьВнешнююКомпонентуНаитивноНеИзолированно", EslTypes.Boolean_, PARA("<файл_компоненты_>", EslTypes.String_), PARA("<имя_компоненты_>", EslTypes.String_) ),

					FUNC("ПутьБезПоследнегоРазделителя", EslTypes.String_, PARA("<путь_>", EslTypes.String_)),
					FUNC("ПутьДостроить", EslTypes.String_, PARA("<путь_>", EslTypes.String_), PARA("<добавить_>", EslTypes.String_) ),
					FUNC("ПутьНаУровеньВыше", EslTypes.String_, PARA("<путь_без_последнего_разделителя_>", EslTypes.String_)),
					FUNC("ПутьККонфигурации", EslTypes.String_),

					FUNC("СравнитьВерсию", EslTypes.Number_, PARA("<строка_версия_1_>", EslTypes.String_), PARA("<строка_версия_2_>", EslTypes.String_) ),

					FUNC("Типа", EslTypes.TypeType_, PARA("<ИмяТипа>", EslTypes.String_)),
				};

				return new EslDeclModule(ModuleNameEn_, ModuleCodeEn_, true, my_declarations_all_exp, my_declarations_all_exp);  // true - глобальный
			}
		} // class EsbCommon
	} // namespace EsbProgectPodules
} // namespace EsbStudio.Esl
