/*
	"ESB" is designed to create external native components for the 1C(tm) program.
	Copyright © 2023 IntUnsigned	(v8classes@gmail.com)
	"ESB" is free software under LGPLv2.1 license with essential restriction about binary part (view esb-license.txt)
	"ESB" IS DISTRIBUTED "AS-IS" WITHOUT ANY, EVEN THE IMPLIED, WARRANTY. 
	IN NO EVENT SHALL THE AUTHORS OR THE COPYRIGHT OWNER BE LIABLE FOR ANY CLAIM,
	DAMAGES OR OTHER LIABILITY ARISING OUT OF THE USE OF THIS SOFTWARE.
	THE AUTHOR DOES NOT GIVE ANY GUARANTEES AND DOES NOT BEAR ANY RESPONSIBILITY REGARDING THE USE OF 1C(tm) COMPONENTS PRODUCED USING 
	THE ESB LIBRARY IN THE 1C(tm) ENVIRONMENT.
*/


#pragma once
#ifndef AFX_USTRING_H_
#define AFX_USTRING_H_

#include <cassert>

namespace afx_ustring {

	// СТРОКОВОЕ. НЕЗАВИСИМОЕ. НЕ ТРЕБУЕТ ИНСЛУДОВ. используется в consteval
	//----------------------------------------------------------------------------------------------------------------------
	//difference between two letters is bit 5: if bit 5 is set, we are dealing with a lower case letter, if this bit is not set we are dealing with an uppercase letter....
	// LETTER	DEC		BINARY VALUE
	//	G		71		0100 0111
	//	g		103		0110 0111

#ifdef _MSC_VER
	template<class T>
	inline constexpr bool is_uchar_type = (std::is_same_v<T, wchar_t> || std::is_same_v<T, char16_t>);
#else
	inline constexpr bool is_uchar_type = (std::is_same_v<T, char16_t>);
#endif

	template<class T>
	concept UCharConcept = is_uchar_type<T>;

	template<class UStrIteratorType>
	inline constexpr bool is_ustring_raw_pointer = (std::is_pointer_v<UStrIteratorType> && is_uchar_type<std::remove_cv_t<std::remove_pointer_t<UStrIteratorType>>>);





	template<class UCharType>
	constexpr unsigned short uchar16_value(UCharType uch_) {
		static_assert(is_uchar_type<UCharType>, "not a UCharType");
		return static_cast<unsigned short>(uch_);
	}


	constexpr inline char to_locase_en(char ch_) { return (ch_ >= 'A' && ch_ <= 'Z') ? (ch_ | 0b00100000) : ch_; }
	constexpr inline char to_upcase_en(char ch_) { return (ch_ >= 'a' && ch_ <= 'z') ? (ch_ & 0b11011111) : ch_; }
	static_assert(to_upcase_en(to_locase_en('A')) == 'A', "!!!");




	template<class UCharType>
	constexpr inline UCharType to_upcase_en(UCharType wch_) {
		static_assert(is_uchar_type<UCharType>, "not a UCharType");
		return (wch_ >= 'a' && wch_ <= 'z') ? static_cast<UCharType>(wch_ & 0b11011111) : wch_;
	}
	template<class UCharType>
	constexpr inline UCharType to_locase_en(UCharType wch_) {
		static_assert(is_uchar_type<UCharType>, "not a UCharType");
		return (wch_ >= 'A' && wch_ <= 'Z') ? static_cast<UCharType>(wch_ | 0b00100000) : wch_;
	}
	static_assert(to_upcase_en(to_locase_en(L'A')) == L'A', "!!!");



	//А	CYRILLIC CAPITAL LETTER A (U+0410)	feff0410
	//constexpr int xru_Ё = L'Ё';		//0100 0000.0001
	//constexpr int xru_ё = L'ё';		//
	////...
	//constexpr int xru_A = L'А';		//0100 0001.0000
	//constexpr int xru_a = L'а';		//0100 0011.0000
	//
	//constexpr int xru_Я = L'Я';		//0100 0010.1111
	//constexpr int xru_я = L'я';		//0100 0100.1111
	//constexpr int xru_shift = L'а' - L'А';

