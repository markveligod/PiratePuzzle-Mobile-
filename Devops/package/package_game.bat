@echo off

call "%~dp0\..\config.bat"

"%RunUATPath%" BuildCookRun ^
-project="%ProjectPath%" ^
-targetplatform="%Platform%" -cookflavor=ETC2 ^
-clientconfig="%Configuration%" ^
-archivedirectory="%ArchivePath%" ^
-build -cook -package -stage -archive -pak -compressed -clean -distribution

