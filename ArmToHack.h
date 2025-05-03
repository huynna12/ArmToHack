#ifndef ARM_TO_HACK_H_ 
#define ARM_TO_HACK_H_ 

#include <string>
#include <map>
#include <fstream>
#include "token_io.h"
using namespace std;

// The ArmToHack class is to translate ARM assembly code into Hack assembly code.
class ArmToHack {
private:
    ifstream inFile; // Input file stream for reading ARM assembly code.
    ofstream outFile; // Output file stream for writing Hack assembly code.
    int lineNumber = 0; // Tracks the current line number during translation.

    // Maps ARM registers to their Hack equivalents.
    map<string, string> registerMap = {
        {"R0", "0"}, {"R1", "1"}, {"R2", "2"}, {"R3", "3"}, 
        {"R4", "4"}, {"R5", "5"}, {"R6", "6"}, {"R7", "7"}, 
        {"R8", "8"}, {"R9", "9"}, {"R10", "10"}, {"R11", "11"},
        {"R12", "12"}, {"R13", "13"}, {"R14", "14"}, {"R15", "15"},
        {"FP", "12"}, {"SP", "13"}, {"LR", "14"}, {"PC", "15"}
    };

    // Maps ARM jump instructions to their Hack equivalents.
    map<string, string> jumpMap = {
        {"BEQ", "JEQ"}, {"BNE", "JNE"},
        {"BLT", "JLT"}, {"BLE", "JLE"},
        {"BGT", "JGT"}, {"BGE", "JGE"},
        {"BL", "JMP"}, {"BAL", "JMP"}
    };

    // Maps labels to their corresponding memory addresses.
    map<string, string> label_toAddress;

    // Maps memory addresses to their corresponding labels.
    map<int, string> address_toLabel;

public:
    // Constructor: Initializes the ArmToHack object.
    ArmToHack();

    // Resets the internal state of the translator.
    void reset();

    // Writes a single line of translated Hack assembly code to the output file.
    void write_line(const string& line);

    // Translates an ARM assembly file (fIn) into a Hack assembly file (fOut).
    void translate(const string& fIn, const string& fOut);

private:
    // Performs the first pass of translation, identifying labels and addresses.
    void translateFirstPass(const string& fIn, const string& fOut);

    // Translates a single line of ARM assembly code into Hack assembly code.
    void translate(string& line);

    // Write the operand2 field for a given token
    void write_oper2(string& token);

    // Translates MOV instructions from ARM to Hack assembly.
    void translateMOV(string& line);

    // Translates ADD, SUB, and RSB instructions from ARM to Hack assembly.
    void translateADD_SUB_RSB(string& line);

    // Translates CMP instructions from ARM to Hack assembly.
    void translateCMP(string& line);

    // Handles translation of END instructions.
    // (Create a infinite loop)
    void translateEND(string& line);

    // If the label is already defined, we write the address
    // Otherwise, we store the label in address_toLabel and write -1
    void writeJumpAddress(string& label);

    // Translates jump instructions from ARM to Hack assembly.
    void translateJumps(string& line);

    // Performs the second pass of translation, 
    // resolving labels and jumps.
    void translateSecondPass(const string& fIn, const string& fOut);

    // Writes a PC-relative jump instruction.
    void write_pcjump();
};

#endif