#include "utils.h"

namespace U
{
    std::string concatenateArgs(std::vector<std::string>& args, int start)
    {
        std::string str;
        for(size_t i = start; i < args.size(); i++)
        {
            std::string arg = args[i];
            bool space = arg.find(" ") != std::string::npos;
            if(space) str += "\"";
            str += arg;
            if(space) str += "\"";
            if(i != args.size() - 1)
                str += " ";
        }
        return str;
    }
    std::string escapeCode(int n)
    {
        switch(n)
        {
            case ESC_RESET: return "\033[0m";
            case ESC_BOLD: return "\033[1m";
            case ESC_ERROR: return "\033[38;5;9m";
            default: return "";
        }
    }
}
