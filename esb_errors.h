/*
	"ESB" is designed to create external native components for the 1C(tm) program.
	Copyright © 2023 IntUnsigned	(v8classes@gmail.com)
	"ESB" is free software under LGPLv2.1 license with essential restriction about binary part (view esb-license.txt)
	"ESB" IS DISTRIBUTED "AS-IS" WITHOUT ANY, EVEN THE IMPLIED, WARRANTY. IN NO EVENT SHALL THE AUTHORS OR THE COPYRIGHT OWNER BE LIABLE FOR ANY CLAIM, 
	DAMAGES OR OTHER LIABILITY ARISING OUT OF THE USE OF THIS SOFTWARE.
	THE AUTHOR DOES NOT GIVE ANY GUARANTEES AND DOES NOT BEAR ANY RESPONSIBILITY REGARDING THE USE OF 1C(tm) COMPONENTS PRODUCED USING THE ESB LIBRARY IN THE 1C(tm) ENVIRONMENT.
*/


#pragma once
#ifndef __ESB_ERRORS_H__
#define __ESB_ERRORS_H__

#include "esb_app.h"



//ТУДУ: Сообщения об ошибках вытянуты из прошлого компилятора (поэтому и ESL...).
//		Сделаны еще во времена 14-й студии и 4-го clang, потому и юникод. 
//		Одноязычные.
//		Косноязычные.
//		Малоинформативные (см.ESL_THROW_VALUE_TYPE_MISMATCH, ESL_THROW_TYPE_BYNAME_NOT_FOUND, т.п.)
//		Все переписать :).
//
// Не в namespace esb и дефайнами потому как не всегда в namespace esb испоьзуются.


//Ошибка:
// Иногда при отладке удобно перед ошибкой получить ассерт и свалиться в отладчик.
// НО НУЖНО УЧИТЫВАТЬ, что это отрабатывает глобально и в тех случаях когда ошибка делается специально для ее перехвата
#if defined(NDEBUG) || 1
#	define ES_THROW(ET_)						esb::throw_error(ESB_T("ЕСБ::") ET_)
#else
#	define ES_THROW(ET_)						assert(ET_==0),	esb::throw_error(ESB_T("ЕСБ::") ET_)
#endif

#define ESL_T(T_)							ESB_T(T_)
#define ESL_THROW(ET_)						ES_THROW(ET_)


