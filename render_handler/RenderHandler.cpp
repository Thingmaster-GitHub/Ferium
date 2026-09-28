#include "RenderHandler.h"

using namespace ferium;

VkResult RenderHandler::start(GLFWwindow* window,const char* windowName, const char* engineName)
{
    VkSurfaceKHR surface;

    createInstance(windowName,engineName);
    setupDebugMessenger();
    //initialize subclass specific behavior
    //might move this maybe
    initialize();
    pickPhysicalDevice();

    VkResult err = glfwCreateWindowSurface(m_instance, window, NULL, &surface);

    return err;
}
void RenderHandler::end()
{
#ifdef NDEBUG
#else

    DestroyDebugUtilsMessengerEXT(m_instance, m_debugMessenger, nullptr);
#endif
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

#ifdef NDEBUG
    createInfo.enabledLayerCount = 0;

    createInfo.pNext = nullptr;
#else
    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
    //this will handle the validation layers
    checkValidationLayerSupport(&createInfo,&debugCreateInfo);

#endif
    //this will handle extensions
    checkExtensionSupport(&createInfo);


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
        }

        if(!foundCurrent)
            return false;
    }

    return true;
}
void RenderHandler::checkValidationLayerSupport(VkInstanceCreateInfo* createInfo,VkDebugUtilsMessengerCreateInfoEXT* debugCreateInfo)
{
#ifdef NDEBUG

    createInfo->enabledLayerCount = 0;
#else
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

    populateDebugMessengerCreateInfo(debugCreateInfo);

    createInfo->enabledLayerCount = layerCount;
    createInfo->ppEnabledLayerNames = validationLayers;



    createInfo->pNext = debugCreateInfo;
#endif

}
void RenderHandler::checkExtensionSupport(VkInstanceCreateInfo* createInfo)
{
    //glfw things for vulkan
    uint32_t extensionCount = 0;
    const char** extensions;


    extensions = getVulkanExtensions(&extensionCount,glfwGetRequiredInstanceExtensions(&extensionCount));

#ifdef NDEBUG//this is the only one working :(
#else
    extensions= append(extensions,&extensionCount,"VK_EXT_debug_utils");
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
VKAPI_ATTR VkBool32 VKAPI_CALL RenderHandler::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
{
    std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

    return VK_FALSE;
}
void RenderHandler::setupDebugMessenger()
{
#ifdef NDEBUG
    return;
#endif
    VkDebugUtilsMessengerCreateInfoEXT createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debugCallback;
    createInfo.pUserData = nullptr; // Optional

    if (CreateDebugUtilsMessengerEXT(m_instance, &createInfo, nullptr, &m_debugMessenger) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to set up debug messenger!");
    }

}

VkResult RenderHandler::CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger)
{

    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr)
    {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    } else
    {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}
void RenderHandler::DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator)
{
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr)
    {
        func(instance, debugMessenger, pAllocator);//crashes here?
    }

}
void RenderHandler::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT* createInfo)
{
    createInfo->sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo->messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo->messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo->pfnUserCallback = debugCallback;
}
void RenderHandler::pickPhysicalDevice()
{
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(m_instance, &deviceCount, nullptr);
    VkPhysicalDevice* devices = (VkPhysicalDevice*)malloc((deviceCount) * sizeof(VkPhysicalDevice));
    vkEnumeratePhysicalDevices(m_instance, &deviceCount, devices);

    if (deviceCount == 0) {
        throw std::runtime_error("failed to find GPU with Vulkan support!");
    }
    m_physicalDevice=VK_NULL_HANDLE;

    for(int i=0;i<deviceCount&&m_physicalDevice == VK_NULL_HANDLE;i++)
    {
        if(isDeviceSuitable(devices[i]))
        {
            m_physicalDevice=devices[i];
        }
    }
    if (m_physicalDevice == VK_NULL_HANDLE) {
        throw std::runtime_error("failed to find a suitable GPU!");
    }
}
bool RenderHandler::isDeviceSuitable(VkPhysicalDevice device)
{
    //TODO add some reasonable stuff here, and probably some subclass specific behavior
    //all of my devices have only 1 gpu/igpu, so I have almost no way of testing this
    //for now, just having vulkan support should be enough
    return true;
}
