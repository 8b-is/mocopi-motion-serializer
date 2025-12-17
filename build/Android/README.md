# Cross-Platform Android Library Build Scripts

This directory contains scripts for building Android libraries on Windows, macOS, and Linux environments.

## File List

### For Windows Environment
1. **build_mmf.bat** - Batch file for Windows Command Prompt
2. **build_mmf.ps1** - PowerShell script for Windows (Recommended)

### For macOS/Linux Environment  
3. **build_mmf.sh** - Shell script for macOS/Linux

## Usage

### Windows Environment

#### PowerShell Script (Recommended)

**Using environment variables (recommended):**
```powershell
# Set environment variable
$env:ANDROID_NDK_ROOT="C:\path\to\your\android-ndk-r21e"

# Release build
.\build_mmf.ps1

# Debug build
.\build_mmf.ps1 -BuildType debug
```

**Direct execution (works without environment variables):**
```powershell
# Release build (uses default path in script)
.\build_mmf.ps1

# Debug build
.\build_mmf.ps1 -BuildType debug
```

**First-time execution note**: PowerShell execution policy setting may be required:
```powershell
Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope CurrentUser
```

#### Batch File

**Using environment variables (recommended):**
```cmd
# Set environment variable
set ANDROID_NDK_ROOT=C:\path\to\your\android-ndk-r21e

# Release build
build_mmf.bat

# Debug build
build_mmf.bat -d
```

**Direct execution (works without environment variables):**
```cmd
# Release build (uses default path in script)
build_mmf.bat

# Debug build
build_mmf.bat -d
```

### macOS/Linux Environment

**Using environment variables (recommended):**
```bash
# Set environment variable
export ANDROID_NDK_ROOT="/path/to/your/android-ndk-r21e"

# Grant execution permission (first time only)
chmod +x build_mmf.sh

# Release build
./build_mmf.sh

# Debug build
./build_mmf.sh -d
```

**Direct execution (works without environment variables):**
```bash
# Grant execution permission (first time only)
chmod +x build_mmf.sh

# Release build (uses default path in script)
./build_mmf.sh

# Debug build
./build_mmf.sh -d
```

## Prerequisites

### Common Requirements
- Appropriate Android NDK is installed
- Correct Android.mk and Application.mk files are placed in the project root

### Windows Environment
- **NDK Path**: `E:\Android_NDK\android-ndk-r21e`
- If path differs, modify the `ANDROID_NDK_ROOT` variable in the script

### macOS Environment

#### NDK Installation Methods

**1. Via Android Studio (Recommended)**
1. Launch Android Studio
2. `Tools` → `SDK Manager`
3. Check `NDK (Side by side)` in the `SDK Tools` tab
4. Click `Apply` to install

Installation path: `$HOME/Library/Android/sdk/ndk/[version]/`

**2. Direct Download**
```bash
cd /usr/local/share
sudo wget https://dl.google.com/android/repository/android-ndk-r21e-darwin-x86_64.zip
sudo unzip android-ndk-r21e-darwin-x86_64.zip
sudo mv android-ndk-r21e android-ndk
```

**3. Using Homebrew**
```bash
brew install android-ndk
```

#### NDK Path Configuration Examples
Modify the `ANDROID_NDK_ROOT` variable in the `build_mmf.sh` file according to your environment:

```bash
# If installed via Android Studio
ANDROID_NDK_ROOT="$HOME/Library/Android/sdk/ndk/21.4.7075529"

# If installed directly
ANDROID_NDK_ROOT="/usr/local/share/android-ndk"

# If installed via Homebrew
ANDROID_NDK_ROOT="$(brew --prefix)/share/android-ndk"
```

#### How to Check NDK Path
```bash
# If installed via Android Studio
ls -la "$HOME/Library/Android/sdk/ndk/"

# If installed via Homebrew
brew --prefix
ls -la "$(brew --prefix)/share/"
```

### Linux Environment

#### NDK Installation Methods

**1. Download from Official Site (Recommended)**
```bash
cd /opt
sudo wget https://dl.google.com/android/repository/android-ndk-r21e-linux-x86_64.zip
sudo unzip android-ndk-r21e-linux-x86_64.zip
sudo mv android-ndk-r21e android-ndk
sudo chown -R $USER:$USER android-ndk
```

