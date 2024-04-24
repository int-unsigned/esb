using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
#if DEBUG || DEBUG_IN_RELEASE
	using System.Diagnostics;
#endif

//
using Microsoft.VisualStudio.Package;
using System.Collections.Specialized;
//
using System.Runtime.CompilerServices; // Для

// Наша EsbStudio преимущественно синхронна и асинхронную функциональность НЕ ИСПОЛЬЗУЕТ
// Принципиально. Принципиально считаю, что помогать кодеру нужно синхронно, а ждут пусть другие
// Возможно когда-то эта модель будет изменена - тогда нужно будет разобираться в местах помеченных этим макросом
// (VSTHRD010 - это ругань по поводу возможного обращения к EnvDTE-элементам из НЕ основного потока
//	с тревованием Microsoft.VisualStudio.Shell.ThreadHelper.ThrowIfNotOnUIThread(); или еще как)
#if ESB_STUDIO_SYNC_MODE
#pragma warning disable VSTHRD010
#endif


//using static EsbStudio.Esl.EslContextApi;


/*
// Нас просят найти МетодИнфо для какого-то имени в источнике_ в позиции контекст_
// 0. Это имя может быть локальным в функции (или вообще вне функции). Для этого нужен полный парсинг источника - пока не поддерживаем - TODO
// 1. Это имя пожет быть определено в ЭтомМодуле (в источнике). Для этого тоже нужен парсинг декларций (или возможно отслеживание кейвордов Перем, Функция, Процедура)
//		- пока мы наперед знаем, что в модуле OrdinaryApplicationModule.bsl есть
			Перем __EsbComponent__ экспорт;
			Перем __EsbComponentObject__ экспорт;
			Процедура ПриНачалеРаботыСистемы()
			Процедура ПриЗавершенииРаботыСистемы ()
		- в модуле EsbCommon (глобальный)
			Функция Типа(ИмяТипа) Экспорт
			Функция СравнитьВерсию(строка_версия_1_, строка_версия_2_) Экспорт
			Функция ВерсияМеньше(строка_версия_1_, строка_версия_2_) Экспорт
			Функция ВерсияБольше(строка_версия_1_, строка_версия_2_) Экспорт
			Функция ПутьБезПоследнегоРазделителя(путь_) Экспорт
			Функция ПутьККонфигурации() Экспорт
			Функция ПутьНаУровеньВыше(путь_без_последнего_разделителя_) Экспорт
			Функция ПутьДостроить(путь_, добавить_) Экспорт
			Функция ЕсбПутьДляОтладкиПостроить(путь_к_проекту_, версия_платформы_, тип_платформы_, использовать_дебаг_длл_) Экспорт
			Функция ЕсбПутьДляОтладки(использовать_дебаг_длл_) Экспорт
			Функция ПодключитьВнешнююКомпонентуНаитивноНеИзолированно(файл_компоненты_, имя_компоненты_) Экспорт
			Функция ПодключитьВнешнююКомпонентуЕсб(файл_компоненты_) Экспорт
			Функция ЕсбСовместимаяПлатформа() Экспорт
		- в модуле EsbManager (обычный)
			Функция Загрузить(файл_компоненты_, out_EsbComponent__, out_EsbComponentObject__) Экспорт
			Процедура Выгрузить(in_out_EsbComponent_, in_out_EsbComponentObject_) Экспорт
			(TODO разумнее переименовать ЗагрузитьЕсбКомпоненту, ВыгрузитьЕсбКомпоненту а то похоже что сам менеджер Загрузить/Выгрузить)
		- в модуле EsbComponent (обычный)
			Функция ИмяФайлаКомпоненты() Экспорт
			Функция Загрузить(путь_к_файлу_компоненты_) Экспорт
			Процедура Выгрузить() Экспорт
			Функция ЭтотКомпонент() экспорт
		-  в "фиктивном" модуле н.р. Как... (см.ниже)
	2. Это имя может быть определено в контекстах ГЛОБАЛЬНЫХ МОДУЛЕЙ проекта как ЭКСПОРТ (или один сводный контекст для них сделать 1С-Common)
		- см.выше. у нас пока только один глобальный модуль EsbCommon
		+ !!! это имя может быть просто именем одного из НЕ-ГЛОБАЛЬНЫХ модулей проекта (пока это EsbManager и EsbComponent)
		+ т.е. у этого самого 1С-Common или лучше ProjectContext есть Методы (сводный список один на всех) и Свойства - имена ОБЫЧНЫХ Модулей
		+ и это именно модель 1С, т.к. в модулях 1С нельзя делать экспортируемые переменные. поэтому ProjectContext.Property всегда ИмяМодуля
		- ТАКЖЕ это имя может быть определено в неком "фиктивном" модуле - н.р. гипотетический модуль Как - 
			- Функция Как.Строка(Значение_ Как Любое)
			- т.п.
	3. Это имя может быть определено в Глобальном Контексте 1С (GlobalContext, V8Context, PlatformContext ?)
		* Их там множество
		* Плюс нужно делать контекст в зависимости от версии Есб-Проекта (и проч. возможных настроек проекта)
		  пока ограничимся
		- Функция СтрДлина(<Строка> Как Строка) Как Число
			Варианты метода (просто, только один)
			- СтрДлина(<Строка> Как Строка) 
		- Сообщить(<ТекстСообщения> Как Строка, <Статус> Как СтатусСообщения = СтатусСообщения.Обычное) 
			Варианты метода:
			- Процедура Сообщить(<ТекстСообщения> Как Строка) 
			- Процедура Сообщить(<ТекстСообщения> Как Строка, <Статус> Как СтатусСообщения) 


	Конструкторы типов после Новый
	Фактически пробел после кейворда Новый работает как '.' после имени
		- Массив - это НЕ ФУНКЦИЯ, А КОНСТРУКТОР ТИПА выглядит аналогично функции !!! И показывать это НУЖНО только после НОВЫЙ !!!
					не GetNameMethods а GetCtorMethods - TODO
		  Массив([ <Опущено> | <КоличествоЭлементов1>,...,<КоличествоЭлементовN> | <ФиксированныйМассив> ] Как Массив
			Варианты:
				- Массив() Как Массив
				- Массив(<ФиксированныйМассив> Как ФиксированныйМассив) Как Массив
				- Массив(<КоличествоЭлементов1> Как Число,...,<КоличествоЭлементовN> Как Число) Как Массив
				! НО НЕ ИМЕЯ ТИПОВ мы можем показать вариант с одним параметром только как
				- Массив([ <<ФиксированныйМассив> Как ФиксированныйМассив> | <<КоличествоЭлементов1> Как Число> ]) Как Массив
	

	ТАКЖЕ. Можно учесть, что для запроса GetNameMethods можно не учитывать Свойства, или точнее Имена у которых НЕТ МЕТОДОВ - н.р. ИменаМодулейПроекта или Перем
			или для ГлобальногоКонтекста не просматривать Енумераторы - выше <Статус> - это Енумератор СтатусСообщения

	
	ВСЕ эти имена используются двояко - запросы МетодИнфо, Аутос (ctrl-K,I), Мышь - работают с полным именем и им лучше HashSet - О(1)
	А запрос Дополнить должен показать сортированный список похожего и ему лучше сортированный Массив (или Мап) - О(логарифм)
	ПОЭТОМУ лучше собрать их ВСЕ в один Мап, который покроет все требования, хотя, возможно не самым эффективным образом в отдельных случаях
	(простор для оптимизаций)
	МАП, а не Массив, т.к. Имена (Декларации) могут добавляться и удаляться для модулей проекта.
	ЛУЧШЕ СРАЗУ сделать КонтекстЮнион, т.к. кроме прочего имена еще должны искаться в определенном порядке - сначала локальные, потом..., а ГлобальныйКонтекст в конце
	ТОГДА и организация каждого контекста может быть уникальной - у Глобального можно СортМассив с HashSet - он не меняется, у остальных - Мап

	
*/




