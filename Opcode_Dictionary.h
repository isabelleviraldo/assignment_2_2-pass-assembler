#ifndef OPCODE_DICTIONARY_H
#define OPCODE_DICTIONARY_H
#include <string>
#include <unordered_map>
using namespace std;

struct opSpecs {
	int opcode;
	int fmt;
};


const unorder_map<string, opSpecs> opcodeDictionary;
#endif