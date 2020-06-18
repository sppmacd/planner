#pragma once

#include <map>
#include <string>

#include "ByteStream.h"
#include "GroupType.h"
#include "TaskList.h"

class Planner
{
    public:
        Planner(/*Args&*/);
        virtual ~Planner();

        void init();
        static Planner& the();
        bool running() const { return m_running; }
        void stop();

        std::string getCurrentTaskList() const { return m_currentTaskList; }
        GroupType getCurrentGroupType() const { return m_currentGroupType; }
        bool setCurrentTaskList(std::string);
        bool setCurrentGroupType(GroupType);
        TaskList* getTaskList(std::string = "");

        void initTaskList(std::string name);
        bool removeTaskList(std::string name);
        std::vector<std::string> taskLists();
        GroupType parseGroupName(std::string);

        void clear();
        bool load(std::string fileName); //todo: save previous state and restore on error
        bool save(std::string fileName);
        void setDirty() { m_dirty = true; }
        bool askNotSaved(std::string action);
        bool ask(std::string prompt);
        bool isDirty() { return m_dirty; }
        std::string getCurrentFile();

    private:
        friend ByteStream& operator<<(ByteStream& _str, const Planner& _data);
        friend ByteStream& operator>>(ByteStream& _str, Planner& _data);

        bool m_running;
        std::string m_currentTaskList;
        GroupType m_currentGroupType;
        std::map<std::string, TaskList*> m_taskLists;
        bool m_dirty;
        std::string m_currentFile;
};
