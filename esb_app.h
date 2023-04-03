#pragma once
#ifndef __ESB_APP_H__
#define __ESB_APP_H__

//нужно инициализировать идентификаторы платформы, com-идентификаторы и crt-идентификаторы и чтобы все это не конфликтовало
//для винды остановился на следущей комбинации
//сначала всегда и везде нужный assert (его хедер подключает все что нужно для с\с++ crt
#include <cassert>
#include <string>
#include <string_view>

// просто удобства препроцессора
#include "esb_pp.h"



// Файл esb_app_config.h не часть папки esb. Вы можете создать его в папке своего проекта и подключать до подключения esb.h 
// или в свойствах проекта указать как дополнтельную папку проекта .\ тогда этот файл будет найден и подключен автоматически
#if !defined(ESB_APP_CONFIG_H_) && __has_include("esb_app_config.h")
#include "esb_app_config.h"
#endif


//TOBE: Это блок настроек есб по-умолчанию. При разростании заслуживает вынесения в отдельный файл esb_config_def.h
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


#ifndef ESB_USE_RUSSIAN
#define ESB_USE_RUSSIAN	0
#endif


#if defined(ESB_USE_ISEQUALGUID_SSE) || defined(ESB_USE_ISEQUALGUID_64_2) || defined(ESB_USE_ISEQUALGUID_32_4)
//ok
#else
#define ESB_USE_ISEQUALGUID_64_2 1
#endif


#if defined(ESB_USE_DETOUR) && ESB_USE_DETOUR
#	ifdef ESB_USE_DETOUR_PATH
#		define ESB_DETOUR_INCLUDE	PP_STRINGIZE(  PP_CAT(ESB_USE_DETOUR_PATH , /include/detours.h ) )
#		define ESB_DETOUR_LIB		PP_STRINGIZE(  PP_CAT(ESB_USE_DETOUR_PATH , /lib.X86/detours.lib ) )
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

#define ESB_T(STR_)		L ## STR_


namespace esb {
	using refcounter_t = unsigned long;
	//INFO	В классической реализации dispatch интерфейса от MS DISPID - это int, но в этом есть некоторая неоднозначность для отрицательных значений
	//		Отрицательные DISPID могут показывать и invalid-dispid и magic-dispid (нр. DISPID_VALUE, etc.)
	//		Но я пока не сталкивался у 1С с magic-dispid, а постоянная оценка валидности, неоднозначности и преобразовании к unsigned при обращении
	//		к массивам с dispid-информацией и при вызовах методов очень неудобна.
	//		Поэтому наш dispid - это int и он может быть не-валидным, если отрицателен, а если валиден то преобразуется во всегда валидный dispix
	//		(диспатч-индекс). 
	//TOBE	В процессе int/dispid_t/dispix_t/. Кое где может быть не до конца подчищено. Возможно нужно переназвать to_dispid/to_dispix.
	//		Возможно нужно ужесточить до class enum или struct.
	using dispid_t = int;
	using dispix_t = unsigned;
	inline constexpr dispid_t	as_dispid(dispix_t ix)			{ return static_cast<dispid_t>(ix); }
	inline constexpr dispix_t	as_dispix(dispid_t id)			{ return static_cast<dispix_t>(id); }
	inline constexpr bool		is_valid_dispid(dispid_t id)	{ return (id >= 0); }
	inline constexpr bool		is_valid_dispid_index(dispid_t id, size_t size_) { 
		return (is_valid_dispid(id) && as_dispix(id) < size_); 
	}
	constexpr dispid_t DISPID_NOTFOUND = -1;


	//TOBE	Заготовка. По всему коду масса wstring. Нужно рефакторить если идти дальше.
	using strchar_t = wchar_t;
	using string_t = std::wstring;
	using string_view_t = std::wstring_view;
	using strview_t = string_view_t;

	//esbhlp
	ESB_NORETURN inline void				throw_error(const strview_t& error_text_);
}



