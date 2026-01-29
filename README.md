# 🎮 Chip8-core

> **📚 Academic Project Notice**  
> This project was developed as part of an academic semester coursework and served as a valuable learning exercise in cybersecurity, machine learning, and network analysis. While it demonstrates core concepts of intrusion detection systems, **this repository is no longer actively maintained and will not receive future updates**. The code remains available for educational purposes and as a reference for students and researchers interested in ML-powered security solutions.

Big Defend is an Intrusion Detection System (IDS) leveraging **Machine Learning** and **Real-Time Packet Capture**. It captures live network traffic using Scapy, trains an anomaly detection model (using scikit-learn), and monitors network activity for potential threats, presenting controls and logs via a sleek **GUI built with PySide6**.

---
<div align="center">

**A modern C++ implementation of the classic CHIP-8 emulator**

[![Build Status](https://img.shields.io/badge/build-in--progress-yellow.svg)](https://github.com/adawatia/chip8-core)
[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![C++](https://img.shields.io/badge/C%2B%2B-17-orange.svg)](https://isocpp.org/)
[![Platform](https://img.shields.io/badge/platform-Windows%20%7C%20Linux%20%7C%20macOS-lightgrey.svg)](https://github.com/adawatia/chip8-core)
[![Development Status](https://img.shields.io/badge/status-Core%20Complete-brightgreen.svg)](https://github.com/adawatia/chip8-core)

</div>


## 📖 About

Chip8-core is a high-performance, cross-platform CHIP-8 emulator written in modern C++. CHIP-8 is an interpreted programming language that was first used on some microcomputers in the mid-1970s. This emulator brings classic games like Pong, Tetris, and Space Invaders back to life with pixel-perfect accuracy and smooth gameplay.

## 🚧 Current Development Status

**Phase 1: Core Implementation ✅ COMPLETE**
- CPU emulation and instruction set implementation
- Memory management system
- Display buffer handling
- Input processing logic
- Audio system architecture

## ✨ Features

- 🚀 **High Performance**: Optimized C++ core implementation ready for 60 FPS gameplay
- 🎯 **Pixel Perfect**: Accurate emulation of the original CHIP-8 specifications
- 🧠 **Complete Core**: Full CPU, memory, and instruction set implementation
- 🎵 **Audio Ready**: Audio system architecture prepared for SFML integration
- ⌨️ **Input System**: Flexible input handling ready for SFML keyboard integration
- 📱 **Cross Platform**: Designed for Windows, Linux, and macOS compatibility
- 🎨 **Modern UI Ready**: Prepared for ImGui integration with debug capabilities
- 🔧 **Extensible Architecture**: Clean, modular design for easy feature additions
- 🎮 **SFML Powered**: Next-gen graphics and audio through SFML framework

## 🎨 Upcoming Visual Experience

<div align="center">

### 🖥️ Modern Interface Design
> **Next Phase**: Beautiful SFML-powered graphics with ImGui integration

| Component | Technology | Status |
|-----------|------------|--------|
| 🎮 **Core Engine** | Modern C++17 | ✅ **Complete** |
| 🖼️ **Graphics Rendering** | SFML | 🔄 **In Progress** |
| 🎛️ **Debug Interface** | ImGui | 🔄 **In Progress** |
| 🔊 **Audio System** | SFML Audio | 🔄 **In Progress** |

</div>

### Planned Features
- **🌟 Smooth SFML Rendering**: Hardware-accelerated graphics with crisp pixel scaling
- **🎛️ Interactive ImGui Debugger**: Real-time register, memory, and stack visualization  
- **🎨 Customizable Themes**: Multiple color schemes and visual styles
- **🔊 Authentic Audio**: Classic CHIP-8 beep with SFML's audio engine
- **⚡ Real-time Performance**: Live FPS counter and performance metrics

## 🚀 Quick Start

<div align="center">

### 🎯 Current Status: Core Engine Complete!

![Progress](https://progress-bar.dev/60/?title=Development%20Progress&width=400&color=babaca&suffix=%20Complete)

</div>

### Prerequisites

- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
- CMake 3.15 or higher
- SFML development libraries (for upcoming Phase 2)
- ImGui (will be integrated in Phase 2)

### Current Build (Core Engine)

```bash
# 1. Clone the repository
git clone https://github.com/adawatia/chip8-core.git
cd chip8-core

# 2. Build the core engine
mkdir build && cd build
cmake ..
make -j4

# 3. Run comprehensive tests
./tests/chip8_tests
```

### 🔮 Coming Soon (Phase 2 - SFML/ImGui Integration)

Once Phase 2 is complete, you'll experience:

```bash
# 🎮 Load and run ROMs with beautiful GUI
./chip8-core games/pong.ch8

# 🎨 Start with modern ROM browser interface  
./chip8-core --gui

# 🔧 Launch with integrated debugger
./chip8-core --debug games/space_invaders.ch8
```

## 🎮 Usage (Preview)

<div align="center">

### 🎯 Default CHIP-8 Keypad Mapping

| **CHIP-8** | **Keyboard** | | **CHIP-8** | **Keyboard** |
|:----------:|:------------:|-|:----------:|:------------:|
| `1` `2` `3` `C` | `1` `2` `3` `4` | | `4` `5` `6` `D` | `Q` `W` `E` `R` |
| `7` `8` `9` `E` | `A` `S` `D` `F` | | `A` `0` `B` `F` | `Z` `X` `C` `V` |

</div>

### 🚀 Planned Command Line Interface

```bash
# 🎮 Basic ROM execution
./chip8-core [ROM_FILE]

# 🎨 Advanced options (coming soon)
./chip8-core --scale 10 --theme retro --debug games/tetris.ch8

# 📊 Performance profiling  
./chip8-core --profile --fps-counter games/pong.ch8

# 🎵 Audio options
./chip8-core --volume 0.8 --audio-buffer 512 games/invaders.ch8
```

### ⚙️ Future Configuration System

```ini
[Display]
scale = 10
fullscreen = false  
theme = neon_green
vsync = true

[Audio]
enabled = true
volume = 0.7
sample_rate = 44100

[Debug]  
show_registers = true
show_memory = true
step_mode = false

[Performance]
target_fps = 60
cpu_frequency = 500
```

## 🏗️ Architecture Overview

<div align="center">

### 🧠 Clean, Modular Design

```
┌─────────────────────────────────────────────────────────┐
│                    🎮 CHIP8-CORE                        │
├─────────────────────────────────────────────────────────┤
│  Phase 1: ✅ CORE ENGINE (Complete)                     │
│  ┌─────────────┐ ┌─────────────┐ ┌─────────────┐        │
│  │   🧠 CPU    │ │ 💾 MEMORY   │ │ 📺 DISPLAY  │        │
│  │   Engine    │ │  Manager    │ │   Buffer    │        │
│  └─────────────┘ └─────────────┘ └─────────────┘        │
│  ┌─────────────┐ ┌─────────────┐ ┌─────────────┐        │
│  │ ⌨️ INPUT    │ │ 🔊 AUDIO    │ │ 🎲 TIMER    │        │
│  │  Handler    │ │  System     │ │  System     │        │
│  └─────────────┘ └─────────────┘ └─────────────┘        │
├─────────────────────────────────────────────────────────┤
│  Phase 2: 🔄 UI INTEGRATION (In Progress)               │
│  ┌─────────────┐ ┌─────────────┐ ┌─────────────┐        │
│  │ 🖼️ SFML     │ │ 🎛️ ImGui    │ │ 🎨 RENDERER │        │
│  │ Graphics    │ │ Interface   │ │   Engine    │        │
│  └─────────────┘ └─────────────┘ └─────────────┘        │
└─────────────────────────────────────────────────────────┘
```

</div>

### 📁 Project Structure

```
chip8-core/
├── 🧠 src/core/           # ✅ Core emulation engine
│   ├── cpu.cpp           # Complete CPU implementation
│   ├── memory.cpp        # Memory management system  
│   ├── display.cpp       # Display buffer handling
│   ├── input.cpp         # Input processing logic
│   └── audio.cpp         # Audio system foundation
├── 🎨 src/ui/             # 🔄 UI layer (Phase 2)
│   ├── sfml_renderer.cpp # SFML graphics integration
│   └── imgui_interface.cpp # Debug UI implementation
├── 📂 include/            # Header files
├── 🎮 games/              # Sample ROM collection
├── 🧪 tests/              # Comprehensive test suite
└── 📚 docs/               # Technical documentation
```

## 🧪 Testing

Run the test suite to ensure everything works correctly:

```bash
cd build
make test

# Or run specific tests
./tests/cpu_tests
./tests/memory_tests
```

## 🤝 Contributing

Contributions are welcome! Please feel free to submit a Pull Request. For major changes, please open an issue first to discuss what you would like to change.

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

### Development Guidelines

- Follow the existing code style
- Add tests for new features
- Update documentation as needed
- Ensure all tests pass before submitting

## 📚 Resources

- [CHIP-8 Technical Reference](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM)
- [CHIP-8 Wikipedia](https://en.wikipedia.org/wiki/CHIP-8)
- [Awesome CHIP-8](https://github.com/tobiasvl/awesome-chip-8)

## 📝 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

- Original CHIP-8 designers for creating this iconic system
- The emulation community for their detailed documentation
- Contributors and testers who helped improve this project

## 📊 Project Stats

![GitHub stars](https://img.shields.io/github/stars/adawatia/chip8-core.svg?style=social)
![GitHub forks](https://img.shields.io/github/forks/adawatia/chip8-core.svg?style=social)
![GitHub issues](https://img.shields.io/github/issues/adawatia/chip8-core.svg)
![GitHub last commit](https://img.shields.io/github/last-commit/adawatia/chip8-core.svg)

---

<div align="center">
Made with ❤️ by <a href="https://github.com/adawatia">adawatia</a>
</div>
