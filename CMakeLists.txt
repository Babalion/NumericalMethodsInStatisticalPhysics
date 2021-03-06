cmake_minimum_required(VERSION 3.17)
project(NumerischeMethodenStatistischenPhysik)

find_package(OpenCV REQUIRED)

#find_package(GSL REQUIRED)
#link_libraries(GSL::gsl GSL::gslcblas)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -pthread")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-Ofast")
set(BUILD_ARCH "-m64")


#add all .cpp files from the Projects. CONFIGURE_DEPENDS not recommended but useful:
#https://cmake.org/cmake/help/latest/command/file.html#glob
file(GLOB HelloWorldCPPs CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/Projects/HelloWorld/*.cpp)
add_executable(HelloWorld ${HelloWorldCPPs})
target_link_libraries(HelloWorld ${ARMADILLO_LIBRARIES} ${OpenCV_LIBS} CvPlot)
set_target_properties(HelloWorld PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

file(GLOB RNGTestsCPPs CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/Projects/RngTests/*.cpp)
add_executable(RNGTests ${RNGTestsCPPs})
target_link_libraries(RNGTests)
set_target_properties(RNGTests PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

set(ISING_SOURCE_DIR ${PROJECT_SOURCE_DIR}/Projects/Ising)

file(GLOB isingCPPs CONFIGURE_DEPENDS ${ISING_SOURCE_DIR}/ising.cpp ${ISING_SOURCE_DIR}/SpinLattice2level.cpp ${ISING_SOURCE_DIR}/SpinLatticeTheta.cpp)
add_executable(ising ${isingCPPs})
target_link_libraries(ising ${OpenCV_LIBS} CvPlot stdc++fs)
set_target_properties(ising PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")


file(GLOB isingHeadlessCPPs CONFIGURE_DEPENDS ${ISING_SOURCE_DIR}/SpinLattice2level.cpp ${ISING_SOURCE_DIR}/isingHeadless.cpp)
add_executable(isingHeadless ${isingHeadlessCPPs} Projects/Ising/isingHeadless.cpp)
target_link_libraries(isingHeadless stdc++fs)
set_target_properties(isingHeadless PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
