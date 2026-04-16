/*
* Purpose: Testing file for address_helper.cpp
* Authors: Steph Huynh (cssc2524) and Isabella Viraldo (cssc2555)
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

	assert(isIndexedOperand("TABLE,X") == true);
	assert(isIndexedOperand("TABLE") == false);
	assert(isIndirectOperand("@PTR") == true);
	assert(isIndirectOperand("PTR") == false);
	assert(isImmediateOperand("#100") == true);
	assert(isImmediateOperand("100") == false);
	assert(isImmediateConstantOperand("#LENGTH") == true);

	unordered_map<string, int> exSymTab = {
		{"TABLE", 0x1000},
		{"COUNT", 0x1500}

	};
	
	int addr = 0;

	assert(lookupSymbolAddress("TABLE", exSymTab, addr) == true);
	assert(addr == 0x1000);

	assert(lookupSymbolAddress("@COUNT", exSymTab, addr) == true);
	assert(addr == 0x1500);

	int disp = 0;

	assert(pcWorks(10, 0, disp) == true);
	assert(disp == 7);


}