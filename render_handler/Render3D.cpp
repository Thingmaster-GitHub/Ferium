#include "Render3D.h"

using namespace ferium;

void Render3D::initialize()
{

}
const char** Render3D::getVulkanExtensions(uint32_t* extensionCount,const char** glfwExtensions)
{
    //at this point, you could search for whatever the wanted extensions are
    uint32_t glfwCount = *extensionCount;
    (*extensionCount)++;//I don't have a thing to add yet

    //behold, an overcomplicated bit of code so I didn't have to use std::vector
    const char** output = (const char**)malloc((*extensionCount) * sizeof(const char*));

    for(int i=0;i<glfwCount;i++)
    {
        output[i]=glfwExtensions[i];
    }

    //this is a sort of stand in, as not many extensions are needed or need to be specified here.
    output[glfwCount]="VK_KHR_get_surface_capabilities2";
    return output;
}
