// -------------------------------------------------------
// a demo test with a simple mpi C & Python code.
// -------------------------------------------------------
// compile with
// clang execute_py.c main.c  -ldl -rdynamic -lmpi
//
#include "execute_py.h"
#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {

 if (argc != 3) {
  fprintf(stderr, "Requires 2 args : Python shared lib location + script");
  return 1;
 }

 int ierr, num_procs, my_id;
 ierr = MPI_Init(&argc, &argv);

 // do somethign with mpi
 ierr = MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
 ierr = MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
 printf("Hello world! I'm process %i out of %i processes\n", my_id, num_procs);

 // launch python
 execute_python_file(argv[1], argv[2]);

 // check it has not been finalized
 int final;
 MPI_Finalized(&final);
 if (final) {
  fprintf(stderr, "MPI is finalized on node %i\n", my_id);
  return 1;
 }

 // wait for all python to conclude
 // and redo some mpi after closing python
 MPI_Barrier(MPI_COMM_WORLD);

 ierr = MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
 ierr = MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
 printf("Hello again ! I'm process %i out of %i processes\n", my_id, num_procs);

 MPI_Finalize();
}
