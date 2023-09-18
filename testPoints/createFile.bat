@echo off
setlocal enabledelayedexpansion

for /L %%i in (1, 1, 6) do (
    set "filename=testfile%%i.txt"
    echo Creating !filename!
    echo. > !filename!

    set "filename=input%%i.txt"
    echo Creating !filename!
    echo. > !filename!

    set "filename=output%%i.txt"
    echo Creating !filename!
    echo. > !filename!
)

endlocal
