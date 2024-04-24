using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
#if DEBUG || DEBUG_IN_RELEASE
	using System.Diagnostics;
#endif

using Microsoft.VisualStudio.Package;
using System.Collections;
using System.Collections.Specialized;
using Microsoft.VisualStudio.TextManager.Interop;	//TextSpan


namespace EsbStudio.Esl
{
	internal class EslParseCallInfo
	{
		internal int				currentParameter;
		internal StringCollection	names;
		internal ArrayList			sourceLocations;
	}

	class EslParseMethodCalls 
	{
		internal Stack				calls;
		internal EslParseCallInfo	call;
		//
		public EslParseMethodCalls() {
			this.calls = new Stack();
			this.Push(new StringCollection(), new ArrayList());
		}
		//
		public void Push(StringCollection names, ArrayList sourceLocations) {
			this.calls.Push((object)this.call);
			this.call = new EslParseCallInfo();
			this.call.names = names;
			this.call.sourceLocations = sourceLocations;
		}
		public void NextParameter() => ++this.call.currentParameter;
		public void Pop() {
			if (this.calls.Count <= 0)
				return;
			this.call = (EslParseCallInfo)this.calls.Pop();
		}
		public EslParseCallInfo GetCurrentMethodCall() => this.call;
	}


	//TODO	Вообще было бы лучше для каждого Request свой Sink, а не общий для всех
	//		И вообще эта конструкция с унаследованием от AuthoringSink дурацкая, т.к. их AuthoringSink дурацкий
	//		Но отказаться от этого можно только переписав все Апи где их AuthoringRequest и AuthoringSink используется.. внимательно..
	//		т.к. Request не интерфейс, а фактически просто структура в которой прописано, что Sink это AuthoringSink, а он дурацкий, но ему активно в internal потроха лазють..
	//		Много.. Будем постепенно..
	//		Пока просто дублируем (правим) функциональность
	
