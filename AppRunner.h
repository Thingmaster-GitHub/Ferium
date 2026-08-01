#pragma once

#include "window_handler/WindowHandler.h"
#include <GLFW/glfw3.h>
#include <iostream>
//a basic thing intended to run any type of rendering application
//handles vulkan things
namespace ferium
{
    class AppRunner
    {
    public:
        //initializes an AppRunner
        //please ignore my non-descriptive comments, I'll probably add detail later
        AppRunner(WindowHandler* window) : m_window(window){}

        //runs the program (wow! :3)
        void run(std::string name);

    private:
        WindowHandler* m_window;

    };
}
