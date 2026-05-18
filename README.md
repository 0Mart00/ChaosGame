
```markdown
# ChaosGame

A high-performance, multi-platform C implementation of the Chaos Game algorithm, incorporating hardware-accelerated real-time rendering and low-latency network state synchronization.

---

## 1. Technical Architecture

The application is structured around a decoupled, single-threaded high-performance simulation loop that interlaces frame updates with asynchronous network polling.

### Core Technologies
*   **Language Standard:** ISO C99 / C11 compliant source code.
*   **Graphics Subsystem:** `raylib` — a hardware-accelerated utility wrapper providing an OpenGL abstraction layer for low-overhead primitive rendering.
*   **Networking Subsystem:** `ENet` — a lightweight network library operating over raw UDP sockets, ensuring reliable, in-order packet delivery for deterministic simulation syncing without TCP overhead.
*   **Build Automation:** Native GNU Make deployment matrix.

---
---

## 2. Configuration Matrix

Runtime parameters, network execution contexts, and fractal densities are exposed via internal global matrices or dedicated config tables:

| Parameter | Identifier | Mathematical Symbol | Operational Scope |
| :--- | :--- | :--- | :--- |
| **Vertex Count** | `vertices` | $n$ | Total attractor nodes (e.g., 3: Triangle, 4: Square). |
| **Compression Ratio** | `scale_factor` | $r$ | Step distance multiplier toward the targeted vertex. |
| **Iteration Rate** | `sample_size` | - | Points calculated and pushed to the frame buffer per cycle. |
| **Network Protocol** | `enet_layer` | - | Relays deterministic PRNG seeds to connected clients. |

---

## 3. Cross-Platform Environment Setup

The codebase explicitly requires external development headers for graphics linking and networking sockets.

### Linux (Ubuntu / Debian Architecture)
Install the hardware development wrappers alongside the raw dependency packages:
```bash
sudo apt-get update
sudo apt-get install -y libraylib-dev libenet-dev libx11-dev libxrandr-dev libxi-dev libxinerama-dev libxcursor-dev libgl1-mesa-dev make gcc

```

### macOS (Darwin Architecture)

Deploy runtime links using the Homebrew package manager:

```bash
brew install raylib enet make

```

### Windows (MSYS2 / MinGW-w64 Toolchain)

Open your **MSYS2 MinGW64** terminal instance and execute the package management system tool:

```bash
pacman -S --noconfirm mingw-w64-x86_64-gcc mingw-w64-x86_64-raylib mingw-w64-x86_64-enet make

```

---

## 4. Compilation & Execution

Compilation targets are configured via the native `Makefile` located at the root directory.

### Build Executables

Execute the GNU compiler collection string matching the local operating system:

```bash
make

```

### Runtime Targets

The compilation step produces platform-specific native binaries:

* **POSIX Compliant Environments (Linux / macOS):** `./chaos_rts`
* **Windows Architecture (PE Format):** `chaos_rts.exe`

### Purge Build Artifacts

To wipe object files and enforce a complete recompilation from scratch:

```bash
make clean && make

```
```

The automation pipeline evaluates matrix builds for `ubuntu-latest`, `macos-latest`, and `windows-latest` across isolated VMs. Successful target builds isolate and upload the compilation assets (`chaos_rts` / `chaos_rts.exe`) as immutable workflow build artifacts.

```

```
