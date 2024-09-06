# Set minimum CMake version
cmake_minimum_required(VERSION 3.15)
project(gr-lora LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
#causes python errors
#cmake_policy(SET CMP0148 NEW)
#cmake_policy(SET CMP0167 NEW)

# Enable testing
enable_testing()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

# Set the prefix path for finding GNU Radio (conda use assumed for now)
set(CMAKE_PREFIX_PATH $ENV{CONDA_PREFIX}/lib/cmake/gnuradio)
set(CMAKE_INSTALL_PREFIX $ENV{CONDA_PREFIX})
message(STATUS "CONDA PREFIX: ${CMAKE_INSTALL_PREFIX}")


find_package(Gnuradio 3.10 REQUIRED COMPONENTS  runtime  analog blocks digital filter fft)

if(Gnuradio_VERSION VERSION_LESS "3.8")
    find_package(Volk)
endif()

if(NOT Gnuradio_FOUND)
    message(FATAL_ERROR "GnuRadio Runtime required to compile")
endif()
if (Gnuradio_FOUND)
    message(STATUS "GNU Radio found: ${Gnuradio_VERSION}")
endif()


find_package(Boost 1.65 REQUIRED COMPONENTS filesystem system unit_test_framework)

# Find CppUnit library (conda assumed)
find_library(CPPUNIT_LIB cppunit HINTS $ENV{CONDA_PREFIX}/lib)

include(GrPlatform) #define LIB_SUFFIX
set(GR_RUNTIME_DIR      ${CMAKE_INSTALL_PREFIX}/bin)
set(GR_LIBRARY_DIR      ${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX})
set(GR_INCLUDE_DIR      ${CMAKE_INSTALL_PREFIX}/include/lora)
set(GR_DATA_DIR         ${CMAKE_INSTALL_PREFIX}/share)
set(GR_PKG_DATA_DIR     ${GR_DATA_DIR}/${CMAKE_PROJECT_NAME})
set(GR_DOC_DIR          ${GR_DATA_DIR}/doc)
set(GR_PKG_DOC_DIR      ${GR_DOC_DIR}/${CMAKE_PROJECT_NAME})
set(GR_CONF_DIR         ${CMAKE_INSTALL_PREFIX}/etc)
set(GR_PKG_CONF_DIR     ${GR_CONF_DIR}/${CMAKE_PROJECT_NAME}/conf.d)
set(GR_LIBEXEC_DIR      ${CMAKE_INSTALL_PREFIX}/libexec)
set(GR_PKG_LIBEXEC_DIR  ${GR_LIBEXEC_DIR}/${CMAKE_PROJECT_NAME})
set(GRC_BLOCKS_DIR      ${GR_PKG_DATA_DIR}/grc/blocks)

message(STATUS "blocks dir:  ${GR_PKG_DATA_DIR}")

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

install(FILES cmake/Modules/loraConfig.cmake DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/cmake/lora)

# Uninstall target
configure_file(
    ${CMAKE_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in
    ${CMAKE_BINARY_DIR}/cmake_uninstall.cmake
    @ONLY
)
add_custom_target(uninstall
    ${CMAKE_COMMAND} -P ${CMAKE_BINARY_DIR}/cmake_uninstall.cmake
)
