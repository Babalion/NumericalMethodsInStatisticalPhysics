cmake_minimum_required(VERSION 3.17 FATAL_ERROR)

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