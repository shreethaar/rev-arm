# Cross-Compilation Notes: ARM64 on x86

A comprehensive guide for cross-compiling ARM64 binaries on x86 systems using `aarch64-linux-gnu-gcc`.

## Table of Contents
- [Installation](#installation)
- [Basic Usage](#basic-usage)
- [C++ Programs](#c-programs)
- [Common Compilation Options](#common-compilation-options)
- [Library Linking](#library-linking)
- [Testing ARM64 Binaries](#testing-arm64-binaries)
- [Troubleshooting QEMU Issues](#troubleshooting-qemu-issues)
- [Verification](#verification)
- [Best Practices](#best-practices)

## Installation

### Ubuntu/Debian
```bash
sudo apt update
sudo apt install gcc-aarch64-linux-gnu
```

### Fedora/RHEL
```bash
sudo dnf install gcc-aarch64-linux-gnu
```

## Basic Usage

Use `aarch64-linux-gnu-gcc` as a drop-in replacement for `gcc`:

```bash
# Simple compilation
aarch64-linux-gnu-gcc -o hello_arm64 hello.c

# With optimization
aarch64-linux-gnu-gcc -O2 -o hello_arm64 hello.c

# Separate compilation and linking
aarch64-linux-gnu-gcc -c -o hello.o hello.c
aarch64-linux-gnu-gcc -o hello_arm64 hello.o
```

## C++ Programs

For C++ code, use the C++ cross-compiler:

```bash
aarch64-linux-gnu-g++ -o program_arm64 program.cpp
```

## Common Compilation Options

```bash
# Static linking (recommended for testing)
aarch64-linux-gnu-gcc -static -o hello_arm64 hello.c

# Debug symbols
aarch64-linux-gnu-gcc -g -o hello_arm64 hello.c

# Specify ARM64 architecture explicitly
aarch64-linux-gnu-gcc -march=armv8-a -o hello_arm64 hello.c

# Optimization levels
aarch64-linux-gnu-gcc -O0  # No optimization
aarch64-linux-gnu-gcc -O1  # Basic optimization
aarch64-linux-gnu-gcc -O2  # Standard optimization
aarch64-linux-gnu-gcc -O3  # Aggressive optimization
```

## Library Linking

For programs requiring external libraries:

```bash
# Install ARM64 development libraries
sudo apt install libc6-dev-arm64-cross

# Link with math library
aarch64-linux-gnu-gcc -o program program.c -lm

# Link with multiple libraries
aarch64-linux-gnu-gcc -o program program.c -lm -lpthread
```

## Testing ARM64 Binaries

### Install QEMU
```bash
sudo apt install qemu-user qemu-user-static
```

### Method 1: Static Binaries (Recommended)
```bash
# Compile with static linking
aarch64-linux-gnu-gcc -static -o program program.c

# Run directly
qemu-aarch64 ./program
```

### Method 2: Dynamic Binaries with Library Path
```bash
# Install ARM64 libraries
sudo apt install libc6-arm64-cross

# Run with library path
qemu-aarch64 -L /usr/aarch64-linux-gnu ./program
```

## Troubleshooting QEMU Issues

### Problem: "Could not open '/lib/ld-linux-aarch64.so.1'"

This error occurs with dynamically linked binaries. Here are the solutions:

#### Solution 1: Static Linking (Easiest)
```bash
aarch64-linux-gnu-gcc -static -o program program.c
qemu-aarch64 ./program
```

#### Solution 2: Install ARM64 Libraries
```bash
sudo apt install libc6-arm64-cross
qemu-aarch64 -L /usr/aarch64-linux-gnu ./program
```

#### Solution 3: Use qemu-aarch64-static
```bash
qemu-aarch64-static ./program
```

#### Solution 4: Enable Multiarch (Ubuntu/Debian)
```bash
sudo dpkg --add-architecture arm64
sudo apt update
sudo apt install libc6:arm64
qemu-aarch64 ./program
```

#### Solution 5: Manual Library Path
```bash
# Find ARM64 libraries
find /usr -name "ld-linux-aarch64.so.1" 2>/dev/null

# Use the found path
qemu-aarch64 -L /usr/aarch64-linux-gnu ./program
```

## Verification

### Check Binary Architecture
```bash
file program_arm64
# Expected output: program_arm64: ELF 64-bit LSB executable, ARM aarch64...

# More detailed info
readelf -h program_arm64 | grep Machine
# Expected: Machine: AArch64
```

### Check Dependencies
```bash
# For dynamically linked binaries
aarch64-linux-gnu-readelf -d program_arm64

# Check if static
file program_arm64 | grep static
```

## Best Practices

### For Development and Testing
1. **Use static linking** for simple testing: `-static`
2. **Always verify** the binary architecture with `file` command
3. **Test with QEMU** before deploying to ARM64 systems

### For Production
1. **Use dynamic linking** to reduce binary size
2. **Optimize appropriately**: `-O2` for most cases
3. **Include debug symbols** during development: `-g`
4. **Strip symbols** for production: `aarch64-linux-gnu-strip`

### Common Workflows

#### Development Workflow
```bash
# Compile with debug info
aarch64-linux-gnu-gcc -g -static -o program program.c

# Test
qemu-aarch64 ./program

# Debug if needed
gdb-multiarch ./program
```

#### Production Workflow
```bash
# Compile optimized
aarch64-linux-gnu-gcc -O2 -o program program.c

# Strip symbols
aarch64-linux-gnu-strip program

# Verify
file program
```

## Environment Variables

Useful environment variables for cross-compilation:

```bash
export CC=aarch64-linux-gnu-gcc
export CXX=aarch64-linux-gnu-g++
export AR=aarch64-linux-gnu-ar
export STRIP=aarch64-linux-gnu-strip

# For autotools-based projects
export HOST=aarch64-linux-gnu
```

## Makefile Example

```makefile
# Cross-compilation Makefile
CC = aarch64-linux-gnu-gcc
CXX = aarch64-linux-gnu-g++
CFLAGS = -O2 -Wall
LDFLAGS = -static

SOURCES = main.c utils.c
OBJECTS = $(SOURCES:.c=.o)
TARGET = program_arm64

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

test: $(TARGET)
	qemu-aarch64 ./$(TARGET)

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: test clean
```

## Notes

- ARM64 and AArch64 refer to the same architecture
- The toolchain produces Linux binaries - won't work for bare metal without additional setup
- For bare metal or embedded development, you might need `aarch64-none-elf-gcc` instead
- Always test your cross-compiled binaries on the target platform when possible

## Quick Reference

| Task | Command |
|------|---------|
| Basic compile | `aarch64-linux-gnu-gcc -o prog prog.c` |
| Static compile | `aarch64-linux-gnu-gcc -static -o prog prog.c` |
| C++ compile | `aarch64-linux-gnu-g++ -o prog prog.cpp` |
| Check architecture | `file prog` |
| Run with QEMU | `qemu-aarch64 ./prog` |
| Run with libs | `qemu-aarch64 -L /usr/aarch64-linux-gnu ./prog` |