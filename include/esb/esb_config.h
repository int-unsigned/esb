/*
	"ESB" is designed to create external native components for the 1C(tm) program.
	Copyright © 2023 IntUnsigned	(v8classes@gmail.com)
	"ESB" is free software under LGPLv2.1 license with essential restriction about binary part (view esb-license.txt)
	"ESB" IS DISTRIBUTED "AS-IS" WITHOUT ANY, EVEN THE IMPLIED, WARRANTY. IN NO EVENT SHALL THE AUTHORS OR THE COPYRIGHT OWNER BE LIABLE FOR ANY CLAIM, 
	DAMAGES OR OTHER LIABILITY ARISING OUT OF THE USE OF THIS SOFTWARE.
	THE AUTHOR DOES NOT GIVE ANY GUARANTEES AND DOES NOT BEAR ANY RESPONSIBILITY REGARDING THE USE OF 1C(tm) COMPONENTS PRODUCED USING THE ESB LIBRARY IN THE 1C(tm) ENVIRONMENT.
*/


#pragma once
#ifndef ESB_CONFIG_H_
#define ESB_CONFIG_H_


// "базовое" конфигурирование. используется и в _dispinfo. включает попытку загрузки пользовательского "esb_app_config.h"
#include "esb_config_base.h"
//
#include "esb_pragmas.h"


//нужно инициализировать идентификаторы платформы, com-идентификаторы и crt-идентификаторы и чтобы все это не конфликтовало
//для винды остановился на следущей комбинации
//сначала всегда и везде нужный assert (его хедер подключает все что нужно для с\с++ crt
#include <cassert>
#include <string>
#include <string_view>

#define _SYS_GUID_OPERATORS_	// no InlineIsEqualGUID, IsEqualGUID, guid==guid, ets.. Only GUID definition
#include "guiddef.h"


// используемый "макроязык" требует совместимого препроцессора (посчитал тянуть две версии препроцессора нецелесообразным)
#if !defined(_MSVC_TRADITIONAL) || _MSVC_TRADITIONAL
#	error esb requires c++ standart compaitable preprocessor (/Zc:preprocessor).
#endif
// просто удобства препроцессора
#include "esb/afx/afx_pp.h"


//INFO	Поскольку код использующий esb линкуется с библиотекой esbhlp мы полагаемся на определенные правила бинарной совместимости
//		нам нужно быть уверенными что и компилируемый код и esbhlp бинарно совместимы. поэтому помещаем в obj-файл проверку на совместимость (detect_mismatch)
// 
// Значение задается просто цифрой, т.к. прагме detect_mismatch нужно дать значение в виде строки, а макрос она разворачивать не хочет
// (может как-то можно заставить, но решил не утруждаться -	"меньше кода - меньше глюков")
#if UINTPTR_MAX == UINT32_MAX
#	define ESB_x32
#	define ESB_POINTER_SIZE		4
#	define ESB_POINTER_SIZE2	8
#	define ESB_PLATFORM_NAME	w32
#elif UINTPTR_MAX == UINT64_MAX
#	define ESB_x64
#	define ESB_POINTER_SIZE		8
#	define ESB_POINTER_SIZE2	16
#	define ESB_PLATFORM_NAME	w64
#else
#	error unexpected UINTPTR_MAX undefined or have wrong value.
#endif


#ifdef NDEBUG
#	define ESB_CONFIGURATION_NAME Release
#else
#	define ESB_CONFIGURATION_NAME Debug
#endif



// Файл esb_app_config.h не часть папки esb. Вы можете создать его в папке своего проекта и подключать до подключения esb.h 
// или в свойствах проекта указать как дополнтельную папку проекта $(MSBuildProjectDirectory) тогда этот файл будет найден и подключен автоматически
//#if !defined(ESB_APP_CONFIG_H_) && __has_include("esb_app_config.h")
//#include "esb_app_config.h"
//#endif
// ^^^^^^^^^^^^^^^^^^^^^^^^		в грузится esb_config_base.h


