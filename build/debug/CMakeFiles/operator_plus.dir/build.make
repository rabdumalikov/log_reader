# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.27

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.27.0/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.27.0/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/Users/v/Documents/C++ practice/submition"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/Users/v/Documents/C++ practice/submition/build/debug"

# Include any dependencies generated for this target.
include CMakeFiles/operator_plus.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/operator_plus.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/operator_plus.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/operator_plus.dir/flags.make

CMakeFiles/operator_plus.dir/example/operator_plus.cpp.o: CMakeFiles/operator_plus.dir/flags.make
CMakeFiles/operator_plus.dir/example/operator_plus.cpp.o: /Users/v/Documents/C++\ practice/submition/example/operator_plus.cpp
CMakeFiles/operator_plus.dir/example/operator_plus.cpp.o: CMakeFiles/operator_plus.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="/Users/v/Documents/C++ practice/submition/build/debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/operator_plus.dir/example/operator_plus.cpp.o"
	/usr/local/opt/llvm/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/operator_plus.dir/example/operator_plus.cpp.o -MF CMakeFiles/operator_plus.dir/example/operator_plus.cpp.o.d -o CMakeFiles/operator_plus.dir/example/operator_plus.cpp.o -c "/Users/v/Documents/C++ practice/submition/example/operator_plus.cpp"

CMakeFiles/operator_plus.dir/example/operator_plus.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/operator_plus.dir/example/operator_plus.cpp.i"
	/usr/local/opt/llvm/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/v/Documents/C++ practice/submition/example/operator_plus.cpp" > CMakeFiles/operator_plus.dir/example/operator_plus.cpp.i

CMakeFiles/operator_plus.dir/example/operator_plus.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/operator_plus.dir/example/operator_plus.cpp.s"
	/usr/local/opt/llvm/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/v/Documents/C++ practice/submition/example/operator_plus.cpp" -o CMakeFiles/operator_plus.dir/example/operator_plus.cpp.s

CMakeFiles/operator_plus.dir/src/log_reader.cpp.o: CMakeFiles/operator_plus.dir/flags.make
CMakeFiles/operator_plus.dir/src/log_reader.cpp.o: /Users/v/Documents/C++\ practice/submition/src/log_reader.cpp
CMakeFiles/operator_plus.dir/src/log_reader.cpp.o: CMakeFiles/operator_plus.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="/Users/v/Documents/C++ practice/submition/build/debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/operator_plus.dir/src/log_reader.cpp.o"
	/usr/local/opt/llvm/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/operator_plus.dir/src/log_reader.cpp.o -MF CMakeFiles/operator_plus.dir/src/log_reader.cpp.o.d -o CMakeFiles/operator_plus.dir/src/log_reader.cpp.o -c "/Users/v/Documents/C++ practice/submition/src/log_reader.cpp"

CMakeFiles/operator_plus.dir/src/log_reader.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/operator_plus.dir/src/log_reader.cpp.i"
	/usr/local/opt/llvm/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/v/Documents/C++ practice/submition/src/log_reader.cpp" > CMakeFiles/operator_plus.dir/src/log_reader.cpp.i

CMakeFiles/operator_plus.dir/src/log_reader.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/operator_plus.dir/src/log_reader.cpp.s"
	/usr/local/opt/llvm/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/v/Documents/C++ practice/submition/src/log_reader.cpp" -o CMakeFiles/operator_plus.dir/src/log_reader.cpp.s

CMakeFiles/operator_plus.dir/src/matcher.cpp.o: CMakeFiles/operator_plus.dir/flags.make
CMakeFiles/operator_plus.dir/src/matcher.cpp.o: /Users/v/Documents/C++\ practice/submition/src/matcher.cpp
CMakeFiles/operator_plus.dir/src/matcher.cpp.o: CMakeFiles/operator_plus.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="/Users/v/Documents/C++ practice/submition/build/debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/operator_plus.dir/src/matcher.cpp.o"
	/usr/local/opt/llvm/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/operator_plus.dir/src/matcher.cpp.o -MF CMakeFiles/operator_plus.dir/src/matcher.cpp.o.d -o CMakeFiles/operator_plus.dir/src/matcher.cpp.o -c "/Users/v/Documents/C++ practice/submition/src/matcher.cpp"

