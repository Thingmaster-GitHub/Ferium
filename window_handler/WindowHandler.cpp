#include "WindowHandler.h"


using namespace ferium;

VkResult WindowHandler::open(const char* windowName, const char* engineName)
{
    glfwInit();

    glfwInitVulkanLoader(vkGetInstanceProcAddr);

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    m_window = glfwCreateWindow(640, 480, windowName, NULL, NULL);//TODO make this reasonable (get screen width & height or something)
    glfwSetKeyCallback(m_window, keyCallback);

    //now, to actually start vulkan
    VkResult err = m_renderer->start(m_window,windowName,engineName);

    //return if there was an error (wow!)
    if(err)
        return err;

    while(!glfwWindowShouldClose(m_window))
    {
        glfwPollEvents();

    }
    m_renderer->end();

    glfwDestroyWindow(m_window);

    glfwTerminate();
    //at this point, we can be certain there is no error, but to save making a new VkResult, just return the one that we have
    return err;

}
void WindowHandler::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{

}
