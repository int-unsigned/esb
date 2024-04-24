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
	using static EslContextApi;


	internal static class EslEngineUI
	{
		//TODO	Все в try-catch
		internal static string GetDeclarationQuickInfoText(EslDecl declaration_) 
		{
			try
			{
				return declaration_.GetDeclarationDescriptopn_ForQuickInfoTip();
			}
			catch (Exception ex)
			{
				return ex.Message;
				//return EslParseResult.ErrorParseString(ex);
			}
		}
		internal static EslDecl GetNameDeclarationForQuickInfo(EslSource source_, int context_line_, int context_col_, StringCollection names_) 
		{
			try
			{
				return EslEngine.GetNameDeclaration(source_, context_line_, context_col_, names_, names_.Count);
			}
			catch (Exception ex)
			{
				return EslParseResult.ErrorParseDecl(ex);
			}
		}
		internal static Methods GetNameMethodsForMethodsTip(EslSource source_, int context_line_, int context_col_, StringCollection names_, int expected_to_display_param_index_) 
		{
			try
			{
				var methods = EslEngine.GetNameMethods(source_, context_line_, context_col_, names_);
				if (methods == null)
					return EslParseResult.NoMethods_;

				//TODO	Конечно выяснять какой конкретно метод наиболее подходит под "описание" из source_ можно намного толковее. Но Потом.. Когда полный парсинг будет
				//		Пока просто отсеиваем неподходящие по кол-ву параметров и показываем первый подходящий. Или 0 по-умолчанию
				//TODO	"Их" реализация пока все равно методы на ходу перещелкивать не умеет..
				int default_meth = 0;
				for (int i_meth = 0; i_meth < methods.Length; ++i_meth)
				{
					// Нам дают индекс параметра который хочет видеть юзер. Поэтому <= - для 0-го параметра мы должны начать с метода с 0-м параметров.
					//TODO	Это верно для МетодТип при вводе, но при Аутос когда нужно информировать о методе который уже полностью написан в программе со всеми параметрами
					//		мы должны делать именно < - чтобы для Фуу(1) не начинать с варианта Фуу()
					//		ЭТО ДОЛЖНА БЫТЬ функция GetNameMethodsForInfo - и у нее другой смысл - не expected_to_display_param_index_, а entered_params_count
					//		И ДЛЯ нее другой алгоритм - показать строго == methods[i_meth].GetParamsCount()
					if (expected_to_display_param_index_ <= methods[i_meth].GetParamsCount())
					{
						default_meth = i_meth;
						break;
					}
				}

				return new EslDeclarationsMethods(methods, default_meth);
			}
			catch (Exception ex) {
				return EslParseResult.ErrorParseMethods(ex);
			}
		}


		// Показ списка членов после '.'
		internal static Declarations GetMembers(EslSource source_, int context_line_, int context_col_, StringCollection names_, int use_names_count_) 
		{
			try
			{
				EslDecl[] decls = EslEngine.GetNameMembers(source_, context_line_, context_col_, names_, use_names_count_);
				if (decls == null)
					return EslParseResult.NoDeclarations_;

				return new EslDeclarationsOnArray(decls);
			}
			catch (Exception ex) {
				return EslParseResult.ErrorParseDeclarations(ex);
			}
		}


		// Это вызывается для дополнения имени - т.е. последний элемент names_ только какой-то предполагаемый кусок имени декларации
		// но все предыдущие должны быть нормальными именами
		// (выписана вся в EslEngineUI, т.к. собственно к EslEngine отношения не имеет. язык с недописанными словами не работает)
		internal static Declarations GetCompleteWord(EslSource source_, int context_line_, int context_col_, StringCollection names_) 
		{
			try
			{
				Debug.Assert(names_ != null && names_.Count >= 1);
				//
				if (names_ == null || names_.Count == 0)
					return EslParseResult.NoDeclarations_;

				//TODO	Как отбирать имена для показа в списке - этот вопрос требует изучения..
				//		с одной стороны лучше "побольше", т.к. после формирования и показа списка на экране он больше не перезагружается
				//		а только обрабатывается на месте (вроде..)
				//		А с другой стороны очень большой список будет тормозить и бестолков..

				if (names_.Count == 1)
				{
					HashSet<EslDecl> match_declarations = EslEngine.GetMatchDeclarations(source_, context_line_, context_col_, names_[0]);
					Debug.Assert(match_declarations != null); //там вроде всегда новый создается, хотя могут вернуть и пустой
					List<EslDecl> match_declarations_list = match_declarations.ToList();
					match_declarations_list.Sort(EslDeclComparier.Instance_);
					return new EslDeclarationsOnList(match_declarations_list);
				}
				else
				{
					int name_last_index = names_.Count - 1;
					EslDecl context_declaration = EslEngine.GetNameDeclaration(source_, context_line_, context_col_, names_, name_last_index);
					if (context_declaration == null)
						return null;
					EslDecl[] context_declaration_declarations = EslEngine.GetContextDeclarationMembers(context_declaration);
					if (context_declaration_declarations == null)
						return null;

					string name_part = names_[name_last_index];
					//TODO	Вот здесь допускаю, что юзер сказал дополнить сразу после точки, не введя ни одного символа для name_part
					//		тогда получается, что мы должны ему вернуть весь список, но он у нас в массиве, а нужен Лист
					//		Глупо. Тем более, что у нас Declarations есть и на базе массива и на базе Лист
					if (name_part.Length == 0)
						return new EslDeclarationsOnArray(context_declaration_declarations); // массив деклараций всегда должен быть сортирован. TODO перепровериться в дебаге

					List<EslDecl> declarations_match = new List<EslDecl> { };
					string declaration_code_part = MakeNameCode(name_part);
					foreach (EslDecl declaration in context_declaration_declarations)
					{
						//TODO	Как отбирать имена для показа в списке - этот вопрос требует изучения..
						//		с одной стороны лучше "побольше", т.к. после формирования и показа списка на экране он больше не перезагружается
						//		а только обрабатывается на месте (вроде..)
						//		А с другой стороны очень большой список будет тормозить и бестолков..
						if (EslEngine.IsMatchDeclaration(declaration, declaration_code_part))
							declarations_match.Add(declaration);
					}
					// массив деклараций всегда должен быть сортирован. TODO перепровериться в дебаге
					// поэтому получившаяся из него последовательная выборка тоже будет сортирована
					return new EslDeclarationsOnList(declarations_match);
				}
			}
			catch (Exception ex) {				
				return EslParseResult.ErrorParseDeclarations(ex);
			}
		}
	}// class EslEngineUI
} // namespace EsbStudio.Esl


