cmake_minimum_required (VERSION 3.1)
project (atomic)
find_package (Threads)
add_executable(atomic atomic/main.cpp)
target_link_libraries (atomic ${CMAKE_THREAD_LIBS_INIT})

