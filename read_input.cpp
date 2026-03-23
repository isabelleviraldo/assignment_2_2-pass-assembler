#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "ParseInput.h"
using namespace std;


ParseInput parseSicLine (string line){
	ParseInput p;
	original = line;	// keep copy of original line being read

	// if line doesn't contain anything
	if (line.empty()){
		return p;
	}

	// open input stream
	istringstream input(line);

	// if line is empty or contains spaces/tabs, skip line
	// find_first_not_of returns "no positions" if so
	if (line.find_first_not_of(" \t\r\n") == string::npos){
		p.emptyComment = true;	// ensure line is ignored
		return p;
	}


	// use isspace to filter out any whitespaces (in our .sic document, they show up as tabs
	if (!isspace(line[0])) {
		
		// get label and opcode given it doesn't begin with whitespace
		input >> p.label;
		input >> p.opcode;
	}	
	else {
		// lines without labels (aka space at line[0]) --> get next string, which should be the opcode
		input >> p.opcode;
	}
	
	// grab the operand
	input >> p.operand;

	// Testing to print and ensure results are correctly parsed
	cout << "label = " << p.label << endl;
	cout << "opcode = " << p.opcode << endl;
	cout << "operand = " << p.operand << endl;
	// use original comment to do tests 
	// ADD UNIT TESTS HERE

	return p;
}
