/*
* Purpose: main.cpp
* Authors: Steph Huynh(cssc2524) and Isabella Viraldo(cssc2555)
* Description: where our main program runs from.
*/
#include <iostream>
#include <fstream>
#include <string>
#include "pass1.h"
#include <pass2.h>
using namespace std;


int main(int argc, char* argv[]) {

	// no input files given
	if (argc < 2)
	{
		cerr << "lxe: no input file given.\n";
		cerr << "Terminating program" << endl;
		return 1;
	}

	// Grab all input files by looping	
	for (int i = 1; i < argc; i++)
	{
		string filename = argv[i];

		// begin pass 1 to read file and such
		cout << "Begin pass 1" << endl;
		Pass1OutputFiles p1 = runPass1(filename);


		cout << "Begin pass 2" << endl;


		runPass2(filename, p1.listingFile, p1.symtabFile);


		cout << "Program complete" << endl;

	}
	return 0;
}