//Неожиданное значение.
#define ESL_THROW_UNEXPECTED()				ESL_THROW(ESL_T("\u041D\u0435\u043E\u0436\u0438\u0434\u0430\u043D\u043D\u043E\u0435 \u0437\u043D\u0430\u0447\u0435\u043D\u0438\u0435."))
//Не реализовано yet...
#define ESL_THROW_NOTIMPL()					ESL_THROW(ESL_T("\u041D\u0435 \u0440\u0435\u0430\u043B\u0438\u0437\u043E\u0432\u0430\u043D\u043E yet..."))
//Не удалось получить значение числа."))
#define ESL_THROW_ERRNUMBER()				ESL_THROW(ESL_T("\u041D\u0435 \u0443\u0434\u0430\u043B\u043E\u0441\u044C \u043F\u043E\u043B\u0443\u0447\u0438\u0442\u044C \u0437\u043D\u0430\u0447\u0435\u043D\u0438\u0435 \u0447\u0438\u0441\u043B\u0430."))
//Преобразование к типу Число не может быть выполнено.
#define ESL_THROW_NOTNUMBER()				ESL_THROW(ESL_T("\u041F\u0440\u0435\u043E\u0431\u0440\u0430\u0437\u043E\u0432\u0430\u043D\u0438\u0435 \u043A \u0442\u0438\u043F\u0443 \u0427\u0438\u0441\u043B\u043E \u043D\u0435 \u043C\u043E\u0436\u0435\u0442 \u0431\u044B\u0442\u044C \u0432\u044B\u043F\u043E\u043B\u043D\u0435\u043D\u043E."))
//Не удалось получить значение строки."))
#define ESL_THROW_ERRSTRING()				ESL_THROW(ESL_T("\u041D\u0435 \u0443\u0434\u0430\u043B\u043E\u0441\u044C \u043F\u043E\u043B\u0443\u0447\u0438\u0442\u044C \u0437\u043D\u0430\u0447\u0435\u043D\u0438\u0435 \u0441\u0442\u0440\u043E\u043A\u0438."))
//Преобразование к типу Строка не может быть выполнено.
#define ESL_THROW_NOTSTRING()				ESL_THROW(ESL_T("\u041F\u0440\u0435\u043E\u0431\u0440\u0430\u0437\u043E\u0432\u0430\u043D\u0438\u0435 \u043A \u0442\u0438\u043F\u0443 \u0421\u0442\u0440\u043E\u043A\u0430 \u043D\u0435 \u043C\u043E\u0436\u0435\u0442 \u0431\u044B\u0442\u044C \u0432\u044B\u043F\u043E\u043B\u043D\u0435\u043D\u043E."))
//Не удалось получить значение даты.
#define ESL_THROW_ERRDATE()					ESL_THROW(ESL_T("\u041D\u0435 \u0443\u0434\u0430\u043B\u043E\u0441\u044C \u043F\u043E\u043B\u0443\u0447\u0438\u0442\u044C \u0437\u043D\u0430\u0447\u0435\u043D\u0438\u0435 \u0434\u0430\u0442\u044B."))
//Преобразование к типу Дата не может быть выполнено.
#define ESL_THROW_NOTDATE()					ESL_THROW(ESL_T("\u041F\u0440\u0435\u043E\u0431\u0440\u0430\u0437\u043E\u0432\u0430\u043D\u0438\u0435 \u043A \u0442\u0438\u043F\u0443 \u0414\u0430\u0442\u0430 \u043D\u0435 \u043C\u043E\u0436\u0435\u0442 \u0431\u044B\u0442\u044C \u0432\u044B\u043F\u043E\u043B\u043D\u0435\u043D\u043E."))
//Не удалось получить значение булево.
#define ESL_THROW_ERRBOOL()					ESL_THROW(ESL_T("\u041D\u0435 \u0443\u0434\u0430\u043B\u043E\u0441\u044C \u043F\u043E\u043B\u0443\u0447\u0438\u0442\u044C \u0437\u043D\u0430\u0447\u0435\u043D\u0438\u0435 \u0431\u0443\u043B\u0435\u0432\u043E."))
//Преобразование к типу Булево не может быть выполнено.
#define ESL_THROW_NOTBOOL()					ESL_THROW(ESL_T("\u041F\u0440\u0435\u043E\u0431\u0440\u0430\u0437\u043E\u0432\u0430\u043D\u0438\u0435 \u043A \u0442\u0438\u043F\u0443 \u0411\u0443\u043B\u0435\u0432\u043E \u043D\u0435 \u043C\u043E\u0436\u0435\u0442 \u0431\u044B\u0442\u044C \u0432\u044B\u043F\u043E\u043B\u043D\u0435\u043D\u043E."))	
//Операции сравнения на больше - меньше допустимы только для значений совпадающих примитивных типов(Булево, Число, Строка, Дата)
#define ESL_THROW_NOTCOMPARABLE()			ESL_THROW(ESL_T("\u041E\u043F\u0435\u0440\u0430\u0446\u0438\u0438 \u0441\u0440\u0430\u0432\u043D\u0435\u043D\u0438\u044F \u043D\u0430 \u0431\u043E\u043B\u044C\u0448\u0435 - \u043C\u0435\u043D\u044C\u0448\u0435 \u0434\u043E\u043F\u0443\u0441\u0442\u0438\u043C\u044B \u0442\u043E\u043B\u044C\u043A\u043E \u0434\u043B\u044F \u0437\u043D\u0430\u0447\u0435\u043D\u0438\u0439 \u0441\u043E\u0432\u043F\u0430\u0434\u0430\u044E\u0449\u0438\u0445 \u043F\u0440\u0438\u043C\u0438\u0442\u0438\u0432\u043D\u044B\u0445 \u0442\u0438\u043F\u043E\u0432(\u0411\u0443\u043B\u0435\u0432\u043E, \u0427\u0438\u0441\u043B\u043E, \u0421\u0442\u0440\u043E\u043A\u0430, \u0414\u0430\u0442\u0430)"))
//Получение элемента по индексу для значения не определено.
#define ESL_THROW_NOTINDICES()				ESL_THROW(ESL_T("\u041F\u043E\u043B\u0443\u0447\u0435\u043D\u0438\u0435 \u044D\u043B\u0435\u043C\u0435\u043D\u0442\u0430 \u043F\u043E \u0438\u043D\u0434\u0435\u043A\u0441\u0443 \u0434\u043B\u044F \u0437\u043D\u0430\u0447\u0435\u043D\u0438\u044F \u043D\u0435 \u043E\u043F\u0440\u0435\u0434\u0435\u043B\u0435\u043D\u043E."))
//Получение элемента по индексу для значения не определено.
#define ESL_THROW_NOTARRAYRO()				ESL_THROW(ESL_T("\u041F\u043E\u043B\u0443\u0447\u0435\u043D\u0438\u0435 \u044D\u043B\u0435\u043C\u0435\u043D\u0442\u0430 \u043F\u043E \u0438\u043D\u0434\u0435\u043A\u0441\u0443 \u0434\u043B\u044F \u0437\u043D\u0430\u0447\u0435\u043D\u0438\u044F \u043D\u0435 \u043E\u043F\u0440\u0435\u0434\u0435\u043B\u0435\u043D\u043E."))
//Изменение значения элемента по индексу для значения не определено.
#define ESL_THROW_NOTARRAYFS()				ESL_THROW(ESL_T("\u0418\u0437\u043C\u0435\u043D\u0435\u043D\u0438\u0435 \u0437\u043D\u0430\u0447\u0435\u043D\u0438\u044F \u044D\u043B\u0435\u043C\u0435\u043D\u0442\u0430 \u043F\u043E \u0438\u043D\u0434\u0435\u043A\u0441\u0443 \u0434\u043B\u044F \u0437\u043D\u0430\u0447\u0435\u043D\u0438\u044F \u043D\u0435 \u043E\u043F\u0440\u0435\u0434\u0435\u043B\u0435\u043D\u043E."))
//Получение элемента по ключу для значения не определено.
#define ESL_THROW_NOTCOLLECTRO()			ESL_THROW(ESL_T("\u041F\u043E\u043B\u0443\u0447\u0435\u043D\u0438\u0435 \u044D\u043B\u0435\u043C\u0435\u043D\u0442\u0430 \u043F\u043E \u043A\u043B\u044E\u0447\u0443 \u0434\u043B\u044F \u0437\u043D\u0430\u0447\u0435\u043D\u0438\u044F \u043D\u0435 \u043E\u043F\u0440\u0435\u0434\u0435\u043B\u0435\u043D\u043E."))
//Изменение значения элемента по ключу для значения не определено.
#define ESL_THROW_NOTCOLLECTFS()			ESL_THROW(ESL_T("\u0418\u0437\u043C\u0435\u043D\u0435\u043D\u0438\u0435 \u0437\u043D\u0430\u0447\u0435\u043D\u0438\u044F \u044D\u043B\u0435\u043C\u0435\u043D\u0442\u0430 \u043F\u043E \u043A\u043B\u044E\u0447\u0443 \u0434\u043B\u044F \u0437\u043D\u0430\u0447\u0435\u043D\u0438\u044F \u043D\u0435 \u043E\u043F\u0440\u0435\u0434\u0435\u043B\u0435\u043D\u043E."))
//Значение соответствующее ключу не задано.
#define ESL_THROW_NOTCOLLECT_VAL()			ESL_THROW(ESL_T("\u0417\u043D\u0430\u0447\u0435\u043D\u0438\u0435 \u0441\u043E\u043E\u0442\u0432\u0435\u0442\u0441\u0442\u0432\u0443\u044E\u0449\u0435\u0435 \u043A\u043B\u044E\u0447\u0443 \u043D\u0435 \u0437\u0430\u0434\u0430\u043D\u043E."))
//Итератор для значения не определен
#define ESL_THROW_NOENUM()					ESL_THROW(ESL_T("\u0418\u0442\u0435\u0440\u0430\u0442\u043E\u0440 \u0434\u043B\u044F \u0437\u043D\u0430\u0447\u0435\u043D\u0438\u044F \u043D\u0435 \u043E\u043F\u0440\u0435\u0434\u0435\u043B\u0435\u043D."))
//ТУДУ: имя поля?
//Поле объекта не обнаружено.
#define ESL_THROW_OBJECT_PROP_NOT_FOUND()	ESL_THROW(ESL_T("\u041F\u043E\u043B\u0435 \u043E\u0431\u044A\u0435\u043A\u0442\u0430 \u043D\u0435 \u043E\u0431\u043D\u0430\u0440\u0443\u0436\u0435\u043D\u043E."))
//ТУДУ: имя метода?
//Метод объекта не обнаружен.
#define ESL_THROW_OBJECT_METH_NOT_FOUND()	ESL_THROW(ESL_T("\u041C\u0435\u0442\u043E\u0434 \u043E\u0431\u044A\u0435\u043A\u0442\u0430 \u043D\u0435 \u043E\u0431\u043D\u0430\u0440\u0443\u0436\u0435\u043D."))
//Значение не является значением объектного типа.
#define ESL_THROW_NOTCTX()					ESL_THROW(ESL_T("\u0417\u043D\u0430\u0447\u0435\u043D\u0438\u0435 \u043D\u0435 \u044F\u0432\u043B\u044F\u0435\u0442\u0441\u044F \u0437\u043D\u0430\u0447\u0435\u043D\u0438\u0435\u043C \u043E\u0431\u044A\u0435\u043A\u0442\u043D\u043E\u0433\u043E \u0442\u0438\u043F\u0430."))
//Значение свойства объекта не получено.
#define ESL_THROW_NOT_OBJECT_PROP_GET()		ESL_THROW(ESL_T("\u0417\u043D\u0430\u0447\u0435\u043D\u0438\u0435 \u0441\u0432\u043E\u0439\u0441\u0442\u0432\u0430 \u043E\u0431\u044A\u0435\u043A\u0442\u0430 \u043D\u0435 \u043F\u043E\u043B\u0443\u0447\u0435\u043D\u043E."))
//Значение свойству объекта не присвоено.
#define ESL_THROW_NOT_OBJECT_PROP_SET()		ESL_THROW(ESL_T("\u0417\u043D\u0430\u0447\u0435\u043D\u0438\u0435 \u0441\u0432\u043E\u0439\u0441\u0442\u0432\u0443 \u043E\u0431\u044A\u0435\u043A\u0442\u0430 \u043D\u0435 \u043F\u0440\u0438\u0441\u0432\u043E\u0435\u043D\u043E."))
//UNEXPECTED? вызов известных методов для известных объектов всегда должен быть успешен
//Вызов функции объекта не выполнен.
#define ESL_THROW_CALL_OBJECT_FUNC_FAILED()	ESL_THROW(ESL_T("\u0412\u044B\u0437\u043E\u0432 \u0444\u0443\u043D\u043A\u0446\u0438\u0438 \u043E\u0431\u044A\u0435\u043A\u0442\u0430 \u043D\u0435 \u0432\u044B\u043F\u043E\u043B\u043D\u0435\u043D."))
//Вызов процедуры объекта не выполнен.
#define ESL_THROW_CALL_OBJECT_PROC_FAILED()	ESL_THROW(ESL_T("\u0412\u044B\u0437\u043E\u0432 \u043F\u0440\u043E\u0446\u0435\u0434\u0443\u0440\u044B \u043E\u0431\u044A\u0435\u043A\u0442\u0430 \u043D\u0435 \u0432\u044B\u043F\u043E\u043B\u043D\u0435\u043D."))
//Вызов процедуры объекта как функции.
#define ESL_THROW_NOTCTXRETVAL()			ESL_THROW(ESL_T("\u0412\u044B\u0437\u043E\u0432 \u043F\u0440\u043E\u0446\u0435\u0434\u0443\u0440\u044B \u043E\u0431\u044A\u0435\u043A\u0442\u0430 \u043A\u0430\u043A \u0444\u0443\u043D\u043A\u0446\u0438\u0438."))
//Несоответствие количества аргументов при вызове метода объекта.
#define ESL_THROW_NOTCTXPARACOUNT()			ESL_THROW(ESL_T("\u041D\u0435\u0441\u043E\u043E\u0442\u0432\u0435\u0442\u0441\u0442\u0432\u0438\u0435 \u043A\u043E\u043B\u0438\u0447\u0435\u0441\u0442\u0432\u0430 \u0430\u0440\u0433\u0443\u043C\u0435\u043D\u0442\u043E\u0432 \u043F\u0440\u0438 \u0432\u044B\u0437\u043E\u0432\u0435 \u043C\u0435\u0442\u043E\u0434\u0430 \u043E\u0431\u044A\u0435\u043A\u0442\u0430."))
//Значение по-умолчанию для аргумента не получено.
#define ESL_THROW_NOTCTXDEFVAL()			ESL_THROW(ESL_T("\u0417\u043D\u0430\u0447\u0435\u043D\u0438\u0435 \u043F\u043E-\u0443\u043C\u043E\u043B\u0447\u0430\u043D\u0438\u044E \u0434\u043B\u044F \u0430\u0440\u0433\u0443\u043C\u0435\u043D\u0442\u0430 \u043D\u0435 \u043F\u043E\u043B\u0443\u0447\u0435\u043D\u043E."))
//Значение свойства объекта не доступно для чтения.
#define ESL_THROW_NOTCTXPROPGET_DENY()		ESL_THROW(ESL_T("\u0417\u043D\u0430\u0447\u0435\u043D\u0438\u0435 \u0441\u0432\u043E\u0439\u0441\u0442\u0432\u0430 \u043E\u0431\u044A\u0435\u043A\u0442\u0430 \u043D\u0435 \u0434\u043E\u0441\u0442\u0443\u043F\u043D\u043E \u0434\u043B\u044F \u0447\u0442\u0435\u043D\u0438\u044F."))
//Значение свойства объекта не доступно для изменения.
#define ESL_THROW_NOTCTXPROPSET_DENY()		ESL_THROW(ESL_T("\u0417\u043D\u0430\u0447\u0435\u043D\u0438\u0435 \u0441\u0432\u043E\u0439\u0441\u0442\u0432\u0430 \u043E\u0431\u044A\u0435\u043A\u0442\u0430 \u043D\u0435 \u0434\u043E\u0441\u0442\u0443\u043F\u043D\u043E \u0434\u043B\u044F \u0438\u0437\u043C\u0435\u043D\u0435\u043D\u0438\u044F."))
//Индекс находится за пределами границ массива.
#define ESL_THROW_ARRAY_INDEX_ERR()			ESL_THROW(ESL_T("\u0418\u043D\u0434\u0435\u043A\u0441 \u043D\u0430\u0445\u043E\u0434\u0438\u0442\u0441\u044F \u0437\u0430 \u043F\u0440\u0435\u0434\u0435\u043B\u0430\u043C\u0438 \u0433\u0440\u0430\u043D\u0438\u0446 \u043C\u0430\u0441\u0441\u0438\u0432\u0430."))
//Ошибочное количество параметров конструктора объекта.
#define ESL_THROW_CTOR_PARA_COUNT()			ESL_THROW(ESL_T("\u041E\u0448\u0438\u0431\u043E\u0447\u043D\u043E\u0435 \u043A\u043E\u043B\u0438\u0447\u0435\u0441\u0442\u0432\u043E \u043F\u0430\u0440\u0430\u043C\u0435\u0442\u0440\u043E\u0432 \u043A\u043E\u043D\u0441\u0442\u0440\u0443\u043A\u0442\u043E\u0440\u0430 \u043E\u0431\u044A\u0435\u043A\u0442\u0430."))
//Ошибка выделения памяти
#define ESL_THROW_BAD_ALLOC()				ESL_THROW(ESL_T("\u041e\u0448\u0438\u0431\u043a\u0430\u0020\u0432\u044b\u0434\u0435\u043b\u0435\u043d\u0438\u044f\u0020\u043f\u0430\u043c\u044f\u0442\u0438."))
//Попытка выделения памяти нулевого размера
#define ESL_THROW_NIL_ALLOC()				ESL_THROW(ESL_T("\u041f\u043e\u043f\u044b\u0442\u043a\u0430\u0020\u0432\u044b\u0434\u0435\u043b\u0435\u043d\u0438\u044f\u0020\u043f\u0430\u043c\u044f\u0442\u0438\u0020\u043d\u0443\u043b\u0435\u0432\u043e\u0433\u043e\u0020\u0440\u0430\u0437\u043c\u0435\u0440\u0430."))
//Деление на ноль
#define ESL_THROW_DIV_BY_ZERO()				ESL_THROW(ESL_T("\u0414\u0435\u043b\u0435\u043d\u0438\u0435\u0020\u043d\u0430\u0020\u043d\u043e\u043b\u044c."))
//
#define ESL_THROW_UNEXPECTED_QUERY_INTERFACE_RESULT()		ESL_THROW(ESL_T("Неожиданный результат при попытке запроса интерфейса объекта."))
//
#define ESL_THROW_UNEXPECTED_QUERY_INTERFACE_FAILED()		ESL_THROW(ESL_T("Запрос интерфейса объекта не выполнен."))
//ТУДУ: Неплохо бы сказать какого типа значение требуется.. 
//Ожидается значение другого типа.
#define ESL_THROW_VALUE_TYPE_MISMATCH()						ESL_THROW(ESL_T("Ожидается значение другого типа."))
//начение для номера месяца года должно быть числом от 1 до 12. 
#define ESL_THROW_MONTH_VALUE_INVALID()						ESL_THROW(ESL_T("Значение для номера месяца года должно быть числом от 1 до 12."))
//Попытка изменить значение константного аргумента.
#define ESL_THROW_UNEXPECTED_VARIABLE_SET_VALUE()			ESL_THROW(ESL_T("Попытка изменить значение константного аргумента."))
//
#define ESL_THROW_TYPE_BYNAME_NOT_FOUND()					ESL_THROW(ESL_T("Тип <по-имени> не найден."))
// 
#define ESL_THROW_EXPECTED_UNSIGNED()						ESL_THROW(ESL_T("Ожидается не-отрицательное числовое значение."))
//
#define ESL_THROW_NUMERIC_NOT_INT_OR_INTMAX_OVERFLOW()		ESL_THROW(ESL_T("Числовое значение не целое или превосходит по величине максимальное Целое."))
//
#define ESL_THROW_NUMERIC_LONG_OVERFLOW()					ESL_THROW(ESL_T("Числовое значение превосходит по величине тип long."))
//
#define ESL_THROW_NUMERIC_SIZET_OVERFLOW()					ESL_THROW(ESL_T("Числовое значение превосходит по величине тип size_t."))

