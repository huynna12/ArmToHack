#include "ArmToHack.h"
#include "token_io.h"
#include <sstream>
#include <iomanip>
using namespace std;

ArmToHack::ArmToHack() : lineNumber(0) {}

// Reset the translator whenever we translate a new file
void ArmToHack::reset() {
    lineNumber = 0;
    label_toAddress.clear();
    address_toLabel.clear();
}

// Write a line to the output file
void ArmToHack::write_line(const string& line) {
    outFile << line << "\n";
    lineNumber++;
}

// Translate the ARM assembly code to Hack assembly code by calling 2 passes
void ArmToHack::translate(const string& fIn, const string& fOut) {
    reset();
    translateFirstPass(fIn, fIn + ".tmp");
    translateSecondPass(fIn + ".tmp", fOut);
}

// The first pass
void ArmToHack::translateFirstPass(const string& fIn,const string& fOut) {
    inFile.open(fIn);
    outFile.open(fOut);

    string line = read_line(inFile);
    string second = peek_second(line);

    // Read until the end of the file
    while (line != "") {
        translate(line);
        line = read_line(inFile);
    }

    inFile.close();
    outFile.close();
}

// The translate function determines the type of instruction
void ArmToHack::translate(string& line) {
    // Determine the instruction type and call the appropriate method
    string first = peek_first(line);
    string second = peek_second(line);

    // Handle MOV, ADD, SUB, RSB, CMP, and END instructions
    if (first == "MOV") {
        translateMOV(line);
    } 
    else if (first == "ADD" || first == "SUB" || first == "RSB") {
        translateADD_SUB_RSB(line);
    } 
    else if (first == "CMP") {
        translateCMP(line);
    } 
    else if (first == "END") {
        translateEND(line);
    
    } 
    // Handle jumps
    else if (jumpMap.find(first) != jumpMap.end()) {
        translateJumps(line);
    } 
    // Handle labels
    else {
        if (second == "") {
            label_toAddress[first] = to_string(lineNumber);
        }
    }
}

// Write the operand2 field for a given token
void ArmToHack::write_oper2(string& token) {
    // If the token is a register
    if (registerMap.find(token) != registerMap.end()) {
        string regValue = registerMap[token];
        write_line("@" + regValue);
        write_line("D=M"); 
    } 
    else {
        // If the value is negative
        bool isNegative = (token.length() > 1 && token[1] == '-');

        // Strip '#' and '+' or '-' signs
        strip(token, "#,+-");

        // Load the value into A
        write_line("@" + token);  

        if (isNegative) {
            write_line("D=-A");  // D = -value
        }
        else {
            write_line("D=A");  // D = value
        }
    }
}

// Translate MOV instruction
void ArmToHack::translateMOV(string& line) {
    //MOV
    extract_token(line); 
    
    // Get the destination and source registers
    string dest = extract_token(line);
    string src = extract_token(line);

    // Get the value of src
    write_oper2(src);

    // Store the value in the destination register
    write_line("@" + registerMap[dest]);
    write_line("M=D"); 

    // If the destination is PC, handle jump 
    if (dest == "PC") {
        write_pcjump(); 
    }
}

// Add src2 from src1 and store in dest
void ArmToHack::translateADD_SUB_RSB(string& line) {
    // ADD or SUB or RSB
    string first = extract_token(line); 

    // Get the destination and source registers
    string dest = extract_token(line);
    string src1 = extract_token(line);
    string src2 = extract_token(line);

    // Get the value of src2
    write_oper2(src2);

    write_line("@" + registerMap[src1]);

    if(first == "ADD") {
        write_line("D=D+M"); // D = src1 + src2
    } 
    else if (first == "SUB") {
        write_line("D=M-D"); // D = src1 - src2
    } 
    else if (first == "RSB") {
        write_line("D=D-M"); // D = src2 - src1
    }

    // Store the result in the destination register
    write_line("@" + registerMap[dest]);
    write_line("M=D"); 

    // If the destination is PC, handle jump 
    if (dest == "PC") {
        write_pcjump();
    }
}

// Compare src1 and src2 and store the 
void ArmToHack::translateCMP(string& line) {
    extract_token(line); // CMP
    string reg    = extract_token(line);
    string oper_2 = extract_token(line);

    write_oper2(oper_2);
    write_line("@" + registerMap[reg]);
    write_line("D=M-D"); // D = reg - oper_2
}

// If the label is already defined, we write the address
// Otherwise, we store the label in address_toLabel and write -1
void ArmToHack::writeJumpAddress(string& label) {
    if(label_toAddress.find(label) != label_toAddress.end()) {
        write_line("@" + label_toAddress[label]); 
    } 
    else {
        address_toLabel[lineNumber] = label;
        write_line("@" + to_string(-1));
    }
}

// Translate jumps
void ArmToHack::translateJumps(string& line) {
    string jumpCmd = extract_token(line); // e.g., BNE
    string label = extract_token(line);   // label

    if(jumpCmd == "BL" || jumpCmd == "BAL") {
        if(jumpCmd == "BL") {
            // 6 because BL takes 6 lines
            write_line("@" + to_string(lineNumber + 6)); // Address of the next instruction
            write_line("D=A"); // Store the address in D
            write_line("@" + registerMap["LR"]); // Store the return address in LR
            write_line("M=D");
        }

        writeJumpAddress(label);
        write_line("0;JMP"); // Unconditional jump to the label
    }
    else {
        writeJumpAddress(label); 
        write_line("D;" + jumpMap[jumpCmd]);
    }
}

// Translate the END instruction
void ArmToHack::translateEND(string& line) {
    // END    
    extract_token(line); 

    // Address of the next instruction
    int nextLine = lineNumber + 1;
    write_line("@" + to_string(nextLine)); 
    write_line("0;JMP"); 
}

// The second pass translates the labels to their corresponding addresses
void ArmToHack::translateSecondPass(const string &fIn, const string &fOut) {
    //Reset the line number
    lineNumber = 0;

    inFile.open(fIn);
    outFile.open(fOut);

    string line = read_line(inFile);

    // Read until the end of the file
    while (line != "") {
        // If the line contains valid instruction
        if(address_toLabel.find(lineNumber) == address_toLabel.end()) {
            write_line(line);
        } 
        // If the line needs to be corrected
        else { 
            string label = address_toLabel[lineNumber];
            write_line("@" + label_toAddress[label]); // Load the address of the label

        }
        line = read_line(inFile);
    }

    inFile.close();
    outFile.close();
}

// The function write_pcjump() is used to handle the PC register.
void ArmToHack::write_pcjump() {
    write_line("@" + registerMap["PC"]);
    write_line("A=M");
    write_line("0;JMP");
}
