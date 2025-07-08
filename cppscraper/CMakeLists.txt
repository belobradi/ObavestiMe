# CMakeList.txt : CMake project for cppscraper, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (cppscraper "cppscraper.cpp" "cppscraper.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET cppscraper PROPERTY CXX_STANDARD 20)
endif()

find_package(CURL REQUIRED)
find_package(unofficial-gumbo CONFIG REQUIRED)

target_link_libraries(cppscraper PRIVATE CURL::libcurl unofficial::gumbo::gumbo)

# TODO: Add tests and install targets if needed.
