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


    //this will handle extensions
    checkExtensionSupport(&createInfo);
    //this will handle the validation layers
    checkValidationLayerSupport(&createInfo);

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
const bool RenderHandler::findValidationLayers(const std::vector< VkLayerProperties >* layersAvailable, const char** layersNeeded, const uint32_t layersNeededCount, const char** notFoundName)
{

    for(int i=0;i<layersNeededCount;i++)
    {
        bool foundCurrent = false;
        *notFoundName=layersNeeded[i];
        for(int j=0;j<layersAvailable->size();j++)
        {
            foundCurrent=foundCurrent||(strcmp(layersAvailable->at(j).layerName,*notFoundName)==0);
            std::cout<<layersAvailable->at(j).layerName<<"\n";
        }

        if(!foundCurrent)
            return false;
    }

    return true;
}
void RenderHandler::checkValidationLayerSupport(VkInstanceCreateInfo* createInfo)
{
#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif
    if(enableValidationLayers)
    {
        //temporarally defined here to allow for subclass specific behavior, as, unlike with extensions, glfw requests no validation layers
        const char** validationLayers = (const char**)malloc(1*sizeof(const char*));
        validationLayers[0]="VK_LAYER_KHRONOS_validation";
        uint32_t layerCount = 1;

        validationLayers = getValidationLayers(&layerCount,validationLayers);

        uint32_t availablelayerCount;


        vkEnumerateInstanceLayerProperties(&availablelayerCount, nullptr);
        std::vector<VkLayerProperties> availableLayers(availablelayerCount);
        vkEnumerateInstanceLayerProperties(&availablelayerCount, availableLayers.data());

        const char* notFoundName = nullptr;

        if(!findValidationLayers(&availableLayers,validationLayers,layerCount,&notFoundName))
        {
            throw std::runtime_error(std::string("failed to find validation layer \"") + notFoundName +"\"!");
        }

        createInfo->enabledLayerCount = layerCount;
        createInfo->ppEnabledLayerNames = validationLayers;
    }
    else
        createInfo->enabledLayerCount = 0;

}
void RenderHandler::checkExtensionSupport(VkInstanceCreateInfo* createInfo)
{
    //glfw things for vulkan
    uint32_t extensionCount = 0;
    const char** extensions;


    extensions = getVulkanExtensions(&extensionCount,glfwGetRequiredInstanceExtensions(&extensionCount));

#ifdef DEBUG
    extensions= append(extensions,extensionCount,"VK_EXT_debug_utils")
#endif
    uint32_t extensionCountAvailable = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCountAvailable, nullptr);

    std::vector<VkExtensionProperties> extensionsAvailable(extensionCountAvailable);

    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCountAvailable, extensionsAvailable.data());

    //defined here as to make using findExtensions simpler
    const char* notFoundName = nullptr;

    if(!findExtensions(&extensionsAvailable,extensions,extensionCount,&notFoundName))
    {
        throw std::runtime_error(std::string("failed to find vulkan extension \"") + notFoundName +"\"!");
    }

    createInfo->enabledExtensionCount = extensionCount;
    createInfo->ppEnabledExtensionNames = extensions;

}
const char** RenderHandler::append(const char** array,uint32_t* size, const char* data)
{
    //using this might be a bit inefficient, but it's only used for intialization so whatever
    uint32_t initialSize = *size;

    (*size)++;

    const char** output = (const char**)malloc((*size) * sizeof(const char*));

    for(int i=0;i<initialSize;i++)
    {
        output[i]=array[i];
    }

    output[initialSize]=data;
    return output;
}
