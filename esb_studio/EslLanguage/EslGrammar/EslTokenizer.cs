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

namespace EsbStudio.Esl
{
	class BslTokensScanner : IScanner
	{
		/////////////////////////////////////////////////////  
		// Fields  
		private int		m_offset;
		private string	m_source;
		private int		m_source_len;   //TODO	Лучше _index_max - везде именно он используется

		/////////////////////////////////////////////////////  
		// Enumerations  
		private enum ParseState
		{
			InQuotes = 1,
			InMeth = 2,
		}

		/////////////////////////////////////////////////////  
		// IScanner methods  
		/*	Вызывается (обычно?) в колоризаторе (IVsColorizer) построчно, для БЫСТРОЙ раскраски строки. Типа так:
			*	https://learn.microsoft.com/en-us/dotnet/api/microsoft.visualstudio.package.iscanner?view=visualstudiosdk-2022
			*	https://learn.microsoft.com/en-us/dotnet/api/microsoft.visualstudio.package.iscanner.scantokenandprovideinfoaboutit?view=visualstudiosdk-2022#microsoft-visualstudio-package-iscanner-scantokenandprovideinfoaboutit(microsoft-visualstudio-package-tokeninfo-system-int32@)
				this.scanner.SetSource(text, 0);  
                TokenInfo tokenInfo = new TokenInfo();  
                while (this.scanner.ScanTokenAndProvideInfoAboutIt(tokenInfo, ref state))  
                {  
                    // Do something with tokenInfo  
                }
				
			*/
		public bool ScanTokenAndProvideInfoAboutIt(TokenInfo out_token_info_, ref int in_out_running_state_) 
		{
			/* В начале каждой строки MPF передает классу Colorizer значение, которое будет использоваться в качестве переменной состояния, передаваемой сканеру. 
				* В конце каждой строки сканер возвращает обновленную переменную состояния. MPF кэширует эту информацию о состоянии для каждой строки, 
				* чтобы сканер мог начать анализ с любой строки без необходимости начинать с начала исходного файла.
				* https://learn.microsoft.com/en-us/previous-versions/visualstudio/visual-studio-2015/extensibility/internals/legacy-language-service-parser-and-scanner?view=vs-2015
				* начальное состояние state_ получается из Colorizer.GetStartState(out int start);  *** The base method always returns 0. ***
				* https://learn.microsoft.com/en-us/dotnet/api/microsoft.visualstudio.package.colorizer.getstartstate?view=visualstudiosdk-2022#microsoft-visualstudio-package-colorizer-getstartstate(system-int32@)
				*/

			// то есть m_offset это индекс первого символа строки с которого мне надо начинать анализ
			if (out_token_info_ == null)
				return false;

			if (m_offset < 0 || m_offset >= m_source_len)
				return false;
			
			int token_end = GetNextToken(m_offset, out_token_info_, ref in_out_running_state_);
			m_offset = token_end + 1;
			return true;
		}
		public void SetSource(string source, int offset) 
		{
			//Debug.WriteLine($"TOkenize: {source}");

			m_offset = offset;
			m_source = source;
			m_source_len = source.Length;
		}

		/////////////////////////////////////////////////////  
		// Private methods  
		// 
		// ** useful BitOperations Class	https://learn.microsoft.com/en-us/dotnet/api/system.numerics.bitoperations?view=net-8.0
		static bool state_mask(int state_, ParseState state_mask_) {
			return ((state_ & (int)state_mask_) != 0);
		}
		static void state_mask_set(ref int in_out_state_, ParseState state_mask_) {
			in_out_state_ |= (int)state_mask_;
		}
		static void state_mask_reset(ref int in_out_state_, ParseState state_mask_) {
			in_out_state_ &= ~(int)state_mask_;
		}

