using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
#if DEBUG
	using System.Diagnostics;
#endif



namespace EsbStudio 
{
	internal static class Util
	{
		public static readonly Microsoft.VisualStudio.TextManager.Interop.TextSpan SpanEmpty_;
	}


	internal static class Errors
	{
		public static ArgumentException ArgumentException(object obj_, string arg_name_, object arg_value_, [System.Runtime.CompilerServices.CallerMemberName] string memb_name_ = "") {
			return new ArgumentException($"{obj_.ToString()}::{memb_name_}: Invalid {arg_name_} := {arg_value_}.");
		}
	}
}

namespace EsbStudio.Esl
{
	internal class BslKeywords
	{
		public static string[] KeywordsArrayEn_ = new string[]
		{
			"AND",
			"BREAK",
			"CONTINUE",
			"DO",
			"EACH",
			"ELSE",
			"ELSIF",
			"ENDDO",
			"ENDFUNCTION",
			"ENDIF",
			"ENDPROCEDURE",
			"ENDTRY",
			"EXCEPT",
			"EXPORT",
			"FALSE",
			"FOR",
			"FUNCTION",
			"IF",
			"IN",
			"NOT",
			"NULL",
			"OR",
			"PROCEDURE",
			"RAISE",
			"RETURN",
			"THEN",
			"TO",
			"TRUE",
			"TRY",
			"UNDEFINED",
			"USE",
			"WHILE",
			"VAR"
		};
		//
		public static HashSet<string> KeywordsSetEn_ = new HashSet<string>(KeywordsArrayEn_, StringComparer.OrdinalIgnoreCase);
		//
		public static bool is_keyword_char_1st_en(char ch_) {
			switch (ch_)
			{
				case 'A': case 'a':
				case 'B': case 'b':
				case 'C': case 'c':
				case 'D': case 'd':
				case 'E': case 'e':
				case 'F': case 'f':
				case 'I': case 'i':
				case 'N': case 'n':
				case 'O': case 'o':
				case 'P': case 'p':
				case 'R': case 'r':
				case 'T': case 't':
				case 'U': case 'u':
				case 'W': case 'w':
				case 'V': case 'v':
					return true;
				default:
					return false;
			}
		}

		public static string[] KeywordsArrayRu_ = new string[] 
		{
			"NULL",		//NULL он и в русском NULL
			"ВОЗВРАТ",
			"ВЫЗВАТЬИСКЛЮЧЕНИЕ",
			"ДЛЯ",
			"ЕСЛИ",
			"И",
			"ИЗ",
			"ИЛИ",
			"ИНАЧЕ",
			"ИНАЧЕЕСЛИ",
			"ИСКЛЮЧЕНИЕ",
			"ИСПОЛЬЗОВАТЬ",
			"ИСТИНА",
			"КАЖДОГО",
			"КОНЕЦЕСЛИ",
			"КОНЕЦПОПЫТКИ",
			"КОНЕЦПРОЦЕДУРЫ",
			"КОНЕЦФУНКЦИИ",
			"КОНЕЦЦИКЛА",
			"ЛОЖЬ",
			"НЕ",
			"НЕОПРЕДЕЛЕНО",
			"ПЕРЕМ",
			"ПО",
			"ПОКА",
			"ПОПЫТКА",
			"ПРЕРВАТЬ",
			"ПРОДОЛЖИТЬ",
			"ПРОЦЕДУРА",
			"ТОГДА",
			"ФУНКЦИЯ",
			"ЦИКЛ",
			"ЭКСПОРТ",
		};
		//
		public static HashSet<string> KeywordsSetRu_ = new HashSet<string>(KeywordsArrayRu_, StringComparer.OrdinalIgnoreCase);

		public static bool is_keyword_char_1st_ru(char ch_) {
			switch (ch_) { 
				case 'N': case 'n':
				case 'В': case 'в':
				case 'Д': case 'д':
				case 'Е': case 'е':
				case 'И': case 'и':
				case 'К': case 'к':
				case 'Л': case 'л':
				case 'Н': case 'н':
				case 'П': case 'п':
				case 'Т': case 'т':
				case 'Ф': case 'ф':
				case 'Ц': case 'ц':
				case 'Э': case 'э':
					return true;
				default:
					return false;
			}
		}

	public static bool get_keyword_ru(string source_line_, char ch_1st_, int index_1st_, int len_, ref string keyword_) {
		if (is_keyword_char_1st_ru(ch_1st_))
		{
			string s_token = source_line_.Substring(index_1st_, len_);
			bool b_try_keyword = KeywordsSetRu_.TryGetValue(s_token, out keyword_);
			return b_try_keyword;
		}
			
		return false;
	}

	public static bool is_keyword_ru(string source_line_, char ch_1st_, int index_1st_, int len_) {
			if (is_keyword_char_1st_ru(ch_1st_))
			{
				string s_name = source_line_.Substring(index_1st_, len_);
				return KeywordsSetRu_.Contains(s_name);
			}
			return false;
		}
		public static bool is_keyword_en(string source_line_, char ch_1st_, int index_1st_, int len_) {
			if (is_keyword_char_1st_en(ch_1st_))
			{
				string s_name = source_line_.Substring(index_1st_, len_);
				return KeywordsSetEn_.Contains(s_name);
			}
			return false;
		}
		public static bool is_keyword(string source_line_, char ch_1st_, int index_1st_, int len_) {
			return (is_keyword_ru(source_line_, ch_1st_, index_1st_, len_) || is_keyword_en(source_line_, ch_1st_, index_1st_, len_));
		}
	} // class BslKeywords

} // namespace EsbStudio.Esl
