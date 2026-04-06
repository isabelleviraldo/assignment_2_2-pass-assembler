#ifndef OPCODE_DICTIONARY_H
#define OPCODE_DICTIONARY_H
#include <string>
#include <unordered_map>
using namespace std;

struct opSpecs {
	int opcode;
	int fmt;
};


bool getOpcodeSpecs(const string& mnemonic, opSpecs& specs);
// const unorder_map<string, opSpecs> opcodeDictionary;
bool isDirective(const string& mnemonic);
bool getRegisterNumber(const string& regName, int& regNum);

#endif