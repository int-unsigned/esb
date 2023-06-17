/*
	"ESB" is designed to create external native components for the 1C(tm) program.
	Copyright © 2023 IntUnsigned	(v8classes@gmail.com)
	"ESB" is free software under LGPLv2.1 license with essential restriction about binary part (view esb-license.txt)
	"ESB" IS DISTRIBUTED "AS-IS" WITHOUT ANY, EVEN THE IMPLIED, WARRANTY. IN NO EVENT SHALL THE AUTHORS OR THE COPYRIGHT OWNER BE LIABLE FOR ANY CLAIM, 
	DAMAGES OR OTHER LIABILITY ARISING OUT OF THE USE OF THIS SOFTWARE.
	THE AUTHOR DOES NOT GIVE ANY GUARANTEES AND DOES NOT BEAR ANY RESPONSIBILITY REGARDING THE USE OF 1C(tm) COMPONENTS PRODUCED USING THE ESB LIBRARY IN THE 1C(tm) ENVIRONMENT.
*/


#pragma once
#ifndef __ESB_BASE_H__
#define __ESB_BASE_H__

#include "esb_core.h"


namespace esb {
	//---------------------------------------- БАЗОВЫЕ КЛАССЫ ЯЗЫКА ---------------------------------------------
	//
	class Undef;
	class NullValue;
	class Boolean;
	class Numeric;
	class DateTime;
	//String - в core



	class Undef : public Value {
		ESB_CLASS_IMPLEMENT_MAKE_VAL(Undef)
	protected:
		ESB_CLASS_IMPLEMENT_MAKE_OPT(Undef)
	public:
		ESB_DECLARE_TYPEINFO()
			ESB_IMPLEMENT_ARGBYVAL_OPERATOR(Undef)
	public:
		static const Undef& Value_;
		static const Undef& Default_;// { Undef::Value_ };
	public:
		Undef() throw() : Value(Undef::Value_)
		{}
	};


	template<>
	inline Arbitrary MakeArbitraryDefault<Undef>() {
		return make_copy<Arbitrary>(get_interface(Undef::Default_));
	}
	// это "нормально". см. примечания перед Var<..>
	ESB_WARNING_SUPRESS(ESB_WARN_THIS_FOR_INIT_BASE)
	inline Var<Arbitrary>::Var(const Undef&) throw() : VarArgumentByValOnThisImpl<Var<Arbitrary>>{ *this }, Arbitrary{ MakeArbitraryDefault<Undef>() }
	{}
	ESB_WARNING_RESTORE()
	inline Var<Arbitrary>::Var() throw() : Var(Undef::Value_)
	{}


	class NullValue : public Value {
		ESB_CLASS_IMPLEMENT_MAKE_VAL(NullValue)
	protected:
		ESB_CLASS_IMPLEMENT_MAKE_OPT(NullValue)
	public:
		ESB_DECLARE_TYPEINFO()
		ESB_IMPLEMENT_ARGBYVAL_OPERATOR(NullValue)
		static const NullValue&	Value_;
		static const NullValue& Default_;
	public:
		NullValue() throw() : Value(Value_)
		{}
	};


	class Boolean : public Value {
		ESB_CLASS_IMPLEMENT_MAKE_VAL(Boolean)
	protected:
		ESB_CLASS_IMPLEMENT_MAKE_OPT(Boolean)
	public:
		ESB_DECLARE_TYPEINFO()
		ESB_IMPLEMENT_ARGBYVAL_OPERATOR(Boolean)
		static const Boolean&	True_;
		static const Boolean&	False_;
		static const Boolean&	Default_;	// = False_
		static Boolean Create(bool value_) { return (value_) ? Boolean::True_ : Boolean::False_; }
	public:
		Boolean(bool val_) throw() : Value( ((val_)? True_ : False_) )
		{}
		operator bool() const noexcept { 
			if (this->Value::operator==(Boolean::True_))
				return true;
			assert(this->Value::operator==(Boolean::False_));
			return false;
		}
	};


	class Numeric : public Value {
		ESB_CLASS_IMPLEMENT_MAKE_VAL(Numeric)
	protected:
		ESB_CLASS_IMPLEMENT_MAKE_OPT(Numeric)
	public:
		ESB_DECLARE_TYPEINFO()
		ESB_IMPLEMENT_ARGBYVAL_OPERATOR(Numeric)
		static const Numeric& Value_0_;
		static const Numeric& Value_1_;
		static const Numeric& Value__1_;	//-1
		static const Numeric& Default_;		// = Value_0_;
	public:
		ESB_EXPLICIT_NUMERIC
		Numeric(short val_) throw() : Value(create_numeric_value(val_))
		{}
		ESB_EXPLICIT_NUMERIC
		Numeric(int val_) throw() : Value(create_numeric_value(val_))
		{}
		ESB_EXPLICIT_NUMERIC
		Numeric(long val_) throw() : Value(create_numeric_value(val_))
		{}
		ESB_EXPLICIT_NUMERIC
		Numeric(long long val_) throw() : Value(create_numeric_value(val_))
		{}
		ESB_EXPLICIT_NUMERIC
		Numeric(unsigned short val_) throw() : Value(create_numeric_value(val_))
		{}
		ESB_EXPLICIT_NUMERIC
		Numeric(unsigned int val_) throw() : Value(create_numeric_value(val_))
		{}
		ESB_EXPLICIT_NUMERIC
		Numeric(unsigned long val_) throw() : Value(create_numeric_value(val_))
		{}
		ESB_EXPLICIT_NUMERIC
		Numeric(unsigned long long val_) throw() : Value(create_numeric_value(val_))
		{}
		ESB_EXPLICIT_NUMERIC
		Numeric(double val_) throw() : Value(create_numeric_value(val_))
		{}

		std::strong_ordering operator<=>(const Numeric& other_) const	{
			if (Value::operator==(other_))
				return std::strong_ordering::equivalent;
			else {
				int cmp = Compare(other_);
				return (cmp == 0)? std::strong_ordering::equivalent : ((cmp < 0)? std::strong_ordering::less : std::strong_ordering::greater);
			}
		}
		// возможно компилятор может и сам переписать.. а может и нет..
		inline bool operator==(const Numeric& other_) const				{ return (Value::operator==(other_))? true : (Compare(other_) == 0);	}
		inline bool operator!=(const Numeric& other_) const				{ return !operator==(other_); }

		Numeric& operator+() { return *this; }
		Numeric  operator-() const;

		Numeric operator+(const Numeric& rval_) const;
		Numeric operator-(const Numeric& rval_) const;
		Numeric operator*(const Numeric& rval_) const;
		Numeric operator/(const Numeric& rval_) const;
		Numeric operator%(const Numeric& rval_) const;
	
		Numeric& operator+=(const Numeric& rval_)	{ *this = *this + rval_;	return *this; }
		Numeric& operator-=(const Numeric& rval_)	{ *this = *this - rval_;	return *this; }
		Numeric& operator*=(const Numeric& rval_)	{ *this = *this * rval_;	return *this; }
		Numeric& operator/=(const Numeric& rval_)	{ *this = *this / rval_;	return *this; }
		Numeric& operator%=(const Numeric& rval_)	{ *this = *this % rval_;	return *this; }

		Numeric& operator++()						{ return operator+=(Numeric::Value_1_);	}
		Numeric& operator--()						{ return operator-=(Numeric::Value_1_);	}
		Numeric  operator++(int)					{
			Numeric tmp = *this;
			*this = tmp + Numeric::Value_1_;
			return tmp;
		}
		Numeric  operator--(int)					{
			Numeric tmp = *this;
			*this = tmp - Numeric::Value_1_;
			return tmp;
		}

		explicit operator long long() const;
		explicit operator double() const;
		explicit operator long() const;
		explicit operator int() const { 
			// в отличие от create_... оператора инт нет. эмулируем.
			static_assert(sizeof(long) == sizeof(int));
			return operator long(); 
		}

		Numeric		Abs(void) const;
		Numeric		Floor(void) const;
		int 		Compare(const Numeric& other_) const;
		Numeric		Pow(const Numeric& exp_) const;
		Numeric 	Round(int digits_) const;
		Numeric 	Round(int digits_, const RoundModeValue& mode_) const;
		int 		Sign(void) const;
		String		ToString(void) const;
		void		LengthAndPrecision(unsigned int& out_lenght_, unsigned int& out_precision_);
		
		using to_integer_t = long long;
		bool		ToInteger(to_integer_t& out_integer_) const;

		//TOBE:	Incompleted
	};
#if ESB_USE_RUSSIAN
	using Число = Numeric;
#endif


	template<class IntegerT>
	IntegerT ToIntegerOrThrow(const Numeric& value_);

	template<>
	inline Numeric::to_integer_t ToIntegerOrThrow<Numeric::to_integer_t>(const Numeric& value_) {
		Numeric::to_integer_t i;
		if (value_.ToInteger(i))
			return i;
		ESL_THROW_NUMERIC_NOT_INT_OR_INTMAX_OVERFLOW();
	}

	constexpr auto xxm =  std::numeric_limits<long long>::min();

	template<class IntegerT>
	inline IntegerT ToIntegerOrThrow(const Numeric& value_) {
		Numeric::to_integer_t i = ToIntegerOrThrow<Numeric::to_integer_t>(value_);
		if constexpr (std::is_same_v<Numeric::to_integer_t, IntegerT>) {
			return i;
		}
		else {
			if (std::cmp_greater_equal(i, std::numeric_limits<IntegerT>::min()) && std::cmp_less_equal(i, std::numeric_limits<IntegerT>::max()))
				return static_cast<IntegerT>(i);
		}
		throw_error_integer_overflow<IntegerT>();
	}

	inline unsigned int AsUnsignedOrThrow(const Numeric& value_) {
		return ToIntegerOrThrow<unsigned int>(value_);
	}

	inline size_t AsIndex(const Numeric& value_, size_t size_) {
		size_t i = ToIntegerOrThrow<size_t>(value_);
		if (i  >= size_)
			ESL_THROW_ARRAY_INDEX_ERR();
		else
			return i;
	}


	//class String определен в esb_core


	class DateTime: public Value {
		ESB_CLASS_IMPLEMENT_MAKE_VAL(DateTime)
	protected:
		ESB_CLASS_IMPLEMENT_MAKE_OPT(DateTime)
	public:
		ESB_DECLARE_TYPEINFO()
		ESB_IMPLEMENT_ARGBYVAL_OPERATOR(DateTime)
	public:
		DateTime(int year_, int month_, int day_);
		DateTime(int year_, int month_, int day_, int hour_, int min_, int sec_);
		DateTime operator+(int sec_) const;
		DateTime operator+(const Numeric& sec_) const		{ return operator+(static_cast<long>(sec_)); }
		bool operator==(const DateTime& other_) const;
		bool operator<(const DateTime& other_) const;
		bool operator>(const DateTime& other_) const;
		int GetHour(void)const;
		int GetMinute(void)const;
		int GetMonth(void)const;
		int GetMonthDay(void)const;
		int GetQuarter(void)const;
		int GetSecond(void)const;
		int GetWeekDay(void)const;
		int GetYear(void)const;
		int GetYearDay(void)const;
		int GetYearWeek(void)const;
		DateTime BegOfDay(void)	const;
		DateTime BegOfHour(void)	const;
		DateTime BegOfMinute(void)	const;
		DateTime BegOfMonth(void)	const;
		DateTime BegOfQuart(void)	const;
		DateTime BegOfWeek(void)	const;
		DateTime BegOfYear(void)	const;
		DateTime EndOfDay(void)		const;
		DateTime EndOfHour(void)	const;
		DateTime EndOfMinute(void)	const;
		DateTime EndOfMonth(void)	const;
		DateTime EndOfQuart(void)	const;
		DateTime EndOfWeek(void)	const;
		DateTime EndOfYear(void)	const;
		DateTime AddMonth(int n_)	const;
		DateTime AddMonth(const Numeric& n_) const { return AddMonth(static_cast<long>(n_)); }
		String	 ToString()			const;
		//TOBE:	Incompleted
		//void Split(int&, int&, int&)const;
		//void Split(int&, int&, int&, int&, int&, int&)const;
		//FromString etc
		
