#include <dlfcn.h>
#include <stdio.h>

// -----------------------------------------------
// The function to run Python
// -----------------------------------------------

#define AS_STRING(X) AS_STRING2(X)
#define AS_STRING2(X) #X

// loads the function F from the shared lib
#define LOAD(F, R, ...)                                                        \
 R (*F)(__VA_ARGS__);                                                          \
 *(void**)(&F) = dlsym(handle, AS_STRING(F));                                  \
 if ((error = dlerror()) != NULL) {                                            \
  fprintf(stderr, "%s\n", error);                                              \
  return 1;                                                                    \
 }

int execute_python_file(const char* python_so, const char* filename) {
 static void* handle = NULL; // will be not NULL iif Python already runs
 char* error;

 // opens the python shared lib and init the interpreter
 if (!handle) {
  handle = dlopen(python_so, RTLD_GLOBAL | RTLD_LAZY);
  if (!handle) {
   fprintf(stderr, "Can not find Python !\n%s\n", dlerror());
   return 1;
  }
  dlerror();                   // Clear any existing error
  LOAD(Py_Initialize, void, ); // loads the functions that we will need
  (*Py_Initialize)();          // initialize the interpreter
 }

 // check Python is running if handle is not NULL
 LOAD(Py_IsInitialized, int, );
 if (!(*Py_IsInitialized)()) {
  fprintf(stderr, "Python Interpreter failed to initialize");
  return 1;
 }

 // Open the script file, report error, and run it in the interpreter
 FILE* file = fopen(filename, "r");
 if (!file) {
  fprintf(stderr, "file %s not found \n", filename);
  return 1;
 }
 // LOAD(PyRun_SimpleString, int, const char *);
 LOAD(PyRun_SimpleFile, int, FILE*, const char*);
 (*PyRun_SimpleFile)(file, filename);

 // We never close the interpreter nor the handle
 // close the interpreter
 // LOAD(Py_Finalize, void, );
 // (*Py_Finalize)();
 // Never close !
 // close the shared lib
 // dlclose(handle);

 return 0;
}
