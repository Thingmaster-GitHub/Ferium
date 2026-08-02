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

        //TODO stop being so stingy with comments for functions, lest I forget what they do
        //creates the WindowHandler
        WindowHandler(RenderHandler* renderer) : m_renderer(renderer){}

        //opens the window
        VkResult open(const char* windowName,const char* engineName);

    private:

        //thingy for glfw for, as you may guess, input handling
        //I've overused the word "handling" in this project, haven't I?
        static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        RenderHandler* m_renderer;
        GLFWwindow* m_window;
    };
}
