using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
#if DEBUG || DEBUG_IN_RELEASE
	using System.Diagnostics;
#endif

using Microsoft.VisualStudio;
using Microsoft.VisualStudio.Package;
using Microsoft.VisualStudio.Shell;
using Microsoft.VisualStudio.Shell.Interop;
using Microsoft.VisualStudio.TextManager.Interop;

using System.Collections.Specialized;   //X_AuthoringSink
using System.Collections;               //X_AuthoringSink


// Наша EsbStudio преимущественно синхронна и асинхронную функциональность НЕ ИСПОЛЬЗУЕТ
// Принципиально. Принципиально считаю, что помогать кодеру нужно синхронно, а ждут пусть другие
// Возможно когда-то эта модель будет изменена - тогда нужно будет разобираться в местах помеченных этим макросом
// (VSTHRD010 - это ругань по поводу возможного обращения к EnvDTE-элементам из НЕ основного потока
//	с тревованием Microsoft.VisualStudio.Shell.ThreadHelper.ThrowIfNotOnUIThread(); или еще как)
#if ESB_STUDIO_SYNC_MODE
#pragma warning disable VSTHRD010
#endif


//TODO	нужно присмотреться к логу - там ош какие-то
//C:\Users\Garry\AppData\Roaming\Microsoft\VisualStudio\16.0_87a5a19cExp\ActivityLog.xml


namespace EsbStudio.Esl
{
	public static class TextSpanExt
	{
		public static readonly TextSpan TextSpanEmpty_;
		public static TextSpan MakeTextSpan(int line_, int col_, int end_line_, int end_col_) {
			TextSpan sp;
			sp.iStartLine = line_;
			sp.iStartIndex = col_;
			sp.iEndLine = end_line_;
			sp.iEndIndex = end_col_;
			return sp;
		}
	}


	public static class Extensions 
	{
		public static int GetTokenTextLen(this TokenInfo token_info_) {
			return (token_info_.EndIndex - token_info_.StartIndex + 1);
		}
		public static string GetTokenTextFrom(this TokenInfo token_info_, string line_text_) {
			return line_text_.Substring(token_info_.StartIndex, token_info_.GetTokenTextLen());
		}
		public static string GetTokenTextFromLineUpToCol(this TokenInfo token_info_, string line_text_, int end_col_) {
			//(token_info_.EndIndex - token_info_.StartIndex + 1);
			if (end_col_ < token_info_.StartIndex || end_col_ > token_info_.EndIndex)
				return null;	//там нашего токена еще или уже нет. а с нулл пусть сами разбираются - неча ересь подсовывать

			int token_part_len = end_col_ - token_info_.StartIndex + 1;
			return line_text_.Substring(token_info_.StartIndex, token_part_len);
		}
		public static bool IsTriggerMemberSelect(this TokenInfo token_info_) {
			return ((token_info_.Trigger & TokenTriggers.MemberSelect) == TokenTriggers.MemberSelect);
		}
		public static bool IsTriggerMethodTip(this TokenInfo token_info_) {
			// MethodTip это маска для триггеров ParameterStart, ParameterNext, ParameterEnd, Parameter
			return ((token_info_.Trigger & TokenTriggers.MethodTip) != 0);
		}
		public static bool TypeIsSomeCodeText(this TokenInfo token_info_) {
			//TODO	нужно окончательно разобраться в TokenType.Unknown - или использувать его как-то или запретить где-нибудь ассертом
			//		(как минимум для парсера, вообще-то конечно он может иметь спец. смысл.)
			//		не заперщать его, а просто не включать ни в какую группу .Is...
			return (token_info_.Type == TokenType.Text || token_info_.Type == TokenType.Identifier || token_info_.Type == TokenType.Keyword); ;
		}


		public static TextSpan GetTextSpanFor(this TokenInfo token_info_, int line_) {
			return make_TextSpan(line_, token_info_.StartIndex, line_, token_info_.EndIndex);
		}

		public static TextSpan new_TextSpan(int line_, int col_, int end_line_, int end_col_) {
			return new TextSpan { iStartLine = line_, iStartIndex = col_, iEndLine = end_line_, iEndIndex = end_col_ };
		}
		public static TextSpan make_TextSpan(int line_, int col_, int end_line_, int end_col_) {
			TextSpan sp;
			sp.iStartLine = line_;
			sp.iStartIndex = col_;
			sp.iEndLine = end_line_;
			sp.iEndIndex = end_col_;
			return sp;
		}
	} // static class Extensions 
} // namespace EsbStudio.Esl