**2. Using Package Manager**
```bash
# Ubuntu/Debian
sudo apt-get install android-tools-ndk

# Arch Linux
sudo pacman -S android-ndk

# Fedora/RHEL
sudo dnf install android-tools
```

#### NDK Path Configuration Examples
```bash
# If installed directly
ANDROID_NDK_ROOT="/opt/android-ndk"

# Or
ANDROID_NDK_ROOT="/usr/local/android-ndk"

# If installed via package manager
ANDROID_NDK_ROOT="/usr/lib/android-ndk"
```

## Output

When build succeeds, libraries are output to the following location:
```
build/Android/mocopi_motion_format/lib/android/arm64-v8a/
├── libmocopi_motion_serializer.so
```

## Supported Platforms

| OS | Script | Status | Notes |
|---|---|---|---|
| Windows 10/11 | `build_mmf.ps1` | ✅ Recommended | Rich error handling |
| Windows 10/11 | `build_mmf.bat` | ✅ Supported | Simple execution |
| macOS | `build_mmf.sh` | ✅ Supported | Intel/Apple Silicon support |
| Linux | `build_mmf.sh` | ✅ Supported | Major distributions |

## Troubleshooting

### Common Issues

#### NDK Not Found
```
Error: Android NDK not found at [path]
```
**Solution**: Modify the `ANDROID_NDK_ROOT` variable in each script to the correct path

#### Build Errors
1. Check Android NDK version compatibility with the project
2. Verify location of Application.mk and Android.mk files
3. Check if required development tools are installed

### Windows-Specific Issues

#### PowerShell Execution Policy Error
```powershell
Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope CurrentUser
```

#### Batch File Hangs
- Recommend using PowerShell script (`build_mmf.ps1`)

### macOS-Specific Issues

#### Xcode Command Line Tools Required
```bash
xcode-select --install
```

#### NDK Not Found
```bash
# Search for NDK
find /Users -name "ndk-build" 2>/dev/null | head -5

# Check Android Studio path
ls -la "$HOME/Library/Android/sdk/ndk/"
```

#### Homebrew Path Issues
```bash
# Check Homebrew prefix
brew --prefix

# Check NDK existence
ls -la "$(brew --prefix)/share/" | grep android
```

### Linux-Specific Issues

#### Missing Dependencies
```bash
# Ubuntu/Debian
sudo apt-get install build-essential

# Fedora/RHEL
sudo dnf groupinstall "Development Tools"

# Arch Linux
sudo pacman -S base-devel
```

#### Permission Issues
```bash
# Change owner of NDK directory
sudo chown -R $USER:$USER /opt/android-ndk

# Or grant execution permission
chmod +x /opt/android-ndk/ndk-build
```

## Performance Comparison

| Script | Execution Speed | Error Handling | Output Quality | Recommendation |
|---|---|---|---|---|
| build_mmf.ps1 | 🟢 Fast | 🟢 Excellent | 🟢 Detailed | ⭐⭐⭐ |
| build_mmf.bat | 🟡 Normal | 🟡 Basic | 🟡 Basic | ⭐⭐ |
| build_mmf.sh | 🟢 Fast | 🟢 Excellent | 🟢 Detailed | ⭐⭐⭐ |

## Notes

- This script builds libraries for arm64-v8a architecture
- If other architectures (x86_64, armeabi-v7a, etc.) are needed, modify APP_ABI in the Application.mk file accordingly
- Correct Android.mk file must be placed in the project root
- Depending on NDK version, API level compatibility issues may occur

## Developer Information

### Changing Build Architecture
To build other architectures, edit the following file:

```makefile
# jni/arm64-v8a/Application_arm64-v8a_release.mk
APP_ABI := arm64-v8a x86_64 armeabi-v7a  # Multiple architectures
# Or
APP_ABI := all  # All architectures
```

### Custom Build Configuration
Detailed NDK build configuration is done in these files:
- `jni/arm64-v8a/Application_arm64-v8a_release.mk` - Application settings
- `jni/arm64-v8a/Android_arm64-v8a.mk` - Build settings
- `Android.mk` - Project root build settings