namespace EsbStudio.Esl
{
	using static EslContextApi;


	internal class EslEngine 
	{

		//TODO
		static EslDeclModule[] ProjectModulesGlobal_;
		// Common - это не Объектный!, Common - это именно общий, но не глобальный!. Объектный - это у форм и т.п.
		static EslDeclModule[] ProjectModulesCommon_;
		//TODO В какой-то момент нужен будет HashSet

		//TODO	Мы сейчас для простоты тупо поднимаем EslEngine статически.
		//		Далее нужно сделать создание экземпляра EslEngine для конкретной версии 1С из настроек поекта (или по-умолчанию для EslLanguageService)
		//		(если .bsl фал был загружен просто так, без есб-проекта)
		//		И также инициализация ее списком модулей этого проекта для которого нас подняли(EsbProjectModules...)
		//		Этот экземпляр EslEngine должен храниться где-то в EslLanguageService и даватся параметром каждому создаваемому EslSource
		//		(т.к. парсинг так или иначе танцуется для EslSource)
		static EslEngine() 
		{
			EslContextGlobal.Initialize();
			ProjectModulesGlobal_ = new EslDeclModule[] { EsbProjectModules.AppModule.CreateInstance(), EsbProjectModules.EsbCommon.CreateInstance() };
			ProjectModulesCommon_ = new EslDeclModule[] { EsbProjectModules.EsbComponent.Instance_, EsbProjectModules.EsbManager.CreateInstance() };
		}
		////////////////////////////


