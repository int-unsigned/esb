using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
#if DEBUG || DEBUG_IN_RELEASE
	using System.Diagnostics;
#endif


using Microsoft.VisualStudio.Package;
using System.ComponentModel.Composition;
using Microsoft.VisualStudio.Utilities;				//export attribute + etc...
using System.Runtime.InteropServices;				//guid attribute
using Microsoft.VisualStudio.TextManager.Interop;   //IVsTextLines Interface


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
	// Для Студии-15 есть пошаговая инструкция как делать эту LanguageService (но почему-то объявлена устаревшей)
	//https://learn.microsoft.com/ru-ru/previous-versions/visualstudio/visual-studio-2015/extensibility/internals/implementing-a-legacy-language-service2?view=vs-2015
	//НА ЗАМЕТКУ. Там какая-то проблема у человека была
	//https://stackoverflow.com/questions/5211018/implementing-a-language-service-by-using-the-managed-package-framework
	/*	Я провел дополнительную отладку и заметил, что экспериментальный экземпляр Visual Studio зависает, неоднократно вызывая следующее:
		Microsoft.VisualStudio.Editor.Implementation.VsFontsAndColorsInformation.TryGetIndexForNativeItem(имя строки, IVsColorTable colorTable, Int32& colorTableIndex)
		Догадавшись, я указал для параметра RequestStockColors значение true в атрибуте ProvideLanguageServiceAttribute, и теперь я могу открыть свой тип файла.
		Мой простой сканер даже имеет рабочую раскраску синтаксиса!
	*/




	//Основное описание сервиса
	//https://learn.microsoft.com/en-us/dotnet/api/microsoft.visualstudio.package.languageservice?view=visualstudiosdk-2022
	//он унаследован пного от чего, в том числе и IVsLanguageInfo. Редактор Razor непосредственно наследуется от IVsLanguageInfo без наследования от LanguageService
	//https://learn.microsoft.com/en-us/dotnet/api/microsoft.visualstudio.textmanager.interop.ivslanguageinfo?view=visualstudiosdk-2022
	//
	//https://learn.microsoft.com/ru-ru/previous-versions/visualstudio/visual-studio-2015/extensibility/internals/implementing-a-legacy-language-service2?view=vs-2015
	[Guid(DefConstants.LanguageServiceGuidString_)]
	public class EslLanguageService : LanguageService
	{
		public EslLanguageService() : base() { }    //*Remember to call the base class constructor in your derived class's constructor.*	TODO	А это в шарпах обязательно что ли?
		//
		private LanguagePreferences m_LanguagePreferences;
		//
		public override string Name => DefConstants.LanguageName_;

		public override int GetFileExtensions(out string pbstrExtensions) {
			//The extensions are stored in a semi-colon delimited list. For example, if your language service supports C++, C#, and Visual Basic, it might return ".cpp;.cs;.vb"
			pbstrExtensions = DefConstants.FileExtensions_;
			return Microsoft.VisualStudio.VSConstants.S_OK;
		}
		public override string GetFormatFilterList() {
			return DefConstants.FileFiltersForSaveAs_;
		}
		public override LanguagePreferences GetLanguagePreferences()
		{
			Debug.WriteLine("EslLanguageService::GetLanguagePreferences");

			//* It is expected that you will have a single preferences object shared with all instances of your language service.
			//	ЭТО КАК??? Статический где-то в пакете делать что ли?
			//* If you do not need any custom preferences beyond those supplied by the LanguagePreferences class, you can just return an instance of the LanguagePreferences class.
			if (this.m_LanguagePreferences == null)
			{
				this.m_LanguagePreferences = new LanguagePreferences(this.Site,	typeof(EslLanguageService).GUID, this.Name);
				if (this.m_LanguagePreferences != null)
				{
					this.m_LanguagePreferences.Init();  // Must do this first!
					
					// Temporarily enable the following properties.
					this.m_LanguagePreferences.EnableMatchBraces = true;
					this.m_LanguagePreferences.ParameterInformation = true;
				}
			}
			return this.m_LanguagePreferences;
		}

		public override IScanner GetScanner(IVsTextLines buffer) 
		{
			return new BslTokensScanner();
		}
		internal EslSource GetSourceOfRequest(EslParseRequest request_) 
		{
			/*	Парсинг инициируется для Source, который переадресует запрос своему EslLanguageService
			 *	Проблема в том, что не все и не всегда запросы на парсинг приходят в результате набора текста
			 *	а могут инициироваться через меню или клавиатурными командами
			 *	Ctrl-J				Список членов				ParseReason.DisplayMemberList
			 *	Ctrl-Shift-Пробел	Сведения о параметрах		ParseReason.MethodTip
			 *	Ctrl-K, Ctrl-I		Краткие сведения			ParseReason.QuickInfo
			 *	Ctrl-Пробел			Завершить слово				ParseReason.CompleteWord
			 *	F12					Перейти						ParseReason.Goto
			 *	А поскольку раскрашивание/токенизация тут не вызывалась, то и информации о токене где команда вызывалась НЕТ
			 *	Есть только request_.Text - полный текст файла и Line+Col (ну видимо где-то есть еще и про Selection какой-нибудь TextSpan)
			 *	Можно было бы через request_.View и LanguageService.GetSource([string fname] | [IVsTextLines buffer] | [IVsTextView view])
			 *	добраться до собственно буфера, получить и текст строки из Line+Col и IVsTextColorState::GetColorStateAtStartOfLine и т.п.
			 *	НО!!! НЕСКОЛЬКО РАЗ видел предупреждения, что вызывать эти методы из BackgruondThread нельзя - опасно...
			 *	А типичная реализация от МС вызывает эти запросы на парсинг именно Backround.
			 *	т.е., из хелпа для request_.Text: "Фактически, это единственный способ, которым синтаксический анализатор должен получить доступ к анализируемому источнику."
			 *	Смысл в том, что IVsTextView это фактически интерфейс Окна и о нем написано что:
			 *		"IVsTextView — это, по сути, окно редактора, отображаемое в пользовательском интерфейсе (UI)."
			 *		"Этот интерфейс не является потокобезопасным. Вы не должны вызывать что-либо в этом интерфейсе ни из чего, кроме основного потока пользовательского интерфейса."
			 *		!И еще можно! - "Вызовите метод GetWindowHandle и используйте его PostMessageдля отправки сообщений в основной поток."
			 *						т.е. даже из Бакграунда с ним можно работать как с обычным Window окном (если Оно умеет в стандартные WM_MESSAGE)
			 *	НО!!!	Суть в том, что это именно "типичная реализация" LanguageService.CreateParseRequest(... view IVsTextView)
			 *			создает ParseRequest именно в таком виде и уже ПОТОМ инициируется Background Parsing
			 *			В LanguageService.CreateParseRequest только написано, что 
			 *				"Объект IVsTextView, передаваемый этому методу, предназначен только для внутреннего использования и не должен быть доступен анализатору метода ParseSource . 
			 *				Этот синтаксический анализатор должен обращаться к источнику только через текст, передаваемый через объект ParseRequest , созданный этим методом."
			 *			А в самом методе LanguageService.CreateParseRequest я вполне могу создать СвойParseRequest и добавить ему какие либо свойства
			 *	
			 *	
			 *		//!! This property is for internal use only. It cannot be accessed from a background thread.
					//!! https://learn.microsoft.com/en-us/dotnet/api/microsoft.visualstudio.package.parserequest.view?view=visualstudiosdk-2022#microsoft-visualstudio-package-parserequest-view
					var view = request_.View;
					//TODO	LanguageService.GetSource([string fname] | [IVsTextLines buffer] | [IVsTextView view])
					Microsoft.VisualStudio.TextManager.Interop.IVsTextLines text_lines_buffer = null;
					int hr = view.GetBuffer(out text_lines_buffer);
					Microsoft.Verify.HResult(hr);
					Microsoft.VisualStudio.TextManager.Interop.IVsTextColorState colorState = (Microsoft.VisualStudio.TextManager.Interop.IVsTextColorState)text_lines_buffer;
					int line_state = -1;
					int hr_line_state = colorState.GetColorStateAtStartOfLine(request_.Line, out line_state);

			 */
			return (EslSource)this.GetSource(request_.View);
		}

		public override Source CreateSource(IVsTextLines buffer_) 
		{
			return new EslSource(this, buffer_, this.GetColorizer(buffer_));    // Код прямо из под dotPeek EslLanguageService::CreateSource(IVsTextLines), только свое создаем
		}
		public override ParseRequest CreateParseRequest(Source source_, int line_, int col_, TokenInfo token_, string source_text_, string source_file_, 
																															ParseReason reason_, IVsTextView source_view_) 
		{ 
			bool parse_synchronous = false;
			if (!this.Preferences.EnableAsyncCompletion)
				parse_synchronous = true;
			return new EslParseRequest(line_, col_, token_, source_text_, source_file_, reason_, source_view_, source_.CreateAuthoringSink(reason_, line_, col_), parse_synchronous);
		}

		public override AuthoringScope ParseSource(ParseRequest request_) {
			Microsoft.VisualStudio.Shell.ThreadHelper.ThrowIfNotOnUIThread();
			return EslParse.DoParse(this, request_);
		}

	} // EslLanguageService


