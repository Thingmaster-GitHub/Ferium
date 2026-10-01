#pragma once


#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

//a small class to help with queue management
#include <iostream>
namespace ferium
{
    struct queueIndiciesContainer
    {
        uint32_t* index;
        bool needed;//if the queue is needed
        bool hasBitFlag;

        const char* name;//plaintext name, allows for non-bit flag queue features to be searched for
        const uint32_t size;
        const uint32_t* unflagedIndicies;
    };

    //this will just search for the index with the most requested feautures
    //maybe I'll add a weighting system eventually
    class QueueHandler
    {
    public:
        QueueHandler();
        void addRequirement();

        //returns the index assigned to the queue with the givin bits
        uint32_t* getQueue(VkQueueFlagBits bits);
        //returns the index assigned to the queue with the givin name
        uint32_t* getQueue(const char* name);

        bool foundQueues();

    private:
        void calculateOptimal();

    };
}
