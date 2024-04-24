using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
#if DEBUG || DEBUG_IN_RELEASE
	using System.Diagnostics;
#endif
//
using Microsoft.VisualStudio;
using Microsoft.VisualStudio.Package;
using Microsoft.VisualStudio.TextManager.Interop;
//
using System.Runtime.InteropServices;   //Marshal.ThrowExceptionForHR(hr);



namespace EsbStudio.Esl
{
	internal sealed class EslSource : Source
	{
		MethodData	m_ref_method_data;
		TokenInfo[] m_ref_last_GetTokenInfo_LineInfo;
		int			m_ref_last_GetTokenInfo_Line;

		public override Microsoft.VisualStudio.Package.MethodData CreateMethodData() 
		{
			// для того чтобы контролировать поведение MethodTip нам нужна ссылка на ...
			// но Source ее на отдает, но вызывает этот метод для создания в конструкторе.
			// public virtual MethodData CreateMethodData() => new MethodData(this.service.Site);
			//  перехватываем и запоминаем,
			//	!!НО КАРЛ!! вызов виртуальных методов в конструкторе МОВЕТОН! Или в шарпах это нормально?	TODO

			m_ref_method_data = base.CreateMethodData();
			return m_ref_method_data;
		}
		public EslSource(LanguageService service_, IVsTextLines text_lines_, Colorizer colorizer_) : base(service_, text_lines_, colorizer_)
		{ }
		//Пока у меня нет внутри никакого объекта требующего финализации я могу не переимплементировать IDispose, иначе см.сюда
		//https://learn.microsoft.com/en-us/dotnet/standard/garbage-collection/implementing-dispose#cascade-dispose-calls
		//++
		//https://metanit.com/sharp/tutorial/8.2.php
		public override void Dispose() 
		{
			/*	** Если ваш класс имеет поле или свойство IDisposable , но не владеет им, то есть класс не создает объект, тогда классу не нужно реализовывать IDisposable .
			 *	1. МЫ СЕБЯ запечатали, поэтому нам всякое .Dispose(bool) не надо. А у Source его и нет.
			 *	Если у нас появятся какие-то поля которые мы будем сами создавать через new, то для каждого нужен блок типа (из кода того же Source)
			 *		try
					{
						if (this.methodData != null)
						{
						  this.methodData.Dispose();
						  this.methodData = (MethodData) null;
						}
					}
					catch (COMException ex)
					{ }
				Если появятся поля на которые у нас просто ссылки, но мы их не создавали а откуда-то получили, то ...
					try
					{
					  if (this.colorState != null)
					    this.colorState = (IVsTextColorState) null;
					}
					catch (COMException ex)
					{ }
				Если будет что-то unmanaged, то что-то типа
					Marshal.Release(this.pUnkTextLines);
					SafeHandle..._safeHandle.Dispose();	и т.п. смотреть надо
			 */

			try
			{
				if (m_ref_method_data != null)
					m_ref_method_data = null;
				if(m_ref_last_GetTokenInfo_LineInfo != null)
					m_ref_last_GetTokenInfo_LineInfo = null;
			}
			catch (COMException ex)
			{ }

			// В конце ВСЕГДА диспозим базового и предыдущее все ОБЯЗАНО быть в try чтобы мы СЮДА ДОБРАЛИСЬ!
			base.Dispose();
		}
		//TODO	С деструктором их непонял до конца, но у базового он пустой..
		//-------------------------------------------------------------------

		/*	TODO	Ругается на Microsoft.VisualStudio.NativeMethods.ThrowOnFailure
		 *				error CS0122: "NativeMethods" недоступен из-за его уровня защиты.
		 *			странно раньше по хелпу он был Паблик, а теперь dotPeek показывает его как internal - т.е. для самого .Package он доступен а мне нет..
		 *			или я что-то непонял..
		 *	Просто выдернул оттуда код.
		 *	ПС. Failed где-то "нормальная" доступная реализация есть, но и она везде такая..
		 */
		public static bool NativeMethods_Failed(int hr) => hr < 0;
		public static int NativeMethods_ThrowOnFailure(int hr, params int[] expectedHRFailure) {
			if (NativeMethods_Failed(hr) && (expectedHRFailure == null || Array.IndexOf<int>(expectedHRFailure, hr) < 0))
				Marshal.ThrowExceptionForHR(hr);
			return hr;
		}
		public static int NativeMethods_ThrowOnFailure(int hr) => NativeMethods_ThrowOnFailure(hr, (int[])null);
		//-------------------------------------------------------------------

