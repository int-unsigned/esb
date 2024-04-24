using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
#if DEBUG || DEBUG_IN_RELEASE
	using System.Diagnostics;
#endif


// Наша EsbStudio преимущественно синхронна и асинхронную функциональность НЕ ИСПОЛЬЗУЕТ
// Принципиально. Принципиально считаю, что помогать кодеру нужно синхронно, а ждут пусть другие
// Возможно когда-то эта модель будет изменена - тогда нужно будет разобираться в местах помеченных этим макросом
// (VSTHRD010 - это ругань по поводу возможного обращения к EnvDTE-элементам из НЕ основного потока
//	с тревованием Microsoft.VisualStudio.Shell.ThreadHelper.ThrowIfNotOnUIThread(); или еще как)
#if ESB_STUDIO_SYNC_MODE
#pragma warning disable VSTHRD010
#endif



namespace EsbStudio.Esl
{
	internal class EslTypes {
		// войд всегда должен быть "special handled" поэтому имена ему даем нулл, чтобы вылететь если что.
		private class _EslTypeVoid : EslType { public _EslTypeVoid() : base() { } }
		static internal EslType Void_ = new _EslTypeVoid();
		// Это и не Войд, и не Неопределено и не Любое (которые нормальные типы) - парсер не вывел... Можно считать или Любое или Ошибка в зависимости от предпочтений
		static internal EslType Unknown_ = new EslType("Неизвестное");
		static internal EslType Undefined_ = new EslType("Неопределено");
		static internal EslType Any_ = new EslType("Любое");
		static internal EslType Boolean_ = new EslType("Булево");
		static internal EslType Number_ = new EslType("Число");
		static internal EslType DateTime_ = new EslType("ДатаВремя");
		static internal EslType String_ = new EslType("Строка");
		static internal EslType TypeType_ = new EslType("Тип");

		//TODO	Перекрестные ссылки при инициализации - РАЗРУЛИТЬ!!!!
		//ВСЕ ПРИМИТИВНЫЕ ТИПЫ должны быть проинициализированы ПЕРЕД ЛЮБЫМИ сложными, которые могут ссылаться на ДРУГИЕ ТИПЫ !!!


		//TODO	в Esb UndefOrNumeric вообще-то, но везде общепринято для nullable типов сам тип ставить в конце - Number?
		//		нужно как-то урегулировать до единообразия
		//		Также текст (имя) типа - Число?, ЧислоИлиНеопределено, [Число,Неопределено], [Число|Неопределено], ...
		//		(с учетом что еще появятся всякие [Число|Строка|Картинка]. через запятую читабельнее вообще-то..)
		//		Нужно разделить понятия ИмяТипа и ТекстТипа (дисплей текст)
		static internal EslType NumberOrUndef_ = new EslType("[Число,Неопределено]", Number_, Undefined_);

		//TODO EslContext_EsbObject у нас в системе типов не зарегестрирован (и имя у него может быть разное в зависимости от проекта)
		static internal EslType EsbObject_ = new EslType(EsbProject.EsbObject.GetNameEn(), EslType.InterfaceIncomplete.Tag_);
		// у самой есб-компоненты есть свой тип и интерфейс, гхм.. она класс тоже из с++ 
		static internal EslType EsbComponent_ = new EslType(EsbProject.EsbComponent.EsbComponentNameEn_, EslType.InterfaceIncomplete.Tag_);
		
		//TODO	Тупо как-то из-а их тупого хэша массив типов делать. Вообще-то массив типов мне нужен потом будет для Новый... Но всех сразу.. потом.
		static HashSet<EslType> TypesSet_ = new HashSet<EslType>(
			new EslType[]{ Void_, Unknown_, Undefined_, Any_, Boolean_, Number_, DateTime_, String_, TypeType_, NumberOrUndef_, EsbObject_, EsbComponent_ }, 
					EslTypeComparierEq.Instance_ );

		//1. Нам нужны ВСЕ типы для строгого поиска по имени для парсера (пока только из есб-модуля надо)
		//		ТАКЖЕ при этом должны быть созданы какие-то Alais для Есб, т.к. в с++ некоторые имена отличаются
		//2. Нам нужны все типы с конструкторами в сортированном массиве для показа списка после Новый
		//		(грубо говоря можно считать что после Новый стоит '.', а не ' ' и у этого Новый. есть контекст из типов)
		//3. При создании типов с контекстами (и/или конструкторами) их сначала надо создавать ПРЕДВАРИТЕЛЬНО БЕЗ КОНТЕКСТОВ
		//		только чтобы объект был, и только когда все созданы каждому присвоить свой контекст
		//		А иначи крах при перекрестных связях типа как у Массив есть конструктор от ФиксированныйМассив,
		//		А у самого ФиксированныйМассив есть конструктор от Массив.

		// регистрация класса сразу и целиком
		internal static EslTypeClass RegisterTypeClass(EslName name_, EslDecl[] interface_, EslMeth[] constructors_) 
		{
			//TODO Вот вообще-то все эти Register.. используются только (пока) при первоначальной загрузке, когда 
			//		а) искать ничего не надо б) и регистрировать все можно потом гамузом
			//		НУЖЕН EslTypesRegistrar - типа некоторый пакет, который изначально пуст, его там заполняют, и потом мы его себе гамузом вливаем
			//		ПЕРЕПИСАТЬ ВСЕ НАХРЕН!
			EslType t_found = EslContextApi.FindTypeByName(TypesSet_, name_);
			if (t_found != null)
				return t_found as EslTypeClass; // если мы нашли тип по имени, то возвращаем его только если он класс. иначе нулл - что означает регистрация неуспешна

			EslTypeClass t_new = new EslTypeClass(name_, interface_, constructors_);
			bool b_ok = TypesSet_.Add(t_new);
			Debug.Assert(b_ok);
			return (b_ok)? t_new : null;
		}

