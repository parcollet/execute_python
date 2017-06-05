from mpi4py import MPI
import numpy

comm = MPI.COMM_WORLD
rank = comm.Get_rank()

print "Python I am node ", rank

#from pytriqs.gf import *

