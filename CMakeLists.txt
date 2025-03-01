cmake_minimum_required(VERSION 2.5)
project(LSX_muduo)

#yyg_muduo最终编译成so 动态库，设置动态库的输出路径，放在根目录的ilb文件夹下
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)  #别写成LIBRAY_OUTPUT_DIRECTORY了

#设置调试信息以及启动C++11语言标准
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11")

include_directories(${PROJECT_SOURCE_DIR}/include)
#定义参与编译的源代码文件
aux_source_directory(./src SRC_LIST)
#编译生成动态库LSX_muduo
add_library(LSX_muduo SHARED ${SRC_LIST})