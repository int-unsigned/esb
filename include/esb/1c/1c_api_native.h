/*
	"ESB" is designed to create external native components for the 1C(tm) program.
	Copyright © 2023 IntUnsigned	(v8classes@gmail.com)
	"ESB" is free software under LGPLv2.1 license with essential restriction about binary part (view esb-license.txt)
	"ESB" IS DISTRIBUTED "AS-IS" WITHOUT ANY, EVEN THE IMPLIED, WARRANTY. IN NO EVENT SHALL THE AUTHORS OR THE COPYRIGHT OWNER BE LIABLE FOR ANY CLAIM,
	DAMAGES OR OTHER LIABILITY ARISING OUT OF THE USE OF THIS SOFTWARE.
	THE AUTHOR DOES NOT GIVE ANY GUARANTEES AND DOES NOT BEAR ANY RESPONSIBILITY REGARDING THE USE OF 1C(tm) COMPONENTS PRODUCED USING THE ESB LIBRARY IN THE 1C(tm) ENVIRONMENT.
*/

#pragma once
#ifndef ESB_API_NATIVE_H_
#define ESB_API_NATIVE_H_

#include "esb/esb_base.h"
//
#include <concepts>
#include <compare>


namespace esb
{
	// api native (not present in contexts)
	// 
	// КОНСТРУКТОРЫ ЗНАЧЕНИЙ (которые Boolean, Numeric и т.п. - у нас здесь это имена классов - поэтому к функциям добавил To...)
	// NOTE	Возможно классам сделать такие шаблонные конструкторы и будет в точность как в 1С, но... конструкторы иногда вызываются имплисит 
	//		или вооще неожиданно или конфликтовать начинают.. Не считаю целесообразным это делать.
	template<class EsbClassT>
	inline Boolean ToBoolean(const EsbClassT& Value_) {
		if constexpr (std::is_same_v<std::remove_cvref_t<EsbClassT>, Numeric>)
			return (static_cast<int>(Value_))? Boolean::True_ : Boolean::False_;
		else if constexpr (has_meth_ToBoolean<EsbClassT>)
			return Value_.ESB_CONVERTER_METH_ToBoolean();
		else {
			IValuePtr pv = IValue_ToBoolean(*get_interface(Value_));
			if (pv)
				return make<Boolean>(std::move(pv));
			else
				ESL_THROW_NOTBOOL();
		}
	}

	template<class EsbClassT>
	inline Numeric ToNumeric(const EsbClassT& Value_) {
		if constexpr (std::is_same_v<std::remove_cvref_t<EsbClassT>, Boolean>)
			return (static_cast<bool>(Value_))? Numeric::Value_1_ : Numeric::Value_0_;
		else if constexpr (has_meth_ToNumeric<EsbClassT>)
			return Value_.ESB_CONVERTER_METH_ToNumeric();
		else {
			//Значение типа Строка преобразуется в число в том случае, если параметр является строковым представлением литерала численного типа.
			//При этом строковое представление анализируется как с учетом региональных установок информационной базы, так и без их учета.
			//В противном случае будет выдано сообщение об ошибке выполнения функции.
			IValuePtr pv = IValue_ToNumeric(*get_interface(Value_));
			if (pv)
				return make<Numeric>(std::move(pv));
			else
				ESL_THROW_NOTNUMBER();
		}
	}
	template<class EsbClassT>
	inline Numeric ToNumber(const EsbClassT& Value_) {
		return ToNumeric(Value_);
	}
	// специальный случая для Boolean т.к. может быть constexpr и возвращать лвал (это используется в Min/Max)
	inline constexpr const Numeric& ToNumeric(const Boolean& Value_) {
		return (Value_) ? Numeric::Value_1_ : Numeric::Value_0_;
	}
	inline constexpr const Numeric& ToNumber(const Boolean& Value_) {
		return ToNumeric(Value_);// (Value_) ? Numeric::Value_1_ : Numeric::Value_0_;
	}

