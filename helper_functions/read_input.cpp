/*
* Purpose: read_input.cpp source file
* Authors: Steph Huynh (cssc2524) (824058671)
*          Isabelle Viraldo (cssc2555) (828115945)
* Class Info: CS 530, Spring 2026
* Assignment Info: Assignment #2, Limited XE Assembler
*
* Description: parses input file into: label, opcode, operand
*/

#include "read_input.h"

#include <sstream>
#include <string>

#include "address_helper.h"
#include "hex_helper.h"

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

	input >> p.operand;

	// use address_helpers to flag addressing modes
	p.cleanedOperand = cleanOperand(p.operand);		// get rid of '#', '@', and ',X'
	p.isIndexed = isIndexedOperand(p.operand);		// True if ',X'
	p.isImmediate = isImmediateOperand(p.operand);		// True if '#'	
	p.isIndirect = isIndirectOperand(p.operand);		// true if '@'
	
	// check for hex literals using functions from hex_helper
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
