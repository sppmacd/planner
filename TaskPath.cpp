#include "TaskPath.h"

#include <string>
#include <vector>

#include <cli/utils.h>

TaskPath::TaskPath(std::string pList, GroupType pGroup)
    : m_taskList(pList)
    , m_groupType(pGroup)
    , m_valid(true)
{
}
TaskPath::TaskPath(std::string path)
    : m_valid(false)
{
    m_valid = parse(path);
}
std::string TaskPath::toString() const
{
    return "/" + m_taskList + "/" + m_groupType.getName();
}
bool TaskPath::parse(std::string path)
{
    std::vector<std::string> components;
    int lastSlash = -1;
    path += "/\0";
    for(size_t s = 0; s < path.size(); s++)
    {
        char c = path[s];
        if(c == '/')
        {
            std::string comp = path.substr(lastSlash + 1, s - lastSlash - 1);
            if(!comp.empty())
                components.push_back(comp);
            lastSlash = s;
        }
    }
    //DBGPRINT_VECTOR(components);

    if(components.size() != 2)
        return false;

    m_taskList = components[0];
    m_groupType = components[1];

    return true;
}
bool TaskPath::isValid() const
{
    return m_valid;
}
