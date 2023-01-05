find_package(GTest CONFIG REQUIRED)
add_executable(chartfile_test chartfile_test.cpp)

enable_testing()

target_link_libraries(chartfile_test
    PRIVATE
        GTest::gtest
        GTest::gtest_main
        oesenc
)

add_test(NAME chartfile_test COMMAND chartfile_test)
