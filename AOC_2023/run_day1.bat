@echo off

if "%1" == "new" (
    RMDIR /S /Q build
    mkdir build
)
cd day_1
cmake -B ../build
cd ..