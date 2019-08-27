@echo off

Set	FILENAME=%*

if "%FILENAME%" == "" (
	echo Input filename to sign please.
	goto exit
) 

call "%~p0\bin\signcode.bat" "%FILENAME%" 

:exit