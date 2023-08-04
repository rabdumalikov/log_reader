cmake_minimum_required(VERSION 3.5.1)

project(logger)

if (WIN32)
    set(EXECUTABLE_OUTPUT_PATH "${logger_SOURCE_DIR}/out/binaries")
endif()

find_package(Git)
message(STATUS "GIT_EXECUTABLE='${GIT_EXECUTABLE}'")

if (CMAKE_BUILD_TYPE MATCHES Debug)
    message("LOGGER Debug.build Compiler=${CMAKE_CXX_COMPILER_ID}")
else()
    # -fno-omit-frame-pointer - use this flag for profiler 
    add_compile_options( -O3 -Wall -pedantic -Wno-unused-lambda-capture )
    message("LOGGER Release.build Compiler=${CMAKE_CXX_COMPILER_ID}")
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    if ("${CMAKE_CXX_SIMULATE_ID}" STREQUAL "MSVC")
        add_compile_options( /std:c++17 )
    else()
        add_compile_options( -std=c++17 )
        add_compile_options( -stdlib=libc++ )
    endif()
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    add_compile_options( /std:c++17 )
endif()

if (Git_FOUND)
    include(DownloadProject.cmake)
    download_project(PROJ                catchtest
                    GIT_REPOSITORY      https://github.com/catchorg/Catch2.git
                    GIT_TAG             v2.x
                    ${UPDATE_DISCONNECTED_IF_AVAILABLE}
    )

    add_subdirectory(${catchtest_SOURCE_DIR} ${catchtest_BINARY_DIR})

    add_executable(matcher_test tests/matcher_tests.cpp src/rules.cpp)
    target_include_directories(matcher_test PUBLIC $<BUILD_INTERFACE:${logger_SOURCE_DIR}>)
    target_include_directories(matcher_test PUBLIC $<BUILD_INTERFACE:${catchtest_SOURCE_DIR}/single_include/>)

    add_executable(log_reader_test tests/log_reader_tests.cpp src/rules.cpp src/matcher.cpp src/log_reader.cpp)
    target_include_directories(log_reader_test PUBLIC $<BUILD_INTERFACE:${logger_SOURCE_DIR}>)
    target_include_directories(log_reader_test PUBLIC $<BUILD_INTERFACE:${catchtest_SOURCE_DIR}/single_include/>)
endif()


add_executable(main src/main.cpp src/log_reader.cpp src/matcher.cpp src/rules.cpp)
target_include_directories(main PUBLIC $<BUILD_INTERFACE:${logger_SOURCE_DIR}>)

add_executable(operator_plus example/operator_plus.cpp src/log_reader.cpp src/matcher.cpp src/rules.cpp)
target_include_directories(operator_plus PUBLIC $<BUILD_INTERFACE:${logger_SOURCE_DIR}>)