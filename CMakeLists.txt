cmake_minimum_required(VERSION 3.2)
project(Convolution CXX CUDA)
#set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set (CMAKE_CXX_COMPILER clang++)
set (CMAKE_C_COMPILER clang)
cmake_policy(SET CMP0104 NEW)
set(CMAKE_CUDA_ARCHITECTURES 70)


find_package(CUDA REQUIRED)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS} "include")
file(GLOB SRC "src/*.cpp" "src/*.cu")
add_executable(main ${SRC})
target_link_libraries(main ${OpenCV_LIBS})