# mathd

**`mathd`** is a lightweight, offline functional graphing calculator written in modern C++. It uses a CLI-based interface with powerful function parsing, computation, and plotting capabilities — built to be fast, modular, and free from bloat.

> Think Desmos, but offline and terminal-native.

---

## 🚀 Features

- 📈 Plot mathematical functions via terminal (ASCII or GUI frontend planned)
- 📐 Supports common operations: `+, -, *, /, ^`, trigonometric and logarithmic functions
- 🧮 Parse expressions like `f(x) = sin(x) + log(x^2)`
- ⚙️ CLI input via [CLI11](https://github.com/CLIUtils/CLI11)
- 🧩 Modular structure — computation handled via class-based C++ headers
- 💻 Fast build with CMake, cross-platform (Linux tested, Windows soon™)

---

## 🧰 Requirements

- C++17 or newer
- [CLI11](https://github.com/CLIUtils/CLI11) (included or submodule)
- [CMake](https://cmake.org/) >= 4.0

---

## 🛠️ Building

```bash
git clone https://github.com/yourname/mathd.git
cd mathd
mkdir build && cd build
cmake ..
make
