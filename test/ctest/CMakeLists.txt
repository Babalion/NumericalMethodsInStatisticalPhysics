cmake_minimum_required(VERSION 3.17 FATAL_ERROR)

find_package(Armadillo REQUIRED)
include_directories(${ARMADILLO_INCLUDE_DIRS})
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set(BUILD_ARCH "-m64")

#-----------------------------------------------------------------------------------------------------------
#-------------------------MATPLOT-CPP-----------------------------------------------------------------------
#-----------------------------------------------------------------------------------------------------------
#include(GNUInstallDirs)
#set(PACKAGE_NAME matplotlib_cpp)
#set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/${PACKAGE_NAME}/cmake)


# Library target

#install(
#        TARGETS matplotlib_cpp
#        EXPORT install_targets
#)
#-----------------------------------------------------------------------------------------------------------
#-----------------------------------------------------------------------------------------------------------

set(ProjectsPath ../../Projects)


# this macro generates the commented out code in Hello-World-section
macro(ADD_SIMPLE_TESTS projectName)
    add_executable(ctest_exe_test${projectName} test${projectName}.cpp ${ProjectsPath}/${projectName}/${projectName}.cpp)
    add_test(ctest_test${projectName} ctest_exe_test${projectName})
endmacro()
#-----------------------------------------------------------------------------------------------------------------------
#-------------TEST-OF: HelloWorld---------------------------------------------------------------------------------------
#-----------------------------------------------------------------------------------------------------------------------

#add_executable(ctest_exe_testHelloWorld testHelloWorld.cpp ${ProjectsPath}/HelloWorld/HelloWorld.cpp)
#add_test(ctest_testHelloWorld ctest_exe_testHelloWorld)
ADD_SIMPLE_TESTS(HelloWorld)
#-----------------------------------------------------------------------------------------------------------------------
#-------------TEST-OF: RngTests-----------------------------------------------------------------------------------------
#-----------------------------------------------------------------------------------------------------------------------
ADD_SIMPLE_TESTS(RngTests)
#-----------------------------------------------------------------------------------------------------------------------
#-------------TEST-OF: Sandpiles - Configuration------------------------------------------------------------------------
#-----------------------------------------------------------------------------------------------------------------------
add_executable(ctest_exe_testConfiguration testConfiguration.cpp ${ProjectsPath}/SandPiles/Cell.cpp ${ProjectsPath}/SandPiles/Slope_Cell.cpp ${ProjectsPath}/SandPiles/Configuration.cpp ${ProjectsPath}/SandPiles/RNG_MT19937.cpp)
add_test(ctest_exe_testConfiguration ctest_exe_testConfiguration)
target_link_libraries(ctest_exe_testConfiguration ${ARMADILLO_LIBRARIES})
set_target_properties(ctest_exe_testConfiguration PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")