	template<class EsbClassT>
	inline String ToString(const EsbClassT& Value_) {
		if constexpr (has_meth_ToString<EsbClassT>)
			return Value_.ESB_CONVERTER_METH_ToString();
		else {
			// Гхм... --Преобразование к строке значений остальных типов производится **по мере возможности** и 
			//			обычно содержит **читабельное** представление значения.---> ??
			// будем бросать ошибку по аналогии с другими методами.
			IValuePtr pv = IValue_ToString(*get_interface(static_cast<const Value&>(Value_)));
			if (pv)
				return make<String>(std::move(pv));
			else
				ESL_THROW_NOTSTRING();
		}
	}

	template<class EsbClassT>
	inline DateTime ToDate(const EsbClassT& Value_) {
		if constexpr (has_meth_ToDate<EsbClassT>)
			return Value_.ESB_CONVERTER_METH_ToDate();
		else {
			IValuePtr pv = IValue_ToDate(*get_interface(Value_));
			if (pv)
				return make<DateTime>(std::move(pv));
			else
				ESL_THROW_NOTDATE();
		}
	}
	//DateTime Date(ConstPara<Numeric> Year_, ConstPara<Numeric> Month_, ConstPara<Numeric> Day_, ArgByValOpt<Numeric> Hour_ = Undef::Value_, ArgByValOpt<Numeric> Minute_ = Undef::Value_, ArgByValOpt<Numeric> Second_ = Undef::Value_)
	//Numeric NumberFromBinaryString(ConstPara<StringValue> BinaryString_)
	//---------------------------------





	// template<std::same_as<Numeric>... NumericT> 
	// ClassMix<Numeric, StringValue, DateTime, BoolValue> 
	// Min(ConstPara<Numeric> Value1_, ArgErrUnk<Arbitrary> _, ConstPara<NumericT> ...ValueN_)
	// 
	// Тот или иной семантический вариант функции определяется по типу данных первого параметра. 
	// В случае несовпадения типа второго и последующих параметров с требуемым, их значения преобразуются к требуемому типу в соответствии с правилами преобразования типов. 
	// Если тип первого параметра не соответствует ни одному из допустимых типов, 
	// то в зависимости от ситуации может производиться преобразование типов или возбуждаться состояние ошибки исполнения.
	// Гхм...
	//	- если первый Дата, то остальные  должны быть Дата и ничего никуда не конвертируется, или ошибка. Сравнение просто <
	//	- если первый Строка, то остальные должны быть строка и ничего никуда не конвертируется. сравнивается через коллатор
	//	- если первый Число, то остальные могут быть либо Число, либо Буул (Буул приводится к 0/1). Возвращается минимальное: т.е. Мин(10, Истина) дает Буул::Истина (а не число!)
	//	- если первый Буул, то тоже что для числа. Также Мин(Истина,0) возвращает Число::0
	//		то есть Буул в целях сравнения всегда тихо конвертируется в число, и возвращается собственно говоря (минимальное)
	//		каждое очередное сравнение может перещелкивать тип результата (ну неболее двух раз)
	//	допускается вызов с одним аргументом

	template<class EsbClassT>
	struct minmax_comparator_t {
		const EsbClassT& data_;
		const minmax_comparator_t<EsbClassT>& operator<(const minmax_comparator_t<EsbClassT>& other_) const	{ return (data_ < other_.data_)? *this : other_; }
		const minmax_comparator_t<EsbClassT>& operator>(const minmax_comparator_t<EsbClassT>& other_) const { return (data_ > other_.data_)? *this : other_; }
	};

	template<std::same_as<DateTime>... DataTypeT>
	DateTime Min(const DateTime& a1_, const DataTypeT&... ax_) {
		//1) ( pack op ... ):	Unary right fold (E op ...) becomes (E1 op (... op (EN-1 op EN)))
		//2) ( ... op pack ):	Unary left fold(... op E) becomes(((E1 op E2) op ...) op EN)
		//3) ( pack op ... op init ):	Binary right fold (E op ... op I) becomes (E1 op (... op (EN−1 op (EN op I))))
		//4) ( init op ... op pack ):	Binary left fold (I op ... op E) becomes ((((I op E1) op E2) op ...) op EN)
		//for Min<>(a1_, __ax_1, __ax_2)
		//return (cmp_t{ a1_ }).operator<(cmp_t{ __ax_1 }).operator<(cmp_t{ __ax_2 }).it_;
		//eval from left to right -> a1 compares with __ax_1, then result compares with __ax_2 and return result of second comparision. cycle not used
		return (minmax_comparator_t{ a1_ } < ... < minmax_comparator_t{ ax_ }).data_;
	}
	template<std::same_as<String>... DataTypeT>
	String Min(const String& a1_, const DataTypeT&... ax_) {
		return (minmax_comparator_t{ a1_ } < ... < minmax_comparator_t{ ax_ }).data_;
	}
	template<std::same_as<Numeric>... DataTypeT>
	Numeric Min(const Numeric& a1_, const DataTypeT&... ax_) {
		return (minmax_comparator_t{ a1_ } < ... < minmax_comparator_t{ ax_ }).data_;
	}
	template<std::same_as<Boolean>... DataTypeT>
	Boolean Min(const Boolean& a1_, const DataTypeT&... ax_) {
		return (minmax_comparator_t{ a1_ } < ... < minmax_comparator_t{ ax_ }).data_;
	}