#pragma warning disable 649
		internal static class EslDefinitions
		{		
			//https://blog.280z28.org/archives/2013/04/165/
			/* The language name (used for the language service) and content type must be the same
			 * due to the way Visual Studio internally registers file extensions and content types.
			 */
			const string ContentTypeName_ = Esl.DefConstants.LanguageName_;
			[Export]
			[Name(ContentTypeName_)]
			[BaseDefinition("code")]
			/*	[BaseDefinition("projection")]
			 *	Типы контента могут быть основаны на нулях или более существующих типах контента. Это встроенные типы:
			 *	...Code: for code of all kinds. Inherits from "text".
			 *	https://learn.microsoft.com/en-us/visualstudio/extensibility/language-service-and-editor-extension-points?view=vs-2022
			 */
			internal static readonly ContentTypeDefinition EslContentType_ = null;

			[Export]
			[FileExtension(Esl.DefConstants.FileExtension1_)]
			[ContentType(ContentTypeName_)]
			internal static readonly FileExtensionToContentTypeDefinition EslFileExtensionDefinition1_ = null;

			[Export]
			[FileExtension(Esl.DefConstants.FileExtension2_)]
			[ContentType(ContentTypeName_)]
			internal static readonly FileExtensionToContentTypeDefinition EslFileExtensionDefinition2_ = null;

			//TODO	Надо что-то придумать с переключением en-ru.. Видимо в настройках Сервиса должен быть Default, и в настройка проекта Default
			//		а при открытии файла (и вне проекта и в проекте) перепроверка и назначение для Source свойства опять же Default
			//		НО при показе Инфы и Типа учет имени метода ru/en и переключения на лету... 
			//		Пока все буде Ру..
			internal static readonly string TypePrefixToken_			= "Как";
			internal static readonly string TypePrefixWithSpace_		= TypePrefixToken_ + " ";
			//TODO	В окне подсказки параметры показывает через запятую, но без пробела. Возможно настройки просто нужны
			internal static readonly string ParamsDelimiterWithSpace_	= ", ";
			internal static readonly char	ParamsListBeginChar = '(';
			internal static readonly string ParamsListBegin = ParamsListBeginChar.ToString();
			internal static readonly char	ParamsListFinalChar = ')';
			internal static readonly string ParamsListFinal = ParamsListFinalChar.ToString();

			internal static readonly char IndexListBeginChar = '[';
			internal static readonly char IndexListFinalChar = ']';

			//
			internal static readonly string MethKindFunctionText			= "Функция";
			internal static readonly string MethKindFunctionTextWithSpace	= MethKindFunctionText + " ";
			internal static readonly string MethKindProcedureText			= "Процедура";
			internal static readonly string MethKindProcedureTextWithSpace	= MethKindProcedureText + " ";
			internal static readonly string MethKindOperatorText			= "Оператор";
			internal static readonly string MethKindOperatorTextWithSpace	= MethKindOperatorText + " ";
			internal static readonly string MethKindPropertyText			= "Свойство";
			internal static readonly string MethKindPropertyTextWithSpace	= MethKindPropertyText + " ";


		// вот это вот [ContentType(ContentTypeName_)] нужно будет добавлять во все экспортируемые классы кому это требуется
	}
#pragma warning restore 649
}
