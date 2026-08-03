#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdexcept>
#include <vector>
#include <iostream>
#include <string.h>
namespace ferium
{
    class RenderHandler
    {
    public:
        //TODO probably change this so it returns a vulkan error code if one is given
        VkResult start(GLFWwindow* window,const char* windowName, const char* engineName);

        //a funny name for a destrucor that I thought would fit alongside the "start" function :)
        void end();
    protected:
        virtual void initialize() = 0;//TODO probably remove this
        virtual const char** getVulkanExtensions(uint32_t* extensionCount,const char** glfwExtensions) = 0;

        VkInstance m_instance;

    private:
        void createInstance(const char* windowName,const char* engineName);
        //a simple function to verify that all needed extensions are found
        const bool findExtensions(const std::vector<VkExtensionProperties>* extensionsAvailable,const char** extensionsNeeded,const uint32_t extensionsNeededCount,const char** notFoundName);
    };
}
