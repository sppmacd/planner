#include "TaskList.h"

#include <algorithm>
#include <ostream>
#include <set>
#include <vector>

#include "cli/utils.h"
#include "Group.h"
#include "GroupType.h"
#include "Task.h"
#include "Time.h"

TaskList::~TaskList()
{
    for(auto& group: m_groups)
        delete group.second;
}

void TaskList::print(std::ostream& out) const
{
    if(m_groups.empty())
    {
        out << "Empty" << std::endl;
        return;
    }
    out << "Groups: " << std::endl;
    for(auto& par: m_groups)
    {
        out << "  " << par.first.getName() << std::endl;
        auto tasks = par.second->getAllTasksSorted();
        for(auto& task: tasks)
        {
            out << "    " << task << std::endl;
        }
        out << std::endl;
    }
    out << std::endl;
}

void TaskList::printGraph(std::ostream& out, bool proportional) const
{
    out << std::endl;
    if(m_groups.empty())
    {
        out << U::esc(ESC_ERROR) << "Empty" << std::endl;
        return;
    }
    std::set<Time> usedTimes;
    if(proportional)
        out << U::esc(ESC_BOLD) << " HH:MM:SS - HH:MM:SS | ";
    else
        out << U::esc(ESC_BOLD) << " HH:MM:SS | ";

    Time _min(24,0,0);
    Time _max(0);
    size_t spacing = 0;

    for(auto& par: m_groups)
    {
        std::set<Time> groupUsed = par.second->getUsedTimes();
        usedTimes.insert(groupUsed.begin(), groupUsed.end());

        if(proportional)
        {
            _min = std::min(_min, *std::min_element(groupUsed.begin(), groupUsed.end()));
            _max = std::max(_max, *std::max_element(groupUsed.begin(), groupUsed.end()));
        }

        out << "  " << par.first.getName() << "  | ";
    }
    if(proportional)
    {
        size_t entries = 24;
        for(size_t s = 0; s < entries; s++)
        {
            double fac = double(s) / entries;
            usedTimes.insert(Time((_max.timestamp() - _min.timestamp()) * fac + _min.timestamp()));
        }
    }

    out << U::esc(ESC_RESET) << std::endl;

    Time lastTime;
    for(auto it = usedTimes.begin(); it != usedTimes.end();)
    {
        auto itCurr = it;
        auto itNext = ++it;

        auto time = *itCurr;
        auto timeNext = (itNext == usedTimes.end() ? Time(0) : *itNext);

        if(proportional)
            out << " \033[32m" << time << " - " << timeNext << U::esc(ESC_RESET) << " | ";
        else
            out << " \033[32m" << time << U::esc(ESC_RESET) << " | ";

        for(auto& par: m_groups)
        {
            unsigned taskCount = 0;
            std::map<size_t, Task*> startingTasks;
            for(size_t s = 0; s < LINE_EVERY; s++)
            {
                std::map<size_t, Task*> runningTasks = par.second->getRunningTasksAt(time, s, spacing);
                if(runningTasks.empty())
                {
                    out << " ";
                }
                else if(runningTasks.size() == 1)
                {
                    auto first = runningTasks.begin();
                    out << first->second->getCharacter();
                    if(first->second->getStartTime() == time)
                        startingTasks.insert(*first);
                    taskCount++;
                }
                else
                {
                    ASSERT(false);
                }
            }

            if(taskCount > 1)
                out << "\033[5;91;1m!\033[25;0m";
            else
                out << " ";

            out << "| ";
            for(auto& task: startingTasks)
                out << "\033[33m" << task.second->getName() << "(" << task.first << ")" << ", ";

        }
        out << std::endl;
        lastTime = time;
    }
}

void TaskList::printSummary(std::ostream& out) const
{
    if(m_groups.empty())
    {
        out << "Empty" << std::endl;
        return;
    }
    out << "Groups: " << std::endl;
    for(auto& par: m_groups)
    {
        out << par.first.getName();
        size_t s = 0;
        for(auto& line: *par.second)
        {
            s += line.size();
        }
        out << " (" << s << " tasks" << "), " << std::endl;
    }
    out << std::endl;
}

Group& TaskList::addGroup(const GroupType& groupType)
{
    Group* group = new Group;
    m_groups.insert(std::make_pair(groupType, group));
    return *group;
}

bool TaskList::hasGroup(GroupType type)
{
    return m_groups.find(type) != m_groups.end();
}

std::vector<std::string> TaskList::groups()
{
    std::vector<std::string> list;
    for(auto& str: m_groups)
        list.push_back(str.first.getName());
    return list;
}

Group* TaskList::getGroup(GroupType type)
{
    auto it = m_groups.find(type);
    if(it == m_groups.end())
        return NULL;
    return it->second;
}

bool TaskList::removeGroup(GroupType type)
{
    auto group = m_groups.find(type);
    if(group != m_groups.end())
    {
        delete group->second;
        m_groups.erase(group);
        return true;
    }
    printf("TaskList::removeGroup(): Invalid group specified: %s\n", type.getName().c_str());
    return false;
}

ByteStream& operator<<(ByteStream& _str, const TaskList& _data)
{
    LOG_WRITE(TaskList&, 0);
    _str << _data.m_groups;
    return _str;
}
ByteStream& operator>>(ByteStream& _str, TaskList& _data)
{
    LOG_READ(TaskList&, 0);
    _str >> _data.m_groups;
    return _str;
}
