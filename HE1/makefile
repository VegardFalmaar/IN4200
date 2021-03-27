# fname="read_graph_from_file1.cpp"
fname="read_graph_from_file2.cpp"

compile:
	g++ $(fname) -o out.exe --std=c++17 -Wall

run:
	./out.exe

execute: compile run

run_memcheck:
	valgrind --leak-check=yes ./out.exe

memcheck: compile run_memcheck
