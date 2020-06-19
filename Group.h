#pragma once

#include <array>
#include <map>
#include <ostream>
#include <set>
#include <string>
#include <vector>

#include "ByteStream.h"
#include "Task.h"
#include "Time.h"

#define LINE_EVERY 6

class Group
{
    public:
        typedef std::vector<std::pair<unsigned, std::pair<size_t,Task*>>> TaskListUID;

        inline Group(): m_uniqueId(0) {}
        virtual ~Group();
        inline auto begin() const { return m_taskLines.begin(); }
        inline auto end() const { return m_taskLines.end(); }
        size_t addTask(std::string name, const Time& start, const Time& finish);
        bool removeTask(size_t id);
        Task* getTask(size_t id);
        std::map<size_t, Task*> getRunningTasksAt(const Time&, unsigned line = LINE_EVERY, size_t spacing = 0) const;
        std::set<Time> getUsedTimes() const;
        TaskListUID getAllTasksSorted() const;

    private:
        friend ByteStream& operator<<(ByteStream& _str, const Group& _data);
        friend ByteStream& operator>>(ByteStream& _str, Group& _data);

        bool isLineFreeAt(unsigned line, const Time& start, const Time& finish) const;

        std::array<std::map<size_t, Task*>, LINE_EVERY> m_taskLines;
        size_t m_uniqueId;
};

std::ostream& operator<<(std::ostream& _1, Group::TaskListUID::value_type& _2);
