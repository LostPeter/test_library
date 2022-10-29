set(SOURCES
    malloc.c
    rpmalloc.c
    )

set(HEADERS
    rpnew.h
    rpmalloc.h
    )

add_library(rpmalloc STATIC ${SOURCES} ${HEADERS})
target_include_directories(rpmalloc PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})