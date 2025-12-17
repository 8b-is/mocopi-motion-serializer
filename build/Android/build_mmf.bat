@echo off
REM
REM Copyright (C) 2025 Sony Corporation
REM
REM Licensed under the Apache License, Version 2.0 (the "License");
REM you may not use this file except in compliance with the License.
REM You may obtain a copy of the License at
REM
REM      http://www.apache.org/licenses/LICENSE-2.0
REM
REM Unless required by applicable law or agreed to in writing, software
REM distributed under the License is distributed on an "AS IS" BASIS,
REM WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
REM See the License for the specific language governing permissions and
REM limitations under the License.
REM

REM Android build script for Windows
setlocal enabledelayedexpansion

REM Android executable creation & deployment (set ANDROID_NDK_ROOT environment variable or modify the path below)
if not defined ANDROID_NDK_ROOT (
    set ANDROID_NDK_ROOT=E:\Android_NDK\android-ndk-r21e
    echo Warning: ANDROID_NDK_ROOT not set, using default: !ANDROID_NDK_ROOT!
    echo Please set ANDROID_NDK_ROOT environment variable to your NDK installation path
)

REM Save current directory
set BUILD_DIR=%cd%

REM Determine build type based on arguments
if "%1"=="-d" (
    echo Start the debug build.
    set BUILD_TYPE=debug
    set APPLICATION_MK=%BUILD_DIR%\..\..\jni\arm64-v8a\Application_arm64-v8a_debug.mk
) else if "%1"=="-r" (
    echo Start the release build.
    set BUILD_TYPE=release
    set APPLICATION_MK=%BUILD_DIR%\..\..\jni\arm64-v8a\Application_arm64-v8a_release.mk
) else (
    echo Start the release build.
    set BUILD_TYPE=release
    set APPLICATION_MK=%BUILD_DIR%\..\..\jni\arm64-v8a\Application_arm64-v8a_release.mk
)

REM Move to project root
pushd ..\..

REM Execute NDK build
echo Executing NDK build command...
"%ANDROID_NDK_ROOT%\ndk-build.cmd" NDK_PROJECT_PATH=. NDK_APPLICATION_MK="!APPLICATION_MK!"
echo NDK build completed with error level: %ERRORLEVEL%

if %ERRORLEVEL% neq 0 (
    echo NDK build failed with error code %ERRORLEVEL%
    popd
    exit /b %ERRORLEVEL%
)

REM Return to build directory
popd

REM Prepare output directory
if exist "%BUILD_DIR%\mocopi_motion_serializer\lib\android" (
    rmdir /s /q "%BUILD_DIR%\mocopi_motion_serializer\lib\android"
)
mkdir "%BUILD_DIR%\mocopi_motion_serializer\lib\android"

REM Execute move process only if libs directory exists and contains files
if exist "..\..\libs" (
    echo Moving libraries to output directory...
    xcopy /s /y "..\..\libs\*" "%BUILD_DIR%\mocopi_motion_serializer\lib\android\"
) else (
    echo Warning: No libraries found in libs directory
)

REM Cleanup
if exist "..\..\libs" rmdir /s /q "..\..\libs"
if exist "..\..\obj" rmdir /s /q "..\..\obj"

echo Build completed successfully.