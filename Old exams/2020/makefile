CXX = g++
CXXFLAGS = --std=c++17 -Wall -O3 -fopenmp

p2.exe: p2.o
	$(CXX) $(CXXFLAGS) $^ -o $@

p2.o: p2.cpp

execute: p2.exe
	./$^

clean:
	rm *.exe *.o