// если никакие языки не определяли, то будем англискими
#if !(defined(ESB_USE_ENGLISH) || defined(ESB_USE_RUSSION))
#	define ESB_USE_ENGLISH		1
#endif
#if !(ESB_USE_ENGLISH || ESB_USE_RUSSION)
#	error One of languige must be set!
#endif


#define ESB_VER_v8300	8300
#define ESB_VER_v8309	8309
#define ESB_VER_v8311	8311
#define ESB_VER_v8315	8315	//no ABI changed
#define ESB_VER_v8318	8318	//no ABI changed
#define ESB_VER_v8320	8320
#define ESB_VER_v8323	8323

#if defined(ESB_VER)
#	if ESB_VER == ESB_VER_v8309 || ESB_VER == ESB_VER_v8311 || ESB_VER == ESB_VER_v8320 || ESB_VER == ESB_VER_v8323
		// assume ok
#	else
#		error Unknown ESB_VER!!!
#	endif
#else
//latest
//#	define ESB_VER	ESB_VER_v8323
//#	define ESB_VER	ESB_VER_v8320
//#	define ESB_VER	ESB_VER_v8318
//#	define ESB_VER	ESB_VER_v8315
//#	define ESB_VER	ESB_VER_v8311
#	define ESB_VER	ESB_VER_v8309
#endif


#define ESB_VER_NAME					PP_CAT(v, ESB_VER)

#define ESB_ESBHLPLIB_PATH_SUFFIX		PP_STRINGIZE(ESB_VER_NAME) PP_STRINGIZE(ESB_CONFIGURATION_NAME) "/" PP_STRINGIZE(ESB_PLATFORM_NAME)
#define ESB_ESBHLPLIB_NAME				esbhlp.lib
#define ESB_ESBHLPLIB_PATH_NAME			ESB_ESBHLPLIB_PATH_SUFFIX "/" PP_STRINGIZE(ESB_ESBHLPLIB_NAME)

//NOTE	Все что есть ..PATH.. должно иметь в конце '/'
//		без / это как-бы имя папки, а "путь" всегда в конце с /
#define ESB_VER_FOLDER_RELATIVE			PP_PATH(1c, ESB_VER_NAME)

// мы полагаем, что в путях проекта настроен путь "include" (или иначе) в котором есть наше esb/	(и мы там лежим)
// (или указано ESB_PATH_ROOT где мы находимся)
#if !defined(ESB_PATH_ROOT_FOLDER)
#	define ESB_PATH_ROOT_FOLDER esb
#endif

#if !__has_include(   PP_STRINGIZE(PP_PATH(ESB_PATH_ROOT_FOLDER, esb_config.h))  )
#	error esb library path not configured properly!
#endif

#define ESB_VER_FOLDER					PP_PATH(ESB_PATH_ROOT_FOLDER, ESB_VER_FOLDER_RELATIVE)
#define ESB_INCLUDE_1C_MODULE(FILE_)	PP_PATH_STRING( ESB_VER_FOLDER, FILE_ )

#define ESB_VER_DISPINFO_FOLDER			ESB_VER_FOLDER_RELATIVE
#define ESB_VER_DISPINFO_BASE_H			PP_PATH_STRING(ESB_VER_DISPINFO_FOLDER, esdisp_base.h)
#define ESB_VER_DISPINFO_ALL_H			PP_PATH_STRING(ESB_VER_DISPINFO_FOLDER, esdisp_all.h)



//TOBE: Это блок настроек есб по-умолчанию. При разростании заслуживает вынесения в отдельный файл esb_config_def.h
//

// использование с++ "свойств"
#ifndef ESB_USE_PROPERTIES
#	define ESB_USE_PROPERTIES	1
#endif

#if defined(ESB_USE_EXPLICIT_STRING) && ESB_USE_EXPLICIT_STRING
#define ESB_EXPLICIT_STRING		explicit
#else
#define ESB_EXPLICIT_STRING	
#endif