	// мал клоп, да вонюч...
	template<class EsbClassT>
	concept NumericOrBooleanConcept = (std::same_as<Boolean, EsbClassT> || std::same_as<Numeric, EsbClassT>);

	using NumericOrBooleanType = OneOf<Numeric, Boolean>;

	struct minmax_comparator_nb {
		const Value&	value_;
		const Numeric*	pcomp_;
		constexpr minmax_comparator_nb(const Numeric& val_) noexcept : value_(val_), pcomp_(&val_)				{}
		constexpr minmax_comparator_nb(const Boolean& val_) noexcept : value_(val_), pcomp_(&ToNumber(val_))	{}
		minmax_comparator_nb(const minmax_comparator_nb&) = delete;
		minmax_comparator_nb& operator=(const minmax_comparator_nb&) = delete;
		minmax_comparator_nb& operator=(minmax_comparator_nb&&) = delete;
		const minmax_comparator_nb& operator<(const minmax_comparator_nb& other_) const		{ return (*pcomp_ < *other_.pcomp_)? *this : other_; }
		const minmax_comparator_nb& operator>(const minmax_comparator_nb& other_) const		{ return (*pcomp_ > *other_.pcomp_)? *this : other_; }
		NumericOrBooleanType make_object() const											{ return make_copy<NumericOrBooleanType>(get_interface(value_));	}
	};

	template<NumericOrBooleanConcept Arg1T, NumericOrBooleanConcept... ArgsT>
	NumericOrBooleanType Min(const Arg1T& a1_, const ArgsT&... ax_) {
		return (minmax_comparator_nb{ a1_ } < ... < minmax_comparator_nb{ ax_ }).make_object();
	}


	template<std::same_as<DateTime>... DataTypeT>
	DateTime Max(const DateTime& a1_, const DataTypeT&... ax_) {
		return (minmax_comparator_t{ a1_ } > ... > minmax_comparator_t{ ax_ }).data_;
	}
	template<std::same_as<String>... DataTypeT>
	String Max(const String& a1_, const DataTypeT&... ax_) {
		return (minmax_comparator_t{ a1_ } > ... > minmax_comparator_t{ ax_ }).data_;
	}
	template<std::same_as<Numeric>... DataTypeT>
	Numeric Max(const Numeric& a1_, const DataTypeT&... ax_) {
		return (minmax_comparator_t{ a1_ } > ... > minmax_comparator_t{ ax_ }).data_;
	}
	template<std::same_as<Boolean>... DataTypeT>
	Boolean Max(const Boolean& a1_, const DataTypeT&... ax_) {
		return (minmax_comparator_t{ a1_ } > ... > minmax_comparator_t{ ax_ }).data_;
	}
	template<NumericOrBooleanConcept Arg1T, NumericOrBooleanConcept... ArgsT>
	NumericOrBooleanType Max(const Arg1T& a1_, const ArgsT&... ax_) {
		return (minmax_comparator_nb{ a1_ } > ... > minmax_comparator_nb{ ax_ }).make_object();
	}



	// навязываем использование ErrorInfo только в блоке catch
	inline ErrorInfoValue ErrorInfo(const core::Exception& e_) {
		return make<ErrorInfoValue>(create_error_value(e_));
	}

