/*
* Purpose: test_object_code.cpp source file
* *Authors: Steph Huynh (cssc2524) and Isabelle Viraldo (cssc2555)
* Class Info: CS 530, Spring 2026
* Assignment Info: Assignment #2, Limited XE Assembler
*/


#include <iostream>
#include <cassert>

#include "obj_code.h"

using namespace std;


int main()
{
    cout << hex;
    cout << "Running test for obj_code" << endl;

    // ---------------- Format 1 ----------------
    Format1Code f1;

    assert(f1.getHexValue() == 0x00);

    f1.setOpcode(0xC4);
    assert(f1.getHexValue() == 0xC4);
    assert(f1.getOpcode() == 0xC4);

    // ---------------- Format 2 ----------------
    Format2Code f2;

    assert(f2.getHexValue() == 0x0000);

    f2.setOpcode(0xB4);
    assert(f2.getOpcode() == 0xB4);

    f2.setR1(0x1);
    assert(f2.getR1() == 0x1);

    f2.setR2(0x2);
    assert(f2.getR2() == 0x2);

    assert(f2.getHexValue() == 0xB412);

    // another simple case
    Format2Code f2b;
    f2b.setOpcode(0x90);
    f2b.setR1(0xA);
    f2b.setR2(0xF);
    assert(f2b.getHexValue() == 0x90AF);

    // ---------------- Format 3 ----------------
    Format3Code f3;

    assert(f3.getHexValue() == 0x000000);
    assert(f3.getE() == false);   // format 3 starts with e = 0

    f3.setOpcode(0x48);
    assert(f3.getOpcode() == (0x48 >> 2));   // stored as upper 6 bits

    f3.setN(true);
    assert(f3.getN() == true);

    f3.setI(true);
    assert(f3.getI() == true);

    f3.setP(true);
    assert(f3.getP() == true);

    f3.setAddressField(0xABC);
    assert(f3.getAddressField() == 0xABC);

    // final expected object code:
    // opcode 0x48 -> upper 6 bits = 0x12
    // n=1, i=1, p=1, addr=ABC
    assert(f3.getHexValue() == 0x4B2ABC);

    // simple extra case
    Format3Code f3b;
    f3b.setOpcode(0x3C);
    f3b.setN(true);
    f3b.setI(true);
    f3b.setAddressField(0x123);
    assert(f3b.getAddressField() == 0x123);

    // ---------------- Format 4 ----------------
    Format4Code f4;

    assert(f4.getHexValue() == 0x00100000);
    assert(f4.getE() == true);   // format 4 always has e = 1

    f4.setOpcode(0x48);
    assert(f4.getOpcode() == (0x48 >> 2));

    f4.setN(true);
    assert(f4.getN() == true);

    f4.setI(true);
    assert(f4.getI() == true);

    f4.setAddressField(0x12345);
    assert(f4.getAddressField() == 0x12345);

    // opcode 0x48 -> upper 6 bits = 0x12
    // n=1, i=1, e=1, addr=12345
    assert(f4.getHexValue() == 0x4B112345);

    // another simple case
    Format4Code f4b;
    f4b.setOpcode(0x90);
    f4b.setAddressField(0xABCDE);
    assert(f4b.getAddressField() == 0xABCDE);
    assert(f4b.getE() == true);

    cout << "SUCCESS test run for obj_code" << endl;
    return 0;
}