	namespace letters_val {
		inline constexpr unsigned short ru_а = u'а';
		inline constexpr unsigned short ru_А = u'А';
		inline constexpr unsigned short ru_я = u'я';
		inline constexpr unsigned short ru_Я = u'Я';
		inline constexpr unsigned short ru_ё = u'ё';
		inline constexpr unsigned short ru_Ё = u'Ё';
	}

	template<class UCharType>
	constexpr inline UCharType to_upcase_ru(UCharType wch_) {
		static_assert(is_uchar_type<UCharType>, "not a UCharType");
		return (wch_ == letters_val::ru_ё) ? static_cast<UCharType>(letters_val::ru_Ё) :
			(wch_ >= letters_val::ru_а && wch_ <= letters_val::ru_я)? 
				static_cast<UCharType>(wch_ - static_cast<UCharType>(letters_val::ru_а - letters_val::ru_А)) :
				wch_;
	}
	template<class UCharType>
	constexpr inline UCharType to_locase_ru(UCharType wch_) {
		static_assert(is_uchar_type<UCharType>, "not a UCharType");
		return (wch_ == letters_val::ru_Ё)? static_cast<UCharType>(letters_val::ru_ё) :
			(wch_ >= letters_val::ru_А && wch_ <= letters_val::ru_Я)? 
				static_cast<UCharType>(wch_ + static_cast<UCharType>(letters_val::ru_а - letters_val::ru_А)) :
				wch_;
	}
	static_assert(to_upcase_ru(to_locase_ru(L'Ж')) == L'Ж', "!!!");
	static_assert(to_upcase_ru(to_locase_ru(u'ж')) == u'Ж', "!!!");


	template<class UCharType>
	constexpr inline UCharType to_upcase_enru(UCharType wch_) {
		static_assert(is_uchar_type<UCharType>, "not a UCharType");
		return (wch_ >= 'a' && wch_ <= 'z') ? static_cast<UCharType>(wch_ & 0b11011111) : to_upcase_ru(wch_);
	}
	template<class UCharType>
	constexpr inline UCharType to_locase_enru(UCharType wch_) {
		static_assert(is_uchar_type<UCharType>, "not a UCharType");
		return (wch_ >= 'A' && wch_ <= 'Z') ? /*-->*/ static_cast<UCharType>(wch_ | 0b00100000) : to_locase_ru(wch_);
	}
	static_assert(to_upcase_enru(to_locase_enru(L'Ж')) == L'Ж', "!!!");


	template<class UCharType>
	constexpr inline UCharType to_nocase(UCharType wch_) {
		return to_upcase_enru(wch_);
	}
	//----------------------------------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------------------------------



	//TOBE: нужно переделать через requers проверяющий что на входе UChType и на выходе UChType, но не обязательно одинаковые!
	template<UCharConcept UChType>
	using UCharConverterType = UChType(UChType);

	template<class T>
	inline constexpr bool is_uchar_converter = false;
	template<class UChType>
	inline constexpr bool is_uchar_converter<UCharConverterType<UChType>> = true;


	template<UCharConcept UChType>
	class CStrIteratorBase {
	protected:
		const UChType* m_psz;
		constexpr CStrIteratorBase(const UChType* psz_) : m_psz(psz_)
		{
			assert(m_psz);
		}
		template<size_t NSize>
		constexpr CStrIteratorBase(const UChType(&asz_)[NSize]) : m_psz(&asz_)
		{}
	public:
		constexpr bool operator==(const CStrIteratorBase& other_) const {
			return (m_psz == other_.m_psz);
		}
		constexpr bool operator==(const UChType* other_psz_) const {
			return (m_psz == other_psz_);
		}
	};

	template<UCharConcept UChType, auto UCharConverterPtr = nullptr>
	class CStrIterator : public CStrIteratorBase<UChType> {
		static_assert(is_uchar_converter< std::remove_pointer_t<decltype(UCharConverterPtr)> >, "UChar16ConverterPtr is not converter func.");
	public:
		constexpr CStrIterator(const UChType* psz_) : CStrIteratorBase<UChType>(psz_)
		{}
		template<size_t NSize>
		constexpr CStrIterator(const UChType(&asz_)[NSize]) : CStrIteratorBase<UChType>(asz_)
		{}
		constexpr CStrIterator& operator++()	{ ++this->m_psz;	return *this; }
		constexpr auto operator*() const		{ return UCharConverterPtr(*this->m_psz); }	//UCharConverterPtr не обязательно возвращает тот же тип, что и получает
	};

