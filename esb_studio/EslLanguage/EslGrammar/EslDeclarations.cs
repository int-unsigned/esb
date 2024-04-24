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
	using static EslContextApi;



	internal class EslName
	{
		internal const int				CodeHashNull_ = 0;
		internal static readonly int	CodeHashVoid_ = MakeNameCodeHash("void");
		//
		private string	m_name;
		private string	m_code;
		private int		m_code_hash;
		protected void SetNameData(string name_, string code_, int hash_) {
			m_name = name_;
			m_code = code_;
			m_code_hash = hash_;
		}
		protected void SetNameData(string name_, string code_) {
			SetNameData(name_, code_, MakeNameCodeHash(code_));
		}
		protected void SetNameData(string name_) {
			SetNameData(name_, MakeNameCode(name_));
		}
		protected void SetNameData(EslName other_) {
			SetNameData(other_.m_name, other_.m_code, other_.m_code_hash);
		}
		//
		protected EslName() {
			m_name = null;
			m_code = null;
			m_code_hash = CodeHashNull_;
		}
		//
		internal EslName(string name_, string code_) {
			Debug.Assert(name_ != null && code_ != null);
			Debug.Assert(name_.Length > 0 && MakeNameCode(name_) == code_);
			SetNameData(name_, code_);
		}
		internal EslName(string name_) {
			Debug.Assert(name_ != null && name_.Length > 0);
			SetNameData(name_);
		}
		//
		internal string GetNameText() {
			return m_name;
		}
		internal string GetNameCode() {
			return m_code;
		}
		internal int GetNameHash() {
			return m_code_hash;
		}
		internal bool IsEqual(string other_code_, int other_hash_) {
			// Если Хэши не равны, то и имена не равны тоже, но если равны возможны коллизии
			if (this.m_code_hash != other_hash_)
				return false;
			return (m_code == other_code_);
		}
		internal bool IsEqual(EslName other_) {
			if (other_ == null)
				return false;
			if (Object.ReferenceEquals(this, other_))
				return true;
			return IsEqual(other_.GetNameCode(), other_.GetNameHash());
		}
		//
		internal int CompareTo(EslName other_) {
			if (other_ == null)	//Мы не можем быть нулл, поэтому всегда больше нулл
				return 1;
			if (Object.ReferenceEquals(this, other_))
				return 0;
			return String.CompareOrdinal(this.m_code, other_.m_code);
		}
	} // class EslName



	internal class EslPara 
	{
		string	m_name;
		EslType m_type;
		string	m_display;
		string	m_description;
		public EslPara(string name_, EslType type_, string display_, string description_) 
		{
			Debug.Assert(name_ != null && name_.Length > 0);
			Debug.Assert(type_ != null);

			m_name = name_;
			m_type = type_;
			m_display = display_;
			m_description = description_;
		}
		public string GetName()				{ return m_name; }
		public EslType GetDataType()		{ return m_type; }
		public string GetDescription()		{ return m_description; }
		public string GetDisplay()			{
			if (m_display == null)
				m_display = EslContextInformation.ParaDisplay(this);
			return m_display; 
		}
	}
	//
	internal class EslParaSet
	{
		internal static EslPara[] Empty_ = new EslPara[] { };
		//
		public EslPara[] Para_;
	}


	internal class EslMeth 
	{
		internal const string	OperatorIndexableName_ = "[]";
		//
		string			m_name;
		EslType			m_rettype;
		EslPara[]		m_para;			//TODO	Надо бы попробовать НУЛЛ для свойств, а для методов без параметров сделать стат-емпти-массив	
		string			m_display;      //TODO	Как используется этот display_ непонятно. на экране всегда имя функции
		string			m_description;
		public EslMeth(string name_, EslType rettype_, EslPara[] para_, string display_, string description_) 
		{
			//! Без имени НЕ БЫВАЕТ. TODO Без Имени бывает у Конструкторов Классов! Пока будет имя класса - но набор деклараций конмтрукторов при этом не уникален по имени
			Debug.Assert(name_ != null && name_.Length > 0);
			m_name = name_;
			m_rettype = rettype_;
			m_para = para_;
			//
			m_display = display_;       //TODO	Как-то этот display_ не используется. Всегда имя показывает.. А было-бы неплохо добавлять Функция/Процедура. Нужно настройки искать..
			m_description = description_;		
		}
		public bool HasReturnValue()				{ return EslContextApi.HasReturnValue(this); }
		public string GetName()						{ return m_name; }
		public EslType GetReturnType()				{ return m_rettype; }
		public string GetDescription()				{
			if (m_description == null)
				m_description = EslContextInformation.MethDescriptoin(this);
			return m_description; 
		}
		public string GetDisplayText()				{ 
			if(m_display == null)
				m_display = EslContextInformation.MethDisplay(this);
			return m_display; 
		}
		//
		public int GetParamsCount()					{ return (m_para == null)? -1 : m_para.Length; }	//Для свойств -1
		public EslPara GetParam(int para_index_)	{ return m_para[para_index_]; }
		//
		public EslPara[] GetParams()				{ return m_para; }
	}



	/*	EslDecl ОСНОВНОЙ КЛАСС системы объектов редактора
	 *	Все что видит и умеет редактор должно так или иначе выражаться в той или иной декларации языка с которым мы работаем
	 *	(в отличие от парсера, который мыслит стейтментами и Аст)
	 *	Декларация может содержать в себе вложенные декларации и так далее. любой элемент языка - это граф деклараций.
	 *	
	 *	Даже нужно уточнить - "языка" имеется языка полностью скажем и вместе с препроцессором (пре-транслятором)
	 *	т.к. например в с++ компилятор директив препроцессора не видит, а для редактора с++ они вполне себе декларации, которые нужно обслуживать
	 *	и для других языков возможно тоже самое - н.р. <Globals>.<...> в редакторе можно показать какой-нибудь список, а при экспорте в 1С заменить на ей понятное
	 *	Или гипотетический модуль Как. Здесь я могу перехватывать обращения к этому имени и показывать набор функций преобразований Как.Строка(Значение), Как.Массив(Значение)
	 *	(которых в природе не существует), а при сохранении в 1С сделать такой модуль только для используемых типов.
	 *	
	 *	Граф Деклараций вообще-то универсальная структура в которой можно описать все.
	 *	К примеру даже Метод - это набор Деклараций .ИмяМетода, .СписоПараметров, .ВозвращаемоеЗначение, т.р.
	 *	(возможны кстати еще аттрибуты - н.р. Const для Декларации Метода, или Глобальный для Декларации Модуля)
	 *	а .СписокПараметров - это массив Деклараций Параметр, которая также имеет декларации Имя, Тип, т.п.
	 *	
	 *	Фактически некоторый XML получается и здесь главное вовремя остановиться и не упасть Декларации каждой запятой
	 *	иначи с такой супер-универсальной структурой приктически работать умаешься и компьютер замучаешь
	 *	
	 *	В конце концов нам не на выставку, а просто человеку помочь программу написать надо.
	 */

	/*	TODO	Нужно у базового EslDecl сделать енум с типом Декларации и все конструкторы protected
	 *			подумать над целесообразностью таскать в базе m_methods. чаще всего он используется взаимоисключающе с m_declarations
	 *			(все эти IsModuleDeclaration если тот-то нулл, а это не нулл, а тут рыбу заворачивали решатся через енум EslDecl.Type)
	 *			
	 *			Категорически не забыть, что в базе у Декларации должны быть еще SourceURI и SourcePoint - где она определна
	 *			т.к. без этого мы Goto (F12) сделать не можем
	 *			также может быть полезным DocComment для .Description из текста кода читать.
	 *			(SourceURI и SourcePoint) для редактора это необходимая информация. class EslURI ?? .URIText .URIPoint ?
	 *			(URI - Universal Resource Identificator)
	 */

	// Чтобы не было путаницы между методами базы и наследников все методы базы имеют префикс Declaration
	// т.е. у базы GetDeclarationDeclarations, а у наследника EslDeclModule .GetModuleDeclarationsExport
	// (хотя возвращают одно и то же, пока..)
	// Вроде напрашивается виртуальный интерфейс сделать, но это "красивости"
	// вся основная обработка идет невзирая на тип декларации и только изредка приходится приводить к конкретной
	// пока, по крайней мере. на красивом интерфейсе просто тупо потеряем производительность.

	internal class EslDecl 
	{
		EslName				m_name;
		private string		m_description = null;
		private EslMeth[]	m_methods = null;
		private EslDecl[]	m_declarations = null;
		public EslDecl(EslMeth[] methods_) 
		{
			// Для того чтобы имя показывать в IntelliSence у него должны быть Methods - интерфейс такой
			// 
			// Для Свойств попробуем исхитриться с кол-вом параметров -1
			Debug.Assert(methods_ != null && methods_.Length > 0);
			m_name = new EslName(methods_[0].GetName());
			m_methods = methods_;			
		}
		public EslDecl(string name_, EslDecl[] declarations_) {
			Debug.Assert(declarations_ != null && declarations_.Length > 0);
			m_declarations = declarations_;
			m_name = new EslName(name_);
		}
		protected EslDecl(EslName name_, EslDecl[] declarations_, EslMeth[] methods_) {
			//в протектед конструкторах мы ничего не проверяем - это дело наследников (н.р. у войд имя нулл)
			m_name = name_;
			m_declarations = declarations_;
			m_methods = methods_;
		}
		// Для EslDeclModule, которые заранее знают свое имя
		protected EslDecl(string name_, string code_, EslDecl[] declarations_) {
			// Не проверяем declarations_ на нулл и пустой массив. Вполне могут быть модули без единого экспорта.
			m_declarations = declarations_;
			m_name = new EslName(name_, code_);
		}
		protected void SetDeclarations(EslDecl[] declarations_) {
			m_declarations = declarations_;
		}
		protected void SetDefinition(EslDecl[] declarations_, EslMeth[] methods_) {
			m_declarations = declarations_;
			m_methods = methods_;
		}
		//---------- см. в EslContextApi::FindDeclarationByCode
		protected EslDecl() {
			
			m_name = null;
			m_description = null;
			m_methods = null;
			m_declarations = null;
		}
		protected void SetName(EslName name_) {
			m_name = name_;
		}
		//
		// public ------------>
		public EslName GetName() {
			return m_name;
		}
		public EslMeth[] GetDeclarationMethods() {
			return m_methods;
		}
		public EslDecl[] GetDeclarationDeclarations() {
			return m_declarations;
		}
		public string GetDeclarationDescription() {
			return m_description;
		}
		public bool IsModuleDeclaration() {
			return (m_declarations != null && m_methods == null);
		}
		public bool IsTypeDeclaration() {
			// У типа, в отличие от модуля, в m_methods должен быть конструктор (как-бы декларации нет, а набор методов с одним именем (возможно стандартным каким-то) есть) 
			return (m_declarations != null && m_methods != null);
		}

		// Используется в Declarations::GetDisplayText для отображения в списке дополнения имени.
		// должно быть по возможности кратким и по возможности емким. у самого шарпа прости имя.
		// а для доп-инфы справа(слева) от списка окно подсказки Declarations::GetDescription в котором можно много интересного рассказать
		//TODO	Как вариант можно в списке показывать 1С-декларацию без типов параметров скажем в виде
		//		СтрДлина(<Строка>) -> Число
		//		СтрДлина(<Строка>)->Число
		//		может возникнуть проблема, что в окошко списка или не влезет или мы вынудим Студию лишний раз ширину списка пересчитывать
		//		или при широком списке места на экране для емкого описания в окне подсказки не хватит
		//		т.к. параметров бывает оочень много..
		// Представляется, что приписывать в начале Функция/Процедура ненедо - с толку сбивает при вводе т.к. на экране у тебя просто имя
		// для этого Глиф (картинка) есть.
		public string GetDeclarationDisplayText_ForCompletitionList() {
			return GetName().GetNameText();
		}
		public string GetDeclarationDescriptopn_ForCompletitionListTip() {
			if (this.m_description == null) 
				this.m_description = EslContextDescriptoins.GetDeclarationMethodsDescriptopn_ForCompletitionListTip(this);
			
			return this.m_description;
		}
		public string GetDeclarationDescriptopn_ForQuickInfoTip() {
			//TODO	Неправильно нам здесь разные Description_ формировать. Они должны саме себе что надо формировать. У Декл Description_ должен быть один.
			return GetDeclarationDescriptopn_ForCompletitionListTip();
		}
		public int GetDeclarationGlyph_ForCompletitionList() {
			//TODO	GetGlyph - это картинка слева от имени в списке. Штука полезная показать где Функция, где Процедура, т.п.
			//		нужно разбираться где и откуда их брать. вроде в студии мноого готовых уже есть
			//		http://glyphlist.azurewebsites.net/knownmonikers/		- ???
			//		ПС. по хелпу если вернуть -1, то типа нет картинки. На пратике генерится исключение. 0 - непонятно че, но без ошибок.
			return 0;
		}
	} // class EslDecl
	//
	//
	class EslDeclComparierEq : IEqualityComparer<EslDecl>
	{
		public bool Equals(EslDecl a, EslDecl b) 
		{
			Debug.Assert(a != null && b != null);

			if (ReferenceEquals(a, b))
				return true;
			if (a is null || b is null)
				return false;

			return a.GetName().IsEqual(b.GetName());			
		}
		public int GetHashCode(EslDecl decl_) {
			return decl_.GetName().GetNameHash();
		}
		//
		public static EslDeclComparierEq Instance_ = new EslDeclComparierEq();
	}
	class EslDeclComparier : IComparer<EslDecl>
	{
		public int Compare(EslDecl a, EslDecl b) {
			Debug.Assert(a != null && b != null);
			return a.GetName().CompareTo(b.GetName());
		}
		//
		public static EslDeclComparier Instance_ = new EslDeclComparier();
	}


	internal class EslDeclModule : EslDecl 
	{	
		HashSet<EslDecl>	m_declarations_export_index;
		EslDecl[]			m_declarations_all;
		HashSet<EslDecl>	m_declarations_all_index;
		bool				m_is_global_module;
		public EslDeclModule(string module_name_, string module_code_, bool is_global_module_, EslDecl[] declarations_all_, EslDecl[] declarations_export_) 
																							: base(module_name_, module_code_, declarations_export_)
		{
			//нулл не позволяем. пустые массивы позволяем. экспорт-декларац должно быть не меньше чем всех
			Debug.Assert(declarations_all_ != null && declarations_export_ != null);
			Debug.Assert(declarations_all_.Length >= declarations_export_.Length);

			m_declarations_all = declarations_all_;
			//TODO	Видимо возможна оптимизация для малых модулей и модулей у которых чего-либо нет или когда все одинаково. многовато хэшсетов. непростые они
			m_declarations_all_index	= declarations_all_.ToHashSet(EslDeclComparierEq.Instance_);
			if (ReferenceEquals(declarations_all_, declarations_export_))
				m_declarations_export_index = m_declarations_all_index;
			else
				m_declarations_export_index = declarations_export_.ToHashSet(EslDeclComparierEq.Instance_);
			m_is_global_module = is_global_module_;
		}
		//
		internal EslName GetModuleName() {
			return base.GetName();
		}
		internal bool IsGlobalModule() {
			return m_is_global_module;
		}
		internal EslDecl GetModuleDeclaration() {
			return this;
		}
		internal EslDecl[] GetModuleDeclarationsExport() {
			return base.GetDeclarationDeclarations();
		}
		internal EslDecl[] GetModuleDeclarationsAll() {
			return base.GetDeclarationDeclarations();
		}
		internal EslDecl GetModuleDeclarationExport(EslName declaration_name_) {
			return FindDeclaration(m_declarations_export_index, declaration_name_);
		}
		internal EslDecl GetModuleDeclarationAny(EslName declaration_name_) {
			return FindDeclaration(m_declarations_all_index, declaration_name_);
		}
	} // class EslDeclModule


	/*	EslType, хотя унаследовн от Decl как и все, но суффикса Декл не имеет. с одной стороны для простоты,
	 *	а с другой еще подумать надо, т.к. у типа получается два Декл (как минимум) один для имени, другой для определения
	 *	могут совпадать, а могут и нет, и еще деклараций имен несколько может быть.. 
	 *	TODO подумать..
	 *	
	 *	TODO	сам EslType также сделать строго базой с протектед конструкторами. создавать только наследников.
	 */
	internal class EslType : EslDecl
	{
		public class InterfaceIncomplete 
		{
			public static readonly InterfaceIncomplete Tag_ = new InterfaceIncomplete();
			private InterfaceIncomplete() {}
		}
		private static readonly EslDecl[]	m_interface_incomplete = new EslDecl[]{};
		// в отличие от нашего внутреннего m_interface_incomplete со спец.смыслом, этих можно публичить. пусть пользуются
		public static readonly EslDecl[]	InterfaceEmpty_ = new EslDecl[] { };
		public static readonly EslMeth[]	ConstructorsAbsent_ = new EslMeth[]{};
		//
		/*		private EslName				m_name;
				private EslDecl[]			m_interface = null;
				private EslDecl[]			m_constructors = null;
		*/
		private EslType[]			m_types = null;
		// для Войд без имени
		protected EslType() : base (null, m_interface_incomplete, ConstructorsAbsent_) 
		{ }
		internal EslType(EslName name_, InterfaceIncomplete tag_) : base(name_, m_interface_incomplete, ConstructorsAbsent_) 
		{ }
		// public
		// это для простых типов (и енум-типов, НЕ енумератор-типов) без конструкторов, интерфейса и т.п.
		// EslDecl считает, что она декларация типа, когда есть и декларации и метода (могут быть пустыми)
		internal EslType(string name_) : base(new EslName(name_), InterfaceEmpty_, ConstructorsAbsent_) 
		{ }
		//internal EslType(EslName name_) : base(name_, InterfaceEmpty_, ConstructorsAbsent_) { }
		// это когда псевдоним делают. новое имя из строки, а интерфейс весь из другого типа
		internal EslType(EslName name_, EslDecl[] interface_, EslMeth[] constructors_) : base(name_, interface_, constructors_) 
		{ }		
		// для тип-юнион. фактически псевдоним, специфический. ни методов, ни конструктора
		internal EslType(string name_, params EslType[] types_) : base(new EslName(name_), InterfaceEmpty_, ConstructorsAbsent_) 
		{
			m_types = types_;
		}
		// public
		public bool			IsComplete()		{ return (base.GetDeclarationDeclarations() != m_interface_incomplete); }
		public EslDecl[]	GetInterface()		{
			Debug.Assert(IsComplete());
			return (IsComplete())? base.GetDeclarationDeclarations() : null; 
		}
		public EslMeth[]	GetConstructors()	{
			Debug.Assert(IsComplete());
			return (IsComplete()) ? base.GetDeclarationMethods() : null;
		}
		public new void SetDefinition(EslDecl[] interface_, EslMeth[] constructors_) 
		{
			Debug.Assert(GetName() != null); //нету для войда определения
			//Нормальный Тип должен быть с Декл не нулл и constructors_ (хотя они могут быть пустыми)
			Debug.Assert(interface_ != null && constructors_ != null);
			//TODO какой-то ThrowHelper нужен.. с красивыми сообщениями и куда-то их красиво показывать..
			if (interface_ == null || constructors_ == null)
				throw new ArgumentNullException("interface_ == null || constructors_ == null");
			else
				base.SetDefinition(interface_, constructors_);
		}		
	}

	class EslTypeComparierEq : IEqualityComparer<EslType>
	{
		public bool Equals(EslType a, EslType b) {
			Debug.Assert(a != null && b != null);

			if (ReferenceEquals(a, b))
				return true;
			if (a is null || b is null)
				return false;

			if (ReferenceEquals(a, EslTypes.Void_))
				return (ReferenceEquals(b, EslTypes.Void_))? true : false;
			else if (ReferenceEquals(b, EslTypes.Void_))
				return false;
		
			return a.GetName().IsEqual(b.GetName());
		}
		public int GetHashCode(EslType type_) {
			Debug.Assert(type_ != null);
			if (ReferenceEquals(type_, EslTypes.Void_))
				return EslName.CodeHashVoid_;
			else
				return type_.GetName().GetNameHash();
		}
		//
		public static EslTypeComparierEq Instance_ = new EslTypeComparierEq();
	}





	internal class EslTypeEnumType : EslType
	{
		public EslTypeEnumType(EslName name_) : base(name_, EslType.InterfaceEmpty_, EslType.ConstructorsAbsent_) { }
	}
	//
	internal class EslTypeEnumerator : EslType
	{
		public EslTypeEnumerator(EslName name_, EslDecl[] enum_elements_) : base(name_, enum_elements_, EslType.ConstructorsAbsent_)
		{ }
	}
	internal class EslTypeAlais: EslType
	{
		public EslTypeAlais(EslName name_, EslType source_) : base(name_, source_.GetInterface(), source_.GetConstructors()) { }
	}
	internal class EslTypeClass: EslType
	{
		public EslTypeClass(EslName name_, EslType.InterfaceIncomplete tag_) : base(name_, tag_) { }
		public EslTypeClass(EslName name_, EslDecl[] interface_, EslMeth[] constructors_) : base(name_, interface_, constructors_) { }
	}



} //namespace EsbStudio.Esl
