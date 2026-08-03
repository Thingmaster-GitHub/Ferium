#pragma once

#include "RenderHandler.h"

namespace ferium
{
    class Render3D : public RenderHandler
    {
    public:
        Render3D(){}

    protected:

        void initialize() override;

        const char** getVulkanExtensions(uint32_t* extensionCount,const char** glfwExtensions) override;
    };
}