		static EslDeclModule GetProjectModuleCommon(EslName module_name_) {
			foreach (EslDeclModule module in ProjectModulesCommon_) {
				if (module.GetModuleName().IsEqual(module_name_))
					return module;
			}
			return null;
		}
		static EslDeclModule GetProjectModuleGlobal(EslName module_name_) {
			foreach (EslDeclModule module in ProjectModulesGlobal_){
				if (module.GetModuleName().IsEqual(module_name_))
					return module;
			}
			return null;
		}
		static EslDeclModule GetProjectModule(EslName module_name_) {
			EslDeclModule m = GetProjectModuleCommon(module_name_);
			if (m != null)
				return m;
			return GetProjectModuleGlobal(module_name_);
		}
		static EslDeclModule GetProjectModuleFromSource(EslSource source_) {
			//TODO	Нужно как-то или модуль с соурсом ассоциировать - то ли модуль в соурс прописать, то ли соурс в модуль
			//		искомый модуль может быть только среди объектных, а проверяем по строгому совпадению имени - ? Почему? Нью не хочется. Нужно в соурс засунуть!
			string esl_module_name_text = source_.GetEslModuleName();
			foreach (EslDeclModule module in ProjectModulesCommon_) {
				if (module.GetModuleName().GetNameText() == esl_module_name_text)
					return module;
			}
			return null;
		}


		static EslDecl[] GetModuleEsports(EslDecl module_declaration_) {
			Debug.Assert(module_declaration_.IsModuleDeclaration());
			return ((EslDeclModule)module_declaration_).GetModuleDeclarationsExport();	//вот же статик каста то не хватает..
		}

		// В GetNameDeclaration вызывается для каждого имени после первого т.е. имеется цель раскрутить <>.Foo().Bar.Baz...
		// В нашем языке после первого элемента могут следовать тоолько какие-то методы (свойства) имеющие (не-имеющие) тип
		// и нам надо отдать интерфейс (декларации методов) этого возвращаемого типа
		static EslDecl[] GetContextDeclarationsFirst(EslDecl declaration_) 
		{
			// Декларация модуля может быть первой в списке. Вернем его декларации
			if (declaration_.IsModuleDeclaration())
				return GetModuleEsports(declaration_);
			//TODO Вот здесь, если у наших типов появятся статические методы, мы должны вернуть их список
			//		т.е. конструкция вида System.String.<...>
			if (declaration_.IsTypeDeclaration())
				return null;

			return GetContextMethodReturnTypeDeclarationsOrNull(declaration_);
		}

		static EslDecl[] GetContextDeclarationsNext(EslDecl declaration_) 
		{
			// Декларация модуля может быть только первой в списке деклараций. если мы ее встретили следующей, то что-то не так. вернем нулл
			if (declaration_.IsModuleDeclaration())
				return null;
			// если по какой-то неведомой причине мы встретили декларацию типа где-либо кроме первой в имени
			// то возвращаем нулл - нет у нас такой конструкции в языке (пока)
			//TODO Вот здесь, если у наших типов появятся статические методы, мы должны вернуть их список
			//		т.е. конструкция вида System.String.<...>
			if (declaration_.IsTypeDeclaration())       //D+M
				return null;

			return GetContextMethodReturnTypeDeclarationsOrNull(declaration_);
		}

