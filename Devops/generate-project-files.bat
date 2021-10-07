@echo off
ECHO Options Params
SET VersionSelector=d:\Epic Games\Launcher\Engine\Binaries\Win64\UnrealVersionSelector.exe
SET ProjectName=PiratePuzzle.uproject
SET ProjectPath=%CD%\..\%ProjectName%
SET EnginePath=d:\UE_4.26
SET UBTRelativePath=Engine\Binaries\DotNET\UnrealBuildTool.exe

ECHO %VersionSelector%
ECHO %ProjectName%
ECHO %ProjectPath%
ECHO %EnginePath%

ECHO Start Selector
"%VersionSelector%" -switchversionsilent "%ProjectPath%" "%EnginePath%"
"%EnginePath%\%UBTRelativePath%" -projectfiles -progress -project="%ProjectPath%"
ECHO Finish Selector. Press Enter.
PAUSE