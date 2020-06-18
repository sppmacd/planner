#pragma once

#include <string>

#include "GroupType.h"

class TaskPath
{
    public:
        TaskPath(std::string pList, GroupType pGroup);
        TaskPath(std::string path);
        std::string toString() const;
        bool parse(std::string path);
        bool isValid() const;
        std::string getTaskList() { return m_taskList; }
        GroupType getGroupType() { return m_groupType; }
    private:
        std::string m_taskList;
        GroupType m_groupType;
        bool m_valid;
};
