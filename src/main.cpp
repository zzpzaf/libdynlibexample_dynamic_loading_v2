//libm_dynamic_loading_v2

/* 
Dynamic Loading of a shared library and using one of its functions
Targer library: "libdynlibexample.so" (or "libdynlibexample.dylib" for macOS)
(You can put it in the bin folder)
Target library's functions: "message", "factor", and "revstring"
*/


#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <string>
#include <sstream>
#include "utils.h"


// int getfirstinteger(std::string);

int main(int argc, char **argv)
{


    // Here we define the name of the shared library 
    std::string soname;
    //--soname = "libm"; 
    soname = "libdynlibexample";
    // For Linux-es the extension is 'so' For macOS-es is 'dylib'
    std::string libext = "so";
    #ifdef __APPLE__
        libext = "dylib";
    #endif 
    std::string libfullname = soname + "." + libext;

    // Here we define the function symbols/function names
    //-- std::string func;
    std::string func1, func2, func3;
    // -- func1 = "sqrt";
    func1 = "message";
    func2 = "factor";
    func3 = "revstring";


    // Here we declare just a pointer (a handler) of our target library  
    void * plibobj;

    // If we are going to access a function we have to know its type and the type of its parameters
    // Here we declare the pointers of our target functions 
    //-- double (*fsqr)(double nn) ;
    void (*fmessage)();
    int (*ffactor)(int nn);
    std::string (*frevstring)(std::string, int);

    // Load the library object in a new-allocated memory (*handle)
    // Since we dont care about other symbols of the library, here, we use the RTL_LAZY mode-flag.
    // RTL_LAZY generally instructs the dlopen to resolve symbols only if the code references them, and upon code execution.
    // If the symbol is never referenced, then it is never resolved. Note that RTL_LAZY is OK for function references. 
    // However, for references to variables, better to use the RTL_NOW to resolve them immediatly upon library loading
    plibobj = dlopen(libfullname.c_str(), RTLD_LAZY); 
    // If there is an error, output it and exit
    if (!plibobj) {
        std::cerr << "Error loading the library " + libfullname + " - " << dlerror() << "\n";
        // fprintf(stderr, "%s\n",error);
        exit(EXIT_FAILURE);
    }

    //Clear any error
    dlerror();    

  

    // Here we get the pointers of our target functions, each one is just a pointer to an undefined object
    void * psqr1 =  dlsym(plibobj, func1.c_str()) ;
    void * psqr2 =  dlsym(plibobj, func2.c_str()) ;
    void * psqr3 =  dlsym(plibobj, func3.c_str()) ;
    
    // Again, if there is an error accessing the symbol, output it and exit
    if (psqr1 == NULL) {
        std::cerr << "Error accessing the symbol:" + func1 + dlerror() << "\n";
        exit(EXIT_FAILURE);
    } 
        if (psqr2 == NULL) {
        std::cerr << "Error accessing the symbol:" + func2 + dlerror() << "\n";
        exit(EXIT_FAILURE);
    } 
        if (psqr3 == NULL) {
        std::cerr << "Error accessing the symbol:" + func3 + dlerror() << "\n";
        exit(EXIT_FAILURE);
    } 

    // In C++, a void pointer (here the *psqr returned as result of dlsym) has to be converted to a pointer of the appropriate type before it can be used.
    // Here we convert the pointers returned from dlsym, to the type of each target function
    // Actually we cast it to the declared type of the pointer of the target function.
    // For casting we use the reinterpret_cast<>. It does not check if the pointer type and data pointed by the pointer is same or not.
    // The reinterpret_cast converts any pointer type to any other pointer type. And guarantees that when we again reinterpret_cast the 
    // casted pointer back to the original type, we get the original value. 
    fmessage = reinterpret_cast<decltype(fmessage)>(psqr1);
    ffactor = reinterpret_cast<decltype(ffactor)>(psqr2);
    frevstring = reinterpret_cast<decltype(frevstring)>(psqr3);


    // Just invoke the void function (that outputs a message defined in the loaded library)
     (*fmessage)();
    std::cout << "\n";





    // Get a number from user-input and calculate its factorial
    int n; 
    std::string fstr;
    std::cout << "Give me an integer from 2 to 31 to find its factorial: ";
    //std::cin >> n;
    std::getline(std::cin >> std::ws, fstr );
    // Call this function to get only the first decimal digits (as integer) from the input 
    n = getfirstinteger(fstr);
    if (n < 2) n = 2;
    if (n > 31) n = 31;

    // Calculate the factorial result    
    int r = (*ffactor)(n);

    // Output the input number, and the result
    printf("The factorial of %d is: %d\n", n, r);





    // Get a phrase and a number from user-input and return its reversed characters
    // First get the phrase
    std::string str = "";
    std::cout << "Give me a word/phrase to reverse it: ";
    //std::cin >> str;
    std::getline(std::cin >> std::ws, str );

    // Next, get the number of characters
    int m = 0; 
    std::cout << "Give me th number of reversing characters : "; // << '\n';
    std::string lstr="";
    std::getline(std::cin >> std::ws, lstr );

    // Call this function to get only the first decimal digits (as integer) from the input    
    m = getfirstinteger(lstr);

    std::string rstr = (*frevstring)(str, m);
    std::cout << "The input string is: \'" << str << "\'. The reverse part of the last " << m << " characters is: \'" << rstr << "\'.\n";  






    //Unload the library - free memory
    dlclose(plibobj);   
    exit(EXIT_SUCCESS);
    
}




