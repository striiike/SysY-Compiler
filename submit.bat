@echo off
setlocal

:: 定义源文件和目标文件的路径
set "sourceFile=testfile.c"
set "targetDir=testPoints"
set "targetFile=testPoints\testfile1.txt"

:: 检查源文件是否存在
if not exist "%sourceFile%" (
    echo Source file "%sourceFile%" does not exist.
    exit /b 1
)

:: 检查目标目录是否存在，如果不存在则创建
if not exist "%targetDir%" (
    mkdir "%targetDir%"
)

:: 移除源文件的前四行，并将其保存到目标文件中
more +5 "%sourceFile%" > "%targetFile%"

:: 运行 testPoints 文件夹中的 zipFile.bat
cd "%targetDir%"
call zipFile.bat

endlocal
