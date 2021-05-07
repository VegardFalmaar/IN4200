compile:
	# /usr/lib64/openmpi/bin/mpic++ MPI_double_layer_convolution.cpp -o MPI_double_layer_convolution.exe --std=c++17 -Wall -O3
	mpic++ MPI_double_layer_convolution.cpp -o MPI_double_layer_convolution.exe --std=c++17 -Wall -O3

execute: compile
	# /usr/lib64/openmpi/bin/mpirun -np 2 ./MPI_double_layer_convolution.exe 8 4 2 2
	# mpirun -np 4 ./MPI_double_layer_convolution.exe 10 10 1 1
	mpirun -np 8 ./MPI_double_layer_convolution.exe 10000 8000 9 9

test_compile:
	g++ tests.cpp -g -o tests.exe --std=c++17 -Wall

test_execute:
	./tests.exe

memcheck: test_compile
	valgrind --trace-children=yes --track-fds=yes --track-origins=yes --leak-check=full --show-leak-kinds=all ./tests.exe
