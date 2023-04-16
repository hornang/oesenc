cmake_minimum_required(VERSION 3.15)

project(oesenc LANGUAGES CXX)

include(CTest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(oesenc
    include/oesenc/chartfile.h
    include/oesenc/position.h
    include/oesenc/rect.h
    include/oesenc/s57.h
    src/chartfile.cpp
    src/filereader.h
    src/filereader.cpp
    src/opencpn.cpp
    src/position.cpp
    src/vectorreader.h
    src/vectorreader.cpp
    src/reader.h
    src/rect.cpp
    src/s57.cpp
)

find_package(tinyxml2)

if (tinyxml2_FOUND)
    set(OESENC_KEYLISTREADER_ENABLED TRUE)
    target_sources(oesenc PRIVATE
        src/keylistreader.cpp
        include/oesenc/keylistreader.h
    )

    target_link_libraries(oesenc PRIVATE tinyxml2::tinyxml2)
else()
    set(OESENC_KEYLISTREADER_ENABLED FALSE)
    message(WARNING "tinyxml2 not found so not building keylistreader.cpp")
endif()

include(GenerateExportHeader)
generate_export_header(oesenc)

target_include_directories(oesenc PUBLIC include)
target_include_directories(oesenc PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

if (BUILD_TESTING)
    add_subdirectory(tests)
endif()
