#include <readline/readline.h>
#include <readline/history.h>

#include <cli/commands.h>
#include <cli/utils.h>
#include <Planner.h>

using namespace std;

// Command check
int command(std::vector<std::string>& args)
{
    std::string cmd = args[0];
    #undef COMMAND_PROTOTYPE
    #define COMMAND_PROTOTYPE(x,y) else if(cmd == #x) {return c_##x(args);}
    if(0)
    {

    }
    ENUMERATE_COMMANDS
    else
    {
        std::cout << U::esc(ESC_ERROR);
        printf("%s: invalid command\n", cmd.c_str());
    }
    return -1;
}

int main(int argc, const char* argv[])
{
    // todo: catch ^C signal
    Planner::the().init();

    printf("\n"
           "\033[0;92mBusiness Planner CLI v1.0 :: Sppmacd (c) 2020\n"
           "\033[0;92mType 'help' for help, 'exit' to close\n"
           "\033[31;0m\n"
           );
    while(Planner::the().running())
    {
        // get entry from std in
        std::vector<std::string> args;
        char prompt[256];
        snprintf(prompt, 256, "%s\033[1;34m%s$%s(%s) \033[0m> \033[97m",
                 (Planner::the().isDirty() ? "\033[5;91m*\033[25;0m" : ""),
                 Planner::the().getCurrentFile().c_str(),
                 Planner::the().getCurrentTaskList().c_str(),
                 Planner::the().getCurrentGroupType().getName().c_str());

        std::string s;
        char* input = readline(prompt);
        if(!input)
            continue;
        add_history(input);
        s = input;
        free(input);

        // parse entry
        std::istringstream iss;
        iss.str(s);
        bool quot = false;
        std::string arg;
        char lastC = 0;
        while(!iss.eof())
        {
            int c = iss.get();
            if(c == '"' && lastC != '\\')
            {
                quot = !quot;
            }
            else if(c == '\\' && iss.peek() == '"')
            {
                // nothing
            }
            else if((isspace(c) && !quot) || c == -1)
            {
                if(!isspace(iss.peek()))
                {
                    args.push_back(arg);
                    arg.clear();
                }
            }
            else
            {
                arg += c;
            }
            lastC = c;
        }

        if(!args.empty() && !s.empty())
        {
            std::cout << U::esc(ESC_RESET);
            int i = command(args);
            //std::cout << "\n>> " << i << " <<";
            if(!Planner::the().running())
                return i;
        }
        printf("\n");
    }
    return 0;
}