	//TODO	Катастрофичести ВАЖНО полностью перейти на свой Синк и избавиться от этой мерзости
	//		(для этого нужно в EslSource переписать MethodTip и CompleteWord, которые окнами управляют. Может еще что где.. разбираться надо!)
	class EslParseSink : AuthoringSink
	{
		internal StringCollection		Names_;
		internal ArrayList				SourceLocations_;
		internal EslParseMethodCalls	MethodCalls_;   //error CS0122: "MethodCalls" недоступен из-за его уровня защиты.
		//
		public EslParseSink(ParseReason reason_, int line_, int col_, int max_errors_) : base(reason_, line_, col_, max_errors_)
		{
			//this.MsSink_ = ms_sink_;
			this.Names_ = new StringCollection();
			this.SourceLocations_ = new ArrayList();
			this.MethodCalls_ = new EslParseMethodCalls();
		}

// Для памяти как сделано	
/*    public bool BraceMatching { get {
        switch (this.reason) {
          case ParseReason.HighlightBraces:
          case ParseReason.MemberSelectAndHighlightBraces:
          case ParseReason.MatchBraces:
            return true;
          default:
            return false;
        }
      }}
 */	
/*		public bool FindNames {	get	{
			switch (this.Reason) {
				case ParseReason.MemberSelect:
				case ParseReason.MemberSelectAndHighlightBraces:
				case ParseReason.CompleteWord:
				case ParseReason.DisplayMemberList:
				case ParseReason.QuickInfo:
				case ParseReason.MethodTip:
				case ParseReason.Autos:			return true;
				default:						return false;
			}
		}}
*/
/*		public bool MethodParameters { get {
			switch (this.Reason) {
				case ParseReason.QuickInfo:	
				case ParseReason.MethodTip:		return true;
				default:						return false;
			}
		}}
*/
		public override void StartName(TextSpan span, string name) 
		{
			if (!this.FindNames)
				return;
			int iStartLine = span.iStartLine;
			int iStartIndex = span.iStartIndex;
			int iEndLine = span.iEndLine;
			int iEndIndex = span.iEndIndex;
			if (	iStartLine < 0 || 
					iStartIndex < 0 || 
					iStartLine > iEndLine || 
					(iStartLine == iEndLine && iEndIndex < iStartIndex) || 
					iStartLine > this.Line || 
					iEndLine < this.Line		// (*1) Вот это условие не даст нам трассировать от имени в предыдущей строке - ЕСЛИ ИМЯ ЗАКОНЧИЛОСЬ РАНЕЕ НАШЕЙ СТРОКИ - ЭТО НЕ ИМЯ
				)
				return;
			this.Names_.Add(name);
			this.SourceLocations_.Add((object)span);
			//
			base.StartName(span, name);
		}
		public override void QualifyName(TextSpan selectorContext, TextSpan nameContext, string name) 
		{
			if (!this.FindNames)
				return;
			int iStartLine1 = selectorContext.iStartLine;
			int iStartIndex1 = selectorContext.iStartIndex;
			int iEndIndex1 = selectorContext.iEndIndex;
			int iEndLine1 = selectorContext.iEndLine;
			int iStartLine2 = nameContext.iStartLine;
			int iStartIndex2 = nameContext.iStartIndex;
			int iEndIndex2 = nameContext.iEndIndex;
			int iEndLine2 = nameContext.iEndLine;
			if (iStartLine1 < 0 || iStartIndex1 < 0 || 
					iStartLine1 == iEndLine1 && iEndIndex1 < iStartIndex1 || 
					iStartLine2 < iStartLine1 || 
					iStartIndex2 < 0 || 
					iStartLine2 == iEndLine2 && iEndIndex2 < iStartIndex2 || 
					iStartLine2 > this.Line || 
					iEndLine2 < this.Line)
				return;
			this.Names_.Add(name);
			this.SourceLocations_.Add((object)nameContext);
			//
			base.QualifyName(selectorContext, nameContext, name);
		}
		public override void StartParameters(TextSpan context) {
			if (!this.MethodParameters)
				return;
			int iStartLine = context.iStartLine;
			int iStartIndex = context.iStartIndex;
			if (iStartLine < 0 || iStartIndex < 0 || 
					this.Line <= iStartLine && (this.Line != iStartLine || this.Column < iStartIndex)
				)
				return;
			this.MethodCalls_.Push(this.Names_, this.SourceLocations_);
			this.Names_ = new StringCollection();
			this.SourceLocations_ = new ArrayList();
			//
			base.StartParameters(context);
		}
		public override void NextParameter(TextSpan context) {
			if (!this.MethodParameters)
				return;
			int iStartLine = context.iStartLine;
			int iStartIndex = context.iStartIndex;
			if (iStartLine < 0 || 
				iStartIndex < 0 || 
				this.Line <= iStartLine && (this.Line != iStartLine || this.Column <= iStartIndex)
				)
				return;
			this.MethodCalls_.NextParameter();
			//
			base.NextParameter(context);
		}
		public override void EndParameters(TextSpan context) {
			if (!this.MethodParameters)
				return;
			int iStartLine = context.iStartLine;
			int iStartIndex = context.iStartIndex;
			if (iStartLine < 0 || iStartIndex < 0 || 
					this.Line <= iStartLine && (this.Line != iStartLine || this.Column <= iStartIndex)
				)
				return;
			this.MethodCalls_.Pop();
			//
			base.EndParameters(context);
		}
	}

	/*	А используется все это в Source
	 *	
		internal void HandleMethodTipResponse(ParseRequest req)
		{
		  try
		  {
			if (this.service == null || req.Timestamp != this.requestSync)
			  return;
			CallInfo currentMethodCall = req.Sink.MethodCalls.GetCurrentMethodCall();
			if (currentMethodCall != null)
			{
			  StringCollection names = currentMethodCall.names;
			  if (names.Count != 0)
			  {
				ArrayList sourceLocations = currentMethodCall.sourceLocations;
				string name = names[names.Count - 1];
				TextSpan context = (TextSpan) sourceLocations[names.Count - 1];
				Methods methods = req.Scope.GetMethods(context.iStartLine, context.iStartIndex, name);
				if (methods != null)
				{
				  int currentParameter = currentMethodCall.currentParameter;
				  this.methodData.Refresh(req.View, methods, currentParameter, context);
				  return;
				}
			  }
			}
			this.DismissCompletor();
		  }
		  catch
		  { }
		}
	*/



