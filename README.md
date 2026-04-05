# ARM to Hack Translator

A two-pass ARM assembly to Hack assembly translator written in C++.

---

## What it does

Takes ARM assembly code as input and translates it into Hack assembly — the instruction set used in the [Nand2Tetris](https://www.nand2tetris.org/) course.

### Example

**Input** (`program.arm`):
```
MOV R0, R1
```

**Output** (`program.hack`):
```
@1
D=M
@0
M=D
```

---

## Supported Instructions

| ARM | Description |
|-----|-------------|
| `MOV` | Copy value between registers or load immediate |
| `ADD` | Addition |
| `SUB` | Subtraction |
| `RSB` | Reverse subtract |
| `CMP` | Compare two values |
| `BNE` | Branch if not equal |
| `BL` | Branch and link (function call) |
| `BAL` | Branch always |
| `END` | Terminate program |

---

## How it works

The translator uses a **two-pass approach**:

1. **First pass** — scans ARM instructions, builds a label-to-address map, and marks unresolved forward references
2. **Second pass** — resolves all forward references by replacing placeholders with the correct label addresses

---

## Build & Run

### Compile
```bash
clang++ -std=c++14 -arch arm64 -o ArmToHack ArmToHack.cpp token_io.cpp main.cpp
```

### Run
```bash
./ArmToHack
```

This translates all 10 test programs in the `test/` directory:
- Input: `test/program1.arm` ... `test/program10.arm`
- Output: `test/program1.hack` ... `test/program10.hack`

---

## Project Structure

```
├── ArmToHack.h       # Class definition
├── ArmToHack.cpp     # Translation logic (two-pass)
├── token_io.h        # Token I/O helpers
├── token_io.cpp
├── main.cpp          # Entry point — runs all 10 test files
└── test/
    ├── program1.arm  # ARM input files
    ├── program1.hack # Translated Hack output
    └── ...
```
