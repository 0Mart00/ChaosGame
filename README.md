
```

* **Language Standard:** ISO C99 / C11
* **Graphics subsystem:** `raylib` (OpenGL abstraction layer for responsive input handling and GPU-accelerated primitive rendering).
* **Networking subsystem:** `ENet` (Provides a thin, reliable UDP protocol layer over raw sockets, preserving low latency while ensuring state synchronization).
* **Build Architecture:** GNU Make compilation matrix.

---

## Cross-Platform Compilation

The project features an automated **GitHub Actions CI Pipeline** (`Multi-platform Build`) that builds and verifies code integrity across three major OS platforms on every push or pull request to the `main` branch.

### Prerequisites & Dependency Installation

#### Linux (Ubuntu / Debian derivatives)
Install the development packages for standard X11 window management, OpenGL runtime, raylib, and ENet:
```bash
sudo apt-get update
sudo apt-get install -y libraylib-dev libenet-dev libx11-dev libxrandr-dev libxi-dev libxinerama-dev libxcursor-dev libgl1-mesa-dev make gcc

```

#### macOS

Install dependencies via Homebrew:

```bash
brew install raylib enet make

```

#### Windows (MSYS2 Environment)

Open your **MSYS2 MinGW64** terminal and install the native 64-bit GCC toolchain alongside pre-built libraries:

```bash
pacman -S --noconfirm mingw-w64-x86_64-gcc mingw-w64-x86_64-raylib mingw-w64-x86_64-enet make

```

---

## Execution and Building

To compile the native executable, navigate to the root directory containing the project `Makefile` and run:

```bash
make

```

### Build Artifacts

Upon successful compilation, the build system generates an optimized native binary:

* **Linux / macOS:** `./chaos_rts`
* **Windows:** `chaos_rts.exe`

To clear intermediate object files and rebuild from a clean state:

```bash
make clean
make

```
