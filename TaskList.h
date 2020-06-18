#pragma once

#include <map>
#include <ostream>

#include "Group.h"
#include "GroupType.h"

class TaskList
{
    public:
        virtual ~TaskList();
        void print(std::ostream& out) const;
        void printGraph(std::ostream& out, bool proportional = false) const;
        void printSummary(std::ostream& out) const;
        Group& addGroup(const GroupType&);
        bool hasGroup(GroupType type);
        std::vector<std::string> groups();
        Group* getGroup(GroupType);
        bool removeGroup(GroupType);

    private:
        friend ByteStream& operator<<(ByteStream& _str, const TaskList& _data);
        friend ByteStream& operator>>(ByteStream& _str, TaskList& _data);

        std::map<GroupType, Group*> m_groups;
};
