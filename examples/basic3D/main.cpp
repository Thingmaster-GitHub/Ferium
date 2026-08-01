#include <iostream>
#include "../../AppRunner.h"
#include "../../render_handler/Render3D.h"
using namespace ferium;

//this basic example of this api creates a basic rotating triangle
//this is TODO so please check back later or something
int main(int argc, char *argv[])
{

    //the renderer must be initialized to specify what type of rendering is to be done
    Render3D renderer = Render3D();
    WindowHandler win = WindowHandler(&renderer);
    AppRunner runner = AppRunner(&win);

    runner.run("basic 3D example");

    return 0;
}

