/*
* Purpose: read_input.cpp source file
* Author: Steph Huynh and Isabelle Viraldo
* Description: parses input file into: label, opcode, operand
*
*
*
*/

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "read_input.h"
#include "hex_helpers.h"
#include "address_helper.h"
using namespace std;


ParseInput parseSicLine (string line){
	ParseInput p;
	p.original = line;	// keep copy of original line being read

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
	//TODO: change this so if it says X'F' then the value is just F (in hex) but if it is just a number (#100 or 100 or whatever, then it will pass the number value decimal -> hexidecimal)

	input >> p.operand;

	// use address_helpers to flag addressing modes
	p.cleanedOperand = cleanOperand(p.operand);		// get rid of '#', '@', and ',X'
	p.isIndexed = isIndexedOperand(p.operand);		// True if ',X'
	p.isImmediate = isImmediateOperand(p.operand);		// True if '#'	
	p.isIndirect = isIndirectOperand(p.operand);		// true if '@'
	
	// check for hex literals using functions from hex_helpers
	if (isHexLiteral(p.operand)) {
		// get raw hex string and convert to an int and format to pure hex (Reduces errors)
		int decVal = parseOperandValue(p.operand);
		p.hex = toHexString(decVal, 1);
	}
	// check for immediate constants ("#")
	else if (isImmediateConstantOperand(p.operand)) {
		int decVal = 0;
		immediateAddress(p.operand, decVal);	// get number val
		p.hex = toHexString(decVal, 4);		// convert to 4 digit hex string

	}
	// handles immediate symbols/labels or normal symbols/registers
	else {
		p.hex = p.cleanedOperand;
	}

	return p;
}
