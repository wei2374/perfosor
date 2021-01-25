set(MY_LIB_SOURCES
    src/cssl.c
    src/helper.c 
    src/datex_lib.c)

add_library(my_library STATIC ${MY_LIB_SOURCES})
add_library(my_vector INTERFACE)

target_sources(my_vector INTERFACE 
	${CMAKE_CURRENT_SOURCE_DIR}/include/cvector.h
)
target_include_directories(my_library PRIVATE include/)
