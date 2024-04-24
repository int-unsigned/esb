@echo off
rem 	в конце пути к конфигурации 1С не желательно ставить завершающий '\'
rem		или нужно доработать примитивный ПутьФайлЕсбКомпоненты()
rem
rem		Студия ни в какую не хочет передавать аргументы отладки в кавычках
rem		поэтому путь к платформе тупо передаем через три последних параметра %2 %3 %4 
rem		и конкатенируем их через пробел аккуратно - это будет путь. три на всякий случай..
rem		(первым параметром передается относительный путь к компоненте. относительный чтобы таже казусов не было)
rem		(аккуратно конкатенировать надо т.к. запуск с лишним пробелом - ок, а EXIST этого не понимает)
rem @echo on

:: TODO для красоты можно параметр из IDE передавать, 
::		но тогда его нужно делать первым, а путь конкатенировать начиная со второго
goto RUN_1C_MESSAGES_RU
set RUN_1C_MESSAGE_STARTING=Staring 1C......................................
set RUN_1C_MESSAGE_DONE=Done.
set RUN_1C_MESSAGE_NOT_FOUND=1C executable file not found
set RUN_1C_MESSAGE_NO_1C=Start debuging for configuration '%1%' is not configured
goto RUN_1C

:: Для любителей
:RUN_1C_MESSAGES_RU
chcp 1251 >NUL
set RUN_1C_MESSAGE_STARTING=Запускаем 1С......................................
set RUN_1C_MESSAGE_DONE=Готово.
set RUN_1C_MESSAGE_NOT_FOUND=Исполняемый файл 1С не найден
set RUN_1C_MESSAGE_NO_1C=Запуск отладки для конфигурации '%1%' не настроен
chcp 866 >NUL


:RUN_1C
IF "%2" == "" GOTO EXIT_ERROR_NO_1C

SET RUN_1C_PATH=%2
IF NOT "%3" == "" SET RUN_1C_PATH=%2 %3
IF NOT "%4" == "" SET RUN_1C_PATH=%2 %3 %4


SET RUN_1C_BIN=%RUN_1C_PATH%\1cv8.exe
IF NOT EXIST "%RUN_1C_BIN%" GOTO EXIT_ERROR_1C_BIN_INVALID

echo %RUN_1C_MESSAGE_STARTING%
:: При таком запуске окно консоли остается открытым до закрытия 1С
"%RUN_1C_BIN%" ENTERPRISE /F".\1s" /C"--esbrun=%1%"
:: При таком окно консоли "блымнет" и закроется (на мой взгляд с мессаджем красивее)
REM start "" "%RUN_1C_BIN%" ENTERPRISE /F".\1s" /C"--esbrun=%1%"
echo %RUN_1C_MESSAGE_DONE%
exit

:EXIT_ERROR_1C_BIN_INVALID
echo %RUN_1C_MESSAGE_NOT_FOUND% "%RUN_1C_BIN%" 
pause
exit

:EXIT_ERROR_NO_1C
echo %RUN_1C_MESSAGE_NO_1C%
pause
exit