namespace EsbStudio.Esl
{
	internal class EslDeclarationsMethods : Methods
	{
		/*	TODO	Вообще-то стандартный класс MethodData не совсем хорошо нам подходит т.к. исходя из
		 *			https://learn.microsoft.com/en-us/dotnet/api/microsoft.visualstudio.package.methoddata.getmethodtext?view=visualstudiosdk-2022
		 *			неким текстом можна как ПРЕДВАРЯТЬ так и ПОСЛЕ так и ВМЕСТЕ
		 *			т.е. для запроса MTT_TYPEPREFIX возвращать Функция/Процедура, а для запроса MTT_TYPEPOSTFIX возвращать 'Как Тип' или НУЛЛ
		 *			(оно само в тип/не-тип неразбирается - ей надо только что перед/после написать, вроде...)
		 */
		public override bool TypePrefixed
		{
			get => false;   // Это где тип (если есть) форматировать - перед методом или после. см.выше.. Этот флаг нам в MethodData и мешается..
		}
		public override string Delimiter
		{
			get { return EslDefinitions.ParamsDelimiterWithSpace_; }	//TODO	В окне подсказки параметры показывает через запятую, но без пробела. Возможно настройки просто нужны
		}
		public override string TypePrefix 
		{
			/* Methods представляется студией как единый букет методов, которые ВСЕ либо возвращают значение (Тип) - Функции
			 * либо не возвращают (Тип-Нулл) - Процедуры.
			 * Возвращаемый тип может быть разным, но для всего букета функций он должен БЫТЬ НЕ НУЛЛ
			 * Исходя из кода MethodData.GetMethodText (который форматирует Текст), то 
			 *		sType = this.methods.TypePrefix + methods.GetType(method) + this.methods.TypePostfix;
			 * То есть нужно с пробелом.
			 */
			get 
			{
				if (HasReturnValue())
					return EslDefinitions.TypePrefixWithSpace_;          //TODO Сделать для английского варианта (видимо настройка должна быть где-то в классе Source и/или в проекте)
				else
					return null;
			}
		}
		public override int DefaultMethod
		{
			//TODO	Как-то оно само не переключается, скажем, на сигнатуру с двумя параметрами... Может настройки какие нужны, а может ЭТО задействовать
			//		** The index of the first method signature to show the user when the IntelliSense method tip is displayed. **
			get {
				return m_meths_default; 
			}	
		}
		//
		EslMeth[]		m_meths;
		int				m_meths_default;
		public EslDeclarationsMethods(EslMeth[] meths_, int meths_default_) 
		{
			Debug.Assert(meths_ != null && meths_.Length > 0);
			m_meths				= meths_;
			m_meths_default		= meths_default_;
		}
		EslMeth get_meth(int meth_index_) {
			if (meth_index_ < 0 || meth_index_ >= m_meths.Length) throw Errors.ArgumentException(this, nameof(meth_index_), meth_index_);
			return m_meths[meth_index_];
		}
		//public:
		public bool HasReturnValue()							{ return EslContextApi.HasReturnValue(m_meths); }
		public override int GetCount()							{ return m_meths.Length; }
		public override string GetDescription(int meth_index_)	{ return get_meth(meth_index_).GetDescription(); }
		public override string GetName(int meth_index_)			{
			return get_meth(meth_index_).GetName();	
		}
		public override int GetParameterCount(int meth_index_)	{ return get_meth(meth_index_).GetParamsCount();	}
		public override void GetParameterInfo(int meth_index_, int para_index_, out string out_name_, out string out_display_, out string out_description_) 
		{
			EslMeth meth = get_meth(meth_index_);
			EslPara[] meth_params = meth.GetParams();
			if (meth_params is null)
			{  // Свойство не имеет параметров вообще. Если мы здесь, то это по видимому ошибка, т.к. GetParameterCount должен был вернуть -1 и мы здесь быть не должны
				Debug.Assert(false);
				out_name_ = null;       //TODO	Надо посмотреть не поведение при возврате нулл
				out_display_ = null;
				out_description_ = null;
			}
			else if (para_index_ < meth_params.Length)
			{
				EslPara para = meth.GetParam(para_index_);
				out_name_ = para.GetName();
				out_display_ = para.GetDisplay();
				out_description_ = para.GetDescription();
				if (out_description_ == null)
					out_description_ = EslContextInformation.ParaDescriptoin(meth, para, para_index_);
			}
			else { // а зедсь по видимому какая-то ошибка
				Debug.Assert(false);
				out_name_ = null;       //TODO	Надо посмотреть не поведение при возврате нулл
				out_display_ = null;
				out_description_ = null;
			}
		}
		public override string GetType(int meth_index_)	{
			EslType meth_rettype = get_meth(meth_index_).GetReturnType();
			//TODO для процедуры вернется Void_. У него EslTypes.Void_.Name_ == null задумал я так чтобы напарываться и спец-обработку делать
			//		вот здесь для него возвращаю null. Надо посмотреть где сработает
			return (meth_rettype == EslTypes.Void_)? null : meth_rettype.GetName().GetNameText();
		}	
	}


