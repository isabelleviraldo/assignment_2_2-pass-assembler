#
# Purpose: Makefile
# Authors: Steph Huynh (cssc2524) (824058671)
#          Isabelle Viraldo (cssc2555) (828115945)
# Class Info: CS 530, Spring 2026
# Assignment Info: Assignment #2, Limited XE Assembler
#
# Description: makes .exe files to run limited xe assembler
#

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Ihelper_functions -I.

all: lxe

lxe:
	$(CXX) $(CXXFLAGS) main.cpp pass1.cpp pass2.cpp Opcode_Dictionary.cpp \
	helper_functions/read_input.cpp \
	helper_functions/address_helper.cpp \
	helper_functions/hex_helper.cpp \
	helper_functions/obj_code.cpp \
	-o lxe

clean:
	rm -f lxe *.st *.l