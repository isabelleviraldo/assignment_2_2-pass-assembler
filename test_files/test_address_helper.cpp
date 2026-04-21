/*
* Purpose: Testing file for address_helper.cpp
* Authors: Steph Huynh (cssc2524) (824058671)
*          Isabelle Viraldo (cssc2555) (828115945)
* Class Info: CS 530, Spring 2026
* Assignment Info: Assignment #2, Limited XE Assembler
*
* Description: Ensure address_helper.cpp runs properly
*/


#include <cassert>
#include <iostream>
#include <unordered_map>

#include "address_helper.h"

using namespace std;


int main() {

    cout << "Running address_helper test...\n";

    unordered_map<string, int> symTab = {
        {"TABLE", 0x1000},
        {"COUNT", 0x1500}
    };

    int addr = 0;
    int disp = 0;
    int value = 0;

    // cleanOperand
    assert(cleanOperand("#100") == "100");
    assert(cleanOperand("TABLE,X") == "TABLE");

    // operand type checks
    assert(isIndexedOperand("TABLE,X") == true);
    assert(isIndirectOperand("@PTR") == true);
    assert(isImmediateOperand("#5") == true);
    assert(isImmediateConstantOperand("#123") == true);

    // symbol lookup
    assert(lookupSymbolAddress("TABLE", symTab, addr) == true);
    assert(addr == 0x1000);

    // PC-relative
    assert(pcWorks(10, 0, disp) == true);
    assert(disp == 7);

    // base-relative
    assert(baseWorks(2500, 0, disp) == true);
    assert(disp == 2500);

    // immediate value
    assert(immediateAddress("#42", value) == true);
    assert(value == 42);

    // calculateAddress
    AddressResult r1 = calculateAddress("#100", 0, 3, symTab, false, 0);
    assert(r1.valid == true);
    assert(r1.fieldValue == 100);

    AddressResult r2 = calculateAddress("TABLE", 0, 4, symTab, false, 0);
    assert(r2.valid == true);
    assert(r2.fieldValue == 0x1000);

    // encoding
    assert(encodeAddressField(-1, 12) == 0xFFF);

    cout << "SUCCESS: address_helper test passed\n";
    return 0;
}
