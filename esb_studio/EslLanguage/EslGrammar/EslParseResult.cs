using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
//
using Microsoft.VisualStudio.Package;
#if DEBUG || DEBUG_IN_RELEASE
	using System.Diagnostics;
#endif


namespace EsbStudio.Esl
{
	internal static class EslParseResult 
	{
		internal static readonly Methods		NoMethods_		= new EslMethDeclareMethods_0();
		internal static readonly Declarations	NoDeclarations_ = new EslMethDeclarations_0();
		//
		//TODO Нужно какое-то логирование и (оч-бы неплохо) показать юзеру как-то. хотя бы на первых порах в целях отработки алгоритмов
		//		пока просто возвращаем No... как будто бы ничего не произошло
		internal static Methods ErrorParseMethods(Exception ex_, [System.Runtime.CompilerServices.CallerMemberName] string invoker_ = "") {
			Debug.Assert(false);
			return NoMethods_;
		}
		internal static Declarations ErrorParseDeclarations(Exception ex_, [System.Runtime.CompilerServices.CallerMemberName] string invoker_ = "") {
			Debug.Assert(false);
			return NoDeclarations_;
		}
		internal static EslDecl ErrorParseDecl(Exception ex_, [System.Runtime.CompilerServices.CallerMemberName] string invoker_ = "") {
			Debug.Assert(false);
			return null;
		}
		internal static string ErrorParseString(Exception ex_, [System.Runtime.CompilerServices.CallerMemberName] string invoker_ = "") {
			Debug.Assert(false);
			return null;
		}

	}

	internal class EslMethDeclareMethods_0 : Methods
	{
		public override int GetCount()							{ return 0; }
		public override string GetDescription(int meth_index_)	{ throw Errors.ArgumentException(this, nameof(meth_index_), meth_index_); }
		public override string GetName(int meth_index_)			{ throw Errors.ArgumentException(this, nameof(meth_index_), meth_index_); }
		public override int GetParameterCount(int meth_index_)	{ throw Errors.ArgumentException(this, nameof(meth_index_), meth_index_); }
		public override void GetParameterInfo(int meth_index_, int para_index_, out string out_name_, out string out_display_, out string out_description_) 
																{ throw Errors.ArgumentException(this, nameof(meth_index_), meth_index_); }
		public override string GetType(int meth_index_)			{ throw Errors.ArgumentException(this, nameof(meth_index_), meth_index_); }
	}
	internal class EslMethDeclarations_0 : Declarations
	{
		public override int GetCount()							{ return 0;	}
		public override string GetDescription(int meth_index_)	{ throw Errors.ArgumentException(this, nameof(meth_index_), meth_index_); }
		public override string GetDisplayText(int meth_index_)	{ throw Errors.ArgumentException(this, nameof(meth_index_), meth_index_); }
		public override int GetGlyph(int meth_index_)			{ throw Errors.ArgumentException(this, nameof(meth_index_), meth_index_); }
		public override string GetName(int meth_index_)			{ throw Errors.ArgumentException(this, nameof(meth_index_), meth_index_); }
	}
	internal class EslParserResultForScope_Null : AuthoringScope 
	{
		static Declarations m_declarations = new EslMethDeclarations_0();
		static Methods		m_methods = new EslMethDeclareMethods_0();
		public override string GetDataTipText(int line, int col, out Microsoft.VisualStudio.TextManager.Interop.TextSpan out_text_span_tip_show_) 
		{
			out_text_span_tip_show_ = Util.SpanEmpty_;
			return null;
		}
		public override Declarations GetDeclarations(Microsoft.VisualStudio.TextManager.Interop.IVsTextView view, int line, int col, TokenInfo info, ParseReason reason) 
		{
			return m_declarations;
		}
		public override Methods GetMethods(int line, int col, string name) 
		{
			return m_methods;
		}
		public override string Goto(Microsoft.VisualStudio.VSConstants.VSStd97CmdID cmd, Microsoft.VisualStudio.TextManager.Interop.IVsTextView textView, 
																				int line, int col, out Microsoft.VisualStudio.TextManager.Interop.TextSpan out_text_uri_target_) 
		{
			out_text_uri_target_ = Util.SpanEmpty_;
			return null;
		}
	}



} // namespace EsbStudio.Esl
