#pragma once
#include <windows.h>
#include <string>
#include <cmath>
#include "CPU.h"

struct Box {
    int x;
    int y;
    int w;
    int h;
};

class Renderer {
private:
    HWND window;
    CPU* cpu;
    float animation;

    Box inputBox{ 35, 160, 190, 120 };
    Box instrMemBox{ 500, 160, 200, 120 };
    Box cpuChipBox{ 750, 130, 470, 360 };
    Box controlUnitBox{ 795, 280, 200, 82 };
    Box aluBox{ 795, 392, 200, 78 };
    Box registerBox{ 1030, 305, 160, 118 };

    Box instructionPanel{ 35, 510, 310, 220 };
    Box registerPanel{ 370, 510, 250, 220 };
    Box memoryPanel{ 645, 510, 180, 220 };
    Box logPanel{ 850, 510, 360, 220 };

public:
    Renderer() {
        window = NULL;
        cpu = NULL;
        animation = 0.0f;
    }

    void setWindow(HWND hwnd) {
        window = hwnd;
    }

    void setCPU(CPU* cpuPointer) {
        cpu = cpuPointer;
    }

    void resetAnimation() {
        animation = 0.0f;
    }

    void updateAnimation(bool running) {
        if (!running) {
            return;
        }

        animation += 0.035f;

        if (animation >= 1.0f) {
            animation = 0.0f;

            if (cpu != NULL) {
                cpu->step();
            }
        }
    }

    COLORREF color(int r, int g, int b) {
        return RGB(r, g, b);
    }

    void drawText(HDC hdc, int x, int y, const std::string& textValue, COLORREF textColor, int size = 16, bool bold = false) {
        SetTextColor(hdc, textColor);
        SetBkMode(hdc, TRANSPARENT);

        HFONT font = CreateFontA(
            size,
            0,
            0,
            0,
            bold ? FW_BOLD : FW_NORMAL,
            FALSE,
            FALSE,
            FALSE,
            ANSI_CHARSET,
            OUT_DEFAULT_PRECIS,
            CLIP_DEFAULT_PRECIS,
            DEFAULT_QUALITY,
            FF_DONTCARE,
            "Consolas"
        );

        HFONT oldFont = (HFONT)SelectObject(hdc, font);
        TextOutA(hdc, x, y, textValue.c_str(), (int)textValue.size());
        SelectObject(hdc, oldFont);
        DeleteObject(font);
    }

    void drawCenteredText(HDC hdc, Box box, const std::string& textValue, COLORREF textColor, int size = 16, bool bold = false) {
        SetTextColor(hdc, textColor);
        SetBkMode(hdc, TRANSPARENT);

        HFONT font = CreateFontA(
            size,
            0,
            0,
            0,
            bold ? FW_BOLD : FW_NORMAL,
            FALSE,
            FALSE,
            FALSE,
            ANSI_CHARSET,
            OUT_DEFAULT_PRECIS,
            CLIP_DEFAULT_PRECIS,
            DEFAULT_QUALITY,
            FF_DONTCARE,
            "Consolas"
        );

        HFONT oldFont = (HFONT)SelectObject(hdc, font);

        SIZE textSize;
        GetTextExtentPoint32A(hdc, textValue.c_str(), (int)textValue.size(), &textSize);

        int x = box.x + (box.w - textSize.cx) / 2;
        int y = box.y + (box.h - textSize.cy) / 2;

        TextOutA(hdc, x, y, textValue.c_str(), (int)textValue.size());

        SelectObject(hdc, oldFont);
        DeleteObject(font);
    }

    void drawBox(HDC hdc, Box box, COLORREF fillColor, COLORREF borderColor, int borderWidth = 2) {
        HBRUSH brush = CreateSolidBrush(fillColor);
        HPEN pen = CreatePen(PS_SOLID, borderWidth, borderColor);

        HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, brush);
        HPEN oldPen = (HPEN)SelectObject(hdc, pen);

        RoundRect(hdc, box.x, box.y, box.x + box.w, box.y + box.h, 14, 14);

        SelectObject(hdc, oldBrush);
        SelectObject(hdc, oldPen);