namespace EsbStudio.Esl
{
	internal partial class EslParse
	{

	} // class EslParser
} // namespace EsbStudio.Esl



/* ХУЙНЯ КАКАЯ-ТО ПОЛЕЗНАЯ
		 *     internal enum BackgroundRequestReason
    {
        MemberSelect, // ".", also triggered by some some other tokens like ".."
        MemberSelectAndHighlightBraces,		// unused? No F# tokens have both MatchBraces and MemberSelect
        MatchBracesAndMethodTip,			// close-paren
        MatchBraces,						// moving cursor etc.
        FullTypeCheck,						// triggered on idle
        CompleteWord,						// Ctrl-space
        DisplayMemberList,					// Ctrl-J
        QuickInfo,							// mouse hover
        MethodTip,							// open-paren, close-paren or comma
        Goto, // F12
        /// <summary>
        /// This reason is used when we want to trigger only 'parsing' (without type-checking) to
        /// update the untyped AST information (e.g. when a different file is opened). After updating
        /// the untyped scope (in F# LS), a 'null' can be returned as the result of 'ExecuteBackgroundRequest'.
        /// </summary>
        ParseFile

    internal abstract class IntellisenseInfo_DEPRECATED
    {
        internal abstract System.Tuple<string,TextSpan> GetDataTipText(int line, int col);
        internal abstract Microsoft.FSharp.Control.FSharpAsync<Declarations_DEPRECATED> GetDeclarations(ITextSnapshot textSnapshot, int line, int col, BackgroundRequestReason reason);
        internal abstract Microsoft.FSharp.Core.FSharpOption<MethodListForAMethodTip_DEPRECATED> GetMethodListForAMethodTip();
        internal abstract GotoDefinitionResult_DEPRECATED Goto(IVsTextView textView, int line, int col);
        internal abstract void GetF1KeywordString(TextSpan span, IVsUserContext context);
    }
*/


namespace EsbStudio.Esl
{
	internal partial class EslParse
	{
		public static AuthoringScope ParseResultNull_ = new EslParserResultForScope_Null();

		public static AuthoringScope MakeResult_Unsupported(ParseRequest request_) {
			Debug.Assert(false);
			throw Errors.ArgumentException("EslParser", nameof(request_.Reason), request_.Reason);
		}
		public static AuthoringScope ParseResultUnexpected(ParseRequest request_) {
			Debug.Assert(false);
			throw Errors.ArgumentException("EslParse Result Unexpected for:", nameof(request_.Reason), request_.Reason);
		}
		public static AuthoringScope ParseResultNotImplemented(ParseReason reason_) {
			Debug.WriteLine($"EslParser::DoParse not implemented for Reason = {reason_}");
			return ParseResultNull_;
		}

		//TODO	Мы должны возвращать наш EslResult c текстом ошибки, а дальше ее либо как-то показать, либо записать, либо throw сделать
		public static AuthoringScope ParseResultError(ParseRequest request_, string error_text_) {
			Debug.WriteLine($"EslParse result for reason '{request_.Reason}' error: '{error_text_}'");	
			return ParseResultNull_;
		}
		public static AuthoringScope ParseResultAssert(ParseRequest request_, string error_text_) {
			Debug.WriteLine($"EslParse result for reason '{request_.Reason}' assert: '{error_text_}'");
			return ParseResultNull_;
		}

	} // class EslParser
} // namespace EsbStudio.Esl




namespace EsbStudio.Esl
{
	internal partial class EslParse
	{