		static void state_set_string_multyline_begin(ref int in_out_state_) {
			Debug.Assert(!state_is_string(in_out_state_));
			state_mask_set(ref in_out_state_, ParseState.InQuotes);
		}
		static void state_set_string_multyline_final(ref int in_out_state_) {
			Debug.Assert(state_is_string(in_out_state_));
			state_mask_reset(ref in_out_state_, ParseState.InQuotes);
		}
		static bool state_is_string(int state_) {
			return state_mask(state_, ParseState.InQuotes);
		}

		static bool state_is_meth(int state_) {
			return state_mask(state_, ParseState.InMeth);
		}
		static void state_set_meth_begin(ref int in_out_state_) {
			Debug.Assert(!state_is_meth(in_out_state_));
			state_mask_set(ref in_out_state_, ParseState.InMeth);
		}
		static void state_set_meth_final(ref int in_out_state_) {
			Debug.Assert(state_is_meth(in_out_state_));
			state_mask_reset(ref in_out_state_, ParseState.InMeth);
		}


		private int GetNextToken(int char_index_1st_, TokenInfo token_info_, ref int in_out_state_) 
		{
			Debug.Assert(char_index_1st_ >= 0 && char_index_1st_ < m_source_len);
			//
			char ch_1st = m_source[char_index_1st_];

			if (state_is_string(in_out_state_))
			{
				// в состояние InQuotes мы можем оказаться если был перенос строки на следующую строку
				// (т.к. иначе мы бы нашли вторую кавычку в той же строке и вышли из режима)
				// здесь у нас допустим комментарий, пустая строка, пробелы и/или строка начинающаяся с |
				switch (ch_1st) 
				{
					case ' ':	return trace_white_space(token_info_, char_index_1st_);
					case '\t':	return trace_white_tab(token_info_, char_index_1st_);
					case '/':
						{	// в продолжении строки встретился или комментарий или это ошибка (TODO	возможно лучше Ош до конца строки)
							int char_index_next = char_index_1st_ + 1;
							if (char_index_next < m_source_len && m_source[char_index_next] == '/')
								return fill_token_comment_line(token_info_, char_index_1st_);
							else
								return fill_token_error(token_info_, char_index_1st_, char_index_1st_);
						}
					case '|': 
						{
							int char_index_next = char_index_1st_ + 1;
							if (char_index_next < m_source_len)		// продолжение многостроки до конца строки
								return make_token_string_multyline_middle(token_info_, char_index_1st_, m_source_len - 1);
							else {
								int index_of_next_quote = m_source.IndexOf('"', char_index_next);
								if(index_of_next_quote == -1)
									return make_token_string_multyline_middle(token_info_, char_index_1st_, m_source_len - 1);
								else
									return make_token_string_multyline_final(token_info_, char_index_1st_, index_of_next_quote, ref in_out_state_);
							}
						}
					default:
						return fill_token_error(token_info_, char_index_1st_, char_index_1st_);
				}
			}
			else
			{
				// Parse the token starting at index, returning the  
				// token's start and end index in tokenInfo, along with  
				// the token's type and color to use.  
				// If the token is a quoted string and the string continues  
				// on the next line, set state to InQuotes.  
				// If the token is a comment and the comment continues  
				// on the next line, set state to InComment.  

				/*
					* Delimiter	operates as a separator between two language elements. For example, in C#, the period "." between class name and member name.
					* Identifier	An identifier or name. For example, the name of a variable, method, or class.
					* Keyword		A language keyword, an identifier that is reserved by the language. For example, in C#, do, while, foreach ...
					* Operator		A punctuation character that has a specific meaning in a language. For example, in C#, arithmetic operators +, -,
					* WhiteSpace	A space, tab, or newline. Typically, a contiguous run of any whitespace is considered a single whitespace token. 
					* String		Typically defined as zero or more characters bounded by double quotes.
					* Unknown		The token is an unknown type. This is typically used for any token not recognized by the parser and should be considered an error in the code being parsed.
					*/
				switch (ch_1st) 
				{
					case ' ':	return trace_white_space(token_info_, char_index_1st_);
					case '\t':	return trace_white_tab(token_info_, char_index_1st_);
					case '\n':	return fill_token_white(token_info_, char_index_1st_, char_index_1st_);
					//
					case '/':
						int char_index_next = char_index_1st_ + 1;
						if (char_index_next < m_source_len && m_source[char_index_next] == '/')
							return fill_token_comment_line(token_info_, char_index_1st_);
						else
							return fill_token_operator(token_info_, char_index_1st_);
					//
					case '"':	return make_token_string(token_info_, char_index_1st_, ref in_out_state_);
					//
					case '(':	return fill_token_bra_l_bra(token_info_, char_index_1st_);
					case ')':	return fill_token_bra_r_bra(token_info_, char_index_1st_);
					case ',':	return make_token_param_separator(token_info_, char_index_1st_);
					//
					case '[':	return fill_token_bra_l_bra(token_info_, char_index_1st_);
					case ']':	return fill_token_bra_r_bra(token_info_, char_index_1st_);
					//
					case '.':	return fill_token_delimiter(token_info_, char_index_1st_);	//TODO	нужен state .Member - Member НЕ кейворд!
					//
					case '+': case '-':	case '*': /*case '/':*/  case '%':
					case '=':	return fill_token_operator(token_info_, char_index_1st_);		// односимвольное. state оставляем тем же
																								
					case '?':	return fill_token_operator(token_info_, char_index_1st_);		//TODO	А может это кейворд?
					//																		
					case '0': case '1': case '2': case '3':case '4': case '5': case '6': case '7': case '8': 
					case '9':	return trace_number(token_info_, char_index_1st_);
					//
					case '\'':	return trace_date(token_info_, char_index_1st_);
					//
					case '&':   return trace_name_attr(token_info_, char_index_1st_);
					case '_':	return make_name_underscore(token_info_, char_index_1st_);
					case '#':	return trace_name_preproc(token_info_, char_index_1st_);
					case ';':	return fill_token_operator(token_info_, char_index_1st_);       //TODO	А может это кейворд?
																								//
					case '~':   //ОПЕРАТОР Перейти ~ВыходИзДвойногоЦикла;	ИЛИ	МЕТКА	~ВыходИзДвойногоЦикла:
					{
						//TODO	метка-аргумент и метка-собственно-метка - это вообще-то разные вещи.. можно подумать здесь..
						return make_name_label(token_info_, char_index_1st_);
					}
					default:
					{   // если буква - то или кейворд или идентификатор. иначе ошибка.
						if (Char.IsLetter(ch_1st))
							return trace_name(token_info_, ch_1st, char_index_1st_, ref in_out_state_);
						else
							return fill_token_error(token_info_, char_index_1st_, char_index_1st_);
					}
				}
			}
		}


