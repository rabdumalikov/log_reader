
## Overview:

  

  

Application for filtering out log files.

  

# Main-Files:

  

-  `src/log_reader.hpp` - LogReader implementation

-  `src/matching.hpp` - engine that search for matches using provided set of rules like `*`, and `?`.

-  `src/rules.hpp` - abstraction for controlling and introducing new rules

-  `src/main.cpp` - application that accept two arguments and filter out logs based on provided pattern.

# Unit-Tests:

I have used [Catch2](https://github.com/catchorg/Catch2) as a unit test library.

  

-  `tests/matcher_tests.cpp` - unit tests for _matching.hpp_.

-  `tests/log_reader_tests.cpp` - unit tests for _log_reader.hpp_.

  

# Extension-Example:

  

-  `example/operator_plus.cpp` - contain an example of how a new rule can be introduced. To demonstrate it I have implemented operator `+`.

  
  
  
  
  

_______________

  

  

## Minimum C++ standard:

  

C++17

  

_______________

  

  

## Supported Compilers:

  

I personally tested on following:

  

-  **clang++\-16** (or later)

-  **Visual Studio 2017** (or later)

  

## How to build:

  

- Windows:
	1) **CLONE**: this repository
	2) **OPEN**: In Visual Studio 2017 as follow: `File` &#8594; `Open` &#8594; `CMake`
	3) **BUILD**: `CMake` &#8594; `Build All`
	4) **RUN**: `.\out\binaries\main.exe "sometext.txt" "*pattern*"`

- Linux/Macos:
	1) **CLONE**: this repository
	2) **BUILD**: `./build.sh` `<compiler_name>`
	3) **RUN**: `./build/debug/main "sometext.txt" "*pattern*"`

  

_______________

  

## License:

  

Boost Software License( Version 1.0 )

  

_______________