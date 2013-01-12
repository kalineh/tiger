set appTitle=leaptest
set appDirectory=leaptest


@echo off

set exportRootFolder=build\
set exportFolder=%exportRootFolder%%appTitle%\
set dstExe=%appTitle%.exe
set gamesPath=%exportFolder%game\
set appFullPath=%gamesPath%%appDirectory%\

:: Remove old builds
echo Removing directory %exportRootFolder% ...
if EXIST %exportRootFolder% rmdir /S /Q %exportRootFolder%
mkdir %exportRootFolder%

:: Svn export the folder
echo Svn exporting resources\ to %exportFolder%
svn export resources\ %exportFolder%
if %errorlevel% neq 0 pause /b %errorlevel%
copy %srcExe% "%exportFolder%%dstExe%"


:: save only the current game dir
rmdir /S /Q %gamesPath%
mkdir %gamesPath%
echo.
echo Exporting out only: %appFullPath%
svn export resources\game\%appDirectory% %appFullPath%