from mpi4py import MPI

comm = MPI.COMM_WORLD
rank = comm.Get_rank()

print "Python I am node ", rank
