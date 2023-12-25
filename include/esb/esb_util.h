/*
	"ESB" is designed to create external native components for the 1C(tm) program.
	Copyright © 2023 IntUnsigned	(v8classes@gmail.com)
	"ESB" is free software under LGPLv2.1 license with essential restriction about binary part (view esb-license.txt)
	"ESB" IS DISTRIBUTED "AS-IS" WITHOUT ANY, EVEN THE IMPLIED, WARRANTY. IN NO EVENT SHALL THE AUTHORS OR THE COPYRIGHT OWNER BE LIABLE FOR ANY CLAIM, 
	DAMAGES OR OTHER LIABILITY ARISING OUT OF THE USE OF THIS SOFTWARE.
	THE AUTHOR DOES NOT GIVE ANY GUARANTEES AND DOES NOT BEAR ANY RESPONSIBILITY REGARDING THE USE OF 1C(tm) COMPONENTS PRODUCED USING THE ESB LIBRARY IN THE 1C(tm) ENVIRONMENT.
*/


#pragma once
#ifndef ESBUTIL_H
#define ESBUTIL_H

#include <type_traits>
#include <array>
//
#include "afx/afx_pp.h"


// ------------------------------------------------------------------------------------------------------------------------------------------------------------
//


// реализация через deprecated требует отключения /sdl у компилятора (в debug-build включен по-умолчанию)
// т.к. в режиме /sdl warning-depricated трактуется как ошибка компиляции
// Обе реализации ничего не вносят ни в код, ни в структуру классов - только типы
// 
// работает в name-space, class-space, function-space, 
// не работает при разворачивании template - при инстациировании шаблона инстанциируется только необходимое, а не это...
#define STATIC_WARNING_F(EXPR_, MSG_)									\
	struct PP_CAT(warning_,__LINE__) {									\
	  [[deprecated(MSG_)]]	void _(std::false_type const&) {}			\
							void _(std::true_type const& ) {}			\
	  PP_CAT(warning_,__LINE__)() {_(std::bool_constant<(EXPR_)>());}	\
	}

// работает в name-space, class-space, при разворачивании template, не работает в function-space - вообще не компилируется - в функциях запрещены шаблоны
// если не зависит от параметров шаблона, то срабатывает всегда - и при объявлении шаблона и при инстанциировании
#define STATIC_WARNING_T(EXPR_, MSG_)												\
	template<bool> struct PP_CAT(warning_,__LINE__) {};								\
	template<> struct [[deprecated(MSG_)]] PP_CAT(warning_, __LINE__) <false> {};	\
	using PP_CAT(warning__,__LINE__)  = PP_CAT(warning_,__LINE__)<(EXPR_)>

// при такой декларации работать будет везде кроме функций, а в функциях будет ошибка компиляции и нужно будет добавить _F
#define STATIC_WARNING(EXPR_, MSG_)	 STATIC_WARNING_T(EXPR_, MSG_)
// ------------------------------------------------------------------------------------------------------------------------------------------------------------



// ни пуя не работает!
//template <bool cond_v>//, typename Then, typename OrElse>
//constexpr decltype(auto) constexpr_iif(auto cond_, auto then_, auto or_else_) {
//	if constexpr (static_cast<bool>(cond_)) {
//		return std::forward<decltype(then_)>(then_);
//	}
//	else {
//		return std::forward<decltype(or_else_)>(or_else_);
//	}
//}

#define CONSTEXPR_IIF(COND_, THEN_, ELSE_)	\
	if constexpr (COND_)	\
		THEN_;				\
	else 					\
		ELSE_




constexpr bool is_power_of_2(size_t value_) {
	return value_ != 0 && (value_ & (value_ - 1)) == 0;
}
constexpr size_t round_up(size_t size_, size_t align_) {
	assert(is_power_of_2(align_));
	return (size_ + size_t(align_ - 1)) & ~(size_t(align_ - 1));
}
constexpr size_t round_up_4(size_t size_) {
	return round_up(size_, 4);
}
static_assert(round_up(7, 4) == 8);

inline consteval bool is_aligned_as(uintptr_t addr_, size_t align_) {
	assert(is_power_of_2(align_));
	return (addr_!=0) && ((addr_ & (align_ - 1)) == 0);
}