#if defined(ESB_USE_EXPLICIT_NUMERIC) && ESB_USE_EXPLICIT_NUMERIC
#define ESB_EXPLICIT_NUMERIC		explicit
#else
#define ESB_EXPLICIT_NUMERIC	
#endif


#ifndef ESB_USE_AUTO_CONVERTION_VALUE_TO_CPP_TYPE
#define ESB_USE_AUTO_CONVERTION_VALUE_TO_CPP_TYPE 1
#endif


// Поиск индекса свойства объекта устроен так (вернее их строка), что полностью корректно это только через String, но String всегда аллоцирует новую строку
// в мем-пуле 1С. Это досаднейшая особенность чреватая и исключениями и просадкой производительности.
// В реализации метода _Find от wstring_view сделана попытка эмулировать String без лишней аллокации. Работает. Вроде. Но всякое может быть. Тогда придется отключить.
#ifndef ESB_USE_OBJECT_DISPATCH_FIND_STRVIEW_HACK
#define ESB_USE_OBJECT_DISPATCH_FIND_STRVIEW_HACK	1
#endif


// Экспериментальный прямой доступ к содержимому BinaryDataBuffer минуя диспатч интерфейс. Только для для v8300. (см.esb_es_file.h::BinaryDataBuffer)
// Точно работает в 8.3.9.1818 х86, 8.3.10.2772 х64, 8.3.11.2831 х86
// Точно не работает в 8.3.23.1688
// Видимо изменения были где-то между 8.3.11.2831 и 8.3.23.1688, но когда точно - неизвестно. Нужно экспериментировать и проверять.
#if defined(ESB_USE_BYTEBUFFER_DIRECT_HACK_v8300) && ESB_USE_BYTEBUFFER_DIRECT_HACK_v8300 && ESB_VER < ESB_VER_v8320
#	define ESB_USE_BYTEBUFFER_DIRECT_HACK	1
#else
#	define ESB_USE_BYTEBUFFER_DIRECT_HACK	0
#endif

#ifndef ESB_USE_RUSSIAN
#define ESB_USE_RUSSIAN	0
#endif

#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
#	define ESB_USING_RU(TYPE_RU_, TYPE_EN_)		using TYPE_RU_ = TYPE_EN_;
#else
#	define ESB_USING_RU(TYPE_RU_, TYPE_EN_)
#endif

#if defined(ESB_USE_ISEQUALGUID_SSE) || defined(ESB_USE_ISEQUALGUID_64_2) || defined(ESB_USE_ISEQUALGUID_32_4)
//ok
#else
#define ESB_USE_ISEQUALGUID_64_2 1
#endif


#if defined(ESB_USE_DETOUR) && ESB_USE_DETOUR
#	ifdef ESB_USE_DETOUR_PATH
#		define ESB_DETOUR_INCLUDE	PP_STRINGIZE(  PP_PATH(ESB_USE_DETOUR_PATH , include/detours.h ) )
#		if defined(ESB_x32)
#			define ESB_DETOUR_LIB		PP_STRINGIZE(  PP_PATH(ESB_USE_DETOUR_PATH , lib.X86/detours.lib ) )
#		elif defined(ESB_x64)
#			define ESB_DETOUR_LIB		PP_STRINGIZE(  PP_PATH(ESB_USE_DETOUR_PATH , /lib.X64/detours.lib)  )
#		endif
#		if !__has_include(   ESB_DETOUR_INCLUDE  )
#			error ms-detours not found in path
#		endif
#	else	//nodef	ESB_USE_DETOUR_PATH
#		error for using detours your mast define valid ESB_USE_DETOUR_PATH or disable ESB_USE_DETOUR 
#	endif	//ifdef ESB_USE_DETOUR_PATH
#endif
// end esb_config_default ----------------------------


