cmake_minimum_required(VERSION 3.15)

project(oesenc LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
    add_compile_options(/WX)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

add_library(oesenc
    include/oesenc/area.h
    include/oesenc/chartfile.h
    include/oesenc/chartreader.h
    include/oesenc/deptharea.h
    include/oesenc/depth.h
    include/oesenc/line.h
    include/oesenc/polygon.h
    include/oesenc/position.h
    include/oesenc/rect.h
    include/oesenc/s57.h
    include/oesenc/sounding.h
    src/area.cpp
    src/chartfile.cpp
    src/deptharea.cpp
    src/depth.cpp
    src/line.cpp
    src/chartreader.cpp
    src/polygon.cpp
    src/position.cpp
    src/rect.cpp
    src/s57.cpp
    src/sounding.cpp
    src/opencpn.cpp
)

include(GenerateExportHeader)
generate_export_header(oesenc)

target_include_directories(oesenc PUBLIC include)
target_include_directories(oesenc PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
