/*
* Purpose: test_parser.cpp
* *Authors: Steph Huynh (cssc2524) and Isabelle Viraldo (cssc2555)
* Class Info: CS 530, Spring 2026
* Assignment Info: Assignment #2, Limited XE Assembler
* Description: verify parsing and opcode lookup
* independently from the main Pass 1 and Pass 2 workflow.
*/


#include <iostream>
#include <cassert>

#include "read_input.h"

using namespace std;


int main() {
    cout << "Running test for parser/read_input" << endl;

    // blank line
    ParseInput p1 = parseSicLine("");
    assert(p1.label == "");
    assert(p1.opcode == "");
    assert(p1.operand == "");

    // whitespace-only line
    ParseInput p2 = parseSicLine("    ");
    assert(p2.emptyComment == true);

    // line with label, opcode, operand
    ParseInput p3 = parseSicLine("FIRST STL RETADR");
    assert(p3.label == "FIRST");
    assert(p3.opcode == "STL");
    assert(p3.operand == "RETADR");
    assert(p3.cleanedOperand == "RETADR");
    assert(p3.isIndexed == false);
    assert(p3.isImmediate == false);
    assert(p3.isIndirect == false);

    // line without label
    ParseInput p4 = parseSicLine("    RSUB");
    assert(p4.label == "");
    assert(p4.opcode == "RSUB");
    assert(p4.operand == "");

    // immediate operand
    ParseInput p5 = parseSicLine("    LDA #100");
    assert(p5.opcode == "LDA");
    assert(p5.operand == "#100");
    assert(p5.cleanedOperand == "100");
    assert(p5.isImmediate == true);
    assert(p5.hex == "0064");

    // indirect operand
    ParseInput p6 = parseSicLine("    LDA @PTR");
    assert(p6.opcode == "LDA");
    assert(p6.operand == "@PTR");
    assert(p6.cleanedOperand == "PTR");
    assert(p6.isIndirect == true);

    // indexed operand
    ParseInput p7 = parseSicLine("    STA TABLE,X");
    assert(p7.opcode == "STA");
    assert(p7.operand == "TABLE,X");
    assert(p7.cleanedOperand == "TABLE");
    assert(p7.isIndexed == true);

    // hex literal
    ParseInput p8 = parseSicLine("    BYTE X'0F'");
    assert(p8.opcode == "BYTE");
    assert(p8.operand == "X'0F'");
    assert(p8.hex == "F");

    // char literal
    ParseInput p9 = parseSicLine("    BYTE C'EOF'");
    assert(p9.opcode == "BYTE");
    assert(p9.operand == "C'EOF'");
    assert(p9.hex == "C'EOF'");

    cout << "SUCCESS test run for parser/read_input" << endl;
    return 0;
}