CMakeFiles/operator_plus.dir/src/matcher.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/operator_plus.dir/src/matcher.cpp.i"
	/usr/local/opt/llvm/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/v/Documents/C++ practice/submition/src/matcher.cpp" > CMakeFiles/operator_plus.dir/src/matcher.cpp.i

CMakeFiles/operator_plus.dir/src/matcher.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/operator_plus.dir/src/matcher.cpp.s"
	/usr/local/opt/llvm/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/v/Documents/C++ practice/submition/src/matcher.cpp" -o CMakeFiles/operator_plus.dir/src/matcher.cpp.s

CMakeFiles/operator_plus.dir/src/match_rules.cpp.o: CMakeFiles/operator_plus.dir/flags.make
CMakeFiles/operator_plus.dir/src/match_rules.cpp.o: /Users/v/Documents/C++\ practice/submition/src/match_rules.cpp
CMakeFiles/operator_plus.dir/src/match_rules.cpp.o: CMakeFiles/operator_plus.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="/Users/v/Documents/C++ practice/submition/build/debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/operator_plus.dir/src/match_rules.cpp.o"
	/usr/local/opt/llvm/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/operator_plus.dir/src/match_rules.cpp.o -MF CMakeFiles/operator_plus.dir/src/match_rules.cpp.o.d -o CMakeFiles/operator_plus.dir/src/match_rules.cpp.o -c "/Users/v/Documents/C++ practice/submition/src/match_rules.cpp"

CMakeFiles/operator_plus.dir/src/match_rules.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/operator_plus.dir/src/match_rules.cpp.i"
	/usr/local/opt/llvm/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/v/Documents/C++ practice/submition/src/match_rules.cpp" > CMakeFiles/operator_plus.dir/src/match_rules.cpp.i

CMakeFiles/operator_plus.dir/src/match_rules.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/operator_plus.dir/src/match_rules.cpp.s"
	/usr/local/opt/llvm/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/v/Documents/C++ practice/submition/src/match_rules.cpp" -o CMakeFiles/operator_plus.dir/src/match_rules.cpp.s

# Object files for target operator_plus
operator_plus_OBJECTS = \
"CMakeFiles/operator_plus.dir/example/operator_plus.cpp.o" \
"CMakeFiles/operator_plus.dir/src/log_reader.cpp.o" \
"CMakeFiles/operator_plus.dir/src/matcher.cpp.o" \
"CMakeFiles/operator_plus.dir/src/match_rules.cpp.o"

# External object files for target operator_plus
operator_plus_EXTERNAL_OBJECTS =

operator_plus: CMakeFiles/operator_plus.dir/example/operator_plus.cpp.o
operator_plus: CMakeFiles/operator_plus.dir/src/log_reader.cpp.o
operator_plus: CMakeFiles/operator_plus.dir/src/matcher.cpp.o
operator_plus: CMakeFiles/operator_plus.dir/src/match_rules.cpp.o
operator_plus: CMakeFiles/operator_plus.dir/build.make
operator_plus: CMakeFiles/operator_plus.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir="/Users/v/Documents/C++ practice/submition/build/debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX executable operator_plus"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/operator_plus.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/operator_plus.dir/build: operator_plus
.PHONY : CMakeFiles/operator_plus.dir/build

CMakeFiles/operator_plus.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/operator_plus.dir/cmake_clean.cmake
.PHONY : CMakeFiles/operator_plus.dir/clean

CMakeFiles/operator_plus.dir/depend:
	cd "/Users/v/Documents/C++ practice/submition/build/debug" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/Users/v/Documents/C++ practice/submition" "/Users/v/Documents/C++ practice/submition" "/Users/v/Documents/C++ practice/submition/build/debug" "/Users/v/Documents/C++ practice/submition/build/debug" "/Users/v/Documents/C++ practice/submition/build/debug/CMakeFiles/operator_plus.dir/DependInfo.cmake" "--color=$(COLOR)"
.PHONY : CMakeFiles/operator_plus.dir/depend

