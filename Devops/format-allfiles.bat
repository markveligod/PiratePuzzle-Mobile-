@echo off

ECHO Start format...
CD ..\Source
FOR /R %%f IN (*.cpp, *.h, *.cs) DO (
ECHO %%f
clang-format -i %%f
)

ECHO End format. Press enter.

PAUSE