		public string GetEslModuleName() 
		{
			//TODO	закэшировать
			
			//TODO	В моей "EsbStudio" (или по правилам 1С, хз.., но пока так) файлы лежат по директориям и собственно имя модуля это имя папки на два шага вверх
			//d:\_ida_work_9_\1C\$work\esb-dev\esb_studio\EsbStudio\ProjectTemplateVCProject\1s_files\CommonModules\EsbCommon\Ext\Module.bsl
			// или //d:\_ida_work_9_\1C\$work\esb-dev\esb_studio\EsbStudio\ProjectTemplateVCProject\1s_files\Ext\OrdinaryApplicationModule.bsl
			string s_file_path = this.GetFilePath();
			string s_file_name = System.IO.Path.GetFileNameWithoutExtension(s_file_path);
			if (String.Equals(s_file_name, "Module", StringComparison.OrdinalIgnoreCase)) {
				string s_dir1 = System.IO.Path.GetDirectoryName(s_file_path);
				string s_dir2 = System.IO.Path.GetDirectoryName(s_dir1);
				string s_name = System.IO.Path.GetFileName(s_dir2);
				return s_name;
			}
			else
				return s_file_name;
		}

		public TokenInfo[] GetLineTokens(int line_) 
		{
			// Вот здесь стандартный Colorizer::GetLineInfo() старается по возможности вернуть последнюю заэшированную ->	return this.cachedLineInfo; 
			// т.е. полный массив токенов строки ВСЕГДА запрашивается (или пересоздается)
			// НО! Это не учитывается и не отдается дальше для парсинга, а отдается ТОЛЬКО ПОСЛЕДНИЙ ТОКЕН
			//TokenInfo[] lineInfo = this.GetColorizer().GetLineInfo(this.GetTextLines(), line, this.ColorState);
			m_ref_last_GetTokenInfo_Line = line_;
			m_ref_last_GetTokenInfo_LineInfo = this.GetColorizer().GetLineInfo(this.GetTextLines(), line_, this.ColorState);
			//
			return m_ref_last_GetTokenInfo_LineInfo;
		}
		public override TokenInfo GetTokenInfo(int line_, int col_) 
		{	/* Из Source::GetTokenInfo(...)
		 	 * 
		 	*/
			TokenInfo info = new TokenInfo();
			TokenInfo[] line_tokens = GetLineTokens(line_);
			if (line_tokens != null)
			{
				if (col_ > 0)
					--col_;
				this.GetTokenInfoAt(line_tokens, col_, ref info);
			}
			return info;
		}

		//TODO	Правильным решением было-бы не "кэшировать" эту информацию здесь, а упаковать ее в свой AuthoringSink (а не только последний токен)
		//		НО! Там много оверрайдить надо. Пока так..
		public TokenInfo[] GetLineTokensLastQueried_TODO(EslParseRequest request_) {
			//TODO	Это наше m_ref_last_GetTokenInfo_LineInfo нужно засунуть в наш же EslParseRequest в OnCommand
			//		а в тех местах где парсится по тригеру просто брать. в иных местах НЕ брать, а брать GetLineTokens - колорайзер тоже кэширует
			Debug.Assert(m_ref_last_GetTokenInfo_Line == request_.Line);
			if (m_ref_last_GetTokenInfo_Line != request_.Line)
				return null;
			return m_ref_last_GetTokenInfo_LineInfo;
		}

		public override AuthoringSink CreateAuthoringSink(ParseReason parse_request_reason_, int parse_request_line_, int parse_request_col_) 
		{
			//int maxErrorMessages = this.service.Preferences.MaxErrorMessages;
			//AuthoringSink(reason, line, col, maxErrorMessages);

			//TODO	!!! Их дурацкий AuthoringSink от которого мы вынуждены наследоваться и функциональность которого мы переопределить не можем и дублируем
			//		!!! Еще к тому же считает, что параметры учитывать для причины MemberSelect НЕНУЖНО
			//		!!! (и для остальных причин тоже см. в EslParseSink
			//		!!! и чтобы наши алгоритмы работали нужно ему установить причину либо QuickInfo либо MethodTip
			//		!!! ТАКЖЕ эта зараза не будет считать параметры (и имена) в режиме HighlightBraces, MemberSelectAndHighlightBraces, MatchBraces
			//			и здесь никаких пересечений по режимам с QuickInfo или CompleteWord (хотя возможно и действительно для скобок ненадо)
			//		ПОЭТОМУ тупо переопределяем, НО НУЖНО переписывать этот класс и связанные с ним обработки в потрохах Source и возможно еще где
			//		ПОМНИМ, что теперь у нас parse_request_reason_ НЕ РАВНЯЕТСЯ parse_sink_reason_
			ParseReason sink_reason;
			switch (parse_request_reason_) {
				case ParseReason.HighlightBraces:
				case ParseReason.MemberSelectAndHighlightBraces:
				case ParseReason.MatchBraces:
					sink_reason = parse_request_reason_;
					break;
				default:
					sink_reason = ParseReason.MethodTip;	// ПЕРЕОПРЕДЕЛЯЕМ
					break;
			}

			return new EslParseSink(sink_reason, parse_request_line_, parse_request_col_, this.LanguageService.Preferences.MaxErrorMessages); 
		}

