# fname="read_graph_from_file1.cpp"
# fname="read_graph_from_file2.cpp"
# fname="read_graph_from_file2_slow.cpp"
# fname=create_SNN_graph1.cpp
fname=main.cpp

compile:
	g++ $(fname) -o main.exe --std=c++17 -Wall -O3

run:
	./main.exe

execute: compile run

compile_test:
	g++ tests.cpp -g -o tests.exe --std=c++17 -Wall -O3

run_test:
	./tests.exe

test: compile_test run_test

run_memcheck:
	valgrind --trace-children=yes --track-fds=yes --track-origins=yes --leak-check=full --show-leak-kinds=all ./tests.exe

memcheck: compile_test run_memcheck
