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
    uint32_t extensionCount = 0;
    const char** extensions;


    extensions = getVulkanExtensions(&extensionCount,glfwGetRequiredInstanceExtensions(&extensionCount));

    //testing
    uint32_t extensionCountAvailable = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCountAvailable, nullptr);

    std::vector<VkExtensionProperties> extensionsAvailable(extensionCountAvailable);

    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCountAvailable, extensionsAvailable.data());

    {
        const char* notFoundName = nullptr;

        if(!findExtensions(&extensionsAvailable,extensions,extensionCount,&notFoundName))
        {
            throw std::runtime_error(std::string("failed to find vulkan extension \"") + notFoundName +"\"!");
        }

    }
    createInfo.enabledExtensionCount = extensionCount;
    createInfo.ppEnabledExtensionNames = extensions;

    createInfo.enabledLayerCount = 0;

    //TODO check if I need to add anything here for macos/osx compatability
    if (vkCreateInstance(&createInfo, nullptr, &m_instance) != VK_SUCCESS) {
        throw std::runtime_error("failed to create instance!");
    }
}
const bool RenderHandler::findExtensions(const std::vector< VkExtensionProperties >* extensionsAvailable, const char** extensionsNeeded, const uint32_t extensionsNeededCount, const char** notFoundName)
{

    for(int i=0;i<extensionsNeededCount;i++)
    {
        bool foundCurrent = false;
        *notFoundName=extensionsNeeded[i];
        for(int j=0;j<extensionsAvailable->size();j++)
        {
            foundCurrent=foundCurrent||(strcmp(extensionsAvailable->at(j).extensionName,*notFoundName)==0);
        }

        if(!foundCurrent)
            return false;
    }

    return true;
}
