# "cv_demo" is the name of the CMake project and the resulting executable that will be built.
# It is a simple demo application that uses OpenCV (specifically modules core, imgproc, highgui).
# The source files for this demo are Untitled2.cpp and idctutils.cpp.

cmake_minimum_required(VERSION 3.20)
project(cvs_demos CXX)

# Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# (Optional but helpful) Let you override from CLI. Default points to vcpkg OpenCV 2.4:
# You said headers are in C:\vcpkg\installed\x64-windows\include\opencv2.4\opencv2
# For CMake's find_package, the correct hint is the share folder below:
set(OpenCV_ROOT "C:/vcpkg/installed/x64-windows/share/opencv2" CACHE PATH "OpenCV 2.4 root from vcpkg")

# Find OpenCV (2.4). We only need core/imgproc/highgui.
find_package(OpenCV REQUIRED COMPONENTS core imgproc highgui)

message(STATUS "OpenCV version: ${OpenCV_VERSION}")
message(STATUS "OpenCV include dirs: ${OpenCV_INCLUDE_DIRS}")
message(STATUS "OpenCV libs: ${OpenCV_LIBS}")

add_executable(cv_demos
    Untitled2.cpp
    idctutils.cpp
)

# This tells CMake to add the found OpenCV include directories to our project,
# and to link our executable with the OpenCV libraries we requested above.
target_include_directories(cv_demos PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(cv_demos PRIVATE ${OpenCV_LIBS})