namespace esb {

	template<typename>
	inline constexpr bool always_false_v = false;

	template <class TypeT, class... TypesListT>
	inline constexpr bool is_type_in_list = std::disjunction_v<std::is_same<TypeT, TypesListT>...>;

	template<class... Types>
	struct typepack;
	template <>
	struct typepack<> {
		struct size : std::integral_constant<size_t, 0>{};
	};
	template<class ThisT, class... RestT>
	class typepack<ThisT, RestT...>
	{
		template<size_t IndexN, class... Ts>						struct _item;
		template<size_t IndexN, class TsHead, class... TsTail>		struct _item<IndexN, TsHead, TsTail...> : _item<IndexN - 1, TsTail...> {};
		template<class TsHead, class... TsTail>						struct _item<0, TsHead, TsTail...> { using type = TsHead; };
		template<size_t IndexN>		struct _item<IndexN> { static_assert(always_false_v<std::integral_constant<size_t, IndexN>>, "typepack index out of bounds"); };
	public:
		template<typename Type>	
		struct contain : std::bool_constant<is_type_in_list<Type, ThisT, RestT... >> {};
		struct size : std::integral_constant<size_t, sizeof...(RestT)+1> {};
		template<size_t IndexN>
		using item_t = _item<IndexN, ThisT, RestT...>::type;
	};
	template<typename... Types>
	struct typepack<typepack<Types...>> : typepack<Types...> {
	};

	// typepack_unique ------------------------------------------------------
	// финализация рекурсии когда (typename...) - пусто. (т.к. если не пусто то должна отработать специализация ниже), 
	// а в T при этом собрается весь уникальный typepack
	template <typename UniqTypepackT, typename...>
	struct typepack_unique_impl {
		using type = UniqTypepackT;
	};

	//специализация когда пакет раскрывается так, что в X (test-unique) что-то попадает (ну и возможно в хвост Xs... пападает)
	template <typename... UniqTs, typename X, typename... Xs>
	struct typepack_unique_impl<typepack<UniqTs...>, X, Xs...>	// в typepack<UniqTs...> все UniqTs... уже уникальны (или пусто). X - то что проверяем сейчас. Xs... - хвост
		: std::conditional_t<(std::is_same_v<X, UniqTs> || ...)	// наследуемся от.. по условию что X(test-unique) один из типов которые в хвосте (... раскроется в s_same_v<X, UniqTs1> || s_same_v<X, UniqTs2> ...)
		, typepack_unique_impl<typepack<UniqTs...>, Xs...>		// если || выше истинно значит X не уникален и мы его "проглатываем" и углубляемся в хвост дальше
		, typepack_unique_impl<typepack<UniqTs..., X>, Xs...>>	// иначе, X уникальный и мы дописываем его к списку типов, которые уже в пакете (UniqTs...) и углубляемся дальше в остаток
	{};															// то есть в результате мы или наследуемся и наследуемся, пока в не упремся в хвостовую typepack_unique_impl
																// где ... пусто, а в T собрался весь уникальный typepack и он становится нашим полем type
	template <typename>
	struct typepack_unique;			//forward definition для структуры чтобы для нашего typepack сделать специализацию
									//В специализации мы "вынимаем" список типов из typepack в Ts...
	template <typename... Xs>		//И для начала вызываем процесс с пустым списком. По мере проверки уникальности он будет заполняться
	struct typepack_unique <typepack<Xs...>> : public typepack_unique_impl<typepack<>, Xs...> {};

	template <typename TypePackT>
	using typepack_unique_t = typename typepack_unique<TypePackT>::type;	//Если Tp это наш typepack, то уйдем на специализацию для него. иначе будет ош.
	//------------------------------------------------------------------------


	// это должно работать для всех tuple-like структур (** Честно содрал https://stackoverflow.com/a/62089731)
	// TOBE: нужно проверить
#if 0
	// end of recursive call: tuple is forwared using `type`
	template <typename T, typename... Ts>
	struct unique_impl { using type = T; };