		// регистрация только Incomplete класса, фактически имени. требует потом .SetDefinition (или не требует - так и останется Incomplete, бывает..)
		internal static EslTypeClass RegisterTypeClassName(EslName name_) 
		{
			//TODO Вот опять эта реализация хэшсета не дает работать с букетами напрямую. поэтому поиск будет делаться два раза - раз чтобы проверить, два чтобы вставить.
			//		ПЕРЕПИСАТЬ ВСЕ НАХРЕН!
			EslType t_found = EslContextApi.FindTypeByName(TypesSet_, name_);
			if (t_found != null)
				return t_found as EslTypeClass; // если мы нашли тип по имени, то возвращаем его только если он класс. иначе нулл - что означает регистрация неуспешна

			EslTypeClass t_new = new EslTypeClass(name_, EslType.InterfaceIncomplete.Tag_);
			bool b_ok = TypesSet_.Add(t_new);
			Debug.Assert(b_ok);
			return (b_ok)? t_new : null;
		}

		internal static EslType GetTypeOrRegisterTypeAlias(EslType type_, string alias_name_) 
		{
			EslType t_found = GetTypeByName(alias_name_);
			if (t_found != null)
				return t_found;
			//TODO	Мы не можем регистрировать псевдонимы типа, если он инкомплете так как последующие SetDefinition должны быть и для типа и для псевдонима. увы
			//		увы пока модель такая. тип не знает о своих псевдонимах..
			//TODO	Это проблема, т.к. все равно возможные последующие модификации интерфейса типа не приведут к изменению псевдонимов
			//		нужно как-то задействовать поле m-types для этой функциональности
			//TODO	!!! Возможно тип нужно НЕ НАСЛЕДОВАТЬ от декларации, а держать ее внутри? тогда и Инкомплете упростится и все псевдонимы будут ссылать в одно место? !!!
			Debug.Assert(type_.IsComplete());
			if (!type_.IsComplete())
				return null;
			EslTypeAlais t_new = new EslTypeAlais(new EslName(alias_name_), type_);
			bool b_ok = TypesSet_.Add(t_new);
			Debug.Assert(b_ok);
			return (b_ok)? t_new : null;
		}


		internal static EslTypeEnumType RegisterEnumType(EslName name_) 
		{
			// если таковое имя уже зарегистрировано как тип (любой, не только енум), то возвращаем нулл. для get-or-register есть другой метод
			if(GetTypeByName(name_) != null )
				return null;

			EslTypeEnumType t_new = new EslTypeEnumType(name_);
			bool b_ok = TypesSet_.Add(t_new);
			Debug.Assert(b_ok);
			return (b_ok)? t_new : null;
		}

		internal static EslTypeEnumerator RegisterEnumerator(EslName name_, EslDecl[] enum_elements_) 
		{
			if (GetTypeByName(name_) != null)
				return null;

			EslTypeEnumerator t_new = new EslTypeEnumerator(name_, enum_elements_);
			bool b_ok = TypesSet_.Add(t_new);
			Debug.Assert(b_ok);
			return (b_ok)? t_new : null;
		}

		internal static EslType GetTypeByName(string type_name_) 
		{
			return EslContextApi.FindTypeByName(TypesSet_, type_name_);
		}
		internal static EslType GetTypeByName(EslName type_name_) {
			return EslContextApi.FindTypeByName(TypesSet_, type_name_);
		}

		internal static EslType GetOrCreateType(string name_) {
			EslType t_found = GetTypeByName(name_);
			if (t_found != null)
				return t_found;

			//TODO	Этот метод вызывается из EslContext_EsbObject для получения типа Esl из типов Esb.
			//		Ну как минимум мы должны какой-то HashSet сделать, а не всегда новй возвращать
			//		(ПС. Из Esb могут приходить как Ru, так и En и не всегда они один-к-одному на Esl-имена ложатся)
			return new EslType(name_);
		}

		// ПС. GetObject.. потому что GetTypeContext резервирую для статиков Типа
		internal static EslDecl[] GetObjectContextNotNull(EslType object_type_not_be_null_) 
		{
			//TODO
			// Смысл эттой индирекции получения EslDecl[] интерфейса через отдельную функцию, а не непосредственно от самого EslType ?
			// А у всех типов интерфейсы "застывшие", а у EsbObject_ постоянно перевычисляется. поэтому он Incomplete задекларирован
			// На самом деле он должен парситься при изменении esbmain.cpp и в сам тип присваиваться. как-то..
			// а пока делаем немного кружным путем избегая виртуальщины

			//TODO	А еще у нас есть псевдонимы типов! и что с ними делать.. их много может быть..
			if (Object.ReferenceEquals(object_type_not_be_null_, EslTypes.EsbObject_))
				return EsbProject.EsbObject.GetEsbInterface();

			if (Object.ReferenceEquals(object_type_not_be_null_, EslTypes.EsbComponent_))
				return EsbProject.EsbComponent.GetEsbInterface();

			return object_type_not_be_null_.GetInterface();
		}

	} // class EslTypes

} // namespace EsbEngine.Esl
