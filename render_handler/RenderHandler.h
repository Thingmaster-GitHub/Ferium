#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdexcept>
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


        VkInstance m_instance;

    private:
        void createInstance(const char* windowName,const char* engineName);
    };
}
