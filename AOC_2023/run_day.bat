@echo off
REM Usage: "run_day.bat n" for executing day n CMake solution (needs day_n folder to exist)
REM Optional second parameter: "run_day.bat n new" to rebuild CMake cache, necessary when changing day

if "%2" == "new" (
    RMDIR /S /Q build
    mkdir build
)
set "day=day_%1"
cd %day%
cmake -B ../build
cd ..