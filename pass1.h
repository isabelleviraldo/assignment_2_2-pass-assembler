#ifndef PASS1_H
#define PASS1_H

#include <string>
#include <vector>
#include <unordered_map>
#include "helper_functions/read_input.h"

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