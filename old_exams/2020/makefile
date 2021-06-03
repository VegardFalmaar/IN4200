CXX = g++
CXXFLAGS = --std=c++17 -Wall -O3 -fopenmp

p2.exe: p2.o
	$(CXX) $(CXXFLAGS) $^ -o $@

p2.o: p2.cpp module.cpp

p3.exe: p3.o
	$(CXX) $(CXXFLAGS) $^ -o $@

p3.o: p3.cpp module.cpp

execute: p3.exe
	./$^

clean:
	rm *.exe *.o
