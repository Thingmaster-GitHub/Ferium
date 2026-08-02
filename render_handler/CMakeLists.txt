set(render_handler_SRCS
        RenderHandler.cpp
        Render3D.cpp
)

find_package (glfw3 REQUIRED)
find_package (Vulkan REQUIRED)

add_library(render_handler STATIC ${render_handler_SRCS})

target_link_libraries(render_handler
        PRIVATE
                glfw
                vulkan
)
