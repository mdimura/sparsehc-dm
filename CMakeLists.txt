cmake_minimum_required(VERSION 2.8.8)
project (sparsehc-dm)

FIND_PACKAGE(PythonInterp)
FIND_PACKAGE(PythonLibs)
FIND_PACKAGE(Boost COMPONENTS python)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS} ${PYTHON_INCLUDE_DIRS})
set(LIBS ${LIBS} ${Boost_LIBRARIES} ${PYTHON_LIBRARIES})

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

file(GLOB SHARED_SOURCES src/averagecluster.cpp src/cluster.cpp src/common.cpp src/completecluster.cpp src/dendrogram.cpp src/inmatrix.cpp src/matrix.cpp src/singlecluster.cpp)
ADD_LIBRARY(sparsehc_dm SHARED sparsehc-dm_python.cpp ${SHARED_SOURCES})
target_link_libraries(sparsehc_dm ${LIBS})
set_target_properties(sparsehc_dm PROPERTIES PREFIX "")