// Строить итераторы на основе "голого" указателя на коллекцию или на основе InterfacePtr
// в случае InterfacePtr итераторы (и объекты-ссылки), которые они возвращают становятся самостоятельными объектаим и удерживают коллекцию в памяти
// это медленнее, может быть черевато циклическим ссылками, но не будет "висящих ссылок"
// На основе "голого" указателя - это с++way, быстрее, но можно попасть на висящую ссылку, когда коллекция уже умерла, а итератор еще жив.
//TOBE:	Имплементировано только для IxCollectionIterator`s.	Прочие имплементации сами по себе нуждаются в осмыслении и доработке
#ifndef ESB_USE_OWNING_COLLECTION_ITERATOR
#define ESB_USE_OWNING_COLLECTION_ITERATOR		0
#endif



//----------------------------
#define ESB_NORETURN	__declspec(noreturn)
#define ESB_NODISCARD	[[nodiscard]]
#define ESB_UNUSED		[[maybe_unused]]



namespace esb 
{
	using refcounter_t = unsigned long;
	//INFO	В классической реализации dispatch интерфейса от MS DISPID - это int, но в этом есть некоторая неоднозначность для отрицательных значений
	//		Отрицательные DISPID могут показывать и invalid-dispid и magic-dispid (нр. DISPID_VALUE, etc.)
	//		Но я пока не сталкивался у 1С с magic-dispid, а постоянная оценка валидности, неоднозначности и преобразовании к unsigned при обращении
	//		к массивам с dispid-информацией и при вызовах методов очень неудобна.
	//		Поэтому наш dispid - это int и он может быть не-валидным, если отрицателен, а если валиден то преобразуется во всегда валидный dispix
	//		(диспатч-индекс). 
	//TOBE	В процессе int/dispid_t/dispix_t/. Кое где может быть не до конца подчищено. Возможно нужно переназвать to_dispid/to_dispix.
	//		Возможно нужно ужесточить до class enum или struct.
	using isize_t = unsigned int;
	using ihash_t = intptr_t;
	using ssize_t = ptrdiff_t;

	using dispid_t = int;
	// using dispix_t = unsigned int;	перенесено выше в esb_config_base.h
	constexpr dispid_t DISPID_NOTFOUND = -1;
	constexpr dispix_t DISPIX_NOTFOUND = dispix_t(DISPID_NOTFOUND);

	inline constexpr dispid_t	as_dispid(dispix_t ix)			{ return static_cast<dispid_t>(ix); }
	inline constexpr dispix_t	as_dispix(dispid_t id)			{ return static_cast<dispix_t>(id); }
	inline constexpr bool		is_valid_dispid(dispid_t id)	{ return (id >= 0); }
	inline constexpr bool		is_valid_dispid_index(dispid_t id, size_t size_) { 
		return (is_valid_dispid(id) && as_dispix(id) < size_); 
	}
	

#if ESB_VER < ESB_VER_v8311
	using strchar_t = wchar_t;
	using string_t = std::wstring;
	using string_view_t = std::wstring_view;
	using strview_t = string_view_t;
#	define ESB_T(XT_)	L ## XT_

#	define ESB_DECL_INTEGRAL_TO_USTRING(VAL_T)		\
		ESB_NODISCARD inline string_t to_ustring(VAL_T val_)		{ return std::to_wstring(val_); }

	ESB_NODISCARD inline string_t to_ustring(double val_)		{ return std::to_wstring(val_); }
	ESB_NODISCARD inline string_t to_ustring(float val_)		{ return std::to_wstring(val_); }
	ESB_NODISCARD inline string_t to_ustring(long double val_)	{ return std::to_wstring(val_); }
#else
//#elif ESB_VER < ESB_VER_v8320
	using strchar_t = char16_t;
	using string_t = std::basic_string<strchar_t>;
	using string_view_t = std::basic_string_view<strchar_t>;
	using strview_t = string_view_t;
#	define ESB_T(XT_)	u ## XT_

	//TOBE:	нет (пока) в с++ std методов to_u16string поэтому мы пока привязываемся к МС-интернал-реализации. Будут проблемы - будем решать..
#	define ESB_DECL_INTEGRAL_TO_USTRING(VAL_T)		\
		ESB_NODISCARD inline string_t to_ustring(VAL_T val_)	{	return std::_Integral_to_string<strchar_t>(val_); }

