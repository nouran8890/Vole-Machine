// VoleMachine2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Program.h"

int main() {
    // Create instances of Memory, Register, and CPU
    Memory memory;
    Register registers;
    CPU cpu;
    string namefile;
    cout << "Enter the file : ";
    cin >> namefile;
    // Load instructions from a file into memory
    memory.loadFromFile(namefile); // Replace with your actual file path

    // Set the program counter to the starting address (0 in this case)
    //cpu.setProgCounter(0);

    // Run the CPU until it encounters the halt instruction (C000)
    while (cpu.isRunning()) {
        try {
            // Fetch and decode the instruction
            Instruction instruction = cpu.fetch_Decode(memory, registers);
            // Execute the instruction
            cpu.execute(instruction, registers, cpu);

            // Check if the instruction is C000 to stop the program
            if (instruction.opcode == 'C' && instruction.address == 0) {
                std::cout << "Reached C000 instruction. Halting the program." << std::endl;
                break;
            }
        }
        catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            break; // Exit loop on error
        }
    }

    // Print memory and registers for debugging
    memory.printMemory();
    registers.printRegisters();

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
