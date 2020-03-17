cmake_minimum_required(VERSION 3.0.0)
project(main VERSION 0.1.0)
SET(CMAKE_CXX_COMPILER "g++")
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -DDEBUG -pthread" )
find_package (Threads REQUIRED)
# find_package(Python REQUIRED)
aux_source_directory(src/ DIR_SRCS)
# 寻找pkg-config，其实是找到与之相关的.cmake文件
find_package(PkgConfig REQUIRED)
# 利用pkg-config寻找opencv.pc这个文件
pkg_check_modules(OPENCV REQUIRED opencv4)
# 链接头文件,打印头文件信息
include_directories(./include) #包含工作区下面的include文件夹，写工程时推荐头文件、源文件分开。
include_directories(${OPENCV_INCLUDE_DIRS})
# 链接库文件
link_libraries(${OPENCV_LIBRARIES})
add_executable(main ${DIR_SRCS})
set(CPACK_PROJECT_NAME ${main})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})