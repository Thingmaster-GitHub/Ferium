#include "AppRunner.h"

using namespace ferium;

void AppRunner::run(const char* name)
{

    if (m_window->open(name,"Ferium") != VK_SUCCESS) {
        throw std::runtime_error("failed to create window!");
    }
}
