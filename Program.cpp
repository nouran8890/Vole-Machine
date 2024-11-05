#include "Program.h"


string decimalToBinary(int decimal) {
    if (decimal == 0) return "0";

    string binary;
    while (decimal > 0) {
        binary = std::to_string(decimal % 2) + binary;  // Prepend each remainder
        decimal /= 2;
    }
    return binary;
}

string binaryAdd(const string& a, const string& b) {
    std::string result = "";
    int carry = 0;

    // Start from the least significant bit
    for (int i = a.size() - 1; i >= 0; i--) {
        int sum = (a[i] - '0') + (b[i] - '0') + carry;
        result = char((sum % 2) + '0') + result; // Append the result of the current bit
        carry = sum / 2; // Update carry
    }

    if (carry) {
        result = '1' + result; // If there's a carry at the end, add it to the result
    }

    return result;
}

int binaryToDecimal(string& binStr) {
    int decimalValue = 0;
    int length = binStr.size();

    // Iterate over each character in the binary string
    for (int i = 0; i < length; ++i) {
        if (binStr[i] == '1') {
            // Multiply 1 by 2^(position from the right)
            decimalValue += pow(2, length - 1 - i);
        }
    }
    return decimalValue;
}


string decToHex(int dec) {
    char hexDigits[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
    string hex = "";
    if (dec == 0) {
        return "0";
    }
    else {
        while (dec > 0) {
            int RemainderOfDivision = dec % 16;
            hex = hexDigits[RemainderOfDivision] + hex;
            dec /= 16;
        }
    }
    return hex;
}


Memory::Memory() {
    for (int i = 0; i < 256; ++i) {
        memory[i] = "00"; // Initialize memory with default value
    }
}

std::string Memory::getCell(int index) {
    if (index >= 0 && index < 256) {
        return memory[index];
    }
    else {
        std::cerr << "Error: Invalid memory address " << index << ". Returning default value '00'." << std::endl;
        return "00";
    }
}

void Memory::setCell(int index, const std::string& val) {
    if (index >= 0 && index < 256) {
        memory[index] = val;
    }
    else {
        std::cerr << "Error: Invalid memory address." << std::endl;
    }
}

void Memory::loadFromFile(const std::string& fileName) {
    std::ifstream inputFile(fileName);
    int index = 0;
    std::string line;
    int invalidLineCount = 0;

    if (!inputFile) {
        std::cerr << "Error: Could not open the file." << std::endl;
        return;
    }

    while (std::getline(inputFile, line) && index < 256) {
        if (line.length() == 4) { // Ensure each line is exactly 4 characters
            std::string part1 = line.substr(0, 2);
            std::string part2 = line.substr(2, 2);
            bool isValid = true;

            // Validate part1
            if (part1[0] != '1' && part1[0] != '2' && part1[0] != '3'
                && part1[0] != '4' && part1[0] != '5' && part1[0] != '6'
                && part1[0] != 'B' && part1[0] != 'C') {
                isValid = false;
                std::cerr << "Error: Invalid instruction in line: " << line << std::endl;
            }
            else if ((part1[0] == 'C' || part1[0] == '4') && part1[1] != '0') {
                isValid = false;
                std::cerr << "Error: Invalid instruction in line: " << line << std::endl;
            }

            // Validate part2
            if (!isHexadecimal(part2)) {
                isValid = false;
                std::cerr << "Error: Invalid instruction in line: " << line << std::endl;
            }

            // Set valid values in memory
            if (isValid) {
                memory[index++] = part1;
                if (index < 256) {
                    memory[index++] = part2;
                }
            }
            else {
                invalidLineCount++;
            }
        }
        else {
            std::cerr << "Error: Line too short or too long: " << line << std::endl;
            invalidLineCount++;
        }
    }

    if (invalidLineCount > 0) {
        std::cerr << "Total invalid lines: " << invalidLineCount << std::endl;
    }
}

void Memory::printMemory() {
    std::cout << "Memory cells:\n";
    std::cout << "   ";
    for (int col = 0; col < 16; ++col) {
        std::cout << std::hex << std::uppercase << std::setw(2) << col << " ";
    }
    std::cout << "\n";

    for (int row = 0; row < 16; ++row) {
        std::cout << std::hex << std::uppercase << std::setw(2) << row << " ";
        for (int col = 0; col < 16; ++col) {
            int index = row * 16 + col;
            std::cout << std::setw(2) << memory[index] << " "; // Ensure setw(2) applies to each memory cell
        }
        std::cout << "\n";
    }
}

// Helper function to check if a string represents a valid 2-digit hexadecimal number

bool Memory::isHexadecimal(const std::string& str) {
    if (str.length() != 2) return false;
    for (char c : str) {
        if (!((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F'))) {
            return false;
        }
    }
    return true;
}

Register::Register() {
    for (int i = 0; i < 16; ++i) {
        registers[i] = 0; // Initialize all registers to 0
    }
}

int Register::getRegister(int index) const {
    if (index >= 0 && index < 16) {
        return registers[index];
    }
    else {
        cerr << "Error: Invalid register index." << endl;
        return -1;
    }
}

void Register::setRegister(int index, int value) {
    if (index >= 0 && index < 16) {
        registers[index] = value;
    }
    else {
        cerr << "Error: Invalid register index." << endl;
    }
}

void Register::printRegisters() const {
    cout << "Registers:\n";

    for (int j = 0; j < 16; ++j) {
        cout << hex << uppercase << setw(2) << j << " ";
    }
    cout << "\n";

    cout << " ";
    for (int i = 0; i < 16; i++) {
        cout << setw(2) << hex << uppercase << registers[i] << " ";
    }
    cout << "\n";
}

void ALU::add5(Instruction& ir, Register& r) {
    string binS, binT, res;
    int regs, regt;
    regs = r.getRegister(ir.operand1);
    regt = r.getRegister(ir.operand2);
    binS = decimalToBinary(regs);
    binT = decimalToBinary(regt);
    res = binaryAdd(binS, binT);
    r.setRegister(ir.regAddr, binaryToDecimal(res));
}

void ALU::add6(Instruction& ir, Register& r) {
    // Implementation
}

void CU::load2(const Instruction& ir, Register& r) {
    r.setRegister(ir.regAddr, ir.address);
}

void CU::store3(Instruction& ir, Memory& m, Register& r) {
    int n = r.getRegister(ir.regAddr);
    string sn = decToHex(n);
    if (ir.address == 0) {
        cout << sn << endl;
    }
    else
        m.setCell(ir.address, sn);
}

void CU::move(Instruction& ir, Register& r) {
    int c = r.getRegister(ir.regAddr);
    r.setRegister(ir.operand1, c);
}
void CU::jump(Instruction& ir) {
    // Implementation
}

void CU::halt() {
    running = false;
    cout << "Program has halted." << std::endl;

}

bool CU::isRunning() const {
    return running;
}

Instruction CPU::fetch_Decode(Memory& m, Register& r) {
    Instruction ir;

    std::string cell = m.getCell(progCounter);
    if (cell.empty()) {
        throw std::runtime_error("Error: Failed to fetch instruction from memory.");
    }

    ir.opcode = cell[0];  // Extract opcode
    instructionRegister = cell[1];  // Extract register part

    memCell = m.getCell(progCounter + 1);
    if (memCell.empty()) {
        throw std::runtime_error("Error: Failed to fetch memory cell for address.");
    }

    ir.regAddr = hexToDec(instructionRegister);
    ir.address = hexToDec(memCell);

    // Check if opcode requires operands
    if (ir.opcode == '5' || ir.opcode == '6') {
        ir.operand1 = hexToDec(std::string(1, memCell[0]));
        ir.operand2 = hexToDec(std::string(1, memCell[1]));
    }

    progCounter += 2;  // Move to the next instruction
    return ir;
}

void CPU::execute(Instruction& instr, Register& r, CPU cpu) {
    switch (instr.opcode) {
    case '1':
        //cu.load1(instr.regAddr, instr.address);
        break;
    case '2':
        cu.load2(instr, r);
        break;
    case '3':
        // cu.store(instr.operand1, instr.address);
        break;
    case '4':
        //cu.jump(instr.address);
        break;
    case '5': // mine 
        alu.add5(instr, r);
        break;
    case '6':
        //cu.jump(instr.address);
        break;
    case 'B':
        //cu.jump(instr.address);
        break;
    case 'C':  // Halt
        cu.halt();
        break;
    default:
        std::cout << "Unknown opcode: " << instr.opcode << std::endl;
        cu.halt(); // Halt on unknown opcode
    }
}

int CPU::hexToDec(const std::string& hexStr) {
    int dec = 0;
    try {
        for (char hexDigit : hexStr) {
            int digit;
            if (hexDigit >= '0' && hexDigit <= '9') {
                digit = hexDigit - '0';
            }
            else if (hexDigit >= 'A' && hexDigit <= 'F') {
                digit = hexDigit - 'A' + 10;
            }
            else {
                throw std::invalid_argument("Error: Invalid hex digit encountered.");
            }
            dec = dec * 16 + digit;
        }
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return dec;
}

string CPU::hexToBin(string hex) {
    unordered_map<char, string> hexToBinMap = {
    {'0', "0000"}, {'1', "0001"}, {'2', "0010"}, {'3', "0011"},
    {'4', "0100"}, {'5', "0101"}, {'6', "0110"}, {'7', "0111"},
    {'8', "1000"}, {'9', "1001"}, {'A', "1010"}, {'B', "1011"},
    {'C', "1100"}, {'D', "1101"}, {'E', "1110"}, {'F', "1111"}
    };

    string binary = "";
    for (char hexDigit : hex) {
        if (hexToBinMap.find(hexDigit) != hexToBinMap.end()) {
            binary += hexToBinMap[hexDigit];
        }
    }

    return binary;
}

// Method to check if the CPU is still running
bool CPU::isRunning() const {
    return running;
}

// Method to stop the CPU
void CPU::stop() {
    running = false;
}

// Instruction CPU::cycle(Memory& m,Register& r,CPU cpu) {

//     Instruction ir = fetch_Decode(m, r);

//     execute(ir, r,cpu);


//     return ir;
// }



/*
void Machine::loadProgramFile() {
    // Implementation
}

void Machine::outputState() {
    // Implementation
}

bool MainUI::getFileOrInstructions() {
    // Implementation
}

void MainUI::DisplayMenu() {
    // Implementation
}

string MainUI::inputFileName() {
    // Implementation
}

string MainUI::inputInstruction() {
    // Implementation
}

char MainUI::inputChoice() {
    // Implementation
}
*/