	//TOBE: ErrorDescription как-то формируется из полей ErrorInfoValue. надо разобраться...
	//Применять данную функцию имеет смысл только внутри операторных скобок Исключение - КонецПопытки, так как в ином случае она вернет пустую строку.
	//StringValue ErrorDescription()



	// Never
	//Arbitrary Eval(ConstPara<StringValue> Expression_)
	//Arbitrary ProceedWithCall(ArgByValOpt<Arbitrary> Value1ValueN_)
	//ValueTable CheckScriptCircularRefs(ArgByValOpt<Arbitrary> Value_, ArgByValOpt<StringValue> ValueDescription_)
	//StringValue GetExternalResourcesMode()
	//void SetSafeModeOff(ArgByValOpt<BoolValue> Disable_)
	//BoolValue GetSafeModeDisabled()
	//BoolValue MapRepresentationSupported()
	//


	// ДАТА
	inline DateTime EndOfHour(const DateTime& Date_)		{ return Date_.EndOfHour(); }
	inline DateTime BegOfMinute(const DateTime& Date_)		{ return Date_.BegOfMinute(); }
	inline DateTime EndOfMinute(const DateTime& Date_)		{ return Date_.EndOfMinute(); }
	inline DateTime CurrentDate()							{ return DateTime::GetCurrentDate(); }
	inline DateTime BegOfQuarter(const DateTime& Date_)		{ return Date_.BegOfQuart(); }
	inline DateTime BegOfMonth(const DateTime& Date_)		{ return Date_.BegOfMonth(); }
	inline DateTime EndOfMonth(const DateTime& Date_)		{ return Date_.EndOfMonth(); }
	inline DateTime EndOfYear(const DateTime& Date_)		{ return Date_.EndOfYear(); }
	inline DateTime BegOfWeek(const DateTime& Date_)		{ return Date_.BegOfWeek(); }
	inline DateTime EndOfWeek(const DateTime& Date_)		{ return Date_.EndOfWeek(); }
	inline DateTime BegOfHour(const DateTime& Date_)		{ return Date_.BegOfHour(); }
	inline DateTime EndOfQuarter(const DateTime& Date_)		{ return Date_.EndOfQuart(); }
	inline DateTime BegOfDay(const DateTime& Date_)			{ return Date_.BegOfDay(); }
	inline DateTime EndOfDay(const DateTime& Date_)			{ return Date_.EndOfDay(); }
	inline DateTime BegOfYear(const DateTime& Date_)								{ return Date_.BegOfYear(); }
	inline DateTime AddMonth(const DateTime& Date_, int CountOfMonths_)				{ return Date_.AddMonth(CountOfMonths_); }
	inline DateTime AddMonth(const DateTime& Date_, const Numeric& CountOfMonths_)	{ return Date_.AddMonth(CountOfMonths_); }
	inline Numeric Year(const DateTime& Date_)										{ return Numeric{ Date_.GetYear() }; }
	inline Numeric Month(const DateTime& Date_)				{ return Numeric{ Date_.GetMonth() }; }
	inline Numeric DayOfYear(const DateTime& Date_)			{ return Numeric{ Date_.GetYearDay() }; }
	inline Numeric WeekDay(const DateTime& Date_)			{ return Numeric{ Date_.GetWeekDay() }; }
	inline Numeric Day(const DateTime& Date_)				{ return Numeric{ Date_.GetMonthDay() }; }
	inline Numeric Hour(const DateTime& Date_)				{ return Numeric{ Date_.GetHour() }; }
	inline Numeric Minute(const DateTime& Date_)			{ return Numeric{ Date_.GetMinute() }; }
	inline Numeric Second(const DateTime& Date_)			{ return Numeric{ Date_.GetSecond() }; }
	inline Numeric WeekOfYear(const DateTime& Date_)		{ return Numeric{ Date_.GetYearWeek() }; }
	// 


	// ЧИСЛО
	//Numeric NumberFromHexString(ConstPara<StringValue> HexString_)
	//
	inline Numeric Int(const Numeric& Number_) {
		return Numeric(static_cast<int>(Number_));
	}
	// 


