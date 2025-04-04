#include "Program.h"
#include <iostream>
using namespace std;

// Helper Funtions used in several classed
string decimalToBinary(int decimal) {
    if (decimal == 0) return "0";
    string binary;
    while (decimal > 0) {
        binary = std::to_string(decimal % 2) + binary;
        decimal /= 2;
    }
    return binary;
}

int hexToDec(const std::string& hexStr) {
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

// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

void Screen::printScreenHex()
{
    cout << "HexaDecimal Screen: ";
    cout << "[ ";
    for (int i = 0; i < screen.size()-1; i++) {
        cout << screen[i] << " , ";
    }
    cout << screen[screen.size() - 1];
    cout << " ]" << endl;
}

void Screen::printScreenASCI() {   
    int decscreen;
    cout << "Asci Screen: ";
    cout << "[ ";
    for (int i = 0; i < screen.size()-1; i++) {
        decscreen = hexToDec(screen[i]);
        cout << char(decscreen) << " , ";
    }
    cout << char(hexToDec(screen[screen.size() - 1]));
    cout << " ]" << endl;

}

// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

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
    ifstream inputFile(fileName);
    int index = 10;
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

// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

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


// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

string ALU::binaryAdd(string a, string b) {
    string result = "";
    int carry = 0;

    // Add leading zeroes to make both strings of length 8 bits
    while (a.size() < 8) {
        a = "0" + a;
    }
    while (b.size() < 8) {
        b = "0" + b;
    }

    // Start from the least significant bit
    for (int i = a.size() - 1; i >= 0; i--) {
        int sum = (a[i] - '0') + (b[i] - '0') + carry;
        result = char((sum % 2) + '0') + result;
        carry = sum / 2;
    }


    // Append final carry if there's overflow
    if (carry) {
        result = '1' + result;
    }

    // Truncate to 8 bits if overflow occurred
    if (result.size() > 8) {
        result = result.substr(result.size() - 8);
    }
    return result;
}

// Get the two's complement of a binary string
string ALU::twosComplement(string binNum) {
    string numComp = "";

    // Flip the bits
    for (char n : binNum) {
        numComp += (n == '1') ? '0' : '1';
    }

    // Add 1 to the flipped bits
    return binaryAdd(numComp, "1");
}


void ALU::add5(Instruction& ir, Register& r) {

    // convert decimal values in registers to binary
    string binS = decimalToBinary(r.getRegister(ir.operand1));
    string binT = decimalToBinary(r.getRegister(ir.operand2));

    // add them in binary 
    string result = binaryAdd(binS, binT);
    int res = binaryToDecimal(result);

    //get the 2's Complement of the result
    //int res = twosComTodec(result);
    r.setRegister(ir.regAddr, res);

}


// Helper function to convert fractional part of a float to binary with specified precision
string ALU::fracToBinary(float fractionalPart, int precision) {
    string binary;
    while (fractionalPart > 0 && precision-- > 0) {
        fractionalPart *= 2;
        int bit = static_cast<int>(fractionalPart);
        binary += to_string(bit);
        fractionalPart -= bit;
    }
    return binary;
}

// Main function to convert float to custom IEEE-like hex format
string ALU::floatToHexWithBias(float value, int bias) {
    // Step 1: Handle the sign bit
    int sign = (value < 0) ? 1 : 0;
    value = (value < 0) ? value * -1 : value;

    // Step 2: Separate integer and fractional parts
    int integerPart = static_cast<int>(value);
    float fractionalPart = value - integerPart;

    // Step 3: Convert integer part to binary
    string binaryInteger = decimalToBinary(integerPart);

    // Step 4: Convert fractional part to binary
    string binaryFractional = fracToBinary(fractionalPart);

    // Step 5: Normalize the binary representation to fit 0.M format
    string normalizedBinary;
    int exponent = 0;

    if (integerPart != 0) {
        // Non-zero integer part, so shift to 0.M format
        normalizedBinary = binaryInteger + binaryFractional;
        exponent = binaryInteger.size();  // Offset to make it 0.M instead of 1.M
    }
    else {
        // Integer part is zero, so find the first '1' in fractional part
        int firstOne = binaryFractional.find('1');
        if (firstOne != string::npos) {
            exponent = -(firstOne);
            normalizedBinary = binaryFractional.substr(firstOne);
        }
        else {
            return "0";  // Handle case of 0.0
        }
    }

    // Step 6: Set the mantissa to exactly 4 bits
    string mantissa = normalizedBinary.substr(0, 4);
    if (mantissa.size() < 4) { // if it's <4 fill with leading zeroes
        mantissa.append(4 - mantissa.size(), '0');
    }

    // Step 7: Calculate biased exponent
    int biasedExponent = exponent + bias;
    string binaryExponent = decimalToBinary(biasedExponent);

    // Ensure exponent is 3 bits
    while (binaryExponent.size() < 3) {
        binaryExponent = "0" + binaryExponent;
    }



    // Step 8: Combine sign, exponent, and mantissa
    string ieeeBinary = to_string(sign) + binaryExponent + mantissa;
    int ieeeDecimal = binaryToDecimal(ieeeBinary);
    string hexValue = decToHex(ieeeDecimal);




    return hexValue.empty() ? "0" : hexValue;
}

double ALU::doubleBinaryToDecimal(const string& binary) {
    size_t pointPos = binary.find('.');

    // Separate integer and fractional parts
    string integerPart = pointPos == string::npos ? binary : binary.substr(0, pointPos);
    string fractionalPart = pointPos == string::npos ? "" : binary.substr(pointPos + 1);

    // Convert integer part to decimal
    double decimalValue = 0;
    for (int i = 0; i < integerPart.size(); ++i) {
        if (integerPart[i] == '1') {
            decimalValue += pow(2, integerPart.size() - i - 1);
        }
    }

    // Convert fractional part to decimal
    for (int i = 0; i < fractionalPart.size(); ++i) {
        if (fractionalPart[i] == '1') {
            decimalValue += pow(2, -(i + 1));
        }
    }

    return decimalValue;
}

double ALU::HexToDouble(string hexString) {
    string binary;
    unordered_map<char, string> hexToBinMap = {
        {'0', "0000"}, {'1', "0001"}, {'2', "0010"}, {'3', "0011"},
        {'4', "0100"}, {'5', "0101"}, {'6', "0110"}, {'7', "0111"},
        {'8', "1000"}, {'9', "1001"}, {'A', "1010"}, {'B', "1011"},
        {'C', "1100"}, {'D', "1101"}, {'E', "1110"}, {'F', "1111"}
    };

    // Convert hex string to binary string
    for (char hexDigit : hexString) {
        if (hexToBinMap.find(hexDigit) != hexToBinMap.end()) {
            binary += hexToBinMap[hexDigit];
        }
    }

    int sign = binary[0] - '0';
    string charexponent = binary.substr(1, 3);
    int exponent = binaryToDecimal(charexponent) - 4;
    string mantissa = "0." + binary.substr(4);  // Explicit normalization as 0.M

    // Convert binary mantissa to decimal
    double mantissaDecimal = doubleBinaryToDecimal(mantissa);

    // Calculate the final value using (-1)^sign * mantissa * 2^(exponent)
    double doubleNum = mantissaDecimal * pow(2, exponent);

    // Apply sign
    if (sign == 1) {
        doubleNum = -doubleNum;
    }

    return doubleNum;
}

void ALU::add6(Instruction& ir, Register& r) {

    // convert Integer values in registers S & T to hexadecimal
    string hexregS = decToHex(r.getRegister(ir.operand1));
    string hexregT = decToHex(r.getRegister(ir.operand2));

    // convert the hexa to double 
    double floatregS = HexToDouble(hexregS);
    double floatregT = HexToDouble(hexregT);

    // perform addition like normal on floating point numbers
    double result = floatregS + floatregT;

    // convert the result to hexadecimal
    string hexResult = floatToHexWithBias(result);

    // since we store integers in registers -> convert the hexa result to decimal
    int intResult = hexToDec(hexResult);

    // set the target register R with the result in decimal form
    r.setRegister(ir.regAddr, intResult);

}

// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

//  1
void CU::load1(const Instruction& ir, Register& r, Memory& m) {
    // Get the data from the specified memory address
    string memoryData = m.getCell(ir.address);
}


// 2
void CU::load2(const Instruction& ir, Register& r, Memory& m) {
    // set the register to the value in the memory address given
    r.setRegister(ir.regAddr,ir.address);
}


// 3
void CU::store3(Instruction& ir, Memory& m, Register& r, Screen& S) {
    int n = r.getRegister(ir.regAddr);  // Get value from specified register
    string sn = decToHex(n);             // Convert the value to hexadecimal format

    if (ir.address == 0) {
        // If address is 0, add the hexadecimal value to the screen vector to output
        S.screen.push_back(sn);
    }
    else {
        // Otherwise, store the value in the specified memory cell
        m.setCell(ir.address, sn);
    }
}

//  4
void CU::move(Instruction& ir, Register& r) {
    int c = r.getRegister(ir.regAddr);
    r.setRegister(ir.operand1, c);
}

//  B
void CU::jump(Instruction& ir, Register& r, Memory& m, CPU& cpu) {
    int x = r.getRegister(ir.regAddr);
    int y = r.getRegister(0);
    if (x == y) {
        cpu.progCounter = ir.address;
    }
}

//  C000
void CU::halt() {
    running = false;
    cout << "Program has halted." << std::endl;

}

bool CU::isRunning() const {
    return running;
}

// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^


Instruction CPU::fetch_Decode(Memory& m, Register& r) {
    Instruction ir;

    std::string cell = m.getCell(progCounter);
    if (cell.empty()) {
        throw std::runtime_error("Error: Failed to fetch instruction from memory.");
    }

    ir.opcode = cell[0];  // Extract opcode
    instructionRegister = cell[1];  // Extract register part Address in Hexadecimal

    memCell = m.getCell(progCounter + 1);
    if (memCell.empty()) {
        throw std::runtime_error("Error: Failed to fetch memory cell for address.");
    }

    ir.regAddr = hexToDec(instructionRegister);  // Extract Register Address and Convert to Decimal
    ir.address = hexToDec(memCell);

    // Check if opcode requires operands
    if (ir.opcode == '5' || ir.opcode == '6') {
        ir.operand1 = hexToDec(std::string(1, memCell[0]));
        ir.operand2 = hexToDec(std::string(1, memCell[1]));
    }

    progCounter += 2;  // Move to the next instruction
    return ir;
}

void CPU::execute(Instruction& instr, Register& r, CPU cpu, Memory& m, Screen& S) {
    switch (instr.opcode) {
    case '1':  // Load from Instruction to Register
        cpu.cu.load1(instr, r, m);
        break;
    case '2':  // Load from Memory to Register
        cpu.cu.load2(instr, r, m);
        break;
    case '3':  // Store to Memory and to Screen
        cpu.cu.store3(instr, m, r, S);
        break;
    case '4':  // Move Instruction 4
        cpu.cu.move(instr, r);
        break;
    case '5':  // add integer in 2's Complement
        cpu.alu.add5(instr, r);
        break;
    case '6':
        cpu.alu.add6(instr, r); //add float
        break;
    case 'B':
        cpu.cu.jump(instr, r, m, cpu);
        break;
    case 'C':  // Halt
        cpu.cu.halt();
        break;
    default:
        std::cout << "Unknown opcode: " << instr.opcode << std::endl;
        cu.halt(); // Halt on unknown opcode
    }
}


// Method to check if the CPU is still running
bool CPU::isRunning() const {
    return running;
}

// Method to stop the CPU
void CPU::stop() {
    running = false;
}

