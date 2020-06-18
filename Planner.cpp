#include "Planner.h"

#include <ByteStream.h>
#include <cli/commands.h>
#include <cli/utils.h>
#include <readline/readline.h>
#include <unistd.h>

Planner::Planner(/*Args&*/)
    : m_running(false)
    , m_dirty(false)
{
}

Planner::~Planner()
{
    clear();
}

void Planner::init()
{
    if(!m_running)
    {
        // first init
        m_running = true;
    }
    else
    {
        // user-triggered inits
        if(!askNotSaved("init"))
            return;
    }

    clear();
    std::vector<std::string> args = {"init_tasklist", "default"};
    c_init_tasklist(args);
    m_currentFile = "";
    m_dirty = false;
}

Planner& Planner::the()
{
    static Planner planner;
    return planner;
}

void Planner::stop()
{
    if(!askNotSaved("stop"))
        return;
    m_running = false;
}

bool Planner::setCurrentTaskList(std::string list)
{
    if(m_taskLists.find(list) == m_taskLists.end())
        return false;
    m_currentTaskList = list;
    return true;
}

bool Planner::setCurrentGroupType(GroupType type)
{
    TaskList* list = m_taskLists[m_currentTaskList];
    if(!list->hasGroup(type))
        return false;
    m_currentGroupType = type;
    return true;
}

void Planner::initTaskList(std::string name)
{
    TaskList*& _list = m_taskLists[name];
    if(_list)
        delete _list;
    _list = new TaskList;
}

bool Planner::removeTaskList(std::string name)
{
    auto _list = m_taskLists.find(name);
    if(_list != m_taskLists.end())
    {
        delete _list->second;
        m_taskLists.erase(_list);

        if(name == m_currentTaskList)
            m_currentTaskList = "";

        return true;
    }
    printf("Planner::removeTaskList(): Invalid tasklist specified: %s\n", name.c_str());
    return false;
}

void Planner::clear()
{
    for(auto& str: m_taskLists)
        delete str.second;
    m_taskLists.clear();
}

TaskList* Planner::getTaskList(std::string list)
{
    //printf("'%s'\n", list.c_str());
    if(list.empty())
        list = m_currentTaskList;
    auto it = m_taskLists.find(list);
    if(it == m_taskLists.end())
        return NULL;
    return it->second;
}

std::vector<std::string> Planner::taskLists()
{
    std::vector<std::string> list;
    for(auto& str: m_taskLists)
        list.push_back(str.first);
    return list;
}

GroupType Planner::parseGroupName(std::string name)
{
    if(name == "." || name.empty())
        return getCurrentGroupType();
    return name;
}

bool Planner::load(std::string fileName)
{
    if(!askNotSaved("load"))
        return true;

    ByteStream stream;
    stream.open(fileName, std::ios::in);
    if(stream.fail())
    {
        printf("Planner::load(): Invalid file name!\n");
        return true;
    }

    clear();
    stream >> *this;
    if(stream.fail())
    {
        std::cout << U::esc(ESC_ERROR) << "Failed to load file (invalid format)" << std::endl;
        return false;
    }
    m_dirty = false;
    m_currentFile = fileName;
    return true;
}

bool Planner::save(std::string fileName)
{
    if(fileName != m_currentFile && access(fileName.c_str(), F_OK) != -1)
    {
        if(!ask("Overwrite file '" + fileName + "'"))
            return true;
    }

    ByteStream stream;
    stream.open(fileName, std::ios::out | std::ios::trunc);
    if(stream.fail())
    {
        printf("Planner::save(): Invalid file name!\n");
        return false;
    }
    stream << *this;

    if(stream.fail())
        return false;

    m_dirty = false;
    m_currentFile = fileName;
    return true;
}

bool Planner::askNotSaved(std::string action)
{
    if(m_dirty)
    {
        return ask("You are trying to do'" + action + "'\n" + "\033[91mYou have unsaved changes. Continue anyway?\033[0m");
    }
    return true;
}

bool Planner::ask(std::string prompt)
{
    char* c = readline((prompt + "\n(y/n, default n) ").c_str());
    if(c && c[0] == 'y')
        return true;
    return false;
}

std::string Planner::getCurrentFile()
{
    return m_currentFile;
}

ByteStream& operator<<(ByteStream& _str, const Planner& _data)
{
    LOG_WRITE(Planner&, 0);
    _str << _data.m_currentTaskList << _data.m_currentGroupType << _data.m_taskLists;
    return _str;
}
ByteStream& operator>>(ByteStream& _str, Planner& _data)
{
    LOG_READ(Planner&, 0);
    _str >> _data.m_currentTaskList >> _data.m_currentGroupType >> _data.m_taskLists;
    return _str;
}
