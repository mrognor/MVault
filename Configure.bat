@echo off
cd /d %~dp0
if not exist Build mkdir Build
cd Build
IF "%1"=="" (
    SET /P AREYOUSURE=Generate for MinGW ([Y]/N)
    IF /I "%AREYOUSURE%" NEQ "N" (
        cmake -G "MinGW Makefiles" ..
        xcopy /Y "..\Resources\Csv\CPU_benchmark_v4.csv" "."
    ) else (
        cmake ..
    )
) else (
    cmake %* ..
)
cd ..