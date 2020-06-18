// *** ADDED BY HEADER FIXUP ***
#include <string>
#include <vector>
// *** END ***
#ifndef COMMANDS_H
#define COMMANDS_H

#include "config.h"

// Command prototypes
#define COMMAND_PROTOTYPE(x,y) int c_##x(std::vector<std::string>& args);

#define ENUMERATE_COMMANDS \
    COMMAND_PROTOTYPE(add_group, "Add group to current tasklist") \
    COMMAND_PROTOTYPE(add_task, "Add new task to current group") \
    COMMAND_PROTOTYPE(ch_group, "Change current group") \
    COMMAND_PROTOTYPE(ch_tasklist, "Change current task list") \
    COMMAND_PROTOTYPE(exit, "Exit the console") \
    COMMAND_PROTOTYPE(help, "Print Help message") \
    COMMAND_PROTOTYPE(new_file, "Create the new file") \
    COMMAND_PROTOTYPE(init_tasklist, "Clear the task list") \
    COMMAND_PROTOTYPE(load, "Loads tasks from file") \
    COMMAND_PROTOTYPE(ls_groups, "List groups in current task list") \
    COMMAND_PROTOTYPE(ls_tasklists, "List task lists") \
    COMMAND_PROTOTYPE(ls_tasks, "List tasks in group") \
    COMMAND_PROTOTYPE(print, "Print the current task list") \
    COMMAND_PROTOTYPE(rm_group, "Remove group from tasklist") \
    COMMAND_PROTOTYPE(rm_task, "Remove task from group") \
    COMMAND_PROTOTYPE(rm_tasklist, "Remove tasklist") \
    COMMAND_PROTOTYPE(save, "Saves tasks to file") \
    COMMAND_PROTOTYPE(sh, "Run system shell command")

ENUMERATE_COMMANDS

typedef int(*CommandHandler)(std::vector<std::string>& args);
struct Command {std::string name; std::string description; CommandHandler handler;};
extern Command commands[];

#endif //COMMANDS_H
