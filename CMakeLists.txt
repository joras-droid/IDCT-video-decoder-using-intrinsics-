cmake_minimum_required(VERSION 3.20)
project(cvs_demos CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(OpenCV_ROOT "${VCPKG_INSTALLED_DIR}/x64-windows/share/opencv4")
  
find_package(OpenCV REQUIRED)
add_executable(cv_demos Untitled2.cpp idctutils.cpp)
target_link_libraries(cv_demos PRIVATE ${OpenCV_LIBS})