		//https://learn.microsoft.com/ru-ru/dotnet/api/microsoft.visualstudio.package.tokentype?view=visualstudiosdk-2022
		/*
			*	TokenColor		Color		Determines the color index to use for the token.
										* The value handled by this property is an index into the ColorableItem list maintained by the LanguageService class. 
										* The TokenColor enumeration is just a guide to five typical color indices that correspond to the default ColorableItem list. 
										* If you supply your own ColorableItem list, you can make sure the first set of colorable items correspond to this enumeration 
										* and add more color indices beyond that or replace the enumeration values entirely with your own meaning.
										* Примечание	Color index 0 is reserved and always represents the default text color the user has specified. 
										*				The scanner should return 0 for any token type of Text.
			*	int				EndIndex	Determines the index of the last character of the token.
			int				StartIndex	Determines the index of the first character of the token.
			int				Token		Language Specific - наш перечислитель (или еще какая информация) о токене
			TokenTriggers	Trigger		Returns a combination of flags from the TokenTriggers enumeration.
										* A token trigger is used to signal a particular language feature in support of IntelliSense. 
										* For example, MatchBraces can be returned when a closing brace is parsed.
			TokenType		Type		Determines the type of the token.
										* The types specified in this enumeration cover tokens that can appear in all common computer languages. 
										* More importantly, these are the token types that the default managed package framework language service classes understand. 
										* You can add additional types that your scanner support but you should first adhere to the meanings of the types described here.
			*/
		enum EslTokenFlags { _0 }
		int trace_name_attr(TokenInfo token_info_, int char_index_1st_) {
			//TODO	аттрибут как препроц. это не совсем одно и тоже..
			return trace_name_preproc(token_info_, char_index_1st_);
		}
		int trace_name_preproc(TokenInfo token_info_, int char_index_1st_) {
			int char_index = char_index_1st_;
			while (true)
			{
				int char_index_next = char_index + 1;   //CharUnicodeInfo.GetUnicodeCategory вроде лучше, но сложнее и для текста программы ненужна
				if (char_index_next < m_source_len && Char.IsLetter(m_source[char_index_next]))
					char_index = char_index_next;
				else
					break;
			};
			//TODO	для препроцессора мы не разбираемся со словом и тупо ставим keyword. умнее надо.. (и пробела между # и словом не допускаем - ? а может у 1С допустимо?)
			return fill_token_name_keyword(token_info_, char_index_1st_, char_index);
		}
		static bool IsLetterOrDigitOrUnderscore(char ch_) {
			if (ch_ == '_')
				return true;
			return Char.IsLetterOrDigit(ch_);
		}
		int trace_name_len_letter_or_digit_or_underscore(int char_index_1st_) {
			int name_len = 1;
			while (true)
			{
				int char_index_next = char_index_1st_ + name_len;   //CharUnicodeInfo.GetUnicodeCategory вроде лучше, но сложнее и для текста программы ненужна
				// Char.IsLetterOrDigit(ch_) возвращает именно IsLetter or IsDigit, но последующие после первого символа также могут быть _
				if ( char_index_next < m_source_len && IsLetterOrDigitOrUnderscore(m_source[char_index_next]) )
					name_len++;
				else
					break;
			};
			return name_len;
		}

