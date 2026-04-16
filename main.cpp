/*
* Purpose: main.cpp
* Authors: Steph Huynh(cssc2524) and Isabella Viraldo(cssc2555)
* Description: where our main program runs from.
*/
#include <iostream>
#include <fstream>
#include <string>
#include "read_input.h"
#include "Opcode_Dictionary.h"
#include "pass1.h"
using namespace std;


int main(int argc, char* argv[]) {

	// no input files given
	if (argc < 2)
	{
		cerr << "lxe: no input file given.\n";
		cerr << "Terminating program" << endl;
		return 1;
	}

	// Grab all input files by looping	
	for (int i = 1; i < argc; i++)
	{
		string filename = argv[i];

		// begin pass 1 to read file and such
		runPass1(filename);

		//// open file to read 
		//ifstream inputFile(filename.c_str());

		////error handling for input file
		//if (!inputFile) {
		//	cerr << "Error: could not open input file " << filename << endl;
		//	cerr << "Terminating program" << endl;
		//	return 1;

		//}

		//cout << "Processing " << filename << endl;
		//string line;

		//// read file line by line until none left
		//while (getline(inputFile, line)) {
		//	// put pass 1 and 2 info here (or function calls)
		//	ParseInput parsedLine = parseSicLine(line);
		//	// skip empty lines
		//	if (parsedLine.emptyComment == true) {
		//		continue;
		//	}
		//	cout << "label = " << parsedLine.label << endl;
		//	cout << "opcode = " << parsedLine.opcode << endl;
		//	cout << "operand = " << parsedLine.operand << endl;

		//	opSpecs currentInstruction;
		//	if (getOpcodeSpecs(parsedLine.opcode, currentInstruction)) {
		//		cout << "Valid instruction found. Hex: " << hex << currentInstruction.opcode
		//			<< " | Format: " << dec << currentInstruction.fmt << endl;
		//	}
		//	else {
		//		cout << "not a valid instruction/directive\n";
		//	}
		//}
	}
	return 0;
}