	internal static class EslParseApi
	{
		/* Тригер сюда срабатывает при ЛЮБОЙ '(' ',' ')'
		 * Наша задача оттрасировать взад от этой позиции к первой незакрытой '(' и перед ней найти идентификатор
		 * и потом пойти вперед по ходу вызывая .StartName, .QualifyName, .StartParameters, .NextParameter, .EndParameters
		 * Причем что логика AuthoringSink устроена в виде стека и при .EndParameters он вложенное выражение делает Pop и возвращается к искомому
		 * т.е. в случае Фуу(Бар(..), Бар будет снята со стека и мы вернемся опять к Фуу
		 * Главное только скобки правильно просчитать
		 * Даже в случае Фуу(); Бар'(' если будем начинать с начала строки то все должно быть Ок (вроде...)
		 * Только нужно строки в кавычках и комментарии правильно учитывать
		 * 
		 testfunc("a string",3);
				^^          ^ ^
				12          3 4
		1 -The parser calls StartName with the text "testfunc".		https://learn.microsoft.com/en-us/dotnet/api/microsoft.visualstudio.package.authoringsink.startname
		2 -The parser calls StartParameters.						https://learn.microsoft.com/en-us/dotnet/api/microsoft.visualstudio.package.authoringsink.startparameters
		3 -The parser calls NextParameter.							https://learn.microsoft.com/en-us/dotnet/api/microsoft.visualstudio.package.authoringsink.nextparameter
		4 -The parser calls EndParameters.							https://learn.microsoft.com/en-us/dotnet/api/microsoft.visualstudio.package.authoringsink.endparameters
		*/



