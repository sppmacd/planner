#include "Group.h"

#include <algorithm>
#include <set>
#include <string>
#include <vector>

#include "Task.h"
#include "Time.h"

Group::~Group()
{
    for(auto& line: m_taskLines)
    {
        for(auto& task: line)
            delete task.second;
    }
}

size_t Group::addTask(std::string name, const Time& start, const Time& finish)
{
    for(size_t s = 0; s < m_taskLines.size(); s++)
    {
        if(isLineFreeAt(s, start, finish))
        {
            auto& line = m_taskLines[s];
            line.insert(std::make_pair(++m_uniqueId, new Task(name, start, finish)));
            return m_uniqueId;
        }
    }
    std::cout << "No free line to add task!" << std::endl;
    return 0;
}

bool Group::removeTask(size_t id)
{
    for(size_t s = 0; s < m_taskLines.size(); s++)
    {
        auto& taskLine = m_taskLines[s];
        auto it = taskLine.find(id);
        if(it == taskLine.end())
        {
            continue;
        }
        delete it->second;
        taskLine.erase(id);
        return true;
    }
    printf("Group::removeTask(): no task found: %zu", id);
    return false;
}

Task* Group::getTask(size_t id)
{
    for(size_t s = 0; s < m_taskLines.size(); s++)
    {
        auto& taskLine = m_taskLines[s];
        auto it = taskLine.find(id);
        if(it == taskLine.end())
        {
            continue;
        }
        return it->second;
    }
    return NULL;
}

std::map<size_t, Task*> Group::getRunningTasksAt(const Time& time, unsigned line, size_t spacing) const
{
    std::map<size_t, Task*> taskList;

    // todo: optimize it (O(x^2)??)
    for(size_t s = 0; s < m_taskLines.size(); s++)
    {
        if(line == LINE_EVERY || line == s)
        {
            for(auto& task: m_taskLines[s])
            {
                if(task.second->isRunningAt(time, spacing))
                    taskList.insert(task);
            }
        }
    }
    return taskList;
}

std::set<Time> Group::getUsedTimes() const
{
    std::set<Time> times;
    for(size_t s = 0; s < m_taskLines.size(); s++)
    {
        for(auto& task: m_taskLines[s])
        {
            times.insert(task.second->getStartTime());
            times.insert(task.second->getFinishTime());
        }
    }
    return times;
}

Group::TaskListUID Group::getAllTasksSorted() const
{
    Group::TaskListUID tasks;
    for(size_t s = 0; s < m_taskLines.size(); s++)
    {
        for(auto& task: m_taskLines[s])
        {
            tasks.push_back(std::make_pair(s, task));
        }
    }
    std::stable_sort(tasks.begin(), tasks.end(), [](auto& _1, auto& _2) {
        return _1.second.second->getStartTime() < _2.second.second->getStartTime();
    });
    return tasks;
}

bool Group::isLineFreeAt(unsigned line, const Time& start, const Time& finish) const
{
    ASSERT(line < LINE_EVERY);
    auto& arr = m_taskLines[line];
    for(auto& task: arr)
    {
        if(task.second->getStartTime() < finish && task.second->getFinishTime() > start)
        {
            return false;
        }
    }
    return true;
}

std::ostream& operator<<(std::ostream& _1, Group::TaskListUID::value_type& _2)
{
    return _1 << "ID " << _2.second.first << ": (" << *_2.second.second << ") at line " << _2.first;
}

ByteStream& operator<<(ByteStream& _str, const Group& _data)
{
    LOG_WRITE(Group&, 0);
    _str << ((uint32_t)_data.m_uniqueId) << _data.m_taskLines;
    return _str;
}
ByteStream& operator>>(ByteStream& _str, Group& _data)
{
    LOG_READ(Group&, 0);
    _str >> ((uint32_t&)_data.m_uniqueId) >> _data.m_taskLines;
    return _str;
}

