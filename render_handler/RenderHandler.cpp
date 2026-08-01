#include "RenderHandler.h"

using namespace ferium;

bool RenderHandler::start(GLFWwindow* window)
{
    VkSurfaceKHR surface;

    //initialize subclass specific behavior
    initialize();

    VkResult err = glfwCreateWindowSurface(m_instance, window, NULL, &surface);

    //if err is true, there is an error, and starting has failed and we return false.
    return !err;
}
