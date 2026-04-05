# ARM to Hack Translator

A two-pass ARM assembly to Hack assembly translator written in C++.

---

## What it does

Takes ARM assembly code as input and translates it into Hack assembly — the instruction set used in the [Nand2Tetris](https://www.nand2tetris.org/) course.

### Example

This ARM program initializes registers and runs a loop, accumulating a sum until a counter reaches 5:

**Input** (`program5.arm`):
```asm
MOV   R0, #5
MOV   R1, #0
MOV   R2, #1
LOOP
ADD   R1, R1, R2
ADD   R2, R2, #1
CMP   R2, #5
BLE   LOOP
END
```

**Output** (`program5.hack`):
```asm
@5
D=A
@0
M=D
@0
D=A
@1
M=D
@1
D=A
@2
M=D
@2
D=M
@1
D=D+M
@1
M=D
@1
D=A
@2
D=D+M
@2
M=D
@5
D=A
@2
D=M-D
@12
D;JLE
@31
0;JMP
```

The translator correctly resolves the `LOOP` label to address `12` and generates the conditional jump `D;JLE` from `BLE`, demonstrating label resolution and branch translation.

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
