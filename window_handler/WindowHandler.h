//handles windows and inputs

#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <iostream>

#include "../render_handler/RenderHandler.h"

namespace ferium
{
    class WindowHandler
    {
    public:

        WindowHandler(RenderHandler* renderer) : m_renderer(renderer){}

        bool open(const char* windowName, std::string engineName);

    private:

        static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
        RenderHandler* m_renderer;
        GLFWwindow* m_window;
    };
}