	template<UCharConcept UChType>
	class CStrIterator<UChType, nullptr> : public CStrIteratorBase<UChType> {
	public:
		constexpr CStrIterator(const UChType* psz_) : CStrIteratorBase<UChType>(psz_)
		{}
		template<size_t NSize>
		constexpr CStrIterator(const UChType(&asz_)[NSize]) : CStrIteratorBase<UChType>(asz_)
		{}
		constexpr CStrIterator& operator++()	{ ++this->m_psz;	return *this; }
		constexpr auto operator*() const		{ return *this->m_psz; }
	};


	template<UCharConcept UChType>
	struct CStrIteratorNoCase : public CStrIterator<UChType, to_nocase<UChType>> {
		constexpr CStrIteratorNoCase(const UChType* psz_) : CStrIterator<UChType, to_nocase<UChType>>(psz_)
		{}
		template<size_t NSize>
		constexpr CStrIteratorNoCase(const UChType(&asz_)[NSize]) : CStrIterator<UChType, to_nocase<UChType>>(asz_)
		{}
	};







	template<class UStrIteratorType1, class UStrIteratorType2 = UStrIteratorType1>
	constexpr bool cstr_equal(UStrIteratorType1 sit1_, UStrIteratorType2 sit2_) {
		if constexpr (is_ustring_raw_pointer<UStrIteratorType1>)
			assert(sit1_);
		if constexpr (is_ustring_raw_pointer<UStrIteratorType2>)
			assert(sit2_);

		// раверство указателей означает равенство строк. в т.ч. xchar_t* можно сравнивать с итератором на нем построенном.
		if (sit1_ == sit2_)
			return true;
		// иначе сравниваем
		auto			wch1 = uchar16_value(*sit1_);
		auto			wch2 = uchar16_value(*sit2_);
		while (wch1 == wch2) {
			if (wch2 == 0)
				return true;	// в теле цикла мы пока символы строк равны. значит если *pch1 == 0, то и *pch2 == 0. значит строки закончились и равны
			else {
				++sit1_;
				++sit2_;
				wch1 = uchar16_value(*sit1_);
				wch2 = uchar16_value(*sit2_);
			}
		}
		// сюда мы попадаем только если строки стали не равны. в том числе если одна из строк закончилась символом 0, а вторая еще не закончилась.
		return false;
	}


	template<class UStrIteratorType1, class UStrIteratorType2 = UStrIteratorType1>
	constexpr int cstr_compare(UStrIteratorType1 sit1_, UStrIteratorType2 sit2_) {
		if constexpr (is_ustring_raw_pointer<UStrIteratorType1>)
			assert(sit1_);
		if constexpr (is_ustring_raw_pointer<UStrIteratorType2>)
			assert(sit2_);

		// раверство указателей означает равенство строк. в т.ч. xchar_t* можно сравнивать с итератором на нем построенном.
		if constexpr (std::is_same_v<UStrIteratorType1, UStrIteratorType2>) {
			if (sit1_ == sit2_)
				return 0;
		}

		// иначе сравниваем
		auto wch1 = uchar16_value(*sit1_);
		auto wch2 = uchar16_value(*sit2_);
		while (wch1 == wch2) {
			if (wch2 == 0)
				return 0;	// в теле цикла мы пока символы строк равны. значит если *pch1 == 0, то и *pch2 == 0. значит строки закончились и равны
			else {
				++sit1_;
				++sit2_;
				wch1 = uchar16_value(*sit1_);
				wch2 = uchar16_value(*sit2_);
			}
		}
		// сюда мы попадаем только если строки стали не равны. в том числе если одна из строк закончилась символом 0, а вторая еще не закончилась.
		// учитывая что символ 0 это минимальное значение символа дастаточно просто сравнить эти символы. 
		// если *pch1 < *pch2, то -1 (первая строка меньше) : иначе 1 (первая строка больше); 0 (равенство здесь быть не может по условию цикла)
		return (wch1 < wch2) ? -1 : 1;
	}

}	//namespace afx_ustring


#endif	//AFX_USTRING_H_