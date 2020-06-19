#include "commands.h"

#include <unistd.h>

#include <Planner.h>
#include "utils.h"

// Commands to be used in HELP
#undef COMMAND_PROTOTYPE
#define COMMAND_PROTOTYPE(x,y) {#x,y,c_##x},

Command commands[] =
{
    ENUMERATE_COMMANDS
    {"", "", NULL}
};

// Command definitions
#undef COMMAND_PROTOTYPE
#define COMMAND_PROTOTYPE(x) int c_##x(std::vector<std::string>& args)

/////////////////
// DEFINITIONS //
/////////////////

COMMAND_PROTOTYPE(add_group)
{
    if(args.size() != 2)
    {
        printf("Usage: add_group <group name>");
        return 1;
    }
    std::string name = args[1];
    TaskList* list = Planner::the().getTaskList();
    if(!list)
    {
        printf("No current tasklist set! Use 'ch_tasklist'\n");
        return 1;
    }
    list->addGroup(GroupType(name));
    Planner::the().setDirty();
    return 0;
}

COMMAND_PROTOTYPE(add_task)
{
    if(args.size() != 5)
    {
        printf("Usage: add_task <group> <name> <startTime> <endTime>\n");
        return 1;
    }
    GroupType groupName = Planner::the().parseGroupName(args[1]);
    std::string name = args[2];
    Time start = args[3];
    if(!start.isValid())
    {
        printf("start: Invalid format\n");
    }
    Time finish = args[4];
    if(!finish.isValid())
    {
        printf("finish: Invalid format\n");
    }

    TaskList* list = Planner::the().getTaskList();
    if(!list)
    {
        printf("No current tasklist set! Use 'ch_tasklist'\n");
        return 1;
    }
    Group* group = list->getGroup(groupName);
    if(!group)
    {
        printf("Invalid group: %s", groupName.getName().c_str());
        return 1;
    }
    group->addTask(name, start, finish);
    Planner::the().setDirty();
    return 0;
}

COMMAND_PROTOTYPE(ch_group)
{
    if(args.size() == 1)
        printf("%s\n", Planner::the().getCurrentGroupType().getName().c_str());
    else
    {
        std::string newGroup = U::concatenateArgs(args, 1);

        if(Planner::the().getCurrentGroupType().getName() == newGroup)
            return 0;

        bool rc = Planner::the().setCurrentGroupType(newGroup);
        if(!rc)
        {
            printf("Group is not valid!\n");
            return 1;
        }
    }
    Planner::the().setDirty();
    return 0;
}

COMMAND_PROTOTYPE(ch_tasklist)
{
    if(args.size() == 1)
        printf("%s\n", Planner::the().getCurrentTaskList().c_str());
    else
    {
        std::string newTaskList = U::concatenateArgs(args, 1);

        if(Planner::the().getCurrentTaskList() == newTaskList)
            return 0;

        bool rc = Planner::the().setCurrentTaskList(U::concatenateArgs(args, 1));
        if(!rc)
        {
            printf("Tasklist is not valid!\n");
            return 1;
        }
    }
    Planner::the().setDirty();
    return 0;
}

COMMAND_PROTOTYPE(edit_task)
{
    if(args.size() < 3)
    {
        printf("Usage: edit_task <group> <task ID> [(name <name>|start <start time>|finish <finish time>)...]\n");
        return 1;
    }

    // identifying data
    GroupType groupType = Planner::the().parseGroupName(args[1]);
    size_t taskId = 0;
    try
    {
        taskId = stol(args[2]);
    }
    catch(...)
    {
        printf("Invalid task ID (is not a number)\n");
        return 1;
    }
    TaskList* taskList = Planner::the().getTaskList();
    if(!taskList)
    {
        printf("No current tasklist set! Use 'ch_tasklist'.\n");
        return 1;
    }
    Group* group = taskList->getGroup(groupType);
    if(!group)
    {
        printf("Invalid group specified!\n");
        printf("Hint: Use '.' to refer to current group.\n");
        return 1;
    }
    Task* task = group->getTask(taskId);
    if(!task)
    {
        printf("Invalid task ID!\n");
        printf("Hint: Use 'print' to get list of tasks.\n");
        return 1;
    }

    // load values to change
    if((args.size() - 3) % 2 != 0)
    {
        printf("Error: Invalid argument count! (was %zu)\n", (args.size() - 3));
        printf("Hint: Use <name> <value> syntax, e.g 'edit_task . 0 name Test start 0:0:0'\n");
        return 1;
    }
    std::string argName, argValue;
    size_t changes = 0;
    for(size_t s = 3; s < args.size(); s++)
    {
        if(argName.empty())
        {
            argName = args[s];
            argValue.clear();
        }
        else
        {
            argValue = args[s];
            if(argName == "name")
                task->setName(argValue);
            else if(argName == "start")
                task->setStartTime(argValue);
            else if(argName == "finish")
                task->setFinishTime(argValue);
            else
            {
                printf("Error: Invalid argument name: %s\n", argName.c_str());
                printf("Hint: Valid argument names: 'name', 'start' and 'finish'\n");
                continue;
            }
            changes++;
            argName.clear();
        }
    }
    if(changes > 0)
    {
        Planner::the().setDirty();
        return 0;
    }
    printf("Nothing changed\n");
    return 0;
}

COMMAND_PROTOTYPE(exit)
{
    Planner::the().stop();
    int rv = 0;
    if(args.size() > 1)
    {
        try
        {
            rv = stoi(args[1]);
        }
        catch(...) {} //nothing
    }
    return rv;
}

