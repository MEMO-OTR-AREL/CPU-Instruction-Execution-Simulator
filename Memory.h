#pragma once
#include <vector>
#include "Instruction.h"

class Memory {
private:
    std::vector<Instruction> instructionMemory;
    std::vector<int> dataMemory;

public:
    Memory() {
        dataMemory.resize(8, 0);
    }

    void loadProgram(const std::vector<Instruction>& program) {
        instructionMemory = program;
    }

    Instruction readInstruction(int address) const {
        if (address >= 0 && address < (int)instructionMemory.size()) {
            return instructionMemory[address];
        }

        return { "END", 0, 0, "END" };
    }

    int readData(int address) const {
        if (address >= 0 && address < (int)dataMemory.size()) {
            return dataMemory[address];
        }

        return 0;
    }

    void writeData(int address, int value) {
        if (address >= 0 && address < (int)dataMemory.size()) {
            dataMemory[address] = value;
        }
    }

    const std::vector<Instruction>& getInstructions() const {
        return instructionMemory;
    }

    const std::vector<int>& getDataMemory() const {
        return dataMemory;
    }

    int programSize() const {
        return (int)instructionMemory.size();
    }
};