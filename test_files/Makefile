#
# Purpose: Makefile
# Authors: Steph Huynh (cssc2524) (824058671)
#          Isabelle Viraldo (cssc2555) (828115945)
# Class Info: CS 530, Spring 2026
# Assignment Info: Assignment #2, Limited XE Assembler
#
# Description: makes .exe files to test helper functions
#


CXX = g++
CXXFLAGS = -std=c++17 -Wall -I../helper_functions -I..

all: test_addr test_hex test_obj test_parser

test_addr:
	$(CXX) $(CXXFLAGS) test_address_helper.cpp ../helper_functions/address_helper.cpp -o test_addr

test_hex:
	$(CXX) $(CXXFLAGS) test_hex_helper.cpp ../helper_functions/hex_helper.cpp -o test_hex

test_obj:
	$(CXX) $(CXXFLAGS) test_obj_code.cpp ../helper_functions/obj_code.cpp -o test_obj

test_parser:
	$(CXX) $(CXXFLAGS) test_parser.cpp ../helper_functions/read_input.cpp ../helper_functions/address_helper.cpp ../helper_functions/hex_helper.cpp -o test_parser

clean:
	rm -f test_addr test_hex test_obj test_parser