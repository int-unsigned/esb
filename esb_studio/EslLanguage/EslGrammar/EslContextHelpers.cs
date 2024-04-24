using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
#if DEBUG || DEBUG_IN_RELEASE
	using System.Diagnostics;
#endif


//TODO	Разобраться и причести в порядок эти все Апи, Апи2, т.п....



namespace EsbStudio.Esl
{
	internal static class EslContextApi2 
	{
		internal static EslParaSet PARASET(params EslPara[] para_)								{ return new EslParaSet { Para_ = para_ }; 		}
		internal static EslPara PARA(string name_, EslType type_)								{ return new EslPara(name_, type_, null, null); }
		internal static EslDecl DECL(params EslMeth[] meth_)									{ return new EslDecl(meth_);		}
		internal static EslPara[] PARAMS(params EslPara[] para_)								{ return para_; }
		internal static EslMeth METH(string name_, EslType type_)								{ return new EslMeth(name_, type_, EslParaSet.Empty_, null, null); }
		internal static EslMeth METH(string name_, EslType type_, params EslPara[] para_)		{ return new EslMeth(name_, type_, para_, null, null); }
		internal static EslMeth METH(string name_, EslType type_, EslParaSet paraset_)			{ return new EslMeth(name_, type_, paraset_.Para_, null, null); }
		internal static EslMeth[] DECL_METHODS(string name_, EslType type_, params EslParaSet[] parasets_) {
			if (parasets_.Length == 0)
				return new EslMeth[] { METH(name_, type_) };

			EslMeth[] meth_ = new EslMeth[parasets_.Length];
			for (int i_paraset = 0; i_paraset < parasets_.Length; ++i_paraset)	{
				meth_[i_paraset] = METH(name_, type_, parasets_[i_paraset]);
			}
			return meth_;
		}
		internal static EslDecl DECL(string name_, EslType type_, params EslParaSet[] parasets_) {
			return DECL(DECL_METHODS(name_, type_, parasets_));
		}
		internal static EslDecl FUNC(string name_, EslType type_, params EslParaSet[] parasets_) 	{ return DECL(name_, type_, parasets_);	}
		internal static EslDecl FUNC(string name_, EslType type_, params EslPara[] para_)			{ return DECL(METH(name_, type_, para_)); }
		internal static EslDecl FUNC(string name_, EslType type_)									{ return DECL(METH(name_, type_)); }

		internal static EslDecl PROC(string name_, params EslParaSet[] parasets_)					{ return DECL(name_, EslTypes.Void_, parasets_); }
		internal static EslDecl PROC(string name_, params EslPara[] para_)							{ return DECL(METH(name_, EslTypes.Void_, para_)); }
		internal static EslDecl PROC(string name_)													{ return DECL(METH(name_, EslTypes.Void_)); }
		//TODO	Поскольку наши METH требуют имени и это требование глубоко обусловлено выводом на экран в удобоваримом виде
		//		будем конструкторы классов определять именем класса. Запомним.
		//		(возможно спец.имя придумать вроде Конструктор<<КлассИмя>> ?)

		internal static EslMeth[] CTOR(EslType type_, params EslParaSet[] parasets_)				{ return DECL_METHODS(type_.GetName().GetNameText(), type_, parasets_); }
		internal static EslMeth[] CTOR(EslType type_, params EslPara[] para_)						{ return DECL_METHODS(type_.GetName().GetNameText(), type_, PARASET(para_)); }
		internal static EslMeth[] CTOR(EslType type_)												{ return DECL_METHODS(type_.GetName().GetNameText(), type_, PARASET()); }

		internal static EslDecl PROP(string name_, EslType type_)									{ return DECL(new EslMeth(name_, type_, null, null, null)); }
		// не уверен что из себя представляют элементы перечисления - свойства?, константы?, другое..
		// пока делаю как свойства. потом посмотрим
		//TODO	Как минимум они в окне подсказки дурацки выглядят: "Свойство ОК Как КодВозвратаДиалога". нуно доделывать им свой Decl или Meth? хз.. пока
		internal static EslDecl ENUM_ELEM(string name_, EslType type_)								{ return PROP(name_, type_); }
		// а это свойства глобального контекста представляющие энумераторы. возмежно им спец.обработка нужна, как минимум они RO
		internal static EslDecl ENUM_PROP(EslTypeEnumType enum_type_, EslTypeEnumerator enumerator_type_)	{ 
			return PROP(enum_type_.GetName().GetNameText(), enumerator_type_); 
		}
		//TODO	Для каждого типа декларации нужен свой класс. пока так. переменные фактически аналогичны свойствам (rw-свойствам, точнее. TODO флаги свойствам нужны)
		internal static EslDecl VARI(string name_, EslType type_)								{ return PROP(name_, type_); }
		internal static EslDecl CONST(string name_, EslType type_)								{ return PROP(name_, type_); }
	}


