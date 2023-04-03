﻿#pragma once
#ifndef ESB_APP_CONFIG_H_
#define ESB_APP_CONFIG_H_

// Данный файл описывает конфигурацию Вашего ЕСБ-приложения. Он должен быть включен до включения esb.h
// (или, если этот файл доступен для включения esb_app.h подключит его сам. см. esb_app.h)


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

// Для демонстрации возможности есб-по-русски. На данный момент очень ограниченная поддержка
#define ESB_USE_RUSSIAN				1


// для использования ЕСБ как SCOM модуля или подключайте компоненту из bin директории 1С, или используйте закгрузку при помощи ms-detours
// (вам нужно скачать его из https://github.com/microsoft/Detours, скомпилировать для Вашего компьютера и прописать в ESB_USE_DETOUR_PATH ваш путь к библиотеке)
// использование ЕСБ не как SCOM модуль, а как объект компоненты также возможно, но нужно внимательно следить чтобы все объекты созданные и запомненные
// в глобальных переменных 1С были освобождены до освобождения самого объекта ЕСБ и сама переменная хранящая объект ЕСБ была очищена в
// Процедуре ПриЗавершенииРаботыСистемы().
#define ESB_USE_DETOUR				1
#define ESB_USE_DETOUR_PATH			../../../$cpp/ms-detours/detours

#endif	//ESB_APP_CONFIG_H_