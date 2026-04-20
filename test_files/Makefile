CXX = g++
CXXFLAGS = -std=c++17 -Wall -I../helper_functions -I..

all: test_addr test_hex test_obj test_parser

test_addr:
	$(CXX) $(CXXFLAGS) test_address_helper.cpp ../helper_functions/address_helper.cpp -o test_addr

test_hex:
	$(CXX) $(CXXFLAGS) test_hex_helpers.cpp ../helper_functions/hex_helpers.cpp -o test_hex

test_obj:
	$(CXX) $(CXXFLAGS) test_obj_code.cpp ../helper_functions/obj_code.cpp -o test_obj

test_parser:
	$(CXX) $(CXXFLAGS) test_parser.cpp ../helper_functions/read_input.cpp ../helper_functions/address_helper.cpp ../helper_functions/hex_helpers.cpp -o test_parser

clean:
	rm -f test_addr test_hex test_obj test_parser