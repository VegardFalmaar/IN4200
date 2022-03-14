# IN4200 Home Exam 2 - MPI parallelization of convolution computation

As the tasks specify, I have assumed that `M` and `N` are large compared to
`K1` and `K2` as well as the number of processes used to run the MPI function.

*Note:*<br>
My implementation works with 0 errors (when comparing the MPI parallelized
function to the sequential function) when I run the code on my personal
computer with input matrices of any size. It does, however, report a varying
number of mismatching elements when run on IFI servers with input matrices
above a certain size. (a few hundred) times (a few hundred) elements works
well, but larger matrices cause errors. I am assuming that this is caused by
restricted memory usage/availability on the IFI servers.



## Strategy

I have chosen to implement the `MPI_double_layer_convolution` function such
that each process can apply both kernels to its input before communicating the
final result. I.e there is no communication between the processes to share the
results of the first convolution before the processes proceed to the second
convolution.

This involves a larger initial communication because the different processes
need more overlap in input than what could be the case with other strategies.
It also means that the values of some rows of the intermediate matrix (the
result of applying the first convolution) are computed on more than one
process. The benefit is that it does reduce communication later. This is a
tradeoff which could be examined in detail, but it is not within the scope of
this project.

The division of labor between the different processes is done in a
straight-forward one-dimensional fashion by assigning the input row-wise to the
processes. Say the input consists of 10 rows and you run the code with four
processes, then the input would be divided as follows (not counting overlap):
- Process 0: Rows 0 and 1
- Process 1: Rows 2, 3 and 4
- Process 2: Rows 5 and 6
- Process 3: Rows 7, 8 and 9

In order to reduce memory usage, only process 0 allocates an output array. All
processes use an intermediate array in which the results of the first
convolution are stored. All processes except process 0 reuse the input array to
store the results of the second convolution instead of allocating a new output
array.



## Code

I have implemented the tasks in the exam in C++, and the code is compiled using
the C++17 standard.

The `makefile` shows how to compile and run the code, both on a machine where
the mpi compiler is available as `mpic++` and `mpirun`, and on the IFI servers.

I am using `MPI_Scatterv` with overlap between the data sent to the different
processes. This is permitted on the OpenMPI installations on the IFI servers
(and on my personal computer), but not, as I understand it, on all MPI
installations.

I wrote some tests (in `tests.cpp`) prior to writing the parallelized function.
I later rewrote one of them slightly to verify that the
`single_layer_convolution` function performs as intented, the rest was
discarded as it was inconvenient to test the parallel code this way. As a
result, the test code is unnecessarily verbose. As stated earlier, the results
of the parallelized function is compared to the sequential function in
`MPI_main.cpp`.


### Running the code (please also refer to the makefile)

The code can be run as follows
```bash
make execute    # to compile and run the code
make test       # to compile and run the test of the single layer convolution
```



## Reflection

I would say I have achieved satisfying results with my approach. Running the
code with the following parameters:
- `M` = 10 000
- `N` = 8 000
- `K1` = 11
- `K2` = 9

takes (from a rough average of 3 runs) 1.4 seconds parallelized with eight
processes compared to 9.5 seconds for the serial code.

The best approach is of course dependent on the size of the different matrices.
My approach is best suited for large kernel matrices as the numerical
calculations then take longer compared to communication than for small kernels.
With the same values of `M` and `N` as above, running the code with `K1` and
`K2` smaller than 5 gives a speedup of about 300% using eight processes. With
`K1` and `K2` around 15, the parallel code runs about seven times as fast with
eight processors as the sequential code. This is also the case for smaller
input matrices. For fewer processes, e.g. 4, the speedup is arguable better. A
few simple runs showed that the code executed exactly 4 times as fast with 4
processes as the sequential code.

The effectiveness of my approach depends on the specific size of the problem,
but for values of `K1` and `K2` larger than, say, 10, I am very pleased with
the speedup from the parallelization. It could be worth noting that my approach
may be well suited to be run on a single desktop computer, but could be less
efficient on a network of nodes with slower internode communication.

I have not taken into account different optimizations that could be done to the
`single_layer_convolution` function. Perhaps dividing the input into
2-dimensional blocks instead of moving through the matrix row by row could
increase the chance of reusing data in the lower level caches, thereby reducing
the computing time for both the sequential and the parallel code.