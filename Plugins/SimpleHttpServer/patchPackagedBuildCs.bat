@echo off
setlocal EnableExtensions EnableDelayedExpansion

rem Usage:
rem   PatchPackagedBuildCs.bat "<OutputPath>" "<PluginName>" [true|false]
rem Example:
rem   PatchPackagedBuildCs.bat "C:\Repo\Packages\SimpleHttpServer" "SimpleHttpServer" true

set "OUTPUT_PATH=%~1"
set "PLUGIN_NAME=%~2"
set "USE_PRECOMPILED=%~3"

if "%OUTPUT_PATH%"=="" (
  echo Error: OUTPUT_PATH is required.
  exit /b 1
)
if "%PLUGIN_NAME%"=="" (
  echo Error: PLUGIN_NAME is required.
  exit /b 1
)

if "%USE_PRECOMPILED%"=="" set "USE_PRECOMPILED=true"

if /I not "%USE_PRECOMPILED%"=="true" if /I not "%USE_PRECOMPILED%"=="false" (
  echo Error: USE_PRECOMPILED must be true or false. Got: "%USE_PRECOMPILED%"
  exit /b 1
)

set "OUT_BUILD_CS=%OUTPUT_PATH%\Source\%PLUGIN_NAME%\%PLUGIN_NAME%.Build.cs"

if not exist "%OUT_BUILD_CS%" (
  echo Error: Packaged Build.cs not found.
  echo   %OUTPUT_PATH%\Source\%PLUGIN_NAME%\%PLUGIN_NAME%.Build.cs
  exit /b 1
)

set "PS1=%~dp0patchPackagedBuildCs.ps1"
if not exist "%PS1%" (
  echo Error: Patch script not found: "%PS1%"
  exit /b 1
)

echo Patching packaged Build.cs...
echo   Path           : %OUT_BUILD_CS%
echo   bUsePrecompiled: %USE_PRECOMPILED%

powershell -NoProfile -ExecutionPolicy Bypass -File "%PS1%" -Path "%OUT_BUILD_CS%" -Value "%USE_PRECOMPILED%"
if errorlevel 1 (
  echo Error: Failed to patch packaged Build.cs.
  exit /b %ERRORLEVEL%
)

echo Done.
endlocal
exit /b 0