		// В GetNameMembers мы вызываемся ПОСЛЕ GetNameDeclaration для получения списка мемберов ('.') этой декларации
		// соответственно элемент declaration_ у нас может быть либо именем модуля для которого мы должны вернуть его экспорты
		// либо чем-то имеющием ретурн-тип-данных
		internal static EslDecl[] GetContextDeclarationMembers(EslDecl declaration_) 
		{
			if (declaration_.IsModuleDeclaration())
				return GetModuleEsports(declaration_);
			// если по какой-то неведомой причине мы встретили декларацию типа где-либо кроме первой в имени
			// то возвращаем нулл - нет у нас такой конструкции в языке (пока)
			//TODO Вот здесь, если у наших типов появятся статические методы, мы должны вернуть их список
			//		т.е. конструкция вида System.String.<...>
			if (declaration_.IsTypeDeclaration())       //D+M
				return null;

			return GetContextMethodReturnTypeDeclarationsOrNull(declaration_);
		}
		internal static EslDecl[] GetContextMethodReturnTypeDeclarationsOrNull(EslDecl declaration_) 
		{
			EslType data_type_may_null = EslContextApi.GetDeclarationMethodsReturnDataType(declaration_);
			if (data_type_may_null == null)
				return null;
			if (data_type_may_null == EslTypes.Void_)
				return null;

			return EslTypes.GetObjectContextNotNull(data_type_may_null);
		}

		// С точки зрения Интеллисенс его не интересует есть ли у кодовой конструкции представленной строкой expr_name_
		// какой бы то ни было тип с точки зрения языка. Ему главное можно после '.' вывалить список или нет.
		// Например бывают namespace, которые не Типы, но список (контекст) имеют
		// или мы вообще можем сделать какую-нибудь <Глобалс>. после которого покажем список.
		// (а после выбора элемента еще получившееся выражение <Глобалс>.Элемент заменим на конструкцию кода)
		// т.е. Контекст для Интеллисенс есть некоторое НАД-Множество системы типов языка существующее в процессе редактирования
		internal static EslDecl TryGetDeclarationFromExpr(EslSource source_, int context_line_, int context_col_, string expr_name_) 
		{
			EslDeclModule this_module = GetProjectModuleFromSource(source_);
			if (this_module == null) {
				// это допустимо - загрузили какой-то иной файл? TODO?
			}

			EslDecl declaration = null;
			EslName declaration_name = new EslName(expr_name_);

			//1. проверка имени именно в этом модуле (и внутренние имена тоже)			
			if (this_module != null) {
				if (TryGetDeclarationInModuleInternal(this_module, source_, context_line_, context_col_, declaration_name, ref declaration))
					return declaration;
				// Обращение по имени модуля внутри самого модуля - это тот же экспорт-контекст
				if (!this_module.IsGlobalModule() && this_module.GetModuleName().IsEqual(declaration_name))
					return this_module.GetModuleDeclaration();
			}

			//2. Проверка имени во всех модулях, а для Объектных модулей проверка на имя модуля как имя контекста
			//TODO	Имеет смысл сделать один Modules контекст, а не рыться в цикле - ХА! тогда придется придумать как исключить this_module !
			//2.1	Проверить все Is-GlobalModule на предмет экспорта expr_code (исключая this_module если он есть и если он Is-GlobalModule)
			if (this_module != null && this_module.IsGlobalModule()) { 
				foreach (EslDeclModule module in ProjectModulesGlobal_) {
					if (!Object.ReferenceEquals(module, this_module) && TryGetDeclarationInModuleExports(module, declaration_name, ref declaration))
						return declaration;
				}
			}
			else { 
				foreach (EslDeclModule module in ProjectModulesGlobal_) {
					if (TryGetDeclarationInModuleExports(module, declaration_name, ref declaration))
						return declaration;
				}
			}
			//2.2	Проверить все НЕ-GlobalModule на предмет что expr_code это имя expr_esl_name (исключая this_module если он есть и он НЕ-GlobalModule)
			EslDeclModule module_from_code = GetProjectModuleCommon(declaration_name);
			if (module_from_code != null && !Object.ReferenceEquals(module_from_code, this_module)) //ReferenceEquals возвращает true только для двух нулл, а у нас module_from_code НЕ тулл
				return module_from_code.GetModuleDeclaration();	// возвращаем декларацию модуля, если не вышли на себя

			//3. Если ниче не нашли - то Глобальный контекст
			return EslContextGlobal.FindDeclaration(declaration_name);
		}
		internal static bool TryGetDeclarationInModuleInternal(EslDeclModule module_, EslSource source_, int context_line_, int context_col_, EslName declaration_name_,
																																		ref EslDecl out_declaration_) {
			out_declaration_ = module_.GetModuleDeclarationAny(declaration_name_);
			return (out_declaration_ != null);
		}
		internal static bool TryGetDeclarationInModuleExports(EslDeclModule module_, EslName declaration_name_, ref EslDecl out_declaration_) {
			// Мы игнорируем всяческие параметры и вид вызова (функция, процедура, свойство) - нам главное что имя нашлось 
			// и если нашлось то контекст наш (даже если пустой или вообще непременимо)
			//TODO Это не совсем верно, т.к. некоторые функции могут иметь разный возвращаемый тип в зависимости от параметров. на будущее.
			out_declaration_ = module_.GetModuleDeclarationExport(declaration_name_);
			return (out_declaration_ != null);
		}



