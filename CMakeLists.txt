# 设置最低cmake版本
cmake_minimum_required(VERSION 3.10)
# 设置项目名字
project(LightBarDetect)

# OpenCV路径
set(OpenCV_DIR "/usr/local/opencv4.5.4/lib/cmake/opencv4")

# 寻找OpenCV库
find_package(OpenCV REQUIRED)

# 添加头文件路径
include_directories(${OpenCV_INCLUDE_DIRS})

# 把main.cpp编译成叫detect的程序
add_executable(detect main.cpp)

# 程序链接OpenCV库
target_link_libraries(detect ${OpenCV_LIBS})