#ifdef _LIB
// в режиме .lib -->
#ifdef NDEBUG		// _LIB - release
#ifdef ESB_RELEASE_BUILD
// полностью release-build - без ассертов вообще. требует дополнительно макроса ESB_RELEASE_BUILD
#define ESB_ASSERT(expression)
#else
// обычный release-build - это assertion-build - ассерты отбрасываются на handler компоненты
extern ESB_NORETURN void esb_assert_handler(unsigned line_);
#define ESB_ASSERT(expression) (void)( (!!(expression)) || (esb_assert_handler((unsigned)(__LINE__)), 0)  )
#endif
#else				// _LIB - debug (фактически assertion-build). ESB_ASSERT отбрасывается на esb_assert_handler, просто assert нету
extern ESB_NORETURN void esb_assert_handler(unsigned line_);
#define ESB_ASSERT(expression) (void)( (!!(expression)) || (esb_assert_handler((unsigned)(__LINE__)), 0)  )
// также в debug-build мы отклбчаем с++ assert
#undef assert
#define assert(xpr)
#endif
#else
// для не _LIB все ассерты - это ассерты
#define ESB_ASSERT(X_)					assert(X_)
#endif	//#ifdef _LIB


//TODO: Надо как-то по другому т.к. "TEXT_"==0 бросает варнинг "операция с адресом текстовой константы"
#define ESB_VERIFY(COND_, TEXT_)		ESB_ASSERT( ((COND_) || TEXT_==0) )
#define ESB_ASSERT_INDEX(IX_, CNT_)		ESB_ASSERT( (IX_) >= 0 && (IX_) < (CNT_) )




//оператор назначения неявно определен как удаленный
#define ESB_WARN_NO_OPERATOR_ASSIGN_COPY	4626
//оператор назначения перемещением неявно определен как удаленный
#define ESB_WARN_NO_OPERATOR_ASSIGN_MOVE	5027
//оператор назначения неявно определен как удаленный
//оператор назначения перемещением неявно определен как удаленный
#define ESB_WARN_NO_OPERATOR_ASSIGN_ANY		ESB_WARN_NO_OPERATOR_ASSIGN_COPY  ESB_WARN_NO_OPERATOR_ASSIGN_MOVE
//класс содержит виртуальные функции, но его ТРИВИАЛЬНЫЙ деструктор не является виртуальным; деструктор для экземпляров производных от этого класса объектов может работать некорректно.
#define ESB_WARN_NO_VIRTUAL_TRIVIAL_DTOR	5204
//класс содержит виртуальные функции, но его НЕТРИВИАЛЬНЫЙ деструктор не является виртуальным; деструктор экземпляров этого класса может работать некорректно.
#define ESB_WARN_NO_VIRTUAL_NONTRIVIAL_DTOR	4265
#define ESB_WARN_NO_VIRTUAL_ANY_DTOR		ESB_WARN_NO_VIRTUAL_TRIVIAL_DTOR  ESB_WARN_NO_VIRTUAL_NONTRIVIAL_DTOR
//конструктор по умолчанию неявно определен как удаленный
#define ESB_WARN_NO_DEFAULT_CTOR			4623
//конструктор копий неявно определен как удаленный
#define ESB_WARN_NO_CTOR_COPY				4625
//конструктор перемещения неявно определен как удаленный
#define ESB_WARN_NO_CTOR_MOVE				5026
//esb::DispInfoStatMeth: "3"-байтовые поля добавлены после данные-член "esb::DispInfoStatMeth::has_retval_"
#define ESB_WARN_PADDING					4820
//относительный путь поиска включаемых файлов содержит ".." (для esbhlp так и должно быть)
#ifndef ESB_WARN_INCLUDE_RELATIVE
#define ESB_WARN_INCLUDE_RELATIVE			4464
#endif // !ESB_WARN_INCLUDE_RELATIVE
//this: использован в списке инициализации базового класса
#define ESB_WARN_THIS_FOR_INIT_BASE			4355
//5246: "std::array<DispInfoT,4>::_Elems": инициализация подобъекта должна быть заключена в фигурные скобки
//ТУДУ: это ошибочная реакция на массив в студии 16.... в 17.2 вроде исправлено. здесь приходится давить.
//https://stackoverflow.com/questions/70127146/warning-c5246-the-initialization-of-a-subobject-should-be-wrapped-in-braces
#define ESB_WARN_INIT_WITH_BRACES			5246
// cl некорректно дает ворнинг: warning C4384: директиву #pragma "detect_mismatch" следует использовать только в глобальной области
// МС "Under Investigation" этой иссуи. Давим.
// https://developercommunity.visualstudio.com/t/incorrect-warning-using-pramga-detect-mismatch-aft/1320137
#define ESB_WARN_DETECT_MISMATCH_IN_SCOPE	4384



