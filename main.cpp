#include <iostream>
#include <fstream>
#include <string>

using namespace std;


int main (int argc, char* argv[]) {
	
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
	
		// open file to read 
		ifstream inputFile(filename.c_str());

		//error handling for input file
		if (!inputFile) {
			cerr <<	"Error: could not open input file " << filename << endl;
			cerr << "Terminating program" << endl;
			return 1;

		}

		cout << "Processing " << filename << endl;
		string line;
		while (inputFile) {
			// put pass 1 and 2 info here (or function calls)			

		}
	}	

}
