rem set ENGINE_ROOT_PATH=%~dp0
set ENGINE_ROOT_PATH=D:\dev\ue4.git\
set GAME_ROOT_PATH=%~dp0
set GAME_NAME=miniue4

"%ENGINE_ROOT_PATH%Engine\Binaries\Win64\UE4Editor-Cmd.exe" "%GAME_ROOT_PATH%%GAME_NAME%.uproject" -run=linter -RuleSet=name -html -UTF8Output -ReportBaseURL="" 