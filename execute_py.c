#include <dlfcn.h>
#include <stdio.h>

// -----------------------------------------------
// The function to run Python
// -----------------------------------------------

#define AS_STRING(X) AS_STRING2(X)
#define AS_STRING2(X) #X

// loads the function F from the shared lib
#define LOAD(F, R, ...)                                                        \
 void (*F)(__VA_ARGS__);                                                       \
 *(void**)(&F) = dlsym(handle, AS_STRING(F));                                  \
 if ((error = dlerror()) != NULL) {                                            \
  fprintf(stderr, "%s\n", error);                                              \
  return 1;                                                                    \
 }

int execute_python_file(const char* python_so, const char* filename) {
 static void* handle = NULL;
 char* error;

 // opens the python shared lib and report errors
 if (!handle) {
  printf("Opening Python Interpreter\n");
  
  handle = dlopen(python_so, RTLD_GLOBAL | RTLD_LAZY);
  if (!handle) {
   fprintf(stderr, "Can not find Python !\n%s\n", dlerror());
   return 1;
  }
  dlerror(); // Clear any existing error

  // loads the functions that we will need
  LOAD(Py_Initialize, void*, );
  LOAD(Py_Finalize, void*, );

  // initialize the interpreter
  (*Py_Initialize)();
 }

 // LOAD(PyRun_SimpleString, int, const char *);
 LOAD(PyRun_SimpleFile, int, FILE*, const char*);

 // Open the script file, report error, and run it in the interpreter
 FILE* file = fopen(filename, "r");
 if (!file) {
  fprintf(stderr, "file %s not found \n", filename);
  return 1;
 }
 (*PyRun_SimpleFile)(file, filename);

 // close the interpreter
 // (*Py_Finalize)();

 // Never close !
 // close the shared lib
 // dlclose(handle);

 return 0;
}
