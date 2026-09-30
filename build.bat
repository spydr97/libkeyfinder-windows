@echo off
REM Build kfcli.exe (static) on Windows with MSVC + vcpkg (manifest mode).
REM Requires: Visual Studio 2019/2022/BuildTools with "Desktop development with C++",
REM and a vcpkg checkout (set VCPKG_ROOT, or it defaults to %USERPROFILE%\vcpkg).
setlocal EnableDelayedExpansion

if "%VCPKG_ROOT%"=="" set "VCPKG_ROOT=%USERPROFILE%\vcpkg"
if not exist "%VCPKG_ROOT%\scripts\buildsystems\vcpkg.cmake" (
  echo ERROR: vcpkg not found at "%VCPKG_ROOT%".
  echo   git clone https://github.com/microsoft/vcpkg "%VCPKG_ROOT%"
  echo   "%VCPKG_ROOT%\bootstrap-vcpkg.bat"
  exit /b 1
)

REM Locate Visual Studio and activate the x64 MSVC environment.
set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
for /f "usebackq tokens=*" %%i in (`"%VSWHERE%" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath`) do set "VSPATH=%%i"
if "%VSPATH%"=="" (
  echo ERROR: Visual Studio with the C++ toolset was not found.
  exit /b 1
)
call "%VSPATH%\VC\Auxiliary\Build\vcvars64.bat" || exit /b 1

REM Use the CMake and Ninja that ship with Visual Studio.
set "PATH=%VSPATH%\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin;%VSPATH%\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja;%PATH%"

cd /d "%~dp0"
cmake -B build -S . -G Ninja ^
  -DCMAKE_TOOLCHAIN_FILE="%VCPKG_ROOT%\scripts\buildsystems\vcpkg.cmake" ^
  -DVCPKG_TARGET_TRIPLET=x64-windows-static ^
  -DCMAKE_BUILD_TYPE=Release || exit /b 1
cmake --build build || exit /b 1

if not exist dist mkdir dist
copy /Y build\kfcli.exe dist\keyfinder.exe >nul
echo.
echo Built: dist\keyfinder.exe
