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
#include "helper_functions/read_input.h"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <unordered_map>
#include "Opcode_Dictionary.h"
#include "helper_functions/hex_helpers.h"
#include "helper_functions/address_helper.h"
#include <vector>
using namespace std;


void runPass1(const string& filename) {
	// open file to read 
	ifstream inputFile(filename.c_str());

	//error handling for input file
	if (!inputFile) {
		cerr << "Error: could not open input file " << filename << endl;
		cerr << "Terminating program" << endl;
		return;		// exit program if file DNE

	}

	cout << "Processing " << filename << endl;

	//creating symTab	
	unordered_map<string, int> symTab;	// store labels and addresses
	vector <Literal> littab;	// store literals (handles =C'EOF') waiting for addresses
	vector<lineInfo> sicData;	// helps pass2 as it saves the parsed lines

	//tracking variables
	int LOCCTR = 0;		
	int startAddr = 0;	// when program begins
	string csect = "NA";	// control section name (program name)
	string line;

	// read file line by line until none left
	while (getline(inputFile, line)) {
		// parse each line into label, code, opcode
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

		// keep track of current instruction address/specs before LOCCTR continues
		int currentAddr = LOCCTR;
		string searchLine = parsedLine.opcode;

		//keep track of format 4/extended version
		bool fmt4 = false;
		
		// handle "+" of string - extended format 4 if it starts with it
		if (!searchLine.empty() && searchLine[0] == '+') {
			searchLine = searchLine.substr(1);	// remove the '+' to be able to look up in dictionary
			fmt4 = true;
		}

		// intialize START directive
		if (searchLine == "START") {
			csect = parsedLine.label;		// save program name
			LOCCTR = stoi(parsedLine.hex, nullptr, 16);		// set starting address from hex string
			startAddr = LOCCTR;		// remember start --> useful for length of program later
			symTab[csect] = LOCCTR;		// add program naem to symTab

			// save line for pass 2 and continue to next line
			sicData.push_back({ LOCCTR, parsedLine });
			continue;
		}

		 //start LITTAB (literals such as c'eof'
		if (!parsedLine.operand.empty() && parsedLine.operand[0] == '=') {
			string body = parsedLine.operand.substr(1);		// take out '='
			bool exists = false;

			// check if we already have this literal
			for (const auto& l : littab) {
				if (l.name == parsedLine.operand) {
					exists = true;
					break;
				}
			}

			// if it's new then add it to literal pool
			if (!exists) {
				Literal newLit;
				newLit.name = parsedLine.operand;

				// get value as an int and calculate how many bytes it would take
				int value = byteObjectValue(parsedLine.operand.substr(1));
				newLit.hexData = toHexString(value, getByteDirectiveLength(parsedLine.operand.substr(1)) * 2);
				newLit.length = getByteDirectiveLength(parsedLine.operand.substr(1));
				// -1 means it's waiting to get to END to be able to get address
				newLit.address = -1;	
				littab.push_back(newLit);
			}
		}
	
		// add label/symbols and location counter to symTab
		if (!parsedLine.label.empty() && parsedLine.label != "*") {
			// prevent duplicate labels --> only add if it's not in there already
			if (symTab.find(parsedLine.label) == symTab.end()) {
				symTab[parsedLine.label] = LOCCTR;
			}
			else {
				cerr << "Error: duplicate label found (not allowed) [" << parsedLine.label << "]" << endl;
			}

		}
		
		// update LOCCTR
		opSpecs currInstruction;
			
			// if it's standard instruction, use dictionary
			if (getOpcodeSpecs(searchLine, currInstruction)) {
				if (fmt4) {
					LOCCTR += 4;	// add 4 bytes to indicate fmt 4
				}
				else {
					// if not, use dictionary format
					LOCCTR += currInstruction.fmt;
				}
			}
			//if not instruction, check if it's a directive
			else if (searchLine != "START" && searchLine != "END" && searchLine != "BASE") {
				if (searchLine == "WORD") {
					LOCCTR += 3;	// WORD = 3 bytes
				}
				else if (searchLine == "RESW") {
					// use stoi on cleanedOperand because we need base 10 int for this
					LOCCTR += 3 * stoi(parsedLine.cleanedOperand);	// 3 bytes per reserved word
				}
				else if (searchLine == "RESB") {
					// use stoi on cleanedOperand because we need base 10 for this
					LOCCTR += stoi(parsedLine.cleanedOperand);	// 1 byte per reserved byte
				}
				else if (searchLine == "BYTE") {
					// ok to use
					LOCCTR += getByteDirectiveLength(parsedLine.operand);	
				}
				// if it's from LIT TAB/pool
				else if (searchLine == "LTORG") {
					// give addresees to all literals
					for (auto& l : littab) {
						if (l.address == -1) {
							l.address = LOCCTR;	
							LOCCTR += l.length;		// move LOCCTR forward by literal's size
						}
					}
				}
			}
			// helps pass 2 so it doesn't need to reread text
			sicData.push_back({ currentAddr, parsedLine });

	}
	for (auto& l : littab) {
		if (l.address == -1) {
			l.address = LOCCTR;
			LOCCTR += l.length;		// move LOCCTR forward by literal's size
		}
	}
	// Testing:
	cout << "\nFinal Hash Map State \n";
	for (const auto& pair : symTab) {
		cout << "Key: " << pair.first << "\t | LOCCTR: " << pair.second << "\n";
	}

	// creating .st (symTab) file
	string outputFile = filename.substr(0, filename.find_last_of(".")) + ".st";
	ofstream st(outputFile.c_str());

	if (!st) {
		cerr << "Error: could not create symtab file (.st)" << outputFile << endl;
	}
	else {
		int progLength = LOCCTR - startAddr;	// length of program

		// symtab section titles/headers (following example on canvas)
		st << left << setw(8) << "CSect"
			<< setw(10) << "Symbol"
			<< setw(10) << "Value"
			<< setw(10) << "LENGTH"
			<< "Flags:" << endl;
		st << "-----------------------------------------" << endl;

		// control section 
		st << left << setw(8) << csect
			<< setw(10) << ""
			<< toHexString(startAddr, 6) << "  "
			<< toHexString(progLength, 6) << endl;

		// adding each symbol from symTab
		for (const auto& pair : symTab) {
			if (pair.first == csect) continue;	// skip control program section
			st << left << setw(8) << "" << setw(10) << pair.first
				<< toHexString(pair.second, 6) << "             R" << endl;		

		}

		// Print Literal Table
		st << endl << "Literal Table" << endl;
		st << left << setw(10) << "Name" << setw(12) << "Operand" << setw(10) << "Address" << "Length:" << endl;
		st << "-----------------------------------------------" << endl;

		for (const auto& l : littab) {
			// remove '='
			string nameDisp = l.name.substr(1);
			// clean up displayed name
			if (nameDisp.size() > 3) nameDisp = nameDisp.substr(2, nameDisp.size() - 3);

			st << left << setw(10) << nameDisp << setw(12) << l.hexData
				<< toHexString(l.address, 6) << "    " << dec << l.length << endl;
		}
		st.close();
		cout << "Success: Symbol table created at " << outputFile << endl;



	}
}