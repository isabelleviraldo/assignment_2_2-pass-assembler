/*
* Purpose: test_parser.cpp
* *Authors: Steph Huynh (cssc2524) and Isabelle Viraldo (cssc2555)
* Class Info: CS 530, Spring 2026
* Assignment Info: Assignment #2, Limited XE Assembler
* Description: verify parsing and opcode lookup
* independently from the main Pass 1 and Pass 2 workflow.
*/
#include <iostream>
#include <fstream>
#include <string>

#include "helper_functions/read_input.h"
#include "Opcode_Dictionary.h"

using namespace std;

int main(int argc, char* argv[]) {
    // no input files given
    if (argc < 2) {
        cerr << "Usage: ./test_parser <filename.sic>\n";
        cerr << "Terminating program" << endl;
        return 1;
    }

    string filename = argv[1];

    // open file to read 
    ifstream inputFile(filename.c_str());

    // error handling for input file
    if (!inputFile) {
        cerr << "Error: could not open input file " << filename << endl;
        cerr << "Terminating program" << endl;
        return 1;
    }

    cout << "Processing " << filename << endl;
    string line;

    // read file line by line until none left
    while (getline(inputFile, line)) {

        ParseInput parsedLine = parseSicLine(line);

        // skip empty lines
        if (parsedLine.emptyComment == true) {
            continue;
        }

        cout << "label = [" << parsedLine.label << "]" << endl;
        cout << "opcode = [" << parsedLine.opcode << "]" << endl;
        cout << "operand = [" << parsedLine.operand << "]" << endl;

        opSpecs currentInstruction;
        if (getOpcodeSpecs(parsedLine.opcode, currentInstruction)) {
            cout << "Valid instruction found. Hex: " << hex << currentInstruction.opcode
                << " | Format: " << dec << currentInstruction.fmt << endl;
        }
        else {
            cout << "not a valid instruction/directive\n";
        }
        cout << "-----------------------------------" << endl;
    }

    return 0;
}