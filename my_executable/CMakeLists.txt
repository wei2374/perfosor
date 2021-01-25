set(MY_EXE_RES
    src/serial.c)

add_executable(my_executable ${MY_EXE_RES})
target_include_directories(my_executable PRIVATE ../my_library/include/)
target_link_libraries(my_executable PRIVATE my_library my_vector)