#include "WindowHandler.h"


using namespace ferium;

bool WindowHandler::open(const char* windowName, std::string engineName)
{
    glfwInit();

    glfwInitVulkanLoader(vkGetInstanceProcAddr);

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    m_window = glfwCreateWindow(640, 480, windowName, NULL, NULL);//TODO make this reasonable
    glfwSetKeyCallback(m_window, key_callback);

    //now, to actually start vulkan

    while(!glfwWindowShouldClose(m_window))
    {

    }
}
void WindowHandler::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{

}
