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

void runPass1(const string& filename);

#endif 