	internal static class EslContextApi
	{
		internal static bool HasReturnValue(EslMeth meth_) {
			return (meth_.GetReturnType() != EslTypes.Void_);
		}
		internal static bool HasReturnValue(EslMeth[] meth_) {
			// наборы методов далжны быть либо все Войд (Процедуры), либо все что-то возвращать (Функции)
			// (для набора методов Функции допустимо возвращать разные значения в зависимости от набора параметров)
			bool b_has_retval = HasReturnValue(meth_[0]);
#if DEBUG
			for (int i = 1; i < meth_.Length; ++i)
			{
				Debug.Assert(HasReturnValue(meth_[i]) == b_has_retval);
			}
#endif
			return b_has_retval;
		}
		internal static EslType GetDeclarationMethodsReturnDataType(EslMeth[] declaration_methods_) 
		{	// 1. Мы здесь перепроверяемся, что все методы либо функции либо войд
			// 2. TODO В какой-то момент нам придется решать вопрос юнион-типов и окажется тип данных декларации - это массив или более сложная структура
			if (HasReturnValue(declaration_methods_))
				return declaration_methods_[0].GetReturnType();
			else
				return EslTypes.Void_;
		}
		internal static EslType GetDeclarationMethodsReturnDataType(EslDecl declaration_) {
			EslMeth[] declaration_methods = declaration_.GetDeclarationMethods();
			if (declaration_methods == null || declaration_methods.Length == 0)
				return null;    //фигня какая-то. не метод. не наше дело
			return GetDeclarationMethodsReturnDataType(declaration_methods);
		}
		internal static bool IsEqualNamesCodes(string code1_u_case_, string code2_u_case_) {
			return (code1_u_case_ == code2_u_case_);
		}
		internal static bool IsEqualNames(string name1_x_case_, string name2_x_case_) {
			return String.Equals(name1_x_case_, name2_x_case_, StringComparison.OrdinalIgnoreCase);
		}
		internal static string MakeNameCode(string name_) {
			return name_.ToUpper();
		}
		internal static int MakeNameCodeHash(string name_code_) {
			return name_code_.GetHashCode();
		}
		internal static int CompareNamesNoCase(string name1_, string name2_) {
			return String.Compare(name1_, name2_, StringComparison.OrdinalIgnoreCase);
		}

		internal static bool IsEqualNameCode(string name_x_case_, string code_u_case_) {
			return (MakeNameCode(name_x_case_) == code_u_case_);
		}


		//TODO	Данная функция вызывается из EslEngine::GetMembers в таком виде, потому что GetContextFromExpr возвращает тупо массив EslDecl[]
		//		а откуда он произошел информация теряется. А там кроме тупо массива может быть и Хэш! 
		//		Все таки нужен сласс EslContext объединяющий и Массив и Хэш (ну и может еще что..)
		internal static EslDecl FindDeclarationByName(EslDecl[] declarations_, string declaration_name_) {
			string	declaration_code = MakeNameCode(declaration_name_);
			int		declaration_hash = MakeNameCodeHash(declaration_code);
			foreach (EslDecl declaration in declarations_) {
				if (declaration.GetName().IsEqual(declaration_code, declaration_hash))
					return declaration;
			}
			return null;
		}


		//TODO	!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		private class EslDeclForFind : EslDecl
		{
			EslDeclForFind() : base() { }
			public new void SetName(EslName name_) {
				base.SetName(name_);
			}
			public static EslDeclForFind Instance_ = new EslDeclForFind();
		}
		//
		internal static EslDecl FindDeclaration(HashSet<EslDecl> declarations_index_, EslName declaration_name_) {
			//TODO	Интересно что исходники этого свободно выложены (надо бы полазить, поучиться..)
			//		https://github.com/dotnet/runtime/blob/5535e31a712343a63f5d7d796cd874e563e5ac14/src/libraries/System.Private.CoreLib/src/System/Collections/Generic/HashSet.cs#L465C13-L476C26

			//TODO	Тупейший интерфейс ихнего HashSet не позволяет ни получить элемент по ключу отличающемуся от самого Т ни вообще нормально получить
			//		элемент как .Item или null к примеру - типа для этот Dictionary есть..
			//		потому как private int FindItemIndex(T item)

			//TODO	И даже так не совсем хорошо, т.к. HashSet из массива создается с EslDeclComparierEqNameName, а у нас с одной стороны уже CODE_. 
			//		Нужно свой Хэш-Индекс делать..
			EslDeclForFind.Instance_.SetName(declaration_name_);			
			EslDecl decl_in_hashset = null;
			if (declarations_index_.TryGetValue(EslDeclForFind.Instance_, out decl_in_hashset))
				return decl_in_hashset;

			return null;
		}