#define ESB_WARNING_SUPRESS( WARN_ )		\
	__pragma(warning(push))					\
	__pragma(warning( disable : WARN_ ))	

#define ESB_WARNING_SUPRESS_NO_VIRTUAL_DTOR_ANY()	\
	ESB_WARNING_SUPRESS( ESB_WARN_NO_VIRTUAL_TRIVIAL_DTOR ESB_WARN_NO_VIRTUAL_NONTRIVIAL_DTOR)

#define ESB_WARNING_SUPRESS_IMPLICIT_ASSIGN_DELETED_WHEN_REF_MEMBER()	\
	ESB_WARNING_SUPRESS( ESB_WARN_NO_OPERATOR_ASSIGN_ANY )

#define ESB_WARNING_RESTORE()	\
	__pragma(warning(pop))



#define ESB_DECLARE_NOCOPYMOVE(T_)						\
	T_(const T_&) = delete;								\
	T_(T_&&) = delete;									\
	T_& operator=(const T_&) = delete;					\
	T_& operator=(T_&&) = delete

#define ESB_DECLARE_NOCOPYMOVE_DTORDEFAULT(T_)			\
	ESB_DECLARE_NOCOPYMOVE(T_);							\
	~T_() = default		

#define ESB_DECLARE_NOCOPYMOVE_DTORDEFAULT_NOCTOR(T_)	\
	ESB_DECLARE_NOCOPYMOVE_DTORDEFAULT(T_);				\
	T_() = delete




#define ESB_PRAGMA_DETECT_MISMATCH(TAG_, VAL_)	\
	__pragma(detect_mismatch(TAG_, VAL_))

#define ESB_DETECT_MISMATCH(TAG_)				\
	ESB_PRAGMA_DETECT_MISMATCH(#TAG_, PP_STRINGIZE(TAG_))

#define ESB_CHECK_AND_DETECT_MISMATCH(VAL_, CHECK_)			\
	static_assert(VAL_ == (CHECK_), #VAL_);					\
	ESB_PRAGMA_DETECT_MISMATCH(#VAL_, PP_STRINGIZE(VAL_))


//INFO	Поскольку код использующий esb линкуется с библиотекой esbhlp мы полагаемся на определенные правила бинарной совместимости
//		нам нужно быть уверенными что и компилируемый код и esbhlp бинарно совместимы. поэтому помещаем в obj-файл проверку на совместимость (detect_mismatch)
// 
// Значение задается просто цифрой, т.к. прагме detect_mismatch нужно дать значение в виде строки, а макрос она разворачивать не хочет
// (может как-то можно заставить, но решил не утруждаться -	"меньше кода - меньше глюков")
#if UINTPTR_MAX == UINT32_MAX
#	define ESB_POINTER_SIZE	4
#elif UINTPTR_MAX == UINT64_MAX
#	define ESB_POINTER_SIZE	8
#else
#	error unexpected UINTPTR_MAX undefined or have wrong value.
#endif

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


#endif	//__ESB_APP_H__