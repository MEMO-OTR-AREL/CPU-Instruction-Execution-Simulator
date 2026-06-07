#pragma once
#include <string>
#include "Instruction.h"

class ControlUnit {
public:
    std::string decodeInstruction(const Instruction& instruction) const {
        return instruction.opcode;
    }

    std::string generateControlSignals(const Instruction& instruction) const {
        if (instruction.opcode == "LOAD") {
            return "Register Write";
        }

        if (instruction.opcode == "ADD" || instruction.opcode == "SUB") {
            return "ALU Operation + Register Write";
        }

        if (instruction.opcode == "STORE") {
            return "Memory Write";
        }

        if (instruction.opcode == "LOADM") {
            return "Memory Read + Register Write";
        }

        if (instruction.opcode == "PRINT") {
            return "Output Register";
        }

        return "No Operation";
    }
};