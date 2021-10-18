@echo off

rem Engine params
set EnginePath=D:\UE_4.26

set UBTRelativePath=Engine\Binaries\DotNET\UnrealBuildTool.exe
set VersionSelector=d:\Epic Games\Launcher\Engine\Binaries\Win64\UnrealVersionSelector.exe

rem !! Engine version for packaging !!
set RunUATPath=%EnginePath%\Engine\Build\BatchFiles\RunUAT.bat

rem Project params
set ProjectRoot=C:\Users\mark\Desktop\NewPiratePuzzle
set ProjectPureName=PiratePuzzle
set ProjectName=%ProjectPureName%.uproject
set ProjectPath=%ProjectRoot%\%ProjectName%

rem Build params
set Platform=Win64
set Configuration=Development
set ArchivePath=%ProjectRoot%\Build

rem Other params
set SourceCodePath=%ProjectRoot%\Source
set dirsToRemove=Intermediate DerivedDataCache Saved Binaries .vs .idea
set filesToRemove=*.sln

rem Target params
set COPYRIGHT_LINE=// My game copyright
