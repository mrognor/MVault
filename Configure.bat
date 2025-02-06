@echo off
cd /d %~dp0
if not exist Build mkdir Build
cd Build
IF "%1"=="" (
    SET /P AREYOUSURE=Generate for MinGW ([Y]/N)
    IF /I "%AREYOUSURE%" NEQ "N" (
        cmake -D CMAKE_EXPORT_COMPILE_COMMANDS=1 -G "MinGW Makefiles" ..
        xcopy /Y "..\Resources\Csv\CPU_benchmark_v4.csv" "."
        xcopy /Y "compile_commands.json" ".."
    ) else (
        cmake ..
    )
) else (
    cmake %* ..
)
cd ..