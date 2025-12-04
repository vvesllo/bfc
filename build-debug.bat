@echo off
cls
cmake --build .\build\ --config Debug
echo -- Running program
build\Debug\bfc.exe