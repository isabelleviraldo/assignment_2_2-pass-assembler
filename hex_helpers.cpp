#include "hex_helpers.h"
#include <stdexcept>
#include <cctype>

using namespace std;

bool isHexLiteral(const string& operand) {
    return operand.length() >= 4 &&
           operand[0] == 'X' &&
           operand[1] == '\'' &&
           operand[operand.length() - 1] == '\'';
}

bool isCharLiteral(const string& operand) {
    return operand.length() >= 4 &&
           operand[0] == 'C' &&
           operand[1] == '\'' &&
           operand[operand.length() - 1] == '\'';
}

int parseOperandValue(const string& operand) {
    if (isHexLiteral(operand)) {
        string hexDigits = operand.substr(2, operand.length() - 3);

        for (char c : hexDigits) {
            if (!isxdigit(static_cast<unsigned char>(c))) {
                throw invalid_argument("Invalid hex digit in operand: " + operand);
            }
        }

        return stoi(hexDigits, nullptr, 16);
    }

    return stoi(operand);
}

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