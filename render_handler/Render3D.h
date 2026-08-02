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
    };
}
