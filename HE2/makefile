compile:
	g++ tests.cpp -g -o tests.exe --std=c++17 -Wall

execute: compile
	./tests.exe
	# python3 create_test.py

memcheck:
	g++ tests.cpp -g -o tests.exe --std=c++17 -Wall
	valgrind --trace-children=yes --track-fds=yes --track-origins=yes --leak-check=full --show-leak-kinds=all ./tests.exe
