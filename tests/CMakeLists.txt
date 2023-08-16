find_package(GTest REQUIRED)

enable_testing()

function(addTest)
    set(BASENAME ${ARGV0}_test)
    add_executable(${BASENAME} ${BASENAME}.cpp)
    target_link_libraries(${BASENAME} PRIVATE GTest::gtest GTest::gtest_main oesenc)
    add_test(NAME ${BASENAME} COMMAND ${BASENAME})

    if(WIN32 AND BUILD_SHARED_LIBS)
        add_custom_command(TARGET ${BASENAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:${BASENAME}> $<TARGET_FILE_DIR:${BASENAME}>
            COMMAND_EXPAND_LISTS
        )
    endif()
endfunction()

addTest(chartfile)
addTest(servercontrol)

if(OESENC_KEYLISTREADER_ENABLED)
    addTest(keylistreader)
endif()
