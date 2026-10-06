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
        finished = (m_requirements.at(i).needed&&m_requirements.at(i).index)   ||  !m_requirements.at(i).needed;//broken?
    }

    return finished;
}
void QueueHandler::addRequirement(uint32_t* indices,uint32_t size,const char* name, bool needed)
{
    queueIndiciesContainer tmp;

    tmp.index=nullptr;
    tmp.needed=needed;
    tmp.hasBitFlag=false;

    tmp.name=name;
    tmp.size=&size;
    tmp.unflagedIndices=indices;

    m_requirements.push_back(tmp);
};
void QueueHandler::addRequirement(VkQueueFlagBits bits,const char* name, bool needed)
{
    queueIndiciesContainer tmp;
    tmp.index=nullptr;
    tmp.needed=needed;
    tmp.hasBitFlag=true;
    tmp.bitFlag=bits;
    tmp.name=name;

    m_requirements.push_back(tmp);
}
void QueueHandler::calculateOptimal(uint32_t effort)
{
    //annoyingly, I have no way of testing this
    //this garbage laptop only has 1 queue family :(

    //TODO figure out how to test this!
    uint32_t* indicesTally = new uint32_t[m_queueFamilyCount]();

    for(uint32_t count=0;count<effort;count++)//not using i, as this is not used as an index
    {
        for(auto& requirement:m_requirements)
        {
            //unfortunatly I do need the index of this for some things :(
            for(uint32_t i=0;i<m_queueFamilyCount;i++)//i - queue family index
            {
                //if the index is not defined and it the queue family contains it
                if(contains(requirement,i))
                {
                    if(!requirement.index)
                    {
                        requirement.index=&i;
                        indicesTally[i]++;
                    }
                    else if(indicesTally[*requirement.index]-1<indicesTally[i])
                    {
                        indicesTally[*requirement.index]--;

                        requirement.index=&i;

                        indicesTally[i]++;
                    }
                }
            }
        }
    }
}
bool QueueHandler::contains(queueIndiciesContainer requirement,uint32_t queueFamilyIndex)
{

    if(requirement.hasBitFlag)
    {
        return m_queueFamilies[queueFamilyIndex].queueFlags& requirement.bitFlag;
    }
    else
    {
        for(int i=0;i<*requirement.size;i++)
        {
            if(requirement.unflagedIndices[i]==queueFamilyIndex)
                return true;
        }
        return false;
    }
}