#define ESL_THROW_INTEGER_OVERFLOW(INTEGER_DT_)				ESL_THROW(ESL_T("Числовое значение превосходит по величине тип " #INTEGER_DT_ "." ))

namespace esb {
	template<class IntegerT>
	ESB_NORETURN inline void throw_error_integer_overflow();
#define ESB_THROW_INTEGER_OVERFLOW_IMPL(INTEGER_DT_)	\
	template<> \
	ESB_NORETURN inline void throw_error_integer_overflow<INTEGER_DT_>()	{ \
		ESL_THROW_INTEGER_OVERFLOW(INTEGER_DT_); \
	}

	ESB_THROW_INTEGER_OVERFLOW_IMPL(signed char)
	ESB_THROW_INTEGER_OVERFLOW_IMPL(short)
	ESB_THROW_INTEGER_OVERFLOW_IMPL(int)
	ESB_THROW_INTEGER_OVERFLOW_IMPL(long)
	ESB_THROW_INTEGER_OVERFLOW_IMPL(long long)
	ESB_THROW_INTEGER_OVERFLOW_IMPL(unsigned char)
	ESB_THROW_INTEGER_OVERFLOW_IMPL(unsigned short)
	ESB_THROW_INTEGER_OVERFLOW_IMPL(unsigned int)
	ESB_THROW_INTEGER_OVERFLOW_IMPL(unsigned long)
	ESB_THROW_INTEGER_OVERFLOW_IMPL(unsigned long long)
	ESB_THROW_INTEGER_OVERFLOW_IMPL(/*plain*/ char)
#undef ESB_THROW_INTEGER_OVERFLOW_IMPL

	//template<> ESB_NORETURN inline void throw_error_integer_overflow<long>()	{ ESL_THROW_NUMERIC_LONG_OVERFLOW(); }
	//template<> ESB_NORETURN inline void throw_error_integer_overflow<size_t>()	{ ESL_THROW_NUMERIC_SIZET_OVERFLOW(); }
}


#endif // !__ESB_ERRORS_H__