		//TODO	Таже херня
		private class _EslTypeForFind : EslType
		{
			private class _EslNameForFind : EslName 
			{
				private _EslNameForFind() : base() 
				{ }
				public static _EslNameForFind Instance_ = new _EslNameForFind();
				public new void SetNameData(string name_) {
					base.SetNameData(name_);
				}
				public new void SetNameData(EslName name_) {
					base.SetNameData(name_);
				}
			}

			_EslTypeForFind() : base(_EslNameForFind.Instance_, EslType.InterfaceIncomplete.Tag_) { }

			public void SetName(string name_) {
				_EslNameForFind.Instance_.SetNameData(name_);
			}
			public new void SetName(EslName name_) {
				_EslNameForFind.Instance_.SetNameData(name_);
			}

			public static _EslTypeForFind Instance_ = new _EslTypeForFind();
		}
		internal static EslType FindTypeByName(HashSet<EslType> elements_set_, string name_) 
		{
			Debug.Assert(name_ != null);

			_EslTypeForFind.Instance_.SetName(name_);
			EslType elem_in_hashset = null;
			if (elements_set_.TryGetValue(_EslTypeForFind.Instance_, out elem_in_hashset))
				return elem_in_hashset;

			return null;
		}
		internal static EslType FindTypeByName(HashSet<EslType> elements_set_, EslName name_) 
		{
			Debug.Assert(name_ != null);

			EslType elem_in_hashset = null;
			_EslTypeForFind.Instance_.SetName(name_);			
			bool b_found = elements_set_.TryGetValue(_EslTypeForFind.Instance_, out elem_in_hashset);
			if (b_found)
				return elem_in_hashset;

			return null;
		}

	} //static class EslContextApi



	internal static class EslContextInformation
	{
		internal static string ParaDescriptoin(string  para_name_, string para_type_) {
			return $"[in] {para_name_} parameter of type '{para_type_}' description for test purposes";
		}

		internal static string ParaDisplay(string para_name_, string para_type_) {
			return $"{para_name_} {EslDefinitions.TypePrefixWithSpace_}{para_type_}";
		}

		internal static string ParaDisplayText_1(EslPara para_) {
			return para_.GetName();
		}
		internal static string ParaDisplayText_2(EslPara para_) {
			string s_name = para_.GetName();
			return (s_name[0] == '<') ? s_name : $"<{s_name}>";
		}
		internal static string ParaDisplayText_3(EslPara para_) {
			string s_name = para_.GetName();
			return $"{para_.GetName()} {EslDefinitions.TypePrefixWithSpace_}{para_.GetDataType()}";
		}

		internal static string ParaDisplay(EslPara para_) {
			return ParaDisplay(para_.GetName(), para_.GetDataType().GetName().GetNameText());
		}
		internal static string ParaDescriptoin(EslMeth meth_, EslPara para_, int para_index_) {
			return ParaDescriptoin(para_.GetName(), para_.GetDataType().GetName().GetNameText());
		}

		internal static string ParaText(EslPara para_) {
			return ParaDisplay(para_.GetName(), para_.GetDataType().GetName().GetNameText());
		}
		internal static string ParaText(EslPara[] para_) {
			Debug.Assert(para_ != null);
			if (para_.Length == 0)
				return String.Empty;
			string s_para = ParaText(para_[0]);
			for (int i_para = 1; i_para < para_.Length; ++i_para) {
				s_para += EslDefinitions.ParamsDelimiterWithSpace_ + ParaText(para_[i_para]);
			}
			return s_para;
		}
		internal static string ParaTextList(EslPara[] para_) {
			return $"{EslDefinitions.ParamsListBegin}{ParaText(para_)}{EslDefinitions.ParamsListFinal}";
		}
		internal static string ParaTextIndexes(EslPara[] para_) {
			return $"{EslDefinitions.IndexListBeginChar}{ParaText(para_)}{EslDefinitions.IndexListFinalChar}";
		}
		internal static bool MethIsFunction(EslMeth meth_) {
			return (meth_.GetReturnType() != EslTypes.Void_);
		}
		internal static bool MethIsProcedure(EslMeth meth_) {
			return (meth_.GetReturnType() == EslTypes.Void_);
		}
		internal static string MethKindText(EslMeth meth_) {

			return (MethIsFunction(meth_))? EslDefinitions.MethKindFunctionText : EslDefinitions.MethKindProcedureText;
		}
		static string MethReturnTypeSuffix(EslMeth meth_) {
			return $" {EslDefinitions.TypePrefixWithSpace_}{meth_.GetReturnType().GetName().GetNameText()}";
		}
		internal static string MethDescriptoin(EslMeth meth_) {
			return $"This is a '{MethDisplay(meth_)}' description for test purposes";
		}