		//
		internal static HashSet<EslDecl> GetMatchDeclarations(EslSource source_, int context_line_, int context_col_, string name_part_) 
		{			
			string				code_part = MakeNameCode(name_part_);
			HashSet<EslDecl>	match_declarations = new HashSet<EslDecl>(EslDeclComparierEq.Instance_);
			EslDeclModule		this_module = GetProjectModuleFromSource(source_);

			if (this_module != null)
				AddMatchDeclarationsInternal(match_declarations, this_module, source_, context_line_, context_col_, code_part);

			AddMatchDeclarationsModules(match_declarations, this_module, code_part);
			AddMatchDeclarationsGlobal(match_declarations, code_part);

			return match_declarations;
		}
		internal static void AddMatchDeclarationsInternal(HashSet<EslDecl> match_declarations_, EslDeclModule module_, 
																				EslSource source_, int context_line_, int context_col_, string declaration_code_part_) {
			AddMatchDeclarations(match_declarations_, module_.GetModuleDeclarationsAll(), declaration_code_part_);
			if (!module_.IsGlobalModule())
				AddMatchDeclarationsModuleSelf(match_declarations_, module_, declaration_code_part_);
		}

		internal static void AddMatchDeclarationsProjectModulesGlobalExclude(HashSet<EslDecl> match_declarations_, EslDeclModule exclude_module_, string code_part_) {
			foreach (EslDeclModule module in ProjectModulesGlobal_)	{
				if (Object.ReferenceEquals(module, exclude_module_))
					continue;
				AddMatchDeclarationsExports(match_declarations_, module, code_part_);
			}
		}
		internal static void AddMatchDeclarationsProjectModulesGlobalAll(HashSet<EslDecl> match_declarations_, string code_part_) {
			foreach (EslDeclModule module in ProjectModulesGlobal_)	{
				AddMatchDeclarationsExports(match_declarations_, module, code_part_);
			}
		}

		internal static void AddMatchDeclarationsProjectObjectModulesExclude(HashSet<EslDecl> match_declarations_, EslDeclModule exclude_module_, string code_part_) {
			foreach (EslDeclModule module in ProjectModulesCommon_)	{
				if (Object.ReferenceEquals(module, exclude_module_))
					continue;
				AddMatchDeclarationsModuleSelf(match_declarations_, module, code_part_);
			}
		}
		internal static void AddMatchDeclarationsProjectObjectModulesAll(HashSet<EslDecl> match_declarations_, string code_part_) {
			foreach (EslDeclModule module in ProjectModulesCommon_)	{
				AddMatchDeclarationsModuleSelf(match_declarations_, module, code_part_);
			}
		}

		internal static void AddMatchDeclarationsModules(HashSet<EslDecl> match_declarations_, EslDeclModule exclude_module_, string code_part_) 
		{
			// если exclude_module_ это ДА-IsGlobalModule (ProjectModulesGlobal_), то его экспорты добавлять не надо - они уже должно быть добавлено в Internals
			if (exclude_module_ != null && exclude_module_.IsGlobalModule())
				AddMatchDeclarationsProjectModulesGlobalExclude(match_declarations_, exclude_module_, code_part_);
			else
				AddMatchDeclarationsProjectModulesGlobalAll(match_declarations_, code_part_);

			// если exclude_module_ это НЕ IsGlobalModule (ProjectObjectModules_), то его имя добавлять не надо - оно уже должно быть добавлено в Internals
			if (exclude_module_ != null && !exclude_module_.IsGlobalModule())
				AddMatchDeclarationsProjectObjectModulesExclude(match_declarations_, exclude_module_, code_part_);
			else
				AddMatchDeclarationsProjectObjectModulesAll(match_declarations_, code_part_);
		}