		int make_name_label(TokenInfo token_info_, int char_index_1st_) {
			int name_len = trace_name_len_letter_or_digit_or_underscore(char_index_1st_);
			if (name_len == 1)
				return fill_token_error(token_info_, char_index_1st_, char_index_1st_);	//между : и именем ничего лишнего не должно быть
			else
				return fill_token_name_ident(token_info_, char_index_1st_, char_index_1st_ + name_len - 1);	//TODO	метки желательно другим цветом
		}
		int make_name_underscore(TokenInfo token_info_, int char_index_1st_) {
			int name_len = trace_name_len_letter_or_digit_or_underscore(char_index_1st_);
			return fill_token_name_ident(token_info_, char_index_1st_, char_index_1st_ + name_len - 1);
		}
		int trace_name(TokenInfo token_info_, char ch_1st_alpha_, int char_index_1st_, ref int in_out_state_) 
		{

			int name_len = trace_name_len_letter_or_digit_or_underscore(char_index_1st_);

			string s_keyword = null;
			if (BslKeywords.get_keyword_ru(m_source, ch_1st_alpha_, char_index_1st_, name_len, ref s_keyword))
			{
				bool b_state_meth = state_is_meth(in_out_state_);
				if (!b_state_meth && (s_keyword == "ФУНКЦИЯ" || s_keyword == "ПРОЦЕДУРА"))
					state_set_meth_begin(ref in_out_state_);
				else if (b_state_meth && (s_keyword == "КОНЕЦФУНКЦИИ" || s_keyword == "КОНЕЦПРОЦЕДУРЫ"))
					state_set_meth_final(ref in_out_state_);

				return fill_token_name_keyword(token_info_, char_index_1st_, char_index_1st_ + name_len - 1);
			}
			else {
				return fill_token_name_ident(token_info_, char_index_1st_, char_index_1st_ + name_len - 1);
			}
		}
		int fill_token_name_keyword(TokenInfo token_, int char_index_1st_, int char_index_last_) {
			return fill_token(token_, char_index_1st_, char_index_last_, TokenType.Keyword, TokenColor.Keyword);
		}
		int fill_token_name_ident(TokenInfo token_, int char_index_1st_, int char_index_last_) {
			return fill_token(token_, char_index_1st_, char_index_last_, TokenType.Identifier, TokenColor.Identifier);
		}

