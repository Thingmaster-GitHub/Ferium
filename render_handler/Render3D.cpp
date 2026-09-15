#include "Render3D.h"

using namespace ferium;

void Render3D::initialize()
{

}
const char** Render3D::getVulkanExtensions(uint32_t* extensionCount,const char** glfwExtensions)
{
    const char** output = append(glfwExtensions,extensionCount,"VK_KHR_get_surface_capabilities2");
    return output;
}
const char** Render3D::getValidationLayers(uint32_t * layerCount, const char ** validationLayersInitial)
{
    //comented out as I discovered my laptop does not support this validation layer
    //const char** output = append(validationLayersInitial, layerCount,"VK_LAYER_MESA_overlay");

    //return output;
    //this function does not currently do anything
    return validationLayersInitial;
}