	//TODO: а для double и т.п. вообще затычка! нету для них _Integral_to_string - они не-интеграл! переделать!
	ESB_NODISCARD inline string_t to_ustring(double val_)		{ // convert double to wstring
		std::wstring s = std::to_wstring(val_);
		return string_t{ (const char16_t*) s.data(), s.length() };
	}
	ESB_NODISCARD inline string_t to_ustring(float val_)		{ return to_ustring(static_cast<double>(val_));}
	ESB_NODISCARD inline string_t to_ustring(long double val_)	{ return to_ustring(static_cast<double>(val_));	}
//#else
//#	error ESB_VER_xxx not properly defined!
#endif

	ESB_DECL_INTEGRAL_TO_USTRING(int)
	ESB_DECL_INTEGRAL_TO_USTRING(unsigned int)
	ESB_DECL_INTEGRAL_TO_USTRING(long)
	ESB_DECL_INTEGRAL_TO_USTRING(unsigned long)
	ESB_DECL_INTEGRAL_TO_USTRING(long long)
	ESB_DECL_INTEGRAL_TO_USTRING(unsigned long long)
#undef ESB_DECL_INTEGRAL_TO_USTRING


	//esbhlp
	ESB_NORETURN inline void throw_error(const strview_t& error_text_);

}



//TODO: пробовать убрать это все и перейти к string_view
// 
// как аргумент шаблона допускается только struct: error C2993: "esb::FixedString": не является допустимым типом для параметра шаблона "TA", не являющегося типом.
// message : "ptr_" не является открытым, неизменяемым, нестатическим элементом данных.
// clang 14, 15 error: pointer to subobject of string literal is not allowed in a template argument
// mscl 19.29.30146 (vc 16.11.19) - OK, but IDE sow error E2645: недопустимый нетипизированный аргумент шаблона
//	
class FixedString {
	const esb::strchar_t*	m_ptr;
	size_t					m_len;
public:
	template<std::size_t N>
	constexpr FixedString(const esb::strchar_t(&arr_)[N]) : m_ptr(arr_), m_len(N - 1)
	{}
	constexpr FixedString(const esb::strchar_t* ptr_, size_t len_) : m_ptr{ ptr_ }, m_len{ len_ }
	{}
	constexpr size_t len() const { return m_len; }
	//string view -ish
	constexpr size_t				size() const { return m_len; }
	constexpr const esb::strchar_t*	data() const { return m_ptr; }
};


struct FixedTerm {
	FixedString m_string[2];
public:
	constexpr FixedTerm(const FixedString& s1_, const FixedString& s2_) : m_string{ s1_, s2_ }
	{}
	template<size_t NEn, size_t NRu>
	constexpr FixedTerm(const esb::strchar_t(&str1_)[NEn], const esb::strchar_t(&str2_)[NRu]) : m_string{ str1_, str2_ }
	{}
	constexpr size_t size() const {
		return 2;
	}
	constexpr bool index_ok(int index_) const {
		return (index_ >= 0 && (unsigned)index_ < size());
	}
	constexpr const FixedString& operator[](int index_) const {
		assert(index_ok(index_));
		return m_string[index_];
	}
	constexpr const FixedString* at(int index_) const {
		return (index_ok(index_)) ? &m_string[index_] : nullptr;
	}
	constexpr const esb::strchar_t* cstr_at(int index_) const {
		return (index_ok(index_)) ? m_string[index_].data() : nullptr;
	}
};


namespace esb {
	using UniqueTypeId = CLSID;
	using UniqueTypeTerm = FixedTerm;
}




#ifdef _LIB
// в режиме .lib -->
#	ifdef NDEBUG		// _LIB - release
#		ifdef ESB_RELEASE_ASSERTION_BUILD
			// assertion-build в релизе - ассерты отбрасываются на handler компоненты
			extern ESB_NORETURN void esb_assert_handler(unsigned line_);
