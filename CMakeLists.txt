cmake_minimum_required(VERSION 3.15)
project(PLoc)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(Src)
add_subdirectory(Tests)
#add_subdirectory(Tools/Modeling/Src)
add_subdirectory(Tools/Modeling/)
