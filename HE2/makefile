compile:
	# /usr/lib64/openmpi/bin/mpic++ MPI_double_layer_convolution.cpp -o MPI_double_layer_convolution.exe --std=c++17 -Wall
	mpic++ MPI_main.cpp -o MPI_main.exe --std=c++17 -Wall -O3

execute: compile
	# /usr/lib64/openmpi/bin/mpirun -np 4 ./MPI_double_layer_convolution.exe 5000 5000 3 3
	# mpirun -np 3 ./MPI_main.exe 15 8 3 3
	# mpirun -np 12 ./MPI_main.exe 10000 8000 11 9
	mpirun -np 8 ./MPI_main.exe 10000 8000 11 9

test_compile:
	mpic++ tests.cpp -g -o tests.exe --std=c++17 -Wall

test_execute:
	./tests.exe

memcheck: test_compile
	valgrind --trace-children=yes --track-fds=yes --track-origins=yes --leak-check=full --show-leak-kinds=all ./tests.exe
