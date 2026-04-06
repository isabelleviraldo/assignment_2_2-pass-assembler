CXX = g++
CXXFLAGS = -std=c++17 -Wall

test:
	$(CXX) $(CXXFLAGS) test_obj_code.cpp obj_code.cpp -o test_obj

	$(CXX) $(CXXFLAGS) test_read_input.cpp pass1.cpp read_input.cpp Opcode_Dictionary.cpp hex_helpers.cpp -o test_pass1

clean:
	rm -f test_obj
	rm -f test_pass1