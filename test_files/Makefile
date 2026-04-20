CXX = g++
CXXFLAGS = -std=c++17 -Wall -Ihelper_functions -I.

# default target
all: test_addr test_hex test_obj test_read

# address helper
test_addr:
	$(CXX) $(CXXFLAGS) test_address_helper.cpp helper_functions/address_helper.cpp -o test_addr

# hex helper
test_hex:
	$(CXX) $(CXXFLAGS) test_hex_helpers.cpp helper_functions/hex_helpers.cpp -o test_hex

#object code helper
test_obj:
	$(CXX) $(CXXFLAGS) test_object_code.cpp helper_functions/obj_code.cpp -o test_obj

#parser helper
test_read:
	$(CXX) $(CXXFLAGS) test_parser.cpp helper_functions/read_input.cpp helper_functions/address_helper.cpp helper_functions/hex_helpers.cpp -o test_read


clean:
	rm -f test_addr test_hex test_obj test_read