	// recursive call: 1. Consumes the first type of the variadic arguments, 
	//                    if not repeated add it to the tuple.  
	//                 2. Call this again with the rest of arguments
	template <template<class...> class Tuple, typename... Ts, typename U, typename... Us>
	struct unique_impl<Tuple<Ts...>, U, Us...>
		: std::conditional_t<(std::is_same_v<U, Ts> || ...), unique_impl<Tuple<Ts...>, Us...>, unique_impl<Tuple<Ts..., U>, Us...>>
	{};

	// forward definition
	template <class Tuple>
	struct unique_tuple;

	// class specialization so that tuple arguments can be extracted from type
	template <template<class...>class Tuple, typename... Ts>
	struct unique_tuple<Tuple<Ts...>> : public unique_impl<Tuple<>, Ts...> {};

	template <class Tuple>
	using unique_tuple_t = typename unique_tuple<Tuple>::type;
#endif
}


template<typename>
struct template_argument_type;
template<class T>
using template_argument_type_t = template_argument_type<T>::type;


// Класс представляет собой базу для одной ноды списка функций (используется в списках инит\терм). В связи с тем, что он база мы не можем
// здесь решить может ли нода содержать nullptr, может ли нода быть копируемой\перемещаемой, т.п.
// Для списка инит, например, копия означает что какой-то инит будет вызван два раза, что скорее ошибка.
// А список терм, обычно, строится так чтобы два раза ресурсы не освобождать и два раза терм скорее тихо пройдет (хотя и, возможно, это излишне)
// Ввиду всего этого мы, как базовый класс, будем хранить указатель, а не ссылку на метод, но свои конструкторы будем держать protected 
// пусть наследник решает все исходя из своей логики.
// для того чтобы была возможность иметь разные списки мы специализируемся собственно классом списка
template<class StaticMethListT, typename StaticMethT>
class StaticMethListBase {
protected:
	using me_t = StaticMethListT;
	using meth_t = std::remove_cvref_t<StaticMethT>;
	//static const me_t*	root_;		из-за warning C4356: "static data member cannot be initialized via derived class" root_ нужно объявлять в StaticMethListT
	meth_t*				meth_;
	const me_t*			next_;
protected:
	StaticMethListBase(nullptr_t) : meth_(nullptr), next_(StaticMethListT::root_) {
		StaticMethListT::root_ = static_cast<me_t*>(this);
	}
	StaticMethListBase(meth_t& m_) : meth_(m_), next_(StaticMethListT::root_) {
		StaticMethListT::root_ = static_cast<me_t*>(this);
	}
};


// Утилитка для вызова метода экземпляра, как статик-метода
// (когда адрес экзампляра статический и известен на этапе компиляции)
// если в список нужно положить метод класса то можно сделать типа MemberMethInvoker<&instance_XFoo1, &XFoo::init>::invoke
//TODO	надо к invoke шаблонные параметры добавить для универсальности
template<auto InstPtr, auto Meth>
struct MemberMethInvoker {
	using inst_t = std::remove_pointer_t<decltype(InstPtr)>;
	static constexpr inst_t& inst_ = *InstPtr;
	using meth_t = decltype(Meth);
	static constexpr meth_t meth_ = Meth;
	static constexpr decltype(auto) invoke() {
		return (inst_.*meth_)();
	}
};


template<auto Meth>
struct VoidifyInvoker {
	static void invoke() { Meth(); }
};




#ifndef GUID_DEFINED
#define _SYS_GUID_OPERATORS_	// no InlineIsEqualGUID, IsEqualGUID, guid==guid, ets.. Only GUID definition
#include "guiddef.h"
#endif //ifndef GUID_DEFINED


//	Сравнение гуидов ССЕ отлично работает, но... не всегда в случае 1С. Алгоритм имеет смысл если ГУИД выровнен на границу 16 байт и так должно быть для
//	типа ГУИД. Но не в случае 1С. Изредка они подходят к этому вопросу "творчески". Поэтому esbhlp собрана с ESB_USE_ISEQUALGUID_64_2
//	ССЕ можно использовать для своих целей.
#if defined(ESB_USE_ISEQUALGUID_SSE) || defined(ESB_USE_SSE)
#include <emmintrin.h>
#endif

