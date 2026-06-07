#pragma once
#include <vector>

class RegisterFile {
private:
    std::vector<int> registers;

public:
    RegisterFile() {
        registers.resize(8, 0);
    }

    int getRegister(int index) const {
        if (index >= 0 && index < (int)registers.size()) {
            return registers[index];
        }

        return 0;
    }

    void setRegister(int index, int value) {
        if (index >= 0 && index < (int)registers.size()) {
            registers[index] = value;
        }
    }

    void reset() {
        for (int& value : registers) {
            value = 0;
        }
    }

    const std::vector<int>& getAllRegisters() const {
        return registers;
    }
};