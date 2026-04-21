/*
* Purpose: Testing file for hex_helper.cpp
* Authors: Steph Huynh (cssc2524) (824058671)
*          Isabelle Viraldo (cssc2555) (828115945)
* Class Info: CS 530, Spring 2026
* Assignment Info: Assignment #2, Limited XE Assembler
*
* Description: Ensure hex_helper.cpp runs properly
*/


#include <cassert>
#include <iostream>
#include <stdexcept>

#include "hex_helper.h"

using namespace std;


int main()
{
    cout << "Running test for hex_helper" << endl;

    // literal checks
    assert(isHexLiteral("X'0F'") == true);
    assert(isHexLiteral("X'1A2B'") == true);
    assert(isHexLiteral("C'EOF'") == false);

    assert(isCharLiteral("C'EOF'") == true);
    assert(isCharLiteral("C'A'") == true);
    assert(isCharLiteral("X'0F'") == false);

    // parseOperandValue
    assert(parseOperandValue("X'0F'") == 15);
    assert(parseOperandValue("X'1A'") == 26);
    assert(parseOperandValue("25") == 25);

    // BYTE directive length
    assert(getByteDirectiveLength("C'EOF'") == 3);
    assert(getByteDirectiveLength("C'A'") == 1);
    assert(getByteDirectiveLength("X'0F'") == 1);
    assert(getByteDirectiveLength("X'1234'") == 2);

    // toHexString
    assert(toHexString(15, 2) == "0F");
    assert(toHexString(10, 4) == "000A");
    assert(toHexString(255, 2) == "FF");
    assert(toHexString(0, 2) == "00");

    // byteObjectValue
    assert(byteObjectValue("C'EOF'") == 0x454F46);
    assert(byteObjectValue("C'A'") == 0x41);
    assert(byteObjectValue("X'0F'") == 0x0F);

    // simple exception test
    bool caught = false;
    try {
        getByteDirectiveLength("X'F'");
    } catch (const invalid_argument&) {
        caught = true;
    }
    assert(caught == true);

    cout << "SUCCESS test run for hex_helper" << endl;
    return 0;
}