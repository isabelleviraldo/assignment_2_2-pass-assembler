/*
* Purpose: pass1.cpp source file
* Author: Steph Huynh and Isabelle Viraldo
* Description: processes pass 1 by reading .sic file and parsing each line.
* Creates .st file after it has processed all the directives and calculated LOCCTR.
* Also creates a .l listing file with addresses.
*
*/

#include <string>
#include <iostream>
#include "pass1.h"
#include "helper_functions/read_input.h"
#include <fstream>
#include <iomanip>
#include <unordered_map>
#include "Opcode_Dictionary.h"
#include "helper_functions/hex_helpers.h"
#include "helper_functions/address_helper.h"
#include <vector>

using namespace std;

Pass1OutputFiles runPass1(const string& filename) {
    // open file to read
    ifstream inputFile(filename.c_str());

    // error handling for input file
    if (!inputFile) {
        cerr << "Error: could not open input file " << filename << endl;
        cerr << "Terminating program" << endl;
        return {"", ""};   // changed: return empty output file names on failure
    }

    cout << "Processing " << filename << endl;

    // changed: create listing file (.l) during pass 1
    string listingFile = filename.substr(0, filename.find_last_of(".")) + ".l";
    ofstream lst(listingFile.c_str());

    if (!lst) {
        cerr << "Error: could not create listing file (.l) " << listingFile << endl;
        return {"", ""};   // changed: return empty output file names on failure
    }

    // creating symTab
    unordered_map<string, int> symTab;   // store labels and addresses
    vector<Literal> littab;              // store literals waiting for addresses

    // tracking variables
    int LOCCTR = 0;
    int startAddr = 0;
    string csect = "NA";
    string line;

    // read file line by line until none left
    while (getline(inputFile, line)) {
        ParseInput parsedLine = parseSicLine(line);

        // TEST PRINTOUTS
        cout << "\nOriginal line: " << parsedLine.original << endl;
        cout << "Label:   [" << parsedLine.label << "]" << endl;
        cout << "Opcode:  [" << parsedLine.opcode << "]" << endl;
        cout << "Operand: [" << parsedLine.operand << "]" << endl;

        // skip empty lines
        if (parsedLine.emptyComment == true) {
            continue;
        }

        // changed:
        // If the source file already contains a generated literal line such as:
        // *    =C'EOF'
        // skip it so pass1 does not emit that literal again later.
        if (parsedLine.label == "*" &&
            !parsedLine.opcode.empty() &&
            parsedLine.opcode[0] == '=') {
            continue;
        }

        int currentAddr = LOCCTR;
        string searchLine = parsedLine.opcode;

        bool fmt4 = false;
        if (!searchLine.empty() && searchLine[0] == '+') {
            searchLine = searchLine.substr(1);
            fmt4 = true;
        }

        // START directive
        if (searchLine == "START") {
            csect = parsedLine.label;
            LOCCTR = stoi(parsedLine.hex, nullptr, 16);
            startAddr = LOCCTR;
            symTab[csect] = LOCCTR;

            // changed: write START line to listing file with its address
            lst << toHexString(LOCCTR, 4) << "    " << parsedLine.original << endl;
            continue;
        }

        // collect literals
        if (!parsedLine.operand.empty() && parsedLine.operand[0] == '=') {
            bool exists = false;

            for (const auto& l : littab) {
                if (l.name == parsedLine.operand) {
                    exists = true;
                    break;
                }
            }

            if (!exists) {
                Literal newLit;
                newLit.name = parsedLine.operand;

                int value = byteObjectValue(parsedLine.operand.substr(1));
                newLit.hexData = toHexString(
                    value,
                    getByteDirectiveLength(parsedLine.operand.substr(1)) * 2
                );
                newLit.length = getByteDirectiveLength(parsedLine.operand.substr(1));
                newLit.address = -1;
                littab.push_back(newLit);
            }
        }

        // add labels to symbol table
        if (!parsedLine.label.empty() && parsedLine.label != "*") {
            if (symTab.find(parsedLine.label) == symTab.end()) {
                symTab[parsedLine.label] = LOCCTR;
            }
            else {
                cerr << "Error: duplicate label found (not allowed) ["
                     << parsedLine.label << "]" << endl;
            }
        }

        // print LTORG line, assign addresses to pending literals, print generated literal lines into the listing file
        if (searchLine == "LTORG") {
            lst << toHexString(currentAddr, 4) << "    " << parsedLine.original << endl;

            for (auto& l : littab) {
                if (l.address == -1) {
                    l.address = LOCCTR;
                    lst << toHexString(LOCCTR, 4) << "    "
                        << "*\t" << l.name << endl;
                    LOCCTR += l.length;
                }
            }
            continue;
        }

        // changed:
        // before writing END, place all remaining unassigned literals first
        // so they appear before END in the listing file
        if (searchLine == "END") {
            for (auto& l : littab) {
                if (l.address == -1) {
                    l.address = LOCCTR;
                    lst << toHexString(LOCCTR, 4) << "    "
                        << "*\t" << l.name << endl;
                    LOCCTR += l.length;
                }
            }

            // END usually does not carry a printed address in the listing
            lst << "        " << parsedLine.original << endl;
            continue;
        }

        // update LOCCTR
        opSpecs currInstruction;

        if (getOpcodeSpecs(searchLine, currInstruction)) {
            if (fmt4) {
                LOCCTR += 4;
            }
            else {
                LOCCTR += currInstruction.fmt;
            }
        }
        else if (searchLine != "START" && searchLine != "BASE" && searchLine != "NOBASE") {
            if (searchLine == "WORD") {
                LOCCTR += 3;
            }
            else if (searchLine == "RESW") {
                LOCCTR += 3 * stoi(parsedLine.cleanedOperand);
            }
            else if (searchLine == "RESB") {
                LOCCTR += stoi(parsedLine.cleanedOperand);
            }
            else if (searchLine == "BYTE") {
                LOCCTR += getByteDirectiveLength(parsedLine.operand);
            }
        }

        // changed: write normal source line to listing file using current address
        lst << toHexString(currentAddr, 4) << "    " << parsedLine.original << endl;
    }

    // final program length
    int progLength = LOCCTR - startAddr;

    // Testing:
    cout << "\nFinal Hash Map State \n";
    for (const auto& pair : symTab) {
        cout << "Key: " << pair.first << "\t | LOCCTR: " << pair.second << "\n";
    }

    // create .st
    string symtabFile = filename.substr(0, filename.find_last_of(".")) + ".st";
    ofstream st(symtabFile.c_str());

    if (!st) {
        cerr << "Error: could not create symtab file (.st) " << symtabFile << endl;
        lst.close();
        return {listingFile, ""};   // changed: preserve listing file result if .st fails
    }

    st << left << setw(8) << "CSect"
       << setw(10) << "Symbol"
       << setw(10) << "Value"
       << setw(10) << "LENGTH"
       << "Flags:" << endl;
    st << "-----------------------------------------" << endl;

    st << left << setw(8) << csect
       << setw(10) << ""
       << toHexString(startAddr, 6) << "  "
       << toHexString(progLength, 6) << endl;

    for (const auto& pair : symTab) {
        if (pair.first == csect) continue;
        st << left << setw(8) << ""
           << setw(10) << pair.first
           << toHexString(pair.second, 6) << "             R" << endl;
    }

    st << endl << "Literal Table" << endl;
    st << left << setw(10) << "Name"
       << setw(12) << "Operand"
       << setw(10) << "Address"
       << "Length:" << endl;
    st << "-----------------------------------------------" << endl;

    for (const auto& l : littab) {
        string nameDisp = l.name.substr(1);
        if (nameDisp.size() > 3) {
            nameDisp = nameDisp.substr(2, nameDisp.size() - 3);
        }

        st << left << setw(10) << nameDisp
           << setw(12) << l.hexData
           << toHexString(l.address, 6) << "    " << dec << l.length << endl;
    }

    st.close();
    lst.close();

    cout << "Success: Symbol table created at " << symtabFile << endl;
    cout << "Success: Listing file created at " << listingFile << endl;

    // changed: return both generated output file names
    return { listingFile, symtabFile };
}