		public static AuthoringScope DoParse_OnGoto(EslSource source_, EslParseRequest request_) 
		{
			/*	Юзер ткнул в какую-то точку на экране и сказал Перейди!...
		     *	Parse the identifier or expression at the specified location to obtain a possible URI of a file where the identifier is defined, declared, or referenced.
		     *	Нужно вернуть такой AuthoringScope, чтобы он мог ответить в методе 
		     *		public abstract string Goto (.VSStd97CmdID cmd, .IVsTextView textView, int line, int col, out .TextSpan span);
		     *	https://learn.microsoft.com/en-us/dotnet/api/microsoft.visualstudio.package.authoringscope.goto?view=visualstudiosdk-2022
		     *	Вызов сюда приходит из..
		     *	EslLanguage.dll!EsbStudio.Esl.EslParse.DoParse_OnGoto(EsbStudio.Esl.EslLanguageService service_, string source_file_content_, int line_, int col_) Строка 438	C#
 				EslLanguage.dll!EsbStudio.Esl.EslParse.DoParse(EsbStudio.Esl.EslLanguageService service_, EsbStudio.Esl.EslParseRequest request_) Строка 504	C#
 				EslLanguage.dll!EsbStudio.Esl.EslParse.DoParse(EsbStudio.Esl.EslLanguageService service_, Microsoft.VisualStudio.Package.ParseRequest request_) Строка 451	C#
 				EslLanguage.dll!EsbStudio.Esl.EslLanguageService.ParseSource(Microsoft.VisualStudio.Package.ParseRequest request_) Строка 103	C#
 				Microsoft.VisualStudio.Package.LanguageService.15.0.dll!Microsoft.VisualStudio.Package.LanguageService.ParseRequest(Microsoft.VisualStudio.Package.ParseRequest req)	Нет данных
 				Microsoft.VisualStudio.Package.LanguageService.15.0.dll!Microsoft.VisualStudio.Package.Source.BeginParse(int line, int idx, Microsoft.VisualStudio.Package.TokenInfo info, Microsoft.VisualStudio.Package.ParseReason reason, Microsoft.VisualStudio.TextManager.Interop.IVsTextView view, Microsoft.VisualStudio.Package.ParseResultHandler callback)	Нет данных
 				Microsoft.VisualStudio.Package.LanguageService.15.0.dll!Microsoft.VisualStudio.Package.ViewFilter.HandleGoto(Microsoft.VisualStudio.VSConstants.VSStd97CmdID cmd)	Нет данных
 				Microsoft.VisualStudio.Package.LanguageService.15.0.dll!Microsoft.VisualStudio.Package.ViewFilter.HandlePreExec(ref System.Guid guidCmdGroup, uint nCmdId, uint nCmdexecopt, System.IntPtr pvaIn, System.IntPtr pvaOut)	Нет данных
 				Microsoft.VisualStudio.Package.LanguageService.15.0.dll!Microsoft.VisualStudio.Package.ViewFilter.ExecCommand(ref System.Guid guidCmdGroup, uint nCmdId, uint nCmdexecopt, System.IntPtr pvaIn, System.IntPtr pvaOut)	Нет данных
				...
			*/
			return ParseResultNotImplemented(ParseReason.Goto);
		}





