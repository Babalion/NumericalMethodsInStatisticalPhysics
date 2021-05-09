cmake_minimum_required(VERSION 3.17)
project(NumerischeMethodenStatistischenPhysik)

find_package(Armadillo REQUIRED)
include_directories(${ARMADILLO_INCLUDE_DIRS})
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set(BUILD_ARCH "-m64")

#-----------------------------------------------------------------------------------------------------------
#-------------------------MATPLOT-CPP-----------------------------------------------------------------------
#-----------------------------------------------------------------------------------------------------------
include(GNUInstallDirs)
set(PACKAGE_NAME matplotlib_cpp)
set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/${PACKAGE_NAME}/cmake)


# Library target
add_library(matplotlib_cpp INTERFACE)
target_include_directories(matplotlib_cpp
        INTERFACE
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/examples>
        $<INSTALL_INTERFACE:include>
        )
target_compile_features(matplotlib_cpp INTERFACE
        cxx_std_11
        )
# TODO: Use `Development.Embed` component when requiring cmake >= 3.18
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
target_link_libraries(matplotlib_cpp INTERFACE
        Python3::Python
        Python3::Module
        )
find_package(Python3 COMPONENTS NumPy)
if(Python3_NumPy_FOUND)
    target_link_libraries(matplotlib_cpp INTERFACE
            Python3::NumPy
            )
else()
    target_compile_definitions(matplotlib_cpp INTERFACE WITHOUT_NUMPY)
endif()
install(
        TARGETS matplotlib_cpp
        EXPORT install_targets
)
#-----------------------------------------------------------------------------------------------------------
#-----------------------------------------------------------------------------------------------------------

#add all .cpp files from the Projects. CONFIGURE_DEPENDS not recommended but useful:
#https://cmake.org/cmake/help/latest/command/file.html#glob
file(GLOB projects_SRC CONFIGURE_DEPENDS
        ${PROJECT_SOURCE_DIR}/main.cpp
        ${PROJECT_SOURCE_DIR}/Projects/*/*.cpp)


add_executable(NumerischeMethodenStatistischenPhysik ${projects_SRC})
target_link_libraries(NumerischeMethodenStatistischenPhysik ${ARMADILLO_LIBRARIES} matplotlib_cpp)
set_target_properties(NumerischeMethodenStatistischenPhysik PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

set(SandpilesPath "${PROJECT_SOURCE_DIR}/Projects/SandPiles")

add_executable(SandPiles ${SandpilesPath}/SandPiles.cpp ${SandpilesPath}/RNG_MT19937.cpp ${SandpilesPath}/Configuration.cpp ${SandpilesPath}//Cell.cpp)
target_link_libraries(SandPiles ${ARMADILLO_LIBRARIES} matplotlib_cpp)
set_target_properties(SandPiles PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")


enable_testing()
add_subdirectory(test/ctest)