		//NOTE: Не знаю как оно у них работает, но у них задекларировано как инт и абсолютно для month_ нет проверки 1-12
		//		просто тупо -1 делают и значение из массива читают. (кроме 2-февраля - там алгоритм). У себя контроль и бросание ошибки сделал.
		//		Год не проверяю!! Они его тоже не проверяют, хотя в IsLeapYear проверяют - там в отрицательных годах високосных нет. 
		//		(...из сопоставления римских и египетских датировок по оксиринхскому папирусу, опубликованному в 1999 году[6][7], было установлено, что 
		//		високосными годами в Риме были 42, 39, 36, 33, 30, 27, 24, 21, 18, 15, 12, 9 годы до н. э., 8, 12 годы и в последующем каждый четвёртый год
		//		https://ru.wikipedia.org/wiki/%D0%92%D0%B8%D1%81%D0%BE%D0%BA%D0%BE%D1%81%D0%BD%D1%8B%D0%B9_%D0%B3%D0%BE%D0%B4)
		//		Просто год сделал unsigned, а не int.
		//		По хорошему надо нумератор сделать.
		//		Также - оставил возврат int, как у них, хотя отрицательное кол-во дней это нонсенс. Но во всем остальном АПИ год-это-int. 
		//		если передекларировать как unsigned полезут ворнинги где-нибудь. Пусть будет безобразно. но единообразно.
	private:
		static int		DaysPerMonthUnsafe(int year_, int month_);
	public:
		static unsigned	DaysPerMonth(unsigned year_, unsigned month_) {
			if (month_ == 0 || month_ > 12)
				ESL_THROW_MONTH_VALUE_INVALID();
			else
				return (unsigned) DaysPerMonthUnsafe((signed)year_, (signed)month_);
		}
		static DateTime GetCurrentDate(void);
		static DateTime GetCurrentDateUTC(void);
		// Здесь проверяют на <0 и отдают false. Не знаю по-логике правильно это или нет, но по крайней мере уб нет. Пусть как у них бедет.
		static bool 	IsLeapYear(int year_);
	};
}



namespace esb {
//namespace internal {

	template<class T>
	T check_and_make_from_var_ex(const IVariable* pvar_) {
		if constexpr (is_esb_class<T>) {
			return check_and_make_from_var<T>(pvar_);
		}
#if ESB_USE_AUTO_CONVERTION_VALUE_TO_CPP_TYPE
		else if constexpr (std::is_same_v<T, bool>)			{ return static_cast<bool>(check_and_make_from_var<Boolean>(pvar_));	}
		else if constexpr (std::is_same_v<T, signed char>)			{ return ToIntegerOrThrow<signed char>(check_and_make_from_var<Numeric>(pvar_)); }
		else if constexpr (std::is_same_v<T, short>)				{ return ToIntegerOrThrow<short>(check_and_make_from_var<Numeric>(pvar_)); }
		else if constexpr (std::is_same_v<T, int>)					{ return ToIntegerOrThrow<int>(check_and_make_from_var<Numeric>(pvar_));	}
		else if constexpr (std::is_same_v<T, long>)					{ return ToIntegerOrThrow<long>(check_and_make_from_var<Numeric>(pvar_));	}
		else if constexpr (std::is_same_v<T, long long>)			{ return ToIntegerOrThrow<long long>(check_and_make_from_var<Numeric>(pvar_)); }
		else if constexpr (std::is_same_v<T, unsigned char>)		{ return ToIntegerOrThrow<unsigned char>(check_and_make_from_var<Numeric>(pvar_)); }
		else if constexpr (std::is_same_v<T, unsigned short>)		{ return ToIntegerOrThrow<unsigned short>(check_and_make_from_var<Numeric>(pvar_)); }
		else if constexpr (std::is_same_v<T, unsigned int>)			{ return ToIntegerOrThrow<unsigned int>(check_and_make_from_var<Numeric>(pvar_));	}
		else if constexpr (std::is_same_v<T, unsigned long>)		{ return ToIntegerOrThrow<unsigned long>(check_and_make_from_var<Numeric>(pvar_)); }
		else if constexpr (std::is_same_v<T, unsigned long long>)	{ return ToIntegerOrThrow<unsigned long long>(check_and_make_from_var<Numeric>(pvar_)); }
		else if constexpr (std::is_same_v<T, /*plain*/char>)		{ return ToIntegerOrThrow</*plain*/char>(check_and_make_from_var<Numeric>(pvar_)); }
		else if constexpr (std::is_same_v<T, double>)			{ return static_cast<double>(check_and_make_from_var<Numeric>(pvar_));	}
		else if constexpr (std::is_same_v<T, strview_t>)		{ return check_and_make_from_var<String>(pvar_).view_when_safe(); }
		//TOBE: нужно еще std::crono из Даты сделать. А то вечно про дату забываю...
#endif	//ESB_USE_AUTO_CONVERTION_VALUE_TO_CPP_TYPE
		else {
			static_assert(false, "unsuppoted type T");
		}
	}

	template<class T>
	struct compatable_cpp_type {};
	template<> struct compatable_cpp_type<bool>			{ using type = Boolean; };

	template<> struct compatable_cpp_type<signed char>	{ using type = Numeric; };
	template<> struct compatable_cpp_type<short>		{ using type = Numeric; };
	template<> struct compatable_cpp_type<int>			{ using type = Numeric; };
	template<> struct compatable_cpp_type<long>			{ using type = Numeric; };
	template<> struct compatable_cpp_type<long long>	{ using type = Numeric; };

	template<> struct compatable_cpp_type<unsigned char>	{ using type = Numeric; };
	template<> struct compatable_cpp_type<unsigned short>	{ using type = Numeric; };
	template<> struct compatable_cpp_type<unsigned int>		{ using type = Numeric; };
	template<> struct compatable_cpp_type<unsigned long>	{ using type = Numeric; };
	template<> struct compatable_cpp_type<unsigned long long> { using type = Numeric; };

	template<> struct compatable_cpp_type</*plain*/char>		{ using type = Numeric; };
	template<> struct compatable_cpp_type<double>				{ using type = Numeric; };

	template<> struct compatable_cpp_type<strview_t>			{ using type = String; };

	template<class T>
	using compatable_cpp_type_t = compatable_cpp_type<T>::type;

	template<class T>
	inline constexpr bool is_compaitable_cpp_type = requires { typename compatable_cpp_type<T>::type; };
	
	static_assert(is_compaitable_cpp_type<char>);
	static_assert(!is_compaitable_cpp_type<void>);

//}	//namespace esb-internal

	
	namespace internal {
		template<class RetT>
		inline void variable_set_value(IVariable& ret_, RetT&& res_) {
			IVariable_SetValue(ret_, get_interface(std::move(res_)));
		}
		template<class RetT>
		inline void variable_set_value(IVariable* pret_, RetT&& res_) {
			return variable_set_value(*pret_, std::move(res_));
		}

#if ESB_USE_AUTO_CONVERTION_VALUE_TO_CPP_TYPE
#define IMPLEMENT_VARIABLE_SET_VALUE_COMPAITABLE(CPP_T_)							\
		template<>																	\
		inline void variable_set_value<CPP_T_>(IVariable* pret_, CPP_T_&& res_) {	\
			return variable_set_value(pret_, compatable_cpp_type_t<CPP_T_>{res_});	\
		}

		IMPLEMENT_VARIABLE_SET_VALUE_COMPAITABLE(bool)
		IMPLEMENT_VARIABLE_SET_VALUE_COMPAITABLE(signed char)
		IMPLEMENT_VARIABLE_SET_VALUE_COMPAITABLE(short)
		IMPLEMENT_VARIABLE_SET_VALUE_COMPAITABLE(int)
		IMPLEMENT_VARIABLE_SET_VALUE_COMPAITABLE(long)
		IMPLEMENT_VARIABLE_SET_VALUE_COMPAITABLE(long long)
		IMPLEMENT_VARIABLE_SET_VALUE_COMPAITABLE(unsigned char)
		IMPLEMENT_VARIABLE_SET_VALUE_COMPAITABLE(unsigned short)
		IMPLEMENT_VARIABLE_SET_VALUE_COMPAITABLE(unsigned int)
		IMPLEMENT_VARIABLE_SET_VALUE_COMPAITABLE(unsigned long)
		IMPLEMENT_VARIABLE_SET_VALUE_COMPAITABLE(unsigned long long)
		IMPLEMENT_VARIABLE_SET_VALUE_COMPAITABLE(char)
		IMPLEMENT_VARIABLE_SET_VALUE_COMPAITABLE(double)
		IMPLEMENT_VARIABLE_SET_VALUE_COMPAITABLE(strview_t)
#undef IMPLEMENT_VARIABLE_SET_VALUE_COMPAITABLE
#endif	//ESB_USE_AUTO_CONVERTION_VALUE_TO_CPP_TYPE
	}	//namespace esb-internal

	
	//Опциональным для 1С считаем параметр декларированный как UndefOr<...>
	template<typename>
	struct is_param_opt_t {
		static constexpr bool value_ = false;
	};
	template<typename MixClass1stT, typename... RestT>
	inline constexpr bool is_undef_1st = std::is_same_v<MixClass1stT, Undef>;
	template<typename... EsbClassesT>
	struct is_param_opt_t<ClassMix<EsbClassesT...>> {
		static constexpr bool value_ = is_undef_1st<EsbClassesT...>;
	};

	template<typename EsbClassT>
	struct is_param_opt_t<const EsbClassT> : public is_param_opt_t<std::remove_cvref_t<EsbClassT>> {};
	template<typename EsbClassT>
	struct is_param_opt_t<const EsbClassT&> : public is_param_opt_t<std::remove_cvref_t<EsbClassT>> {};

	template<typename EsbClassT>
	inline constexpr bool is_param_opt_v = is_param_opt_t<EsbClassT>::value_;
	static_assert(is_param_opt_v<String> == false);
	static_assert(is_param_opt_v<UndefOr<String>> == true);
	static_assert(is_param_opt_v<const UndefOr<String>> == true);
	static_assert(is_param_opt_v<const UndefOr<String>&> == true);
}	//namespace esb