#			define ESB_ASSERT(expression) (void)( (!!(expression)) || (esb_assert_handler((unsigned)(__LINE__)), 0)  )
#		else
			// полностью release-build - без ассертов вообще. требует дополнительно макроса ESB_RELEASE_BUILD
#			define ESB_ASSERT(expression)
#		endif
#	else				// _LIB - debug (фактически assertion-build). ESB_ASSERT отбрасывается на esb_assert_handler, просто assert нету
#		if 0
			extern ESB_NORETURN void esb_assert_handler(unsigned line_, const char*);
#			define ESB_ASSERT(expression) (void)( (!!(expression)) || (esb_assert_handler((unsigned)(__LINE__), __FILE__), 0)  )
#		else
			extern ESB_NORETURN void esb_assert_handler(unsigned line_);
#			define ESB_ASSERT(expression) (void)( (!!(expression)) || (esb_assert_handler((unsigned)(__LINE__)), 0)  )
#		endif
		// также в debug-build мы отключаем с++ assert
		// (ПОМНИ! <assert.h> не имеет отзащиты! если его включить после нас он опять включится!)
#		undef assert
#		define assert(xpr)	((void)0)
#	endif
#else
// для не _LIB все ассерты - это ассерты
#	define ESB_ASSERT(X_)					assert(X_)
#endif	//#ifdef _LIB


//TODO: Надо как-то по другому т.к. "TEXT_"==0 бросает варнинг "операция с адресом текстовой константы"
#define ESB_VERIFY(COND_, TEXT_)		ESB_ASSERT( ((COND_) || TEXT_==0) )
#define ESB_ASSERT_INDEX(IX_, CNT_)		ESB_ASSERT( (IX_) >= 0 && (IX_) < (CNT_) )






#define ESB_DECLARE_NOCOPYMOVE(T_)						\
	T_(const T_&) = delete;								\
	T_(T_&&) = delete;									\
	T_& operator=(const T_&) = delete;					\
	T_& operator=(T_&&) = delete

#define ESB_DECLARE_NOCOPYMOVE_NOCTOR(T_)				\
	ESB_DECLARE_NOCOPYMOVE(T_);							\
	T_() = delete

//TODO	Вот это вот _DTORDEFAULT малополезная штука т.к. если перенести макрос в приват, то и деструктор становится приват, что нам не нужно.
#define ESB_DECLARE_NOCOPYMOVE_DTORDEFAULT(T_)			\
	ESB_DECLARE_NOCOPYMOVE(T_);							\
	~T_() = default		

#define ESB_DECLARE_NOCOPYMOVE_DTORDEFAULT_NOCTOR(T_)	\
	ESB_DECLARE_NOCOPYMOVE_DTORDEFAULT(T_);				\
	T_() = delete



#define ESB_DETECT_MISMATCH(TAG_)							\
	ESB_PRAGMA_DETECT_MISMATCH(#TAG_, PP_STRINGIZE(TAG_))

#define ESB_CHECK_AND_DETECT_MISMATCH(VAL_, CHECK_)			\
	static_assert(VAL_ == (CHECK_), #VAL_);					\
	ESB_PRAGMA_DETECT_MISMATCH(#VAL_, PP_STRINGIZE(VAL_))

//
ESB_CHECK_AND_DETECT_MISMATCH(ESB_POINTER_SIZE, (sizeof(void*)))



#ifndef ESB_USE_ITERATOR_DEBUGGING
#ifdef NDEBUG
#define ESB_USE_ITERATOR_DEBUGGING 0
#else
#define ESB_USE_ITERATOR_DEBUGGING 1
#endif
#endif


#if defined(ESB_USE_ITERATOR_DEBUGGING) && ESB_USE_ITERATOR_DEBUGGING
#define ESB_USE_ITERATOR_DEBUG_LEVEL 1
#else
#define ESB_USE_ITERATOR_DEBUG_LEVEL 0
#endif


#endif	//ESB_CONFIG_H_