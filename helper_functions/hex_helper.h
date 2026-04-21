/*
* Purpose: hex_helper.h source file
* Authors: Steph Huynh (cssc2524)
*          Isabelle Viraldo (cssc2555) (828115945)
* Class Info: CS 530, Spring 2026
* Assignment Info: Assignment #2, Limited XE Assembler
*
* Description: Gives us helper functions to handle hex conversions
*/

#ifndef HEX_HELPER_H
#define HEX_HELPER_H

#include <cstdint>
#include <string>

using namespace std;


// Returns true if operand looks like X'...'
bool isHexLiteral(const string& operand);

// Returns true if operand looks like C'...'
bool isCharLiteral(const string& operand);

// Converts an operand into an integer value.
int parseOperandValue(const string& operand);

// Returns how many bytes a BYTE directive uses.
int getByteDirectiveLength(const string& operand);

// Converts an integer to a hexadecimal string
std::string toHexString(uint32_t value, int width);

// Converts a BYTE operand to integer value
int byteObjectValue(const string& operand);

#endif