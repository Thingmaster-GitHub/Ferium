#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace ferium
{
    class RenderHandler
    {
    public:
        bool start(GLFWwindow* window);

    protected:
        virtual VkInstance initialize();
        VkInstance m_instance;

    };
}
