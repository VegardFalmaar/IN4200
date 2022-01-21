# compiler
CXX = /usr/local/bin/mpic++
# CXX = /usr/lib64/openmpi/bin/mpic++

# compiler flags
CXXFLAGS = --std=c++17 -Wall -O3

executor = /usr/local/bin/mpirun
executor_IFI = /usr/lib64/openmpi/bin/mpirun

MPI_main.exe: MPI_main.o
	$(CXX) $^ -o $@

MPI_main.o: MPI_double_layer_convolution.cpp

execute: MPI_main.exe
	$(executor) -np 8 ./MPI_main.exe 1000 800 15 15

execute_uio: MPI_main
	$(executor_IFI) -np 4 ./MPI_main.exe 200 200 9 8

test_compile:
	$(CXX) tests.cpp -g -o tests.exe --std=c++17 -Wall

test: test_compile
	./tests.exe

memcheck: test_compile
	valgrind --trace-children=yes --track-fds=yes --track-origins=yes --leak-check=full --show-leak-kinds=all ./tests.exe

clean:
	rm *.exe *.o
