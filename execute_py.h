
// starts the interpreter
int init_python_interpreter(const char* python_so);

// executes code in the Interpreter
int execute_python_file(const char* filename);

// call only ONCE at the very end !
int close_python_interpreter(); 
