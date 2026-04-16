/*
* Authors: Steph Huynh (cssc2524) and Isabella Viraldo (cssc2555)
*/

#include <iostream>
#include <string>
#include "read_input.h"
#include "pass1.h"
#include "pass2.h"

using namespace std;

/*
 * Purpose: run test and print results
 * 
 *
 */

void runTest (string testName, string inputLine, string expectedLabel, string expectedOpcode, string expectedOperand, bool expectedEmpty) {
	ParseInput result = parseSicLine(inputLine);

	bool passed = true;
	
	// Check empty flag 
	if (result.emptyComment != expectedEmpty) {
		cout << "FAILED: expected --> " << expectedEmpty << endl;
		cout << "\tActual --> " << result.emptyComment << endl;
		passed = false;
	}	

	// check other pieces
	if (!expectedEmpty) {
		if (result.label != expectedLabel)
		{
			cout << "FAILED: expected --> " << expectedLabel << endl;
			cout << "\t Actual --> " << result.label << endl;
			passed = false;
		}

		if (result.opcode != expectedOpcode){
			cout << "FAILED: expected --> " << expectedOpcode << endl;
			cout << "\tactual --> " << result.opcode << endl;
			passed = false;

		}
		if (result.operand != expectedOperand) {

			cout << "FAILED: expected --> " << expectedOperand << endl;
			cout << "\tActual --> " << result.operand << endl;
			passed = false;
			}

	}

	if (passed) {cout << "PASS" << endl;}

	
}


int main() {
    cout << "=== Starting Input Reader Tests ===" << endl << endl;

    // Test 1: Standard line with all three parts
    // (Using \t to simulate tab characters from a text file)
    runTest("Test 1: Standard Line", "SUM\tSTART\t0", "SUM", "START", "0", false); 
    // Test 2: Missing label (starts with a tab/space)                                 
    runTest("Test 2: No Label", "\tLDA\t#0", "", "LDA", "#0", false);
    // Test 3: Opcode only (no operand, like RSUB)
    runTest("Test 3: No Operand", "\tRSUB", "", "RSUB", "", false);
    // Test 4: Literal pool initialization
    runTest("Test 4: Literal/Directive", "*\t=C'EOF'", "*", "=C'EOF'", "", false);
    // Test 5: Completely empty string
    runTest("Test 5: Empty Line", "", "", "", "", true);
    // Test 6: A line containing only spaces and tabs
    runTest("Test 6: Whitespace Only", "   \t  \r", "", "", "", true);
	cout << "=== All Tests Completed ===" << endl;


	Pass1OutputFiles files = runPass1("test.txt");

	if (files.listingFile.empty() || files.symtabFile.empty()) {
	    cerr << "Pass 1 failed.\n";
	    return 1;
	}	

	runPass2("test.txt", files.listingFile, files.symtabFile);
	
	return 0;
}
