cmake_minimum_required(VERSION 3.16)

# 设置c++17标准
set(CMAKE_CXX_STANDARD 17)
# Make PROJECT_SOURCE_DIR, PROJECT_BINARY_DIR, and PROJECT_NAME available.
set(PROJECT_NAME MyProject)
project(${PROJECT_NAME})

set(CMAKE_CXX_FLAGS "-g -Wall -Wno-attributes -Wno-unused-variable")


# If you want your own include/ directory, set this, and then you can do
# include_directories(${COMMON_INCLUDES}) in other CMakeLists.txt files.
# set(COMMON_INCLUDES ${PROJECT_SOURCE_DIR}/include)

################################
# Normal Libraries & Executables
################################
file(GLOB_RECURSE DIR_SRC "src/*.cpp")
include_directories("include")
add_library(project1_lib ${DIR_SRC})
add_executable(project1 main.cpp)
# Key idea: SEPARATE OUT your main() function into its own file so it can be its
# own executable. Separating out main() means you can add this library to be
# used elsewhere.

target_link_libraries(project1 project1_lib)

