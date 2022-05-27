cmake_minimum_required(VERSION 3.10..3.23)

get_directory_property(hasParent PARENT_DIRECTORY)
if(hasParent)
    set(PARENT_PROJECT_NAME "${PROJECT_NAME}")
else()
    set(PARENT_PROJECT_NAME "")
endif()

project(ns3helics VERSION 3.36.0 LANGUAGES CXX)

# recommended every project enable_testing
# ctest will work for everyone, forever; return 0 and pass by
# instead of user having to check if ctest is runnable
enable_testing()

set(CMAKE_CXX_STANDARD_MINIMUM 17)
if(NOT "${CMAKE_CXX_STANDARD}")
    set(CMAKE_CXX_STANDARD ${CMAKE_CXX_STANDARD_MINIMUM})
endif()
set(CXX_STANDARD_REQUIRED ON)

# -------------------------------------------------------------
# find required packages, ns3 and HELICS
# -------------------------------------------------------------
if(NOT PARENT_PROJECT_NAME STREQUAL "NS3")
    find_package(ns3 REQUIRED COMPONENTS libcore libinternet)
endif()

find_package(HELICS 2 REQUIRED)

# -------------------------------------------------------------
# setting the RPATH
# -------------------------------------------------------------
if(NOT DEFINED CMAKE_MACOSX_RPATH)
    set(CMAKE_MACOSX_RPATH ON)
endif()

# add the automatically determined parts of the RPATH which point to directories outside the build
# tree to the install RPATH
if(NOT DEFINED CMAKE_INSTALL_RPATH_USE_LINK_PATH)
    set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
endif()

# Add the local directory to the rpath
if(NOT APPLE)
    list(APPEND CMAKE_INSTALL_RPATH $ORIGIN)
    list(APPEND CMAKE_INSTALL_RPATH "\$ORIGIN/../${CMAKE_INSTALL_LIBDIR}")
else()
    list(APPEND CMAKE_INSTALL_RPATH "@loader_path")
    list(APPEND CMAKE_INSTALL_RPATH "@loader_path/../${CMAKE_INSTALL_LIBDIR}")
    list(APPEND CMAKE_INSTALL_RPATH "@executable_path")
    list(APPEND CMAKE_INSTALL_RPATH "@executable_path/../${CMAKE_INSTALL_LIBDIR}")
endif()

# the RPATH to be used when installing, but only if it's not a system directory
list(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES
     "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}" isSystemDir
)
if(isSystemDir STREQUAL "-1")
    list(APPEND CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}")
endif()

list(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES
     "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}" isSystemDir
)
if(isSystemDir STREQUAL "-1")
    list(APPEND CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")
endif()

# -------------------------------------------------------------
# module targets
# -------------------------------------------------------------
set(source_files
    src/ns3/helics/helics.cc
    src/ns3/helics/helics-application.cc
    src/ns3/helics/helics-filter-application.cc
    src/ns3/helics/helics-static-sink-application.cc
    src/ns3/helics/helics-static-source-application.cc
    src/ns3/helics/helics-simulator-impl.cc
    src/ns3/helics/helics-id-tag.cc
    src/ns3/helics/helics-helper.cc
)

set(header_files
    src/ns3/helics/helics.h
    src/ns3/helics/helics-application.h
    src/ns3/helics/helics-filter-application.h
    src/ns3/helics/helics-static-sink-application.h
    src/ns3/helics/helics-static-source-application.h
    src/ns3/helics/helics-simulator-impl.h
    src/ns3/helics/helics-id-tag.h
    src/ns3/helics/helics-helper.h
)

set(test_sources
    test/helics-test-suite.cc
)

add_library(ns3helics SHARED ${source_files} ${header_files})
set_property(TARGET ns3helics PROPERTY PUBLIC_HEADER ${header_files})
set_target_properties(ns3helics PROPERTIES
    VERSION "${PROJECT_VERSION}"
)
target_include_directories(ns3helics PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_link_libraries(ns3helics PUBLIC HELICS::helics-shared ns3::libcore ns3::libinternet)

add_library(ns3::helics::ns3helics ALIAS ns3helics)

option(NS3HELICS_BUILD_EXAMPLES "Build the HELICS-NS3 examples" ON)
if(NS3HELICS_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

include(GNUInstallDirs)
install(TARGETS ns3helics EXPORT ns3helicsTargets
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
            COMPONENT   ns3helics_Runtime
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
            COMPONENT   ns3helics_Runtime
#	    NAMELINK_COMPONENT Development (Requires CMake 3.12+)
    ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
            COMPONENT ns3helics_Development
    PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/ns3/helics"
            COMPONENT ns3helics_Development
)
install(EXPORT ns3helicsTargets NAMESPACE ns3::helics:: DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/ns3helics")
if(NOT PARENT_PROJECT_NAME STREQUAL "NS3")
    export(EXPORT ns3helicsTargets NAMESPACE ns3::helics::) # make the build tree targets importable by other projects
endif()

include(CMakePackageConfigHelpers)
configure_package_config_file(Config.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/ns3helicsConfig.cmake"
    INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/helics-ns3"
)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/ns3helicsConfigVersion.cmake"
    COMPATIBILITY ExactVersion
)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/ns3helicsConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/ns3helicsConfigVersion.cmake"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/ns3helics/"
)
