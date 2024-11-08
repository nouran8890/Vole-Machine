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
#include <vector>
#include <cmath>

using namespace std;

class Screen {
public:
    vector <string> screen;
    void printScreenHex();
    void printScreenASCI();
};
struct Instruction {
    char opcode;
    int regAddr;
    int operand1;
    int operand2;
    int address;
};


class Memory {
private:
    string memory[256];

public:
    Memory();
    std::string getCell(int index);
    void loadFromFile(const std::string& filename);
    void setCell(int index, const std::string& val);
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

    string binaryAdd(string a, string b);
    string twosComplement(string binNum);

    void add5(Instruction& ir, Register& r);


    string fracToBinary(float fractionalPart, int precision = 10);
    string floatToHexWithBias(float value, int bias = 4);
    double doubleBinaryToDecimal(const string& binary);
    double HexToDouble(string hexString);
    
    void add6(Instruction& ir, Register& r);

};

class CPU;

class CU {
private:
    bool running = true;
public:
    void load1(const Instruction& ir, Register& r, Memory& m);
    void load2(const Instruction& ir, Register& r, Memory& m);
    void store3(Instruction& ir, Memory& m, Register& r, Screen& S);
    void move(Instruction& ir, Register& r);
    void jump(Instruction& ir, Register& r, Memory& m, CPU& cpu);
    void halt();
    bool isRunning() const;
};

class CPU {
private:
    ALU alu;
    CU cu;
    string instructionRegister;
    char char_opcode;
    string memCell;
    int intMemCell;
    int intRegCell;
    bool running;


public:
    int progCounter = 10;
    CPU() : running(true) {};
    Instruction fetch_Decode(Memory& m, Register& r);
    void execute(Instruction& instr, Register& r, CPU cpu, Memory& m, Screen& S);
    bool isRunning() const;
    void stop();

};


#endif
