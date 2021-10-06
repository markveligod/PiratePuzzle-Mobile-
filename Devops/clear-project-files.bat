@echo off
SET dirsRemove=Intermediate DerivedDataCache Saved Binaries .vs .idea
SET filesToRemove=*.sln
CD ..
ECHO Start Remove project files...
FOR /R %%f IN (%dirsRemove%) DO (
ECHO %%f
RMDIR /s /q %%f
)

FOR %%f IN (%filesToRemove%) DO (
ECHO %%f
DEL /q %%f
)

ECHO Finished Remove project files. Press Enter.
PAUSE
