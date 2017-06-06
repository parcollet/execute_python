!> Fortran 90 demo of Python embedding
!!
!! To compile:
!!   mpif90 -ldl main.f90 execute_py.o
!!
program exec_py
implicit none

include 'mpif.h'

interface

   function init_python_interpreter(python_so) bind(C)
      use iso_c_binding, only: c_int, c_char
      integer(c_int) :: init_python_interpreter !< Return value
      character(kind=c_char), intent(in) :: python_so(*) !< Python lib path
   end function

   function execute_python_file(python_script) bind(C)
      use iso_c_binding, only: c_int, c_char
      integer(c_int) :: execute_python_file !< Return value
      character(kind=c_char), intent(in) :: python_script(*) !< Python script name
   end function

   function close_python_interpreter() bind(C)
      use iso_c_binding, only: c_int, c_char
      integer(c_int) :: close_python_interpreter !< Return value
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

!launch python
ierr = init_python_interpreter(f2c_string(python_so))

!execute script
ierr = execute_python_file(f2c_string(python_script))

call mpi_barrier(MPI_COMM_WORLD,ierr)

!in between calls
print *, "  Call between: I am rank ", mrank, "  out of ", msize

call mpi_barrier(MPI_COMM_WORLD,ierr)

!execute script again
ierr = execute_python_file(f2c_string(python_script))

call mpi_barrier(MPI_COMM_WORLD,ierr)

!close python
ierr = close_python_interpreter()

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
