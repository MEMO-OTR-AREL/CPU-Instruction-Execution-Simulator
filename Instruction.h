#pragma once
#include <string>

struct Instruction {
    std::string opcode;
    int operand1;
    int operand2;
    std::string rawText;
};
