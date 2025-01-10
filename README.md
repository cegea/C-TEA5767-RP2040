# C-TEA5767-RP2040
TEA5767 C driver for RP2040

## Prerequisites

### Instal Raspberry Pi Pico Extension for VSCODE
Not necessary but it helps with `IntelliSense`.

### Install toolchain

#### Debian / Ubuntu

```bash
sudo apt install cmake gcc-arm-none-eabi libnewlib-arm-none-eabi libstdc++-arm-none-eabi-newlib
```

#### Fedora

```bash
sudo dnf install gcc-arm-linux-gnu \
 arm-none-eabi-gcc-cs-c++ \
 arm-none-eabi-gcc-cs \
 arm-none-eabi-binutils \
 arm-none-eabi-newlib \
 g++
```

## Build project

```bash
mkdir build
cd build
cmake ..
make
```
