#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdexcept>
#include <vector>
#include <iostream>
#include <string.h>


#include "QueueHandler.h"
//this is intended to provide a basic vulkan setup
//designed to be easily and significantly abstracted from to make programing easy while still providing basic rendering setup
//TODO any way of making the queues less set in stone as they currently are
//maybe another class to handle them specifically?
//TODO make it so the number of windows in this is not set at the highest superclass, as it currently is


//TODO separate some of these clunky functions and simplify functionality to a reasonable degree
namespace ferium
{

    class RenderHandler
    {
    public:
        VkResult start(GLFWwindow* window,const char* windowName, const char* engineName);

        //a funny name for a destructor that I thought would fit alongside the "start" function :)
        void end();
    protected:
        virtual void initialize() = 0;//TODO probably remove this
        virtual const char** getVulkanExtensions(uint32_t* extensionCount,const char** glfwExtensions) = 0;
        virtual const char** getValidationLayers(uint32_t* layerCount,const char **validationLayersInitial) = 0;
        VkInstance m_instance;
        VkDebugUtilsMessengerEXT m_debugMessenger;
        VkPhysicalDevice m_physicalDevice;
        VkDevice m_device;//logical device
        //appends the given data to the const char ** array and increases *size by one
        //returns the modified list
        const char** append(const char** array,uint32_t* size, const char* data);
        VkQueue m_graphicsQueue;
        VkSurfaceKHR m_surface;
        VkQueue m_presentQueue;
        QueueHandler m_queueHamburger;
    private:
        //gets required queue families and initializes the QueueHandler
        void findQueueFamilies(VkPhysicalDevice device);
        //creates the instance
        void createInstance(const char* windowName,const char* engineName);
        //a simple function to verify that all needed extensions are found
        const bool findExtensions(const std::vector<VkExtensionProperties>* extensionsAvailable,const char** extensionsNeeded,const uint32_t extensionsNeededCount,const char** notFoundName);
        const bool findValidationLayers(const std::vector<VkLayerProperties>* layersAvailable,const char** layersNeeded,const uint32_t layersNeededCount,const char** notFoundName);
        //will throw an error if requested layers are unavaliable
        void initValidationLayers(VkInstanceCreateInfo* createInfo,VkDebugUtilsMessengerCreateInfoEXT* debugCreateInfo);
        //to not have practicly identical code formated differently
        void initExtensions(VkInstanceCreateInfo* createInfo);


        //for physical device selection
        void pickPhysicalDevice();
        bool isDeviceSuitable(VkPhysicalDevice device);
        //vulkan debug debugCallback
        //from vulkan tutorial
        static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);

        void setupDebugMessenger();
        void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT* createInfo);
        static VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
        void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);
        void createLogicalDevice();

    };
}
