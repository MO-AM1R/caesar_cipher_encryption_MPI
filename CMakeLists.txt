cmake_minimum_required(VERSION 3.27)
project(Problem_2)
set(CMAKE_C_STANDARD 23)
add_executable(Problem_2 main.c)

set(CMAKE_CXX_STANDARD 14)
find_package(MPI REQUIRED)
target_link_libraries(Problem_2 PUBLIC MPI::MPI_CXX)