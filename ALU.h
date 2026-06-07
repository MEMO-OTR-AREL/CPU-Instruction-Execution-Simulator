#pragma once
#include <string>

class ALU {
public:
    int executeOperation(const std::string& opcode, int value1, int value2) const {
        if (opcode == "ADD") {
            return value1 + value2;
        }

        if (opcode == "SUB") {
            return value1 - value2;
        }

        return value1;
    }
};