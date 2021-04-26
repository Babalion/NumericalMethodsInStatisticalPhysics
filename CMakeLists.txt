cmake_minimum_required(VERSION 3.17)
project(NumerischeMethodenStatistischenPhysik)

find_package(Armadillo REQUIRED)
include_directories(${ARMADILLO_INCLUDE_DIRS})
set(CMAKE_CXX_STANDARD 20)

add_executable(NumerischeMethodenStatistischenPhysik main.cpp HelloWorld.cpp RngTests.cpp)
target_link_libraries(NumerischeMethodenStatistischenPhysik ${ARMADILLO_LIBRARIES})