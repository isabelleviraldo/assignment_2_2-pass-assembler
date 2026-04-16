/*
* Purpose: Testing file for hex_helpers.cpp
*Authors: Steph Huynh (cssc2524) and Isabelle Viraldo (cssc2555)
* Class Info: CS 530, Spring 2026
* Assignment Info: Assignment #2, Limited XE Assembler
* Description: Ensure hex_helpers.cpp runs properly
*
*/


#include <iostream>
#include <cassert>
#include "hex_helpers.h"
using namespace std;

int main()
{
	cout << "Running test for hex_helpers" << endl;


	assert(isHexLiteral("X'0F'") == true);
	assert(isHexLiteral("C'EOF'") == false);
	assert(isCharLiteral("C'EOF'") == true);


	assert(parseOperandValue("X'0F'") == 15);
	assert(getByteDirectiveLength("C'EOF'") == 3);


	assert(toHexString(15, 2) == "0F");
	assert(toHexString(10, 4) == "000A");

	assert(byteObjectValue("C'EOF'") == 0x454F46);
	

	cout << "SUCCESS test run for hex_helpers" << endl;
}