	internal class EslDeclarationsOnList : Declarations
	{
		/*	Кроме обязательных к определению абстрактных методов у класса есть еще виртуальные методы с имплементацией по-умолчанию, 
		 *	которые вызывают наши абстрактные переопределенные
		 *	Их тоже можно переопределить для более эффективной реализации
		 *	https://learn.microsoft.com/ru-ru/dotnet/api/microsoft.visualstudio.package.declarations?view=visualstudiosdk-2022&devlangs=csharp&f1url=%3FappId%3DDev16IDEF1%26l%3DRU-RU%26k%3Dk(Microsoft.VisualStudio.Package.Declarations)%3Bk(TargetFrameworkMoniker-.NETFramework%2CVersion%253Dv4.5)%3Bk(DevLang-csharp)%26rd%3Dtrue
		 *	
		 *	* Реализованные методы этого класса предполагают, что объявления хранятся в списке, отсортированном по возрастанию.
		 */
		//
		List<EslDecl> m_decls;		
		public EslDeclarationsOnList(List<EslDecl> decls_) {
			m_decls = decls_;
		}
		public override int GetCount() {
			return m_decls.Count;
		}
		public override string GetDescription(int decl_index_) {
			/* If an item does not contain a description, an empty string can be returned
			 * Sometimes the description can simply be the type of the declaration; for example, an integer variable could have a description of simply "integer" or "int".
			*/
			return m_decls[decl_index_].GetDeclarationDescriptopn_ForCompletitionListTip();
			
		}
		public override string GetDisplayText(int decl_index_) {
			/* The display text is shown in the list but it is the value returned from the GetName method that is inserted. 
			 * It is also the value returned from GetName that is used in finding the best match. This allows the display text, 
			 * for example, to include adornments such as surrounding "<" and ">" characters or show decorated names as used in C++.
			 */
			return m_decls[decl_index_].GetDeclarationDisplayText_ForCompletitionList();
		}
		public override int GetGlyph(int decl_index_) {
			/* The image to be shown typically indicates a type for the associated item. For example, an image could indicate the item is a method, a variable, or an event handler.
			 * The image index is used on the image list that was supplied to the CompletionSet object and typically is the same image list that is returned from the 
			 * GetImageList method in the LanguageService class.
			 * 
			 * The index of the image from an image list, otherwise -1.
			 */
			return m_decls[decl_index_].GetDeclarationGlyph_ForCompletitionList();
		}
		public override string GetName(int decl_index_) {
			/* The name of the item is what is searched for as the user types characters and is what is inserted into the source when the user types a completion character. 
			 * The name is not necessarily the same as what is displayed in the list. 
			 * However, the name is what is searched on when the GetBestMatch method is called so the displayed text should at the very least contain the name of the item.
			 * 
			 * If successful, returns the name of the item; otherwise, returns null.
			 */
			return m_decls[decl_index_].GetName().GetNameText();
		}
	}