	//TOBE:
	// МАТЕМАТИКА
	//Numeric ACos(ConstPara<Numeric> Number_)
	//Numeric ASin(ConstPara<Numeric> Number_)
	//Numeric ATan(ConstPara<Numeric> Number_)
	//Numeric Cos(ConstPara<Numeric> Angle_)
	//Numeric Exp(ConstPara<Numeric> Number_)
	//Numeric Pow(ConstPara<Numeric> Base_, ConstPara<Numeric> Factor_)
	//Numeric Sin(ConstPara<Numeric> Angle_)
	//Numeric Sqrt(ConstPara<Numeric> Number_)
	//Numeric Tan(ConstPara<Numeric> Angle_)
	//Numeric Log(ConstPara<Numeric> Number_)
	//Numeric Log10(ConstPara<Numeric> Number_)
	//


	//String Format(ConstPara<Numeric> Value_, ArgByValOpt<StringValue> FormatString_ = Undef::Value_)
	//String Format(ConstPara<DateTime> Value_, ArgByValOpt<StringValue> FormatString_ = Undef::Value_)
	//String Format(ConstPara<BoolValue> Value_, ArgByValOpt<StringValue> FormatString_ = Undef::Value_)
	inline String Format(ConstPara<Numeric> Value_, const String& FormatString_)	{ return FormatVar(Value_, FormatString_);}
	inline String Format(ConstPara<Numeric> Value_)									{ return FormatVar(Value_, String::Empty_);}
	inline String Format(ConstPara<Numeric> Value_, const Undef&)					{ return FormatVar(Value_, String::Empty_); }
	inline String Format(ConstPara<DateTime> Value_, const String& FormatString_)	{ return FormatVar(Value_, FormatString_); }
	inline String Format(ConstPara<DateTime> Value_)								{ return FormatVar(Value_, String::Empty_); }
	inline String Format(ConstPara<DateTime> Value_, const Undef&)					{ return FormatVar(Value_, String::Empty_); }
	inline String Format(ConstPara<Boolean> Value_, const String& FormatString_)	{ return FormatVar(Value_, FormatString_); }
	inline String Format(ConstPara<Boolean> Value_)									{ return FormatVar(Value_, String::Empty_); }
	inline String Format(ConstPara<Boolean> Value_, const Undef&)					{ return FormatVar(Value_, String::Empty_); }



	// ТИП
	// 
	template<EsbClassConcept EsbClassT>
	constexpr const TypeDef& TypeDefOf = EsbClassT::Type_;

	template<EsbClassConcept EsbClassT>
	constexpr const TypeDef& TypeOf() {
		return EsbClassT::Type_;
	}
	template<EsbClassConcept EsbClassT>
	constexpr const TypeDef& TypeOf(const EsbClassT&) {
		return EsbClassT::Type_;
	}
	inline const TypeDefObject TypeOf(const Arbitrary& val_) {
		//non template<> due to different return type.
		IType* pt = IValue_GetType(*get_interface(val_));
		assert(pt);
		return TypeDefObject{ *pt };
	}
	inline const TypeDefObject Type(const string_view_t& name_) {
		IType* pt = IType_FromName(name_);
		if (pt == nullptr)
			ESL_THROW_TYPE_BYNAME_NOT_FOUND();
		return TypeDefObject{ *pt };
	}



	// ---- СТРОКА
	inline Numeric StrLen(const String& Line_) { 
		return Numeric{ Line_.length() }; 
	}
	inline String Left(const String& Line_, const Numeric& CountOfCharacters_) {
		int n_char = static_cast<int>(CountOfCharacters_);
		size_t len = Line_.length();
		if (len == 0 || n_char <= 0) return String::Empty_;
		return String{ strview_t{Line_.c_str(), std::min(static_cast<size_t>(n_char), len) } };
	}
	inline String Right(const String& Line_, const Numeric& CountOfCharacters_) {
		int in_char = static_cast<int>(CountOfCharacters_);
		size_t len = Line_.length();
		if (len == 0 || in_char <= 0) return String::Empty_;
		size_t un_char = static_cast<unsigned>(in_char);
		if (un_char >= len) return Line_;
		size_t off = len - un_char;
		return String{ strview_t{Line_.c_str() + off, un_char} };
	}
	inline void trim_l(const strchar_t*& ptr_, size_t& len_) {
		while (len_ > 0 && is_space(*ptr_)) {
			++ptr_;
			--len_;
		}
	}
	inline void trim_r(const strchar_t*& ptr_, size_t& len_) {
		while (len_ > 0 && is_space(*ptr_)) {
			--ptr_;
			--len_;
		}
	}
	inline String TrimL(const String& Line_) {
		const strchar_t* p_head = Line_.c_str();
		size_t len = Line_.length();
		if (len == 0) return String::Empty_;
		trim_l(p_head, len);
		return (len == 0) ? String::Empty_ : String{ strview_t{p_head, len} };
	}
	inline String TrimR(const String& Line_) {
		const strchar_t* p_head = Line_.c_str();
		size_t len = Line_.length();
		if (len == 0) return String::Empty_;
		assert(p_head);
		const strchar_t* p_last = p_head + len - 1;
		trim_r(p_last, len);
		return (len == 0) ? String::Empty_ : String{ strview_t{p_head, len} };
	}
	inline String TrimAll(const String& Line_) {
		const strchar_t* p_head = Line_.c_str();
		size_t len = Line_.length();
		if (len == 0) return String::Empty_;
		assert(p_head);
		const strchar_t* p_last = p_head + len - 1;
		trim_l(p_head, len);
		trim_r(p_last, len);
		return (len == 0) ? String::Empty_ : String{ strview_t{p_head, len} };
	}

