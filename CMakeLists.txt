cmake_minimum_required(VERSION 2.8.3)
project (sparsehc-dm)

find_package(STXXL REQUIRED)
include_directories(${STXXL_INCLUDE_DIRS})
set(LIBS ${LIBS} ${STXXL_LIBRARIES})

find_package(OpenMP REQUIRED)
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")

include_directories(src)
file(GLOB SOURCES "src/*.cpp")
add_executable(sparsehc-dm ${SOURCES})
target_link_libraries(sparsehc-dm ${LIBS})
add_definitions(-std=c++11)
