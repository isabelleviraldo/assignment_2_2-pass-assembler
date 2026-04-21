/*
* Purpose: read_input.h source file
* Authors: Steph Huynh (cssc2524) (824058671)
*          Isabelle Viraldo (cssc2555) (828115945)
* Class Info: CS 530, Spring 2026
* Assignment Info: Assignment #2, Limited XE Assembler
*
* Description: parses input file into: label, opcode, operand
*/

#ifndef READ_INPUT_H
#define READ_INPUT_H

#include <string>

using namespace std;

struct ParseInput {
	bool emptyComment = false;
	int lineCt = 0;

	// variables to hold parsed information
	string original;
	string label;
	string opcode;
	string operand;

	// hold info from helper functions
	string hex;
	string cleanedOperand;
	bool isIndexed = false;
	bool isImmediate = false;
	bool isIndirect = false;
};
ParseInput parseSicLine (string line);



#endif
