cmake_minimum_required(VERSION 3.17)
project(NumerischeMethodenStatistischenPhysik)

find_package(Armadillo REQUIRED)
include_directories(${ARMADILLO_INCLUDE_DIRS})
set(CMAKE_CXX_STANDARD 20)

#add all .cpp files from the Projects. CONFIGURE_DEPENDS not recommended but useful:
#https://cmake.org/cmake/help/latest/command/file.html#glob
file(GLOB projects_SRC CONFIGURE_DEPENDS
        ${PROJECT_SOURCE_DIR}/main.cpp
        ${PROJECT_SOURCE_DIR}/Projects/*/*.cpp)


add_executable(NumerischeMethodenStatistischenPhysik ${projects_SRC})
target_link_libraries(NumerischeMethodenStatistischenPhysik ${ARMADILLO_LIBRARIES})

enable_testing()
add_subdirectory(test/ctest)