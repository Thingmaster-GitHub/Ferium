#pragma once


#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

//a small class to help with queue management
#include <iostream>
#include <vector>
namespace ferium
{
    struct queueIndiciesContainer
    {
        uint32_t* index;
        bool needed;//if the queue is needed
        bool hasBitFlag;
        VkQueueFlagBits bitFlag;

        const char* name;//plaintext name, allows for non-bit flag queue features to be searched for
        const uint32_t* size;
        const uint32_t* unflagedIndicies;
    };

    //this will just search for the index with the most requested feautures
    //maybe I'll add a weighting system eventually
    class QueueHandler
    {
    public:
        QueueHandler(){};

        QueueHandler(VkQueueFamilyProperties* queueFamilies,uint32_t queueFamilyCount);
        //adds a flaged queue reqirement, with, in this case, an optional name
        void addRequirement(VkQueueFlagBits bits,const char* name, bool needed);
        //adds an unflaged queue requrement, with a manditory field for a name
        void addRequirement(uint32_t* indicies,uint32_t size,const char* name, bool needed);
        //returns the index assigned to the queue with the givin bits
        uint32_t* getQueue(VkQueueFlagBits bits);
        //returns the index assigned to the queue with the givin name
        uint32_t* getQueue(const char* name);

        bool foundQueues();

        //calculates a set of indicies which follow all requirements with the minimal amount of indicies chosen
        void calculateOptimal();
        //TODO impliment binary search for unflaged indicies
    private:
        //stores all queue requirements and their found optimal indicies
        std::vector<queueIndiciesContainer> m_requirements;

        VkQueueFamilyProperties* m_queueFamilies;
        uint32_t m_queueFamilyCount;

    };
}
