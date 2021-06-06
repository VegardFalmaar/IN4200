C = gcc
CFLAGS = -Wall -fopenmp
CXX = g++
CXXFLAGS = -Wall -fopenmp

p6.exe: p6.o
	$(CXX) $(CXXFLAGS) $^ -o $@

p5.exe: p5.o
	$(C) $(CFLAGS) $^ -o $@

p5.o: p5.c

p6.o: p6.cpp

execute: p6.exe
	./$^

clean:
	rm *.o *.exe
