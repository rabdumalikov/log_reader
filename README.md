## Overview:

  

Implementation for filtering out log files. 
  

# Main-Files:

  
- `log_reader.hpp` - LogReader implementation
- `matching.hpp` - engine that search for matches using provided set of rules like `*`, and `?`.
- `match_rules.hpp` - abstraction for controlling and introducing new rules
- `main.cpp` - application that accept two arguments and filter out logs based on provided pattern.
   
# Unit-Tests:

- `tests/matcher_tests.cpp` - unit tests for _matching.hpp_.
- `tests/log_reader_tests.cpp` - unit tests for _log_reader.hpp_.

# Extension-Example:

- `example/operator_plus.cpp` - example how new rule can be introduced. In particularly I implemented operator `+`.





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