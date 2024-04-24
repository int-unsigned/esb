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
using Microsoft.VisualStudio.TextManager.Interop;
//
using System.Collections.Specialized;
using System.Collections;


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
	internal partial class EslParse
	{

		//*		MethodTip: Parse the method name before the current position to produce a list of all overloaded method signatures that match the method name.
		// Parse the line specified in req.Line for the token just before req.Col to obtain the name of the method.
		// Find all method signatures with the same name in the existing parse tree and store the list of signatures in the Methods class.
		//1. Эта хрень вызывается и при закрывающей скобке (но тип не показывает, а скрывает видимо вычисляя TextSpan)
		//		т.е. при ')' можно было-бы не парсить а сразу вернуть Пусто НО это может быть внутренниим выражением и нужно вернуть Тип для внешней функции
		//2. У скоупа вызывается только GetMethods, все прочее (GetDataTipText, GetDeclarations, Goto) НЕ вызываются

		public static AuthoringScope DoParse_OnMethodTip(EslSource source_, EslParseRequest request_, bool request_autos_) 
		{
			string			s_request_line_text = source_.GetLine(request_.Line);						
			EslParseSink	request_sink = request_.GetEslParseSink();
			// нас должны вызывать по триггеру из EslSource::OnCommand. GetLineTokensLastQueried_TODO должно быть валидным
			//TODO	На самом деле нас вызывают НЕ ТОЛЬКО по триггеру, а когда окно подсказки активно, то и ПРИ ОЧЕРЕДНОМ КАЖДОМ символе
			//		видимо логика в том, что пытаясь распознавать дополненный токен параметра, мы можем также уточнять подсказку
			//		к примеру если метод перегружен по типам данных, то сначала мы не понимаем какой параметр вводится и показываем все возможные
			//		варианты метода, но когда параметр введен полностью мы можем его распознать, определить его тип и сузить набор возможных методов.
			//		Для нас это означает, что есть два режима - 1) режим инициализации и 2) режим продолжения
			//		Поскольку пока мы не умеем определять тип параметра, то в режиме продолжения мы бесполезны
			//		НО! Мы не можем вернуть .ParseResultNull_, т.к. окно подсказки при этом просто закроется - this.DismissCompletor();
			//		Их .HandleMethodTipResponse устроен так, что ему нельзя сказать "просто не трогай окно - оставь как есть".
			//		нужно что-то думать.. то-ли кэшировать предыдущий результат, то ли флаг какой в EslParseRequest сделать..
			//	ПОКА ПРОСТО отключу эту функциональность (режим продолжения) в EslSource::OnCommand - не будем вызывать .MethodTip без триггера
			TokenInfo request_token = request_.TokenInfo;
			// в дебаге ассерт оставлю для контроля, ну а в релизе пусть будет что будет.. 
			Debug.Assert(request_token != null && request_token.IsTriggerMethodTip());
			
			TokenInfo[] line_tokens = source_.GetLineTokensLastQueried_TODO(request_);
			Debug.Assert(line_tokens != null && line_tokens.Length > 0);
			if (line_tokens == null || line_tokens.Length == 0)
				return ParseResultAssert(request_, "line_tokens == null || line_tokens.Length == 0");

			int current_call_para_index = EslParseApi.ParseTokensAndFillSink(request_sink, request_.TokenInfo, request_.Line, s_request_line_text, line_tokens);

			var current_call = request_sink.MethodCalls_.GetCurrentMethodCall();
			if (current_call == null)
				return ParseResultError(request_, "Выражение для информирования о параметрах <null>.");

			StringCollection current_call_names = current_call.names;
			if(current_call_names == null)
				return ParseResultError(request_, "В выражении для информирования о параметрах имена <null>.");

			int current_call_names_count = current_call_names.Count;
			if (current_call_names_count == 0)
				return ParseResultError(request_, "В выражении для информирования о параметрах имена отсутствуют.");

			return new EslParserResultForScope(null, EslEngineUI.GetNameMethodsForMethodsTip(source_, request_.Line, request_.Col, current_call_names, current_call_para_index), null);
		}



		//		DisplayMemberList:	Parse the separator and the possible name before it, to obtain a list of members to be shown in a member completion list.
		// Parse the line specified in req.Line for the two tokens just before req.Col to get the identifier and the member connector symbol.
		// Find members of the identifer in the parse tree and store the list of members in the Declarations class.

		public static AuthoringScope DoParse_OnDisplayMemberList(EslSource source_, EslParseRequest request_) 
		{
			/*	Юзер ткнул в какую-то точку на экране и сказал показать список членов... - Ctrl-J
			 *	то есть это не триггер после '.', а именно любая точка на экране
		     *	этот запрос приходит к нам из View, а не из нашего EslSource
		     *	мы должны определить токен из request_.Line, request_.Col
		     *	сразу - это должен быть либо .Text либо .Ident, либо .Keyword т.к. мы только приблизительно понимаем код
		     *	затем - мы должны пропарсить строку до и включая этот токен
		     *	и если до и включая этот токен мы получили квалифицированное имя (т.е. два и более имени)
		     *	то можем заказать список мемберов для "имени-минус-один" - предыдущего
		     *	(при этом сам токен мы как имя игнорируем - он вообще может быть кусочком имени и юзер имел ввиду "дополни", посмотрим,
		     *	 надеюсь СамСписок должен справиться, хотя хз.. TODO)
		 	 */

			// request_.Col - это позиция в которой стоит курсор у юзера. соответственно то для чего он хочет список членов
			// заканчивается в ПРЕДЫДУЩЕЙ позиции.
			// В ПЕРДЫДУЩЕЙ позиции может быть просто '.', тогда чистый режим .OnMemberSelect
			//TODO	Копипаста из комплете-ворд. начало одинаковое. свести к общему знаменателю!
			if (request_.Col <= 0)
				return EslParse.ParseResultNull_; // от нулевой позиции нечего дополнять
			int				request_complete_col = request_.Col - 1;
			EslParseSink	request_sink = request_.GetEslParseSink();
			TokenInfo[]		line_tokens = source_.GetLineTokens(request_.Line);
			if (line_tokens == null)
				return EslParse.ParseResultNull_;   // для абсолютно пустой строки не показываем. пусть хоть что-то введет
			TokenInfo		request_token = new TokenInfo();
			int				request_token_index = source_.GetTokenInfoAt(line_tokens, request_complete_col, ref request_token);
			if (request_token_index < 0)
				return EslParse.ParseResultNull_;   // то же самое, что и пусто. нам хоть кусочек имени нужен
			string			request_line_text = source_.GetLine(request_.Line);

			if (request_token.IsTriggerMemberSelect())
				return DoParse_OnMemberSelect(source_, request_, request_sink, request_line_text, line_tokens);
			if(!request_token.TypeIsSomeCodeText())
				return EslParse.ParseResultNull_;

			int current_call_para_index = EslParseApi.ParseTokensAndFillSink(request_sink, request_token, request_.Line, request_line_text, line_tokens);
			StringCollection current_call_names = request_sink.Names_;
			if (current_call_names == null || current_call_names.Count < 2) //!! < 2, только два и больше - для одного показывать нечего
				return EslParse.ParseResultNull_;

			Declarations current_call_members_decl = EslEngineUI.GetMembers(source_, request_.Line, request_.Col, current_call_names, current_call_names.Count - 1);
			if (current_call_members_decl == null)
				return EslParse.ParseResultNull_;

			return new EslParserResultForScope(current_call_members_decl, null, null);
		}

		public static AuthoringScope DoParse_OnMemberSelect(EslSource source_, EslParseRequest request_) 
		{   
			/*	Введена точка '.' Нужно показать список членов.
		     *	(ПС. Именно введена, когда из меню (или горячей клавишей вызвали Ctrl-J) вызвали, то отработает DoParse_OnDisplayMemberList)
			 */
			TokenInfo		request_token = request_.TokenInfo;
			// Нас должны были вызывать по триггеру из EslSource::OnCommand. поэтому request_token должен быть валидным и с триггером и GetLineTokensLastQueried_TODO
			// должна вернуть валидный набор токенов этой строки
			Debug.Assert(request_token != null && request_token.IsTriggerMemberSelect());
			if(request_token == null || !request_token.IsTriggerMemberSelect())
				return ParseResultAssert(request_, "request_token == null || !request_token.IsTriggerMemberSelect()");
			
			TokenInfo[] line_tokens = source_.GetLineTokensLastQueried_TODO(request_);			
			Debug.Assert(line_tokens != null && line_tokens.Length > 0);
			if(line_tokens == null || line_tokens.Length == 0)
				return ParseResultAssert(request_, "line_tokens == null || line_tokens.Length == 0");

			EslParseSink	request_sink = request_.GetEslParseSink();
			string			s_request_line_text = source_.GetLine(request_.Line);

			return DoParse_OnMemberSelect(source_, request_, request_sink, s_request_line_text, line_tokens);
		}
		public static AuthoringScope DoParse_OnMemberSelect(EslSource source_, EslParseRequest request_, EslParseSink request_sink_, string request_line_text_, TokenInfo[] line_tokens_) 
		{
			int current_call_para_index = EslParseApi.ParseTokensAndFillSink(request_sink_, request_.TokenInfo, request_.Line, request_line_text_, line_tokens_);

			// в случае успеха в request_sink.Names_ будет восстановленное из стека имя предыдущего выражения для которого мы должны показать мемберы
			StringCollection current_call_names = request_sink_.Names_;
			if (current_call_names == null || current_call_names.Count == 0)
				return EslParse.ParseResultNull_;

			Declarations current_call_members_decl = EslEngineUI.GetMembers(source_, request_.Line, request_.Col, current_call_names, current_call_names.Count);
			if (current_call_members_decl == null)
				return EslParse.ParseResultNull_;

			return new EslParserResultForScope(current_call_members_decl, null, null);
		}

		public static AuthoringScope DoParse_OnCompleteWord(EslSource source_, EslParseRequest request_) 
		{   
			/*	Юзер ткнул в какую-то точку на экране и сказал Дополнить!...
		     *	Parse to get the partially completed word before the current position in order to show a list of possible completions (members, arguments, methods).
		     *
		     *		Вызов в Source::Completition приходит начиная из .Package.ViewFilter.ExecCommand
		     *	При уже показанном списке дополнительный вводимени к повторным вызовам НЕ ПРИВОДИТ - они работают с ранее отданными данными
		     *	(единственно где можно вмешаться это Siurce::OnCommand или оверрайдить)
			 */

			// Нам надо получить Имя для токена в позиции request_.Col
			// Это может быть вообще нулл (в начале строки), какой-то огрызок мысли,
			// или полноценный токен, который продолжается и после позиции Col и в этом случае нам надо только его кусочек до позиции Col

			// request_.Col - это позиция в которой стоит курсор у юзера. соответственно то что он хочет дополнить 
			// заканчивается в ПРЕДЫДУЩЕЙ позиции.
			if(request_.Col <= 0)
				return EslParse.ParseResultNull_; // от нулевой позиции нечего дополнять
			int request_complete_col = request_.Col - 1;

			EslParseSink	request_sink = request_.GetEslParseSink();
			TokenInfo[]		line_tokens = source_.GetLineTokens(request_.Line);
			if (line_tokens == null)
				return EslParse.ParseResultNull_;   // для абсолютно пустой строки не показываем. пусть хоть что-то введет
			TokenInfo		request_token = new TokenInfo();
			int				request_token_index = source_.GetTokenInfoAt(line_tokens, request_complete_col, ref request_token);
			if (request_token_index < 0)
				return EslParse.ParseResultNull_;   // то же самое, что и пусто. нам хоть кусочек имени нужен

			if(request_token.Type == TokenType.String || request_token.Type == TokenType.LineComment || 
				request_token.Type == TokenType.Operator || request_token.Type == TokenType.WhiteSpace)
				return EslParse.ParseResultNull_;   // для разобранных токенов, которые заведомо не идентификаторы тоже не показываем
			//TODO	TokenType.Comment - это в стиле с++ /**/ у нас такого нет и этот тип можно переиспользовать для чего-нибудь полезного. на заметку
			//TokenType.Delimiter - особый случай. это после '.' должен еще быть TokenTriggers.MemberSelect - это можно показать - обычно мемберов не много
			//TokenType.Keyword	- показываем, т.к. оно может только начинаться с текста кейворда, а юзер хочет что-то еще
			//TokenType.Unknown - хз.. попробуем если текст есть, то покажем. впрочем это для всех токенов так

			//TODO доделать переброс на MemberSelect
			//Debug.Assert(request_token.Trigger != TokenTriggers.MemberSelect);
			string request_line_text = source_.GetLine(request_.Line);

			if (request_token.IsTriggerMemberSelect())
				return DoParse_OnMemberSelect(source_, request_, request_sink, request_line_text, line_tokens);
			
			string request_token_text_part = request_token.GetTokenTextFromLineUpToCol(request_line_text, request_complete_col);
			if(request_token_text_part == null)
				return ParseResultError(request_, "request_token_text_part == null");
			if (request_token_text_part.Length == 0)
				return ParseResultError(request_, "request_token_text_part == empty");  // для валидного токена не должно быть такого. мы его из GetTokenInfoAt получили

			//TODO	Парсим мы до и включая сам request_token несмотря, что предполагаем что он кусочек какой-то (хотя может и быть полным)
			//		как-бы хочется полное выражение захватить в которое юзер ткнул. это "хочется" либо обосновать надо, либо алгоритм менять 
			//		и парсить только до предыдущего. А предыдущий может и не быть или быть опять хз чем... так надежнее как-то
			int current_call_para_index_unused = EslParseApi.ParseTokensAndFillSink(request_sink, request_token, request_.Line, request_line_text, line_tokens);

			StringCollection current_call_names = request_sink.Names_;
			if (current_call_names == null)
				return ParseResultError(request_, "current_call_names == null");

			int current_call_names_count = current_call_names.Count;
			if (current_call_names_count == 0)
				return ParseResultError(request_, "current_call_names_count == 0");

			string request_token_text = request_token.GetTokenTextFrom(request_line_text);
			//TODO Возможно в каких-то случаях это нормально и надо просто что-то подправить или тихо нулл вернуть. будем посмотреть..
			if(current_call_names[current_call_names_count-1] != request_token_text)
				return ParseResultError(request_, $"Unexpected current_call_last_name='{current_call_names[current_call_names_count - 1]}', request_token_text='{request_token_text}'");

			// Парсинг мы заказывали включая и последний токен, а он может быть больше, чем кусочек который юзер хочет дополнить. ограничим
			// (GetCompleteWord считает последний элемент имени кусочком. в том ее и смысл)
			current_call_names[current_call_names_count - 1] = request_token_text_part;
			Declarations match_declarations = EslEngineUI.GetCompleteWord(source_, request_.Line, request_complete_col, current_call_names);
			if (match_declarations == null)
				return EslParse.ParseResultNull_;

			return new EslParserResultForScope(match_declarations, null, null);
		}


		static EslParserResultForScope ParserResultQuickInfoText(string text_) {
			return new EslParserResultForScope(null, null, $"QuickInfo: '{text_}'");
		}
		/*	Юзер ткнул в какую-то точку на экране (или просто мышом провел) и сказал ДайИнфу!...
		 *	Parse the identifier or selection at the given location to obtain type information to be shown in an IntelliSense quick info tool tip.
		 */
		static int m__last_quickinfo_line = 0;
		static int m__last_quickinfo_col = 0;
		static int m__last_quickinfo_counter = 0;
		public static AuthoringScope DoParse_OnQuickInfo(EslSource source_, EslParseRequest request_) 
		{
			/* TODO	
			 * По какой-то непонятной причине из многопоточных недр студии приходит по два запроса подряд на QuickInfo, зараза!
			 * разобраться с этой проблемой быстро не представляется возможным, т.к. оно там из глубины приходит, зараза!
			 * Выглядит это как окошечко два раза появляется. Прикольно. Не смертельно.
			 * Отсечь простым методом второй запрос тоже не получается, т.к. возврат AuthoringScope-Null трактуется как отмена
			 * и только появившееся первое окошко вторым запросом сразу закрывается, ЗАРАЗА!
			 * БУДЕМ СЧИТАТЬ ФИЧЕЙ, до поры...
			 * ПС. попытки считать вызовы и отвечать только на первый или только на второй безуспешны. Если отвечаешь на первы нулл - просто не показывается
			 *		отвечаещь на второй нулл окошко блым - и закрывается..
			Debug.WriteLine($"DoParse_OnQuickInfo line={request_.Line}, col={request_.Col}");
			//Debug.Assert(request_.Line != m__last_quickinfo_line || request_.Col != m__last_quickinfo_col);
			if(request_.Line == m__last_quickinfo_line && request_.Col == m__last_quickinfo_col)
				return ParseResultNull_;
			m__last_quickinfo_line = request_.Line;
			m__last_quickinfo_col = request_.Col;
			*/

			EslParseSink request_sink = request_.GetEslParseSink();

			// Запрос на инфу должен относиться к идентификатору и должен закончиться на нем-же
			// а ПОСЛЕ мы должны просто посчитать сколько "наших" запятых для выяснения кол-ва прописанных парамтров
			// (если Имя - это ИмяДекларации и эта Декларация мульти-методная. )
			// request_.TokenInfo - у нас здесь пустой. нам нужно его получить из 
			TokenInfo[] line_tokens = source_.GetLineTokens(request_.Line);
			if(line_tokens == null)
				return ParserResultQuickInfoText("QuickInfo line_tokens == <null>.");

			TokenInfo request_token = new TokenInfo();
			int request_token_index = source_.GetTokenInfoAt(line_tokens, request_.Col, ref request_token);
			if(request_token_index < 0)
				return ParserResultQuickInfoText("QuickInfo request_token_index < 0.");

			// легче было перечислить для чего показывать...
			if (request_token.Type == TokenType.WhiteSpace || request_token.Type == TokenType.LineComment || request_token.Type == TokenType.Delimiter || 
				request_token.Type == TokenType.Literal || request_token.Type == TokenType.String)
					return ParseResultNull_;

			int current_call_para_index_unused = EslParseApi.ParseTokensAndFillSink(request_sink, request_token, request_.Line, source_.GetLine(request_.Line), line_tokens);

			// Для QuickInfo нам НУЖНО НЕ ПОСЛЕДНИЙ CALL (которого еще может и не быть - РАЗ, а ДВА мы сами сказали на имени ЗАКОНЧИТЬ), а просто накопленное ИМЯ
			StringCollection current_call_names = request_sink.Names_;
			if (current_call_names == null)
				return ParserResultQuickInfoText("Информация о имени <null>.");

			int current_call_names_count = current_call_names.Count;
			if (current_call_names_count == 0)
				return ParserResultQuickInfoText("Информация о имени отсутствует.");

			EslDecl current_call_names_decl = EslEngineUI.GetNameDeclarationForQuickInfo(source_, request_.Line, request_.Col, current_call_names);
			if(current_call_names_decl == null)
				return ParserResultQuickInfoText(EslParseApi.NamesCollectionToString(current_call_names));

			//TODO	Мы показываем информацию о самой декларации, когда после нее могут быть параметры однозначно определяющие какой метод из декларации
			//		прописан в выражении. Но мы ПОСЛЕ имени декларации ничего не парсим и даже кол-во ',' не считаем.
			//		А можем, НО это лучше делать после, т.к. подавляющее большинство Деклараций у нас с одним методом и только некоторые мульти-методные
			//		Поэтому лучше сначала получить собственно декларацию, и уж потом, если она мульти-методная, попытаться пропарсить дальше.
			string s_quick_info = EslEngineUI.GetDeclarationQuickInfoText(current_call_names_decl);
			if(s_quick_info == null)
				return ParserResultQuickInfoText("s_quick_info == null");
			if (s_quick_info.Length == 0)
				return ParserResultQuickInfoText("s_quick_info.Length == 0");

			return new EslParserResultForScope(null, null, s_quick_info);
		}


	} //class EslParse
} // namespace EsbStudio.Esl
