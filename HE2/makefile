compile:
	# g++ tests.cpp -g -o tests.exe --std=c++17 -Wall
	g++ MPI_double_layer_convolution.cpp -g -o MPI_double_layer_convolution.exe --std=c++17 -Wall

execute: compile
	# ./tests.exe
	./MPI_double_layer_convolution.exe
	# python3 create_test.py

memcheck:
	g++ tests.cpp -g -o tests.exe --std=c++17 -Wall
	valgrind --trace-children=yes --track-fds=yes --track-origins=yes --leak-check=full --show-leak-kinds=all ./tests.exe
