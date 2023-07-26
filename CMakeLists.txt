cmake_minimum_required(VERSION 3.5.1)

project(logger)

add_compile_options( -std=c++17 )

IF(CMAKE_BUILD_TYPE MATCHES Debug)
    message("LOGGER Debug.build Compiler=${CMAKE_CXX_COMPILER_ID}")
ELSE()
    # -fno-omit-frame-pointer - use this flag for profiler 
    add_compile_options( -O3 -Wall -pedantic -Wno-unused-lambda-capture )
    message("LOGGER Release.build Compiler=${CMAKE_CXX_COMPILER_ID}")
ENDIF()

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_compile_options( -stdlib=libc++ )
endif()

if (CMAKE_VERSION VERSION_LESS 3.2)
    set(UPDATE_DISCONNECTED_IF_AVAILABLE "")
else()
    set(UPDATE_DISCONNECTED_IF_AVAILABLE "UPDATE_DISCONNECTED 1")
endif()

include(DownloadProject.cmake)
download_project(PROJ                catchtest
                 GIT_REPOSITORY      https://github.com/catchorg/Catch2.git
                 GIT_TAG             v2.x
                 ${UPDATE_DISCONNECTED_IF_AVAILABLE}
)

add_subdirectory(${catchtest_SOURCE_DIR} ${catchtest_BINARY_DIR})

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++ -lc++abi")
endif()

add_executable(matcher_test tests/matcher_tests.cpp match_rules.cpp)
target_include_directories(matcher_test PUBLIC $<BUILD_INTERFACE:${logger_SOURCE_DIR}>)
target_include_directories(matcher_test PUBLIC $<BUILD_INTERFACE:${catchtest_SOURCE_DIR}/single_include/>)

add_executable(log_reader_test tests/log_reader_tests.cpp match_rules.cpp matcher.cpp log_reader.cpp)
target_include_directories(log_reader_test PUBLIC $<BUILD_INTERFACE:${logger_SOURCE_DIR}>)
target_include_directories(log_reader_test PUBLIC $<BUILD_INTERFACE:${catchtest_SOURCE_DIR}/single_include/>)

add_executable(main main.cpp log_reader.cpp matcher.cpp match_rules.cpp)
target_include_directories(main PUBLIC $<BUILD_INTERFACE:${logger_SOURCE_DIR}>)
target_include_directories(main PUBLIC $<BUILD_INTERFACE:${catchtest_SOURCE_DIR}/single_include/>)

add_executable(operator_plus example/operator_plus.cpp log_reader.cpp matcher.cpp match_rules.cpp)
target_include_directories(operator_plus PUBLIC $<BUILD_INTERFACE:${logger_SOURCE_DIR}>)
target_include_directories(operator_plus PUBLIC $<BUILD_INTERFACE:${catchtest_SOURCE_DIR}/single_include/>)


if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options( -Winline )
endif()

#function(build_source_file base_name )
#  message(STATUS ${base_name})
#  add_executable(${base_name} ${base_name}.cpp)
#  target_include_directories(${base_name} PUBLIC $<BUILD_INTERFACE:${eswitch_SOURCE_DIR}/include/eswitch>)
#endfunction(build_source_file)


# list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/)
 #include (gcovr)

 #add_subdirectory("examples")
 #add_subdirectory("doc")