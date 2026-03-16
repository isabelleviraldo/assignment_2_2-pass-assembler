CXX = g++
CXXFLAGS = -std=c++17 -Wall

test:
	$(CXX) $(CXXFLAGS) test_obj_code.cpp obj_code.cpp -o test_obj
	./test_obj

clean:
	rm -f test_obj