#
# Copyright (C) 2025 Sony Corporation
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

# Android build script for Windows PowerShell
param(
    [string]$BuildType = "release"  # -BuildType "debug" or "release"
)

# Android executable creation & deployment (set ANDROID_NDK_ROOT environment variable or modify the path below)
$ANDROID_NDK_ROOT = if ($env:ANDROID_NDK_ROOT) { 
    $env:ANDROID_NDK_ROOT 
} else { 
    Write-Warning "ANDROID_NDK_ROOT environment variable not set, using default path"
    Write-Warning "Please set ANDROID_NDK_ROOT environment variable to your NDK installation path"
    "E:\Android_NDK\android-ndk-r21e"
}

# Determine build type based on arguments
switch ($BuildType.ToLower()) {
    "debug" {
        Write-Host "Start the debug build." -ForegroundColor Green
        $APPLICATION_MK = Join-Path (Get-Location).Path "..\..\jni\arm64-v8a\Application_arm64-v8a_debug.mk"
    }
    "release" {
        Write-Host "Start the release build." -ForegroundColor Green
        $APPLICATION_MK = Join-Path (Get-Location).Path "..\..\jni\arm64-v8a\Application_arm64-v8a_release.mk"
    }
    default {
        Write-Host "Start the release build (default)." -ForegroundColor Green
        $APPLICATION_MK = Join-Path (Get-Location).Path "..\..\jni\arm64-v8a\Application_arm64-v8a_release.mk"
    }
}

try {
    # Move to project root
    Push-Location -Path "..\.."
    $ProjectRoot = Get-Location

    # Execute NDK build
    $ndkBuildCmd = Join-Path $ANDROID_NDK_ROOT "ndk-build.cmd"
    $arguments = @(
        "NDK_PROJECT_PATH=."
        "NDK_APPLICATION_MK=`"$APPLICATION_MK`""
    )
    
    Write-Host "Executing NDK build..." -ForegroundColor Yellow
    Write-Host "Command: $ndkBuildCmd $($arguments -join ' ')" -ForegroundColor Gray
    
    & $ndkBuildCmd $arguments
    
    if ($LASTEXITCODE -ne 0) {
        throw "NDK build failed with error code $LASTEXITCODE"
    }

    # Return to build directory
    Pop-Location

    # Prepare output directory
    $outputDir = Join-Path (Get-Location).Path "mocopi_motion_serializer\lib\android"
    if (Test-Path $outputDir) {
        Remove-Item $outputDir -Recurse -Force
    }
    New-Item -ItemType Directory -Path $outputDir -Force | Out-Null

    # Execute move process only if libs directory exists and contains files
    $libsDir = Join-Path $ProjectRoot "libs"
    if (Test-Path $libsDir) {
        $libFiles = Get-ChildItem $libsDir -Recurse -File
        if ($libFiles.Count -gt 0) {
            Write-Host "Moving libraries to output directory..." -ForegroundColor Yellow
            Copy-Item -Path "$libsDir\*" -Destination $outputDir -Recurse -Force
        } else {
            Write-Warning "No library files found in $libsDir"
        }
    } else {
        Write-Warning "libs directory not found: $libsDir"
    }

    # Cleanup
    $libsCleanup = Join-Path $ProjectRoot "libs"
    $objCleanup = Join-Path $ProjectRoot "obj"
    
    if (Test-Path $libsCleanup) {
        Remove-Item $libsCleanup -Recurse -Force
    }
    if (Test-Path $objCleanup) {
        Remove-Item $objCleanup -Recurse -Force
    }

    Write-Host "Build completed successfully!" -ForegroundColor Green

} catch {
    Write-Error "Build failed: $_"
    Pop-Location -ErrorAction SilentlyContinue
    exit 1
} finally {
    # Clear location stack as a precaution
    while ((Get-Location -Stack).Count -gt 0) {
        Pop-Location -ErrorAction SilentlyContinue
    }
}