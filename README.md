# Simple Calculator App
A minimal, decluttered calculator app built in **C** using **GTK4**.
Compiled using the corresponding **Makefile**.

## Purpose
- To learn App Development through **GTK**
- To do UI in **C**
- To learn about **make**
- For more experience with **GitHub**

## Features
- Minimalistic UI
- Simple operations: +, -, *, /, %
- Supports Backspace and Clear (↵ and C)
- Supports integer-based operations

## Screenshot
[Simple Calculator Screenshot](graphics/screenshot.png)

## Prerequisites
- **GTK4** development libraries
- GCC
- pkg-config
- make (optional)

## Build
Open your terminal and follow the instructions given below:

**With make**

- Run `make` to build the corresponding app
- `make clean` to remove built files

**Without make**

```gcc $( pkg-config --cflags gtk4 ) -o calc src/main.c src/calc.c $( pkg-config --libs gtk4 )```

## Planned Features
- Keyboard input integration
- Floating-point operations
- History panel
- Undo button