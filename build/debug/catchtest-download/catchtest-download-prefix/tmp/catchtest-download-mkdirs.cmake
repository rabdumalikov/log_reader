# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/Users/v/Documents/C++ practice/submition/build/debug/catchtest-src"
  "/Users/v/Documents/C++ practice/submition/build/debug/catchtest-build"
  "/Users/v/Documents/C++ practice/submition/build/debug/catchtest-download/catchtest-download-prefix"
  "/Users/v/Documents/C++ practice/submition/build/debug/catchtest-download/catchtest-download-prefix/tmp"
  "/Users/v/Documents/C++ practice/submition/build/debug/catchtest-download/catchtest-download-prefix/src/catchtest-download-stamp"
  "/Users/v/Documents/C++ practice/submition/build/debug/catchtest-download/catchtest-download-prefix/src"
  "/Users/v/Documents/C++ practice/submition/build/debug/catchtest-download/catchtest-download-prefix/src/catchtest-download-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/v/Documents/C++ practice/submition/build/debug/catchtest-download/catchtest-download-prefix/src/catchtest-download-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/Users/v/Documents/C++ practice/submition/build/debug/catchtest-download/catchtest-download-prefix/src/catchtest-download-stamp${cfgdir}") # cfgdir has leading slash
endif()
