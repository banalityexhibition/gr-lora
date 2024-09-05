# Set minimum CMake version
cmake_minimum_required(VERSION 3.15)
project(gr-lora LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
#causes python errors
#cmake_policy(SET CMP0148 NEW)
cmake_policy(SET CMP0167 NEW)

# Enable testing
enable_testing()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

# Set the prefix path for finding GNU Radio (conda use assumed for now)
set(CMAKE_PREFIX_PATH $ENV{CONDA_PREFIX}/lib/cmake/gnuradio)


find_package(Gnuradio 3.10 REQUIRED COMPONENTS runtime analog blocks digital filter fft)

if(Gnuradio_VERSION VERSION_LESS "3.8")
    find_package(Volk)
endif()

if(NOT Gnuradio_FOUND)
    message(FATAL_ERROR "GnuRadio Runtime required to compile")
endif()
if (Gnuradio_FOUND)
    message(STATUS "GNU Radio found: ${Gnuradio_VERSION}")
endif()


find_package(Boost 1.65 REQUIRED COMPONENTS filesystem system)

# Find CppUnit library (conda assumed)
find_library(CPPUNIT_LIB cppunit HINTS ${CONDA_PREFIX}/lib)



add_subdirectory(lib)  

# Subdirectories for additional components
add_subdirectory(include/lora)
add_subdirectory(swig)
add_subdirectory(python)
add_subdirectory(grc)
add_subdirectory(apps)
add_subdirectory(docs)

target_link_libraries(test-lora
    gnuradio-lora
    ${CPPUNIT_LIB}
)

install(FILES cmake/Modules/loraConfig.cmake DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/lora)

# Uninstall target
configure_file(
    ${CMAKE_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in
    ${CMAKE_BINARY_DIR}/cmake_uninstall.cmake
    @ONLY
)
add_custom_target(uninstall
    ${CMAKE_COMMAND} -P ${CMAKE_BINARY_DIR}/cmake_uninstall.cmake
)
