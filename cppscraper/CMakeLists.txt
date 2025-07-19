# CMakeList.txt : CMake project for cppscraper, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Copy test data files to the build directory.
set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../tests/test_data")
set(DESTINATION_DIR "${CMAKE_CURRENT_BINARY_DIR}/tests/test_data")
file(MAKE_DIRECTORY "${DESTINATION_DIR}")
file(COPY "${SOURCE_DIR}/" DESTINATION "${DESTINATION_DIR}")

# Add source to this project's executable.
add_executable (cppscraper "main.cpp" "GumboParser.cpp" "HtmlReader.cpp" "DataWriter.cpp")

# Enable Hot Reload for MSVC compilers if supported.
if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET cppscraper PROPERTY CXX_STANDARD 20)
endif()

# Find and link required libraries.
find_package(CURL REQUIRED)
find_package(unofficial-gumbo CONFIG REQUIRED)
find_package(SQLite3 REQUIRED)

target_link_libraries(cppscraper PRIVATE CURL::libcurl unofficial::gumbo::gumbo SQLite::SQLite3)

# Set the C++ standard and compiler options.
target_compile_options(cppscraper PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/utf-8 /EHsc>
)