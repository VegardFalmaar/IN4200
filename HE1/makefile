fname=main.cpp

compile:
	# g++ $(fname) -o main.exe -Wall --std=c++17
	# g++ $(fname) -o main.exe -Wall -O3 --std=c++17
	# g++ $(fname) -o main.exe -fopenmp -Wall --std=c++17
	g++ $(fname) -o main.exe -fopenmp -Wall -O3 --std=c++17

run:
	./main.exe

execute: compile run

test:
	### Testing unoptimized serial code
	g++ tests.cpp -o tests.exe --std=c++17 -Wall
	./tests.exe
	### Testing compiler optimized serial code
	g++ tests.cpp -o tests.exe --std=c++17 -Wall -O3
	./tests.exe
	### Testing unoptimized parallel code
	g++ tests.cpp -o tests.exe -fopenmp --std=c++17 -Wall
	./tests.exe
	### Testing compiler optimized parallel code
	g++ tests.cpp -o tests.exe -fopenmp --std=c++17 -Wall -O3
	./tests.exe

memcheck:
	g++ tests.cpp -g -o tests.exe --std=c++17 -Wall
	valgrind --trace-children=yes --track-fds=yes --track-origins=yes --leak-check=full --show-leak-kinds=all ./tests.exe