	// Для дополнения слова (Completition) удобно список, а для полного списка мемберов (после .) удобно напрямую массив
	internal class EslDeclarationsOnArray : Declarations
	{
		/*	Кроме обязательных к определению абстрактных методов у класса есть еще виртуальные методы с имплементацией по-умолчанию, 
		 *	которые вызывают наши абстрактные переопределенные
		 *	Их тоже можно переопределить для более эффективной реализации
		 *	https://learn.microsoft.com/ru-ru/dotnet/api/microsoft.visualstudio.package.declarations?view=visualstudiosdk-2022&devlangs=csharp&f1url=%3FappId%3DDev16IDEF1%26l%3DRU-RU%26k%3Dk(Microsoft.VisualStudio.Package.Declarations)%3Bk(TargetFrameworkMoniker-.NETFramework%2CVersion%253Dv4.5)%3Bk(DevLang-csharp)%26rd%3Dtrue
		 *	
		 *	* Реализованные методы этого класса предполагают, что объявления хранятся в списке, отсортированном по возрастанию.
		 */
		//
		EslDecl[] m_decls;
		public EslDeclarationsOnArray(EslDecl[] decls_) {
			m_decls = decls_;
		}
		public override int GetCount()							{ return m_decls.Length; }
		public override string GetDescription(int decl_index_)	{ return m_decls[decl_index_].GetDeclarationDescriptopn_ForCompletitionListTip(); }
		public override string GetDisplayText(int decl_index_)	{ return m_decls[decl_index_].GetDeclarationDisplayText_ForCompletitionList();	}
		public override int GetGlyph(int decl_index_)			{ return m_decls[decl_index_].GetDeclarationGlyph_ForCompletitionList();	}
		public override string GetName(int decl_index_)			{ return m_decls[decl_index_].GetName().GetNameText();}
	}
} // namespace EsbStudio.Esl
