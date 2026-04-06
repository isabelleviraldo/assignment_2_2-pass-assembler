#ifndef ADDRESS_HELPER_H
#define ADDRESS_HELPER_H

#include <string>
#include <unordered_map>
#include <cstdint>

using namespace std;

struct AddressResult {
    bool valid = false;

    bool usedPC = false;
    bool usedBase = false;
    bool isImmediateConstant = false;

    int targetAddress = 0;
    int fieldValue = 0;   // signed disp for fmt3, full address for fmt4, constant for #num
};

string cleanOperand(const string& operand);
bool isIndexedOperand(const string& operand);
bool isImmediateConstantOperand(const string& operand);

bool lookupSymbolAddress(
    const string& operand,
    const unordered_map<string, int>& symTab,
    int& address
);

int usingPC(int targetAddress, int locctr);
bool pcWorks(int targetAddress, int locctr, int& displacement);

int usingBase(int targetAddress, int baseAddress);
bool baseWorks(int targetAddress, int baseAddress, int& displacement);

bool immediateAddress(const string& operand, int& value);

AddressResult calculateAddress(
    const string& operand,
    int locctr,
    int format,
    const unordered_map<string, int>& symTab,
    bool baseIsSet,
    int baseAddress
);

uint32_t encodeAddressField(int value, int width);

int byteObjectValue(const string& operand);

#endif