		public static AuthoringScope DoParse(EslLanguageService service_, ParseRequest request_) 
		{
			EslParseRequest esl_parse_request = request_ as EslParseRequest;
			if (esl_parse_request == null)
				return ParseResultUnexpected(request_);
			else
				return DoParse(service_, esl_parse_request);
		}
		public static AuthoringScope DoParse(EslLanguageService service_, EslParseRequest request_) 
		{
			Debug.WriteLine($"DoParse Reason: {request_.Reason}, IsSync: {request_.IsSynchronous}");

			/* Несмотря на то, что структура ParseRequest содержит объект IVsTextView , вы не можете использовать этот объект в фоновом потоке из-за проблем с потоками.
			* Фактически, вы не можете использовать в фоновом потоке классы Source , ViewFilter , CodeWindowManager или любой класс, который прямо или косвенно
			* взаимодействует с представлением.
			* Анализатор метода ParseSource должен получить всю информацию об источнике из текста, предоставленного в структуре ParseRequest
			* (которая всегда содержит весь исходный файл).
			* https://learn.microsoft.com/en-us/dotnet/api/microsoft.visualstudio.package.languageservice.parsesource?view=visualstudiosdk-2022#microsoft-visualstudio-package-languageservice-parsesource(microsoft-visualstudio-package-parserequest)
			*/

			/* Имейте в виду, что операция анализа может работать со всем исходным файлом, с одной строкой или даже с одним токеном.
			* Код ParseReason в ParseRequest определяет область действия операции анализа.
			* Возвращенный объект AuthoringScope содержит всю информацию, проанализированную из исходного файла.
			* ПРИМЕЧАНИЕ
			* AuthoringScope — это абстрактный класс, и вы должны получить от него свой собственный класс, чтобы обеспечить необходимую функциональность.
			* Ваша версия AuthoringScope создается в этом методе ParseSource. Вы должны вернуть объект AuthoringScope ,
			* даже если все члены этого класса возвращают нулевые значения!
			*/
			/* The parsing process must be able to parse a single token, a single line, a group of lines, or the entire text, all based on the ParseReason value
			* supplied by the ParseRequest object.
			* https://learn.microsoft.com/ru-ru/dotnet/api/microsoft.visualstudio.package.languageservice.createparserequest?view=visualstudiosdk-2022#microsoft-visualstudio-package-languageservice-createparserequest(microsoft-visualstudio-package-source-system-int32-system-int32-microsoft-visualstudio-package-tokeninfo-system-string-system-string-microsoft-visualstudio-package-parsereason-microsoft-visualstudio-textmanager-interop-ivstextview)
			*/

			//???
			////https://learn.microsoft.com/en-us/visualstudio/extensibility/editor-imports?view=vs-2022

			//НУЖНО ЕЩЕ		https://learn.microsoft.com/ru-ru/dotnet/api/microsoft.visualstudio.package.documentproperties?view=visualstudiosdk-2022


			EslSource source_of_request = service_.GetSourceOfRequest(request_);

			//НАЧИНАЕМ ОТСЮДА	https://learn.microsoft.com/en-us/previous-versions/visualstudio/visual-studio-2017/extensibility/internals/custom-document-properties-in-a-legacy-language-service?view=vs-2017

			if (request_.Reason == ParseReason.None)
			{   //*	None:	Значение-заполнитель, указывающее, что синтаксический анализ выполнять не следует.
				//TODO че тут делать непонятно.. вообще еще не видел чтобы оно сработало..
				return MakeResult_Unsupported(request_);
			}
			else if (request_.Reason == ParseReason.Check)
			{   //*		Check:	Parse the entire source file, checking for errors. This pass should also create lists of matching language pairs, triplets, members, and methods.
				//	Parse the entire source as given in req.Text. Store results in the MyAuthoringScope object.
				/* То есть тут нужно (можно) отпарсить весь файл и заполнить AuthoringSink.AddError 
				 * и прочее (МНОГО в Синк!) https://learn.microsoft.com/en-us/dotnet/api/microsoft.visualstudio.package.authoringsink?view=visualstudiosdk-2022
				 * https://stackoverflow.com/questions/11654744/visual-studio-custom-language-service
				 * нужно учитыват
				*/
				return MakeResult_Unsupported(request_);
			}
			// Эти методы требуют парсинга всего файла
			else if (request_.Reason == ParseReason.DisplayMemberList)	{ return DoParse_OnDisplayMemberList(source_of_request, request_);	}
			
			else if (request_.Reason == ParseReason.Goto)				{ return DoParse_OnGoto(source_of_request, request_);	}
			else if (request_.Reason == ParseReason.QuickInfo)			{ return DoParse_OnQuickInfo(source_of_request, request_);	}

			// Это вызывается когда вводишь '(' и ')' - TokenTriggers.ParameterStart | TokenTriggers.ParameterEnd
			else if (request_.Reason == ParseReason.MethodTip)			{ return DoParse_OnMethodTip(source_of_request, request_, false);}
			// Это вызывается то же самое, но когда из меню "сведения о параметрах" или Ctrl-Shift-Пробел
			else if (request_.Reason == ParseReason.Autos)				{
				//Autos		Parse the code block at the given location to obtain any expressions that might be of interest in the Autos debugging window
				//(an expression is the name of variable or parameter that can be evaluated to produce a value).
				//
				//НО и безо всякого дебага .Autos вызывается из меню как Сведение О Параметрах
				return DoParse_OnMethodTip(source_of_request, request_, true);
			}

			// И ParseReason.CompleteWord и ParseReason.MemberSelect (и ParseReason.MemberSelectAndHighlightBraces) вызываются из Source::Completition
			// только .CompleteWord инициируется студией из .ViewFilter, а .MemberSelect иницируем мы по триггеру
			// но в конечном итоге они обрабатываются Source::CompletitionSet им нужны AuthoringScope::Declarations и на экране показывается СписокМетодов
			else if (request_.Reason == ParseReason.CompleteWord)		{ return DoParse_OnCompleteWord(source_of_request, request_); }
			else if (request_.Reason == ParseReason.MemberSelect)
			{
				//https://learn.microsoft.com/en-us/previous-versions/visualstudio/visual-studio-2017/extensibility/internals/member-completion-in-a-legacy-language-service?view=vs-2017
				return DoParse_OnMemberSelect(source_of_request, request_);
			}
			else if (request_.Reason == ParseReason.HighlightBraces)
			{   /* Parse to find the matching language pairs (such as "{" and "}" or "<" and ">") that enclose the given location so they and their contents can be highlighted.
			 	 */
				return ParseResultNotImplemented(request_.Reason);
			}
			else if (request_.Reason == ParseReason.MatchBraces)
			{   /* Parse the language pair at the given location to finds its match.
			 	 */
				return ParseResultNotImplemented(request_.Reason);
			}
			else if (request_.Reason == ParseReason.MemberSelectAndHighlightBraces)
			{   /* Parse the character at the current location to complete a member selection and to highlight the matching pair to the parsed character (such as a ")" after a method name).
			 	 */
				//https://learn.microsoft.com/en-us/previous-versions/visualstudio/visual-studio-2017/extensibility/internals/member-completion-in-a-legacy-language-service?view=vs-2017
				return ParseResultNotImplemented(request_.Reason);
			}
			else
			{   /*  CodeSpan		Parse the section of code containing the specified location to find the extent of the statement. Used in validating breakpoints.
			     *  ...
				 */
				return ParseResultNotImplemented(request_.Reason);
			}
			//		
		} // DoParseScope
	} // class EslParser





	
	internal class EslParserResultForScopeQuickInfo : AuthoringScope
	{
		private string m_info;
		public EslParserResultForScopeQuickInfo(string info_) {
			this.m_info = info_;
		}
		public override string GetDataTipText(int line, int col, out Microsoft.VisualStudio.TextManager.Interop.TextSpan out_text_span_tip_show_) {
			//out_text_span_tip_show_ = Util.SpanEmpty_;
			out_text_span_tip_show_.iStartLine = line;
			out_text_span_tip_show_.iStartIndex = col;
			out_text_span_tip_show_.iEndLine = line;
			out_text_span_tip_show_.iEndIndex = col + 5;
			return m_info;
		}
		public override Declarations GetDeclarations(Microsoft.VisualStudio.TextManager.Interop.IVsTextView view, int line, int col, TokenInfo info, ParseReason reason) {
			return null;
		}
		public override Methods GetMethods(int line, int col, string name) {
			return null;
		}
		public override string Goto(Microsoft.VisualStudio.VSConstants.VSStd97CmdID cmd, Microsoft.VisualStudio.TextManager.Interop.IVsTextView textView,
																				int line, int col, out Microsoft.VisualStudio.TextManager.Interop.TextSpan out_text_uri_target_) {
			out_text_uri_target_ = Util.SpanEmpty_;
			return null;
		}
	}

