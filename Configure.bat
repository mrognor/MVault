@echo off
if not exist Build mkdir Build
cd Build
IF "%1"=="" (
    SET /P AREYOUSURE=Generate for MinGW (Y/[N])
    IF /I "%AREYOUSURE%" NEQ "Y" (
        cmake ../Source
    ) else (
        cmake -G="MinGW Makefiles" ../Source
    )
) else (
    cmake %* ../Source
)
cd ..