@echo off

:main
:: Step 1: Delete folder "./bin/Debug" if it exists
if exist ".\bin\Debug" (
    echo Deleting .\bin\Debug folder...
    rmdir /s /q ".\bin\Debug"
)

:: Step 2: Run "build.bat"
echo:
echo Running build.bat...
call build.bat
echo:

:: Step 3: Copy "@Files" folder as a whole to "./bin/Debug"
echo Copying ./bin/@Files to ./bin/Debug...
if not exist ".\bin\Debug" mkdir ".\bin\Debug"
xcopy ".\bin\@Files" ".\bin\Debug\@Files" /e /i /y

:: Step 4: Copy files from "./bin/dlls" to "./bin/Debug"
echo Copying files from ./bin/dlls to ./bin\Debug...
xcopy ".\bin\dlls\*.*" ".\bin\Debug" /y

:: Step 5: Run "./bin/Debug/GView.exe" if it exists
if exist ".\bin\Debug\GView.exe" (
    echo Running GView.exe...
    start "" ".\bin\Debug\GView.exe"
) else (
    echo GView.exe not found in ./bin/Debug!
)

:: Completion message
echo All operations completed.

:: Step 6: Prompt for rerun or exit
call :rerun_or_exit
exit /b

:rerun_or_exit
echo.
echo Press ENTER to rerun the script or ESCAPE to exit.
choice /c "ER" /n /m "Press 'E' to rerun or 'R' to exit: "

if errorlevel 2 exit /b
if errorlevel 1 goto :main
