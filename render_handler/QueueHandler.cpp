#include "QueueHandler.h"

using namespace ferium;

QueueHandler::QueueHandler(VkQueueFamilyProperties* queueFamilies,uint32_t queueFamilyCount)
{
    m_queueFamilies=queueFamilies;
    m_queueFamilyCount=queueFamilyCount;
    m_requirements = std::vector<queueIndiciesContainer>();
}

bool QueueHandler::foundQueues()
{
    bool finished=true;

    for(int i=0;i<m_requirements.size();i++)
    {
        finished = (m_requirements.at(i).needed&&!m_requirements.at(i).index)   ||  !m_requirements.at(i).needed;
    }

    return finished;
}
void QueueHandler::addRequirement(uint32_t* indicies,uint32_t size,const char* name, bool needed)
{
    queueIndiciesContainer tmp;

    tmp.needed=needed;
    tmp.hasBitFlag=false;

    tmp.name=name;
    tmp.size=&size;
    tmp.unflagedIndicies=indicies;

    m_requirements.push_back(tmp);
};
void QueueHandler::addRequirement(VkQueueFlagBits bits,const char* name, bool needed)
{
    queueIndiciesContainer tmp;
    tmp.needed=needed;
    tmp.hasBitFlag=true;
    tmp.bitFlag=bits;
    tmp.name=name;

    m_requirements.push_back(tmp);
}
void QueueHandler::calculateOptimal()
{

}
