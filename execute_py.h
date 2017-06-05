

// Launch the intepreter, runs filename into it and close it
// 
// python_so : location of libpython.xx.so or libpython.xx.dylib on OS X
// filename : script to be run in the interpreter
//
int execute_python_file(const char *python_so, const char *filename);
