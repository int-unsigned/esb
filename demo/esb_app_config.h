﻿#pragma once
#ifndef ESB_APP_CONFIG_H_
#define ESB_APP_CONFIG_H_

// Данный файл описывает конфигурацию Вашего ЕСБ-приложения. Он должен быть включен до включения esb.h
// (или, если этот файл доступен для включения из esb_config.h подключит его сам. см. esb_config.h)


// Версия платформы (и есб соответственно) (допустимые значения 8300, 8311, 8320, 8323)
// Вы можете указать ее здесь принудительно иначе esb_app.h настроит последнюю доступную (8323). 
// (для esb_demo указывается в свойствах проекта)
#ifndef ESB_VER
#	define ESB_VER	8309
#endif



// Контролирует принудительное конструирование есб-строки String из с++ строки
// Необходимо помнить, что любое создание есб-строки - это создание строки в пуле памяти 1С и по возможности избегать лишней нагрузки на память
#define ESB_USE_EXPLICIT_STRING		1

// То же самое для есб-числа. Любое есб-число это нетривиальный объект 1С, который также конструируется платформой.
// Если не контролировать его создание, то очень легко даже не желая того перегрузить пул памяти 1С, т.к. в с++ числа используются очень часто, много
// и обычно в с++ Вы не задумываетесь когда пишете 2+2.
#define ESB_USE_EXPLICIT_NUMERIC	0

// Экспериментальная функция разрешения диспатч имен в идентификаторы методов из с++ строки, а не 1С строки (т.е. без лишних аллокаций)
// (при странном и неадекватном поведении можно отключить)
#define ESB_USE_OBJECT_DISPATCH_FIND_STRVIEW_HACK	1

// Экспериментальный прямой доступ к содержимому BinaryDataBuffer минуя диспатч интерфейс. Только для для v8300. (см.esb_es_file.h::BinaryDataBuffer)
// Точно работает в 8.3.9.1818 х86, 8.3.10.2772 х64, 8.3.11.2831 х86
// Точно не работает в 8.3.23.1688
// Видимо изменения были где-то между 8.3.11.2831 и 8.3.23.1688, но когда точно - неизвестно. Нужно экспериментировать и проверять.
#define ESB_USE_BYTEBUFFER_DIRECT_HACK_v8300		0

// Для демонстрации возможности есб-по-русски.
#define ESB_USE_RUSSIAN				1

// По умолчанию конфигурация есб не подключает АПИ платформы. При желании нужно указать это. Также можно подключать АПИ отдельно по каждому провайдеру.
#define ESB_USE_API_ALL				1

// для использования ЕСБ как SCOM модуля или подключайте компоненту из bin директории 1С (Вы должны иметь права на доступ к этой директории!), 
// или используйте закгрузку при помощи ms-detours (это удобно для отладочных сборок, но категорически не рекомендуется для релиза)
// (вам нужно скачать его из https://github.com/microsoft/Detours, скомпилировать для Вашего компьютера и прописать в ESB_USE_DETOUR_PATH ваш путь к библиотеке)
// использование ЕСБ не как SCOM модуль, а как объект компоненты также возможно, но нужно внимательно следить чтобы все объекты созданные и запомненные
// в глобальных переменных 1С были освобождены до освобождения самого объекта ЕСБ и сама переменная хранящая объект ЕСБ была очищена в
// Процедуре ПриЗавершенииРаботыСистемы().
#define ESB_USE_DETOUR				1
#define ESB_USE_DETOUR_PATH			../../$cpp/ms-detours/detours

#endif	//ESB_APP_CONFIG_H_