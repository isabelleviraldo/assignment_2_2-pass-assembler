#ifndef READ_INPUT_H
#define READ_INPUT_H

#include <string>
using namespace std;
struct ParseInput {
	bool emptyComment = false;
	int lineCt = 0;

	// variables to hold parsed information
	string original;
	string label;
	string opcode;
	string operand;
}

ParseInput parseSicLine (string line);



#endif