namespace esb {
	// Синтаксический сахар для создания новых объектов в виде Create<EsbClassT>(args...). 
//NOTE	Функциональность сложная, неоднозначная и неудобная т.к. IDE с параметрами не поможет. Предпочтительно по возможности EsbClassT::Create(...).
//TOBE	Можно сделать New, но возможна неразбериха с new. виде шаблона New не страшно - оно по другому манглится и с new не конфликтует, но человек запутается.
//
namespace internal{
	template<EsbClassConcept EsbClassT, class... ArgsT>
	inline constexpr bool can_call_create_ctor = requires(ArgsT... args_) { EsbClassT{ std::forward<ArgsT>(args_)... }; };
	template<EsbClassConcept EsbClassT, class... ArgsT>
	inline constexpr bool can_call_create_func = requires(ArgsT... args_) { EsbClassT::Create(std::forward<ArgsT>(args_)...); };
	template<EsbClassConcept EsbClassT, class... ArgsT>
	inline constexpr bool can_call_create_tmpl = requires(ArgsT... args_) { EsbClassT::template Create<ArgsT...>(std::forward<ArgsT>(args_)...); };
	// У Структуры первый аргумент фиксировано строка, а остальные уже разворачиваются списком значений. Специально для такого..
	// (этот шаблон нельзя напрямую применить если аргументов не хватает - ош синтаксиса. поэтому для 0 аргументов своя специализация) 
	template<size_t NArgs>
	struct can_call_create_tmpl_fixed_a1 {
		template<EsbClassConcept EsbClassT, class Arg1T, class... ArgsRestT>
		static constexpr bool can_call_create = requires(Arg1T a1, ArgsRestT... args_) {
			EsbClassT::template Create<ArgsRestT...>(std::forward<Arg1T>(a1), std::forward<ArgsRestT>(args_)...);
		};
		template<EsbClassConcept EsbClassT, class Arg1T, class... ArgsRestT>
		static EsbClassT call_create(Arg1T a1_, ArgsRestT... args_) {
			EsbClassT::template Create<ArgsRestT...>(std::forward<Arg1T>(a1_), std::forward<ArgsRestT>(args_)...);
		}
	};
	template<>
	struct can_call_create_tmpl_fixed_a1<0> {
		template<EsbClassConcept EsbClassT, class... ArgsRestT>
		static constexpr bool can_call_create = false;
	};
} //internal

	template<EsbClassConcept EsbClassT, class... ArgsT>
	EsbClassT Create(ArgsT... args_) {
		if constexpr (internal::can_call_create_ctor<EsbClassT, ArgsT...>)
			return EsbClassT{ std::forward<ArgsT>(args_)... };
		else if constexpr (internal::can_call_create_func<EsbClassT, ArgsT...>)
			return EsbClassT::Create(std::forward<ArgsT>(args_)...);
		else if constexpr (internal::can_call_create_tmpl<EsbClassT, ArgsT...>)
			return EsbClassT::template Create<ArgsT...>(std::forward<ArgsT>(args_)...);
		else if constexpr (internal::can_call_create_tmpl_fixed_a1<sizeof...(ArgsT)>::template can_call_create<EsbClassT, ArgsT...>)
			return internal::can_call_create_tmpl_fixed_a1<sizeof...(ArgsT)>::template call_create<EsbClassT, ArgsT...>(std::forward<ArgsT>(args_)...);
		else
			static_assert(always_false_v<EsbClassT>, "EsbClassT not creatable with supplyed arguments.");
	}



#define ESB_CONVERTER_METH_ToBoolean	ToBoolean
#define ESB_CONVERTER_METH_ToNumeric	ToNumeric
#define ESB_CONVERTER_METH_ToString		ToString
#define ESB_CONVERTER_METH_ToDate		ToDate

	//we don't check return type - if we have meth with magic name return type MUST BE corresponding or ERROR. 
	template<class T>
	inline constexpr bool  has_meth_ToBoolean = requires(T & t) { t.ESB_CONVERTER_METH_ToBoolean(); };
	template<class T>
	inline constexpr bool  has_meth_ToNumeric = (requires(T & t) { t.ESB_CONVERTER_METH_ToNumeric(); });
	template<class T>
	inline constexpr bool  has_meth_ToString = requires(T & t) { t.ESB_CONVERTER_METH_ToString(); };
	template<class T>
	inline constexpr bool  has_meth_ToDate = requires(T & t) { t.ESB_CONVERTER_METH_ToDate(); };







}	//namespace esb


//__attribute__((used))
//Критично заключать блок в {}. Иначе ; в конце трактуется как асм-комментарий и все что после нее игнорируется - если использовать макрос внутри другого макроса с переносом строки
#define VARIABLE_USED(VAR_) __asm {mov eax, VAR_}
// для декларации псевдо-енум-структоры ID методов
//TOBO: Нужно в макрос обернуть и саму декларацию структуры - struct MethId -> ESB_DECL_METHID(...), и в этот макрос аргументами передавать ESB_ID
//		`;` после ESB_ID добавить в макрос и убрать из текста.
//		тогда мы получим возможность "в зависимости от" менять способ представления диспатч-идентификаторов. Например для варианта динамической загрузки
//		можем сделать id не constexpr, а просто static и биндить их при startup. Малореально без reflection, но.. когда-то оно будет.
#define ESB_ID(NAME_, VAL_)			static constexpr int NAME_ = VAL_
// в макрос нельзя непосредственно подставить ClassMix<C1_, C2_> т.к. препроцессор воспринимает запятую как разделитель аргуменотов самого макроса. <>-для него не скобки.
#define ESB_MIX2(C1_, C2_)		ClassMix<C1_, C2_>


#define ESB_ARGPACK0()
#define ESB_ARGPACK1(_1)							{&_1}							
#define ESB_ARGPACK2(_1,_2)							{&_1,&_2}
#define ESB_ARGPACK3(_1,_2,_3)						{&_1,&_2,&_3}
#define ESB_ARGPACK4(_1,_2,_3,_4)					{&_1,&_2,&_3,&_4}
#define ESB_ARGPACK5(_1,_2,_3,_4,_5)				{&_1,&_2,&_3,&_4,&_5}
#define ESB_ARGPACK6(_1,_2,_3,_4,_5,_6)				{&_1,&_2,&_3,&_4,&_5,&_6}
#define ESB_ARGPACK7(_1,_2,_3,_4,_5,_6,_7)			{&_1,&_2,&_3,&_4,&_5,&_6,&_7}
#define ESB_ARGPACK8(_1,_2,_3,_4,_5,_6,_7,_8)		{&_1,&_2,&_3,&_4,&_5,&_6,&_7,&_8}
#define ESB_ARGPACK9(_1,_2,_3,_4,_5,_6,_7,_8,_9)	{&_1,&_2,&_3,&_4,&_5,&_6,&_7,&_8,&_9}


#define ESB_INVOKE_CTOR_(CLASS_)							return TypeObj_.CreateValue<CLASS_>
#define ESB_INVOKE_CTOR0(CLASS_)									ESB_INVOKE_CTOR_(CLASS_) ()
#define ESB_INVOKE_CTOR1(CLASS_, _1)								ESB_INVOKE_CTOR_(CLASS_) ({&_1});							
#define ESB_INVOKE_CTOR2(CLASS_, _1,_2)								ESB_INVOKE_CTOR_(CLASS_) ({&_1,&_2});						
#define ESB_INVOKE_CTOR3(CLASS_, _1,_2,_3)							ESB_INVOKE_CTOR_(CLASS_) ({&_1,&_2,&_3});					
#define ESB_INVOKE_CTOR4(CLASS_, _1,_2,_3,_4)						ESB_INVOKE_CTOR_(CLASS_) ({&_1,&_2,&_3,&_4});				
#define ESB_INVOKE_CTOR5(CLASS_, _1,_2,_3,_4,_5)					ESB_INVOKE_CTOR_(CLASS_) ({&_1,&_2,&_3,&_4,&_5});			
#define ESB_INVOKE_CTOR6(CLASS_, _1,_2,_3,_4,_5,_6)					ESB_INVOKE_CTOR_(CLASS_) ({&_1,&_2,&_3,&_4,&_5,&_6});		
#define ESB_INVOKE_CTOR7(CLASS_, _1,_2,_3,_4,_5,_6,_7)				ESB_INVOKE_CTOR_(CLASS_) ({&_1,&_2,&_3,&_4,&_5,&_6,&_7});	
#define ESB_INVOKE_CTOR8(CLASS_, _1,_2,_3,_4,_5,_6,_7,_8)			ESB_INVOKE_CTOR_(CLASS_) ({&_1,&_2,&_3,&_4,&_5,&_6,&_7,&_8});

#define ESB_INVOKE_CTOR_PACK0(CLASS_, ARGS_)						ESB_INVOKE_CTOR_(CLASS_) ( {&ARGS_ ...} );
#define ESB_INVOKE_CTOR_PACK1(CLASS_, _1T, _1V, _ARGS_T, _ARGS_V)	ESB_INVOKE_CTOR_(CLASS_) ( {&_1V, &_ARGS_V ...});


#define ESB_INVOKE_MEMB_FUNC0(RET_, METH_)					return check_and_make<RET_>(IObjectMethods_InvokeAsFunc( *get_context(*this), static_cast<dispix_t>((METH_)) ) )
#define ESB_INVOKE_MEMB_FUNC_(RET_, METH_, ARGS_)			return check_and_make<RET_>(IObjectMethods_InvokeAsFunc(* get_context(*this), static_cast<dispix_t>((METH_)) , ARGS_ ) )
#define ESB_INVOKE_MEMB_FUNC1(RET_, METH_, _1)				ESB_INVOKE_MEMB_FUNC_(RET_, METH_, ESB_ARGPACK1(_1) )
#define ESB_INVOKE_MEMB_FUNC2(RET_, METH_, _1,_2)			ESB_INVOKE_MEMB_FUNC_(RET_, METH_, ESB_ARGPACK2(_1,_2) )
#define ESB_INVOKE_MEMB_FUNC3(RET_, METH_, _1,_2,_3)		ESB_INVOKE_MEMB_FUNC_(RET_, METH_, ESB_ARGPACK3(_1,_2,_3) )
#define ESB_INVOKE_MEMB_FUNC4(RET_, METH_, _1,_2,_3,_4)		ESB_INVOKE_MEMB_FUNC_(RET_, METH_, ESB_ARGPACK4(_1,_2,_3,_4) )
#define ESB_INVOKE_MEMB_FUNC5(RET_, METH_, _1,_2,_3,_4,_5)	ESB_INVOKE_MEMB_FUNC_(RET_, METH_, ESB_ARGPACK5(_1,_2,_3,_4,_5) )


#define ESB_INVOKE_MEMB_PROC0(METH_)						return IObjectMethods_InvokeAsProc( *get_context(*this), static_cast<dispix_t>((METH_))  );
#define ESB_INVOKE_MEMB_PROC_(METH_, ARGS_)					return IObjectMethods_InvokeAsProc( *get_context(*this), static_cast<dispix_t>((METH_)), ARGS_ );
#define ESB_INVOKE_MEMB_PROC1(METH_, _1)					ESB_INVOKE_MEMB_PROC_(METH_, ESB_ARGPACK1(_1) )
#define ESB_INVOKE_MEMB_PROC2(METH_, _1,_2)					ESB_INVOKE_MEMB_PROC_(METH_, ESB_ARGPACK2(_1,_2) )
#define ESB_INVOKE_MEMB_PROC3(METH_, _1,_2,_3)				ESB_INVOKE_MEMB_PROC_(METH_, ESB_ARGPACK3(_1,_2,_3) )
#define ESB_INVOKE_MEMB_PROC4(METH_, _1,_2,_3,_4)			ESB_INVOKE_MEMB_PROC_(METH_, ESB_ARGPACK4(_1,_2,_3,_4) )


