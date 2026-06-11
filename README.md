# CPU Instruction Execution Simulator

This project is a visual CPU instruction execution simulator developed in C++ with Visual Studio.

The simulator demonstrates how a simplified CPU executes instructions step by step using the Fetch, Decode, Execute, and Write Back cycle.

## Project Purpose

The purpose of this project is to visually simulate the basic working logic of a CPU.
It shows how instructions are loaded from memory, decoded by the control unit, processed by the ALU, and written back to registers or memory.

## Main Components

- CPU
- Instruction Memory
- Data Memory
- Control Unit
- ALU
- Register File
- Instruction Register
- Program Counter

## Supported Instructions

| Instruction | Description |
|---|---|
| LOAD | Loads a value into a register |
| ADD | Adds two register values |
| SUB | Subtracts two register values |
| STORE | Stores register value into memory |
| LOADM | Loads value from memory into register |
| PRINT | Displays register value in the execution log |

## Simulation Cycle

1. Fetch Instruction
2. Decode Instruction
3. Execute Operation
4. Write Back

## Controls

| Key | Action |
|---|---|
| R | Run / Pause simulation |
| Space | Step simulation |
| Backspace | Reset CPU |
| Esc | Exit program |

The visual buttons on the simulator screen can also be used:

- RUN
- STEP
- RESET

## Technologies Used

- C++
- Visual Studio 2022
- Windows API
- GDI Drawing

## Project Structure

```text
CPUProject
|-- Kaynak.cpp
|-- Instruction.h
|-- Memory.h
|-- RegisterFile.h
|-- ALU.h
|-- ControlUnit.h
|-- CPU.h
|-- Renderer.h
|-- README.md
```

## Author

Mehmet Ali Oter