constexpr bool is_equal_guid_asis(REFGUID g1, REFGUID g2) {
	return (g1.Data1 == g2.Data1 && g1.Data2 == g2.Data2 && g1.Data3 == g2.Data3 &&
		g1.Data4[0] == g2.Data4[0] &&
		g1.Data4[1] == g2.Data4[1] &&
		g1.Data4[2] == g2.Data4[2] &&
		g1.Data4[3] == g2.Data4[3] &&
		g1.Data4[4] == g2.Data4[4] &&
		g1.Data4[5] == g2.Data4[5] &&
		g1.Data4[6] == g2.Data4[6] &&
		g1.Data4[7] == g2.Data4[7]
		);
}

inline bool is_equal_guid_memcmp(REFGUID a_, REFGUID b_){
	return (std::memcmp(&a_, &b_, sizeof(GUID)) == 0);
}
inline bool is_equal_guid_InlineIsEqualGUID(REFGUID rguid1, REFGUID rguid2) {
	// from InlineIsEqualGUID
	return (
		((unsigned long*)&rguid1)[0] == ((unsigned long*)&rguid2)[0] &&
		((unsigned long*)&rguid1)[1] == ((unsigned long*)&rguid2)[1] &&
		((unsigned long*)&rguid1)[2] == ((unsigned long*)&rguid2)[2] &&
		((unsigned long*)&rguid1)[3] == ((unsigned long*)&rguid2)[3]);
}
inline bool is_equal_guid_64_2(REFGUID rguid1, REFGUID rguid2) {
	return (
		((uint64_t*)&rguid1)[0] == ((uint64_t*)&rguid2)[0] &&
		((uint64_t*)&rguid1)[1] == ((uint64_t*)&rguid2)[1]);
}

#if defined(ESB_USE_ISEQUALGUID_SSE) || defined(ESB_USE_SSE)
inline bool is_equal_guid_sse(REFGUID a_, REFGUID b_) {
	__m128i a = *(__m128i*) & a_;			//	movups
	__m128i b = *(__m128i*) & b_;			//	movups
	__m128i x = _mm_cmpeq_epi32(a, b);		//	if equal == 0xffffffff.0xffffffff.0xffffffff.0xffffffff
	int		r = _mm_movemask_epi8(x);		//	if equal == 0xffff
	return	(r == 0xFFFF);

	//~ code ~
	//movups  xmm0, xmmword ptr ds:_IID_IUnknown	movups - unaligned load (alined mnemonic movaps, also mov between xmm registers)
	//movups  xmm1, xmmword ptr [eax]
	//pcmpeqd xmm1, xmm0
	//pmovmskb eax, xmm1
	//cmp     eax, 0FFFFh
	//jz      short loc_AD
}
#endif


inline bool is_equal_guid(REFGUID a_, REFGUID b_)
{
#ifdef ESB_USE_ISEQUALGUID_SSE
	return is_equal_guid_sse(a_, b_);
#elif defined(ESB_USE_ISEQUALGUID_64_2)
	return is_equal_guid_64_2(a_, b_);
#elif defined(ESB_USE_ISEQUALGUID_32_4)
	// from InlineIsEqualGUID
	return is_equal_guid_InlineIsEqualGUID(a_, b_);
#else
	//produce cycle on 32-bit platform :(
	return is_equal_guid_memcmp(a_, b_);
#endif
}

#define ESB_DEFINE_GUID(TAG_, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8)   TAG_ = { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }



consteval uint8_t from_hex(const char ch_) {
	if ((ch_ >= 'a') && (ch_ <= 'f'))
		return static_cast<uint8_t>(ch_ - 87);
	else if ((ch_ >= 'A') && (ch_ <= 'F'))
		return static_cast<uint8_t>(ch_ - 55);
	else if ((ch_ >= '0') && (ch_ <= '9'))
		return static_cast<uint8_t>(ch_ - '0');
	else
		throw std::exception{};
}
template<size_t NChar>
consteval auto from_hex(const char* pch_);
template<>	consteval auto from_hex<1>(const char* pch_) { return from_hex(pch_[0]); }
template<>	consteval auto from_hex<2>(const char* pch_) { return uint8_t((from_hex<1>(pch_) << 4) | from_hex<1>(pch_ + 1)); }
template<>	consteval auto from_hex<4>(const char* pch_) { return uint16_t((from_hex<2>(pch_) << 8) | from_hex<2>(pch_ + 2)); }
template<>	consteval auto from_hex<8>(const char* pch_) { return uint32_t((from_hex<4>(pch_) << 16) | from_hex<4>(pch_ + 4)); }