#define ESB_INVOKE_PROPERTY_GET(TYPE_, PROP_)		return check_and_make<TYPE_>(IObjectProperties_GetValue(*get_context(*this), (PROP_) ))
#define ESB_INVOKE_PROPERTY_SET(PROP_, VAL_)		return IObjectProperties_SetValue(*get_context(*this), (PROP_) , VAL_)

#define ESB_PROPERTY_FIELD_RO(TYPE_, NAME_)			__declspec(property(get = Get##NAME_))						TYPE_ NAME_
#define ESB_PROPERTY_FIELD_WO(TYPE_, NAME_)			__declspec(property(put = Set##NAME_))						TYPE_ NAME_
#define ESB_PROPERTY_FIELD_RW(TYPE_, NAME_)			__declspec(property(get = Get##NAME_, put = Set##NAME_))	TYPE_ NAME_
#define ESB_PROPERTY_ARRAY_RO(TYPE_, NAME_)			__declspec(property(get = Get##NAME_))						TYPE_ NAME_[]
#define ESB_PROPERTY_ARRAY_RW(TYPE_, NAME_)			__declspec(property(get = Get##NAME_, put = Set##NAME_))	TYPE_ NAME_[]
#define ESB_PROPERTY_ARRAY_WO(TYPE_, NAME_)			__declspec(property(put = Set##NAME_))						TYPE_ NAME_[]


//TOBE: нужно сделать везде генерацию интерфейса так. и кодогенератор научить. тогда от  и т.п. спрятать или вообще избавиться
//		(запись короче и гибче. только ClassMix придется через ESB_MIX, а тот лучше через __VA_ARGS__, а там какие-то проблемы были)
#define ESB_MEMB(TYPE_)		TYPE_ 



namespace esb
{

	//Индексированная коллекция --------------------------------------------------------------------------------------------------------------------
	//

	template<EsbClassConcept EsbValueT, class IxInterfacePtrT>
	class IxCollectionItemROImpl {
	private:
		IxInterfacePtrT		m_interface;	// мы ссылка!
		size_t				m_index;		// мы конст! мы никогда не должны меняться!
	protected:
		const IxInterfacePtrT& get_interface() const	{ return m_interface; }
		size_t get_index() const						{ return m_index; }
	public:
		IxCollectionItemROImpl(const IxInterfacePtrT& interface_, size_t index_) : m_interface(interface_), m_index(index_)
		{}
		EsbValueT GetValue() const								{ return make<EsbValueT>(IxCollection_GetAt(*m_interface, m_index)); }
		operator EsbValueT() const								{ return this->GetValue(); }
	};


	template<EsbClassConcept EsbValueT, class IxInterfacePtrT>
	class IxCollectionItemRO : public IxCollectionItemROImpl<EsbValueT, IxInterfacePtrT> {
	public:
		IxCollectionItemRO(const IxInterfacePtrT& interface_, size_t index_) : IxCollectionItemROImpl<EsbValueT, IxInterfacePtrT>(interface_, index_)
		{}
		ESB_PROPERTY_FIELD_RO(EsbValueT, Value);
	};


	template<EsbClassConcept EsbValueT, class IxInterfacePtrT>
	class IxCollectionItemRW : public IxCollectionItemROImpl<EsbValueT, IxInterfacePtrT> {
	public:
		IxCollectionItemRW(const IxInterfacePtrT& interface_, size_t index_) : IxCollectionItemROImpl<EsbValueT, IxInterfacePtrT>(interface_, index_)
		{}
		void SetValue(ConstPara<EsbValueT> value_)													{ return IxCollection_SetAt(*this->get_interface(), this->get_index(), value_); }
		IxCollectionItemRW& operator =(ConstPara<EsbValueT> value_)									{ SetValue(value_);	return *this; }
		//TODO: Когда мы сюда присваиваем другой "итем", то с++ не хочет использовать никакой другой оператор присвоения кроме явно с нашим типом.
		//		то есть, несмотря на то что мы явно и публично унаследованы от IxCollectionItemROImpl<EsbValueT, IxInterfacePtrT> и если так объявить other_
		//		плюсы этот оператор не рассматривают и пытаются использовать дефолтный для IxCollectionItemRW с ошибкой т.к. m_interface там объявлен const.
		//		В сущности наш итем-класс это аналог ссылки, которая никогда не должна меняться, а при присвоении ей другой такой-же "ссылки" должно произойти
		//		копирование данных (как и с обычными ссылками).
		//		Поэтому сюда еще нужен мув=, и, операторы= для IxCollectionItemRO, и еще возможны варианты на разных IxInterfacePtrT
		//		ПС. Сам класс очень похож на IxCollectionIteratorRef. Нужно бы с ним объедениться!
		//		ППС. Все это касается и AxCollectionItemRW
		IxCollectionItemRW& operator =(const IxCollectionItemRW<EsbValueT, IxInterfacePtrT>& it_)	{ 
			IxCollection_Copy(*it_.get_interface(), it_.get_index(), *this->get_interface(), this->get_index());
			return *this; 
		}
		ESB_PROPERTY_FIELD_RW(EsbValueT, Value);
	};



	template<EsbClassConcept EsbValueT, class IxInterfacePtrT>
	struct IxCollectionImplRO {
		using value_t = EsbValueT;
		using interface_t = IxInterfacePtrT;
	protected:
		IxInterfacePtrT m_interface; //нельзя делать const, т.к. тогда отключается operator=(&&)
	public:
		IxCollectionImplRO(IxInterfacePtrT&& interface_) : m_interface(std::move(interface_))
		{}
		size_t	Size() const											{ return IxCollection_Size(*m_interface); }
		value_t		GetAt(size_t index_) const							{ return make<EsbValueT>(IxCollection_GetAt(*m_interface, index_)); }
	public:
		template_argument_type_t<IxInterfacePtrT>& get_interface() const	{ return *this->m_interface; }
	};


	template<EsbClassConcept EsbValueT, class IxInterfacePtrT>
	class IxCollectionImplRW : public IxCollectionImplRO<EsbValueT, IxInterfacePtrT> {
	public:
		IxCollectionImplRW(IxInterfacePtrT&& interface_) : IxCollectionImplRO<EsbValueT, IxInterfacePtrT>(std::move(interface_))
		{}
		void SetAt(size_t index_, ConstPara<EsbValueT> value_)			{ return IxCollection_SetAt(*this->m_interface, index_, value_);}
	};



	template<EsbClassConcept EsbValueT>
	class IxCollectionRO : public IxCollectionImplRO<EsbValueT, IIxCollectionROPtr> {
		using base_t = IxCollectionImplRO<EsbValueT, IIxCollectionROPtr>;
	public:
		using typename base_t::interface_t;
		using ix_collection_t = IxCollectionRO<EsbValueT>;
		using ix_item_t = IxCollectionItemRO<EsbValueT, interface_t>;
		IxCollectionRO(interface_t&& interface_) : IxCollectionImplRO<EsbValueT, interface_t>(std::move(interface_))
		{}
		const ix_item_t operator[](size_t index_) const					{ return ix_item_t{ this->m_interface, index_ }; }
		ESB_PROPERTY_ARRAY_RO(EsbValueT, At);
	};


	template<EsbClassConcept EsbValueT, class IxInterfacePtrT>
	class IxCollectionRWImpl : public IxCollectionImplRW<EsbValueT, IxInterfacePtrT> {
	public:
		using ix_item_t = IxCollectionItemRW<EsbValueT, IxInterfacePtrT>;
		IxCollectionRWImpl(IxInterfacePtrT&& interface_) : IxCollectionImplRW<EsbValueT, IxInterfacePtrT>(std::move(interface_))
		{}
		ix_item_t operator[](size_t index_)				{ return ix_item_t{ this->m_interface, index_ }; }
		const ix_item_t operator[](size_t index_) const	{ return ix_item_t{ this->m_interface, index_ }; }
		ESB_PROPERTY_ARRAY_RW(EsbValueT, At);
	};


	template<EsbClassConcept EsbValueT>
	class IxCollectionRW : public IxCollectionRWImpl<EsbValueT, IIxCollectionRWPtr> {
		using base_t = IxCollectionImplRO<EsbValueT, IIxCollectionRWPtr>;
	public:
		using typename base_t::interface_t;
		using ix_collection_t = IxCollectionRW<EsbValueT>;
		IxCollectionRW(interface_t&& interface_) : IxCollectionRWImpl<EsbValueT, interface_t>(std::move(interface_))
		{}
	};


	template<EsbClassConcept EsbValueT>
	class IxCollection : public IxCollectionRWImpl<EsbValueT, IIxCollectionPtr> {
		using base_t = IxCollectionImplRO<EsbValueT, IIxCollectionPtr>;
	public:
		using typename base_t::interface_t;
		IxCollection(interface_t&& interface_) : IxCollectionRWImpl<EsbValueT, IIxCollectionPtr>{ std::move(interface_) }
		{}
		void Resize(size_t new_size_)									{ return IxCollection_Resize(*this->m_interface, new_size_);	}
		void Insert(size_t at_index_, ConstPara<EsbValueT> value_)		{ return IxCollection_Insert(*this->m_interface, at_index_, value_);	}
		void Remove(size_t at_index_)									{ return IxCollection_Remove(*this->m_interface, at_index_);	}
	};

	using IxCollectionAny = IxCollection<Arbitrary>;


#define ESB_IMPLEMENT_INDEXED_COLLECTION_()																			\
	using ix_collection_t::At;																						\
	using ix_collection_t::operator[];																				\
	ix_collection_t& AsIndexedCollection()				{ return *static_cast<ix_collection_t*>(this); }			\
	const ix_collection_t& AsIndexedCollection() const	{ return *static_cast<const ix_collection_t*>(this); }

#define ESB_IMPLEMENT_INDEXED_COLLECTION_RO()																		\
	ESB_IMPLEMENT_INDEXED_COLLECTION_()																				\
	using ix_collection_t::GetAt;

#define ESB_IMPLEMENT_INDEXED_COLLECTION_RW()																		\
	ESB_IMPLEMENT_INDEXED_COLLECTION_RO()																			\
	using ix_collection_t::SetAt;