		// MethDisplay системой как таковой не используется, т.к. для списка дополнения вызывается DeclDisplay
		// а MethodTip сам формирует декларацию метода из Имени и Параметров чтобы понимать что "жирненьким" показать в окошке подсказки
		// Поэтому мы будем для MethDisplay возвращать полную декларацию метода со всеми типами и т.п.
		// и использовать ее для отображения подсказки вместе с MethDescription/DeclDescription
		internal static string MethDisplay(EslMeth meth_) 
		{
			EslPara[] meth_params = meth_.GetParams();
			if (meth_params is null)
				return $"{EslDefinitions.MethKindPropertyTextWithSpace}{meth_.GetName()}{MethReturnTypeSuffix(meth_)}";
			else if (ReferenceEquals(meth_.GetName(), EslMeth.OperatorIndexableName_))
				return $"{EslDefinitions.MethKindOperatorTextWithSpace}{meth_.GetName()}{ParaTextIndexes(meth_params)}{MethReturnTypeSuffix(meth_)}";
			else if (meth_.HasReturnValue()) {
				return $"{EslDefinitions.MethKindFunctionTextWithSpace}{meth_.GetName()}{ParaTextList(meth_params)}{MethReturnTypeSuffix(meth_)}";
			}
			else
				return $"{EslDefinitions.MethKindProcedureTextWithSpace}{meth_.GetName()}{ParaTextList(meth_params)}";
		}
	} // class EslContextInformation


	internal static class EslContextDescriptoins
	{
		internal static string GetDeclarationDescriptopn(EslDecl declaration_) 
		{
			//TODO	Загрузка как-то откуда-то
			string decl_id = declaration_.GetName().GetNameCode();
			if (decl_id == "СТРДЛИНА")
				return "Возвращает количество символов в строке.";
			return null;
		}
		internal static string GetDeclarationDescriptionDefault(EslDecl declaration_) 
		{
			string s_declaration_type;
			if (declaration_.IsModuleDeclaration())
				s_declaration_type = "Модуль";
			else
				s_declaration_type = "Декларация";
			return $"{s_declaration_type} '{declaration_.GetName().GetNameText()}' description for test purposes";
		}

		internal static string GetDeclarationDescriptopnFromMethods(EslMeth[] declaration_methods_) 
		{
			Debug.Assert(declaration_methods_ != null && declaration_methods_.Length > 0);

			string s_meth_descr = declaration_methods_[0].GetDescription();
			if (s_meth_descr == null)
			{
				for (int i_meth = 1; i_meth < declaration_methods_.Length; ++i_meth)
				{
					s_meth_descr = declaration_methods_[i_meth].GetDescription();
					if (s_meth_descr != null)
						break;
				}
			}
			return s_meth_descr;
		}

		public static string GetDeclarationMethodsDescriptopn_ForCompletitionListTip(EslDecl declaration_) 
		{
			EslMeth[] declaration_methods = declaration_.GetDeclarationMethods();
			string s_description = "";
			if (declaration_methods != null && declaration_methods.Length > 0)
			{
				s_description = declaration_methods[0].GetDisplayText();
				for (int i_meth = 1; i_meth < declaration_methods.Length; ++i_meth)
				{
					s_description += Environment.NewLine;
					s_description += declaration_methods[i_meth].GetDisplayText();
				}
				s_description += Environment.NewLine;
				//TODO	При показе на экран Их окошко пустые строки игнорирует. Но если описание от списка деклараций методов не отделять, они сливаются
				//		хз.. как не извращайся все равно похабно смотрится. и еще не помещаются длиинные русские декларации нормально. хрень..
				//		свое окно писать надо
				s_description += "------------------";
				s_description += Environment.NewLine;
				string s_description_self = GetDeclarationDescriptopn(declaration_);
				if (s_description_self == null)
					s_description_self = GetDeclarationDescriptopnFromMethods(declaration_methods);
				s_description += s_description_self;
			}
			else {
				s_description = GetDeclarationDescriptopn(declaration_);
				if (s_description == null)
					s_description = GetDeclarationDescriptionDefault(declaration_);
			}

			return s_description;
		}
	}



} // namespace EsbStudio.Esl
