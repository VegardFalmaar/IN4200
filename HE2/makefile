compile:
	# g++ tests.cpp -g -o tests.exe --std=c++17 -Wall
	# mpic++ MPI_double_layer_convolution.cpp -o MPI_double_layer_convolution.exe --std=c++17 -Wall
	/usr/lib64/openmpi/bin/mpic++ MPI_double_layer_convolution.cpp -o MPI_double_layer_convolution.exe --std=c++17 -Wall

execute: compile
	# ./tests.exe
	# ./MPI_double_layer_convolution.exe
	# valgrind --trace-children=yes --track-fds=yes --track-origins=yes --leak-check=full --show-leak-kinds=all ./MPI_double_layer_convolution.exe 10 10 3 3
	# python3 create_test.py
	# mpirun -np 2 ./MPI_double_layer_convolution.exe 8 4 2 2
	/usr/lib64/openmpi/bin/mpirun -np 2 ./MPI_double_layer_convolution.exe 8 4 2 2

memcheck:
	g++ tests.cpp -g -o tests.exe --std=c++17 -Wall
	valgrind --trace-children=yes --track-fds=yes --track-origins=yes --leak-check=full --show-leak-kinds=all ./tests.exe
