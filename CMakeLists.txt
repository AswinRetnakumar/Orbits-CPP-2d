cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(SFML_INTRO)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(SFML
  COMPONENTS 
    system window graphics network audio REQUIRED)

add_executable(ORBIT orbits.cpp)
target_link_libraries(ORBIT sfml-graphics sfml-window sfml-system)
