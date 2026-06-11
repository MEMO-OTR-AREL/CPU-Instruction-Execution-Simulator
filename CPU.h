#pragma once
#include <string>
#include <vector>
#include "Instruction.h"
#include "Memory.h"
#include "RegisterFile.h"
#include "ALU.h"
#include "ControlUnit.h"

class CPU {
private:
    int pc;
    int phase;
    Instruction ir;

    Memory memory;
    RegisterFile registerFile;
    ALU alu;
    ControlUnit controlUnit;

    std::vector<std::string> logs;

public:
    CPU() {
        reset();
    }

    void loadDefaultProgram() {
        std::vector<Instruction> program;

        program.push_back({ "LOAD", 1, 10, "LOAD R1 10" });
        program.push_back({ "LOAD", 2, 20, "LOAD R2 20" });
        program.push_back({ "ADD", 1, 2, "ADD R1 R2" });
        program.push_back({ "STORE", 1, 5, "STORE R1 5" });
        program.push_back({ "LOAD", 3, 8, "LOAD R3 8" });
        program.push_back({ "SUB", 1, 3, "SUB R1 R3" });
        program.push_back({ "PRINT", 1, 0, "PRINT R1" });
        program.push_back({ "LOADM", 4, 5, "LOADM R4 5" });
        program.push_back({ "PRINT", 4, 0, "PRINT R4" });

        memory.loadProgram(program);
        addLog("Program loaded into instruction memory.");
    }

    void reset() {
        pc = 0;
        phase = 0;
        ir = { "NONE", 0, 0, "NONE" };
        registerFile.reset();
        memory.resetData();
        logs.clear();

        addLog("CPU reset completed.");
    }

    void addLog(const std::string& text) {
        logs.push_back(text);

        if (logs.size() > 11) {
            logs.erase(logs.begin());
        }
    }

    void step() {
        if (pc >= memory.programSize()) {
            addLog("CPU halted. Program finished.");
            return;
        }

        if (phase == 0) {
            fetch();
            phase = 1;
        }
        else if (phase == 1) {
            decode();
            phase = 2;
        }
        else if (phase == 2) {
            execute();
            phase = 3;
        }
        else {
            writeBack();
            phase = 0;
        }
    }

    void fetch() {
        ir = memory.readInstruction(pc);
        addLog("FETCH: PC=" + std::to_string(pc) + " -> " + ir.rawText);
    }

    void decode() {
        std::string opcode = controlUnit.decodeInstruction(ir);
        std::string signals = controlUnit.generateControlSignals(ir);

        addLog("DECODE: opcode=" + opcode);
        addLog("SIGNALS: " + signals);
    }

    void execute() {
        if (ir.opcode == "LOAD") {
            registerFile.setRegister(ir.operand1, ir.operand2);
            addLog("EXECUTE: R" + std::to_string(ir.operand1) + " <- " + std::to_string(ir.operand2));
        }
        else if (ir.opcode == "ADD" || ir.opcode == "SUB") {
            int value1 = registerFile.getRegister(ir.operand1);
            int value2 = registerFile.getRegister(ir.operand2);
            int result = alu.executeOperation(ir.opcode, value1, value2);

            registerFile.setRegister(ir.operand1, result);

            addLog("EXECUTE: ALU " + ir.opcode + " result = " + std::to_string(result));
        }
        else if (ir.opcode == "STORE") {
            int value = registerFile.getRegister(ir.operand1);
            memory.writeData(ir.operand2, value);

            addLog("MEMORY WRITE: M[" + std::to_string(ir.operand2) + "] <- R" + std::to_string(ir.operand1));
        }
        else if (ir.opcode == "LOADM") {
            int value = memory.readData(ir.operand2);
            registerFile.setRegister(ir.operand1, value);

            addLog("MEMORY READ: R" + std::to_string(ir.operand1) + " <- M[" + std::to_string(ir.operand2) + "]");
        }
        else if (ir.opcode == "PRINT") {
            int value = registerFile.getRegister(ir.operand1);

            addLog("OUTPUT: R" + std::to_string(ir.operand1) + " = " + std::to_string(value));
        }
    }

    void writeBack() {
        addLog("WRITE BACK: PC updated.");
        pc++;
    }

    std::string getPhaseName() const {
        if (phase == 0) return "FETCH INSTRUCTION";
        if (phase == 1) return "DECODE INSTRUCTION";
        if (phase == 2) return "EXECUTE OPERATION";
        return "WRITE BACK";
    }

    int getPC() const {
        return pc;
    }

    int getPhase() const {
        return phase;
    }

    Instruction getIR() const {
        return ir;
    }

    const Memory& getMemory() const {
        return memory;
    }

    const RegisterFile& getRegisterFile() const {
        return registerFile;
    }

    const std::vector<std::string>& getLogs() const {
        return logs;
    }

    bool isFinished() const {
        return pc >= memory.programSize();
    }
};