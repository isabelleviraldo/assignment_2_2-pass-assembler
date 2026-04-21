/*
* Purpose: pass2.cpp source file
* Authors: Steph Huynh (cssc2524) (824058671)
*          Isabelle Viraldo (cssc2555) (828115945)
* Class Info: CS 530, Spring 2026
* Assignment Info: Assignment #2, Limited XE Assembler
*
* Description: reads pass1 .l and .st files, rebuilds the needed data,
* and writes a .l file with object code appended to each source line.
*/

#ifndef PASS2_H
#define PASS2_H

#include <string>
#include "pass1.h"

using namespace std;

void runPass2(
    const string& sourceFilename,
    const string& listingFile,
    const string& symtabFile
);

#endif