COMMAND_PROTOTYPE(help)
{
    printf("\n\033[1;38;5;241mAvailable commands:\n\n");
    for(Command& cmd: commands)
    {
        if(!cmd.name.empty())
            printf("  \033[0;1m%-16s\033[0;37m%-32s\n", cmd.name.c_str(), cmd.description.c_str());
    }
    return 0;
}

COMMAND_PROTOTYPE(new_file)
{
    Planner::the().init();
    return 0;
}

COMMAND_PROTOTYPE(init_tasklist)
{
    if(args.size() != 2)
    {
        printf("Usage: init_tasklist <name>\n");
        return 1;
    }
    Planner::the().initTaskList(args[1]);
    c_ch_tasklist(args);
    Planner::the().setDirty();
    return 0;
}

COMMAND_PROTOTYPE(load)
{
    if(args.size() == 1)
    {
        printf("Hint: use 'load <file name>' to load other file.\n");
        if(Planner::the().getCurrentFile().empty())
        {
            printf("Error: file name not specified when loading first one\n");
            return false;
        }
        return !Planner::the().load(Planner::the().getCurrentFile());
    }
    else if(args.size() == 2)
    {
        return !Planner::the().load(args[1]);
    }
    else
    {
        printf("Usage: load <file name>\n");
        return 1;
    }
}

COMMAND_PROTOTYPE(ls_groups)
{
    TaskList* current = Planner::the().getTaskList(U::concatenateArgs(args, 1));
    if(current)
    {
        DBGPRINT_VECTOR(current->groups());
        return 0;
    }
    printf("Invalid task list!\n");
    return 1;
}

COMMAND_PROTOTYPE(ls_tasklists)
{
    DBGPRINT_VECTOR(Planner::the().taskLists());
    return 0;
}

COMMAND_PROTOTYPE(ls_tasks)
{
    TaskList* current = Planner::the().getTaskList();
    if(current)
    {
        GroupType groupType = U::concatenateArgs(args, 1);
        if(groupType.getName() == "")
            groupType = Planner::the().getCurrentGroupType();
        Group* group = current->getGroup(groupType);

        if(!group)
        {
            printf("Invalid group!\n");
            return 1;
        }
        DBGPRINT_VECTOR(group->getAllTasksSorted());
    }
    return 0;
}

COMMAND_PROTOTYPE(print)
{
    TaskList* current = Planner::the().getTaskList();
    if(current)
    {
        if(args.size() == 1)
        {
            current->print(std::cout);
            printf("Hint: Use 'print [mode: tasklist|graph|proportional|summary|conflicts]'\n");
        }
        else if(args.size() == 2)
        {
            std::string mode = args[1];
            if(mode == "tasklist")
                current->print(std::cout);
            else if(mode == "graph")
                current->printGraph(std::cout);
            else if(mode == "proportional")
                current->printGraph(std::cout, true);
            else if(mode == "summary")
                current->printSummary(std::cout);
            else if(mode == "conflicts")
                printf("not implemented...\n");
            else
            {
                printf("'mode' must be one of 'tasklist', 'graph', 'proportional', 'summary' or 'conflicts'\n");
                return 1;
            }
        }
        else
        {
            printf("Usage: print [mode: tasklist|graph|proportional|summary]\n");
            return 1;
        }
    }
    else
    {
        printf("No current tasklist set! Use 'ch_tasklist'.\n");
        return 1;
    }
    return 0;
}

COMMAND_PROTOTYPE(rm_group)
{
    if(args.size() != 2)
    {
        printf("Usage: rm_group <group name>\n");
        return 1;
    }

    TaskList* tasklist = Planner::the().getTaskList();
    if(!tasklist)
    {
        printf("No current tasklist set! Use 'ch_tasklist'\n");
        return 1;
    }
    GroupType groupName = Planner::the().parseGroupName(args[1]);
    if(!tasklist->removeGroup(groupName))
    {
        return 1;
    }
    Planner::the().setDirty();

    return 0;
}

COMMAND_PROTOTYPE(rm_task)
{
    if(args.size() != 3)
    {
        printf("Usage: rm_task <group> <task id>\n");
        return 1;
    }
    GroupType groupName;
    size_t taskId = 0;
    try
    {
        groupName = Planner::the().parseGroupName(args[1]);
        taskId = std::stoi(args[2]);
    }
    catch(...)
    {
        printf("<task id> is not a number\n");
        return 1;
    }

    TaskList* tasklist = Planner::the().getTaskList();
    if(!tasklist)
    {
        printf("No current tasklist set! Use 'ch_tasklist'\n");
        return 1;
    }
    Group* group = tasklist->getGroup(groupName);
    if(!group)
    {
        printf("Invalid group specified!\n");
        return 1;
    }
    if(!group->removeTask(taskId))
    {
        return 1;
    }
    Planner::the().setDirty();
    return 0;
}

COMMAND_PROTOTYPE(rm_tasklist)
{
    if(args.size() != 2)
    {
        printf("Usage: rm_tasklist <tasklist name>\n");
        return 1;
    }
    if(!Planner::the().removeTaskList(U::concatenateArgs(args, 1)))
    {
        return 1;
    }

    Planner::the().setDirty();
    return 0;
}

COMMAND_PROTOTYPE(save)
{
    if(args.size() == 1)
    {
        printf("Hint: use 'save <file name>' to save as other file name.\n");
        if(Planner::the().getCurrentFile().empty())
        {
            printf("Error: file name not specified when saving first one\n");
            return false;
        }
        return !Planner::the().save(Planner::the().getCurrentFile());
    }
    else if(args.size() == 2)
    {
        return !Planner::the().save(args[1]);
    }
    else
    {
        printf("Usage: save <file name>\n");
        return 1;
    }
}

COMMAND_PROTOTYPE(sh)
{
    return system(U::concatenateArgs(args, 1).c_str());
}
