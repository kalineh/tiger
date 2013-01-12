set srcExe=vs9\Final\FunkEngine.exe
call ExportBuildHelper.bat

set iniFile=main.ini
set iniFullPath=%exportFolder%common\ini\%iniFile%

set gmDebugFile=Debug.gm
set gmDebugFullPath=%exportFolder%common\gm\%gmDebugFile%

:: Massage INI file
echo.
echo Massaging ini file: %iniFullPath%
cscript /nologo tools\Export\IniMassage.vbs %iniFullPath% >  %iniFullPath%_TEMP
if %errorlevel% neq 0 pause /b %errorlevel%
move /Y  %iniFullPath%_TEMP %iniFullPath%

:: Massage GameMonkey debug file
echo.
echo Massaging GM debug file: %gmDebugFullPath%
cscript /nologo tools\Export\GmDebugMassage.vbs %gmDebugFullPath% >  %gmDebugFullPath%_TEMP
if %errorlevel% neq 0 pause /b %errorlevel%
move /Y  %gmDebugFullPath%_TEMP %gmDebugFullPath%


:: GM convert common GM files
set gmCommonPath=%exportFolder%common\gm
cscript /nologo tools\Export\GmByteCoder.vbs %gmCommonPath%
if %errorlevel% neq 0 pause /b %errorlevel%


:: GM convert game GM files
set gmGamePath=%appFullPath%gm\
cscript /nologo tools\Export\GmByteCoder.vbs %gmGamePath%
if %errorlevel% neq 0 pause /b %errorlevel%

echo.
echo Success!
echo.

pause