	// Псевдо-референс объект для итератора относится только к не-конст итератору!. Конст-итератор в качестве const_reference возвращает само значение (value_type)!
	// поэтому построен этот объект может быть только на IIxCollectionRWPtr, а не RO. 
	template<class EsbValueClassT>
	struct IxCollectionIteratorReft {
		using interface_t = IIxCollectionRWPtr;
		using interface_raw_t = template_argument_type_t<interface_t>;
		using val_t = EsbValueClassT;
		using me_t = IxCollectionIteratorReft<val_t>;
	protected:
#if ESB_USE_OWNING_COLLECTION_ITERATOR != 0
		interface_t		m_coll;
		interface_raw_t& coll() const { return *m_coll; }
#else
		interface_raw_t* m_coll;
		interface_raw_t& coll() const { assert(m_coll);  return *m_coll; }
#endif
		size_t		m_pos;
	public:
#if ESB_USE_OWNING_COLLECTION_ITERATOR != 0
		IxCollectionIteratorReft(const interface_t& collection_, size_t position_) noexcept : m_coll(collection_), m_pos(position_) {}	//copy interface
#else
		IxCollectionIteratorReft(interface_raw_t* collection_, size_t position_) noexcept : m_coll(collection_), m_pos(position_) {}
#endif
	public:
		friend bool operator== (const me_t& a, const me_t& b) noexcept	{ return (&a == &b) || (a.m_coll == b.m_coll && a.m_pos == b.m_pos); };
		val_t GetValue() const											{ return make<val_t>(IxCollection_GetAt(coll(), m_pos)); }
		void SetValue(const val_t& value_) 								{ IxCollection_SetAt(coll(), m_pos, value_); }
		void SetValue(val_t&& value_) 									{ IxCollection_SetAt(coll(), m_pos, std::move(value_)); }
		void SetValueFromVar(ConstPara<val_t> value_) 					{ IxCollection_SetAt(coll(), m_pos, value_); }
		void SetValueFrom(const me_t& other_)							{ if (*this != other_) IxCollection_Copy(other_.coll(), other_.m_pos, this->coll(), this->m_pos); }
		//NOTE	Наш свап принимает нас по значению, а не ссылке т.к. например std::iter_swap вызывает метод swap(*_Left, *_Right);
		//		а rvalue не вяжется к lvalue и непонятка выходит. Также можно было-бы декларироваться как const me_t& но для метода свап это тоже выглядит погано. Хз как лучше..
		friend void swap(me_t a, me_t b) noexcept						{ if (a != b) IxCollection_Swap(a.coll(), a.m_pos, b.coll(), b.m_pos); }
	public:
		operator val_t() const											{ return GetValue(); }
		me_t& operator=(const me_t& other_)								{ SetValueFrom(other_);			return *this; }
		me_t& operator=(me_t&& other_)									{ SetValueFrom(other_);			return *this; }	//копируем, не будем приводить other_ в invalid state
		me_t& operator=(const val_t& value_) 							{ SetValue(value_);				return *this; }
		me_t& operator=(val_t&& value_) 								{ SetValue(std::move(value_));	return *this; }
		// Если мы "ссылка" на Arbitrary, то можем позволить переместить в нас любое значение
		// NOTE	Любое, а не только примитивное значения. Принудительно делая std::move(..) сюда юзер знает, что творит..
		// (поскольку шаблон более ограничен чем просто метод, то при перемещении в нас Arbitrary выберется метод выше. конфликта не будет)
		template<EsbClassConcept EsbValueT>
		requires (is_esb_arbitrary<val_t>)
		me_t& operator=(EsbValueT&& value_) { 
			SetValue( std::move(value_).operator esb::Arbitrary&&() );
			return *this; 
		}
	};



	// Константный итератор может быть построен как на IIxCollectionRWPtr, так и на RO. Он всегда возвращает само значение. Ему объект псевдо-реф не нужен
	template<class EsbValueClassT, class IxCollectionInterfacePtrT>
	requires (std::is_same_v<IxCollectionInterfacePtrT, IIxCollectionRWPtr> || std::is_same_v<IxCollectionInterfacePtrT, IIxCollectionROPtr>)
	class IxCollectionIteratorConst {
		using me_t = IxCollectionIteratorConst<EsbValueClassT, IxCollectionInterfacePtrT>;
	protected:
		using val_t = EsbValueClassT;
		using interface_ptr_t = IxCollectionInterfacePtrT;
		using interface_raw_t = template_argument_type_t<interface_ptr_t>;
		using dif_t = std::ptrdiff_t;
	public:
		using const_reference = val_t;	// !!! Для "итератору-по-интерфейсу" const_reference должна быть собственно значением !!! Концепты !!!
		//
		using iterator_category = std::random_access_iterator_tag;
		using value_type = val_t;
		using difference_type = dif_t;
		using pointer = const_reference*;	//unused
		using reference = const_reference;
	protected:
#if ESB_USE_OWNING_COLLECTION_ITERATOR != 0
		interface_ptr_t		m_col;		// !!! must be NOT REF && NOT CONST for movable !!!
#else									//
		interface_raw_t*	m_col;		// !!! must be NOT REF && NOT CONST for movable !!!
#endif									//
		size_t				m_pos = 0;	// +++ must be copyable<> && default_initializable<>; !!!
#if ESB_USE_ITERATOR_DEBUG_LEVEL != 0
		size_t				m_size = 0;
		//
		void _check_col() const noexcept						{ ESB_VERIFY(this->m_col, "iterator not initialized");	}
		void _check_col(const me_t& other_) const noexcept		{ ESB_VERIFY(other_.m_col, "other iterator not initialized");	}
		void _check_compat(const me_t& other_) const noexcept	{ ESB_VERIFY(other_.m_col && this->m_col == other_.m_col, "other iterators not initialized or incompatible"); }
		void _check_pos_dec() const noexcept					{ ESB_VERIFY(this->m_pos > 0, "cannot decrement iterator"); }
		// pos == size есть допустимая позиция для итератора - это end-итератор. Но pos==size (end) недопустима для deref
		void _check_pos_inc() const noexcept					{ ESB_VERIFY(this->m_pos < m_size, "cannot increment iterator");	}
		void _check_pos_add(const dif_t offset_) const noexcept	{
			ESB_VERIFY(offset_ != 0, "invalid offset for iterator");
			if (offset_ < 0)
				ESB_VERIFY(this->m_pos >= static_cast<unsigned>(-offset_), "cannot decrement iterator -offset");
			else if (offset_ > 0)
				ESB_VERIFY(this->m_pos + offset_ <= m_size, "cannot increment iterator +offset");
		}
		void _check_ref() const noexcept						{ ESB_VERIFY(this->m_pos < m_size, "cannot dereference iterator"); }
		void _check_ref_at(const dif_t offset_) const noexcept	{
			if (offset_ < 0)
				ESB_VERIFY(this->m_pos >= static_cast<unsigned>(-offset_), "cannot dereference iterator at -offset");
			else if (offset_ > 0)
				ESB_VERIFY(this->m_pos + offset_ < m_size, "cannot dereference iterator at +offset");
		}
#else
		void _check_col() const noexcept {}
		void _check_col(const me_t&) const noexcept {}
		void _check_compat(const me_t&) const noexcept {}
		void _check_pos_dec() const noexcept {}
		void _check_pos_inc() const noexcept {}
		void _check_pos_add(const difference_type) const noexcept {}
		void _check_ref() const noexcept {}
		void _check_ref_at(const difference_type) const noexcept {}
#endif // ESB_USE_ITERATOR_DEBUG_LEVEL != 0
		void _dec() noexcept						{ _check_pos_dec();			--this->m_pos;	}
		void _inc() noexcept						{ _check_pos_inc();			++this->m_pos;	}
		void _add(const dif_t offs_) noexcept		{ _check_pos_add(offs_);	this->m_pos += offs_; }
	public:
		IxCollectionIteratorConst() : m_col{ nullptr } {} //= default !!! Концепты!!!;
#if ESB_USE_OWNING_COLLECTION_ITERATOR != 0
		// мы всегда принимаем интерфейс в raw-виде, но в дебаге создаем для него новый интерфейс-птр
		// принимать в raw-виде нужно потому, что наш интерфейс не должен быть конст для удовлетворения концептам,
		// а коллекции, когда они конст, не-константный интерфейс-птр отдать не могут. а не-константный raw-интерфейс из конст-птр отдать могут.
		IxCollectionIteratorConst(interface_raw_t& collection_, size_t position_) noexcept : m_col(interface_ptr_t{ collection_ }), m_pos(position_)
#else
		IxCollectionIteratorConst(interface_raw_t& collection_, size_t position_) noexcept : m_col(&collection_), m_pos(position_)
#endif
		{
#if ESB_USE_ITERATOR_DEBUG_LEVEL != 0
			m_size = IxCollection_Size(collection_);
#endif
		}

	public:
		ESB_NODISCARD const_reference operator*() const {
			_check_col();
			_check_ref();
			return make<val_t>(IxCollection_GetAt(*m_col, m_pos)); //!!! return value! not ref!
		}	
		ESB_NODISCARD const_reference operator[](const dif_t offset_) const noexcept {
			_check_col();
			_check_ref_at(offset_);
			return make<val_t>(IxCollection_GetAt(*m_col, m_pos + offset_)); //!!! return value! not ref!
		}
		//
		me_t& operator++() noexcept											{ _inc();							return *this; }
		me_t operator++(int) noexcept										{ me_t tmp = *this; _inc();			return tmp; }
		me_t& operator--() noexcept											{ _dec();							return *this;	}
		me_t operator--(int) noexcept										{ me_t tmp = *this; _dec();			return tmp; }
		me_t& operator+=(const dif_t offset_) noexcept						{ _add(offset_);					return *this; 	}
		ESB_NODISCARD me_t operator+(const dif_t offset_) const noexcept	{ me_t tmp = *this; tmp += offset_; return tmp; }
		me_t& operator-=(const dif_t offset_) noexcept						{ return *this += -offset_;	}	// МИНУС offset!
		ESB_NODISCARD me_t operator-(const dif_t offset_) const noexcept	{ me_t tmp = *this; tmp -= offset_; return tmp; }
		ESB_NODISCARD dif_t operator-(const me_t& other_) const noexcept	{ 
			_check_col();
			_check_compat(other_);
			return static_cast<difference_type>(m_pos) - static_cast<difference_type>(other_.m_pos);
		}
		ESB_NODISCARD bool operator==(const me_t& other_) const				{ 
			_check_col();
			_check_col(other_); 
			return (m_col == other_.m_col && m_pos == other_.m_pos); 
		}
		ESB_NODISCARD std::strong_ordering operator<=>(const me_t& other_) const noexcept {
			_check_col();
			_check_col(other_);
			const auto col_compare_result = (this->m_col <=> other_.m_col);
			return (col_compare_result == std::strong_ordering::equal)? (this->m_pos <=> other_.m_pos) : col_compare_result;
		}
	};
	//
	// И еще, мать его, надо чтобы к оффсету можно было добавить итератор! Этот итератор (справа от +) принимается byval, модифицируется и возвращается (move-конструкторы!)
	template<class EsbValueClassT, class IxCollectionInterfacePtrT>
	ESB_NODISCARD inline IxCollectionIteratorConst<EsbValueClassT, IxCollectionInterfacePtrT> operator+(
					typename IxCollectionIteratorConst<EsbValueClassT, IxCollectionInterfacePtrT>::difference_type lval_offset_,
					IxCollectionIteratorConst<EsbValueClassT, IxCollectionInterfacePtrT> rval_iterator_) noexcept {
		return rval_iterator_ += lval_offset_;
	}
	//
	static_assert(std::random_access_iterator<IxCollectionIteratorConst<Arbitrary, IIxCollectionROPtr>>);

