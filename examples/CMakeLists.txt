
add_executable(oesu_example oesu_example.cpp)
target_link_libraries(oesu_example PRIVATE oesenc)

add_executable(oesenc_example oesenc_example.cpp)
target_link_libraries(oesenc_example PRIVATE oesenc)
