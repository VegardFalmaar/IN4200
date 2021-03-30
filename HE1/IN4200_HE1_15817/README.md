# IN4200 Home Exam 1 - Counting shared nearest neighbors
Candidate number: 15817



## Code

I have implemented the tasks in the exam in C++, and the code is compiled using
the C++17 standard.


### Declaration of variables

The following description assumes variables defined as
follows:
```c++
char filename[] = "path_to_file";
int N;

// for the 2D matrix implementation
char **table2D;
int **SNN_table;

// for the 1D CRS implementation
int *row_ptr, *col_idx, *SNN_val;
```

### Project code

The following code files are included in the delivery of the exam (note that in
        the code the `const` keyword is added in the function declarations to
        arguments that will not be changed within the function):

- `read_graph_from_file1.cpp`

    Contains the function
    `void read_graph_from_file1 (char *filename, int *N, char ***table2D)`
    designed to be called with
    `read_graph_from_file1(filename, &N, &table2D)`.
    The function sets the value of `N`, allocates a contiguous block of memory
    for `table2D`, and fills it with values.

- `read_graph_from_file2.cpp`

    Contains the function
    `void read_graph_from_file2 (char *filename, int *N, int **row_ptr, int
            **col_idx)`
    designed to be called with
    `read_graph_from_file2 (filename, &N, &row_ptr, &col_idx)`.
    The function sets the values of `N`, allocates memory for `row_ptr` and
    `col_idx`, and fills them with values.

- `create_SNN_graph1.cpp`

    Contains the function
    `void create_SNN_graph1 (int N, char **table2D, int ***SNN_table)`
    designed to be called with
    `create_SNN_graph1(N, table2D, &SNN_table)`
    after the values of `N` and `table2D` have been set by
    `read_graph_from_file1`. The function allocates memory for `SNN_table` and
    fills it with values.

- `create_SNN_graph2.cpp`

    Contains the function
    `void create_SNN_graph2 (int N, int *row_ptr, int *col_idx, int **SNN_val)`
    designed to be called with
    `create_SNN_graph2(N, row_ptr, col_idx, &SNN_val)`
    after the values of `N`, `row_ptr`, and `col_idx` have been set by
    `read_graph_from_file2`. The function allocates memory for `SNN_val` and
    fills it with values.

- `check_node.cpp`

    Contains the function
    `void check_node (int node_idx, int tau, int N, int *row_ptr, int
            *col_idx,int *SNN_val)`
    designed to be called with
    `check_node(node_idx, tau, N, row_ptr, col_idx, SNN_val)`
    for two arbitrary positive integers `node_idx` and `tau`. If `node_idx` is
    greater than or equal to the number of nodes `N` an error message is
    printed. The function checks whether the node corresponding to `node_idx`
    can form a cluster with the given limit `tau`. If yes, it prints the
    indices of all the nodes in the cluster.

- `tests.cpp`

    Contains a separate test function for each of the four first functions
    described above. All the test functions are called in a `main` function.
    The tests are designed to be run using the makefile. The use and sample
    output is described below. The test case is the simple example given in the
    task sheet with a few illegal values added.

- `makefile`

    Contains instructions for compilation and execution of the programs. The
    relevant commands are

    - `make test`

        This compiles and runs the test functions for both serial and parallel
        code, with and without compiler optimization. Sample output is shown
        below.

    - `make memcheck` (requires [Valgrind](https://valgrind.org/))

        This compiles and runs the test code while using Valgrind to check for
        memory leaks and errors.

- `main.cpp`

    Contains example code used during development. The functions above are
    called to read and process the data in the Facebook dataset
    (`data/facebook_combined.txt`). The execution times of the functions are
    measured and printed to the terminal. The code can be compiled and run with
    `make execute`. If desired, the code can be compiled without
    parallelization and/or compiler optimization by commenting/uncommenting
    lines 4-7 in the makefile.


### Deallocation of memory

Following the declaration of variables and calling of functions as described
above, the deallocation of memory should be done like this:
```c++
// for the 2D matrix implementation
delete [] table2D[0];     // deallocate contiguous block of memory
delete [] table2D;
delete [] SNN_table[0];
delete [] SNN_table;

// for the 1D CRS implementation
delete [] row_ptr;
delete [] col_idx;
delete [] SNN_val;
```


### Sample run
```bash
# run the tests
make test
# run the tests while checking for memory errors
make memcheck
# compile and execute the code in main.cpp
make execute
```


### Sample output of `make test`
```bash
### Testing unoptimized serial code
g++ tests.cpp -o tests.exe --std=c++17 -Wall
./tests.exe
7 illegal values excluded
test_read_graph_from_file1() .... Passed
7 illegal values excluded
test_read_graph_from_file2() .... Passed
7 illegal values excluded
test_create_SNN_graph1() ........ Passed
7 illegal values excluded
test_create_SNN_graph2() ........ Passed

### Testing compiler optimized serial code
g++ tests.cpp -o tests.exe --std=c++17 -Wall -O3
./tests.exe
7 illegal values excluded
test_read_graph_from_file1() .... Passed
7 illegal values excluded
test_read_graph_from_file2() .... Passed
7 illegal values excluded
test_create_SNN_graph1() ........ Passed
7 illegal values excluded
test_create_SNN_graph2() ........ Passed

### Testing unoptimized parallel code
g++ tests.cpp -o tests.exe -fopenmp --std=c++17 -Wall
./tests.exe
7 illegal values excluded
test_read_graph_from_file1() .... Passed
7 illegal values excluded
test_read_graph_from_file2() .... Passed
7 illegal values excluded
test_create_SNN_graph1() ........ Passed
7 illegal values excluded
test_create_SNN_graph2() ........ Passed

### Testing compiler optimized parallel code
g++ tests.cpp -o tests.exe -fopenmp --std=c++17 -Wall -O3
./tests.exe
7 illegal values excluded
test_read_graph_from_file1() .... Passed
7 illegal values excluded
test_read_graph_from_file2() .... Passed
7 illegal values excluded
test_create_SNN_graph1() ........ Passed
7 illegal values excluded
test_create_SNN_graph2() ........ Passed
```

