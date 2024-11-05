#ifndef PROGRAM_H
#define PROGRAM_H

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <stdexcept>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <unordered_map>
#include <bitset>


using namespace std;

struct Instruction {
    char opcode;
    int regAddr;
    int operand1;
    int operand2;
    int address;
};
string decimalToBinary(int decimal);
string binaryAdd(const string& a, const string& b);
int binaryToDecimal(string& binStr);

class Memory {
private:
    std::string memory[256];

public:
    Memory();
    std::string getCell(int index);
    void setCell(int index, const std::string& val);
    void loadFromFile(const std::string& fileName);
    void printMemory();

private:
    bool isHexadecimal(const std::string& str);
};


class Register {
private:
    int registers[16];

public:
    Register();
    int getRegister(int index) const;
    void setRegister(int index, int value);
    void printRegisters() const;
};

class ALU {
public:
    void add5(Instruction& ir, Register& r);
    void add6(Instruction& ir, Register& r);

};

class CU {
private:
    bool running = true;
public:
    void load2(const Instruction& ir, Register& r);
    void store3(Instruction& ir, Memory& m, Register& r);
    void move(Instruction& ir, Register& r);
    void jump(Instruction& ir);
    void halt();
    bool isRunning() const;
};

class CPU {
private:
    ALU alu;
    CU cu;
    int progCounter = 0;
    string instructionRegister;
    char char_opcode;
    string memCell;
    int intMemCell;
    int intRegCell;
    bool running;


public:

    CPU() : running(true) {};
    Instruction fetch_Decode(Memory& m, Register& r);
    void execute(Instruction& instr, Register& r, CPU cpu);
    int hexToDec(const std::string& hexStr);
    string decToHex(int dec);
    string hexToBin(string hex);
    //Instruction cycle(Memory& m, Register& r,CPU cpu);
    bool isRunning() const;
    void stop();

};

class Machine {
private:
    CPU processor;
    Memory memory;

public:
    void loadProgramFile();
    void outputState();
};

class MainUI {
private:
    Machine machine;
    bool enterFileOrInstruction;

public:
    bool getFileOrInstructions();
    void DisplayMenu();
    string inputFileName();
    string inputInstruction();
    char inputChoice();
};

#endif
