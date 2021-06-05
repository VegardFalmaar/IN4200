CXX = g++
CXXFLAGS = --std=c++17 -Wall #-O3

p5.exe: p5.o
	$(CXX) $(CXXFLAGS) $^ -o $@

p5.o: p5.cpp
	$(CXX) $(CXXFLAGS) -fopenmp $^ -o $@

p5_MPI.exe: p5_MPI.cpp
	mpic++ $(CXXFLAGS) $^ -o $@

execute: p5_MPI.exe
	# ./$^
	mpirun -np 4 ./$^

clean:
	rm *.exe *.o
