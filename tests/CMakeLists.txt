find_package(GTest REQUIRED)

enable_testing()

function(addTest)
    set(oneValueArgs NAME TIMEOUT)
    cmake_parse_arguments(ADD_TEST "" "${oneValueArgs}" "" ${ARGN})
    set(BASENAME ${ADD_TEST_NAME}_test)
    add_executable(${BASENAME} ${BASENAME}.cpp)
    target_link_libraries(${BASENAME} PRIVATE GTest::gtest GTest::gtest_main oesenc)
    add_test(NAME ${BASENAME} COMMAND ${BASENAME})

    if(ADD_TEST_TIMEOUT)
        set_tests_properties(${BASENAME} PROPERTIES TIMEOUT ${ADD_TEST_TIMEOUT})
    endif()

    if(WIN32 AND BUILD_SHARED_LIBS)
        add_custom_command(TARGET ${BASENAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:${BASENAME}> $<TARGET_FILE_DIR:${BASENAME}>
            COMMAND_EXPAND_LISTS
        )
    endif()
endfunction()

addTest(NAME chartfile)
addTest(NAME servercontrol TIMEOUT 30)

if(OESENC_KEYLISTREADER_ENABLED)
    addTest(NAME keylistreader)
endif()
