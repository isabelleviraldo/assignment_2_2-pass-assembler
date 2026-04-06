/*
* Purpose: pass1.cpp source file
* Author: Steph Huynh and Isabelle Viraldo
* Description: processes pass 1 by reading .sic file and parsing each line.
* Creates .st file after it has processed all the directives and calculated LOCCTR
*
*/

#include <string>
#include <iostream>
#include "pass1.h"
#include "read_input.h"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <unordered_map>
#include "Opcode_Dictionary.h"
#include "hex_helpers.h"
using namespace std;


void runPass1(const string& filename) {
	// open file to read 
	ifstream inputFile(filename.c_str());

	//error handling for input file
	if (!inputFile) {
		cerr << "Error: could not open input file " << filename << endl;
		cerr << "Terminating program" << endl;
		return;

	}

	cout << "Processing " << filename << endl;

	//creating symTab
	unordered_map<string, int> symTab;

	//tracking variables
	int LOCCTR = 0;
	string line;

	// read file line by line until none left
	while (getline(inputFile, line)) {
		// put pass 1 and 2 info here (or function calls)
		ParseInput parsedLine = parseSicLine(line);

		//TEST PRINTOUTS
		cout << "\nOriginal line: " << parsedLine.original << endl;
		cout << "Label:   [" << parsedLine.label << "]" << endl;
		cout << "Opcode:  [" << parsedLine.opcode << "]" << endl;
		cout << "Operand: [" << parsedLine.operand << "]" << endl;

		// skip empty lines
		if (parsedLine.emptyComment == true) {
			continue;
		}

		string searchLine = parsedLine.opcode;

		//keep track of format 4/extended version
		bool fmt4 = false;
		
		// handle "+" of string
		if (!searchLine.empty() && searchLine[0] == '+') {
			searchLine = searchLine.substr(1);
			fmt4 = true;
		}

		// intialize START if given
		if (searchLine == "START") {
			// change string --> int and initialize starting variable with starting address
			LOCCTR = stoi(parsedLine.operand, nullptr, 16);
		}
	
		// add label and location counter to symTab
		if (!parsedLine.label.empty()) {
			symTab[parsedLine.label] = LOCCTR;
		}
	
		opSpecs currInstruction;
				
			if (getOpcodeSpecs(searchLine, currInstruction)) {
				if (fmt4) {
					LOCCTR += 4;
				}
				else {
					LOCCTR += currInstruction.fmt;
				}
			}
			else if (searchLine != "START" && searchLine != "END" && searchLine != "BASE") {
				if (searchLine == "WORD") {
					LOCCTR += 3;
				}
				else if (searchLine == "RESW") {
					LOCCTR += 3 * parseOperandValue(parsedLine.operand);
				}
				else if (searchLine == "RESB") {
					LOCCTR += parseOperandValue(parsedLine.operand);
				}
				else if (searchLine == "BYTE") {
					LOCCTR += getByteDirectiveLength(parsedLine.operand);
				}
			}

	}
	// Testing:
	cout << "\nFinal Hash Map State \n";
	for (const auto& pair : symTab) {
		cout << "Key: " << pair.first << "\t | LOCCTR: " << pair.second << "\n";
	}
}