	template<class EsbValueClassT>
	class IxCollectionIterator : public IxCollectionIteratorConst<EsbValueClassT, IIxCollectionRWPtr> {
		using base_t = IxCollectionIteratorConst<EsbValueClassT, IIxCollectionRWPtr>;
		using me_t = IxCollectionIterator;
		using val_t = typename base_t::val_t;
		using diff_t = typename base_t::dif_t;;
		using ref_t = IxCollectionIteratorReft<val_t>;
	public:
		using const_reference = base_t::const_reference;
		//
		using iterator_category = std::random_access_iterator_tag;
		using value_type = val_t;
		using difference_type = base_t::difference_type;
		using pointer = ref_t*;
		using reference = ref_t;
		//
		using base_t::base_t;		// Конструкторы от базового - все данные там, нам он подходят, у нам ничего особого по поводу конструирования сказать нечего
	public:
		_NODISCARD reference operator*() const noexcept {
			this->_check_col();
			this->_check_ref();
			return ref_t(this->m_col, this->m_pos);
		}
		ESB_NODISCARD reference operator[](const difference_type offset_) const noexcept {
			this->_check_col();
			this->_check_ref_at(offset_);
			return ref_t(this->m_col, this->m_pos + offset_);
		}
		//
		me_t& operator++() noexcept											{ base_t::operator++();						return *this; }
		me_t operator++(int) noexcept										{ me_t tmp = *this;	base_t::operator++();	return tmp; }
		me_t& operator--() noexcept											{ base_t::operator--();						return *this; }
		me_t operator--(int) noexcept										{ me_t tmp = *this; base_t::operator--();	return tmp; }
		me_t& operator+=(const diff_t offset_) noexcept						{ base_t::operator+=(offset_);				return *this; }
		ESB_NODISCARD me_t operator+(const diff_t offset_) const noexcept	{ me_t tmp = *this; tmp += offset_;			return tmp; }
		me_t& operator-=(const diff_t offset_) noexcept						{ base_t::operator-=(offset_);				return *this; }
		using base_t::operator-;	//difference_type operator-(const me_t& other_) - возвращает не итератор, а дифф-тип - можно просто переиспользовать
		ESB_NODISCARD me_t operator-(const diff_t offset_) const noexcept	{ me_t tmp = *this; tmp -= offset_;			return tmp;	}
	};
	//
	template<class EsbValueClassT>
	ESB_NODISCARD inline IxCollectionIterator<EsbValueClassT> operator+(
				typename IxCollectionIterator<EsbValueClassT>::difference_type lval_offset_, 
				IxCollectionIterator<EsbValueClassT> rval_iterator_) noexcept {
		return rval_iterator_ += lval_offset_;
	}
	//
	static_assert(std::random_access_iterator<IxCollectionIterator<String>>);
	//
	using IxCollectionIteratorAny = IxCollectionIterator<Arbitrary>;
	//
	// ^^^^^^^^^^^^^^^^^^^^^^^^^
	// Индексированная коллекция --------------------------------------------------------------------------------------------------------------------
	// ----------------------------------------------------------------------------------------------------------------------------------------------
	// 
	

	// 
	// ----------------------------------------------------------------------------------------------------------------------------------------------
	// Ассоциативная коллекция ----------------------------------------------------------------------------------------------------------------------
	// vvvvvvvvvvvvvvvvvvvvvvv
	
	template<EsbClassConcept EsbClassKeyT, EsbClassConcept EsbClassValT>
	struct ValuePair {
		using interface_t = IValuePairPtr;
	private:
		interface_t	m_interface;
	protected:
		ValuePair(nullptr_t) : m_interface(nullptr) {}
		ValuePair(IValuePairPtr&& interface_) : m_interface(std::move(interface_))
		{}
		ESB_CLASS_DECLARE_NULLABLE(ValuePair);
		bool is_null() const { return m_interface == nullptr; }
		const interface_t& get_interface() const { return m_interface; }
	public:
		// ВАЖНО!	Очень желательно чтобы методы на которых строятся свойства не путались в одной области видимости при наследовании
		//			или при ESB_PROPERTY_FIELD_RO крах МС компилятора!
		//			Поэтому называем Get...
		EsbClassKeyT GetValue1st() const { return make<EsbClassKeyT>(IValuePair_GetKey(*m_interface)); }
		EsbClassValT GetValue2nd() const { return make<EsbClassValT>(IValuePair_GetValue(*m_interface)); }
	};

	using ValuePairAny = ValuePair<Arbitrary, Arbitrary>;


	class KeyAndValue;

	template<EsbClassConcept EsbClassKeyT, EsbClassConcept EsbClassValT>
	class AxCollectionPair : public ValuePair<EsbClassKeyT, EsbClassValT> {
		using base_t = ValuePair<EsbClassKeyT, EsbClassValT>;
	public:
		using base_t::interface_t;
	private:
		ESB_CLASS_DECLARE_NULLABLE(AxCollectionPair);
		AxCollectionPair(nullptr_t) : base_t(nullptr) {}
		using base_t::is_null;
		// нас создают в виде nullable и ему мы доступ к нашим конструкторам дали. поэтому пока более никому наши публичные конструкторы не нужны.
		AxCollectionPair(interface_t&& interface_) : base_t(std::move(interface_))
		{}
	public:
		EsbClassKeyT GetKey() const			{ return base_t::GetValue1st(); }
		EsbClassValT GetValue() const		{ return base_t::GetValue2nd(); }
		ESB_PROPERTY_FIELD_RO(EsbClassKeyT, Key);
		ESB_PROPERTY_FIELD_RO(EsbClassValT, Value);
	public:
		KeyAndValue MakeKeyAndValue() const; //{ после KeyAndValue }
	};

	using AxCollectionPairAny = AxCollectionPair<Arbitrary, Arbitrary>;



	template<class EsbClassKeyT, class EsbClassValT, class AxInterfacePtrT>
	class AxCollectionImplRO {
	protected:
		AxInterfacePtrT m_interface;
	public:
		using pair_t = AxCollectionPair<EsbClassKeyT, EsbClassValT>;
		AxCollectionImplRO(AxInterfacePtrT&& interface_) : m_interface(std::move(interface_))
		{}
		size_t				Size() const								{ return AxCollection_Size(*m_interface); }
		//если не найдено возвращает ifptr(nullptr) и этот nullptr туда, в опт, и перемещается
		ClassOpt<pair_t>	Find(ConstPara<EsbClassKeyT> key_) const	{ return make_opt<pair_t>( AxCollection_Find(*this->m_interface, key_) ); }
	};


	template<class EsbClassKeyT, class EsbClassValT>
	class AxCollectionRO : public AxCollectionImplRO<EsbClassKeyT, EsbClassValT, IAxCollectionROPtr> {
	protected:
		using AxCollectionImplRO<EsbClassKeyT, EsbClassValT, IAxCollectionROPtr>::m_interface;
	public:
		using interface_t = IAxCollectionROPtr;
		using key_t = EsbClassKeyT;
		using value_t = EsbClassValT;
		AxCollectionRO(IAxCollectionROPtr&& interface_) : AxCollectionImplRO<EsbClassKeyT, EsbClassValT, IAxCollectionROPtr>(std::move(interface_))
		{}
	};


	template<class EsbClassKeyT, class EsbClassValT, class AxInterfacePtrT>
	class AxCollectionImplRW : public AxCollectionImplRO<EsbClassKeyT, EsbClassValT, AxInterfacePtrT> {
	protected:
		using AxCollectionImplRO<EsbClassKeyT, EsbClassValT, AxInterfacePtrT>::m_interface;
	public:
		AxCollectionImplRW(AxInterfacePtrT&& interface_) : AxCollectionImplRO<EsbClassKeyT, EsbClassValT, AxInterfacePtrT>(std::move(interface_))
		{}
		void Replace(ConstPara<EsbClassKeyT> Key_, ConstPara<EsbClassValT> NewValue_)	{ return AxCollection_Replace(*this->m_interface, Key_, NewValue_); }
	};


	template<class EsbClassKeyT, class EsbClassValT>
	class AxCollectionRW : public AxCollectionImplRW<EsbClassKeyT, EsbClassValT, IAxCollectionRWPtr> {
	public:
		using interface_t = IAxCollectionRWPtr;
		using key_t = EsbClassKeyT;
		using value_t = EsbClassValT;
		AxCollectionRW(IAxCollectionRWPtr&& interface_) : AxCollectionImplRW<EsbClassKeyT, EsbClassValT, IAxCollectionRWPtr>(std::move(interface_))
		{}
	};


	// Классы AxCollection[xx] - это примитивная имплементация интерфейсов коллекции. без всяких свойств и операторов, т.к. стратегия имплементация этой
	// функциональности сильно зависит от класса, который имплементирует этот интерфейс коллекции.
	// Map к примеру реализует стратегии Get-Or-Undef + Insert-Or-Replace. 
	// А для QueryResult-лалала вообще реализовывать этот интерфейс излишне - вся функциональность реализуется через динамическую коллекцию свойств
	template<class EsbClassKeyT, class EsbClassValT>
	class AxCollection : public AxCollectionImplRW<EsbClassKeyT, EsbClassValT, IAxCollectionPtr> {
	public:
		using interface_t = IAxCollectionPtr;
		AxCollection(interface_t&& interface_) : AxCollectionImplRW<EsbClassKeyT, EsbClassValT, IAxCollectionPtr>(std::move(interface_))
		{}
		using key_t = EsbClassKeyT;
		using value_t = EsbClassValT;
		void Insert(ConstPara<EsbClassKeyT> Key_, ConstPara<EsbClassValT> Value_)	{ return AxCollection_Insert(*this->m_interface, Key_, Value_);		}
		void Remove(ConstPara<EsbClassKeyT> Key_)									{ return AxCollection_Remove(*this->m_interface, Key_);				}
		void Clear()																{ return AxCollection_Clear(*this->m_interface);					}
	};



	// Специализированная имплементация интерфейса AxCollection для Соответствие (Map/FixedMap)
	// Для получения значения используется стратегия get-or-undef, т.е. если ключ отсутствует в коллекции, то возвращается Undef
	// Для установки значения используется стратегия replase-or-insert, т.е. если ключ присутствует в коллекции, то замещаем значения иначе добавляем новую пару
	template<class AxCollectionInterfaceT>
	struct AxCollectionMapItemROImpl
	{
		using interface_t = AxCollectionInterfaceT;
		using key_t = Arbitrary;
		using value_t = Arbitrary;
	private:
		// Мы независимый объект-ассессор для значения и можем жить дольше чем тот, кто нас отдал (создал)
		// для этого мы удерживаем интерфейс коллекции и объект ключа.
		// 
		// TOBE: Можно сделать два режима - а) когда все псевдо-итераторы незавимимы и б) когда они все ссылки
		//		 Все таки с++ - ссылок бояться в с++ не ходить...
		// 
		// вызов GetterT::getValue мы делаем каждый раз когда у нас запрашивают значение. мы не кэширум значение, поскольку не наше это дело.
		// мы всего лишь прокси для интерфейса и как он имплементирован у объекта мы представления не имеем. Кто знает больше - пусть кэширует при необходимости.
		//
		// По сути мы константа в том смысле, что нам нельзя поменять указатель на интерфейс, либо значение ключа - мы всегда указываем на тот элемент коллекции
		// для которога нас создали.
		// Но! Делать интерфейс\ключ константой нельзя т.к. класс тогда становится не копируемым\перемещаемым что лишнее и ненужное ограничение.
		// Наша константность обеспечивается нашим интерфейсом - у нас просто нет публичных методов изменения значения.
		/*const*/ interface_t	m_interface;		// у нас создается копия InterfacePtr
		/*const*/ Var<key_t>	m_key;
	protected:
		const interface_t&	get_interface() const	{ return m_interface; }
		const Var<key_t>&	get_key() const			{ return m_key; }
	public:
		AxCollectionMapItemROImpl(const interface_t& interface_, Var<key_t>&& key_) : m_interface(interface_), m_key(std::move(key_))
		{}
		struct value_getter_t {
			static Arbitrary GetArbitraryValueOrUndef(const interface_t& interface_, ConstPara<key_t> key_) {
				if (IValuePairPtr ppair = AxCollection_Find(*interface_, key_))
					return make<Arbitrary>(IValuePair_GetValue(*ppair));
				else
					return MakeArbitraryDefault<Undef>();
			}
		};
		value_t GetValue() const	{ return value_getter_t::GetArbitraryValueOrUndef(*m_interface, m_key); }
		operator value_t() const	{ return GetValue(); }
	};



