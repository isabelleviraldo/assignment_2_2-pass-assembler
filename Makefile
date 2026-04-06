CXX = g++
CXXFLAGS = -std=c++17 -Wall -Ihelper_functions

test:
	$(CXX) $(CXXFLAGS) test_obj_code.cpp helper_functions/obj_code.cpp -o test_obj

	$(CXX) $(CXXFLAGS) test_read_input.cpp pass1.cpp helper_functions/read_input.cpp Opcode_Dictionary.cpp helper_functions/hex_helpers.cpp -o test_pass1
	
clean:
	rm -f test_obj
	rm -f test_pass1