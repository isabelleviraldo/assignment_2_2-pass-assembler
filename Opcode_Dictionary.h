#pragma once
#include <string>
#include <unordered_map>
using namespace std;

struct opSpecs {
	int opcode;
	int fmt;
};


const unorder_map<string, opSpecs> opcodeDictionary;
