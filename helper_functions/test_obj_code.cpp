/*
* * Authors: Steph Huynh (cssc2524) and Isabella Viraldo (cssc2555)
*/

#include <iostream>
#include "obj_code.h"

int main()
{
    std::cout << std::hex;

    //format 1
    Format1Code f1;

    std::cout << "F1 start: 0x" << f1.getHexValue() << std::endl;

    f1.setOpcode(0xC4);
    std::cout << "F1 opcode set: 0x" << f1.getHexValue() << std::endl;

    //format 2
    Format2Code f2;

    std::cout << "\nF2 start: 0x" << f2.getHexValue() << std::endl;

    f2.setOpcode(0xB4);
    std::cout << "F2 opcode set: 0x" << f2.getHexValue() << std::endl;

    f2.setR1(0x1);
    std::cout << "F2 r1 set: 0x" << f2.getHexValue() << std::endl;

    f2.setR2(0x2);
    std::cout << "F2 r2 set: 0x" << f2.getHexValue() << std::endl;

    //format 3
    Format3Code f3;

    std::cout << "\nF3 start: 0x" << f3.getHexValue() << std::endl;

    f3.setOpcode(0x48);
    std::cout << "F3 opcode set: 0x" << f3.getHexValue() << std::endl;

    f3.setN(true);
    std::cout << "F3 n set: 0x" << f3.getHexValue() << std::endl;

    f3.setI(true);
    std::cout << "F3 i set: 0x" << f3.getHexValue() << std::endl;

    f3.setP(true);
    std::cout << "F3 p set: 0x" << f3.getHexValue() << std::endl;

    f3.setAddressField(0xABC);
    std::cout << "F3 addr set: 0x" << f3.getHexValue() << std::endl;

    //format 4
    Format4Code f4;

    std::cout << "\nF4 start: 0x" << f4.getHexValue() << std::endl;

    f4.setOpcode(0x48);
    std::cout << "F4 opcode set: 0x" << f4.getHexValue() << std::endl;

    f4.setN(true);
    std::cout << "F4 n set: 0x" << f4.getHexValue() << std::endl;

    f4.setI(true);
    std::cout << "F4 i set: 0x" << f4.getHexValue() << std::endl;

    f4.setAddressField(0x12345);
    std::cout << "F4 addr set: 0x" << f4.getHexValue() << std::endl;

    return 0;
}