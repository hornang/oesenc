cmake_minimum_required(VERSION 3.15)

project(oesenc LANGUAGES CXX)

include(GNUInstallDirs)
include(CTest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(oesenc
    include/oesenc/chartfile.h
    include/oesenc/position.h
    include/oesenc/rect.h
    include/oesenc/servercontrol.h
    include/oesenc/serverreader.h
    include/oesenc/s57.h
    src/chartfile.cpp
    src/opencpn.cpp
    src/position.cpp
    src/random.cpp
    src/random.h
    src/rect.cpp
    src/servercontrol.cpp
    src/serverreader.cpp
    src/s57.cpp
)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  target_compile_definitions(oesenc PUBLIC "OESENC_LINUX")
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  target_compile_definitions(oesenc PUBLIC "OESENC_WINDOWS")
endif()

find_package(tinyxml2 QUIET)

if(tinyxml2_FOUND)
    target_link_libraries(oesenc PRIVATE tinyxml2::tinyxml2)
else()
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(tinyxml2 REQUIRED tinyxml2)
    pkg_get_variable(TINYXML_LIB ${tinyxml2_MODULE_NAME} libs)
    target_link_libraries(oesenc PRIVATE ${tinyxml2_LINK_LIBRARIES})
endif()

if(tinyxml2_FOUND)
    set(OESENC_KEYLISTREADER_ENABLED TRUE)
    target_sources(oesenc PRIVATE
        src/keylistreader.cpp
        include/oesenc/keylistreader.h
    )
else()
    set(OESENC_KEYLISTREADER_ENABLED FALSE)
    message(WARNING "tinyxml2 not found so not building keylistreader.cpp")
endif()

include(GenerateExportHeader)
generate_export_header(oesenc)

target_include_directories(oesenc PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<INSTALL_INTERFACE:include>
)

if(OESENC_KEYLISTREADER_ENABLED)
    add_subdirectory(examples)
endif()

if (BUILD_TESTING)
    add_subdirectory(tests)
endif()

include(CMakePackageConfigHelpers)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/OesencConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Oesenc
)

install(TARGETS oesenc
    EXPORT OesencTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
    FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/include/oesenc/chartfile.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/oesenc/chartfile.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/oesenc/opencpn.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/oesenc/position.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/oesenc/rect.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/oesenc/s57.h
        ${CMAKE_CURRENT_BINARY_DIR}/oesenc_export.h
    DESTINATION
        ${CMAKE_INSTALL_INCLUDEDIR}/oesenc
)

if(OESENC_KEYLISTREADER_ENABLED)
    install(
        FILES
            ${CMAKE_CURRENT_SOURCE_DIR}/include/oesenc/keylistreader.h
        DESTINATION
            ${CMAKE_INSTALL_INCLUDEDIR}/oesenc
    )
endif()

install(
    EXPORT OesencTargets
    FILE OesencTargets.cmake
    NAMESPACE Oesenc::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/oesenc
)

install(
    FILES
        ${CMAKE_CURRENT_BINARY_DIR}/OesencConfig.cmake
    DESTINATION
        ${CMAKE_INSTALL_LIBDIR}/cmake/oesenc
)
