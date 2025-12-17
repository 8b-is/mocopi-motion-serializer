#!/bin/bash
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

# Android library build script for Mac/Linux
# Usage:
#   ./build_mmf.sh      # Release build
#   ./build_mmf.sh -r   # Release build (explicit)
#   ./build_mmf.sh -d   # Debug build
#
# Prerequisites:
# - Android NDK is installed
# - ANDROID_NDK_ROOT variable is correctly set

# Android executable creation & deployment (set ANDROID_NDK_ROOT environment variable or modify the path below)
if [ -z "$ANDROID_NDK_ROOT" ]; then
    # Default path setting (modify according to your environment)
    ANDROID_NDK_ROOT="/usr/local/share/android-ndk"
    # Or if installed via Homebrew
    # ANDROID_NDK_ROOT="$(brew --prefix)/share/android-ndk"
    # Or if installed via Android Studio
    # ANDROID_NDK_ROOT="$HOME/Library/Android/sdk/ndk/21.4.7075529"
    
    echo "Warning: ANDROID_NDK_ROOT environment variable not set"
    echo "Using default path: $ANDROID_NDK_ROOT"
    echo "Please set ANDROID_NDK_ROOT environment variable to your NDK installation path"
fi

# Check if NDK path exists
if [ ! -d "$ANDROID_NDK_ROOT" ]; then
    echo "Error: Android NDK not found at $ANDROID_NDK_ROOT"
    echo "Please set ANDROID_NDK_ROOT to your NDK installation path"
    exit 1
fi

# OS detection - conditional branching for macOS
if [[ "$OSTYPE" == "darwin"* ]]; then
  # NDK build command for macOS
  case "$1" in
  "-d") 
    echo "Start the debug build."
    cd ../../;
    "$ANDROID_NDK_ROOT/ndk-build" NDK_PROJECT_PATH=. NDK_APPLICATION_MK="$PWD/jni/arm64-v8a/Application_arm64-v8a_debug.mk"
  ;;
  "-r") 
    echo "Start the release build."
    cd ../../;
    "$ANDROID_NDK_ROOT/ndk-build" NDK_PROJECT_PATH=. NDK_APPLICATION_MK="$PWD/jni/arm64-v8a/Application_arm64-v8a_release.mk"
  ;;
  *) 
    echo "Start the release build."
    cd ../../;
    "$ANDROID_NDK_ROOT/ndk-build" NDK_PROJECT_PATH=. NDK_APPLICATION_MK="$PWD/jni/arm64-v8a/Application_arm64-v8a_release.mk"
  ;;
  esac
elif [[ "$OSTYPE" == "linux-gnu"* ]]; then
  # NDK build command for Linux
  case "$1" in
  "-d") 
    echo "Start the debug build."
    cd ../../;
    "$ANDROID_NDK_ROOT/ndk-build" NDK_PROJECT_PATH=. NDK_APPLICATION_MK="$PWD/jni/arm64-v8a/Application_arm64-v8a_debug.mk"
  ;;
  "-r") 
    echo "Start the release build."
    cd ../../;
    "$ANDROID_NDK_ROOT/ndk-build" NDK_PROJECT_PATH=. NDK_APPLICATION_MK="$PWD/jni/arm64-v8a/Application_arm64-v8a_release.mk"
  ;;
  *) 
    echo "Start the release build."
    cd ../../;
    "$ANDROID_NDK_ROOT/ndk-build" NDK_PROJECT_PATH=. NDK_APPLICATION_MK="$PWD/jni/arm64-v8a/Application_arm64-v8a_release.mk"
  ;;
  esac
else
  echo "Unsupported operating system: $OSTYPE"
  echo "Supported platforms: macOS (darwin), Linux (linux-gnu)"
  exit 1
fi

# Check NDK build result
BUILD_EXIT_CODE=$?
if [ $BUILD_EXIT_CODE -ne 0 ]; then
    echo "Error: NDK build failed with exit code $BUILD_EXIT_CODE"
    exit $BUILD_EXIT_CODE
fi

# Return to build/Android directory
cd build/Android

# Prepare output directory
rm -rf mocopi_motion_serializer/lib/android
mkdir -p mocopi_motion_serializer/lib/android

# Execute move process only if libs directory exists and contains files
if [ -d "../../libs" ] && [ "$(ls -A ../../libs 2>/dev/null)" ]; then
    echo "Moving libraries to output directory..."
    cp -rf ../../libs/* mocopi_motion_serializer/lib/android/
    echo "Successfully copied libraries to mocopi_motion_serializer/lib/android/"
else
    echo "Warning: No libraries found in ../../libs directory"
fi

# Cleanup
rm -rf ../../libs
rm -rf ../../obj

echo "Build completed successfully!"
echo "Output libraries are in: $(pwd)/mocopi_motion_serializer/lib/android/"
