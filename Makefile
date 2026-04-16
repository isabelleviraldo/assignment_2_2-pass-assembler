CXX = g++
CXXFLAGS = -std=c++17 -Wall -Ihelper_functions -I.

lxe: 
	$(CXX) $(CXXFLAGS) main.cpp pass1.cpp pass2.cpp helper_functions/address_helper.cpp helper_functions/obj_code.cpp helper_functions/read_input.cpp Opcode_Dictionary.cpp helper_functions/hex_helpers.cpp -o lxe

clean:
	rm -f assembler test_obj test_read test_pass1 test_addr test_hex *.st *.l