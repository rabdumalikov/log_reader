# Distributed under the OSI-approved MIT License.  See accompanying
# file LICENSE or https://github.com/Crascit/DownloadProject for details.

cmake_minimum_required(VERSION 2.8.2)

project(catchtest-download NONE)

include(ExternalProject)
ExternalProject_Add(catchtest-download
                    GIT_REPOSITORY;https://github.com/catchorg/Catch2.git;GIT_TAG;v2.x;UPDATE_DISCONNECTED 1
                    SOURCE_DIR          "/Users/v/Documents/C++ practice/submition/build/debug/catchtest-src"
                    BINARY_DIR          "/Users/v/Documents/C++ practice/submition/build/debug/catchtest-build"
                    CONFIGURE_COMMAND   ""
                    BUILD_COMMAND       ""
                    INSTALL_COMMAND     ""
                    TEST_COMMAND        ""
)