static_assert(from_hex<8>("12345678") == 0x12345678, " from_hex failed");

// expected {18EBBE5A-080E-4E0A-B181-1C58F8545EFD}
template<size_t NSize>
consteval GUID guid_from_hex(const char(&guid_string_)[NSize]) {
	static_assert(NSize == 37, "expected guid string in registery format (example 18EBBE5A-080E-4E0A-B181-1C58F8545EFD) + zero-terminator-char");
	GUID g{};
	const char* pch = guid_string_;
	g.Data1		= from_hex<8>(pch);		pch += 8;	pch += 1;	//+ delimiter '-'
	g.Data2		= from_hex<4>(pch);		pch += 4;	pch += 1;
	g.Data3		= from_hex<4>(pch);		pch += 4;	pch += 1;
	g.Data4[0]	= from_hex<2>(pch);		pch += 2;
	g.Data4[1]	= from_hex<2>(pch);		pch += 2;	pch += 1;
	g.Data4[2]	= from_hex<2>(pch);		pch += 2;
	g.Data4[3]	= from_hex<2>(pch);		pch += 2;
	g.Data4[4]	= from_hex<2>(pch);		pch += 2;
	g.Data4[5]	= from_hex<2>(pch);		pch += 2;
	g.Data4[6]	= from_hex<2>(pch);		pch += 2;
	g.Data4[7]	= from_hex<2>(pch);		pch += 2;
	return g;
}

// {18EBBE5A-080E-4E0A-B181-1C58F8545EFD}
static_assert(is_equal_guid_asis(guid_from_hex("18EBBE5A-080E-4E0A-B181-1C58F8545EFD"), GUID{ 0x18ebbe5a, 0x80e, 0x4e0a, {0xb1, 0x81, 0x1c, 0x58, 0xf8, 0x54, 0x5e, 0xfd} }));



#if defined(ESB_USE_DETOUR) && ESB_USE_DETOUR
// В свойства проекта добавить пути к папке с ms-detours

#include ESB_DETOUR_INCLUDE
#pragma comment( lib , ESB_DETOUR_LIB )

// Класс упрощает установку перехвата для одной функции. В конструкторе перехват устанавливается, а в деструкторе синимается.
// В качестве метода-перегрузки может принимать лямбду
// Вызов реального (не перехваченного) метода осуществляется ::invoke_real
template<auto FnReal>
class DetourOne {
public:
	using fn_t = std::remove_pointer_t<decltype(FnReal)>;
	static_assert(std::is_pointer_v<decltype(FnReal)>&& std::is_function_v<fn_t>);
private:
	static inline fn_t* m_pfn_real = FnReal;
	fn_t*				m_pfn_over;
	LONG				m_lr;

	using detour_action_t = LONG __stdcall (PVOID*, PVOID);	// DetourAttach / DetourDetach
	static LONG detour_transaction(fn_t** ppfn_real_, fn_t* pfn_over_, detour_action_t& action_) {
		LONG lr = DetourTransactionBegin();
		if (lr == NO_ERROR) {
			DetourSetIgnoreTooSmall(FALSE);
			lr = action_((PVOID*)ppfn_real_, pfn_over_);
			if (lr == NO_ERROR)
				lr = DetourTransactionCommit();
			else
				DetourTransactionAbort();
		}
		return lr;
	}
public:
	DetourOne(fn_t* pfn_over_) : m_pfn_over(pfn_over_) {
		m_lr = detour_transaction(&m_pfn_real, m_pfn_over, DetourAttach);
	}
	explicit operator bool() const {
		return (m_lr == NO_ERROR);
	}
	template<typename... ArgsT>
	static decltype(auto) invoke_real(ArgsT ... args_) {
		return m_pfn_real(std::forward<ArgsT>(args_)...);
	}
	~DetourOne() {
		if (m_lr == NO_ERROR)
			detour_transaction(&m_pfn_real, m_pfn_over, DetourDetach);
	}
};
#endif	//defined(ESB_USE_DETOUR) && ESB_USE_DETOUR

#endif //ESBUTIL_H