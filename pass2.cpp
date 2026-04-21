/*
* Purpose: pass2.cpp source file
* Authors: Steph Huynh (cssc2524) (824058671)
*          Isabelle Viraldo (cssc2555) (828115945)
* Class Info: CS 530, Spring 2026
* Assignment Info: Assignment #2, Limited XE Assembler
*
* Description: reads pass1 .l and .st files, rebuilds the needed data,
* and writes a .l file with object code appended to each source line.
*/

#include "pass2.h"

#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <sstream>
#include <cctype>

#include "Opcode_Dictionary.h"
#include "address_helper.h"
#include "hex_helper.h"
#include "read_input.h"
#include "obj_code.h"

using namespace std;

static bool isHexAddressField(const string& s) {
    if (s.length() != 4) return false;
    for (char c : s) {
        if (!isxdigit(static_cast<unsigned char>(c))) {
            return false;
        }
    }
    return true;
}

static vector<lineInfo> loadListingLines(const string& listingFile) {
    vector<lineInfo> lines;
    ifstream in(listingFile.c_str());

    if (!in) {
        cerr << "Error: could not open listing file " << listingFile << endl;
        return lines;
    }

    string line;

    // Read each line from the pass1 listing and rebuild lineInfo entries
    while (getline(in, line)) {
        lineInfo info;
        info.address = -1;

        string sourcePart = line;

        // Check whether line starts with a hex address
        if (line.length() >= 8 && isHexAddressField(line.substr(0, 4))) {
            info.address = stoi(line.substr(0, 4), nullptr, 16);
            sourcePart = line.substr(8);
        }

        info.data = parseSicLine(sourcePart);
        lines.push_back(info);
    }

    return lines;
}

static void loadSymtabFile(
    const string& symtabFile,
    unordered_map<string, int>& symTab
) {
    ifstream in(symtabFile.c_str());

    if (!in) {
        cerr << "Error: could not open symtab file " << symtabFile << endl;
        return;
    }

    string line;
    bool inLiteralTable = false;
    bool skippedHeader = false;

    // Read .st and rebuild SYMTAB from the symbol section
    while (getline(in, line)) {
        if (line.empty()) continue;

        if (line.find("Literal Table") != string::npos) {
            inLiteralTable = true;
            continue;
        }

        if (line.find("CSect") != string::npos ||
            line.find("Name") != string::npos ||
            line.find("---") != string::npos) {
            continue;
        }

        if (!inLiteralTable) {
            istringstream iss(line);
            vector<string> tokens;
            string tok;

            while (iss >> tok) {
                tokens.push_back(tok);
            }

            if (tokens.empty()) continue;

            // Skip control section line
            if (!skippedHeader) {
                skippedHeader = true;
                continue;
            }

            // Read symbol and value into SYMTAB
            if (tokens.size() >= 2) {
                string symbol = tokens[0];
                string value = tokens[1];

                bool validHex = true;
                for (char c : value) {
                    if (!isxdigit(static_cast<unsigned char>(c))) {
                        validHex = false;
                        break;
                    }
                }

                if (validHex) {
                    symTab[symbol] = stoi(value, nullptr, 16);
                }
            }
        }
    }
}

