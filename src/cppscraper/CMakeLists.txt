# CMakeList.txt : CMake project for cppscraper, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (cppscraper "cppscraper.cpp" "cppscraper.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET cppscraper PROPERTY CXX_STANDARD 20)
endif()

# Find libcurl (vcpkg will handle it)
find_package(CURL REQUIRED)

# Link libcurl
target_link_libraries(cppscraper PRIVATE CURL::libcurl)

# TODO: Add tests and install targets if needed.
