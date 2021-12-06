cmake_minimum_required(VERSION 3.20)
project(mpi)

set(CMAKE_CXX_STANDARD 14)

include_directories(/usr/local/Cellar/open-mpi/4.1.0/include)

add_executable(mpi "first module/task1.cpp" "first module/task2.cpp" "second module/task1.cpp" "second module/task3.cpp")

set(CMAKE_C_COMPILER "/usr/local/Cellar/open-mpi/4.1.0/bin/mpicc")
set(CMAKE_CXX_COMPILER "/usr/local/Cellar/open-mpi/4.1.0/bin/mpic++")

cmake_minimum_required(VERSION 3.6)