		/*	Функция проходится по токенам строки и "вычисляет" выражения вида 'Foo(Bar(1,2).Baz('
		 *	В результате в sink_.MethodCalls_.call накапливается контекст имен текущего (последнего перед trigger_token_) выражения.
		 *	т.е. для 'Foo(Bar(1,2).Baz(' коллекция строк в .call.names будет содержать [Bar,Baz]
		 *	т.е. остается в текущем контексте найти что такое 'Bar', найти в его контексте имя 'Baz', и т.д... 
		 *	и отдать нужную информацию о последнем имени
		 *	ПС. для выражения 'Foo(Bar(1,2).Baz(' инфомация о Foo остается "выше" на стеке вызовов.
		 *		А если бы было что-то типа Foo(); Bar. то информация о Foo "уходит" - снимается со стека - вызов закончен.
		 *		т.е. по "Их" алгоритму при закрывающей скобке - все - вызов закончен и информация о нем снимается со стека
		 *		А нам надо, чтобы она немного "пожила", потому что после ')' может следовать '.' и далее. т.е. "вызов" продолжается мембером
		 *	
		 */
		public static int ParseTokensAndFillSink(EslParseSink sink_, TokenInfo trigger_token_, int request_line_, string line_text_, TokenInfo[] line_tokens_) 
		{
			if (line_tokens_ == null)
				return -1;

			int					param_index = -1;
			bool				m_member_state = false;
			TextSpan			m_member_delimiter_span = TextSpanExt.TextSpanEmpty_;
			EslParseCallInfo	last_callinfo_when_end_param = null;
			/*	Каждый раз при .StartName	в Names(Collection) добавляется Имя
			 *	Каждый раз при .QualifyName в Names(Collection) добавляется Имя точно также (только проверки спанов сложнее)
			 *	Каждый раз при .StartParameters	содержимое Names Пушится на Стек MethodCalls и коллекции Names присваиватся новая пустая StringCollection
			 *	При .EndParameters просто делается .MethodCalls.Pop();
			 *		(интересно что содержимое Names НЕ ОЧИЩАЕТСЯ)
			 *	При .NextParameter просто делается .MethodCalls.NextParameter(), который просто ++this_MethodCalls.call.currentParameter;
			 *	
			 *	ТО ЕСТЬ Names всегда просто коллекция имен а.б.с...
			 *	А после .MethodCalls.Pop() при .EndParameters Names содержит какой-то мусор, который был внутри скобок,
			 *		а собственно выражение к которому можно применить дальнейшие .Member или еще что
			 *		содержится в .MethodCalls.call - CallInfo::StringCollection names
			 */

			//
			foreach (TokenInfo tok in line_tokens_)
			{
				if (tok.EndIndex > trigger_token_.EndIndex)									// сам trigger_token_ мы должны проанализировать, а все что что после - НЕТ
					break;
				if (tok.Type == TokenType.WhiteSpace || tok.Type == TokenType.LineComment)  // полностью игнорируем ненужное
					continue;

				if (tok.Type == TokenType.Delimiter && tok.IsTriggerMemberSelect())
				{
					m_member_state = true;
					m_member_delimiter_span = tok.GetTextSpanFor(request_line_);

					// когда мы увидели '.' значит предыдущее накопленное выражение (last_callinfo_when_end_param) если оно было
					// нам нужно восстановить его из памяти - это случай Фуу().
					// то есть Они у себя благополучно сделали Поп и все Фуу сняли со стека
					if (last_callinfo_when_end_param != null && last_callinfo_when_end_param.names.Count > 0 && last_callinfo_when_end_param.names[0].Length > 0) {
						int names_count = last_callinfo_when_end_param.names.Count;
						for (int i_name = 0; i_name < names_count; ++i_name) {
							sink_.StartName((TextSpan)last_callinfo_when_end_param.sourceLocations[i_name], last_callinfo_when_end_param.names[i_name]);
						}
					}
					last_callinfo_when_end_param = null;	// и по любому забываем
					continue;
				}

				if (tok.Type == TokenType.Identifier)
				{
					string s_name = tok.GetTokenTextFrom(line_text_);
					if (m_member_state)	{
						// QualifyName работает у них как просто StartName, т.е. просто добавляет еще одно имя в Names_
						//TODO	Не буду пока как-то показывать ОШ если перед '.' не было никакого выражения - не понимаю пока как лучше ОШ показать
						//		Разумнее всего было-бы здесь ShowTip выбросить с сообщением
						// НО! Для ситуации Фуу(...).Бар( нам нужен контекст Фуу.Бар а не просто Бар
						// т.е. если у нас уже было накоплены Имена - просто добавляем, а если Имена пусты, то лезем  колл-стэк и сначала добавляем имена оттуда
						if (sink_.Names_.Count > 0)
							sink_.QualifyName(m_member_delimiter_span, tok.GetTextSpanFor(request_line_), s_name);
						else 
						{
							//TODO	А вот здесь какая-то ошибка - мембера привязать не к чему..	Будем считать его просто именем как у них
							sink_.QualifyName(m_member_delimiter_span, tok.GetTextSpanFor(request_line_), s_name);
						}
					}
					else {
						if (sink_.Names_.Count == 0)
							sink_.StartName(tok.GetTextSpanFor(request_line_), s_name);
						else {
							// если имя идет после имени (фуу бар..) подряд без всякого разделителя, то это какая-то ош.
							// реализация от МС их просто накапливает, что для нашего языка неверно - нет у нас такой грамматики
							// нам бы надо как-то заменить имя, но в потроха ИХ реализации не влезешь :(	TODO пока..
							// поэтому даем две команды StartParameters и EndParameters - при этом текущий call кладется на стек
							// (при этом текущий список имен очищается - типа мы готовы к приему нового выраженя)
							// и сразу снимается чтобы собственно call stack не портить (хотя возможно это необязательно..)
							TextSpan name_token = tok.GetTextSpanFor(request_line_); //пришедшее имя считаем разделителем, там все равно только его корректность проверяется
							sink_.StartParameters(name_token);
							sink_.EndParameters(name_token);
							sink_.StartName(name_token, s_name);
						}
					}
				}
				else if (tok.Trigger == TokenTriggers.ParameterStart)
				{
					sink_.StartParameters(tok.GetTextSpanFor(request_line_));
					param_index = 0;
				}
				else if (tok.Trigger == TokenTriggers.ParameterNext)
				{
					sink_.NextParameter(tok.GetTextSpanFor(request_line_));
					param_index++;
				}
				else if (tok.Trigger == TokenTriggers.ParameterEnd)
				{
					// когда делается EndParameters, то текущий контекст вызова перезаписывается со стека (как-бы забывается - закончен и забыт)
					// НО! После ')' может быть '.' и нам надо его сохранить для знания контекста к чему мембера привязать
					last_callinfo_when_end_param = sink_.MethodCalls_.call;
					sink_.EndParameters(tok.GetTextSpanFor(request_line_));
					param_index = -1;
				}

				if (m_member_state)
				{
					m_member_state = false;
					//m_member_delimiter_span = TextSpanEmpty_;	в общем-то лишне
				}				
			}

			
			EslParseCallInfo	current_call_info = sink_.MethodCalls_.call;
			string				current_call_info_names = "";
			if (current_call_info == null)
				current_call_info_names = "<null>";
			else {
				if (current_call_info.names.Count == 0)
					current_call_info_names = "<empty>";
				else if(current_call_info.names.Count == 1)
					current_call_info_names = current_call_info.names[0];
				else {
					current_call_info_names = current_call_info.names[0] + ".";
					for (int i_call = 1; i_call < current_call_info.names.Count; ++i_call) {
						current_call_info_names += current_call_info.names[i_call];
					}
				}
			}



			return param_index;
		}


