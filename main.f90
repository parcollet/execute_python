!> Fortran 90 demo of Python embedding
!!
!! To compile:
!!   mpif90 -ldl main.f90 execute_py.o
!!
program exec_py
implicit none

include 'mpif.h'

interface
   function execute_python_file(python_so, python_script) bind(C)
      use iso_c_binding, only: c_int, c_char
      integer(c_int) :: execute_python_file !< Return value
      character(kind=c_char), intent(in) :: python_so(*) !< Python lib path
      character(kind=c_char), intent(in) :: python_script(*) !< Python script name
   end function
end interface

!
! Main program
!
integer :: narg
integer :: ierr, mrank, msize
character(len=256) :: python_so, python_script

narg = command_argument_count()
if(narg /= 2) then
   print*, "  Requires 2 args: Python shared lib location + script"
   stop
endif

call get_command_argument(1, python_so)
call get_command_argument(2, python_script)

! MPI part
call mpi_init(ierr)

call mpi_comm_size(MPI_COMM_WORLD, msize, ierr)
call mpi_comm_rank(MPI_COMM_WORLD, mrank, ierr)

if(mrank == 0) then
   print*, "  Python lib: ", trim(python_so)
   print*, "  Python script: ", trim(python_script)
endif

print *, "  I am rank ", mrank, "  out of ", msize

ierr = execute_python_file(f2c_string(python_so), f2c_string(python_script))

call mpi_finalize(ierr)

!
! Helper function
!
contains
  function f2c_string(f_str)
     use iso_c_binding, only: c_char, c_null_char
     implicit none
     character(len=*), intent(in) :: f_str
     character(kind=c_char, len=255) :: f2c_string
     f2c_string = trim(f_str)//c_null_char
  end function
end program
