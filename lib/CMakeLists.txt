list(APPEND lora_sources
    demod_impl.cc
    decode_impl.cc
    mod_impl.cc
    encode_impl.cc
)

if(NOT lora_sources)
    message(STATUS "No C++ sources found... skipping lib/")
    return()
endif()

add_library(gnuradio-lora SHARED ${lora_sources})

target_compile_features(gnuradio-lora PUBLIC cxx_std_17)

# Include directories for the library
target_include_directories(gnuradio-lora PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${Boost_INCLUDE_DIRS}
    ${GNURADIO_RUNTIME_INCLUDE_DIRS}
    ${VOLK_INCLUDE_DIRS}
)

# Link required libraries (modern version)
target_link_libraries(gnuradio-lora PUBLIC
    Boost::filesystem
    Boost::system
    gnuradio::gnuradio-runtime
    gnuradio::gnuradio-blocks
    gnuradio::gnuradio-fft
    gnuradio::gnuradio-pmt
    volk
)

set_target_properties(gnuradio-lora PROPERTIES
    DEFINE_SYMBOL "gnuradio_lora_EXPORTS"
)

# Special handling for Apple systems
if(APPLE)
    set_target_properties(gnuradio-lora PROPERTIES
        INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib"
    )
endif()

# Install the built library files
install(TARGETS gnuradio-lora
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

# Unit testing with CppUnit
include(GrTest)

# Add CppUnit test source files
list(APPEND test_lora_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/test_lora.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/qa_lora.cc
)


add_executable(test-lora ${test_lora_sources})

target_include_directories(test-lora PRIVATE
    ${CPPUNIT_INCLUDE_DIR}
)


target_link_libraries(test-lora
    gnuradio-lora
    Boost::unit_test_framework
    ${CPPUNIT_LIBRARIES}
)

GR_ADD_TEST(test-lora test-lora)
