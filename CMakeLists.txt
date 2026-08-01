cmake_minimum_required(VERSION 3.16)


set(ferium_SRCS
    AppRunner.cpp
)


set(EXPORT_COMPILE_COMMANDS ON)

project(ferium LANGUAGES CXX)


find_package (glfw3 REQUIRED)
find_package (Vulkan REQUIRED)

add_subdirectory(examples/)
add_subdirectory(window_handler/)
add_subdirectory(render_handler/)


add_library(ferium STATIC ${ferium_SRCS})
install(TARGETS ferium RUNTIME DESTINATION bin)

target_link_libraries(ferium
#add created libraries here
    examples
    render_handler
    window_handler
)