	struct AxCollectionMapItemRW;

	class AxCollectionMapItemRO : public AxCollectionMapItemROImpl<IAxCollectionROPtr>
	{
		using AxCollectionMapItemROImpl<IAxCollectionROPtr>::get_interface;
		using AxCollectionMapItemROImpl<IAxCollectionROPtr>::get_key;
		friend AxCollectionMapItemRW;
	public:
		AxCollectionMapItemRO(const interface_t& interface_, Var<key_t>&& key_) : AxCollectionMapItemROImpl(interface_, std::move(key_))
		{}
		ESB_PROPERTY_FIELD_RO(value_t, Value);
	};

	
	struct AxCollectionMapItemRW : public AxCollectionMapItemROImpl<IAxCollectionPtr>
	{
		AxCollectionMapItemRW(const interface_t& interface_, Var<key_t>&& key_)	: AxCollectionMapItemROImpl(interface_, std::move(key_))
		{}
		struct value_setter_t {
			static void SetValueReplaceOrInsert(const interface_t& interface_, ConstPara<key_t> key_, ConstPara<value_t> value_) {
				if (IValuePairPtr ppair = AxCollection_Find(*interface_, key_))
					AxCollection_Replace(*interface_, key_, value_);
				else
					AxCollection_Insert(*interface_, key_, value_);
			}
		};
		void SetValue(ConstPara<value_t> value_)							{ 
			return value_setter_t::SetValueReplaceOrInsert(*get_interface(), get_key(), value_);
		}
		AxCollectionMapItemRW& operator=(ConstPara<value_t> NewValue_)	{
			SetValue(NewValue_);
			return *this;
		}
		// См. Примечание для IxCollectionItemRW& operator =
		AxCollectionMapItemRW& operator =(const AxCollectionMapItemRW& other_) {
			if (&other_ != this) {				
				//TOBE: мы не проверяем на присвоение объекта с таким же ключем - мы просто не знаем что такое "тот-же-самый-ключ-для-коллекции"
				//		если узнаем, то можно немного оптимизировать
				AxCollection_ReplaceOrInsertFromGetOrUndef(*get_interface(), get_key(), *other_.get_interface(), other_.get_key());
			}
			return *this;
		}
		AxCollectionMapItemRW& operator =(const AxCollectionMapItemRO& other_) {
			// проверять на само-присвоение здесь не надо, т.к. AxCollectionMapItemRO это элемент другого класса - FixedMap
			AxCollection_ReplaceOrInsertFromGetOrUndef(*get_interface(), get_key(), *other_.get_interface(), other_.get_key());
			return *this;
		}

		ESB_PROPERTY_FIELD_RW(value_t, Value);
	};



	// реализация AxCollection для Map. Интерфейс полный, Ключ Arbitrary, Значение Arbitrary, стратегия гет - ValueOrUndef, стратегия сет - ReplaceOrInsert
	class AxCollectionMap : public AxCollection<Arbitrary, Arbitrary> {
	public:
		AxCollectionMap(IAxCollectionPtr&& interface_) : AxCollection<key_t, value_t>(std::move(interface_))
		{}
	public:
		using ax_collection_t = AxCollectionMap;
		using ax_item_t = AxCollectionMapItemRW;
		
		value_t GetValue(ConstPara<key_t> key_) const {
			return ax_item_t::value_getter_t::GetArbitraryValueOrUndef(*this->m_interface, key_);
		}
		void SetValue(ConstPara<key_t> key_, ConstPara<value_t> value_) {
			return ax_item_t::value_setter_t::SetValueReplaceOrInsert(*this->m_interface, key_, value_);
		}
		ESB_PROPERTY_ARRAY_RW(value_t, Value);
		ax_item_t operator[](Var<key_t> key_) {	// принимаем byval и далее перемещаем. при этом ключ может тихо конструироваться из любого типа (см.Var<Arbitrary>)
			return ax_item_t{*this->m_interface, std::move(key_)};
		}
		const ax_item_t operator[](Var<key_t> key_) const {	
			return ax_item_t{ *this->m_interface, std::move(key_) };
		}
	};


	class AxCollectionMapRO : public AxCollectionRO<Arbitrary, Arbitrary> {
	public:
		AxCollectionMapRO(IAxCollectionROPtr&& interface_) : AxCollectionRO<key_t, value_t>(std::move(interface_))
		{}
	public:
		using ax_collection_t = AxCollectionMapRO;
		using ax_item_t = AxCollectionMapItemRO;

		value_t GetValue(ConstPara<key_t> key_) {
			return ax_item_t::value_getter_t::GetArbitraryValueOrUndef(*this->m_interface, key_);
		}
		ESB_PROPERTY_ARRAY_RO(value_t, Value);
		const ax_item_t  operator[](Var<key_t> key_) const {
			return ax_item_t{*this->m_interface, std::move(key_) };
		}
	};
	//
	// ^^^^^^^^^^^^^^^^^^^^^^^
	// Ассоциативная коллекция ----------------------------------------------------------------------------------------------------------------------
	// ----------------------------------------------------------------------------------------------------------------------------------------------



	// Енумератор коллекции через интерфейс (IEnumXXXX) ---------------------------------------------------------------------------------------------
	// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

	//TOBE: Ну оочень примитивная реализация. Нужно сделать полноценный итератор плюс с возможностью обновления элемента коллекции при проходе.

	template<EsbClassConcept EsbClassT>
	class EnumeratorOf;

//TOBE: Операторы здесь удалены т.к. const IEnumValuesPtr& m_enumerator. Правильно сделать как в итераторе массива - в зависимости от ESB_USE_OWNING_COLLECTION_ITERATOR
//TOBE	Также при переработке нужно учесть, что интерфейс вроде-бы позволяет делать read-write енумератор. возможно у разных классов по-разному имплементировано.
//		Нужно разбираться..
//TOBE	Также (возможно?) в зависимости от имплементации skip можно пытаться делать больше чем forward-only итератор. смотреть надо..
//		поскольку тут полноценно прорабатывать надо пока просто задавлю варн
ESB_WARNING_SUPRESS_IMPLICIT_ASSIGN_DELETED_WHEN_REF_MEMBER()
//
	template<EsbClassConcept EsbClassT>
	class EnumeratorOfIterator {
		const IEnumValuesPtr&	m_enumerator;
		IValuePtr				m_val;
		void setEOF()			{ m_val.reset(); }
		bool isOK() const		{ return (m_val != nullptr ); }
		bool isEOF() const		{ return !isOK(); }
		void fetch_next()		{ m_val = IEnumValues_Next(*m_enumerator); }
		// создать нас может только класс EnumItems. Причем что он решает как нас создать - просто конструктор - из текущего состояния - мы должны вызвать next
		// нр. для Begin EnumItems вызывает reset, а мы вызываем next
		// конструктор с nullptr_t - конструктор для End(). мы не вызываем next и остаемся в состоянии EOF.
		friend EnumeratorOf<EsbClassT>;
		EnumeratorOfIterator(const IEnumValuesPtr& enumerator_) : m_enumerator(enumerator_), m_val(IEnumValues_Next(*m_enumerator)) {} // { fetch_next(); }
		EnumeratorOfIterator(const IEnumValuesPtr& enumerator_, nullptr_t) : m_enumerator(enumerator_), m_val(nullptr)				{}
	public:
		using value_t = EsbClassT;
		using iterator_t = EnumeratorOfIterator<value_t>;
		//TOBE	Нужно точно проверить... в сценарии 
		//			for (auto&& v : arr_.EnumItems())
		//				vector_of_n.emplace_back(std::move(v).as_value<Numeric>());
		//		Когда мы умираем должен отрабатывать этот оператор, в котором мы move-делаем требуемый тип из нашего IValuePtr (обычно Arbitrary)
		//		этот экземпляр по универсальной ссылке передается вызывающему, у вызывающего он тоже привязывается к универсальной ссылке,
		//		и тоже через move кастится к нужному Numeric и у целевого контейнера он помещается через move-конструктор
		//		Т.е. не должно быть ни одного коприрования - никаких Addref/Release
		//	!!!	Возможная проблема - после того как мы обнулили наш IValuePtr мы становимся как-бы isEOF и если нас до смерти еще кто-то проверит на ==end()
		//		то итерация неожидаемо прервется!!! Как этого избежать?? 
		value_t&& operator*() && {
			return check_and_make<value_t>(std::move(m_val));
		}
		value_t operator*() const & {
			// мы не кэшируем значение и проверяем (создаем) его каждый раз по требованию
			return check_and_make_copy<value_t>(m_val);
		}
		// pointer operator->() const noexcept ?? - возможен только для Value/Arbitrary или если мы будем содержать в себе полное значение Var или OptPtr
		iterator_t& operator++() {
			fetch_next();
			return *this;
		}
		// постфикс++. неэффективное полное копирование. не имплементируем. также и всякие operator+
		iterator_t& operator+=(size_t n_skip_) noexcept {
			bool b_has_value = IEnumValues_Skip(*m_enumerator, n_skip_);
			if (b_has_value) 
				fetch_next();  
			else 
				setEOF();
			return *this;
		}
		bool operator==(const iterator_t& other_) const noexcept {
			// равны могут быть только два итератора в состоянии EOF. иные не равны.
			assert(&m_enumerator == &other_.m_enumerator);	//мы должны ссылаться на один и тот-же енумератор.
			return (isEOF() && other_.isEOF());
		}
	};
//
ESB_WARNING_RESTORE()	//ESB_WARNING_SUPRESS_IMPLICIT_ASSIGN_DELETED_WHEN_REF_MEMBER


	template<EsbClassConcept EsbClassT>
	class EnumeratorOf {
		// не конст, потому что теряем копируемость\перемещаемость. просто нам интерфейс через наш публичный интерфейс поменять нельзя
		IEnumValuesPtr m_interface;
		ESB_CLASS_DECLARE_NULLABLE(EnumeratorOf);
		bool is_null() const { return m_interface == nullptr; }
		EnumeratorOf(nullptr_t) : m_interface(nullptr)	{}
	public:
		using interface_t = IEnumValuesPtr;
		using value_t = EsbClassT;
		using iterator_t = EnumeratorOfIterator<value_t>;
		EnumeratorOf(const IEnumValuesPtr&& interface_) : m_interface(std::move(interface_))
		{}
		bool Reset() const								{ return IEnumValues_Reset(*m_interface); }
		bool GetNext(RefPara<EsbClassT> out_) const		{ return IEnumValues_Next(*m_interface, out_);	}
		bool Skip(size_t n_skip_) const					{ IEnumValues_Skip(*m_interface, n_skip_); }
		ClassOpt<EnumeratorOf> Clone() const			{ return make_opt<EnumeratorOf>(IEnumValues_Clone(*m_interface)); }
		iterator_t begin() const						{ IEnumValues_Reset(*m_interface);		return iterator_t{ m_interface }; }
		iterator_t end() const							{ return iterator_t{ m_interface, nullptr };	}
	};

