@echo off
echo ========================================================
echo   Building Career Path Prediction System for Windows
echo ========================================================
echo.

where gcc >nul 2>nul
if %errorlevel% neq 0 (
    echo [ERROR] GCC compiler not found in PATH!
    echo Please install MinGW-w64 / WinLibs / MSYS2 GCC.
    pause
    exit /b 1
)

echo Compiling C source files (Zero external library dependencies required)...
gcc -Wall -Wextra -Iinclude src\main.c src\auth.c src\student.c src\assessment.c src\prediction.c src\gap_analysis.c src\db_handler.c src\report.c src\ui.c src\colors.c src\input_handler.c src\analytics.c src\sha256.c src\sqlite3.c -o career_system.exe

if %errorlevel% equ 0 (
    echo.
    echo [SUCCESS] Build succeeded! Executable generated: career_system.exe
    echo You can now run career_system.exe in CMD/PowerShell or double-click run.bat
) else (
    echo.
    echo [ERROR] Build failed! Check compiler output above.
)
pause
