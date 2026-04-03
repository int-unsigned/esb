@echo off
rem Подставьте свой путь к 1С
rem @echo on
SET RUN_1C_PATH=c:\Program Files\1cv8\8.3.9.1818\bin\
SET THIS_PATH=.
SET S_PATH_TARGET=%THIS_PATH%\1s
SET S_PATH_SOURCE=%THIS_PATH%\1s_files
SET S_FILE_LOG=%THIS_PATH%\_1s_create.log
REM Если /AddInList"<ИмяБазыВСпискеЗапуска>" уже есть в списке, то база создается, но ERRORLEVEL==1 и заполнение НЕ ПРОИСХОДИТ!
REM Поэтому эту опцию просто не указываем, или нужно как-то проверять и(или) обходить... TODO
SET S_ADDINLIST=


:: TODO для красоты можно параметр из IDE передавать, 
::		но тогда его нужно делать первым, а путь конкатенировать начиная со второго
goto RUN_1C_MESSAGES_RU
set RUN_1C_MESSAGE_STARTING=Staring 1C......................................
set RUN_1C_MESSAGE_DONE=Done.
goto RUN_1C

:: Для любителей
:RUN_1C_MESSAGES_RU
chcp 1251 >NUL
set RUN_1C_MESSAGE_STARTING=Запускаем 1С......................................
set RUN_1C_MESSAGE_DONE=Готово.
chcp 866 >NUL


:RUN_1C
echo %RUN_1C_MESSAGE_STARTING%

:: При таком запуске окно консоли остается открытым до закрытия 1С
"%RUN_1C_PATH%\1cv8.exe" CREATEINFOBASE File="%S_PATH_TARGET%"; %S_ADDINLIST% /Out"%S_FILE_LOG%"
IF %ERRORLEVEL% NEQ 0 ( 
   GOTO EXIT_ERROR_CREATE 
)
"%RUN_1C_PATH%\1cv8.exe" DESIGNER /F"%S_PATH_TARGET%" /LoadConfigFromFiles"%S_PATH_SOURCE%" /UpdateDBCfg /Out"%S_FILE_LOG%" -NoTruncate
IF %ERRORLEVEL% NEQ 0 ( 
   GOTO EXIT_ERROR_FILL 
)

echo %RUN_1C_MESSAGE_DONE%
pause
exit

:EXIT_ERROR_CREATE
echo CREATE.RESULT=%ERRORLEVEL%
echo NOT CREATED
pause
exit

:EXIT_ERROR_FILL
echo FILL.RESULT=%ERRORLEVEL%
echo CREATED, BUT EMPTY!!!
pause
