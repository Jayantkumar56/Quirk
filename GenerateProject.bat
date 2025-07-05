
@echo off
setlocal enabledelayedexpansion

echo Generating build files for Quirk-Engine

if exist "build" (
    echo -- 'build' directory already exists.
    echo(
) else (
    set "target_dir=%cd%\build"
    echo -- Creating 'build' directory at !target_dir!
    echo(
    mkdir "!target_dir!"
)

cd build
echo -- Running Cmake...
cmake .. -G "Visual Studio 17 2022"
PAUSE
