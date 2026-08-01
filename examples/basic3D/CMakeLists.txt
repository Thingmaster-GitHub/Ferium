
set(basic3D_SRCS
    main.cpp
)

add_executable(basic3D ${basic3D_SRCS})

target_link_libraries(basic3D
    ferium
)