		internal static void AddMatchDeclarationsModuleSelf(HashSet<EslDecl> match_declarations_, EslDeclModule module_, string declaration_code_part_) {
			EslDecl module_declaration = module_.GetModuleDeclaration();
			if (IsMatchDeclaration(module_declaration, declaration_code_part_))
				match_declarations_.Add(module_declaration);
		}
		internal static void AddMatchDeclarationsExports(HashSet<EslDecl> match_declarations_, EslDeclModule module_, string declaration_code_part_) {
			AddMatchDeclarations(match_declarations_, module_.GetModuleDeclarationsExport(), declaration_code_part_);
		}
		internal static void AddMatchDeclarationsGlobal(HashSet<EslDecl> match_declarations_, string declaration_code_part_) {
			AddMatchDeclarations(match_declarations_, EslContextGlobal.GetModuleDeclarationsExport(), declaration_code_part_);
			//TODO Имена классов мы добавляем невзирая где нас попросили (было Новый или нет) нам сюда от парсера контекст не приходит. Доработать!
			//		Отдельный метод должен быть для .CompleteWordAfterNew.
			//		(и там мы НЕ-должны учитывать наличие конструктора - главное имя. если юзер его выбрал расскажем что к чему)
			AddMatchDeclarations(match_declarations_, EslContextGlobal.GetModuleClassesExport(), declaration_code_part_);
		}
		//TODO	Простецкий алгоритм. Может быть как-то улучшен, быть разным для разный деклараций и т.п.
		internal static void AddMatchDeclarations(HashSet<EslDecl> match_declarations_, EslDecl[] declarations_, string declaration_code_part_) {
			foreach (EslDecl declaration in declarations_) {
				if (IsMatchDeclaration(declaration, declaration_code_part_))
				match_declarations_.Add(declaration);
			}
		}
		internal static bool IsMatchDeclaration(EslDecl declaration_, string declaration_code_part_) {
			return declaration_.GetName().GetNameCode().Contains(declaration_code_part_);
		}
		//----


		internal static EslDecl GetNameDeclaration(EslSource source_, int context_line_, int context_col_, StringCollection names_, int use_names_count_) {
			Debug.Assert(names_ != null && names_.Count >= 1);
			// Первый элемент в коллекции names_ - это "контекст" - т.е. список деклараций.
			// каждый последующий элемент в коллекции names_ применяется к списку деклараций и получется опять список деклараций

			EslDecl root_declaration = TryGetDeclarationFromExpr(source_, context_line_, context_col_, names_[0]);
			if (root_declaration == null)
				return null;
			if (use_names_count_ == 1)
				return root_declaration;

			// Если имен больше одного будем крутить цикл для каждого имени отталкиваясь от предыдущего контекста пока либо в нулл не упремся либо имена не закончатся
			EslDecl	name_declaration = root_declaration;
			for (int i_name = 1; i_name < use_names_count_; ++i_name)
			{
				EslDecl[] name_declaration_declarations = GetContextDeclarationsFirst(name_declaration);
				if (name_declaration_declarations == null)
					return null;

				name_declaration = EslContextApi.FindDeclarationByName(name_declaration_declarations, names_[i_name]);
				if (name_declaration == null)
					return null;
			}

			return name_declaration;
		}




		// Этот метод вызывается для показа списка методов декларации, поэтому мы "раскручиваем" все имена в декларации до конца
		// (Показ списка членов после '.')
		internal static EslDecl[] GetNameMembers(EslSource source_, int context_line_, int context_col_, StringCollection names_, int use_names_count_) 
		{
			EslDecl name_declaration = GetNameDeclaration(source_, context_line_, context_col_, names_, use_names_count_);
			if (name_declaration == null)
				return null;
			
			return GetContextDeclarationMembers(name_declaration);	// там сами разберутся какого типа декларация и что возвращать. если что null вернут.			
		}


		// Этот метод вызывается для MethodTip для показа параметров - foo(<param>)
		// names_ - должна быть полностью разрешаемое в декларацию до последнего элемента,
		// и для декларации этого последнего элемента мы возвращаем список его методов
		internal static EslMeth[] GetNameMethods(EslSource source_, int context_line_, int context_col_, StringCollection names_) 
		{
			Debug.Assert(names_ != null && names_.Count >= 1);

			EslDecl name_declaration = GetNameDeclaration(source_, context_line_, context_col_, names_, names_.Count);
			if (name_declaration != null)
				return name_declaration.GetDeclarationMethods();	// У декларации типа модуль (неймспейс) методы null. само собой никаких параметров.
																	// А если наткнемся по имени на декларацию типа - получим его конструктор

			return null;
		}

	} // class EslEngine

} // namespace EsbStudio.Esl
