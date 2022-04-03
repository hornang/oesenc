cmake_minimum_required(VERSION 3.15)

project(oesenc LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
    add_compile_options(/WX)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

add_library(oesenc
    include/oesenc/chartfile.h
    include/oesenc/polygon.h
    include/oesenc/position.h
    include/oesenc/rect.h
    include/oesenc/s57.h
    src/chartfile.cpp
    src/filereader.h
    src/filereader.cpp
    src/opencpn.cpp
    src/polygon.cpp
    src/position.cpp
    src/vectorreader.h
    src/vectorreader.cpp
    src/reader.h
    src/rect.cpp
    src/s57.cpp
)

find_package(tinyxml2)

if (tinyxml2_FOUND)
    target_sources(oesenc PRIVATE
        src/keylistreader.cpp
        include/oesenc/keylistreader.h
    )

    target_link_libraries(oesenc PRIVATE tinyxml2::tinyxml2)
else()
    message(WARNING "tinyxml2 not found so not building keylistreader.cpp")
endif()


include(GenerateExportHeader)
generate_export_header(oesenc)

target_include_directories(oesenc PUBLIC include)
target_include_directories(oesenc PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
