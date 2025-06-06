# CMakeList.txt : CMake project for cppscraper, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.21)

project (cppscraper)

find_package(CURL REQUIRED)

set(CMAKE_CXX_STANDARD 23)

add_executable (cppscraper "cppscraper.cpp" "cppscraper.h")

target_link_libraries(cppscraper PRIVATE CURL::libcurl)