		int trace_date(TokenInfo token_info_, int char_index_1st_) {
			int char_index = char_index_1st_;
			while (true)
			{
				int char_index_next = char_index + 1;
				if (char_index_next >= m_source_len)
					return fill_token_error(token_info_, char_index_1st_, char_index);
				else if (m_source[char_index_next] == '\'')
					return fill_token_date(token_info_, char_index_1st_, char_index_next);
				else
					char_index = char_index_next;
			};
		}
		int make_token_white(TokenInfo token_info_, int char_index_1st_, char ch_white_1st_) {
			int char_index = char_index_1st_;
			while (true)
			{
				int char_index_next = char_index + 1;
				if (char_index_next < m_source_len && m_source[char_index_next] == ch_white_1st_)
					char_index = char_index_next;
				else
					break;
			};
			return fill_token_white(token_info_, char_index_1st_, char_index);
		}
		int trace_white_space(TokenInfo token_info_, int char_index_1st_) {
			return make_token_white(token_info_, char_index_1st_, ' ');
		}
		int trace_white_tab(TokenInfo token_info_, int char_index_1st_) {
			return make_token_white(token_info_, char_index_1st_, '\t');
		}

		int trace_number(TokenInfo token_info_, int char_index_1st_) {
			int char_index = char_index_1st_;
			while (true) {
				int char_index_next = char_index + 1;
				if (char_index_next >= m_source_len)
					return fill_token_number(token_info_, char_index_1st_);
				char ch = m_source[char_index_next];
				switch (ch) { 
					case '0': case '1': case '2': case '3':case '4': case '5': case '6': case '7': case '8': case '9':
					case '.':
						char_index = char_index_next;
						break;
					default:
						return fill_token_number(token_info_, char_index_1st_, char_index);
				}
			}
		}
		int fill_token(TokenInfo token_, int char_index_1st_, int char_index_last_, TokenType type_, TokenColor color_, TokenTriggers triggers_) {
			token_.StartIndex = char_index_1st_;
			token_.EndIndex = char_index_last_;
			token_.Token = (int)EslTokenFlags._0;
			token_.Color = color_;
			token_.Type = type_;
			token_.Trigger = triggers_;
			return char_index_last_;
		}
		int fill_token(TokenInfo token_, int char_index_1st_, int char_index_last_, TokenType type_, TokenColor color_) {
			return fill_token(token_, char_index_1st_, char_index_last_, type_, color_, TokenTriggers.None);
		}
		//
		int fill_token_number(TokenInfo token_, int char_index_1st_, int char_index_last_) {
			return fill_token(token_, char_index_1st_, char_index_last_, TokenType.Literal, TokenColor.Number);
		}
		int fill_token_number(TokenInfo token_, int char_index_1st_) {
			return fill_token_number(token_, char_index_1st_, m_source_len - 1);
		}
		int fill_token_date(TokenInfo token_, int char_index_1st_, int char_index_last_) {
			return fill_token(token_, char_index_1st_, char_index_last_, TokenType.Literal, TokenColor.Number);    //TODO TokenColor.Number - можно другой цвет
		}
		int fill_token_comment_line(TokenInfo token_, int char_index_1st_) {
			return fill_token(token_, char_index_1st_, m_source_len - 1, TokenType.LineComment, TokenColor.Comment);
		}
		int make_token_string_in_quotes(TokenInfo token_, int char_index_1st_, int char_index_last_) {
			return fill_token(token_, char_index_1st_, char_index_last_, TokenType.String, TokenColor.String);
		}
		int make_token_string_multyline_begin(TokenInfo token_, int char_index_1st_, int char_index_last_, ref int in_out_state_) {
			state_set_string_multyline_begin(ref in_out_state_);
			return fill_token(token_, char_index_1st_, char_index_last_, TokenType.String, TokenColor.String);
		}
		int make_token_string_multyline_middle(TokenInfo token_, int char_index_1st_, int char_index_last_) {
			return fill_token(token_, char_index_1st_, char_index_last_, TokenType.String, TokenColor.String);
		}
		int make_token_string_multyline_final(TokenInfo token_, int char_index_1st_, int char_index_last_, ref int in_out_state_) {
			state_set_string_multyline_final(ref in_out_state_);
			return fill_token(token_, char_index_1st_, char_index_last_, TokenType.String, TokenColor.String);
		}
		int make_token_string(TokenInfo token_info_, int char_index_of_1st_quote_, ref int in_out_state_) {
			int char_index_next = char_index_of_1st_quote_ + 1;
			if (char_index_next < m_source_len)
			{
				int index_of_next_quote = m_source.IndexOf('"', char_index_next);
				if (index_of_next_quote == -1)
					// в этой строке мы больще кавычек не нашли. это начало "многострочной" строки до конца строки (или ош видно будет потом)
					return make_token_string_multyline_begin(token_info_, char_index_of_1st_quote_, m_source_len - 1, ref in_out_state_);
				else
					return make_token_string_in_quotes(token_info_, char_index_of_1st_quote_, index_of_next_quote);
			}
			else
				// на этой строке больше ничего кроме кавычки нет. это начало "многострочной" строки (или ош видно будет потом)
				return make_token_string_multyline_begin(token_info_, char_index_of_1st_quote_, char_index_of_1st_quote_, ref in_out_state_);
		}
		int fill_token_operator(TokenInfo token_, int char_index_1st_) {
			return fill_token(token_, char_index_1st_, char_index_1st_, TokenType.Operator, TokenColor.Text);
		}
		int fill_token_bra_l_bra(TokenInfo token_, int char_index_1st_) {
			return fill_token(token_, char_index_1st_, char_index_1st_, TokenType.Text, TokenColor.Text, TokenTriggers.ParameterStart);
		}
		int fill_token_bra_r_bra(TokenInfo token_, int char_index_1st_) {
			return fill_token(token_, char_index_1st_, char_index_1st_, TokenType.Text, TokenColor.Text, TokenTriggers.ParameterEnd);
		}
		int make_token_param_separator(TokenInfo token_, int char_index_1st_) {
			return fill_token(token_, char_index_1st_, char_index_1st_, TokenType.Text, TokenColor.Text, TokenTriggers.ParameterNext);
		}
		int fill_token_white(TokenInfo token_, int char_index_1st_, int char_index_last_) {
			return fill_token(token_, char_index_1st_, char_index_last_, TokenType.WhiteSpace, TokenColor.Text);
		}
		int fill_token_error(TokenInfo token_, int char_index_1st_, int char_index_last_) {
			return fill_token(token_, char_index_1st_, char_index_last_, TokenType.Unknown, TokenColor.Text);  //TODO TokenColor.Text - нужно свой цвет
		}
		int fill_token_delimiter(TokenInfo token_, int char_index_1st_) {
			return fill_token(token_, char_index_1st_, char_index_1st_, TokenType.Delimiter, TokenColor.Text, TokenTriggers.MemberSelect);
		}
	} // class BslTokensScaner

} // namespace EsbStudio.Esl
