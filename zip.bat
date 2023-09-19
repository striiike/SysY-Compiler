@echo off
setlocal

:: 设置输出ZIP文件的名称
set "outputZipFile=src.zip"

:: 删除已经存在的ZIP文件（如果有的话）
if exist "%outputZipFile%" (
    del "%outputZipFile%"
)

:: 使用PowerShell命令递归地添加所有.cpp、.c和.h文件到ZIP文件中，同时忽略cmake-build-debug文件夹下的文件
powershell -command "Get-ChildItem -Recurse -Include *.cpp,*.c,*.h | Where-Object { $_.FullName -notmatch 'cmake-build-debug' } | Compress-Archive -DestinationPath %outputZipFile%"

:: 恢复环境变量
endlocal

@echo on
