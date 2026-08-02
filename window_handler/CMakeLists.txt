set(window_handler_SRCS
        WindowHandler.cpp
)

find_package (glfw3 REQUIRED)
find_package (Vulkan REQUIRED)

add_library(window_handler STATIC ${window_handler_SRCS})

target_link_libraries(window_handler
        PRIVATE
                glfw
                vulkan
                render_handler
)
