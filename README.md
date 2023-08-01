## Overview:

  

Application for filtering out log files. 
  

# Main-Files:

  
- `src/log_reader.hpp` - LogReader implementation
- `src/matching.hpp` - engine that search for matches using provided set of rules like `*`, and `?`.
- `src/rules.hpp` - abstraction for controlling and introducing new rules
- `src/main.cpp` - application that accept two arguments and filter out logs based on provided pattern.
   
# Unit-Tests:
I have used [Catch2](https://github.com/catchorg/Catch2) as a unit test library.

- `tests/matcher_tests.cpp` - unit tests for _matching.hpp_.
- `tests/log_reader_tests.cpp` - unit tests for _log_reader.hpp_.

# Extension-Example:

- `example/operator_plus.cpp` - contain an example of how a new rule can be introduced. To demonstrate it I have implemented operator `+`.





_______________

  

## Minimum C++ standard:

C++17

_______________

  

## Supported Compilers:

  
I personally tested on following:

-  **clang++\-16** (or later)
  

## How to build:

 

*./build.sh < compiler_name >*

_______________

## License:

Boost Software License( Version 1.0 )

_______________