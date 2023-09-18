@echo off
setlocal

:: Set the name of the ZIP file
set "zipfile=testPoints.zip"

:: Remove existing ZIP file if it exists
if exist %zipfile% (
    echo Removing existing %zipfile%
    del /f %zipfile%
)

:: Create a new ZIP file
echo Creating %zipfile%
powershell -command "& { Compress-Archive -Path testfile*.txt,input*.txt,output*.txt -DestinationPath %zipfile% }"

endlocal
