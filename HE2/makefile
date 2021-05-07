compile:
	# /usr/lib64/openmpi/bin/mpic++ MPI_main.cpp -o MPI_main.exe --std=c++17 -Wall -O3
	mpic++ MPI_main.cpp -o MPI_main.exe --std=c++17 -Wall -O3

execute: compile
	# /usr/lib64/openmpi/bin/mpirun -np 4 ./MPI_main.exe 500 500 9 8
	mpirun -np 4 ./MPI_main.exe 10000 8000 15 15

test_compile:
	mpic++ tests.cpp -g -o tests.exe --std=c++17 -Wall

test: test_compile
	./tests.exe

memcheck: test_compile
	valgrind --trace-children=yes --track-fds=yes --track-origins=yes --leak-check=full --show-leak-kinds=all ./tests.exe
