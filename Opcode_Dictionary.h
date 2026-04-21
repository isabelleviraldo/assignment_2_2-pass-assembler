/*
* Purpose: Opcode_Dictionary.h source file
* Authors: Steph Huynh (cssc2524) (824058671)
*          Isabelle Viraldo (cssc2555) (828115945)
* Class Info: CS 530, Spring 2026
* Assignment Info: Assignment #2, Limited XE Assembler
*
* Description: Contains opcode spec info in form of unordered_map
*/

#ifndef OPCODE_DICTIONARY_H
#define OPCODE_DICTIONARY_H

#include <string>

using namespace std;

struct opSpecs {
	int opcode;
	int fmt;
};


bool getOpcodeSpecs(const string& mnemonic, opSpecs& specs);
bool isDirective(const string& mnemonic);
bool getRegisterNumber(const string& regName, int& regNum);

#endif