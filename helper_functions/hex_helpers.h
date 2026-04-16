#ifndef HEX_HELPERS_H
#define HEX_HELPERS_H

#include <string>
#include <cstdint>

using namespace std;


// Converts an operand into an integer value.
int parseOperandValue(const string& operand);

// Returns how many bytes a BYTE directive uses.
int getByteDirectiveLength(const string& operand);

// Returns true if operand looks like X'...'
bool isHexLiteral(const string& operand);

// Returns true if operand looks like C'...'
bool isCharLiteral(const string& operand);

string toHexString(uint32_t value, int width);
int byteObjectValue(const string& operand);

#endif