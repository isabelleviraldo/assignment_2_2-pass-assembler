/*
* Purpose: hex_helper.cpp source file
* Authors: Steph Huynh (cssc2524) (824058671)
*          Isabelle Viraldo (cssc2555) (828115945)
* Class Info: CS 530, Spring 2026
* Assignment Info: Assignment #2, Limited XE Assembler
*
* Description: Gives us helper functions to handle hex conversions
*/

#include "hex_helper.h"

#include <cctype>
#include <iomanip>
#include <sstream>
#include <stdexcept>

using namespace std;

// Returns true if operand looks like X'...'
bool isHexLiteral(const string& operand) {
    return operand.length() >= 4 &&
           operand[0] == 'X' &&
           operand[1] == '\'' &&
           operand[operand.length() - 1] == '\'';
}

// Returns true if operand looks like C'...'
bool isCharLiteral(const string& operand) {
    return operand.length() >= 4 &&
           operand[0] == 'C' &&
           operand[1] == '\'' &&
           operand[operand.length() - 1] == '\'';
}

// Converts an operand into an integer value.
int parseOperandValue(const string& operand) {
    if (isHexLiteral(operand)) {
        string hexDigits = operand.substr(2, operand.length() - 3);

        for (char c : hexDigits) {
            if (!isxdigit(static_cast<unsigned char>(c))) {
                throw invalid_argument("Invalid hex digit in operand: " + operand);
            }
        }

        return stoi(hexDigits, nullptr, 16);    // convert string to hex/base 16
    }

    return stoi(operand);   // if not hex, convert to base 10/decimal
}

// Returns how many bytes a BYTE directive uses.
int getByteDirectiveLength(const string& operand) {
    if (isHexLiteral(operand)) {
        string hexDigits = operand.substr(2, operand.length() - 3);

        for (char c : hexDigits) {
            if (!isxdigit(static_cast<unsigned char>(c))) {
                throw invalid_argument("Invalid hex digit in BYTE operand: " + operand);
            }
        }

        if (hexDigits.length() % 2 != 0) {
            throw invalid_argument("BYTE hex literal must have even number of hex digits: " + operand);
        }

        return hexDigits.length() / 2;
    }

    if (isCharLiteral(operand)) {
        string chars = operand.substr(2, operand.length() - 3);
        return chars.length();
    }

    throw invalid_argument("Invalid BYTE operand format: " + operand);
}

// Converts an integer to a hexadecimal string
string toHexString(uint32_t value, int width) {
    stringstream ss;
    ss << uppercase << hex << setw(width) << setfill('0') << value;
    return ss.str();
}

// Converts a BYTE operand to integer value
int byteObjectValue(const string& operand) {
    if (isHexLiteral(operand)) {
        string hexDigits = operand.substr(2, operand.length() - 3);
        return stoi(hexDigits, nullptr, 16);
    }

    if (isCharLiteral(operand)) {
        string chars = operand.substr(2, operand.length() - 3);

        int value = 0;
        for (char c : chars) {
            value = (value << 8) | static_cast<unsigned char>(c);
        }
        return value;
    }

    throw invalid_argument("invalid BYTE operand");
}