        DeleteObject(brush);
        DeleteObject(pen);
    }

    void drawLine(HDC hdc, int x1, int y1, int x2, int y2, COLORREF lineColor, int width = 2) {
        HPEN pen = CreatePen(PS_SOLID, width, lineColor);
        HPEN oldPen = (HPEN)SelectObject(hdc, pen);

        MoveToEx(hdc, x1, y1, NULL);
        LineTo(hdc, x2, y2);

        SelectObject(hdc, oldPen);
        DeleteObject(pen);
    }

    void drawArrow(HDC hdc, int x1, int y1, int x2, int y2, COLORREF arrowColor, int width = 3) {
        drawLine(hdc, x1, y1, x2, y2, arrowColor, width);

        double angle = atan2((double)(y2 - y1), (double)(x2 - x1));
        int length = 13;

        int ax1 = x2 - (int)(cos(angle - 0.55) * length);
        int ay1 = y2 - (int)(sin(angle - 0.55) * length);
        int ax2 = x2 - (int)(cos(angle + 0.55) * length);
        int ay2 = y2 - (int)(sin(angle + 0.55) * length);

        drawLine(hdc, x2, y2, ax1, ay1, arrowColor, width);
        drawLine(hdc, x2, y2, ax2, ay2, arrowColor, width);
    }

    POINT calculatePoint(POINT start, POINT end, float t) {
        POINT point;
        point.x = (int)(start.x + (end.x - start.x) * t);
        point.y = (int)(start.y + (end.y - start.y) * t);

        return point;
    }

    POINT getInstructionRegisterPosition() {
        if (cpu == NULL) {
            return { 0, 0 };
        }

        POINT start;
        POINT end;
        int phase = cpu->getPhase();

        if (phase == 0) {
            start = { inputBox.x + inputBox.w, inputBox.y + 55 };
            end = { instrMemBox.x, instrMemBox.y + 55 };
        }
        else if (phase == 1) {
            start = { instrMemBox.x + instrMemBox.w, instrMemBox.y + 65 };
            end = { cpuChipBox.x + 35, cpuChipBox.y + 175 };
        }
        else if (phase == 2) {
            start = { controlUnitBox.x + 95, controlUnitBox.y + controlUnitBox.h };
            end = { aluBox.x + 95, aluBox.y };
        }
        else {
            start = { aluBox.x + aluBox.w, aluBox.y + 40 };
            end = { registerBox.x, registerBox.y + 70 };
        }

        return calculatePoint(start, end, animation);
    }

    void drawBackground(HDC hdc) {
        RECT rect;
        GetClientRect(window, &rect);

        HBRUSH brush = CreateSolidBrush(color(7, 16, 34));
        FillRect(hdc, &rect, brush);
        DeleteObject(brush);

        for (int x = 0; x < rect.right; x += 32) {
            drawLine(hdc, x, 0, x, rect.bottom, color(18, 36, 66), 1);
        }

        for (int y = 0; y < rect.bottom; y += 32) {
            drawLine(hdc, 0, y, rect.right, y, color(18, 36, 66), 1);
        }
    }

    void drawHeader(HDC hdc, bool running) {
        drawText(hdc, 25, 24, "CPU INSTRUCTION EXECUTION SIMULATOR", color(245, 248, 255), 22, true);
        drawText(hdc, 25, 58, "SYSTEM: " + cpu->getPhaseName(), color(55, 255, 120), 24, true);

        Box pcBox{ 700, 35, 160, 70 };
        Box opcodeBox{ 875, 35, 160, 70 };
        Box statusBox{ 1050, 35, 160, 70 };

        drawBox(hdc, pcBox, color(16, 28, 54), color(70, 140, 255), 2);
        drawBox(hdc, opcodeBox, color(16, 28, 54), color(255, 190, 45), 2);
        drawBox(hdc, statusBox, color(16, 28, 54), color(40, 255, 110), 2);

        drawText(hdc, pcBox.x + 14, pcBox.y + 15, "Program Counter", color(255, 255, 255), 15, true);
        drawText(hdc, pcBox.x + 14, pcBox.y + 42, std::to_string(cpu->getPC()), color(255, 255, 255), 16, true);

        drawText(hdc, opcodeBox.x + 14, opcodeBox.y + 15, "Current Opcode", color(255, 255, 255), 15, true);
        drawText(hdc, opcodeBox.x + 14, opcodeBox.y + 42, cpu->getIR().opcode, color(255, 255, 255), 16, true);

        drawText(hdc, statusBox.x + 14, statusBox.y + 15, "Simulation", color(255, 255, 255), 15, true);
        drawText(hdc, statusBox.x + 14, statusBox.y + 42, running ? "RUNNING" : "READY", color(255, 255, 255), 16, true);
    }
    void drawNode(HDC hdc, int x, int y, COLORREF nodeColor) {
        HBRUSH brush = CreateSolidBrush(nodeColor);
        HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, brush);
        Ellipse(hdc, x - 5, y - 5, x + 5, y + 5);
        SelectObject(hdc, oldBrush);
        DeleteObject(brush);
    }

    void drawCpuPackage(HDC hdc) {
        COLORREF pinColor = color(45, 125, 205);

        for (int i = 0; i < 7; i++) {
            int y = cpuChipBox.y + 78 + i * 34;
            drawLine(hdc, cpuChipBox.x - 12, y, cpuChipBox.x, y, pinColor, 3);
            drawLine(hdc, cpuChipBox.x + cpuChipBox.w, y, cpuChipBox.x + cpuChipBox.w + 12, y, pinColor, 3);
        }

        for (int i = 0; i < 8; i++) {
            int x = cpuChipBox.x + 82 + i * 48;
            drawLine(hdc, x, cpuChipBox.y - 10, x, cpuChipBox.y, pinColor, 3);
            drawLine(hdc, x, cpuChipBox.y + cpuChipBox.h, x, cpuChipBox.y + cpuChipBox.h + 10, pinColor, 3);
        }

        drawBox(hdc, cpuChipBox, color(8, 31, 51), color(70, 160, 245), 3);

        drawCenteredText(hdc, { cpuChipBox.x + 20, cpuChipBox.y + 14, cpuChipBox.w - 40, 28 }, "C.P.U. CORE", color(255, 255, 255), 19, true);
        drawCenteredText(hdc, { cpuChipBox.x + 20, cpuChipBox.y + 45, cpuChipBox.w - 40, 22 }, "FETCH  ->  DECODE  ->  EXECUTE  ->  WRITE BACK", color(190, 225, 255), 13);

        Box pcMini{ cpuChipBox.x + 78, cpuChipBox.y + 82, 86, 38 };
        Box irMini{ cpuChipBox.x + 206, cpuChipBox.y + 82, 86, 38 };
        Box busMini{ cpuChipBox.x + 334, cpuChipBox.y + 82, 96, 38 };

        drawBox(hdc, pcMini, color(18, 45, 76), color(80, 160, 255), 2);
        drawBox(hdc, irMini, color(70, 55, 16), color(255, 215, 55), 2);
        drawBox(hdc, busMini, color(16, 58, 50), color(60, 235, 185), 2);

        drawCenteredText(hdc, pcMini, "PC", color(255, 255, 255), 14, true);
        drawCenteredText(hdc, irMini, "IR", color(255, 255, 255), 14, true);
        drawCenteredText(hdc, busMini, "BUS", color(255, 255, 255), 14, true);

        drawArrow(hdc, pcMini.x + pcMini.w, pcMini.y + 19, irMini.x, irMini.y + 19, color(80, 160, 255), 2);
        drawArrow(hdc, irMini.x + irMini.w, irMini.y + 19, busMini.x, busMini.y + 19, color(255, 215, 55), 2);

        int busCenterX = busMini.x + busMini.w / 2;
        drawLine(hdc, busCenterX, busMini.y + busMini.h, busCenterX, controlUnitBox.y - 14, color(60, 235, 185), 3);
        drawLine(hdc, busCenterX, controlUnitBox.y - 14, controlUnitBox.x + 102, controlUnitBox.y - 14, color(60, 235, 185), 3);
        drawLine(hdc, controlUnitBox.x + 102, controlUnitBox.y - 14, controlUnitBox.x + 102, controlUnitBox.y, color(60, 235, 185), 3);
        drawNode(hdc, controlUnitBox.x + 102, controlUnitBox.y, color(60, 235, 185));
    }

    void drawFlow(HDC hdc) {
        drawBox(hdc, inputBox, color(34, 47, 76), color(80, 145, 255), 2);
        drawText(hdc, inputBox.x + 15, inputBox.y + 18, "Input Program", color(255, 255, 255), 16, true);
        drawText(hdc, inputBox.x + 15, inputBox.y + 50, "LOAD / ADD / SUB", color(255, 255, 255), 14);
        drawText(hdc, inputBox.x + 15, inputBox.y + 80, "STORE / LOADM", color(255, 255, 255), 14);

        drawBox(hdc, instrMemBox, color(22, 55, 45), color(60, 235, 185), 2);
        drawText(hdc, instrMemBox.x + 15, instrMemBox.y + 18, "Instruction Memory", color(255, 255, 255), 16, true);
        drawText(hdc, instrMemBox.x + 15, instrMemBox.y + 52, "program array", color(255, 255, 255), 14);
        drawText(hdc, instrMemBox.x + 15, instrMemBox.y + 82, "read(PC)", color(255, 255, 255), 14);

        drawCpuPackage(hdc);

        drawBox(hdc, controlUnitBox, color(51, 34, 70), color(205, 120, 255), 2);
        drawCenteredText(hdc, { controlUnitBox.x + 8, controlUnitBox.y + 10, controlUnitBox.w - 16, 22 }, "Control Unit", color(255, 255, 255), 16, true);
        drawText(hdc, controlUnitBox.x + 22, controlUnitBox.y + 45, "decode()", color(255, 255, 255), 14);
        drawText(hdc, controlUnitBox.x + 22, controlUnitBox.y + 66, "signals", color(255, 255, 255), 14);

        drawBox(hdc, aluBox, color(71, 45, 19), color(255, 180, 25), 2);
        drawCenteredText(hdc, { aluBox.x + 8, aluBox.y + 8, aluBox.w - 16, 22 }, "ALU", color(255, 255, 255), 16, true);
        drawCenteredText(hdc, { aluBox.x + 8, aluBox.y + 34, aluBox.w - 16, 18 }, "ADD / SUB", color(255, 255, 255), 14);
        drawCenteredText(hdc, { aluBox.x + 8, aluBox.y + 56, aluBox.w - 16, 18 }, "execute()", color(255, 255, 255), 13);

        drawBox(hdc, registerBox, color(68, 25, 36), color(255, 90, 130), 2);
        drawCenteredText(hdc, { registerBox.x + 8, registerBox.y + 10, registerBox.w - 16, 24 }, "Registers", color(255, 255, 255), 16, true);

        const std::vector<int>& registers = cpu->getRegisterFile().getAllRegisters();

        for (int i = 0; i < 4; i++) {
            drawText(hdc, registerBox.x + 24, registerBox.y + 40 + i * 19,
                "R" + std::to_string(i) + "=" + std::to_string(registers[i]), color(255, 255, 255), 13);
        }

        drawArrow(hdc, inputBox.x + inputBox.w, inputBox.y + 55, instrMemBox.x, instrMemBox.y + 55, color(80, 160, 255), 3);
        drawText(hdc, 315, 145, "FETCH", color(130, 185, 255), 13, true);

        drawArrow(hdc, instrMemBox.x + instrMemBox.w, instrMemBox.y + 65, cpuChipBox.x + 35, cpuChipBox.y + 190, color(80, 160, 255), 3);
        Box entryLabel{ cpuChipBox.x - 88, cpuChipBox.y + 178, 78, 24 };
        drawBox(hdc, entryLabel, color(7, 16, 34), color(80, 160, 255), 1);
        drawCenteredText(hdc, entryLabel, "CPU IN", color(180, 220, 255), 11, true);

        drawArrow(hdc, controlUnitBox.x + 102, controlUnitBox.y + controlUnitBox.h, aluBox.x + 102, aluBox.y, color(175, 105, 255), 3);
        Box decodeLabel{ controlUnitBox.x + 62, controlUnitBox.y + controlUnitBox.h + 5, 80, 18 };
        drawBox(hdc, decodeLabel, color(8, 31, 51), color(175, 105, 255), 1);
        drawCenteredText(hdc, decodeLabel, "DECODE", color(230, 200, 255), 11, true);

        drawArrow(hdc, aluBox.x + aluBox.w, aluBox.y + 40, registerBox.x, registerBox.y + 70, color(255, 185, 35), 3);
        Box executeLabel{ aluBox.x + aluBox.w + 5, aluBox.y + 20, 82, 18 };
        drawBox(hdc, executeLabel, color(8, 31, 51), color(255, 185, 35), 1);
        drawCenteredText(hdc, executeLabel, "EXECUTE", color(255, 230, 120), 11, true);

        drawArrow(hdc, registerBox.x, registerBox.y + 55, controlUnitBox.x + controlUnitBox.w, controlUnitBox.y + 45, color(255, 95, 135), 3);

        POINT point = getInstructionRegisterPosition();

        HBRUSH brush = CreateSolidBrush(color(255, 215, 55));
        HPEN pen = CreatePen(PS_SOLID, 5, color(150, 120, 20));

        HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, brush);
        HPEN oldPen = (HPEN)SelectObject(hdc, pen);

        Ellipse(hdc, point.x - 22, point.y - 22, point.x + 22, point.y + 22);

        SelectObject(hdc, oldBrush);
        SelectObject(hdc, oldPen);

        DeleteObject(brush);
        DeleteObject(pen);

        drawText(hdc, point.x - 9, point.y - 9, "IR", color(0, 0, 0), 15, true);
    }

    void drawPanels(HDC hdc) {
        drawBox(hdc, instructionPanel, color(17, 28, 49), color(80, 145, 255), 2);
        drawText(hdc, instructionPanel.x + 15, instructionPanel.y + 18, "Instruction Memory", color(255, 255, 255), 16, true);

        const std::vector<Instruction>& program = cpu->getMemory().getInstructions();

        for (int i = 0; i < (int)program.size(); i++) {
            std::string prefix = (i == cpu->getPC()) ? "-> " : "   ";
            COLORREF textColor = (i == cpu->getPC()) ? color(255, 240, 50) : color(235, 245, 255);

            drawText(hdc, instructionPanel.x + 18, instructionPanel.y + 45 + i * 17,
                prefix + std::to_string(i) + ": " + program[i].rawText, textColor, 12);
        }

        drawBox(hdc, registerPanel, color(17, 28, 49), color(255, 90, 130), 2);
        drawText(hdc, registerPanel.x + 15, registerPanel.y + 18, "Register File", color(255, 255, 255), 16, true);

        const std::vector<int>& registers = cpu->getRegisterFile().getAllRegisters();

        for (int i = 0; i < 8; i++) {
            int column = i % 2;
            int row = i / 2;

            drawText(hdc, registerPanel.x + 20 + column * 105, registerPanel.y + 55 + row * 34,
                "R" + std::to_string(i) + " = " + std::to_string(registers[i]), color(255, 255, 255), 15, true);
        }

        drawBox(hdc, memoryPanel, color(17, 28, 49), color(60, 235, 185), 2);
        drawText(hdc, memoryPanel.x + 15, memoryPanel.y + 18, "Data Memory", color(255, 255, 255), 16, true);

        const std::vector<int>& dataMemory = cpu->getMemory().getDataMemory();

        for (int i = 0; i < 8; i++) {
            drawText(hdc, memoryPanel.x + 25, memoryPanel.y + 50 + i * 20,
                "M[" + std::to_string(i) + "] = " + std::to_string(dataMemory[i]), color(220, 255, 245), 13);
        }

        drawBox(hdc, logPanel, color(17, 28, 49), color(255, 180, 25), 2);
        drawText(hdc, logPanel.x + 15, logPanel.y + 18, "Execution Log", color(255, 255, 255), 16, true);

        const std::vector<std::string>& logs = cpu->getLogs();

        int firstLog = 0;

        if (logs.size() > 9) {
            firstLog = (int)logs.size() - 9;
        }

        for (int i = firstLog; i < (int)logs.size(); i++) {
            int visibleIndex = i - firstLog;

            drawText(hdc, logPanel.x + 15, logPanel.y + 48 + visibleIndex * 16, logs[i], color(255, 242, 200), 10);
        }

        drawText(hdc, 35, 762, "Controls: R = Run/Pause   SPACE = Step   BACKSPACE = Reset   ESC = Exit",
            color(255, 255, 255), 13);
    }

    void drawButtons(HDC hdc) {
        Box runBox{ 930, 755, 75, 35 };
        Box stepBox{ 1020, 755, 75, 35 };
        Box resetBox{ 1110, 755, 85, 35 };

        drawBox(hdc, runBox, color(18, 58, 48), color(60, 235, 185), 2);
        drawBox(hdc, stepBox, color(18, 42, 72), color(80, 160, 255), 2);
        drawBox(hdc, resetBox, color(72, 45, 18), color(255, 180, 25), 2);

        drawText(hdc, runBox.x + 22, runBox.y + 9, "RUN", color(255, 255, 255), 15, true);
        drawText(hdc, stepBox.x + 16, stepBox.y + 9, "STEP", color(255, 255, 255), 15, true);
        drawText(hdc, resetBox.x + 15, resetBox.y + 9, "RESET", color(255, 255, 255), 15, true);
    }

    bool isInside(Box box, int x, int y) const {
        return x >= box.x && x <= box.x + box.w && y >= box.y && y <= box.y + box.h;
    }

    bool isRunButton(int x, int y) const {
        return isInside({ 930, 755, 75, 35 }, x, y);
    }

    bool isStepButton(int x, int y) const {
        return isInside({ 1020, 755, 75, 35 }, x, y);
    }

    bool isResetButton(int x, int y) const {
        return isInside({ 1110, 755, 85, 35 }, x, y);
    }

    void drawAll(HDC hdc, bool running) {
        if (cpu == NULL) {
            return;
        }

        drawBackground(hdc);
        drawHeader(hdc, running);
        drawFlow(hdc);
        drawPanels(hdc);
        drawButtons(hdc);
    }
};
