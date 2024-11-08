#include <iostream>
#include "Program.h"

int main() {

    // Create instances of Memory, Register, and CPU
    Memory memory;
    Register registers;
    CPU cpu;
    string fileName;
    Screen S;
    int choice;
    while (true) {
        cout << "For Run as a step Enter 1: " << endl;
        cout << "For Run as a whole Enter 2: " << endl;
        cout << "To Exit the program Enter 0: " << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        while (true) {
            if (choice == 1 || choice == 2) {
                while (true) {
                    cout << "Enter the file: ";
                    cin >> fileName;

                    ifstream testFile(fileName);

                    if (testFile) {
                        memory.loadFromFile(fileName);
                        break;
                    }
                    else {
                        std::cerr << "Error: File not found! Please try again." << std::endl;
                    }
                }
                if (choice == 1) {
                    // Run the CPU until it encounters the halt instruction (C000)
                    while (cpu.isRunning()) {
                        try {
                            // Fetch and decode the instruction
                            Instruction instruction = cpu.fetch_Decode(memory, registers);
                            // Execute the instruction
                            cpu.execute(instruction, registers, cpu, memory, S);

                            // Print memory and registers for debugging
                            memory.printMemory();
                            registers.printRegisters();
                            S.printScreenHex();
                            S.printScreenASCI();

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
                    break;
                }
                else if (choice == 2) {
                    // Run the CPU until it encounters the halt instruction (C000)
                    while (cpu.isRunning()) {
                        try {
                            // Fetch and decode the instruction
                            Instruction instruction = cpu.fetch_Decode(memory, registers);
                            // Execute the instruction
                            cpu.execute(instruction, registers, cpu, memory, S);

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
                    S.printScreenHex();
                    S.printScreenASCI();

                }
                break;
            }
            else if (choice == 0) {
                 cout << "Thanks for using our program \nExiting Program..." << endl;
                 exit(0);
            }
               
            else {
                cout << "Invalid input . PLZ enter a valid number" << endl;
                cin >> choice;
            }
        }


    }
    return 0;
}
