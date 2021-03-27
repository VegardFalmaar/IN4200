# fname="read_graph_from_file1.cpp"
# fname="read_graph_from_file2.cpp"
# fname="read_graph_from_file2_slow.cpp"
# fname=create_SNN_graph1.cpp
fname=main.cpp

compile:
	g++ $(fname) -o out.exe --std=c++17 -Wall #-O3

run:
	./out.exe

execute: compile run

run_memcheck:
	valgrind --leak-check=yes ./out.exe

memcheck: compile run_memcheck

compile_test:
	g++ tests.cpp -o tests.exe --std=c++17 -Wall

run_test:
	./tests.exe

test: compile_test run_test
