#pragma once

// ни от чего не зависим. просто прагмы


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
// warning C5105: Поведение расширения макроса, создающего "defined", не определено
// эта "функциональность" используется у МС в C:\Program Files\Windows Kits\10\Include\10.0.19041.0\um\winbase.h(9531,5)
// ворнинг срабатывает в режиме "совместимого" препроцессора (видимо для MS-традиционного препроцессора это нормально)
// на компиляцию вроде не влияет. будем давить
#define ESB_WARN_DEFINED_IN_MACRO			5105


#define ESB_WARNING_SUPRESS( WARN_ )		\
	__pragma(warning(push))					\
	__pragma(warning( disable : WARN_ ))	

#define ESB_WARNING_RESTORE()				\
	__pragma(warning(pop))

#define ESB_WARNING_SUPRESS_NO_VIRTUAL_DTOR_ANY()						\
	ESB_WARNING_SUPRESS( ESB_WARN_NO_VIRTUAL_TRIVIAL_DTOR ESB_WARN_NO_VIRTUAL_NONTRIVIAL_DTOR)

#define ESB_WARNING_SUPRESS_IMPLICIT_ASSIGN_DELETED_WHEN_REF_MEMBER()	\
	ESB_WARNING_SUPRESS( ESB_WARN_NO_OPERATOR_ASSIGN_ANY )



#define ESB_PRAGMA_DETECT_MISMATCH(TAG_, VAL_)	\
	__pragma(detect_mismatch(TAG_, VAL_))


// требует уже заключенную в кавычки строку
#define ESB_PRAGMA_LINKER(LINKER_OPTION_STRING_)		\
	__pragma( comment(linker, LINKER_OPTION_STRING_) )