	using EnumeratorOfAny = EnumeratorOf<Arbitrary>;


#define ESB_IMPLEMENT_ENUM_ITEMS(CLASS_T_, ITEM_T_)		\
	EnumeratorOf<ITEM_T_> EnumItems() const {			\
	return EnumeratorOf<ITEM_T_>{  query_interface<CLASS_T_, IEnumValuesPtr>( *esb::get_interface(*this) )  }; }


//
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// Енумератор коллекции через интерфейс (IEnumXXXX) ---------------------------------------------------------------------------------------------




	// Динамическая коллекция свойств объектного интерфейса -----------------------------------------------------------------------------------------
	// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
	//
	class DynamicCollectionPropertyBase {
		// мы специально делаем св-во как отдельный объект, а не ссылку на источник чтобы св-во могло жить самостоятельно
		//TOBE	Также надо через ESB_USE_OWNING_COLLECTION_ITERATOR делать разные варианты
		dispid_t	m_id;	// < 0 : invalid, not found
		IObjectPtr	m_ctx;
	protected:
		DynamicCollectionPropertyBase(const IObjectPtr& ctx_, const String& name_) : m_ctx{ ctx_ }
		{ 
			m_id = IObjectProperties_Find(*m_ctx, name_);
		}
#if ESB_USE_OBJECT_DISPATCH_FIND_STRVIEW_HACK
		DynamicCollectionPropertyBase(const IObjectPtr& ctx_, const strview_t& name_) : m_ctx{ ctx_ }
		{
			m_id = IObjectProperties_Find(*m_ctx, name_);
		}
#endif
		const IObjectPtr& get_context() const { return m_ctx; }
	public:
		bool		isOk() const		{ return is_valid_dispid(m_id); }
		dispix_t	GetId() const		{ if (!isOk()) ESL_THROW_OBJECT_PROP_NOT_FOUND();		return as_dispix(m_id); }
	public:
		explicit operator bool() const	{ return isOk(); }
	public:
		ESB_PROPERTY_FIELD_RO(dispix_t, Id);
	};


	template<EsbClassConcept ValueT>
	class DynamicCollectionPropertyRO : public DynamicCollectionPropertyBase {
	public:
		using value_t = ValueT;
		DynamicCollectionPropertyRO(const IObjectPtr& ctx_, const String& name_) : DynamicCollectionPropertyBase{ ctx_, name_ }
		{}
		DynamicCollectionPropertyRO(const IObjectPtr& ctx_, const strview_t& name_) : DynamicCollectionPropertyBase{ ctx_, name_ }
		{}
	public:
		value_t GetValue() const	{ return check_and_make<value_t>(IObjectProperties_GetValue(*this->get_context(), this->GetId())); }
	public:
		operator value_t() const	{ return GetValue(); }
		ESB_PROPERTY_FIELD_RO(value_t, Value);
	};


	template<EsbClassConcept ValueT>
	class DynamicCollectionPropertyWO : public DynamicCollectionPropertyBase {
	public:
		using value_t = ValueT;
		DynamicCollectionPropertyWO(const IObjectPtr& ctx_, const String& name_) : DynamicCollectionPropertyBase{ ctx_, name_ }
		{}
		DynamicCollectionPropertyWO(const IObjectPtr& ctx_, const strview_t& name_) : DynamicCollectionPropertyBase{ ctx_, name_ }
		{}
	public:
		void SetValue(ConstPara<value_t> Value_)								{ return IObjectProperties_SetValue(*this->get_context(), this->GetId(), Value_); }
		DynamicCollectionPropertyWO& operator =(const value_t& value_)		{ SetValue(value_);	return *this; }
	public:
		ESB_PROPERTY_FIELD_WO(value_t, Value);
	};


	template<EsbClassConcept ValueT>
	class DynamicCollectionPropertyRW : public DynamicCollectionPropertyRO<ValueT> {
	public:
		DynamicCollectionPropertyRW(const IObjectPtr& ctx_, const String& name_) : DynamicCollectionPropertyRO<ValueT>{ ctx_, name_ }
		{}
		DynamicCollectionPropertyRW(const IObjectPtr& ctx_, const strview_t& name_) : DynamicCollectionPropertyRO<ValueT>{ ctx_, name_ }
		{}
	public:
		using DynamicCollectionPropertyRO<ValueT>::value_t;
		void SetValue(ConstPara<value_t> Value_)								{ return IObjectProperties_SetValue(*this->get_context(), this->GetId(), Value_); }
	public:
		DynamicCollectionPropertyRW& operator =(const value_t& value_)		{ SetValue(value_);	return *this; }
		ESB_PROPERTY_FIELD_RW(value_t, Value);
	};


	//интеллисенс не справляется с определением концепта для класса который от нас наследуется. все компилируется, но краснота раздражает...
	//template<EsbClassObject EsbObjectT, EsbClass EsbValueT>
	template<class EsbObjectT, EsbClassConcept EsbValueT, class CollectionPropertyClassT>
	class DynamicCollectionImplBaseT {
	protected:
		const EsbObjectT& me() const										{ return *static_cast<const EsbObjectT*>(this); }
		IObject& me_context() const { return *get_context(this->me()); }
	public:
		int Size() const															{ return IObjectProperties_Size(me_context()); }
		int GetFieldId(const String& name_) const									{ return IObjectProperties_Find(me_context(), name_); }
		CollectionPropertyClassT operator[](const String& name_) const				{ return CollectionPropertyClassT{ get_context(me()), name_ }; }
#if ESB_USE_OBJECT_DISPATCH_FIND_STRVIEW_HACK
		CollectionPropertyClassT operator[](const strview_t& name_) const			{ return CollectionPropertyClassT{ get_context(me()), name_ }; }
#endif
	};


	template<class EsbObjectT, EsbClassConcept EsbValueT>
	class DynamicCollectionImplRO : public DynamicCollectionImplBaseT<EsbObjectT, EsbValueT, DynamicCollectionPropertyRO<EsbValueT>> {
	public:
		using dx_collection_t = DynamicCollectionImplRO<EsbObjectT, EsbValueT>;
		using me_accessor_t = DynamicCollectionImplBaseT<EsbObjectT, EsbValueT, DynamicCollectionPropertyRO<EsbValueT>>;
		using key_t = String;
		using value_t = EsbValueT;
		value_t GetField(dispix_t ix_) const						{ return check_and_make<value_t>(IObjectProperties_GetValue(this->me_context(), as_dispid(ix_))); }
		ESB_PROPERTY_ARRAY_RO(value_t, Field);
	};


	template<class EsbObjectT, EsbClassConcept EsbValueT>
	class DynamicCollectionImplWO : public DynamicCollectionImplBaseT<EsbObjectT, EsbValueT, DynamicCollectionPropertyWO<EsbValueT>> {
	public:
		using dx_collection_t = DynamicCollectionImplWO<EsbObjectT, EsbValueT>;
		using me_accessor_t = DynamicCollectionImplBaseT<EsbObjectT, EsbValueT, DynamicCollectionPropertyWO<EsbValueT>>;
		using value_t = EsbValueT;
		void SetField(dispix_t ix_, ConstPara<value_t> Value_)		{ return IObjectProperties_SetValue(this->me_context(), as_dispid(ix_), Value_); }
		ESB_PROPERTY_ARRAY_WO(value_t, Field);
	};


	template<class EsbObjectT, EsbClassConcept EsbValueT>
	class DynamicCollectionImplRW : public DynamicCollectionImplBaseT<EsbObjectT, EsbValueT, DynamicCollectionPropertyRW<EsbValueT>> {
	public:
		using dx_collection_t = DynamicCollectionImplRW<EsbObjectT, EsbValueT>;
		using me_accessor_t = DynamicCollectionImplBaseT<EsbObjectT, EsbValueT, DynamicCollectionPropertyRW<EsbValueT>>;
		using value_t = EsbValueT;
		value_t GetField(dispix_t ix_) const						{ return check_and_make<value_t>(IObjectProperties_GetValue(this->me_context(), as_dispid(ix_))); }
		void SetField(dispix_t ix_, ConstPara<value_t> Value_)		{ return IObjectProperties_SetValue(this->me_context(), as_dispid(ix_), Value_); }
		ESB_PROPERTY_ARRAY_RW(value_t, Field);
	};


#define ESB_FRIEND_DYNAMIC_COLLECTION()		friend dx_collection_t::me_accessor_t;

#define ESB_IMPLEMENT_DYNAMIC_COLLECT()										\
	using dx_collection_t::operator[];										\
	dx_collection_t& AsFieldsCollection()				{ return *this; }	\
	const dx_collection_t& AsFieldsCollection() const	{ return *this; }

#define ESB_IMPLEMENT_DYNAMIC_COLLECT_F()								\
	ESB_IMPLEMENT_DYNAMIC_COLLECT()										\
	using dx_collection_t::GetFieldId;

#define ESB_IMPLEMENT_DYNAMIC_COLLECTION_RO()	\
	ESB_IMPLEMENT_DYNAMIC_COLLECT_F()			\
	using dx_collection_t::GetField;			\
	using dx_collection_t::Field;

#define ESB_IMPLEMENT_DYNAMIC_COLLECTION_RW()	\
	ESB_IMPLEMENT_DYNAMIC_COLLECT_F()			\
	using dx_collection_t::GetField;			\
	using dx_collection_t::SetField;			\
	using dx_collection_t::Field;


#define ESB_IMPLEMENT_DYNAMIC_COLLECT_N(NAME_)														\
	ESB_IMPLEMENT_DYNAMIC_COLLECT()																	\
	auto Get##NAME_##Id(const String& name_)	{ return dx_collection_t::GetFieldId(name_); }

#define ESB_IMPLEMENT_DYNAMIC_COLLECTION_RO_NAME(NAME_)												\
	ESB_IMPLEMENT_DYNAMIC_COLLECT_N(NAME_)															\
	dx_collection_t::value_t Get##NAME_(dispix_t id_) { return dx_collection_t::GetField(id_); }	\
	ESB_PROPERTY_ARRAY_RO(dx_collection_t::value_t, NAME_);

#define ESB_IMPLEMENT_DYNAMIC_COLLECTION_RW_NAME(NAME_)												\
	ESB_IMPLEMENT_DYNAMIC_COLLECT_N(NAME_)															\
	dx_collection_t::value_t Get##NAME_(dispix_t id_)							{ return dx_collection_t::GetField(id_); }			\
	auto Set##NAME_(dispix_t id_, ConstPara<dx_collection_t::value_t> value_)	{ return dx_collection_t::SetField(id_, value_); }	\
	ESB_PROPERTY_ARRAY_RW(dx_collection_t::value_t, NAME_);

	//
	// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
	// Динамическая коллекция свойств объектного интерфейса -----------------------------------------------------------------------------------------


 } // namespace esb
#endif // !__ESB_BASE_H__
