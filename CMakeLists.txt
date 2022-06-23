cmake_minimum_required(VERSION 3.13...3.15)

project(span LANGUAGES CXX VERSION 0.9.0)

if(PROJECT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(SPAN_MASTER_PROJECT ON)
endif()

include(GNUInstallDirs)


add_library(span INTERFACE)
add_library(tcb::span ALIAS span)
#XXX target_sources(span INTERFACE include/tcb/span.hpp)
target_include_directories(span INTERFACE
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)
target_compile_features(span INTERFACE cxx_std_11)

set(TCB_SPAN_TEST_CXX_STD 11 CACHE STRING "C++ standard version for testing")

option(BUILD_TESTS "Build tests" ${SPAN_MASTER_PROJECT})
option(SPAN_INSTALL "Generate the install target" ${SPAN_MASTER_PROJECT})


if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()


if(SPAN_INSTALL)
    include(CMakePackageConfigHelpers)

    install(TARGETS span EXPORT ${PROJECT_NAME}Targets)
    install(DIRECTORY include/tcb DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
    set(configFile ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake)
    set(versionFile ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake)
    set(configInstallDestination lib/cmake/${PROJECT_NAME})

    configure_package_config_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
        ${configFile}
        INSTALL_DESTINATION ${configInstallDestination}
    )
    write_basic_package_version_file(
        ${versionFile}
        COMPATIBILITY SameMajorVersion
    )

    install(FILES ${configFile} ${versionFile} DESTINATION ${configInstallDestination})
    install(
        EXPORT ${PROJECT_NAME}Targets
        NAMESPACE "tcb::"
        DESTINATION ${configInstallDestination}
    )
endif()

