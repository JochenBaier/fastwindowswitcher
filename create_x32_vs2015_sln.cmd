@echo off

set current_drive=%~d0
set current_dir=%~dp0
set build_dir=%~dp0build_x32_vs15
				
@echo current-dir: %current_dir%
@echo current-drive: %current_drive%
@echo build-dir: %build_dir%

pushd "%current_dir%"

rmdir "%build_dir%" /Q /S 2>nul
mkdir "%build_dir%"
cd "%build_dir%"

"C:\Program Files (x86)\CMake\bin\cmake.exe" .. -G"Visual Studio 15 2017"  -T v140 -DCMAKE_PREFIX_PATH=E:\Entwicklung\qt\qt59_vs2015_static
if %errorlevel% NEQ 0 (
  rmdir %build_dir%
  GOTO :EXIT
)

:EXIT
popd

IF %errorlevel% NEQ 0 (
  ECHO Ablauf mit Fehler %errorlevel% abgebrochen!
  PAUSE
)