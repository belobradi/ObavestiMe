@echo off
setlocal enabledelayedexpansion

REM === Paths ===
set VCPKG_DIR=%~dp0vcpkg
set BUILD_DIR=%~dp0out
set SRC_DIR=%~dp0cppscraper
set INSTALL_DIR=%~dp0vcpkg_installed

REM === Bootstrap vcpkg if needed ===
if not exist "%VCPKG_DIR%\vcpkg.exe" (
    echo [INFO] vcpkg submodule not found or not initialized.
    echo [INFO] Initializing git submodule for vcpkg...
    git submodule update --init --recursive
    echo [INFO] Bootstrapping vcpkg...
    pushd "%VCPKG_DIR%"
    call bootstrap-vcpkg.bat
    popd
)

REM === Install vcpkg dependencies (manifest mode) ===
echo [INFO] Installing vcpkg dependencies...
%VCPKG_DIR%\vcpkg.exe install --x-install-root=%INSTALL_DIR%

REM === Set up MSVC environment ===
echo [INFO] Setting up MSVC compiler environment...
call "%ProgramFiles(x86)%\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"

REM === Configure CMake ===
echo [INFO] Configuring CMake...
cmake -S "%SRC_DIR%" -B "%BUILD_DIR%" ^
  -DCMAKE_TOOLCHAIN_FILE="%VCPKG_DIR%\scripts\buildsystems\vcpkg.cmake" ^
  -DCMAKE_PREFIX_PATH="%INSTALL_DIR%\x64-windows" ^
  -DCMAKE_BUILD_TYPE=Release

REM === Build ===
echo [INFO] Building project...
cmake --build "%BUILD_DIR%" --config Release

echo.
echo Build finished.
endlocal
pause