void runPass2(
    const string& sourceFilename,
    const string& listingFile,
    const string& symtabFile
) {
    vector<lineInfo> sicData = loadListingLines(listingFile);

    unordered_map<string, int> symTab;
    loadSymtabFile(symtabFile, symTab);

    // Add literal addresses from generated literal lines in the listing file
    for (const auto& entry : sicData) {
        if (entry.address >= 0 &&
            entry.data.label == "*" &&
            !entry.data.opcode.empty() &&
            entry.data.opcode[0] == '=') {
            symTab[entry.data.opcode] = entry.address;
        }
    }

    string outputFile = sourceFilename.substr(0, sourceFilename.find_last_of(".")) + ".l";
    ofstream out(outputFile.c_str());

    if (!out) {
        cerr << "Error: could not create pass2 output file " << outputFile << endl;
        return;
    }

    bool baseIsSet = false;
    int baseAddress = 0;

    // Process each intermediate line and generate object code when needed
    for (const auto& entry : sicData) {
        int locctr = entry.address;
        ParseInput parsedLine = entry.data;
        string objectCode = "";

        if (parsedLine.emptyComment) {
            out << parsedLine.original << endl;
            continue;
        }

        string opcode = parsedLine.opcode;
        string operand = parsedLine.operand;
        string searchLine = opcode;

        bool fmt4 = false;
        if (!searchLine.empty() && searchLine[0] == '+') {
            fmt4 = true;
            searchLine = searchLine.substr(1);
        }

        // Handle generated literal line
        if (parsedLine.label == "*" && !parsedLine.opcode.empty() && parsedLine.opcode[0] == '=') {
            string litBody = parsedLine.opcode.substr(1);
            int value = byteObjectValue(litBody);
            int len = getByteDirectiveLength(litBody);
            objectCode = toHexString(value, len * 2);

            out << toHexString(locctr, 4) << "    "
                << parsedLine.original
                << "    " << objectCode << endl;
            continue;
        }

        // Handle directives with no object code
        if (searchLine == "START" || searchLine == "END" ||
            searchLine == "RESW"  || searchLine == "RESB") {

            if (locctr >= 0) {
                out << toHexString(locctr, 4) << "    " << parsedLine.original << endl;
            }
            else {
                out << parsedLine.original << endl;
            }
            continue;
        }

        // Set base register using operand symbol
        if (searchLine == "BASE") {
            int addr = 0;
            if (lookupSymbolAddress(operand, symTab, addr)) {
                baseIsSet = true;
                baseAddress = addr;
            } else {
                cerr << "Error: BASE symbol not found [" << operand << "]" << endl;
            }

            out << toHexString(locctr, 4) << "    " << parsedLine.original << endl;
            continue;
        }

        // Clear base-relative mode
        if (searchLine == "NOBASE") {
            baseIsSet = false;
            baseAddress = 0;

            out << toHexString(locctr, 4) << "    " << parsedLine.original << endl;
            continue;
        }

        // Convert BYTE constant to object code
        if (searchLine == "BYTE") {
            int value = byteObjectValue(operand);
            int len = getByteDirectiveLength(operand);
            objectCode = toHexString(value, len * 2);
        }

        // Convert WORD constant to object code
        else if (searchLine == "WORD") {
            int value = parseOperandValue(operand);
            objectCode = toHexString(value, 6);
        }

        // Search OPTAB for OPCODE and generate instruction object code
        else {
            opSpecs currInstruction;
            if (getOpcodeSpecs(searchLine, currInstruction)) {

                // Format 1 instruction
                if (!fmt4 && currInstruction.fmt == 1) {
                    Format1Code code;
                    code.setOpcode(currInstruction.opcode);
                    objectCode = toHexString(code.getHexValue(), 2);
                }

                // Format 2 instruction
                else if (!fmt4 && currInstruction.fmt == 2) {
                    Format2Code code;
                    code.setOpcode(currInstruction.opcode);

                    string r1Name = "";
                    string r2Name = "";
                    size_t commaPos = operand.find(',');

                    if (commaPos != string::npos) {
                        r1Name = operand.substr(0, commaPos);
                        r2Name = operand.substr(commaPos + 1);
                    } else {
                        r1Name = operand;
                    }

                    int r1 = 0;
                    int r2 = 0;

                    // Convert register names to register numbers
                    if (!r1Name.empty()) {
                        if (getRegisterNumber(r1Name, r1)) {
                            code.setR1(r1);
                        } else {
                            cerr << "Error: invalid register [" << r1Name << "]" << endl;
                        }
                    }

                    if (!r2Name.empty()) {
                        if (getRegisterNumber(r2Name, r2)) {
                            code.setR2(r2);
                        } else {
                            cerr << "Error: invalid register [" << r2Name << "]" << endl;
                        }
                    }

                    objectCode = toHexString(code.getHexValue(), 4);
                }

                // Format 3 instruction
                else if (!fmt4 && currInstruction.fmt == 3) {
                    Format3Code code;
                    code.setOpcode(currInstruction.opcode);

                    // Special case for RSUB
                    if (searchLine == "RSUB") {
                        code.setN(true);
                        code.setI(true);
                        code.setX(false);
                        code.setB(false);
                        code.setP(false);
                        code.setE(false);
                        code.setAddressField(0);
                        objectCode = toHexString(code.getHexValue(), 6);
                    }
                    else {
                        // Set n/i bits based on operand addressing mode
                        if (isIndirectOperand(operand)) {
                            code.setN(true);
                            code.setI(false);
                        }
                        else if (isImmediateOperand(operand)) {
                            code.setN(false);
                            code.setI(true);
                        }
                        else {
                            code.setN(true);
                            code.setI(true);
                        }

                        // Set x bit for indexed addressing
                        code.setX(isIndexedOperand(operand));

                        // Calculate address/displacement for format 3
                        AddressResult result = calculateAddress(
                            operand,
                            locctr,
                            3,
                            symTab,
                            baseIsSet,
                            baseAddress
                        );

                        if (!result.valid) {
                            cerr << "Error: could not resolve address for ["
                                 << parsedLine.original << "]" << endl;
                        }
                        else {
                            code.setB(result.usedBase);
                            code.setP(result.usedPC);
                            code.setE(false);

                            uint32_t field = encodeAddressField(result.fieldValue, 12);
                            code.setAddressField(field);

                            objectCode = toHexString(code.getHexValue(), 6);
                        }
                    }
                }

                // Format 4 instruction
                else if (fmt4) {
                    Format4Code code;
                    code.setOpcode(currInstruction.opcode);

                    // Set n/i bits based on operand addressing mode
                    if (isIndirectOperand(operand)) {
                        code.setN(true);
                        code.setI(false);
                    }
                    else if (isImmediateOperand(operand)) {
                        code.setN(false);
                        code.setI(true);
                    }
                    else {
                        code.setN(true);
                        code.setI(true);
                    }

                    code.setX(isIndexedOperand(operand));
                    code.setB(false);
                    code.setP(false);
                    code.setE(true);

                    // Calculate 20-bit address for format 4
                    AddressResult result = calculateAddress(
                        operand,
                        locctr,
                        4,
                        symTab,
                        baseIsSet,
                        baseAddress
                    );

                    if (!result.valid) {
                        cerr << "Error: could not resolve extended address for ["
                             << parsedLine.original << "]" << endl;
                    }
                    else {
                        uint32_t field = encodeAddressField(result.fieldValue, 20);
                        code.setAddressField(field);

                        objectCode = toHexString(code.getHexValue(), 8);
                    }
                }
            }
            else {
                cerr << "Error: invalid operation code [" << opcode << "]" << endl;
            }
        }

        // Write final listing line, with object code if generated
        if (locctr >= 0) {
            out << toHexString(locctr, 4) << "    " << parsedLine.original;
        }
        else {
            out << parsedLine.original;
        }

        if (!objectCode.empty()) {
            out << "    " << objectCode;
        }

        out << endl;
    }

    out.close();
    cout << "Success: Pass 2 listing created at " << outputFile << endl;
}