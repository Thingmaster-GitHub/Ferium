#include "RenderHandler.h"

using namespace ferium;

VkResult RenderHandler::start(GLFWwindow* window,const char* windowName, const char* engineName)
{
    VkSurfaceKHR surface;

    createInstance(windowName,engineName);
    //initialize subclass specific behavior
    //might move this maybe
    initialize();

    VkResult err = glfwCreateWindowSurface(m_instance, window, NULL, &surface);

    return err;
}
void RenderHandler::end()
{
    vkDestroyInstance(m_instance, nullptr);

}
void RenderHandler::createInstance(const char* windowName,const char* engineName)
{
    //most of this section is copied from here: https://vulkan-tutorial.com
    //it's under a creative commons license, but I figure I might as well link to it anyway

    //look, non-hardcoded values! (mostly)
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = windowName;
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = engineName;
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    //a lot of structs in vulkan
    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    //glfw things for vulkan
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;

    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;

    createInfo.enabledLayerCount = 0;

    if (vkCreateInstance(&createInfo, nullptr, &m_instance) != VK_SUCCESS) {
        throw std::runtime_error("failed to create instance!");
    }
}
