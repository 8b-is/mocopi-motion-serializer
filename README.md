# mocopi Motion Serializer

[![License](https://img.shields.io/badge/License-Apache%202.0-blue.svg)](./LICENSE)

mocopi Motion Serializer is a cross-platform library for mocopi-related applications. Provides motion data serialization functionality across multiple platforms.

## License Notice
- This project is licensed under the Apache License 2.0 - see the [LICENSE](./LICENSE) file for details.
- Notwithstanding the foregoing, this repository does not include the mocopi logo or application icons. Use of these assets requires entering into a separate mocopi Logo and Icon License Agreement. ([here](https://www.sony.co.jp/en/Products/mocopi-dev/en/others/LogoGuideline.html))

## Overview

The mocopi Motion Serializer is a library designed for developing mocopi-related applications. It provides mocopi motion data serialization and deserialization functionality across multiple platforms, enabling efficient transmission and processing of motion capture data obtained from mocopi systems.

### Key Features

- Cross-platform motion data serialization
- Efficient binary format optimized for storage and transmission
- UTC timestamp and timecode support
- Thread-safe operations


## Supported Platforms

This library supports the following operating systems and platforms:

- **Windows** (x64)
- **macOS** (Universal Binary - Intel & Apple Silicon)
- **iOS** (arm64)
- **Android** (arm64-v8a, x86_64)

## Build Instructions

### Prerequisites

Each platform requires specific development tools and SDKs:

- **Windows**: Visual Studio 2019 or later
- **macOS/iOS**: Xcode with iOS SDK
- **Android**: Android NDK r21e or later

### Platform-Specific Build Guide

#### Windows
Open the Visual Studio solution:
```
build/Windows/mocopi_motion_serializer.sln
```

#### macOS
Open the Xcode project:
```
build/macOS/mocopi_motion_serializer.xcodeproj
```

#### iOS
Open the Xcode project:
```
build/iOS/mocopi_motion_serializer.xcodeproj
```

#### Android
For detailed Android build instructions, refer to the build guide: [Android Build Guide](build/Android/README.md)

Quick start:
```bash
cd build/Android/
# When using Windows PowerShell (recommended)
.\build_mmf.ps1
# When using macOS/Linux
chmod +x build_mmf.sh && ./build_mmf.sh
```

## Project Structure

```
├── main/                     # Core library source code
├── rust/                     # Zero-dependency #![forbid(unsafe_code)] Rust serializer & mocopi-wupz-pq (4.45x smaller UDP/BLE frames)
├── build/                    # Platform-specific build configurations
│   ├── Android/             # Android NDK build scripts and projects
│   ├── iOS/                 # iOS Xcode projects
│   ├── macOS/               # macOS Xcode projects
│   └── Windows/             # Visual Studio solutions
└── jni/                     # Android JNI configurations
```

### Rust `mocopi-wupz-pq` Crate (`rust/`)

In addition to the C++ `smf1` library in `main/`, the `rust/` directory provides a zero-dependency, `#![forbid(unsafe_code)]` Rust crate (`mocopi-wupz-pq`) implementing both standard `smf1` Box serialization and the ultra-compact **`mwz1` (`wupz` + `p/q` Smallest-Three $S^3$ Quaternion + 12-bit Omni `BFP` Position)** wire format:
- **Smallest-Three $S^3$ Quaternion `p/q` (`QuatSmallestThreePq`, `6 bytes` vs `16 bytes` FP32)**: Stores the 2-bit index of the largest quaternion component + three 15-bit signed rationals over $Q_{S^3} = 23170 \approx \lfloor 16383\sqrt{2}\rfloor$ + 1-bit parity, reconstructing the 4th component via exact integer `isqrt_u64` (measured worst-case angular error `0.005°`).
- **4.45× Frame Compression for Unfragmented UDP & BLE**: Compresses a full 27-bone mocopi frame from **`1,605 bytes` (`smf1`)** down to **`361 bytes` (`mwz1`)**, including an 8-byte `wupz` bitplane XOR-fold integrity seal — fitting **4 full 27-bone mocopi frames inside a single `1,472-byte` UDP payload** without IPv4 fragmentation.
- Run tests: `cargo test --manifest-path rust/Cargo.toml --release`

## Support

For technical support and questions, please join the following Discord server:

**Discord**: https://discord.gg/k55wY45y5N

Also refer to the documentation in the `build/` directory for platform-specific build instructions.

## Resources

- **mocopi Official Developer Site**: [https://sony.net/mocopi-dev/](https://sony.net/mocopi-dev/)

---

**Copyright © 2025 Sony Corporation. All rights reserved.**