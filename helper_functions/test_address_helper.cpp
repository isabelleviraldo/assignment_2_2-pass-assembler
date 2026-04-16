/*
* Purpose: Testing file for address_helper.cpp
* Authors: Steph Huynh (cssc2524) and Isabelle Viraldo (cssc2555)
* Class Info: CS 530, Spring 2026
* Assignment Info: Assignment #2, Limited XE Assembler
*
* Description: Ensure address_helper.cpp runs properly
*
*/

#include <iostream>
#include <cassert>
#include "address_helper.h"
#include <unordered_map>

using namespace std;

int main() {


	cout << "Running address_helper test" << endl;

	assert(cleanOperand("#100") == "100");
	assert(cleanOperand("TABLE,X") == "TABLE");

	// ensure that boolean values are correct given input
	assert(isIndexedOperand("TABLE,X") == true);
	assert(isIndexedOperand("TABLE") == false);
	assert(isIndirectOperand("@PTR") == true);
	assert(isIndirectOperand("PTR") == false);
	assert(isImmediateOperand("#100") == true);
	assert(isImmediateOperand("100") == false);
	assert(isImmediateConstantOperand("#LENGTH") == false);


	// create mock symtab and run functions against it
	unordered_map<string, int> exSymTab = {
		{"TABLE", 0x1000},
		{"COUNT", 0x1500}

	};
	
	int addrFound = 0;

	assert(lookupSymbolAddress("TABLE", exSymTab, addrFound) == true);
	assert(addrFound == 0x1000);


	assert(lookupSymbolAddress("@COUNT", exSymTab, addrFound) == true);
	assert(addrFound == 0x1500);

	int disp = 0;

	assert(pcWorks(10, 0, disp) == true);
	assert(disp == 7);		// 10 - 3 = 7

	assert(baseWorks(2500, 0, disp));
	assert(disp == 2500);

	// calculating format 3 address
	AddressResult ex1 = calculateAddress("#100", 0, 3, exSymTab, false, 0);
	assert(ex1.valid == true);
	assert(ex1.isImmediateConstant == true);
	assert(ex1.fieldValue == 100);

	// format 4
	AddressResult ex2 = calculateAddress("TABLE", 0, 4, exSymTab, false, 0);
	assert(ex2.valid == true);
	assert(ex2.fieldValue == 0x1000);


	// bit masking test
	assert(encodeAddressField(-1, 12) == 0xFFF);	// 12 bit
	assert(encodeAddressField(-1, 20) == 0xFFFFF);	// 20 bit



	cout << "SUCCESS test run for address_helper" << endl;

}