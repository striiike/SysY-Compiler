@echo off
setlocal enabledelayedexpansion
set "archiveName=compiler"
set "winRarPath=D:\Program Files\WinRAR\WinRAR.exe"

if exist "%archiveName%.zip" del "%archiveName%.zip"
if exist "%winRarPath%" (
    "%winRarPath%" a -afzip -r "%archiveName%.zip" driver lexer parser main.cpp
) else (
    echo WinRAR not found at %winRarPath%. Please check the path and try again.
    pause
)