		public override void OnCommand(Microsoft.VisualStudio.TextManager.Interop.IVsTextView textView, VSConstants.VSStd2KCmdID command, char ch) 
		{
			Debug.WriteLine($"EslSource::OnCommand ch='{ch}'");

			if (textView == null || this.LanguageService == null || !this.LanguageService.Preferences.EnableCodeSense)
				return;

			const VSConstants.VSStd2KCmdID CMD_1_TYPECHAR = VSConstants.VSStd2KCmdID.TYPECHAR;
			const VSConstants.VSStd2KCmdID CMD_2_BACKSPACE = VSConstants.VSStd2KCmdID.BACKSPACE;
			const VSConstants.VSStd2KCmdID CMD_5_BACKTAB = VSConstants.VSStd2KCmdID.BACKTAB;
			const VSConstants.VSStd2KCmdID CMD_7_LEFT = VSConstants.VSStd2KCmdID.LEFT;
			const VSConstants.VSStd2KCmdID CMD_8_LEFT_EXT = VSConstants.VSStd2KCmdID.LEFT_EXT;

			bool flag = command == CMD_2_BACKSPACE || command == CMD_5_BACKTAB || command == CMD_7_LEFT || command == CMD_8_LEFT_EXT;
			
			int piLine;
			int piColumn;
			NativeMethods_ThrowOnFailure(textView.GetCaretPos(out piLine, out piColumn));

			TokenInfo tokenInfo = this.GetTokenInfo(piLine, piColumn);

			TokenTriggers trigger = tokenInfo.Trigger;
			if ((trigger & TokenTriggers.MemberSelect) != TokenTriggers.None && command == CMD_1_TYPECHAR)
			{
				ParseReason reason = ((trigger & TokenTriggers.MatchBraces) == TokenTriggers.MatchBraces)? ParseReason.MemberSelectAndHighlightBraces : ParseReason.MemberSelect;
				this.Completion(textView, tokenInfo, reason);
			}
			else if ((trigger & TokenTriggers.MatchBraces) != TokenTriggers.None && 
								this.LanguageService.Preferences.EnableMatchBraces && 
								command != CMD_2_BACKSPACE && 
								(command == CMD_1_TYPECHAR || this.LanguageService.Preferences.EnableMatchBracesAtCaret))
				this.MatchBraces(textView, piLine, piColumn, tokenInfo);

			if ((trigger & TokenTriggers.MethodTip) != TokenTriggers.None && this.m_ref_method_data.IsDisplayed)
			{
				if ((trigger & TokenTriggers.MethodTip) == TokenTriggers.ParameterNext)
					this.m_ref_method_data.AdjustCurrentParameter(!flag || piColumn <= 0 ? 1 : -1);
				else
					this.MethodTip(textView, piLine, !flag || piColumn <= 0 ? piColumn : piColumn - 1, tokenInfo);
			}
			else if ((trigger & TokenTriggers.MethodTip) != TokenTriggers.None && command == CMD_1_TYPECHAR && this.LanguageService.Preferences.ParameterInformation)
			{
				this.MethodTip(textView, piLine, piColumn, tokenInfo);
			}
			else
			{
				if (!this.m_ref_method_data.IsDisplayed)
					return;
				//TODO	Здесь вызывается наш DoParse_OnMethodTip БЕЗ ТРИГГЕРА для "режима продолжения". см.TODO в DoParse_OnMethodTip
				//		Для нас "режим продолжения" пока недоступен, т.к. мы не умеем определять тип параметра и собственно методов перегруженных по типу параметра у нас пока немного
				//		просто отключу этот вызов
				// this.MethodTip(textView, piLine, piColumn, tokenInfo);
			}

		}
	} // class EslSource
}
