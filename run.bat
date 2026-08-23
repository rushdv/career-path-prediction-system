@echo off
if not exist career_system.exe (
    echo Executable career_system.exe not found! Building first...
    call build.bat
)
if exist career_system.exe (
    career_system.exe
)
