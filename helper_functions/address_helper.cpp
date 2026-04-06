#include "address_helper.h"

#include <cctype>
#include <stdexcept>

using namespace std;

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

bool isIndexedOperand(const string& operand) {
    return operand.find(",X") != string::npos;
}

bool isImmediateConstantOperand(const string& operand) {
    if (operand.empty() || operand[0] != '#') {
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

int usingPC(int targetAddress, int locctr) {
    int pc = locctr + 3;
    return targetAddress - pc;
}

bool pcWorks(int targetAddress, int locctr, int& displacement) {
    int disp = usingPC(targetAddress, locctr);

    if (disp >= -2048 && disp <= 2047) {
        displacement = disp;
        return true;
    }

    return false;
}

int usingBase(int targetAddress, int baseAddress) {
    return targetAddress - baseAddress;
}

bool baseWorks(int targetAddress, int baseAddress, int& displacement) {
    int disp = usingBase(targetAddress, baseAddress);

    if (disp >= 0 && disp <= 4095) {
        displacement = disp;
        return true;
    }

    return false;
}

bool immediateAddress(const string& operand, int& value) {
    if (!isImmediateConstantOperand(operand)) {
        return false;
    }

    string cleaned = cleanOperand(operand);
    value = stoi(cleaned);
    return true;
}

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