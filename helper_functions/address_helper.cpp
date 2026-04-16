/*
* Purpose: address_helper .cpp source file
* Authors: Steph Huynh (cssc2524) and Isabelle Viraldo (cssc2555)
* Class Info: CS 530, Spring 2026
* Assignment Info: Assignment #2, Limited XE Assembler
* Description: determining PC relative, base relative for pass 2
*/

#include "address_helper.h"

#include <cctype>
#include <stdexcept>

using namespace std;

// removes formatting characters so we can look up symbol names
string cleanOperand(const string& operand) {
    string result = operand;

    if (!result.empty() && (result[0] == '#' || result[0] == '@')) {
        result = result.substr(1);
    }

    size_t commaPos = result.find(",X");
    if (commaPos != string::npos) {
        result = result.substr(0, commaPos);
    }

    return result;
}

// true of instruction is indexed
bool isIndexedOperand(const string& operand) {
    return operand.find(",X") != string::npos;
}

// true if instruction is indirect
bool isIndirectOperand(const string& operand) {
    if (operand.empty()) {
        return false;
    }

    return operand[0] == '@';
}

// true if immediate
bool isImmediateOperand(const string& operand) {
    if (operand.empty()) {
        return false;
    }
    return operand[0] == '#';
}

// true uf immediate and strictly checks immediate CONSTANT
bool isImmediateConstantOperand(const string& operand) {
    if (!isImmediateOperand(operand)) {
        return false;
    }

    string cleaned = cleanOperand(operand);
    if (cleaned.empty()) {
        return false;
    }

    for (char c : cleaned) {
        if (!isdigit(static_cast<unsigned char>(c))) {
            return false;
        }
    }

    return true;
}


// search symTab; true if found and updates address reference
bool lookupSymbolAddress(
    const string& operand,
    const unordered_map<string, int>& symTab,
    int& address
) {
    string cleaned = cleanOperand(operand);

    auto it = symTab.find(cleaned);
    if (it == symTab.end()) {
        return false;
    }

    address = it->second;
    return true;
}


// calculate displacement of PC
int usingPC(int targetAddress, int locctr) {
    int pc = locctr + 3;
    return targetAddress - pc;
}


// check if PC displacement fits 12 bit signed space
bool pcWorks(int targetAddress, int locctr, int& displacement) {
    int disp = usingPC(targetAddress, locctr);

    if (disp >= -2048 && disp <= 2047) {
        displacement = disp;
        return true;
    }

    return false;
}

// calculate displacement for base relative
int usingBase(int targetAddress, int baseAddress) {
    return targetAddress - baseAddress;
}


// checks if base fits 12 bit unsigned space
bool baseWorks(int targetAddress, int baseAddress, int& displacement) {
    int disp = usingBase(targetAddress, baseAddress);

    if (disp >= 0 && disp <= 4095) {
        displacement = disp;
        return true;
    }

    return false;
}


// get number value from an immediate constant string
bool immediateAddress(const string& operand, int& value) {
    if (!isImmediateConstantOperand(operand)) {
        return false;
    }

    string cleaned = cleanOperand(operand);
    value = stoi(cleaned);
    return true;
}


// decide what addressing mode to use for pass 2 machine code
AddressResult calculateAddress(
    const string& operand,
    int locctr,
    int format,
    const unordered_map<string, int>& symTab,
    bool baseIsSet,
    int baseAddress
) {
    AddressResult result;

    if (format != 3 && format != 4) {
        return result;
    }

    int value = 0;
    if (immediateAddress(operand, value)) {
        result.valid = true;
        result.isImmediateConstant = true;
        result.fieldValue = value;
        return result;
    }

    int target = 0;
    if (!lookupSymbolAddress(operand, symTab, target)) {
        return result;
    }

    result.targetAddress = target;

    if (format == 4) {
        result.valid = true;
        result.fieldValue = target;
        return result;
    }

    int disp = 0;
    if (pcWorks(target, locctr, disp)) {
        result.valid = true;
        result.usedPC = true;
        result.fieldValue = disp;
        return result;
    }

    if (baseIsSet && baseWorks(target, baseAddress, disp)) {
        result.valid = true;
        result.usedBase = true;
        result.fieldValue = disp;
        return result;
    }

    return result;
}

// mask displacement to correct bit width
uint32_t encodeAddressField(int value, int width) {
    if (width <= 0 || width > 32) {
        throw invalid_argument("invalid address width");
    }

    if (width == 32) {
        return static_cast<uint32_t>(value);
    }

    uint32_t mask = (1u << width) - 1u;
    return static_cast<uint32_t>(value) & mask;
}