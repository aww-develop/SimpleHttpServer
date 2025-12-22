@echo off
setlocal

for %%I in ("%~dp0.") do set "PLUGIN_DIR=%%~fI"

set "PLUGIN_NAME=SimpleHttpServer"
set "UE_VERSION=5.7"
set "UE_PATH=C:\Program Files\Epic Games\UE_%UE_VERSION%"

for %%I in ("%PLUGIN_DIR%\..\..") do set "PROJECT_ROOT=%%~fI"

set "PLUGIN_PATH=%PLUGIN_DIR%\%PLUGIN_NAME%.uplugin"

if "%~1"=="" (
    set "OUTPUT_PATH=%PROJECT_ROOT%\Packages\%PLUGIN_NAME%"
) else (
    pushd "%PLUGIN_DIR%"
    for %%I in ("%~1") do set "OUTPUT_PATH=%%~fI"
    popd
)

echo ========================================
echo Plugin Directory : %PLUGIN_DIR%
echo Project Root     : %PROJECT_ROOT%
echo Plugin Path      : %PLUGIN_PATH%
echo Output Path      : %OUTPUT_PATH%
echo UE Path          : %UE_PATH%
echo ========================================
echo.

if not exist "%PLUGIN_PATH%" (
    echo Error: %PLUGIN_PATH% not found.
    exit /b 1
)

if exist "%OUTPUT_PATH%" (
    echo Cleaning output directory...
    rd /s /q "%OUTPUT_PATH%"
)

echo Building plugin...
call "%UE_PATH%\Engine\Build\BatchFiles\RunUAT.bat" BuildPlugin ^
    -Plugin="%PLUGIN_PATH%" ^
    -Package="%OUTPUT_PATH%" ^
    -TargetPlatforms=Win64 ^
    -HostPlatforms=Win64

if errorlevel 1 (
    echo.
    echo Build failed with errorlevel %ERRORLEVEL%.
    exit /b %ERRORLEVEL%
) else (
    echo.
    echo Build completed successfully!
    echo Output: %OUTPUT_PATH%
    
    call "%~dp0patchPackagedBuildCs.bat" "%OUTPUT_PATH%" "%PLUGIN_NAME%" "true"
    if errorlevel 1 (
        echo.
        echo Post-build patch failed with errorlevel %ERRORLEVEL%.
        exit /b %ERRORLEVEL%
    )
)

endlocal
pause