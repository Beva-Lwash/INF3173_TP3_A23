add_library(core
    interceptor.c
    interceptor.h

    fdset.c
    fdset.h

    utils.c
    utils.h
)
target_link_libraries(core PRIVATE m)
target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(core PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/../include)

add_executable(interceptor main_interceptor.c)
target_link_libraries(interceptor PRIVATE core)

add_executable(slomo slomo.c)
