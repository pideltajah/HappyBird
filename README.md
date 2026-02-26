HappyBird
=========

A Flappy Bird inspired game

This game is written in C++ and utilizes the SFML 3 API. The game starts immediately upon execution. The user
left-clicks to "flap" and elevate the bird and right-clicks to dive which accelerates the rate at which the bird
falls. Obstacles must be navigated to gain points and if hit, the bird dies and falls to the ground. The player
gets three lives before it is game over.

## Building and Running

### Prerequisites

| Tool | Windows | Linux |
|---|---|---|
| C++ compiler | MinGW-w64 (extract to `C:/MinGW`, add `C:/MinGW/bin` to PATH) | `g++` via package manager |
| CMake | [cmake.org/download](https://cmake.org/download) | `sudo apt install cmake` |
| Ninja | [github.com/ninja-build/ninja/releases](https://github.com/ninja-build/ninja/releases) | `sudo apt install ninja-build` |
| SFML 3.0 | Download MinGW GCC 64-bit build from [sfml-dev.org](https://sfml-dev.org/download.php), extract to `C:/SFML` | `sudo apt install libsfml-dev` (or build from source if SFML 3 is not yet in your distro's repos) |

### Build Steps

```bash
git clone https://github.com/pideltajah/HappyBird.git
cd HappyBird
mkdir build && cd build
cmake .. -G Ninja
ninja
```

Then run:
- **Windows:** `./HappyBird.exe`
- **Linux:** `./HappyBird`

### Overriding the SFML path (Windows)

If SFML is installed somewhere other than `C:/SFML`, pass the path at configure time:

```bash
cmake .. -G Ninja -DSFML_DIR="D:/libs/SFML/lib/cmake/SFML"
```