	inline String Mid(const String& Line_, const Numeric& InitialNumber_, int CountOfCharacters_ /*= Undef::Value_*/) {
		//Начальный номер символа, с которого начинается выборка.Нумерация символов в строке начинается с 1. 
		//Если указано значение, меньшее или равное нулю, то параметр принимает значение 1.
		int pos = static_cast<int>(InitialNumber_) - 1;		//1C pos 1-based, c++ pos == (pos_1c - 1)
		if (pos < 0) pos = 0;								
		size_t len = Line_.length();
		if (len == 0 || unsigned(pos) >= len || CountOfCharacters_ == 0) 
			return String::Empty_;
		else {
			size_t count_rest = len - pos;						// 0<=(pos_1c-1)<len
			size_t count_for_mid = (CountOfCharacters_ < 0 || (unsigned)CountOfCharacters_ > count_rest)? count_rest /*assume all rest*/ : (unsigned)CountOfCharacters_;
			const strchar_t* p_1st = Line_.c_str();
			assert( p_1st && (p_1st + pos + count_for_mid) <= (p_1st + len) );		//!! new_end <= org_end
			return String{ strview_t{p_1st + pos, count_for_mid} };
		}
	}
	inline String Mid(const String& Line_, const Numeric& InitialNumber_, const Numeric& CountOfCharacters_) {
		return Mid(Line_, InitialNumber_, static_cast<int>(CountOfCharacters_));
	}
	inline String Mid(const String& Line_, const Numeric& InitialNumber_) {
		return Mid(Line_, InitialNumber_, -1);
	}
	inline String Mid(const String& Line_, const Numeric& InitialNumber_, const Undef&) {
		return Mid(Line_, InitialNumber_, -1);
	}

	String Upper(const String& Line_);	//esbhlp
	String Lower(const String& Line_);	//esbhlp
	inline String Char(const Numeric& CharacterCode_) {
		int ich = static_cast<int>(CharacterCode_);
		strchar_t wch = static_cast<strchar_t>(ich);
		return String{ strview_t{&wch,1} };
	}

	inline Numeric CharCode(const String& Line_, int CharacterNumber1Based_ /* =Undef::Value_ */) {
		// при задании недопустимой позиции возвращается -1
		size_t len = Line_.length();
		if (CharacterNumber1Based_ < 1 || (unsigned)CharacterNumber1Based_ > len)
			return Numeric::Value__1_;
		else {
			const strchar_t* pstr = Line_.c_str();
			assert(pstr);
			strchar_t wch = pstr[CharacterNumber1Based_ - 1];
			return Numeric{ static_cast<int>(wch) };
		}
	}
	inline Numeric CharCode(const String& Line_, const Numeric& CharacterNumber_)	{	return CharCode(Line_, static_cast<int>(CharacterNumber_)); }
	inline Numeric CharCode(const String& Line_)									{	return CharCode(Line_, 1); }
	inline Numeric CharCode(const String& Line_, const Undef&)						{	return CharCode(Line_); }

