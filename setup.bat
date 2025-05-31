git clone https://github.com/microsoft/vcpkg.git tools\vcpkg
.\tools\vcpkg\bootstrap-vcpkg.bat
tools\vcpkg\vcpkg integrate install
cmake --preset default