	internal class EslParserResultForScope: AuthoringScope
	{
		private Declarations	m_declarations = null;
		private Methods			m_methods = null;
		private string			m_info;
		public EslParserResultForScope(Declarations declarations_, Methods methods_, string info_)
		{
			this.m_declarations = declarations_;
			this.m_methods = methods_;
			this.m_info = info_;
		}
		public override string GetDataTipText(int line, int col, out Microsoft.VisualStudio.TextManager.Interop.TextSpan out_text_span_tip_show_) {
			//out_text_span_tip_show_ = Util.SpanEmpty_;
			out_text_span_tip_show_.iStartLine = line;
			out_text_span_tip_show_.iStartIndex = col;
			out_text_span_tip_show_.iEndLine = line;
			out_text_span_tip_show_.iEndIndex = col + 5;
			return m_info;
		}
		public override Declarations GetDeclarations(Microsoft.VisualStudio.TextManager.Interop.IVsTextView view, int line, int col, TokenInfo info, ParseReason reason) {
			return m_declarations;
		}
		public override Methods GetMethods(int line, int col, string name) {
			return m_methods;
		}
		public override string Goto(Microsoft.VisualStudio.VSConstants.VSStd97CmdID cmd, Microsoft.VisualStudio.TextManager.Interop.IVsTextView textView,
																				int line, int col, out Microsoft.VisualStudio.TextManager.Interop.TextSpan out_text_uri_target_) {
			out_text_uri_target_ = Util.SpanEmpty_;
			return null;
		}
	}


} // namespace EsbStudio.Esl
