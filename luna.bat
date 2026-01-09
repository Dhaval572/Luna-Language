@echo off
setlocal enabledelayedexpansion

REM ===============================
REM Luna Language Script Runner
REM ===============================

set BUILD_DIR=build
set CONFIG=Debug
set EXE_NAME=luna.exe
set EXE_PATH=.\%BUILD_DIR%\%CONFIG%\%EXE_NAME%

REM === Create build directory if missing ===
if not exist "%BUILD_DIR%" (
    echo [INFO] Creating build directory...
    mkdir "%BUILD_DIR%" || exit /b 1
)

REM === Configure if not already configured ===
if not exist "%BUILD_DIR%\CMakeCache.txt" (
    echo [INFO] Configuring project...
    cmake -S . -B "%BUILD_DIR%" || exit /b 1
)

REM === Build (incremental) ===
cmake --build "%BUILD_DIR%" --config %CONFIG% || exit /b 1

REM === If no script provided, stop here ===
if "%~1"=="" (
    echo [INFO] Build completed. No script provided.
    exit /b 0
)

REM === Check Luna script existence ===
if not exist "%~1" (
    echo [ERROR] Script file "%~1" not found!
    exit /b 1
)

REM === Run the script (EXACT path like screenshot) ===
"%EXE_PATH%" "%~1"

endlocal