/*
* Purpose: pass1.h source file
* Authors: Steph Huynh (cssc2524) (824058671)
*          Isabelle Viraldo (cssc2555) (828115945)
* Class Info: CS 530, Spring 2026
* Assignment Info: Assignment #2, Limited XE Assembler
*
* Description: processes pass 1 by reading .sic file and parsing each line.
* Creates .st file after it has processed all the directives and calculated LOCCTR.
* Also creates a .l listing file with addresses
*/

#ifndef PASS1_H
#define PASS1_H

#include <string>
#include <vector>

#include "read_input.h"

using namespace std;

struct Literal {
    string name;
    string hexData;
    int address = -1;
    int length;
};

struct lineInfo {
    int address;
    ParseInput data;
};

// lets pass1 return both generated output file names
struct Pass1OutputFiles {
    string listingFile;
    string symtabFile;
};

// runPass1 returns the .l and .st file names
Pass1OutputFiles runPass1(const string& filename);

#endif