	inline Numeric StrOccurrenceCount(const String& Line_, const String& SearchSubstring_) {
		const strview_t source = Line_.view();
		const strview_t pattern = SearchSubstring_.view();
		if (source.empty() || pattern.empty())
			return Numeric::Value_0_;
		else {
			size_t cnt = 0;
			size_t pos = source.find(pattern);
			while (pos != strview_t::npos) {
				++cnt;
				pos = source.find(pattern, pos + pattern.length());
			}
			return Numeric{ cnt };
		}
	}

	//К незначащим символам относятся символы "пробел", "неразрывный пробел" (НПП), "табуляция" (горизонтальная Таб и вертикальная ВТаб), 
	//"возврат каретки" (ВК), "перевод строки" (ПС), "перевод формы (страницы)" (ПФ). 
	inline const Boolean& IsBlankString(const String& Line_) {
		size_t len = Line_.length();
		if (len == 0) return Boolean::True_;
		const strchar_t* ptr = Line_.c_str();
		assert(ptr);
		const strchar_t* ptr_end = ptr + len;
		while (ptr < ptr_end) {
			if (!is_space(*ptr)) return Boolean::False_;
			++ptr;
		}
		return Boolean::True_;
	}

	String StrReplace(const String& Line_, const String& SearchSubstring_, const String& ReplaceSubstring_);	//esbhlp
	String Title(const String& Line_);	//esbhlp


	inline Numeric StrLineCount(const String& Line_) {
		// Позволяет посчитать число строк в многострочной строке. Строки в многострочной строке разделены символами перевода строк (Символы.ПС). (LF, 0x0A (10 decimal), \n)
		// Гхм.. судя по коду разделителями продстрок также являются CR и при этом он экранирует LF. т.е. голый CR тоже разделитель
		// причем что new-line в конце строки, то он не начинает новую строку (т.е. в строке из одного \n только одна строка, а не две пустых) 
		const strchar_t* pstr = Line_.c_str();
		size_t n_lines = 1;		// в любой строке есть как минимум одна строка
		if (pstr) {
			const strchar_t* pstr_end = pstr + Line_.length();
			if (pstr != pstr_end) {
				while (true) {
					strchar_t ch = *pstr++;
					if (pstr == pstr_end)
						break;	// последний char не анализируется в любом случае - если он обычный это не важно, если разделитель - игнорируется
					else if (ch == L'\n')
						++n_lines;
					else if (ch == L'\r') {
						++n_lines;
						if (*pstr == L'\n') {
							++pstr;
							if (pstr == pstr_end) 
								break;
						} 
					}
				}
			}
		}
		return Numeric(n_lines);
	}

	inline String StrGetLine(const String& Line_, const Numeric& LineNumber_) 
	{
		auto _find_line_next = [](const strchar_t*& pstr_, const strchar_t* pstr_end_) -> bool {
			while (pstr_ != pstr_end_) {
				strchar_t ch = *pstr_++;
				if (ch == L'\n') {
					return true;
				}
				else if (ch == L'\r') {
					if (pstr_ != pstr_end_ && *pstr_ == L'\n') ++pstr_;
					return true;
				}
			}
			return false;
		};
		auto _find_line = [&_find_line_next](const strchar_t*& pstr_, const strchar_t* pstr_end_, int n_line_need_) -> bool {
			int n_line = 1;
			while (n_line != n_line_need_) {
				if (_find_line_next(pstr_, pstr_end_)) 
					++n_line;
				else
					return false;
			}
			return true;
		};
		auto _find_line_end = [](const strchar_t* pstr_, const strchar_t* pstr_end_) -> const strchar_t* {
			for ( ; pstr_ != pstr_end_; ++pstr_) {
				if (*pstr_ == L'\n' || *pstr_ == L'\r')
					break;
			}
			return pstr_;
		};
		//---------------------
		const strchar_t* pstr = Line_.c_str();
		if (pstr) {
			const strchar_t* pstr_end = pstr + Line_.length();
			if (_find_line(pstr, pstr_end, static_cast<int>(LineNumber_)))
				return String(  strview_t{ pstr, _find_line_end(pstr, pstr_end) }  );
		}
	
		return String::Empty_;
	}
}	// end namespace esb appi native -----------------------------------------------------
#endif // ESB_API_NATIVE_H_