		// вспомогашка
		internal static string NamesCollectionToString(StringCollection names_) {
			if (names_ == null)
				return "<null>";
			int names_count = names_.Count;
			string s_text = "[";
			if (names_count > 0) {
				s_text += ((names_[0] == null)? "<null>" : names_[0]);
				for (int i_name = 1; i_name < names_count; ++i_name) {
					s_text += ("::" + ((names_[i_name] == null)? "<null>" : names_[i_name]));
				}
			}
			s_text += "]";
			return s_text;
		}

	}// class EslParseApi


	internal class EslParseRequest : ParseRequest
	{
		public EslParseRequest(int line_, int col_, TokenInfo token_, string source_text_, string source_file_,
										ParseReason reason_, IVsTextView source_view_, AuthoringSink parse_sink_, bool parse_synchronous_)
			: base(line_, col_, token_, source_text_, source_file_, reason_, source_view_, parse_sink_, parse_synchronous_) 
		{ }

		// Данный конструктор у базового класса просто тупо устанавливает поле terminate и в хелпе указано, что
		//	[in] Set to true if the parsing operation is to be terminated. For this constructor, this argument is must always true.
		// НО! Таким образом ParseRequest (типа запрос на прерывание) нигде и никогда не создается
		// А!!	поле terminate используется только в LanguageService::internal void ParseThread() и только на чтение
		//		в смысле что работающий поток должен просигнализировать что он работу закончил и его можно закрыть
		// Так об поле terminate и в хелпе написано
		// Видимо артефакт какой-то. не реализую.
		//
		//public EslParseRequest(bool terminate_parse_) : base(terminate_parse_) { }


		//TODO Вроде бы мы всегда сами ParseRequest создаем. Тогда к нам всегда должен приходить EslParseSink
		//		создаем то вроде мы в EslLanguageService::CreateParseRequest
		//			new EslParseRequest(line_, col_, token_, source_text_, source_file_, reason_, source_view_, source_.CreateAuthoringSink(reason_, line_, col_), parse_synchronous);
		//		но используем нашу же перегрузку EslSource::CreateAuthoringSink(reason_, line_, col_), но от как базовый Source source_
		//		поэтому надо еще поразбираться откуда и как могут приходить вызовы в EslLanguageService::CreateParseRequest и кто как ParseRequest создает..
		//		пока все .BeginParse не оверрайдишь до конца не уверен, что Синк это наш Синк, а не "их" Синк
		public EslParseSink GetEslParseSink() {
			EslParseSink esl_sink = base.Sink as EslParseSink;
			Debug.Assert(esl_sink != null);
			return esl_sink;
		}
	}




}
